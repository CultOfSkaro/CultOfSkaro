-------------------------------------------------------------------------------
-- 
-- FILE:    rgb2grayscale.vhd
-- AUTHOR:  Barrett Edwards
-- CREATED: 13 April 2007
--
-- DESCRIPTION:
-- This module takes a RGB pixel input and converts it to (Y) grayscale
-- Range: R : 0-255 8-bit unsgined
--        G : 0-255 8-bit unsgined
--        B : 0-255 8-bit unsgined
--        Y : 0-360 9-bit unsgined
--        
--
-- Equation: 
-- Y = 0.3R + 0.59G + 0.11B    : Floating point
-- Y = (77R + 152G  + 28B)/256 : Fixed point
--
-- The second equation is implemented in a ?? stage pipeline.
-- The computations of each stage are listed as follows:
--  Stage0: Stores the input RGB values into a register. This stage is included
--          to aid in helping the core make timing. This stage can be
--          execluded if desired.
--
--  Stage1: Computes the multiplies:    77*R
--                                      152*G
--                                      28*B
--          This stage curerntly uses DSP slices for maximum performance. But
--          LUT multipliers could also be used.
--
--  Stage2: Adds the scaled RG  channels together
--  
--  Stage3: Adds the scaled RGB channels together

--          
--  RESOURCES: for Virtex4 FX60 speed grade -11:
--             The reduced Maximum Frequency on the DSP version is due to the
--             extra time required to get a value out of the register of the
--             DSP slice. This is due to restricted signal routing. If an
--             additional register stage was added to the core, then the DSP
--             version could run faster. But this would add unnecessary slices
--             to the core.
--
-- C_USE_DSP = true
--   Slices  : 31
--   FF      : 58
--   LUTs    : 51
--   BRAMs   : 0
--   DSPs    : 3
--   maxfq   : 201.898 MHz (4.593ns period)
--   latency : 4 cycles
--   
--
-- C_USE_DSP = false
--   Slices  : 190 
--   FF      : 358 
--   LUTs    : 288
--   BRAMs   : 0
--   DSPs    : 0
--   maxfq   : 376.506 MHz (2.656ns period)
--   latency : 6 cycles
--
--  ACCURACY:
--   This core is not *perfectly* accurate so there is a little error noise. To
--   quantify this error in the calculated grayscale values, all possible RGB
--   values were passed through this core. *All Possible* is defined as all the
--   RGB combinations that are possible using the RGB565 format. The true gray
--   values of these RGB565 input values as calcualted in matlab using:
--
--                            gray = [.3 .59 .11]*[r g b]';
--                            
--   were subtracted from the resulting HSV values that were calculated using
--   this core.
--
--                    difference = core_calculated_gray - true_gray
--                    
--   The resulting difference values had these characteristics:
--                   mean       std      min_error    max_error
--   grayscale :    0.0075    0.4062      -1.0800      1.1200
--
-- 
-- 
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity rgb2grayscale is
  generic (
    C_USE_DSP : boolean := false
  );
  port (
    clk         : in  std_logic;
    reset       : in  std_logic;

    R           : in  std_logic_vector( 7 downto 0);
    G           : in  std_logic_vector( 7 downto 0);
    B           : in  std_logic_vector( 7 downto 0);
    en          : in  std_logic;

    Y8          : out std_logic_vector( 7 downto 0);
    Y16         : out std_logic_vector(15 downto 0);    
    valid       : out std_logic);

end rgb2grayscale;

architecture behavioral of rgb2grayscale is
-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------
  component dspmult8_reginout_aubu_en
    port (
      clk  : IN  std_logic;
      a    : IN  std_logic_VECTOR(7 downto 0);
      b    : IN  std_logic_VECTOR(7 downto 0);
      q    : OUT std_logic_VECTOR(15 downto 0);
      sclr : IN  std_logic;
      nd   : IN  std_logic;
      rdy  : OUT std_logic);
  end component;

  component lutmult8_reginout_aubu_en
    port (
      clk  : IN  std_logic;
      a    : IN  std_logic_VECTOR(7 downto 0);
      b    : IN  std_logic_VECTOR(7 downto 0);
      q    : OUT std_logic_VECTOR(15 downto 0);
      sclr : IN  std_logic;
      nd   : IN  std_logic;
      rdy  : OUT std_logic);
  end component;
  
-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------  
  signal multR                          : std_logic_vector(15 downto 0);
  signal multG                          : std_logic_vector(15 downto 0);
  signal multB                          : std_logic_vector(15 downto 0);
  signal stage1                         : std_logic;

  signal stage2                         : std_logic;
  signal stage2_Y                       : std_logic_vector(16 downto 0);
  signal stage2_multB                   : std_logic_vector(15 downto 0);
  
  signal stage3                         : std_logic;
  signal stage3_Y                       : std_logic_vector(16 downto 0);
begin

-------------------------------------------------------------------------------
-- Stage 1
-------------------------------------------------------------------------------


IG: if C_USE_DSP = true generate
  MULTIPLIER_R: dspmult8_reginout_aubu_en
    port map (
        clk  => clk,
        a    => std_logic_vector(to_unsigned(77,8)),
        b    => R,
        q    => multR,
        sclr => reset,
        nd   => en,
        rdy  => stage1);

  MULTIPLIER_G: dspmult8_reginout_aubu_en
    port map (
        clk  => clk,
        a    => std_logic_vector(to_unsigned(152,8)),
        b    => G,
        q    => multG,
        sclr => reset,
        nd   => en,
        rdy  => open);

  MULTIPLIER_B: dspmult8_reginout_aubu_en
    port map (
        clk  => clk,
        a    => std_logic_vector(to_unsigned(28,8)),
        b    => b,
        q    => multb,
        sclr => reset,
        nd   => en,
        rdy  => open);
end generate IG;
-------------------------------------------------------------------------------
IGN: if C_USE_DSP = false generate
  MULTIPLIER_R: lutmult8_reginout_aubu_en
    port map (
        clk  => clk,
        a    => std_logic_vector(to_unsigned(77,8)),
        b    => R,
        q    => multR,
        sclr => reset,
        nd   => en,
        rdy  => stage1);

  MULTIPLIER_G: lutmult8_reginout_aubu_en
    port map (
        clk  => clk,
        a    => std_logic_vector(to_unsigned(152,8)),
        b    => G,
        q    => multG,
        sclr => reset,
        nd   => en,
        rdy  => open);

  MULTIPLIER_B: lutmult8_reginout_aubu_en
    port map (
        clk  => clk,
        a    => std_logic_vector(to_unsigned(28,8)),
        b    => b,
        q    => multb,
        sclr => reset,
        nd   => en,
        rdy  => open); 
end generate IGN;
  
  


 
-------------------------------------------------------------------------------
-- Stage 2
-------------------------------------------------------------------------------

  process(clk,reset,stage1,multR,multG,multB)
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        stage2          <= '0';                
      else
        stage2          <= stage1;
        stage2_Y        <= std_logic_vector(   resize(unsigned(multR),17)
                                             + resize(unsigned(multG),17));
        stage2_multB    <= multB;
      end if; 
    end if;
  end process;

  
-------------------------------------------------------------------------------
-- Stage 3
-------------------------------------------------------------------------------

  process(clk,reset,stage2,stage2_Y,stage2_multB)
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        stage3          <= '0';                
      else
        stage3          <= stage2;
        stage3_Y        <= std_logic_vector(   unsigned(stage2_Y)
                                             + resize(unsigned(stage2_multB),17));
      end if; 
    end if;
  end process;


  

-----------------------------------------------------------------------------
-- Output assignment
-----------------------------------------------------------------------------

  valid <= stage3;
  
  Y8     <= stage3_Y(15 downto 8) when stage3_Y(stage3_Y'left) = '0' else
           (others => '1');
  
  Y16    <= stage3_Y(15 downto 0) when stage3_Y(stage3_Y'left) = '0' else
           (others => '1');



  

  
end behavioral;



 
