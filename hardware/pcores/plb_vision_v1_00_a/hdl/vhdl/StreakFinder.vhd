-------------------------------------------------------------------------------
-- 
-- FILE:                StreakFinder.vhd
-- AUTHOR:              Barrett Edwards
-- CREATED:             18 April 2007
-- ORGINIZATION:        Brigham Young University
--                      Dept. Electrical Engineering
--                      Robot Vision Lab
--
-- DESCRIPTION:
--  This core attempts to find continuous horizontal streaks of 1's in a
--  binary image. It is assumed that the input to this core is a binary image
--  that has been segmented using some technique such as color segmentation,
--  where a 1 signifies that something was found at a particular pixel.
--
--  This core finds these streak and then when the end of the streak is found,
--  this core will output (y,xstart,xend) values that represent the starting
--  and ending point of the streak. These values can then be used in some
--  connected components algorithm.
--
--  To accomplish this, this core waits until a pixel value of 1 is received,
--  and then it stores the current x,y values into a register. This core then
--  waits until it gets an input value of 0. If the width of the streak is
--  greater than a minimum threshold (typicaly 3 pixel wide) then this core
--  will output the y,xstart,xend values and assert the strobe_out signal
--  signifying that this core has found a streak. The minimum streak width is
--  imlpemented in a shift register. The left most bit is set to a 1 when the
--  start of a streak is found. Then each time another pixel is found in the
--  streak the shift register is shifted to the right. So a 3 bit shift
--  register needs 3 pixels in a row to be 1 to shift all the way to the right.
--  Then the output strobe is controlled by the right most bit of the shift
--  register. If the right most bit of the shift register is 1, then we must
--  have found a streak that is at least as long as the shift register and
--  therefore we can output a strobe because this streak is long enough. 
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
--  xin         : is the current x coordinate in the image
--  yin         : is the current y coordinate in the image
--  strobe_in   : is the input strobe that implies the din value is valid
--  din         : is a binary value signifying that some thresholded value has
--                been detected at this pixel. For example, if color
--                segmentation was being performed on an image, then a 1 would
--                indicate that this pixel matched the thresholded values,
--                while a 0 would indicate that this pixel did not match the
--                thresholds
--  
-- SIGNALS: OUTPUTS
--  y           : signifies the row in the image where the streak is
--  xstart      : signifies the starting column of the streak
--  xend        : signifies the ending column of the streak
--  strobe_out  : when 1, this signal implies that the data out values (y,xstart,
--                xend) are valid on this cycle 
--  
-- 

-- RESOURCES: for Xilinx Virtex4 FX60 speed grade -11:
--   Slices: 35
--   FF    : 33
--   LUTs  : 62
--   BRAMs : 0  
--   DSPs  : 0  
--   maxfq : 410.004 MHz (2.439ns period)
--
--
-- REVISONS: 
-- 1.00.a : 18 April 2007 : Original release
-- 
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-------------------------------------------------------------------------------
-- Entity Declaration
-------------------------------------------------------------------------------
entity StreakFinder is
  generic
  (
    C_IMAGE_WIDTH       : integer       := 640;
    C_IMAGE_HEIGHT      : integer       := 480;
    C_XWIDTH            : integer       := 10;
    C_YWIDTH            : integer       := 9;
    C_STREAK_MIN_WIDTH  : integer       := 3
  );
  port
  (
    clk         : in  std_logic;
    reset       : in  std_logic;
    
    strobe_in   : in  std_logic;
    din         : in  std_logic;
    xin         : in  std_logic_vector(C_XWIDTH-1 downto 0);
    yin         : in  std_logic_vector(C_YWIDTH-1 downto 0);

    y           : out std_logic_vector(C_YWIDTH-1 downto 0);
    xstart      : out std_logic_vector(C_XWIDTH-1 downto 0);
    xend        : out std_logic_vector(C_XWIDTH-1 downto 0);
    strobe_out  : out std_logic
  );
end entity StreakFinder;

------------------------------------------------------------------------------
-- Architecture Implementation
------------------------------------------------------------------------------
architecture IMP of StreakFinder is
-------------------------------------------------------------------------------
-- Componenets
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- Functions
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- Signals
-------------------------------------------------------------------------------    
  signal running        : signed(C_STREAK_MIN_WIDTH-1 downto 0);
  signal reg_y          : std_logic_vector(C_YWIDTH-1 downto 0); 
  signal reg_xstart     : std_logic_vector(C_XWIDTH-1 downto 0);
  signal reg_xend       : std_logic_vector(C_XWIDTH-1 downto 0);
  
begin
-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------
  y             <= reg_y;
  xstart        <= reg_xstart;
  xend          <= reg_xend;

  process(clk,reset,strobe_in,din,xin,yin)
  begin
    if rising_edge(clk) then
      if reset = '1' then
        running         <= (others => '0');
        strobe_out      <= '0';
      else
        strobe_out      <= '0'; -- schedule this to be 0

        if strobe_in = '1' then
          if din = '1' and unsigned(xin) /= C_IMAGE_WIDTH-1 then
            if running(running'left) = '0' then
              -- found the start of a new streak
              running(running'left)     <= '1';
              reg_y                     <= yin;
              reg_xstart                <= xin;
            else
              running <= shift_right(running,1);              
            end if;            
          else
            if running(running'left) = '1' then
              -- found the end of a streak              
              running           <= (others => '0');
              reg_xend          <= std_logic_vector(unsigned(xin) - 1);
              
              strobe_out        <= running(0);
            end if;                       
          end if;
        end if;
      end if;      
    end if;
  end process; 

-------------------------------------------------------------------------------
end IMP;



