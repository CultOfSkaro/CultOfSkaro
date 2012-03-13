-------------------------------------------------------------------------------
-- 
-- FILE:    hsvsegmenter.vhd
-- AUTHOR:  Barrett Edwards
-- CREATED: 13 April 2007
--
-- DESCRIPTION:
--  This core performes HSV color segmentation. Input H,S,V values are compared
--  to a set of pre-defined thresholds (Hmax,Hmin,Smax,Smin,Vmax,Smin). If the
--  incoming H,S,V values are within these thresholds, then the output signal
--  seg is asserted high.
--
--  The thresholds are NOT hardcoded into the core. They must be set by an
--  external device. This is usually done by C code running on the PowerPC or
--  other hard/soft core processor. This is done my asserting the appropriate
--  data signal (huemaxmin or satmmvalmm) and then the appropriate write enable
--  signal (hue_we or sat_val_we).
--
-- Range:
--        HueMax        : 0-359 9-bit unsigned
--        HueMin        : 0-359 9-bit unsigned 
--        SaturationMax : 0-255 8-bit unsigned 
--        SaturationMin : 0-255 8-bit unsigned
--        ValueMax      : 0-255 8-bit unsigned
--        ValueMin      : 0-255 8-bit unsigned
--
--        H             : 0-359 9-bit unsgined
--        S             : 0-255 8-bit unsgined
--        V             : 0-255 8-bit unsgined
--        seg           : 0-1   1-bit boolean
--
-- The C_LATENCY generic controls weather core is genereate with no pipeline
-- stages (0) or with one pipeline stage (1). The characteristics of the two
-- configurations are listed below.
-- 
-- RESOURCES: for Virtex4 FX60 speed grade -11:
-- C_LATENCY = 0
--   Slices  : 61
--   FF      : 19 
--   LUTs    : 63
--   BRAMs   : 0
--   DSPs    : 0
--   maxfq   : 122.040 MHz (8.194ns combinational delay path) (Estimated)
--   latency : 0 cycles
--   
--
-- C_LATENCY = 1 
--   Slices  : 67
--   FF      : 57 
--   LUTs    : 63 
--   BRAMs   : 0
--   DSPs    : 0
--   maxfq   : 440.529 MHz (2.270ns period)
--   latency : 1 cycle
-------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

Library XilinxCoreLib;
Use XilinxCoreLib.c_compare_v8_0_comp.all;

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------

entity hsvsegmenter is
  generic (   
    C_LATENCY   : integer := 1  -- 0 or 1
  );
  port
  (
    clk         	: in  std_logic;
    reset       	: in  std_logic;
    ce                  : in  std_logic;
    
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
    
    seg         	: out std_logic;
    valid       	: out std_logic    
  );
end entity hsvsegmenter;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of hsvsegmenter is

  signal Hmax   	: std_logic_vector(8 downto 0);  
  signal Hmin   	: std_logic_vector(8 downto 0);
  signal Hwrap  	: std_logic;
  signal Smax   	: std_logic_vector(7 downto 0);
  signal Smin   	: std_logic_vector(7 downto 0);
  signal Vmax   	: std_logic_vector(7 downto 0);
  signal Vmin   	: std_logic_vector(7 downto 0);


  signal VleVmax        : std_logic;
  signal VgeVmin        : std_logic;
  signal SleSmax        : std_logic;
  signal SgeSmin        : std_logic;
  signal HleHmax        : std_logic;
  signal HgeHmin        : std_logic;

  signal satval_inrange : std_logic;
begin
    
------------------------------------------------------------------------------
-- Store Max and Min Values in registers
------------------------------------------------------------------------------        
  process(clk,hue_we,sat_val_we,HueMax,HueMin,SaturationMax,SaturationMin,ValueMax,ValueMin)
  begin
    if clk'event and clk = '1' then
      if (ce and hue_we) = '1' then
        Hmax <= HueMax;
        Hmin <= HueMin;

        if HueMax > HueMin then Hwrap <= '0';
        else                    Hwrap <= '1';
        end if;
      end if;
      if (ce and sat_val_we) = '1' then
        Smax <= SaturationMax;
        Smin <= SaturationMin;
        Vmax <= ValueMax;
        Vmin <= ValueMin;
      end if;       
    end if;
  end process; 

-------------------------------------------------------------------------------
-- Comparisons
--   V < Vmax
--   V > Vmin
--   S < Smax
--   S > Smin
--   H < Hmax
--   H > Hmin  
-------------------------------------------------------------------------------


IFLATENCY0: if C_LATENCY = 0 generate
    COMP_VLEVMAX: c_compare_v8_0
    generic map(
      C_WIDTH           => 8,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_LE_B     => 0, 
      C_HAS_A_LE_B      => 1)
    port map(
      a         => V,
      b         => Vmax,
      a_le_b    => VleVmax);

  COMP_VGEVMIN: c_compare_v8_0
    generic map(
      C_WIDTH           => 8,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_GE_B     => 0, 
      C_HAS_A_GE_B      => 1)
    port map(
      a         => V,
      b         => Vmin,
      a_ge_b    => VgeVmin);
  
  COMP_SLESMAX: c_compare_v8_0
    generic map(
      C_WIDTH           => 8,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_LE_B     => 0, 
      C_HAS_A_LE_B      => 1)
    port map(
      a         => S,
      b         => Smax,
      a_le_b    => SleSmax);

  COMP_SGESMIN: c_compare_v8_0
    generic map(
      C_WIDTH           => 8,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_GE_B     => 0, 
      C_HAS_A_GE_B      => 1)
    port map(
      a         => S,
      b         => Smin,
      a_ge_b    => SgeSmin);
    
  COMP_HLEHMAX: c_compare_v8_0
    generic map(
      C_WIDTH           => 9,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_LE_B     => 0, 
      C_HAS_A_LE_B      => 1)
    port map(
      a         => H,
      b         => Hmax,
      a_le_b    => HleHmax);

  COMP_HGEHMIN: c_compare_v8_0
    generic map(
      C_WIDTH           => 9,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_GE_B     => 0, 
      C_HAS_A_GE_B      => 1)
    port map(
      a         => H,
      b         => Hmin,      
      a_ge_b    => HgeHmin);  
end generate IFLATENCY0;
-------------------------------------------------------------------------------
IFLATENCY1: if C_LATENCY = 1 generate
  COMP_VLEVMAX: c_compare_v8_0
    generic map(
      C_WIDTH           => 8,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_LE_B     => 1, 
      C_HAS_A_LE_B      => 0)
    port map(
      a         => V,
      b         => Vmax,
      clk       => clk,
      qa_le_b   => VleVmax);

  COMP_VGEVMIN: c_compare_v8_0
    generic map(
      C_WIDTH           => 8,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_GE_B     => 1, 
      C_HAS_A_GE_B      => 0)
    port map(
      a         => V,
      b         => Vmin,
      clk       => clk,
      qa_ge_b   => VgeVmin);
  
  COMP_SLESMAX: c_compare_v8_0
    generic map(
      C_WIDTH           => 8,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_LE_B     => 1, 
      C_HAS_A_LE_B      => 0)
    port map(
      a         => S,
      b         => Smax,
      clk       => clk,
      qa_le_b   => SleSmax);

  COMP_SGESMIN: c_compare_v8_0
    generic map(
      C_WIDTH           => 8,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_GE_B     => 1, 
      C_HAS_A_GE_B      => 0)
    port map(
      a         => S,
      b         => Smin,
      clk       => clk,
      qa_ge_b   => SgeSmin);
    
  COMP_HLEHMAX: c_compare_v8_0
    generic map(
      C_WIDTH           => 9,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_LE_B     => 1, 
      C_HAS_A_LE_B      => 0)
    port map(
      a         => H,
      b         => Hmax,
      clk       => clk,
      qa_le_b   => HleHmax);

  COMP_HGEHMIN: c_compare_v8_0
    generic map(
      C_WIDTH           => 9,     
      C_DATA_TYPE       => 1, --1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, --Must disable default equation 
      C_HAS_QA_GE_B     => 1, 
      C_HAS_A_GE_B      => 0)
    port map(
      a         => H,
      b         => Hmin,
      clk       => clk,
      qa_ge_b   => HgeHmin);  
end generate IFLATENCY1;
            

-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------            
  satval_inrange <= VleVmax and VgeVmin and SleSmax and SgeSmin;
  seg            <= satval_inrange and (HleHmax or  HgeHmin) when Hwrap = '1' else
                    satval_inrange and (HleHmax and HgeHmin);



-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------
IFLATENCY_0: if C_LATENCY = 0 generate
  -- Do not delay the valid signal
  valid <= en;
  
end generate IFLATENCY_0;
 ------------------------------------------------------------------------------            
IFLATENCY_1: if C_LATENCY = 1 generate
   
  -- Delay valid signal by one cycle 
  process(clk,reset,en)
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        valid <= '0';
      else
        valid <= en;
      end if;       
    end if;
  end process;
  
end generate IFLATENCY_1;


  
-------------------------------------------------------------------------------
-- Original Implementation
-------------------------------------------------------------------------------
--  process(H,S,V,Hmax,Hmin,Smax,Smin,Vmax,Vmin)
--  begin
--    seg <= '0';
   
--    if Vmax > unsigned(V) and unsigned(V) > Vmin then
--      if Smax > unsigned(S) and unsigned(S) > Smin then       
--        if Hmax > Hmin then         
--          if Hmax > unsigned(H) and unsigned(H) > Hmin then
--            seg <= '1';
--          end if;
--        else
--          if Hmax > unsigned(H) or  unsigned(H) > Hmin then
--            seg <= '1';
--          end if;
--        end if;       
--      end if;
--    end if;
--  end process;

 
end IMP;
