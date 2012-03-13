-------------------------------------------------------------------------------
-- 
-- FILE:                HarrisFeatureFilter.vhd
-- AUTHOR:              Barrett Edwards
-- CREATED:             19 April 2007
-- ORGINIZATION:        Brigham Young University
--                      Dept. Electrical Engineering
--                      Robot Vision Lab
--
-- DESCRIPTION:
--  The purpose of this core is to filer the resulting Harris feature values
--  that have been compute by a Harris Feature Detector core.
--
--  The resuling Harris strength value (signal: h) is input to this core and 5
--  comparisions are made to determine if it is a valid harris feature that
--  cane be passed on. The first comparison is to determin if the Harris
--  strength value h is greater than a minimum threshold that is asserted on
--  the input pin min_h. The remaining 4 comparisons are to determine if the
--  x,y location in the image is far enough away from the edges of the image.
--
--  There are two main reasons why features cannot be too close to the edge of
--  the image. The first is that the Harris Feature Detector core uses a 3x3 or
--  5x5 kernel centered around each pixel in the image to compute the harris
--  values. When the kernel extends past the edge of the image, then the
--  resulting harris value is undefined (garbage). So it cannot be used. The
--  second reason why the harris features cannot be too close to the edge of
--  the image is for tracking reasons. A correlator core attempts to track
--  Harris  features by searching in a large window possibly 32x64 pixels in
--  size. So the features that we track cannot be too close to the edge or else
--  we will search off the image, which will produce undefined results.
--
-- 
--
-- GENERICS:
--  C_IMAGE_WIDTH       : specifies the width of the image
--  C_IMAGE_HEIGHT      : specifies the height of the image 
--  C_XWIDTH            : specifies the minimum number of bits required to represent
--                        the x coordinate value (i.e. 639 requires 10 bits)
--  C_YWIDTH            : specifies the minimum number of bits required to represent
--                        the y coordinate value (i.e. 479 requires 9 bits)
--  C_MINX              : The minimum x coordinate value that can be accepted
--  C_MINY              : The minimum y coordinate value that can be accepted
--  C_MAXX              : The maximum x coordinate value that can be accepted
--  C_MAXY              : The maximum y coordinate value that can be accepted
--  C_BUFFER_SHIFT_X    : This is the number of pixels that should be
--                        subtracted off the current x coordinate position to
--                        account for the 3x3 or 5x5 kernel shift in the Harris
--                        Feature Detector Core. 
--  C_BUFFER_SHIFT_Y    : This is the number of pixels that should be
--                        subtracted off the current y coordinate position to
--                        account for the 3x3 or 5x5 kernel shift in the Harris
--                        Feature Detector Core.
--
--
-- 
-- SIGNALS: INPUTS
--  min_h               : The minimum threshold that a harris strength must be
--                        greater than to be passed by this filter
--  h                   : The computed Harris Feature Strength value for the
--                        current pixel
--  en                  : Asserted HIGH when the harris value h is valid
--  
-- SIGNALS: OUTPUTS
--  dout_h              : The Harris Feature Strength value that has passed
--                        this filter 
--  dout_x              : The x coordinate of the harris feature that has
--                        passed this filter 
--  dout_y              : The y coordinate of the harris feature that has
--                        passed this filter 
--  dout_valid          : Asserted HIGH when the data out signals are valid
-- 

-- RESOURCES: for Xilinx Virtex4 FX60 speed grade -11:
--   Slices: 56
--   FF    : 39
--   LUTs  : 103
--   BRAMs : 0 
--   DSPs  : 0  
--   maxfq : 250.313 MHz (3.995ns period)
-------------------------------------------------------------------------------


-------------------------------------------------------------------------------
-- Libraries 
-------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

Library XilinxCoreLib;
Use XilinxCoreLib.c_compare_v8_0_comp.all;

-------------------------------------------------------------------------------
-- Entity 
-------------------------------------------------------------------------------
entity HarrisFeatureFilter is
  generic
  (
    C_IMAGE_WIDTH       : integer       := 640;
    C_IMAGE_HEIGHT      : integer       := 480;
    
    C_MINX              : string        := X"0023";  -- 35
    C_MINY              : string        := X"0010";  -- 16
    C_MAXX              : string        := X"025D"; -- 640-35=605= X"025D"
    C_MAXY              : string        := X"1D0";  -- 480-16=464= X"1D0"    

    C_BUFFER_SHIFT_X    : integer       := 1;
    C_BUFFER_SHIFT_Y    : integer       := 1;
    
    C_XWIDTH            : integer       := 10;
    C_YWIDTH            : integer       := 9
  );
  port
  (
    clk         : in  std_logic;
    reset       : in  std_logic;

    min_h       : in std_logic_vector(15 downto 0);
    h           : in std_logic_vector(15 downto 0);    
    en          : in  std_logic;

    dout_h      : out std_logic_vector(15 downto 0);
    dout_x      : out std_logic_vector(C_XWIDTH-1 downto 0);
    dout_y      : out std_logic_vector(C_YWIDTH-1 downto 0);
    dout_valid  : out std_logic
   );
end entity HarrisFeatureFilter;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------
architecture IMP of HarrisFeatureFilter is
-------------------------------------------------------------------------------
-- Components
-------------------------------------------------------------------------------
  component xycounter
    generic (
      C_IMAGE_WIDTH  : integer;
      C_IMAGE_HEIGHT : integer;
      C_XWIDTH       : integer;
      C_YWIDTH       : integer);
    port (
      clk   : in  std_logic;
      reset : in  std_logic;
      en    : in  std_logic;
      x     : out std_logic_vector(C_XWIDTH-1 downto 0);
      y     : out std_logic_vector(C_YWIDTH-1 downto 0));
  end component;

-------------------------------------------------------------------------------
-- Functions
-------------------------------------------------------------------------------
  
-------------------------------------------------------------------------------
-- Signals
-------------------------------------------------------------------------------  
  signal x              : std_logic_vector(C_XWIDTH-1 downto 0);
  signal y              : std_logic_vector(C_YWIDTH-1 downto 0);

  signal bool1          : std_logic;
  signal bool2          : std_logic;
  signal bool3          : std_logic;
  signal bool4          : std_logic;
  signal bool5          : std_logic;
  signal valid          : std_logic;
      
begin

  -----------------------------------------------------------------------------
  -- Comparisons 
  -----------------------------------------------------------------------------

  COMPARE_MIN_HARRIS: c_compare_v8_0
    generic map(
      C_WIDTH           => 16,     
      C_DATA_TYPE       => 1, -- 1:UNSIGNED 0:SIGNED
      C_HAS_A_EQ_B      => 0, -- Must disable default equation 
      C_HAS_QA_GT_B     => 0, 
      C_HAS_A_GT_B      => 1)
    port map(
      a         => h,
      b         => min_h,
      a_gt_b    => bool1);

  COMPARE_LEFTX: c_compare_v8_0
    generic map(
      C_WIDTH           => C_XWIDTH,
      C_DATA_TYPE       => 1, -- 1:UNSIGNED 0:SIGNED
      C_B_CONSTANT      => 1,
      C_B_VALUE         => C_MINX,
      C_HAS_A_EQ_B      => 0, -- Must disable default equation
      C_HAS_QA_GT_B     => 0, 
      C_HAS_A_GT_B      => 1)
    port map(
      a         => x,
      a_gt_b    => bool2);

  
  COMPARE_RIGHTX: c_compare_v8_0
    generic map(
      C_WIDTH           => C_XWIDTH,
      C_DATA_TYPE       => 1, -- 1:UNSIGNED 0:SIGNED
      C_B_CONSTANT      => 1,
      C_B_VALUE         => C_MAXX,
      C_HAS_A_EQ_B      => 0, -- Must disable default equation
      C_HAS_QA_LT_B     => 0, 
      C_HAS_A_LT_B      => 1)
    port map(
      a         => x,
      a_lt_b    => bool3);  
  

  COMPARE_TOPY: c_compare_v8_0
    generic map(
      C_WIDTH           => C_YWIDTH,
      C_DATA_TYPE       => 1, -- 1:UNSIGNED 0:SIGNED
      C_B_CONSTANT      => 1,
      C_B_VALUE         => C_MINY,
      C_HAS_A_EQ_B      => 0, -- Must disable default equation
      C_HAS_QA_GT_B     => 0, 
      C_HAS_A_GT_B      => 1)
    port map(
      a         => y,
      a_gt_b    => bool4);

  
  COMPARE_BOTTOMY: c_compare_v8_0
    generic map(
      C_WIDTH           => C_YWIDTH,
      C_DATA_TYPE       => 1, -- 1:UNSIGNED 0:SIGNED
      C_B_CONSTANT      => 1,
      C_B_VALUE         => C_MAXY,
      C_HAS_A_EQ_B      => 0, -- Must disable default equation
      C_HAS_QA_LT_B     => 0, 
      C_HAS_A_LT_B      => 1)
    port map(
      a         => y,
      a_lt_b    => bool5);  
  
  valid <= en and bool1 and bool2 and bool3 and bool4 and bool5;

  -----------------------------------------------------------------------------
  -- Adjustment
  -----------------------------------------------------------------------------
  process(clk,reset,h,x,y,valid)
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        dout_valid      <= '0';
      else
        dout_h          <= h;
        dout_x          <= std_logic_vector(unsigned(x) - C_BUFFER_SHIFT_X);-- correct for buffer shift
        dout_y          <= std_logic_vector(unsigned(y) - C_BUFFER_SHIFT_Y);-- correct for buffer shift               
        dout_valid      <= valid;
      end if;--rst
    end if;--clk
  end process;  


  -----------------------------------------------------------------------------
  -- xycounter for internal tracking of current location in image
  -----------------------------------------------------------------------------
  XYCOUNTER_I: xycounter
    generic map (
      C_IMAGE_WIDTH  => C_IMAGE_WIDTH,
      C_IMAGE_HEIGHT => C_IMAGE_HEIGHT,
      C_XWIDTH       => C_XWIDTH,
      C_YWIDTH       => C_YWIDTH)
    port map (
      clk   => clk,
      reset => reset,
      en    => en,
      x     => x,
      y     => y);
    
end IMP;



