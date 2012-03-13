-------------------------------------------------------------------------------
--
-- File: usb_core.vhd
-- Author: Wade S. Fife
-- Date: January 11, 2006
--
--                     Copyright (c) 2006 Wade Fife
--
-- DESCRIPTION
--
-- NOTE: The terms IN and OUT are from the host's (the PC's) perspective.
-- Therefore IN refers to transfer from the peripheral (e.g., this core) to the
-- PC and OUT refers to transfer from the PC to this peripheral.
--
-- This module serves as the link between the Cypress USB chip and the FPGA. It
-- provides arbitration between reading from the Cypress USB chip and writing
-- to the Cypress USB chip.
--
-- The complexity of the arbitration logic can be configured using the
-- C_MAX_TRANSFER generic. In the event that there's data to send and data to
-- be receive at the same time, the core will transfer up to C_MAX_TRANSFER
-- then switch to the opposite transfer direction. This optimizes throughput
-- without starving sending or receiving for long periods of time. In order for
-- this to work, the USB chip's FIFO flags (OUT empty and IN full) must be in
-- fixed mode and always active. To disable this level of arbitration, set
-- C_MAX_TRANSFER to 0. If disabled, sending of data initially has priority
-- over receiving data. Once a transfer begins, data will continue to be
-- exclusively sent or received as long as it is available. This can
-- potentially lead to buffer overflows due to starvation. If C_MAX_TRANSFER is
-- set to 1, it allows a send or receive to be interrupted when new data is
-- ready to be received or sent. For best performance, C_MAX_TRANSFER should be
-- set to a larger power of 2 (e.g., 16 or 32).
--
-- The "clk" input should be connected to the main system clock. The "if_clk"
-- input to this module should be the same as (or a lightly skewed version of)
-- the clock connected to IFCLK on the USB chip. This clock can be driven by
-- the USB chip or by a clock generator on the FPGA in the range of 5 to 48
-- MHz. The input if_clk is also used to control some of the logic in this
-- module. Internal FIFOs are used to buffer data, as well as cross the two
-- clock domains.
--
-- In order for this core to work, the firmware can be configured in a variety
-- of ways. An example configuration is shown below:
--
--     - For 16-bit FIFO, AUTOOUT and AUTOIN enabled
--         - EP2FIFOCFG = 0x11 (00010001b)
--         - EP2FIFOCFG.4 = 1 (Auto out enabled)
--         - EP2FIFOCFG.0 = 1 (OUT FIFO in 16-bit mode)
--         - EP6FIFOCFG = 0x09 (00001001b)
--         - EP6FIFOCFG.3 = 1 (Auto in enabled)
--         - EP6FIFOCFG.0 = 1 (IN FIFO in 16-bit mode)
--     - For Synchronous slave FIFO mode with IFCLK driven by USB
--         - IFCONFIG = 0xE3 (10100011b)
--         - IFCONFIG.7 = 1 (use internal clock as IFCLK src)
--         - IFCONFIG.6 = 1 (48 MHz), 0 (30 MHz)
--         - IFCONFIG.5 = 1 (enable IFCLK output)
--         - IFCONFIG.4 = 0 (normal, non-inverted clock)
--         - IFCONFIG.3 = 0 (synchronous mode)
--         - IFCONFIG.2 = 0 (disable diagnostic bits)
--         - IFCONFIG.1,0 = 1,1 (use slave FIFO interface)
--     - For EP2 configured as OUT (quad-buffered recommended)
--         - EP2CFG = 0xA0 (10100?00b)
--         - EP2CFG.7 = 1 (active)
--         - EP2CFG.6 = 0 (OUT)
--         - EP2CFG.5,4 = 1,0 (bulk)
--         - EP2CFG.3 = 0 (512 byte)
--         - EP2CFG.2 = ?
--         - WP1XDF.1,0 = 0,0 (quad buffered)
--     - For EP6 configured as IN (quad-buffered recommended)
--         - EP6CFG = 0xE0 (11100000b)
--         - EP6CFG.7 = 1 (active)
--         - EP6CFG.6 = 1 (IN)
--         - EP6CFG.5,4 = 1,0 (bulk)
--         - EP6CFG.3 = 0 (512 byte)
--         - EP6CFG.2 = ? (read only bit ??)
--         - EP6CFG.1,0 = 0,0 (quad buffered)
--     - For EP4 and EP8 disabled
--         - EP4CFG = 0
--         - EP6CFG = 0
--     - For fixed mode FLAGB IN (EP6) full flag and FLABC OUT (EP2) empty flag
--         - PINFLAGSAB = 0xE0
--         - PINFLAGSCD = 0x08
--
-- NOTE: FIFOPINPOLAR defaults to low asserted inputs.
-- NOTE: The TRM mentions EPxFLAGS as affection FIFO mode but no
--       documentation on this register could be found.
-- NOTE: EPxFIFOIE defaults to FIFO interrupts disabled.
-- NOTE: Default PORTACFG should be sufficient.
-- NOTE: EPxFIFOPFH:L assumes default values. Not sure what defaults do.
-- NOTE: EPxAUTOINLENH:L defaults to 512 byte packets
-- NOTE: Althrough 1024 byte endpoints are supported, maximum size for
--       bulk transfers is 512 bytes in high-speed mode. 64 bytes in full-speed
--       mode.
--
-- NOTES
--
-- Clocking:
--
-- The if_clk input can either be driving by the USB chip or by an alternate
-- clock source from 5 to 48 MHz. The USB firmware must be programmed
-- accordingly. In practice it has been found that some skew between the if_clk
-- in this module and the if_clk used by the USB chip may be necessary. For
-- example, a DCM with "C_CLKOUT_PHASE_SHIFT = FIXED" and "C_PHASE_SHIFT = -20"
-- feeding this module has helped on the Helios board. Other phase shifts may
-- be needed under various circumstances.
--
-- Setup and Hold Times:
--
-- Even with clock phase shift, the setup times for slrd_n and slwr_n may not
-- always be met. However, they're generally only not met when either the USB
-- chip's input FIFO is empty or its output FIFO is full. In these cases, an
-- accidental read or write cycle would be ignored by the USB chip due to it's
-- FIFO state.
--
-- Reset:
--
-- Ensuring proper reset is a tricky issue. It is complicated by the fact that
-- the USB may chip may not be powered when it is not connected and by the fact
-- that a DCM (possibly driven by the USB chip's IFCLK signal) may be used to
-- drive the if_clk signal. The DCM will not function properly unless it is
-- reset after its input clock becomes stable. This core contains a simple
-- reset circuit that runs when then usb_alive signal goes high. This causes
-- the if_clk domain to be reset (by asserting if_reset) and asserts the
-- dcm_reset signal so that a DCM, if used to drive if_clk, will also be
-- reset. The if_reset signal will not come out of reset until the DCM has
-- locked, indicated by the dcm_locked signal. Therefore, if not used, the
-- dcm_locked signal should be tied to '1'. The dcm_reset and dcm_locked signal
-- only need to be used if the DCM's clock input is driving by the USB chip.
--
-- Committing Packets:
--
-- By default, the Cypress USB chip will not commit a packet for transfer until
-- its defined maximum packet size has been reached. However, it may be
-- necessary at times to commit a smaller packet. This can be done by asserting
-- the "commit" input and the "send_wr_en" input high for one cycle. This will
-- insert a commit signal into the FIFO so that the PKTEND signal on the USB
-- chip will be asserted as soon as all words ahead of the commit are written
-- to the USB device. You should NOT write data to send_din on the same cycle
-- that commit is asserted since the data will be ignored.
--
-- Endianness:
--
-- You can use the C_BIG_ENDIAN generic to define whether the 16-bit interface
-- should be treated in a big-endian manner or not. When C_BIG_ENDIAN is TRUE,
-- the most significant byte of send_din will be sent first and it should be
-- assumed that the most significant byte of recv_dout was the first byte
-- sent. If C_BIG_ENDIAN is false then the least significant byte is the first
-- byte sent.
--
-- The FIFO interface on the Cypress USB chip is little endian in nature.
-- Setting C_BIG_ENDIAN to true simply swaps the upper and lower bytes of the
-- fd port.
--
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity usb_core is
  generic (
    C_IN_FIFOADDR  : std_logic_vector(1 downto 0) := "10";
    C_OUT_FIFOADDR : std_logic_vector(1 downto 0) := "00";
    C_BIG_ENDIAN   : boolean := TRUE;
    C_DATA_WIDTH   : integer := 16;     -- Bit width of USB interface, 8 or 16
    C_MAX_TRANSFER : integer := 16;     -- Ideally a power of 2
    C_RST_HIGH     : boolean := FALSE);
  port (
    clk      : in  std_logic;
    rst      : in  std_logic;

    -- Optional DCM signals
    dcm_locked  : in std_logic;       -- Set to '1' if dcm isn't used.
    dcm_reset   : out std_logic;      -- Connect to DCM reset, if used.

    -- Receive FIFO
    recv_empty  : out std_logic;      -- Receive FIFO empty
    recv_rd_en  : in std_logic;       -- Receive FIFO read enable
    recv_dout   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);

    -- Send FIFO
    send_full   : out std_logic;      -- Send FIFO full
    send_empty  : out std_logic;      -- Send FIFO empty
    send_wr_en  : in std_logic;       -- Send FIFO write enable
    send_din    : in std_logic_vector(C_DATA_WIDTH-1 downto 0);

    -- Packet Control
    commit      : in std_logic;       -- Commit the queued data

    -- USB Status
    usb_ready   : out std_logic;      -- Indicates USB is present and ready

    -- USB Signals
    if_clk      : in  std_logic;
    usb_full_n  : in  std_logic;      -- IN FIFO full flag
    usb_empty_n : in  std_logic;      -- OUT FIFO empty flag
    usb_alive   : in  std_logic;      -- USB chip GPIO, high when device active

    sloe_n   : out std_logic;         -- USB FIFO slave operate enable
    slrd_n   : out std_logic;         -- USB FIFO slave read enable
    slwr_n   : out std_logic;         -- USB FIFO slave write enable

    pktend_n : out std_logic;                        -- USB FIFO packet end
    fifoaddr : out std_logic_vector(1 downto 0);     -- USB FIFO address
    fd_I     : in  std_logic_vector(C_DATA_WIDTH-1 downto 0); -- USB FIFO data
    fd_O     : out std_logic_vector(C_DATA_WIDTH-1 downto 0); -- USB FIFO data
    fd_T     : out std_logic_vector(C_DATA_WIDTH-1 downto 0)  -- USB FIFO data
  );

end usb_core;



architecture imp of usb_core is

  -- Tristate control constants
  constant EN_IN  : std_logic_vector(C_DATA_WIDTH-1 downto 0)
    := (others => '1');
  constant EN_OUT : std_logic_vector(C_DATA_WIDTH-1 downto 0)
    := (others => '0');

  component usb_fifo_17
    port (
      din    : IN  std_logic_VECTOR(16 downto 0);
      rd_clk : IN  std_logic;
      rd_en  : IN  std_logic;
      rst    : IN  std_logic;
      wr_clk : IN  std_logic;
      wr_en  : IN  std_logic;
      dout   : OUT std_logic_VECTOR(16 downto 0);
      empty  : OUT std_logic;
      full   : OUT std_logic);
  end component;

  component usb_fifo_16
    port (
      din    : IN  std_logic_VECTOR(15 downto 0);
      rd_clk : IN  std_logic;
      rd_en  : IN  std_logic;
      rst    : IN  std_logic;
      wr_clk : IN  std_logic;
      wr_en  : IN  std_logic;
      dout   : OUT std_logic_VECTOR(15 downto 0);
      empty  : OUT std_logic;
      full   : OUT std_logic);
  end component;

  component usb_fifo_9
    port (
      din    : IN  std_logic_VECTOR(8 downto 0);
      rd_clk : IN  std_logic;
      rd_en  : IN  std_logic;
      rst    : IN  std_logic;
      wr_clk : IN  std_logic;
      wr_en  : IN  std_logic;
      dout   : OUT std_logic_VECTOR(8 downto 0);
      empty  : OUT std_logic;
      full   : OUT std_logic);
  end component;

  component usb_fifo_8
    port (
      din    : IN  std_logic_VECTOR(7 downto 0);
      rd_clk : IN  std_logic;
      rd_en  : IN  std_logic;
      rst    : IN  std_logic;
      wr_clk : IN  std_logic;
      wr_en  : IN  std_logic;
      dout   : OUT std_logic_VECTOR(7 downto 0);
      empty  : OUT std_logic;
      full   : OUT std_logic);
  end component;

  -- State machine type
  type state_type is ( IDLE_STATE,
                       READ_STATE,
                       READ_WRITE_TRANS,
                       WRITE_STATE);
  signal current_state : state_type;
  signal next_state : state_type;

--  attribute fsm_encoding : string; 
--  attribute fsm_encoding of current_state : signal is "gray"; 

  -- Active high reset for "clk" domain
  signal reset : std_logic;

  -- FIFO Signals
  signal recv_full    : std_logic;
  signal recv_wr_en   : std_logic;
  signal recv_din     : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal send_empty_i : std_logic;
  signal send_rd_en   : std_logic;
  signal send_dout    : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal send_din_i   : std_logic_vector(C_DATA_WIDTH downto 0);
  signal send_dout_i  : std_logic_vector(C_DATA_WIDTH downto 0);

  -- Endian correct version of send_dout
  signal send_data    : std_logic_vector(C_DATA_WIDTH-1 downto 0);

  -- Transfer counter for send/receive arbitration
  signal count_reg   : integer range 0 to C_MAX_TRANSFER-1;
  signal count_clear : std_logic;
  signal count_max   : std_logic;

  -- Ready signals for transfers
  signal recv_ready, send_ready : std_logic;

  -- Signals for pktend_n control
  signal commit_del : std_logic;        -- Signal on output of send FIFO

  -- IF_CLK domain reset signals
  signal if_reset : std_logic;
  signal reset_state : std_logic;

begin

  -- Assign reset based on active-high/low generic
  reset <= rst when C_RST_HIGH else (not rst);

  -- Route USB data to recv FIFO, send FIFO to send_data.
  -- Apply endianness.
  USB_WIDTH_16: if C_DATA_WIDTH = 16 generate
    recv_din <= (fd_I(7 downto 0) & fd_I(15 downto 8)) when C_BIG_ENDIAN
                else fd_I;
    send_data <= (send_dout(7 downto 0) & send_dout(15 downto 8))
                 when C_BIG_ENDIAN
                 else send_dout;
  end generate USB_WIDTH_16;
  USB_WIDTH_8: if C_DATA_WIDTH /= 16 generate
    recv_din <= fd_I;
    send_data <= send_dout;
  end generate USB_WIDTH_8;

  -- Determine if state is such that we can send or receive USB data
  recv_ready <= usb_empty_n and (not recv_full);
  send_ready <= usb_full_n and (not send_empty_i);

  -- Route send emtpy to IP also
  send_empty <= send_empty_i;

  
  
  -----------------------------------------------------------------------------
  -- IF_CLK DOMAIN ------------------------------------------------------------
  -----------------------------------------------------------------------------

  
  -- Process for send/receive arbitration control
  USE_ARBITRATION: if C_MAX_TRANSFER > 0 generate
    TRANSFER_COUNT : process (if_clk, if_reset, count_reg, count_max,
                              count_clear)
    begin
      if count_reg = C_MAX_TRANSFER-1 then
        count_max <= '1';
      else
        count_max <= '0';
      end if;
      
      if if_reset = '1' then
        count_reg <= 0;
      elsif if_clk'event and if_clk = '1' then
        if count_clear = '1' then
          count_reg <= 0;
        elsif count_max = '0' then
          count_reg <= count_reg + 1;
        end if;
      end if;
    end process;
  end generate USE_ARBITRATION;

  
  -- Registers for state machine
  FSM_REG : process (if_clk, if_reset, next_state)
  begin
    if if_reset = '1' then
      current_state <= IDLE_STATE;
    elsif if_clk'event and if_clk='1' then
      current_state <= next_state;
    end if;
  end process;

  
  -- Combinational logic for state machine
  FSM_COMB : process (current_state, send_ready, recv_ready, send_data,
                      count_max, usb_alive, commit_del)
  begin
    -- Default values
    fifoaddr <= C_OUT_FIFOADDR;
    sloe_n <= '1';
    slrd_n <= '1';
    slwr_n <= '1';
    fd_T <= EN_IN;
    fd_O <= send_data;
    send_rd_en <= '0';
    recv_wr_en <= '0';
    count_clear <= '0';
    pktend_n <= '1';
    next_state <= current_state;
    
    -- State logic
    if usb_alive = '0' then
      next_state <= IDLE_STATE;
    else
      case current_state is
        when IDLE_STATE =>
          fifoaddr <= C_OUT_FIFOADDR;
          sloe_n <= '0';
          -- Wait until data is available to be written or read
          if send_ready = '1' then
            count_clear <= '1';
            -- next_state <= WRITE_STATE;
            next_state <= READ_WRITE_TRANS;
          elsif recv_ready = '1' then
            count_clear <= '1';
            next_state <= READ_STATE;
          end if;

        when READ_STATE =>
          fifoaddr <= C_OUT_FIFOADDR;
          if recv_ready = '1' then
            -- Read data from USB chip
            sloe_n <= '0';
            slrd_n <= '0';
            recv_wr_en <= '1';

            if C_MAX_TRANSFER > 0 then
              -- Check if we should switch to sending
              if count_max = '1' and send_ready = '1' then
                count_clear <= '1';
                next_state <= READ_WRITE_TRANS;
              end if;
            end if;
          else
            next_state <= IDLE_STATE;
          end if;

        -- Allow cycle to avoid bus fight during read to write transition due
        -- to slow USB chip bus change. Also, this may be necessary to allow
        -- the USB chip sufficient time for the switch.
        when READ_WRITE_TRANS =>
          count_clear <= '1';
          fifoaddr <= C_IN_FIFOADDR;
          fd_T <= EN_IN;
          --fd_T <= EN_OUT;
          fd_O <= send_data;
          next_state <= WRITE_STATE;

        when WRITE_STATE =>
          fifoaddr <= C_IN_FIFOADDR;
          if send_ready = '1' then
            -- Send data to USB chip
            slwr_n <= commit_del;    -- Enable write ONLY if not a commit word
            pktend_n <= not commit_del;
            send_rd_en <= '1';
            fd_T <= EN_OUT;
            fd_O <= send_data;

            if C_MAX_TRANSFER > 0 then
              -- Check if we should switch to receiving
              if count_max = '1' and recv_ready = '1' then
                count_clear <= '1';
                next_state <= READ_STATE;
              end if;
            end if;
          else
            next_state <= IDLE_STATE;
          end if;

        when others =>
          next_state <= IDLE_STATE;
        
      end case;

    end if;
    
  end process;


  
  -----------------------------------------------------------------------------
  -- CLK DOMAIN ---------------------------------------------------------------
  -----------------------------------------------------------------------------

  
  -- This process is in charge or resetting the IF_CLK domain when the USB chip
  -- turns on. This reset is also used to reset any DCMs driving if_clk.
  --
  -- This process also indicates, with the usb_ready signal, when the reset
  -- process is complete and both the USB chip and interface core are ready.
  RESET_PROC : process (clk, reset, reset_state, usb_alive)
  begin
    if reset = '1' then
      if_reset <= '0';
      dcm_reset <= '0';
      usb_ready <= '0';
      reset_state <= '0';
    elsif clk'event and clk = '1' then
      -- If USB is disconnected, reset state machine.
      if usb_alive = '0' then
        dcm_reset <= '0';
        if_reset <= '0';
        usb_ready <= '0';
        reset_state <= '0';

      -- When USB is connected, assert if_clk domain reset.
      elsif reset_state = '0' then 
        if_reset <= '1';
        dcm_reset <= '1';
        usb_ready <= '0';
        reset_state <= '1';

      -- Don't allow if_reset to go high until dcm is locked.
      else
        if_reset <= not dcm_locked;
        usb_ready <= dcm_locked;
        dcm_reset <= '0';
      end if;
    end if;
  end process;


 
  -----------------------------------------------------------------------------
  -- CROSS CLOCK DOMAIN FIFOS -------------------------------------------------
  -----------------------------------------------------------------------------

  GEN_FIFO_16: if C_DATA_WIDTH = 16 generate
    -- USB to FPGA
    RECV_FIFO: usb_fifo_16
      port map (
        rst    => reset,
        -- FIFO IN
        wr_clk => if_clk,
        full   => recv_full,
        wr_en  => recv_wr_en,
        din    => recv_din,
        -- FIFO OUT
        rd_clk => clk,
        empty  => recv_empty,
        rd_en  => recv_rd_en,
        dout   => recv_dout);

    -- FPGA to USB
    send_din_i <= commit & send_din;
    commit_del <= send_dout_i(C_DATA_WIDTH);
    send_dout <= send_dout_i(C_DATA_WIDTH-1 downto 0);
    SEND_FIFO: usb_fifo_17
      port map (
        rst    => reset,
        -- FIFO IN
        wr_clk => clk,
        full   => send_full,
        wr_en  => send_wr_en,
        din    => send_din_i,
        -- FIFO OUT
        rd_clk => if_clk,
        empty  => send_empty_i,
        rd_en  => send_rd_en,
        dout   => send_dout_i);
  end generate;

  GEN_FIFO_OTHER: if C_DATA_WIDTH = 8 generate
    -- USB to FPGA
    RECV_FIFO: usb_fifo_8
      port map (
        rst    => reset,
        -- FIFO IN
        wr_clk => if_clk,
        full   => recv_full,
        wr_en  => recv_wr_en,
        din    => recv_din,
        -- FIFO OUT
        rd_clk => clk,
        empty  => recv_empty,
        rd_en  => recv_rd_en,
        dout   => recv_dout);

    -- FPGA to USB
    send_din_i <= commit & send_din;
    commit_del <= send_dout_i(C_DATA_WIDTH);
    send_dout <= send_dout_i(C_DATA_WIDTH-1 downto 0);
    SEND_FIFO: usb_fifo_9
      port map (
        rst    => reset,
        -- FIFO IN
        wr_clk => clk,
        full   => send_full,
        wr_en  => send_wr_en,
        din    => send_din_i,
        -- FIFO OUT
        rd_clk => if_clk,
        empty  => send_empty_i,
        rd_en  => send_rd_en,
        dout   => send_dout_i);
  end generate;

  GEN_FIFO_8: if C_DATA_WIDTH /= 8 and C_DATA_WIDTH /= 16 generate
    assert C_DATA_WIDTH = 8 or C_DATA_WIDTH = 16
      report "C_DATA_WIDTH can only be 8 or 16!"
      severity ERROR;
  end generate;
  
end architecture;
