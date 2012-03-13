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
-- Version:           1.00.a
-- Description:       User logic.
-- Date:              Thu Jan 14 11:53:27 2010 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
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

-- DESCRIPTION
--
-- This core consists of a quadrature encoder detector for tracking position
-- and direction as indicated by a quadrature encoder. The output of this core
-- is simply a signed integer counter that gives the number of ticks of the
-- encoder. If the channel A encoder signal leads B then the counter is
-- incremented. If channel B leads A then the counter is decremented. By
-- frequently reading the encoder counter value you can determine position and
-- angular velocity of the encoder.
--
-- DOCUMENTATION
--
-- The C_COUNT_WIDTH generic determines the width of the counter. A small
-- counter will roll over sooner than a large counter. How long this takes
-- depends on the precision of the encoder used and the amount of rotation.
--
-- The "enc_a" and "enc_b" inputs are the two encoder channel signals. The
-- synchronous "clr" input allows you to reset the current encoder count. The
-- "count" output signal is the current encoder count, a signed 2's complement
-- number.
--
-------------------------------------------------------------------------------
------------------------------------------------------------------------------

-- DO NOT EDIT BELOW THIS LINE --------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--   C_NUM_REG                    -- Number of software accessible registers
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Reset                 -- Bus to IP reset
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
------------------------------------------------------------------------------

entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    --USER generics added here
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_SLV_DWIDTH                   : integer              := 32;
    C_NUM_REG                      : integer              := 1
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    -- replaced by Bus2IP_Clk  clk : in std_logic;
    -- replaced by Bus2IP_Reset  rst    : in std_logic;
    clr    : in std_logic;
    enc_a  : in  std_logic;
    enc_b  : in  std_logic;
    --not an output, saved into a register count  : out std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    -- ADD USER PORTS ABOVE THIS LINE ------------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  std_logic;
    Bus2IP_Reset                   : in  std_logic;
    Bus2IP_Data                    : in  std_logic_vector(0 to C_SLV_DWIDTH-1);
    Bus2IP_BE                      : in  std_logic_vector(0 to C_SLV_DWIDTH/8-1);
    Bus2IP_RdCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    Bus2IP_WrCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    IP2Bus_Data                    : out std_logic_vector(0 to C_SLV_DWIDTH-1);
    IP2Bus_RdAck                   : out std_logic;
    IP2Bus_WrAck                   : out std_logic;
    IP2Bus_Error                   : out std_logic
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute SIGIS : string;
  attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
  attribute SIGIS of Bus2IP_Reset  : signal is "RST";

end entity user_logic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic is

  --USER signal declarations added here, as needed for user logic
  -- Define enc_reg indices
  constant C_ENC_A : integer := 0;
  constant C_ENC_B : integer := 1;  
  -- Encoder output states
  type enc_state_type is ( ENC_STATE_1,
                           ENC_STATE_2);
  signal enc_state, next_enc_state : enc_state_type;

  signal enc_count_reg : unsigned(0 to C_SLV_DWIDTH-1);
  signal enc_count_inc : std_logic;
  signal enc_count_dec : std_logic;
  signal enc_count_clr : std_logic;
  signal enc_reg : unsigned(0 to 1);


  signal enc_a_buf_1 : std_logic;  -- buffer the encoder inputs to avoid glitching -- new by sf89, proposed by Wade, 2009
  signal enc_a_buf_2 : std_logic;  -- buffer the encoder inputs to avoid glitching -- new by sf89, proposed by Wade, 2009
  signal enc_b_buf_1 : std_logic;  -- buffer the encoder inputs to avoid glitching -- new by sf89, proposed by Wade, 2009;
  signal enc_b_buf_2 : std_logic;  -- buffer the encoder inputs to avoid glitching -- new by sf89, proposed by Wade, 2009
  signal enc_sample_counter : unsigned(0 to 5);
  signal enc_sample_en : std_logic;     -- enable the encoder counter := <[initialization]>
                                     
  ------------------------------------------
  -- Signals for user logic slave model s/w accessible register example
  ------------------------------------------
  signal slv_reg0                       : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal slv_reg_write_sel              : std_logic_vector(0 to 0);
  signal slv_reg_read_sel               : std_logic_vector(0 to 0);
  signal slv_ip2bus_data                : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal slv_read_ack                   : std_logic;
  signal slv_write_ack                  : std_logic;
	
	signal clk : std_logic;
	signal rst : std_logic;

begin

  --USER logic implementation added here
  clk <= Bus2IP_Clk;
  rst <= Bus2IP_Reset;
  enc_count_clr <= clr;
  --count <= std_logic_vector(enc_count_reg);
  slv_reg0 <= std_logic_vector(enc_count_reg);
  -- purpose: The encoder rise time is 500ns.  The Helios FPGA fabric runs at 100Mhz,
  -- so clk may be sampling much much faster than the encoder needs.  To avoid 
  -- glitching during rise and fall times, we'll only sample every 50 clk cycles
  -- type   : sequential
  -- inputs : clk, rst, enc_sample_counter
  -- outputs: enc_sample_en
  SAMPLE_PERIOD: process (clk)
  begin  -- process SAMPLE_PERIOD
    if clk'event and clk = '1' then  -- rising clock edge
      if rst = '1' then                   -- synchronous reset (active high)
        enc_sample_en <= '0';
        enc_sample_counter <= (others => '0');
		 else
			enc_sample_counter <= enc_sample_counter + 1;
			enc_sample_en <= '0';
			if enc_sample_counter = 50 then
				enc_sample_en <= '1';
				enc_sample_counter <= (others => '0');
			end if;
      end if;
    end if;
  end process SAMPLE_PERIOD;
  -----------------------------------------------------------------------------
  -- Buffer the encoder inputs to avoid glitching.  There was a problem with
  -- the encoders suddenly writing crazy negative values to the register and
  -- Wade thought buffering the inputs would fix some of that (plus it's better
  -- hardware practice)  
  -----------------------------------------------------------------------------
  INPUT_BUFFERS: process (clk)
  begin  -- process INPUT_BUFFERS
    if clk'event and clk = '1' then  -- rising clock edge
      if rst = '1' then                   -- synchronous reset (active high)
        enc_a_buf_1 <= '0';
        enc_b_buf_1 <= '0';
        enc_a_buf_2 <= '0';
        enc_b_buf_2 <= '0';
      else
        enc_a_buf_1 <= enc_a;
        enc_b_buf_1 <= enc_b;
        if enc_sample_en = '1' then
          enc_a_buf_2 <= enc_a_buf_1;
          enc_b_buf_2 <= enc_b_buf_1;
        end if;
      end if;
    end if;
  end process INPUT_BUFFERS;
  -----------------------------------------------------------------------------
  -- Encoder counter

  ENC_COUNT_REG_PROC : process (clk)
  begin
    if clk'event and clk='1' then
      if rst = '1' then
        enc_count_reg <= (others => '0');
      else
        if enc_count_clr = '1' then
          enc_count_reg <= (others => '0');
        elsif enc_count_inc = '1' then
          enc_count_reg <= enc_count_reg + 1;
        elsif enc_count_dec = '1' then
          enc_count_reg <= enc_count_reg - 1;
		  end if;
      end if;
    end if;
  end process ENC_COUNT_REG_PROC;

  
  -----------------------------------------------------------------------------
  -- State machine to determine motor direction based on encoder inputs

  ENC_FSM_REG : process (clk)
  begin
	if clk'event and clk='1' then
    if rst = '1' then
      enc_state <= ENC_STATE_1;
		enc_reg <= "00";
    else
      enc_state <= next_enc_state;
      enc_reg <= enc_a_buf_2 & enc_b_buf_2;
    end if;
   end if;
  end process ENC_FSM_REG;

  ENC_FSM_COMB : process (enc_state, enc_reg)
  begin
	enc_count_dec <= '0';
	enc_count_inc <= '0';
	next_enc_state <= enc_state;
    case enc_state is
      when ENC_STATE_1 =>
        if enc_reg = 2 then --a leads b = clockwise rotation (forward)
          enc_count_inc <= '1';
          next_enc_state <= ENC_STATE_2;
        elsif enc_reg = 1 then --b leads a = counterclockwise (reverse)
          enc_count_dec <= '1';
          next_enc_state <= ENC_STATE_2;
		  else
			next_enc_state <= ENC_STATE_1;
        end if;

      when ENC_STATE_2 =>
        if enc_reg = 0 then
			next_enc_state <= ENC_STATE_1;
		  else
			next_enc_state <= ENC_STATE_2;
        end if;
       
      when others =>
        next_enc_state <= ENC_STATE_1;
    end case;
  end process ENC_FSM_COMB;

  -- END USER LOGIC
  
  ------------------------------------------
  -- Example code to read/write user logic slave model s/w accessible registers
  -- 
  -- Note:
  -- The example code presented here is to show you one way of reading/writing
  -- software accessible registers implemented in the user logic slave model.
  -- Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  -- to one software accessible register by the top level template. For example,
  -- if you have four 32 bit software accessible registers in the user logic,
  -- you are basically operating on the following memory mapped registers:
  -- 
  --    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  --                     "1000"   C_BASEADDR + 0x0
  --                     "0100"   C_BASEADDR + 0x4
  --                     "0010"   C_BASEADDR + 0x8
  --                     "0001"   C_BASEADDR + 0xC
  -- 
  ------------------------------------------
  slv_reg_write_sel <= Bus2IP_WrCE(0 to 0);
  slv_reg_read_sel  <= Bus2IP_RdCE(0 to 0);
  slv_write_ack     <= Bus2IP_WrCE(0);
  slv_read_ack      <= Bus2IP_RdCE(0);

  -- implement slave model software accessible register(s)
--  SLAVE_REG_WRITE_PROC : process( Bus2IP_Clk ) is
--  begin
--
--    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
--      if Bus2IP_Reset = '1' then
--        slv_reg0 <= (others => '0');
--      else
--        case slv_reg_write_sel is
--          when "1" =>
--            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
--              if ( Bus2IP_BE(byte_index) = '1' ) then
--                slv_reg0(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
--              end if;
--            end loop;
--          when others => null;
--        end case;
--      end if;
--    end if;
--
--  end process SLAVE_REG_WRITE_PROC;

  -- implement slave model software accessible register(s) read mux
  SLAVE_REG_READ_PROC : process( slv_reg_read_sel, slv_reg0 ) is
  begin

    case slv_reg_read_sel is
      when "1" => slv_ip2bus_data <= slv_reg0;
      when others => slv_ip2bus_data <= (others => '0');
    end case;

  end process SLAVE_REG_READ_PROC;

  ------------------------------------------
  -- Example code to drive IP to Bus signals
  ------------------------------------------
  IP2Bus_Data  <= slv_ip2bus_data when slv_read_ack = '1' else
                  (others => '0');

  IP2Bus_WrAck <= slv_write_ack;
  IP2Bus_RdAck <= slv_read_ack;
  IP2Bus_Error <= '0';

end IMP;
