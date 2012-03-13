-------------------------------------------------------------------------------
-- plb_bram_if_cntlr_1_bram_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library plb_bram_if_cntlr_1_bram_elaborate_v1_00_a;
use plb_bram_if_cntlr_1_bram_elaborate_v1_00_a.all;

entity plb_bram_if_cntlr_1_bram_wrapper is
  port (
    BRAM_Rst_A : in std_logic;
    BRAM_Clk_A : in std_logic;
    BRAM_EN_A : in std_logic;
    BRAM_WEN_A : in std_logic_vector(0 to 7);
    BRAM_Addr_A : in std_logic_vector(0 to 31);
    BRAM_Din_A : out std_logic_vector(0 to 63);
    BRAM_Dout_A : in std_logic_vector(0 to 63);
    BRAM_Rst_B : in std_logic;
    BRAM_Clk_B : in std_logic;
    BRAM_EN_B : in std_logic;
    BRAM_WEN_B : in std_logic_vector(0 to 7);
    BRAM_Addr_B : in std_logic_vector(0 to 31);
    BRAM_Din_B : out std_logic_vector(0 to 63);
    BRAM_Dout_B : in std_logic_vector(0 to 63)
  );

  attribute x_core_info : STRING;
  attribute keep_hierarchy : STRING;
  attribute x_core_info of plb_bram_if_cntlr_1_bram_wrapper : entity is "plb_bram_if_cntlr_1_bram_elaborate_v1_00_a";
  attribute keep_hierarchy of plb_bram_if_cntlr_1_bram_wrapper : entity is "yes";

end plb_bram_if_cntlr_1_bram_wrapper;

architecture STRUCTURE of plb_bram_if_cntlr_1_bram_wrapper is

  component plb_bram_if_cntlr_1_bram_elaborate is
    generic (
      C_MEMSIZE : integer;
      C_PORT_DWIDTH : integer;
      C_PORT_AWIDTH : integer;
      C_NUM_WE : integer;
      C_FAMILY : string
    );
    port (
      BRAM_Rst_A : in std_logic;
      BRAM_Clk_A : in std_logic;
      BRAM_EN_A : in std_logic;
      BRAM_WEN_A : in std_logic_vector(0 to C_NUM_WE-1);
      BRAM_Addr_A : in std_logic_vector(0 to C_PORT_AWIDTH-1);
      BRAM_Din_A : out std_logic_vector(0 to C_PORT_DWIDTH-1);
      BRAM_Dout_A : in std_logic_vector(0 to C_PORT_DWIDTH-1);
      BRAM_Rst_B : in std_logic;
      BRAM_Clk_B : in std_logic;
      BRAM_EN_B : in std_logic;
      BRAM_WEN_B : in std_logic_vector(0 to C_NUM_WE-1);
      BRAM_Addr_B : in std_logic_vector(0 to C_PORT_AWIDTH-1);
      BRAM_Din_B : out std_logic_vector(0 to C_PORT_DWIDTH-1);
      BRAM_Dout_B : in std_logic_vector(0 to C_PORT_DWIDTH-1)
    );
  end component;

begin

  plb_bram_if_cntlr_1_bram : plb_bram_if_cntlr_1_bram_elaborate
    generic map (
      C_MEMSIZE => 16#20000#,
      C_PORT_DWIDTH => 64,
      C_PORT_AWIDTH => 32,
      C_NUM_WE => 8,
      C_FAMILY => "virtex4"
    )
    port map (
      BRAM_Rst_A => BRAM_Rst_A,
      BRAM_Clk_A => BRAM_Clk_A,
      BRAM_EN_A => BRAM_EN_A,
      BRAM_WEN_A => BRAM_WEN_A,
      BRAM_Addr_A => BRAM_Addr_A,
      BRAM_Din_A => BRAM_Din_A,
      BRAM_Dout_A => BRAM_Dout_A,
      BRAM_Rst_B => BRAM_Rst_B,
      BRAM_Clk_B => BRAM_Clk_B,
      BRAM_EN_B => BRAM_EN_B,
      BRAM_WEN_B => BRAM_WEN_B,
      BRAM_Addr_B => BRAM_Addr_B,
      BRAM_Din_B => BRAM_Din_B,
      BRAM_Dout_B => BRAM_Dout_B
    );

end architecture STRUCTURE;

