------------------------------------------------------------------------------
-- user_logic.vhd - entity/architecture pair
------------------------------------------------------------------------------
--
-- ***************************************************************************
-- ** Copyright (c) 1995-2009 Xilinx, Inc.  All rights reserved.            **
-- **                                                                       **
-- ** Xilinx, Inc.                                                          **
-- ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
-- ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
-- ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
-- ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
-- ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
-- ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
-- ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
-- ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
-- ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
-- ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
-- ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
-- ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
-- ** FOR A PARTICULAR PURPOSE.                                             **
-- **                                                                       **
-- ***************************************************************************
--
------------------------------------------------------------------------------
-- Filename:          user_logic.vhd
-- Version:           1.01.a
-- Description:       User logic.
-- Date:              Thu Feb 04 14:27:30 2010 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
--
-- Modified 1/2010 by Travis Haroldsen
--   Updated to implement plb v4.6 needed for EDK 11.1
------------------------------------------------------------------------------
-- Naming Conventions:
--   active low signals:                    "*_n"
--   clock signals:                         "clk", "clk_div#", "clk_#x"
--   reset signals:                         "rst", "rst_n"
--   generics:                              "C_*"
--   user defined types:                    "*_TYPE"
--   state machine next state:              "*_ns"
--   state machine current state:           "*_cs"
--   combinatorial signals:                 "*_com"
--   pipelined or register delay signals:   "*_d#"
--   counter signals:                       "*cnt*"
--   clock enable signals:                  "*_ce"
--   internal version of output port:       "*_i"
--   device pins:                           "*_pin"
--   ports:                                 "- Names begin with Uppercase"
--   processes:                             "*_PROCESS"
--   component instantiations:              "<ENTITY_>I_<#|FUNC>"
------------------------------------------------------------------------------

-- DO NOT EDIT BELOW THIS LINE --------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;
use proc_common_v3_00_a.srl_fifo_f;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--   C_MST_AWIDTH                 -- Master interface address bus width
--   C_MST_DWIDTH                 -- Master interface data bus width
--   C_NUM_REG                    -- Number of software accessible registers
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Reset                 -- Bus to IP reset
--   Bus2IP_Addr                  -- Bus to IP address bus
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
--   IP2Bus_MstRd_Req             -- IP to Bus master read request
--   IP2Bus_MstWr_Req             -- IP to Bus master write request
--   IP2Bus_Mst_Addr              -- IP to Bus master address bus
--   IP2Bus_Mst_BE                -- IP to Bus master byte enables
--   IP2Bus_Mst_Length            -- IP to Bus master transfer length
--   IP2Bus_Mst_Type              -- IP to Bus master transfer type
--   IP2Bus_Mst_Lock              -- IP to Bus master lock
--   IP2Bus_Mst_Reset             -- IP to Bus master reset
--   Bus2IP_Mst_CmdAck            -- Bus to IP master command acknowledgement
--   Bus2IP_Mst_Cmplt             -- Bus to IP master transfer completion
--   Bus2IP_Mst_Error             -- Bus to IP master error response
--   Bus2IP_Mst_Rearbitrate       -- Bus to IP master re-arbitrate
--   Bus2IP_Mst_Cmd_Timeout       -- Bus to IP master command timeout
--   Bus2IP_MstRd_d               -- Bus to IP master read data bus
--   Bus2IP_MstRd_rem             -- Bus to IP master read remainder
--   Bus2IP_MstRd_sof_n           -- Bus to IP master read start of frame
--   Bus2IP_MstRd_eof_n           -- Bus to IP master read end of frame
--   Bus2IP_MstRd_src_rdy_n       -- Bus to IP master read source ready
--   Bus2IP_MstRd_src_dsc_n       -- Bus to IP master read source discontinue
--   IP2Bus_MstRd_dst_rdy_n       -- IP to Bus master read destination ready
--   IP2Bus_MstRd_dst_dsc_n       -- IP to Bus master read destination discontinue
--   IP2Bus_MstWr_d               -- IP to Bus master write data bus
--   IP2Bus_MstWr_rem             -- IP to Bus master write remainder
--   IP2Bus_MstWr_sof_n           -- IP to Bus master write start of frame
--   IP2Bus_MstWr_eof_n           -- IP to Bus master write end of frame
--   IP2Bus_MstWr_src_rdy_n       -- IP to Bus master write source ready
--   IP2Bus_MstWr_src_dsc_n       -- IP to Bus master write source discontinue
--   Bus2IP_MstWr_dst_rdy_n       -- Bus to IP master write destination ready
--   Bus2IP_MstWr_dst_dsc_n       -- Bus to IP master write destination discontinue
------------------------------------------------------------------------------

entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    C_USB_DATA_WIDTH               : integer              := 16;
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_SLV_DWIDTH                   : integer              := 32;
    C_MST_AWIDTH                   : integer              := 32;
    C_MST_DWIDTH                   : integer              := 32;
    C_NUM_REG                      : integer              := 1
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    dcm_locked  : in  std_logic;
    dcm_reset   : out std_logic;
    
    if_clk      : in  std_logic;
    
    usb_full_n  : in  std_logic;      -- IN FIFO full flag
    usb_empty_n : in  std_logic;      -- OUT FIFO empty flag
    usb_alive   : in  std_logic;      -- USB chip GPIO, high when device active

    Interrupt   : out std_logic;
    sloe_n      : out std_logic;         -- USB FIFO slave operate enable
    slrd_n      : out std_logic;         -- USB FIFO slave read enable
    slwr_n      : out std_logic;         -- USB FIFO slave write enable

    pktend_n    : out std_logic;                        -- USB FIFO packet end
    fifoaddr    : out std_logic_vector(1 downto 0);     -- USB FIFO address
    fd_I        : in  std_logic_vector(C_USB_DATA_WIDTH-1 downto 0); -- USB FIFO data
    fd_O        : out std_logic_vector(C_USB_DATA_WIDTH-1 downto 0); -- USB FIFO data
    fd_T        : out std_logic_vector(C_USB_DATA_WIDTH-1 downto 0); -- USB FIFO data
    -- ADD USER PORTS ABOVE THIS LINE ------------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  std_logic;
    Bus2IP_Reset                   : in  std_logic;
    Bus2IP_Addr                    : in  std_logic_vector(0 to 31);
    Bus2IP_Data                    : in  std_logic_vector(0 to C_SLV_DWIDTH-1);
    Bus2IP_BE                      : in  std_logic_vector(0 to C_SLV_DWIDTH/8-1);
    Bus2IP_RdCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    Bus2IP_WrCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    IP2Bus_Data                    : out std_logic_vector(0 to C_SLV_DWIDTH-1);
    IP2Bus_RdAck                   : out std_logic;
    IP2Bus_WrAck                   : out std_logic;
    IP2Bus_Error                   : out std_logic;
    IP2Bus_MstRd_Req               : out std_logic;
    IP2Bus_MstWr_Req               : out std_logic;
    IP2Bus_Mst_Addr                : out std_logic_vector(0 to C_MST_AWIDTH-1);
    IP2Bus_Mst_BE                  : out std_logic_vector(0 to C_MST_DWIDTH/8-1);
    IP2Bus_Mst_Length              : out std_logic_vector(0 to 11);
    IP2Bus_Mst_Type                : out std_logic;
    IP2Bus_Mst_Lock                : out std_logic;
    IP2Bus_Mst_Reset               : out std_logic;
    Bus2IP_Mst_CmdAck              : in  std_logic;
    Bus2IP_Mst_Cmplt               : in  std_logic;
    Bus2IP_Mst_Error               : in  std_logic;
    Bus2IP_Mst_Rearbitrate         : in  std_logic;
    Bus2IP_Mst_Cmd_Timeout         : in  std_logic;
    Bus2IP_MstRd_d                 : in  std_logic_vector(0 to C_MST_DWIDTH-1);
    Bus2IP_MstRd_rem               : in  std_logic_vector(0 to C_MST_DWIDTH/8-1);
    Bus2IP_MstRd_sof_n             : in  std_logic;
    Bus2IP_MstRd_eof_n             : in  std_logic;
    Bus2IP_MstRd_src_rdy_n         : in  std_logic;
    Bus2IP_MstRd_src_dsc_n         : in  std_logic;
    IP2Bus_MstRd_dst_rdy_n         : out std_logic;
    IP2Bus_MstRd_dst_dsc_n         : out std_logic;
    IP2Bus_MstWr_d                 : out std_logic_vector(0 to C_MST_DWIDTH-1);
    IP2Bus_MstWr_rem               : out std_logic_vector(0 to C_MST_DWIDTH/8-1);
    IP2Bus_MstWr_sof_n             : out std_logic;
    IP2Bus_MstWr_eof_n             : out std_logic;
    IP2Bus_MstWr_src_rdy_n         : out std_logic;
    IP2Bus_MstWr_src_dsc_n         : out std_logic;
    Bus2IP_MstWr_dst_rdy_n         : in  std_logic;
    Bus2IP_MstWr_dst_dsc_n         : in  std_logic
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute SIGIS : string;
  attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
  attribute SIGIS of Bus2IP_Reset  : signal is "RST";
  attribute SIGIS of IP2Bus_Mst_Reset: signal is "RST";
  attribute SIGIS of Interrupt     : signal is "INTR_LEVEL_HIGH";
end entity user_logic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic is

	-- fifo for holding burst data
  component plb_burst_fifo
    port (
      din       : IN std_logic_VECTOR(31 downto 0);
      rd_clk    : IN std_logic;
      rd_en     : IN std_logic;
      rst       : IN std_logic;
      wr_clk    : IN std_logic;
      wr_en     : IN std_logic;
      dout      : OUT std_logic_VECTOR(15 downto 0);
      empty     : OUT std_logic;
      full      : OUT std_logic);
  end component;    
     -- Bus decode signals
  -- portion of address bus used for determining registers
  signal addr_sel                       : std_logic_vector(3 downto 0); 

  -- Core status signals
  -- status of usb, dma, etc
  signal status                         : std_logic_vector(5 downto 0);

  -- USB core signals
  signal recv_empty                     : std_logic; -- usb receive fifo empty
  signal recv_rd_en                     : std_logic; -- usb receive fifo read enable
  signal recv_dout                      : std_logic_vector(C_USB_DATA_WIDTH-1 downto 0); -- receiver fifo output
  signal send_full                      : std_logic;  -- usb send fifo full
  signal send_empty                     : std_logic;  -- usb send fifo empty
  signal send_wr_en                     : std_logic;  -- usb send fifo write enable
  signal send_din                       : std_logic_vector(C_USB_DATA_WIDTH-1 downto 0);  -- usb send fifo data in
  signal commit                         : std_logic;  -- data to send to usb complete
  signal usb_ready                      : std_logic;  -- usb is ready

  signal single_send_wr_en              : std_logic; -- send single 16 bit word to fifo
    
  -- PLB Burst signals
  signal dma_start_addr                 : std_logic_vector(Bus2IP_Addr'length-1 downto 0); -- address of burst data to transmit
  signal dma_start_addr_wr_en           : std_logic; -- write enable for dma_start_addr register
  signal dma_start_addr_counter         : std_logic_vector(Bus2IP_Addr'length-1 downto 0); -- memory address to read current burst data from

  
  signal dma_trans_size                 : std_logic_vector(31 downto 0);  -- # of 16 bit words for next transfer, total bytes must equal bus width
  signal dma_trans_size_wr_en           : std_logic; -- write enable for dma_trans_size register
  signal dma_trans_size_remaining       : std_logic_vector(31 downto 0); -- 16 bit words remaining to send
  signal dma_trans_byte_to_recv         : std_logic_vector(31 downto 0); -- 16 bit words remaining to be received
  
  


  -- PLB Burst Fifo control signals
  signal pbf_rd_en                      : std_logic;
  signal pbf_wr_en                      : std_logic;
  signal pbf_out                        : std_logic_vector(15 downto 0);
  signal pbf_empty                      : std_logic;

  signal pbf_rd_en_delayed              : std_logic;  -- this is the pbf_rd_en delayed
                                              -- by one clock cycle to be used
                                              -- as the input to the usb_core
                                              -- send_wr_en signal

  signal dma_session_active             : std_logic; -- current DMA transfer occuring
  signal initiate_dma_transfer          : std_logic; -- initiate dma transfer
  signal terminate_dma_transfer         : std_logic; -- DMA transfer has finished
  
  type fg_state_type is                 ( DMA_SESSION_IDLE,
                                          DMA_SESSION_ASSERT,
										  DMA_SESSION_READ_FINISH,
										  DMA_SESSION_READ,
                                          DMA_SESSION_WAIT);

  signal current_state, next_state      : fg_state_type; -- state of DMA controller

  signal single_commit                  : std_logic; -- commit single word transfer
  signal burst_size                     : std_logic_vector(4 downto 0); -- size of DMA burst transfer
  signal burst_size_wr_en               : std_logic; -- write enable to burst_size register

begin

  -- Extract portion of address to be used
  addr_sel <= Bus2IP_Addr(Bus2IP_Addr'length-7 to Bus2IP_Addr'length-4);

  -- Generate status signal
  status <= dma_session_active & pbf_empty & usb_ready & recv_empty & send_empty & send_full;
  Interrupt <= not recv_empty;                                                      

  -----------------------------------------------------------------------------
  -- Data In/Out Steering Process
  DATA_ROUTE: process (Bus2IP_RdCE,Bus2IP_WrCE,Bus2IP_Data,addr_sel,recv_dout)
  begin
    -- Default values
    IP2Bus_Data                 <= (others => '0');
    IP2Bus_RdAck                <= '0';
    IP2Bus_WrAck                <= '0';

    -- USB core signals
    single_commit               <= '0';
    recv_rd_en                  <= '0';
    single_send_wr_en           <= '0';
    initiate_dma_transfer       <= '0';
    dma_start_addr_wr_en        <= '0';
    dma_trans_size_wr_en        <= '0';
    burst_size_wr_en            <= '0';

	-- PLB slave interface

    -- Handle bus reads
    if Bus2IP_RdCE(0) = '1' then
      Ip2Bus_RdAck <= '1';

      case addr_sel is
        when "0000" =>   -- Status read
          IP2Bus_Data(0 to 31) <=  conv_std_logic_vector(0, 32-status'length) & status;

        when "0001" =>   -- Data read         
          recv_rd_en <= '1';
          IP2Bus_Data(16 to 31) <= recv_dout;

        when "0010" => -- DMA Start Address
          IP2Bus_Data(0 to 31) <= dma_start_addr;

        when "0011" => -- DMA Transfer Size
          IP2Bus_Data(0 to 31) <= dma_trans_size;

        when others => null;                       
      end case;
    end if;


    -- Handle bus writes
    if Bus2IP_WrCE(0) = '1' then
      IP2Bus_WrAck <= '1'; 
        
      case addr_sel is
        when "0000" => -- Control write
          if Bus2IP_Data(31) = '1' then -- force commit
			--  usb_core expects send_wr_en to be high when commit is high.
			--   Data on the bus at this point will be ignored by usb_core.
            single_commit       <= '1';
            single_send_wr_en   <=  '1';
          else
            initiate_dma_transfer <= '1';
          end if;

        when "0001" => -- Data write from Data bus
          single_send_wr_en <= '1';         

        when "0010" => null;
           burst_size_wr_en <= '1';

        when "0011" => 
          dma_trans_size_wr_en <= '1';

        when "0100" =>
          dma_start_addr_wr_en <= '1';
            
        when others => null;
      end case;
    end if;               
  end process;


-------------------------------------------------------------------------------
-- DMA Transfer Registers
--
-- These registers store the start address and the number of 16 bit words to
-- transfer. 
--
-- Since the DATA_ROUTE process is not clocked, the signals need to be clocked
-- into a register which is done here
--
-- These are only loaded if we are not currently in an Active DMA Session.
-- 
-------------------------------------------------------------------------------
  process(Bus2IP_Clk,Bus2IP_Reset,dma_start_addr_wr_en)
  begin
    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Reset = '1' then
        dma_start_addr <= (others => '0');
      else
        if dma_start_addr_wr_en = '1' and (dma_session_active = '0') then
          dma_start_addr <= Bus2IP_Data(0 to 31);
        end if;
      end if;
    end if;
  end process;

  process(Bus2IP_Clk,Bus2IP_Reset,dma_trans_size_wr_en)
  begin
    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Reset = '1' then
        dma_trans_size <= (others => '0');
      else
        if dma_trans_size_wr_en = '1' and (dma_session_active = '0') then
          dma_trans_size <= Bus2IP_Data(0 to 31);
        end if;
      end if;
    end if;
  end process; 


-------------------------------------------------------------------------------
-- DMA Transfer Size Remaining
--
-- The dma_trans_size_remaining signal stores the number of 16 bit words that
-- need to be send to the USB Core. 
-- 
-- The dma_trans_size_remaining and the dma_trans_byte_to_recv signal is loaded 
-- with the value stored in the dma_trans_size register when an DMA Session is 
-- initiated.
-- 
-- Each time a 16 bit value is clocked out of the PLB Burst Fifo, the value in
-- the dma_trans_size_remaining is decremented by one and dma_trans_byte_to_recv
-- is decremented by (MST_DWIDTH / 16). 
--   
-- The dma_trans_byte_to_recv signal stores the number of 16 bit words that need
-- to be received via the DMA read until all words have been received.  It does
-- not stop the DMA transfer, it just prevents any new information from being
-- loaded into the fifo.
--
--
-------------------------------------------------------------------------------  

  process(Bus2IP_Clk,Bus2IP_Reset,initiate_dma_transfer,pbf_rd_en)
  begin
    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Reset = '1' then
        dma_trans_size_remaining <= (others => '0');
		dma_trans_byte_to_recv <= (others => '0');
      else
        if initiate_dma_transfer = '1' and (dma_session_active = '0') then
          dma_trans_size_remaining <= dma_trans_size;
		  dma_trans_byte_to_recv <= dma_trans_size;
        else
          if pbf_rd_en = '1' then
            dma_trans_size_remaining <= dma_trans_size_remaining - 1;
          end if;
		  if Bus2IP_MstRd_src_rdy_n = '0' and dma_trans_byte_to_recv /= X"00000000" then
		    dma_trans_byte_to_recv <= dma_trans_byte_to_recv - (C_MST_DWIDTH / 16);
		  end if;
        end if;
      end if;
    end if; 
  end process;

  
-------------------------------------------------------------------------------
-- Store the PLB Burst Size in a register
-------------------------------------------------------------------------------
  process(Bus2IP_Clk,Bus2IP_Reset,burst_size_wr_en)
  begin
    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Reset = '1' then
        burst_size <= "10000";          -- default to 128 words
      else
        if burst_size_wr_en = '1' then
          burst_size <= Bus2IP_Data(27 to 31);
        end if;
      end if;
    end if;
  end process; 

 -------------------------------------------------------------------------------
 -- DMA Start Address Counter
 -- 
 -- The dma_start_addr_counter is used to assert the the IP2BUS_Addr signal which tells
 -- the PLB Bus where to read from memory
 --
 -- It is loaded with the intial value stored in the dma_start_addr register at
 -- the start if the Entire DMA Session
 --
 -- At the end of each individual DMA Sub-Session, there is a single clock
 -- cycle assertion of the Bus2IP_Mst_Cmplt Signal which indicates that the
 -- burst transaction is completed. When that signal is asserted, we can safely
 -- increment the start address for the next burst 
 --  
 -------------------------------------------------------------------------------  

  process(Bus2IP_Clk,Bus2IP_Reset)
  begin
    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Reset = '1' then
        dma_start_addr_counter <= (others => '0');
      else        
        if initiate_dma_transfer = '1' and (dma_session_active = '0') then
          dma_start_addr_counter <= dma_start_addr;
        else
          if dma_session_active = '1' and Bus2IP_Mst_Cmplt = '1' then
            case burst_size is
				when "00001" => dma_start_addr_counter <= dma_start_addr_counter + 16; -- 8 words = 16 bytes
                when "00010" => dma_start_addr_counter <= dma_start_addr_counter + 32; -- 16 words = 32 bytes
                when "00100" => dma_start_addr_counter <= dma_start_addr_counter + 64; -- 32 words = 64 bytes
                when "01000" => dma_start_addr_counter <= dma_start_addr_counter + 128; -- 64 words = 128 bytes
                when "10000" => dma_start_addr_counter <= dma_start_addr_counter + 256; -- 128 words = 256 bytes
                when others => null;
            end case;
          end if;          
        end if;
      end if;
    end if;
  end process; 

  
 -------------------------------------------------------------------------------
 -- DMA session encapsalator
 --
 -- When the dma_session_active signal is HIGH then we are currently
 -- conducting multiple DMA Sub-Sessions.
 --
 -- Each DMA Sub-Session is a single PLB Burst Read session in which the PLB
 -- will burst data from memory into the PLB_Burst_Fifo. 
 --
 -- With each DMA Sub-Session, the count of remaining bytes to be transfered
 -- will be reduced slowly toward zero.
 --
 -- The dma_session_active signal will stay high as long as the count of bytes
 -- remaing (dma_trans_size_remaining) is greater than zero and the
 -- PLB_Burst_Fifo is not empty. Since the dma_session_active signal will stay
 -- HIGH until the PLB_Burst_Fifo is empty, the dma_session_active will only go
 -- LOW when all the data has been transfered from memory to the USB chip. 
 --
 -- Since the dma_session_active signal indicates that there is still data to
 -- transfer, it is used to initiate individual DMA Sub-Sessions. After each
 -- DMA Sub-Session has completed it will check to see if the
 -- dma_session_active signal is HIGH. If it is HIGH, then it will begin
 -- another DMA Sub-Session.
 --
 --   
 -------------------------------------------------------------------------------
  process(dma_session_active,dma_trans_size_remaining,pbf_empty)
  begin
    terminate_dma_transfer <= '0';
    if (dma_session_active = '1') and (dma_trans_size_remaining = X"00000000") and (pbf_empty = '1') and (send_full = '0')then
      terminate_dma_transfer <= '1';      
    end if;
  end process;
    
  process(Bus2IP_Clk)
  begin
    if Bus2IP_CLK'event and Bus2IP_Clk = '1' then
      if Bus2IP_Reset = '1' then
        dma_session_active <= '0';
      else
        if initiate_dma_transfer = '1' then
          dma_session_active <= '1';
        elsif terminate_dma_transfer = '1' then
          dma_session_active <= '0';
        end if;
      end if;
    end if;
  end process;


 ------------------------------------------------------------------------------
 -- DMA Sub-Session State Machine
 --
 -- STATES:
 --  1: Idle   - This state waits until the dma_session_active is 1
 --  2: Assert - This state waits until the Bus2IP_Mst_CmdAck is 1
 --  3: Read   - This state waits until the Bus2IP_Mst_Cmplt is 1
 --  3: Wait   - This state waits until the PLB Burst FIFO is empty
 -- 
 -----------------------------------------------------------------------------

  process(Bus2IP_Clk,Bus2IP_Reset,next_state)
  begin
    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Reset = '1' then
        current_state <= DMA_SESSION_IDLE;
      else
        current_state <= next_state;
      end if;
    end if;
  end process; 


  process(Bus2IP_Clk,dma_session_active,Bus2IP_Mst_Cmplt,Bus2IP_Mst_CmdAck,pbf_empty, 
		Bus2IP_MstRd_EOF_n, Bus2IP_MstRd_src_rdy_n, dma_start_addr_counter, burst_size,
		send_empty, current_state)
  begin
    next_state <= current_state;

    IP2Bus_MstRd_Req    <= '0';
    IP2Bus_MstWr_Req    <= '0';
    IP2Bus_Mst_Lock     <= '0';
    IP2Bus_Mst_Length   <= (others => '0');
    IP2Bus_Mst_Addr     <= (others => '0');
    IP2Bus_Mst_BE       <= (others => '0'); -- don't care for burst mode
    IP2Bus_Mst_Reset    <= '0';
	IP2Bus_Mst_Type     <= '0';
	
	IP2Bus_MstRd_dst_rdy_n  <= '1';
	IP2Bus_MstRd_dst_dsc_n  <= '1';
    IP2Bus_MstWr_d          <= (others => '0');
    IP2Bus_MstWr_rem        <= (others => '0');
    IP2Bus_MstWr_sof_n      <= '1';
    IP2Bus_MstWr_eof_n      <= '1';
    IP2Bus_MstWr_src_rdy_n  <= '1';
    IP2Bus_MstWr_src_dsc_n  <= '1';

	pbf_wr_en       <= '0';
      
    case current_state is
      when DMA_SESSION_IDLE => 
               if dma_session_active = '1' and send_empty = '1'then
                 next_state <= DMA_SESSION_ASSERT;
               end if;

      when DMA_SESSION_ASSERT => 
               IP2Bus_MstRd_Req         <= '1';
               IP2Bus_Mst_Length        <= "000" & burst_size & "0000";
               IP2Bus_Mst_Addr          <= dma_start_addr_counter;
			   IP2Bus_Mst_Type          <= '1'; -- burst mode
			   IP2Bus_MstRd_dst_rdy_n   <= '0';

			   if Bus2IP_Mst_CmdAck = '1' then
				 next_state <= DMA_SESSION_READ;
			   end if;
			   
      when DMA_SESSION_READ =>

			   IP2Bus_MstRd_dst_rdy_n   <= '0';
			   if Bus2IP_MstRd_src_rdy_n = '0' then
				  if dma_trans_byte_to_recv /= X"00000000" then
				     pbf_wr_en <= '1';
				  end if;

				  if Bus2IP_MstRd_eof_n = '0' then
			         next_state <= DMA_SESSION_READ_FINISH;
			      end if;
		       end if;

	  when DMA_SESSION_READ_FINISH =>
			   if Bus2IP_Mst_Cmplt = '1' then
			     next_state <= DMA_SESSION_WAIT;
			   end if;
      when DMA_SESSION_WAIT =>              
               if pbf_empty = '1' then
                 next_state <= DMA_SESSION_IDLE;
               end if;

      when others => null;
    end case;
  end process; 
  
 ------------------------------------------------------------------------------
 -- PLB Burst Fifo
 --
 -- The PLB Burst Fifo has a 64 bit input that is written to from the PLB Bus.
 -- 
 -- The output of the PLB Burst Fifo is connected to the USB Core chip. The
 -- fifo will clock out the 16 bit data whenever the FIFO is not empty and the
 -- USB Core chip is not have a full send FIFO
 --
 -- The FIFO has a 1 clock cycle delay from the time that the rd_en signal is
 -- asserted until the data out is valid. So a second process is used to delay
 -- the rd_en signal one clock cycle. That delayed signal is then used as the
 -- wr_en for the USB Core. The single_send_wr_en is to allow single 16 bit
 -- words to be written to the USB Core from software.
 --
 -- A third process is used to control what data is put onto the data bus that
 -- goes into the USB Core. This is needed because this user_logic core needs
 -- to be able to write single 16 bit words to the USB Core and Data from DMA
 -- Session transactions
 -- 
 ------------------------------------------------------------------------------

  PLB_BURST_FIFO1: plb_burst_fifo
    port map (
      din       => Bus2IP_MstRd_d,
      rd_clk    => Bus2IP_Clk,
      rd_en     => pbf_rd_en,
      rst       => Bus2IP_Reset,
      wr_clk    => Bus2IP_Clk,
      wr_en     => pbf_wr_en,
      dout      => pbf_out,
      empty     => pbf_empty,
      full      => open);          


  process(send_full,pbf_empty,pbf_out,Bus2IP_Data)
  begin
    send_din    <= Bus2IP_Data(0 to C_USB_DATA_WIDTH-1);
    pbf_rd_en   <= '0';

    if send_full = '0' and pbf_empty = '0' then
      send_din        <= pbf_out;
      pbf_rd_en       <= '1';
    end if;        
  end process;
  send_wr_en    <= pbf_rd_en or single_send_wr_en or (terminate_dma_transfer);
  commit        <= single_commit or (terminate_dma_transfer);
  
  

  IP2Bus_Error       <= '0';
  


 USB_CORE_I: entity plb_usb_v1_01_a.usb_core
    generic map (
        C_IN_FIFOADDR  => "10",
        C_OUT_FIFOADDR => "00",
        C_BIG_ENDIAN   => TRUE,
        C_DATA_WIDTH   => C_USB_DATA_WIDTH,
        C_MAX_TRANSFER => 16,
        C_RST_HIGH     => TRUE)
    port map (
        clk         => Bus2IP_Clk,
        rst         => Bus2IP_Reset,
        dcm_locked  => dcm_locked,
        dcm_reset   => dcm_reset,
        recv_empty  => recv_empty,
        recv_rd_en  => recv_rd_en,
        recv_dout   => recv_dout,
        send_full   => send_full,
        send_empty  => send_empty,
        send_wr_en  => send_wr_en,
        send_din    => send_din,
        commit      => commit,
        usb_ready   => usb_ready,
        if_clk      => if_clk,
        usb_full_n  => usb_full_n,
        usb_empty_n => usb_empty_n,
        usb_alive   => usb_alive,
        sloe_n      => sloe_n,
        slrd_n      => slrd_n,
        slwr_n      => slwr_n,
        pktend_n    => pktend_n,
        fifoaddr    => fifoaddr,
        fd_I        => fd_I,
        fd_O        => fd_O,
        fd_T        => fd_T);

end IMP;
