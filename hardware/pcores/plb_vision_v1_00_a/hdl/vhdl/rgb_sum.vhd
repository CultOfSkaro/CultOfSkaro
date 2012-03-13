------------------------------------------------------------------------------
-- Moniter RGB Channels to Provide Useful Information
--
-- FILE: rgb_sum.vhd
-- AUTHOR: Wade Fife
-- DATE: April 13, 2007
-- MODIFIED: April 19, 2007
--
-- DESCRIPTION
--
-- Monitors the RGB channels keeps a running sum of the R, G, and B values.
--
-- The idea is to track relevent RGB information for use by auto white balance,
-- auto exposure, and auto gain control algorithms.
--
-- Assert the frame_done signal high for one cycle to have the sums for the
-- last frame output and to reset the sums for a new frame. These sums will
-- remain on the output until frame_done is asserted again.
--
-- Use the generic C_SUM_WIDTH to specify the number of sum bits to
-- output. Only the most significant C_SUM_WIDTH bits of the sum are output.
--
-- The C_FRAME_WIDTH and C_FRAME_HEIGHT generics are used only to determine the
-- size of the counter needed for each component.
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity rgb_sum is
  
  generic (
    C_DATA_WIDTH   : integer := 8;
    C_SUM_WIDTH    : integer := 8;
    C_FRAME_WIDTH  : integer := 640;
    C_FRAME_HEIGHT : integer := 480);

  port (
    clk : in std_logic;
    rst : in std_logic;

    frame_done : in std_logic;
    
    wr_en : in std_logic;
    r   : in std_logic_vector(C_DATA_WIDTH-1 downto 0);
    g   : in std_logic_vector(C_DATA_WIDTH-1 downto 0);
    b   : in std_logic_vector(C_DATA_WIDTH-1 downto 0);

    r_sum : out std_logic_vector(C_SUM_WIDTH-1 downto 0);
    g_sum : out std_logic_vector(C_SUM_WIDTH-1 downto 0);
    b_sum : out std_logic_vector(C_SUM_WIDTH-1 downto 0)
    );

end rgb_sum;

architecture imp of rgb_sum is

  -- Function to calculate number of bits needed to represent a given
  -- positive integer (up to max_bits)
  function RequiredBits (int_num, max_bits : integer)
  return integer is
    variable test_vector : unsigned(0 to max_bits-1);
  begin
    test_vector := (others => '0');
    for i in 1 to max_bits loop
      test_vector := test_vector(1 to max_bits-1) & '1';
      if to_integer(test_vector) >= int_num then
        return i;
      end if;
    end loop;
    return max_bits;
  end function RequiredBits;

  -- Calculate total number of pixels per frame
  constant C_FRAME_PIXELS : integer := C_FRAME_WIDTH * C_FRAME_HEIGHT;

  -- Calculate the number of bits needed to represent the maximum sum
  constant C_SUM_BITS : integer := RequiredBits(C_FRAME_PIXELS, 32) +
                                   C_DATA_WIDTH;

  signal r_run_sum : unsigned(C_SUM_BITS-1 downto 0);
  signal g_run_sum : unsigned(C_SUM_BITS-1 downto 0);
  signal b_run_sum : unsigned(C_SUM_BITS-1 downto 0);

begin

  SUM_PROC: process (clk, rst, frame_done, wr_en,
                     r_run_sum, g_run_sum, b_run_sum,
                     r, g, b)
  begin
    if clk'event and clk = '1' then
      if rst = '1' then
        r_run_sum <= (others => '0');
        g_run_sum <= (others => '0');
        b_run_sum <= (others => '0');
      else
        if frame_done = '1' then
          r_run_sum <= (others => '0');
          g_run_sum <= (others => '0');
          b_run_sum <= (others => '0');
        elsif wr_en = '1' then
          r_run_sum <= r_run_sum + unsigned(r);
          g_run_sum <= g_run_sum + unsigned(g);
          b_run_sum <= b_run_sum + unsigned(b);
        end if;
      end if;
    end if;
  end process;

  OUTPUT_PROC: process (clk, rst, frame_done, r_run_sum, g_run_sum, b_run_sum)
  begin
    if clk'event and clk = '1' then
      if rst = '1' then
        r_sum <= (others => '0');
        g_sum <= (others => '0');
        b_sum <= (others => '0');
      else
        if frame_done = '1' then
          r_sum <= std_logic_vector(r_run_sum(r_run_sum'left downto r_run_sum'left-C_SUM_WIDTH+1));
          g_sum <= std_logic_vector(g_run_sum(g_run_sum'left downto g_run_sum'left-C_SUM_WIDTH+1));
          b_sum <= std_logic_vector(b_run_sum(b_run_sum'left downto b_run_sum'left-C_SUM_WIDTH+1));
        end if;
      end if;
    end if;
  end process;
    
end imp;
