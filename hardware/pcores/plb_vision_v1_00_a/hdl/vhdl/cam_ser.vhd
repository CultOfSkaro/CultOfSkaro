-------------------------------------------------------------------------------
-- Micron Camera Serial Bus Interface
--
-- FILE: cam_ser.vhd
-- AUTHOR: Wade S. Fife
-- DATE: June 28, 2004
-- MODIFIED: March 25, 2005
--
-- DESCRIPTION
-- 
-- Provides an interface to the serial control line on Micron MI-SOC-0133
-- digital camera.
--
-- The ready output indicates that the device is not busy and can accept read
-- or write commands. If the ready output is low, then the device is busy
-- processing a command and will not accept a new read or write.
--
-- Writing to a camera register is performed by asserting dev_addr with the
-- device address of the camera (0xB8 for the MI-SOC-0133), reg_addr with the
-- register number you wish to write to, and data_in with the data you wish to
-- write, and assert wr_en with a 1. The data will be registered on the next
-- positive clock edge and the write will begin. The write is not complete
-- until the ready signal goes high.
--
-- Reading is performed similarly by asserting dev_addr and reg_addr, but rd_en
-- should be asserted instead of wr_en. Once the data has been read (which may
-- take some time) it will appear on data_out on the same cycle that the ready
-- output goes back high and will remain their until the next read completes.
--
-- The state machine is rather complicated and will be difficult to follow
-- without a good understanding of Micron's two-wire serial bus
-- protocol. Basically, each bit is divided into four sections: the section
-- waiting until sdata needs to be set (SER_WAIT_DATA_EDGE), the section
-- waiting until sclk needs to go high (SER_WAIT_CLK_PEDGE), two sections
-- waiting until sclk needs to go low (SER_WAIT_CLK_NEDGE_X), and then back to
-- waiting until the data needs to be set again. There are a few variations of
-- these states which handle special cases, including when acknowledge bits are
-- received (SER_WAIT_Z_XXX), when data is read rather than sent
-- (SER_WAIT_RD_XXX), etc. There is also special handling two cases of start
-- bits (SER_WAIT_XXX_START and SER_WAIT_NEW_XXX) as well as stop bits.
--
-- Note on sdata Tri-state Net:
--
-- Apparrently the Xilinx Embedded System Tools (EST) can't do tristate below
-- the top level. So instead of doing a single tristate line, the sdata port is
-- divided into three signals with the _I (input) _O (output) and _T (tristate
-- control) suffixes. At the top level, these are inferred to be inputs to a
-- tristate buffer.
--
-- LIMITATIONS
--
-- - Currently does not support error detection and recovery (indicated by lack
--   of appropriate acknowledge bits).
--
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cam_ser is
  generic (
    -- This specifies the clock divisor used to generate the master signals.
    -- The period of SCLK will be 4*CLK_DIV*(period of clk). Must be < 256.
    C_CLK_DIV : integer := 224);
  
  port (
    clk                : in std_logic;
    reset              : in std_logic;

    wr_en              : in std_logic;
    rd_en              : in std_logic;
    dev_addr           : in std_logic_vector(0 to 7);
    reg_addr           : in std_logic_vector(0 to 7);
    data_in            : in std_logic_vector(0 to 15);
    data_out           : out std_logic_vector(0 to 15);
    ready              : out std_logic;
    
    sdata_I            : in std_logic;
    sdata_O            : out std_logic;
    sdata_T            : out std_logic;
    sclk               : out std_logic);

end cam_ser;

architecture imp of cam_ser is
  -- States
  type ser_state_type is ( SER_IDLE,

                           -- Send initial start bit states
                           SER_WAIT_START,
                           SER_WAIT_CLK_START,
                           SER_WAIT_DATA_START,

                           -- Normal data send states
                           SER_WAIT_DATA_EDGE,
                           SER_WAIT_CLK_PEDGE,
                           SER_WAIT_CLK_NEDGE_A,
                           SER_WAIT_CLK_NEDGE_B,

                           -- Send high Z states (for ACK, NACK)
                           SER_WAIT_Z_CLK_PEDGE,
                           SER_WAIT_Z_CLK_NEDGE_A,
                           SER_WAIT_Z_CLK_NEDGE_B,
                           SER_WAIT_Z_DATA_EDGE,

                           -- Send new stop/start bit states
                           SER_WAIT_NEW_CLK_PEDGE,
                           SER_WAIT_NEW_CLK_NEDGE_A,
                           SER_WAIT_NEW_CLK_NEDGE_B,
                           SER_WAIT_NEW_DATA_EDGE,

                           -- Read data response cycle states
                           SER_WAIT_RD_CLK_PEDGE,
                           SER_WAIT_RD_CLK_NEDGE_A,
                           SER_WAIT_RD_CLK_NEDGE_B,
                           SER_WAIT_RD_DATA_EDGE,
                           SER_WAIT_RD_ACK_CLK_PEDGE,
                           SER_WAIT_RD_ACK_CLK_NEDGE_A,
                           SER_WAIT_RD_ACK_CLK_NEDGE_B,
                           SER_WAIT_RD_ACK_DATA_EDGE,

                           -- Stop bit states
                           SER_WAIT_STOP_CLK,
                           SER_WAIT_STOP_DATA,

                           -- Output received data
                           SER_WRITE_BACK
                         );

  -- These constants may need to be revised!
  constant CYCLE_COUNT_SIZE : integer := 8;
  constant CYCLE_COUNT : integer := C_CLK_DIV;
  constant END_CYCLE_COUNT : unsigned(0 to CYCLE_COUNT_SIZE-1)
    := to_unsigned(CYCLE_COUNT-1, CYCLE_COUNT_SIZE);

  constant EN_OUT : std_logic := '0';   -- Allow output on tristate
  constant EN_IN : std_logic := '1';    -- Allow input on tristate

  -- Possible values for mode_reg (read or write)
  constant WRITE_MODE : std_logic := '0';
  constant READ_MODE : std_logic := '1';

  -- State machine states
  signal current_state, next_state : ser_state_type;

  -- Current command being processed (read or write)
  signal mode_reg : std_logic;
  signal mode_load : std_logic;

  -- Shift register to grab data received on read commands
  signal read_data_reg : std_logic_vector(0 to 15);
  signal read_data_shift : std_logic;

  -- Serial data register, which holds data to be output on sdata
  -- (Includes some start/stop bit data)
  signal sdata_reg : std_logic_vector(0 to 33);
  signal sdata_set_msb : std_logic;
  signal sdata_wr_load : std_logic;
  signal sdata_rd_load : std_logic;
  signal sdata_shift : std_logic;

  -- Serial clock output register (registered to avoid bounce)
  signal sclk_reg : std_logic;
  signal sclk_set : std_logic;
  signal sclk_clear : std_logic;

  -- Counter to count number of bits transmitted
  signal bit_count_reg : unsigned(0 to 2);
  signal bit_count_clear : std_logic;
  signal bit_count_inc : std_logic;

  -- Counter to count number of bytes transmitted
  signal byte_count_reg : unsigned(0 to 2);
  signal byte_count_clear : std_logic;
  signal byte_count_inc : std_logic;

  -- Counter to divide down clk so serial communication is correct speed
  signal cycle_count_reg : unsigned(0 to CYCLE_COUNT_SIZE-1);
  signal cycle_count_clear : std_logic;

  -- Indicates 1/4 period of sclk has elapsed (based on cycle_count_reg)
  signal cycle_end : std_logic;

  signal data_out_load : std_logic;
  
begin

  -- Process for read_data_reg register
  READ_DATA_REG_PROC : process (clk, read_data_shift)
  begin
    if clk'event and clk='1' then
      if read_data_shift = '1' then
        read_data_reg <= read_data_reg(1 to 15) & sdata_I;
      end if;
    end if;
  end process READ_DATA_REG_PROC;

  -- Process for mode_reg register
  MODE_REG_PROC : process (clk, mode_load)
  begin
    if clk'event and clk='1' then
      if mode_load = '1' then
        if wr_en = '1' then
          mode_reg <= WRITE_MODE;
        elsif rd_en = '1' then
          mode_reg <= READ_MODE;
        end if;
      end if;
    end if;
  end process MODE_REG_PROC;

  -- Process for data_reg register
  DATA_REG_PROC : process (clk, reset, sdata_wr_load, sdata_shift)
  begin
    if reset='1' then
      sdata_reg <= (others => '1');
    elsif clk'event and clk='1' then
      -- Note: Extra '1' and '0's are start bit data
      if sdata_set_msb = '1' then
        sdata_reg(0) <= '1';
      elsif sdata_wr_load = '1' then
        sdata_reg <= '1' & '0' & dev_addr(0 to 7) & reg_addr & data_in;
      elsif sdata_rd_load = '1' then
        -- Note: The (not dev_addr(7)) is because device write address
        --       is different by 1 bit.
        sdata_reg <= '1' & '0' & dev_addr(0 to 7)
                     & reg_addr & '1' & '0' & dev_addr(0 to 6)
                     & (not dev_addr(7)) & "000000";
      elsif sdata_shift = '1' then
        sdata_reg <= sdata_reg(1 to 33) & '0';
      end if;
    end if;
  end process DATA_REG_PROC;

  -- Process for clk_reg register
  CLK_REG_PROC: process (clk, reset, sclk_set, sclk_clear)
  begin
    if reset = '1' then
      sclk_reg <= '1';      
    elsif clk'event and clk = '1' then
      if sclk_set = '1' then
        sclk_reg <= '1';
      elsif sclk_clear = '1' then
        sclk_reg <= '0';
      end if;
    end if;
  end process CLK_REG_PROC;
  
  -- Process for bit counter (to determine end of bytes)
  BIT_COUNTER_REG : process (clk, bit_count_clear, bit_count_inc)
  begin
    if clk'event and clk='1' then
      if bit_count_clear = '1' then
        bit_count_reg <= (others => '0');
      elsif bit_count_inc = '1' then
        bit_count_reg <= bit_count_reg + 1;
      end if;      
    end if;
  end process BIT_COUNTER_REG;

  -- Process for byte counter (to determine end of words)
  BYTE_COUNTER_REG : process (clk, byte_count_clear, byte_count_inc)
  begin
    if clk'event and clk='1' then
      if byte_count_clear = '1' then
        byte_count_reg <= (others => '0');
      elsif byte_count_inc = '1' then
        byte_count_reg <= byte_count_reg + 1;
      end if;
    end if;
  end process BYTE_COUNTER_REG;

  -- Process for clock cycle counter, used for clock division
  -- This cycle counter automatically restarts
  CYCLE_COUNTER_REG : process (clk, cycle_count_clear, cycle_end)
  begin
    if clk'event and clk='1' then
      if (cycle_count_clear = '1') or (cycle_end = '1') then
        cycle_count_reg <= (others => '0');
      else
        cycle_count_reg <= cycle_count_reg + 1;
      end if;
    end if;
  end process CYCLE_COUNTER_REG;

  -- Process for cycle_end signal (indicating 1/4 sclk period elapsed)
  CYCLE_END_PROC : process (cycle_count_reg)
  begin
    if cycle_count_reg >= END_CYCLE_COUNT then
      cycle_end <= '1';
    else
      cycle_end <= '0';
    end if;
  end process CYCLE_END_PROC;

  DATA_OUT_REG: process(clk, data_out_load, read_data_reg)
  begin
    if clk'event and clk = '1' then
      if data_out_load = '1' then
        data_out <= read_data_reg;
      end if;
    end if;
  end process DATA_OUT_REG;

  
  -- Main serial driver state machine registers
  SER_FSM_REG: process (clk, reset)
  begin
    if reset = '1' then                 -- asynchronous reset (active low)
      current_state <= SER_IDLE;
    elsif clk'event and clk = '1' then  -- rising clock edge
      current_state <= next_state;
    end if;
  end process SER_FSM_REG;

  -- Main serial driver state machine logic
  SER_FSM_COMB: process (current_state, wr_en, rd_en, cycle_end, sclk_reg,
                         sdata_reg, bit_count_reg, mode_reg, byte_count_reg,
                         read_data_reg)
  begin
    -- Default signal values
    next_state <= current_state;
    ready <= '0';
    mode_load <= '0';
    read_data_shift <= '0';
    sdata_set_msb <= '0';    
    sdata_wr_load <= '0';
    sdata_rd_load <= '0';
    sdata_shift <= '0';
    bit_count_clear <= '0';
    bit_count_inc  <= '0';
    byte_count_clear <= '0';
    byte_count_inc <= '0';
    cycle_count_clear <= '0';
    sclk_clear <= '0';
    sclk_set <= '0';
    data_out_load <= '0';

    sclk <= sclk_reg;
    sdata_O <= sdata_reg(0);
    sdata_T <= EN_OUT;

    case current_state is
      when SER_IDLE =>
        sclk <= sclk_reg;
        ready <= '1';

        -- Load data based on read or write signals (one gets priority)
        sdata_wr_load <= wr_en;
        sdata_rd_load <= rd_en;

        if wr_en = '1' or rd_en = '1' then
          mode_load <= '1';
          -- Initialize sclock high
          sclk_set <= '1';
          -- Set mode, reset bit and byte counts, and cycle count
          mode_load <= '1';
          bit_count_clear <= '1';
          byte_count_clear <= '1';
          cycle_count_clear <= '1';
          next_state <= SER_WAIT_START;
        end if;
        

      -- This section starts the write/read process (sends initial start bit)
        
      when SER_WAIT_START =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sdata_shift <= '1';
          next_state <= SER_WAIT_CLK_START;
        end if;

     when SER_WAIT_CLK_START =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sclk_clear <= '1';
          next_state <= SER_WAIT_DATA_START;
        end if;

      when SER_WAIT_DATA_START =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sdata_shift <= '1';
          next_state <= SER_WAIT_CLK_PEDGE;
        end if;


      -- This is the main states section, where data bits are sent
        
      when SER_WAIT_DATA_EDGE =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sdata_shift <= '1';
          bit_count_inc <= '1';
          if bit_count_reg = "111" then
            next_state <= SER_WAIT_Z_CLK_PEDGE;
          else
            next_state <= SER_WAIT_CLK_PEDGE;
          end if;
        end if;
        
      when SER_WAIT_CLK_PEDGE =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sclk_set <= '1';
          next_state <= SER_WAIT_CLK_NEDGE_A;
        end if;
        
      when SER_WAIT_CLK_NEDGE_A =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          next_state <= SER_WAIT_CLK_NEDGE_B;
        end if;
        
      when SER_WAIT_CLK_NEDGE_B =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sclk_clear <= '1';
          next_state <= SER_WAIT_DATA_EDGE;
        end if;


      -- This section is for when acknowledge bits are received
      -- (sdata must be high-Z tristate)
        
      when SER_WAIT_Z_CLK_PEDGE =>
        sdata_T <= EN_IN;
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sclk_set <= '1';
          next_state <= SER_WAIT_Z_CLK_NEDGE_A;
        end if;
        
      when SER_WAIT_Z_CLK_NEDGE_A =>
        sdata_T <= EN_IN;
        sclk <= sclk_reg;
        if cycle_end = '1' then
          next_state <= SER_WAIT_Z_CLK_NEDGE_B;
        end if;
        
      when SER_WAIT_Z_CLK_NEDGE_B =>
        sdata_T <= EN_IN;
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sclk_clear <= '1';
          next_state <= SER_WAIT_Z_DATA_EDGE;
        end if;
        
      when SER_WAIT_Z_DATA_EDGE =>
        sdata_T <= EN_IN;
        sclk <= sclk_reg;
        if cycle_end = '1' then
          byte_count_inc <= '1';
          bit_count_clear <= '1';

          if mode_reg = WRITE_MODE then
            if byte_count_reg = "011" then
              next_state <= SER_WAIT_STOP_CLK;
            else
              next_state <= SER_WAIT_CLK_PEDGE;
            end if;
          else
            if byte_count_reg = "001" then
              next_state <= SER_WAIT_NEW_CLK_PEDGE;
            elsif byte_count_reg = "010" then
              next_state <= SER_WAIT_RD_CLK_PEDGE;
            else
              next_state <= SER_WAIT_CLK_PEDGE;
            end if;
          end if;
        end if;


      -- This section is for sending a new start bit, used during reads
        
      when SER_WAIT_NEW_CLK_PEDGE =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sclk_set <= '1';
          next_state <= SER_WAIT_NEW_CLK_NEDGE_A;
        end if;

      when SER_WAIT_NEW_CLK_NEDGE_A =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sdata_shift <= '1';
          next_state <= SER_WAIT_NEW_CLK_NEDGE_B;
        end if;

      when SER_WAIT_NEW_CLK_NEDGE_B =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sclk_clear <= '1';
          next_state <= SER_WAIT_NEW_DATA_EDGE;
        end if;

      when SER_WAIT_NEW_DATA_EDGE =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sdata_shift <= '1';
          -- Return to main states
          next_state <= SER_WAIT_CLK_PEDGE;
        end if;


      -- This section is for reading the reply of a read command
        
      when SER_WAIT_RD_CLK_PEDGE =>
        sdata_T <= EN_IN;
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sclk_set <= '1';
          next_state <= SER_WAIT_RD_CLK_NEDGE_A;
        end if;
        
      when SER_WAIT_RD_CLK_NEDGE_A =>
        sdata_T <= EN_IN;
        sclk <= sclk_reg;
        if cycle_end = '1' then
          read_data_shift <= '1';
          next_state <= SER_WAIT_RD_CLK_NEDGE_B;
        end if;
        
      when SER_WAIT_RD_CLK_NEDGE_B =>
        sdata_T <= EN_IN;
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sclk_clear <= '1';
          next_state <= SER_WAIT_RD_DATA_EDGE;
        end if;
        
      when SER_WAIT_RD_DATA_EDGE =>
        sdata_T <= EN_IN;
        sclk <= sclk_reg;
        if cycle_end = '1' then
          bit_count_inc <= '1';

          if bit_count_reg = "111" then
            next_state <= SER_WAIT_RD_ACK_CLK_PEDGE;
          else
            next_state <= SER_WAIT_RD_CLK_PEDGE;
          end if;
        end if;


      -- This section is for the acknwledgments sent during the read phase of a
      -- read command.
        
      when SER_WAIT_RD_ACK_CLK_PEDGE =>
        sclk <= sclk_reg;
        sdata_T <= EN_OUT;
        if byte_count_reg = "100" then
          sdata_O <= '1';
        else
          sdata_O <= '0';
        end if;

        if cycle_end = '1' then
          sclk_set <= '1';
          next_state <= SER_WAIT_RD_ACK_CLK_NEDGE_A;
        end if;
        
      when SER_WAIT_RD_ACK_CLK_NEDGE_A =>
        sclk <= sclk_reg;
        sdata_T <= EN_OUT;
        if byte_count_reg = "100" then
          sdata_O <= '1';
        else
          sdata_O <= '0';
        end if;

        if cycle_end = '1' then
          next_state <= SER_WAIT_RD_ACK_CLK_NEDGE_B;
        end if;
        
      when SER_WAIT_RD_ACK_CLK_NEDGE_B =>
        sclk <= sclk_reg;
        sdata_T <= EN_OUT;
        if byte_count_reg = "100" then
          sdata_O <= '1';
        else
          sdata_O <= '0';
        end if;

        if cycle_end = '1' then
          sclk_clear <= '1';
          next_state <= SER_WAIT_RD_ACK_DATA_EDGE;
        end if;
        
      when SER_WAIT_RD_ACK_DATA_EDGE =>
        sclk <= sclk_reg;
        sdata_T <= EN_OUT;
        if byte_count_reg = "100" then
          sdata_O <= '1';
        else
          sdata_O <= '0';
        end if;

        if cycle_end = '1' then
          byte_count_inc <= '1';
          if byte_count_reg = "100" then
            next_state <= SER_WAIT_STOP_CLK;
          else
            next_state <= SER_WAIT_RD_CLK_PEDGE;
          end if;
        end if;


      -- This section is for sending the final stop bit
        
      when SER_WAIT_STOP_CLK =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sclk_set <= '1';
          next_state <= SER_WAIT_STOP_DATA;
        end if;
        
      when SER_WAIT_STOP_DATA =>
        sclk <= sclk_reg;
        if cycle_end = '1' then
          sdata_set_msb <= '1';         -- Set sdata_reg(0) back to 1
          if mode_reg = WRITE_MODE then
            next_state <= SER_IDLE;
          else
            next_state <= SER_WRITE_BACK;
          end if;
        end if;

        
      -- This state is one cycle for returning the value read
        
      when SER_WRITE_BACK =>
        ready <= '1';
--        data_out <= read_data_reg;
        data_out_load <= '1';
        next_state <= SER_IDLE;
        
      when others =>
        next_state <= current_state;
        
    end case;

  end process SER_FSM_COMB;

end imp;
