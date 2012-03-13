-------------------------------------------------------------------------------
-- jtagppc_cntlr_inst_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library jtagppc_cntlr_v2_01_c;
use jtagppc_cntlr_v2_01_c.all;

entity jtagppc_cntlr_inst_wrapper is
  port (
    TRSTNEG : in std_logic;
    HALTNEG0 : in std_logic;
    DBGC405DEBUGHALT0 : out std_logic;
    HALTNEG1 : in std_logic;
    DBGC405DEBUGHALT1 : out std_logic;
    C405JTGTDO0 : in std_logic;
    C405JTGTDOEN0 : in std_logic;
    JTGC405TCK0 : out std_logic;
    JTGC405TDI0 : out std_logic;
    JTGC405TMS0 : out std_logic;
    JTGC405TRSTNEG0 : out std_logic;
    C405JTGTDO1 : in std_logic;
    C405JTGTDOEN1 : in std_logic;
    JTGC405TCK1 : out std_logic;
    JTGC405TDI1 : out std_logic;
    JTGC405TMS1 : out std_logic;
    JTGC405TRSTNEG1 : out std_logic
  );

  attribute x_core_info : STRING;
  attribute x_core_info of jtagppc_cntlr_inst_wrapper : entity is "jtagppc_cntlr_v2_01_c";

end jtagppc_cntlr_inst_wrapper;

architecture STRUCTURE of jtagppc_cntlr_inst_wrapper is

  component jtagppc_cntlr is
    generic (
      C_DEVICE : string;
      C_NUM_PPC_USED : integer
    );
    port (
      TRSTNEG : in std_logic;
      HALTNEG0 : in std_logic;
      DBGC405DEBUGHALT0 : out std_logic;
      HALTNEG1 : in std_logic;
      DBGC405DEBUGHALT1 : out std_logic;
      C405JTGTDO0 : in std_logic;
      C405JTGTDOEN0 : in std_logic;
      JTGC405TCK0 : out std_logic;
      JTGC405TDI0 : out std_logic;
      JTGC405TMS0 : out std_logic;
      JTGC405TRSTNEG0 : out std_logic;
      C405JTGTDO1 : in std_logic;
      C405JTGTDOEN1 : in std_logic;
      JTGC405TCK1 : out std_logic;
      JTGC405TDI1 : out std_logic;
      JTGC405TMS1 : out std_logic;
      JTGC405TRSTNEG1 : out std_logic
    );
  end component;

begin

  jtagppc_cntlr_inst : jtagppc_cntlr
    generic map (
      C_DEVICE => "4vfx60",
      C_NUM_PPC_USED => 2
    )
    port map (
      TRSTNEG => TRSTNEG,
      HALTNEG0 => HALTNEG0,
      DBGC405DEBUGHALT0 => DBGC405DEBUGHALT0,
      HALTNEG1 => HALTNEG1,
      DBGC405DEBUGHALT1 => DBGC405DEBUGHALT1,
      C405JTGTDO0 => C405JTGTDO0,
      C405JTGTDOEN0 => C405JTGTDOEN0,
      JTGC405TCK0 => JTGC405TCK0,
      JTGC405TDI0 => JTGC405TDI0,
      JTGC405TMS0 => JTGC405TMS0,
      JTGC405TRSTNEG0 => JTGC405TRSTNEG0,
      C405JTGTDO1 => C405JTGTDO1,
      C405JTGTDOEN1 => C405JTGTDOEN1,
      JTGC405TCK1 => JTGC405TCK1,
      JTGC405TDI1 => JTGC405TDI1,
      JTGC405TMS1 => JTGC405TMS1,
      JTGC405TRSTNEG1 => JTGC405TRSTNEG1
    );

end architecture STRUCTURE;

