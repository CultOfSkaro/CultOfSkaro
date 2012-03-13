-------------------------------------------------------------------------------
-- 
-- FILE:                StreakFinder8.vhd
-- AUTHOR:              Barrett Edwards
-- CREATED:             18 April 2007
-- ORGINIZATION:        Brigham Young University
--                      Dept. Electrical Engineering
--                      Robot Vision Lab
--
-- DESCRIPTION:
--
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
--  C_STREAK_MIN_WIDTH  : specifies the minimum width that a streak must be to
--                        be considered a streak. The minimum value that this
--                        should ever take is 2. This functinality is
--                        implemented in a shift register that is
--                        C_STREAK_MIN_WIDTH bits wide, so this width should
--                        not be very large values (i.e. not grater than 10 or
--                        so).
--
-- SIGNALS: INPUTS 
--  
-- SIGNALS: OUTPUTS
--  
-- 

-- RESOURCES: for Xilinx Virtex4 FX60 speed grade -11:
--   C_NUM_STREAK_FINDERS : 1
--   Slices: 65
--   FF    : 53
--   LUTs  : 122
--   BRAMs : 0  
--   DSPs  : 0  
--   maxfq : 264.690 MHz (3.778ns period)
--
--   C_NUM_STREAK_FINDERS : 4
--   Slices: 137
--   FF    : 158
--   LUTs  : 250
--   BRAMs : 0  
--   DSPs  : 0  
--   maxfq : 250.313 MHz (3.995ns period)
--
--   C_NUM_STREAK_FINDERS : 8
--   Slices: 234
--   FF    : 299
--   LUTs  : 422 
--   BRAMs : 0  
--   DSPs  : 0  
--   maxfq : 246.063 MHz (4.064ns period)
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity StreakFinder8 is
  generic
  (
    C_IMAGE_WIDTH       : integer       := 640;
    C_IMAGE_HEIGHT      : integer       := 480;
    C_XWIDTH            : integer       := 10;
    C_YWIDTH            : integer       := 9;    
    C_STREAK_MIN_WIDTH  : integer       := 4;
    C_NUM_STREAK_FINDERS: integer       := 8
  );
  port
  (
    clk         : in  std_logic;
    reset       : in  std_logic;
    
    strobe_in   : in  std_logic;
    din         : in  std_logic_vector(C_NUM_STREAK_FINDERS-1 downto 0);

    streak0     : out std_logic_vector(63 downto 0);
    streak0_en  : out std_logic;  

    streak1     : out std_logic_vector(63 downto 0);
    streak1_en  : out std_logic;

    streak2     : out std_logic_vector(63 downto 0);
    streak2_en  : out std_logic;

    streak3     : out std_logic_vector(63 downto 0);
    streak3_en  : out std_logic;

    streak4     : out std_logic_vector(63 downto 0);
    streak4_en  : out std_logic;

    streak5     : out std_logic_vector(63 downto 0);
    streak5_en  : out std_logic;

    streak6     : out std_logic_vector(63 downto 0);
    streak6_en  : out std_logic;

    streak7     : out std_logic_vector(63 downto 0);
    streak7_en  : out std_logic
  );
end entity StreakFinder8;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of StreakFinder8 is
-------------------------------------------------------------------------------
-- Components
-------------------------------------------------------------------------------
  component StreakFinder
    generic (
      C_IMAGE_WIDTH      : integer;
      C_IMAGE_HEIGHT     : integer;
      C_XWIDTH           : integer;
      C_YWIDTH           : integer;
      C_STREAK_MIN_WIDTH : integer);
    port (
      clk        : in  std_logic;
      reset      : in  std_logic;
      strobe_in  : in  std_logic;
      din        : in  std_logic;
      xin        : in  std_logic_vector(C_XWIDTH-1 downto 0);
      yin        : in  std_logic_vector(C_YWIDTH-1 downto 0);
      y          : out std_logic_vector(C_YWIDTH-1 downto 0);
      xstart     : out std_logic_vector(C_XWIDTH-1 downto 0);
      xend       : out std_logic_vector(C_XWIDTH-1 downto 0);
      strobe_out : out std_logic);
  end component;

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

-----------------------------------------------------------------------------
-- Functions
-----------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- Signals
-------------------------------------------------------------------------------

  signal cur_x          : std_logic_vector(C_XWIDTH-1 downto 0);
  signal cur_y          : std_logic_vector(C_YWIDTH-1 downto 0);

  signal streak_en      : std_logic_vector(7 downto 0);
  signal streaks        : std_logic_vector((8*64)-1 downto 0);
begin

  streak0_en <= streak_en(0);
  streak1_en <= streak_en(1);
  streak2_en <= streak_en(2);
  streak3_en <= streak_en(3);
  streak4_en <= streak_en(4);
  streak5_en <= streak_en(5);
  streak6_en <= streak_en(6);
  streak7_en <= streak_en(7);

  streak0 <= streaks(63+0*64 downto 0+0*64);
  streak1 <= streaks(63+1*64 downto 0+1*64);
  streak2 <= streaks(63+2*64 downto 0+2*64);
  streak3 <= streaks(63+3*64 downto 0+3*64);
  streak4 <= streaks(63+4*64 downto 0+4*64);
  streak5 <= streaks(63+5*64 downto 0+5*64);
  streak6 <= streaks(63+6*64 downto 0+6*64);
  streak7 <= streaks(63+7*64 downto 0+7*64);
  
-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------
GEN: for i in 0 to C_NUM_STREAK_FINDERS-1 generate
  FINDER: StreakFinder
    generic map (
        C_IMAGE_WIDTH      => C_IMAGE_WIDTH,
        C_IMAGE_HEIGHT     => C_IMAGE_HEIGHT,
        C_XWIDTH           => C_XWIDTH,
        C_YWIDTH           => C_YWIDTH,
        C_STREAK_MIN_WIDTH => C_STREAK_MIN_WIDTH)
    port map (
        clk        => clk,
        reset      => reset,
        strobe_in  => strobe_in,
        din        => din(i),
        xin        => cur_x,
        yin        => cur_y,
        y          => streaks(56+i*64 downto 48+i*64),
        xstart     => streaks(41+i*64 downto 32+i*64),
        xend       => streaks(25+i*64 downto 16+i*64),
        strobe_out => streak_en(i));

  streaks(63+i*64 downto 57+i*64) <= (others => '0');
  streaks(47+i*64 downto 42+i*64) <= (others => '0');
  streaks(31+i*64 downto 26+i*64) <= (others => '0');
  streaks(15+i*64 downto  0+i*64) <= (others => '0');
  
end generate GEN;
-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------
  COUNTER: xycounter
    generic map (
        C_IMAGE_WIDTH  => C_IMAGE_WIDTH,
        C_IMAGE_HEIGHT => C_IMAGE_HEIGHT,
        C_XWIDTH       => C_XWIDTH,
        C_YWIDTH       => C_YWIDTH)
    port map (
        clk   => clk,
        reset => reset,
        en    => strobe_in,
        x     => cur_x,
        y     => cur_y);
-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------   
end IMP;



