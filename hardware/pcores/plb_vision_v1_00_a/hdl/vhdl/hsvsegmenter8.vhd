-------------------------------------------------------------------------------
-- 
-- FILE:    hsvsegmenter8.vhd
-- AUTHOR:  Barrett Edwards
-- CREATED: 13 April 2007
--
-- DESCRIPTION:
--  This core is a wrapper around 1 to 8 hsvsegmentation cores.
--  The sel signal selects which of the cores should be written to when
--  threshold values are being written to the core. sel="000" implies that the
--  first hsvsegmenter is being wiriten to when the hsv_we or sat_val_we are
--  asserted high.
-- 
-- RESOURCES: for Virtex4 FX60 speed grade -11: (8 hsvsegmenters)
-- C_LATENCY = 0
--   Slices  : 492
--   FF      : 408
--   LUTs    : 504
--   BRAMs   : 0
--   DSPs    : 0
--   maxfq   : 118.891 MHz (8.411ns combinational delay) (Estimated)
--   latency : 0 cycles
--   
--
-- C_LATENCY = 1 
--   Slices  : 541
--   FF      : 456 
--   LUTs    : 504
--   BRAMs   : 0
--   DSPs    : 0
--   maxfq   : 440.529 MHz (1.170ns period)
--   latency : 1 cycle
-------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------

entity hsvsegmenter8 is
  generic (
    C_NUM_SEGMENTERS    : integer := 8; -- 1 to 8 
    C_LATENCY           : integer := 1  -- 0 or 1
  );
  port
  (
    clk         	: in  std_logic;
    reset       	: in  std_logic;

    sel                 : in  std_logic_vector( 2 downto 0);
    
    hue_we      	: in  std_logic;
    HueMax      	: in  std_logic_vector( 8 downto 0);
    HueMin      	: in  std_logic_vector( 8 downto 0);
    
    sat_val_we          : in  std_logic;
    SaturationMax       : in  std_logic_vector( 7 downto 0);
    SaturationMin       : in  std_logic_vector( 7 downto 0);
    ValueMax            : in  std_logic_vector( 7 downto 0);
    ValueMin            : in  std_logic_vector( 7 downto 0);

    H           	: in  std_logic_vector( 8 downto 0);
    S           	: in  std_logic_vector( 7 downto 0);
    V           	: in  std_logic_vector( 7 downto 0);
    en          	: in  std_logic;
    
    seg         	: out std_logic_vector(C_NUM_SEGMENTERS-1 downto 0);
    valid       	: out std_logic    
  );
end entity hsvsegmenter8;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of hsvsegmenter8 is

-------------------------------------------------------------------------------
-- Components
-------------------------------------------------------------------------------  
  component hsvsegmenter
    generic (
      C_LATENCY : integer);
    port (
      clk           : in  std_logic;
      reset         : in  std_logic; 
      ce            : in  std_logic; 
      hue_we        : in  std_logic;
      HueMax        : in  std_logic_vector( 8 downto 0);
      HueMin        : in  std_logic_vector( 8 downto 0);
      sat_val_we    : in  std_logic;
      SaturationMax : in  std_logic_vector( 7 downto 0);
      SaturationMin : in  std_logic_vector( 7 downto 0);
      ValueMax      : in  std_logic_vector( 7 downto 0);
      ValueMin      : in  std_logic_vector( 7 downto 0);
      H             : in  std_logic_vector( 8 downto 0);
      S             : in  std_logic_vector( 7 downto 0);
      V             : in  std_logic_vector( 7 downto 0);
      en            : in  std_logic;
      seg           : out std_logic;
      valid         : out std_logic);
  end component;
  
-------------------------------------------------------------------------------
-- Signals
-------------------------------------------------------------------------------
  signal sel8bit    : std_logic_vector(7 downto 0);


-------------------------------------------------------------------------------
-- Implementation 
-------------------------------------------------------------------------------      
begin

  process(sel)
  begin 
      case sel is
        when "000" => sel8bit <= "00000001";
        when "001" => sel8bit <= "00000010";
        when "010" => sel8bit <= "00000100";
        when "011" => sel8bit <= "00001000";
        when "100" => sel8bit <= "00010000";
        when "101" => sel8bit <= "00100000";
        when "110" => sel8bit <= "01000000";
        when "111" => sel8bit <= "10000000";
        when others => null;
      end case;
  end process;


   SEG0: hsvsegmenter
    generic map (
        C_LATENCY => C_LATENCY)
    port map (
        clk           => clk,
        reset         => reset,
        ce            => sel8bit(0),
        hue_we        => hue_we,
        HueMax        => HueMax,
        HueMin        => HueMin,
        sat_val_we    => sat_val_we,
        SaturationMax => SaturationMax,
        SaturationMin => SaturationMin,
        ValueMax      => ValueMax,
        ValueMin      => ValueMin,
        H             => H,
        S             => S,
        V             => V,
        en            => en,
        seg           => seg(0),
        valid         => valid);

GEN: for i in 1 to C_NUM_SEGMENTERS-1 generate
   SEGi: hsvsegmenter
    generic map (
        C_LATENCY => C_LATENCY)
    port map (
        clk           => clk,
        reset         => reset,
        ce            => sel8bit(i),
        hue_we        => hue_we,
        HueMax        => HueMax,
        HueMin        => HueMin,
        sat_val_we    => sat_val_we,
        SaturationMax => SaturationMax,
        SaturationMin => SaturationMin,
        ValueMax      => ValueMax,
        ValueMin      => ValueMin,
        H             => H,
        S             => S,
        V             => V,
        en            => en,
        seg           => seg(i),
        valid         => open);
end generate GEN;
     
end IMP;
