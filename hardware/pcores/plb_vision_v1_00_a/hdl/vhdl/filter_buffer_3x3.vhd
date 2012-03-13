-------------------------------------------------------------------------------
-- Data Buffer for 3x3 Image Filter
--
-- FILE: filter_buffer_3x3.vhd
-- AUTHOR: Wade Fife
-- DATE: May 24, 2006
-- MODIFIED: April 25, 2007
--
-- DESCRIPTION
--
-- For the description below, N = 3.
--
-- This buffer is intended to buffer a 2D data set (such as an image) and
-- output NxN blocks of the data set. This would be used, for example, in a 2D
-- image filter, where NxN pixel regions are needed to apply an NxN kernel.
--
-- Data elements are written to the buffer by asserting din and holding wr_en
-- high for one clock cycle. Valid data on the output is indicated by the valid
-- signal going high on the same clock cycle that e11..eNN become valid. Note
-- that the valid signal goes high for the first time when the middle pixel of
-- the NxN output is the first data word that was written to the core.
--
-- Each NxN block of data is output on the element ports, corresponding to the
-- following data order (i.e., e11 is the upper left pixel for the kernel):
--
--       c0  c1  c2  c3  c4  c5  c6
--    r0 *   *   *   *   *   *   *
--    r1 *  e11 e12 e13  *   *   *
--    r2 *  e21 e22 e23  *   *   *
--    r3 *  e31 e32 e33  *   *   *
--    r4 *   *   *   *   *   *   *
--    r5 *   *   *   *   *   *   *
--
-- The reset on this entity, and its sub-hierarchy, is active-high and
-- synchronous.
--
-- A diagram of the data flow for the 3x3 filter_buffer with ROW_DEPTH of 640
-- is shown below. Vertical bars (|X|) mark registers or buffers. Each
-- ROW_BUF_XXX delays its input by C_ROW_WIDTH words.
--
-- din
--  |
--  +->|ROW_0_REG|--------+--------------->|e32_i|------------>|e31_i|
--  |                     |                   |                   |
--  |                     |                   |                   |
--  |                     |                   |                   |
--  \-|ROW_BUF_639|-+-----|------+------------|-->|e22_i|---------|-->|e21_i|
--                  |     |      |            |      |            |      |
--  /---------------/     |      |            |      |            |      |
--  |                     |      |            |      |            |      |
--  \-|ROW_BUF_639|-------|------|------+-----|------|-->|e12_i|--|------|-->|e11_i|
--                        |      |      |     |      |      |     |      |      |
--                       e33    e23    e13   e32    e22    e12   e31    e21    e11
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity filter_buffer_3x3 is
  generic (
    C_ROW_WIDTH  : integer := 640;
    C_DATA_WIDTH : integer := 10);
  port (
    clk   : in  std_logic;
    rst   : in  std_logic;
    din   : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    wr_en : in  std_logic;
    e11   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e12   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e13   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e21   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e22   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e23   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e31   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e32   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e33   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    valid : out std_logic);
end filter_buffer_3x3;


architecture imp of filter_buffer_3x3 is

  -- Function to calculate number of bits needed to represent a given positive
  -- integer (0 to 0x7FFFFFFF).
  function RequiredBits (int_num : integer)
  return integer is
    variable test_vector : unsigned(0 to 31);
  begin
    assert int_num > 0 report "ReqquiredBits() integer must be positive!" severity failure;
    assert int_num <= 2147483647 report "Integer exceeds allwed range" severity failure;
    test_vector := (others => '0');
    for i in 1 to 31 loop
      test_vector := test_vector(1 to 31) & '1';
      if to_integer(test_vector) >= int_num then
        return i;
      end if;
    end loop;
    return 31;
  end function RequiredBits;

  component delay_buffer_ramb16
    generic (
      C_DEPTH : integer;
      C_WIDTH : integer);
    port (
      clk   : in  std_logic;
      rst   : in  std_logic;
      din   : in  std_logic_vector(C_WIDTH-1 downto 0);
      wr_en : in  std_logic;
      dout  : out std_logic_vector(C_WIDTH-1 downto 0);
      valid : out std_logic);
  end component;

  -- Number of writes before output becomes valid
  constant VALID_COUNT_DONE : integer := C_ROW_WIDTH+1;
  
  signal r0_dout : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal r1_dout : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal r2_dout : std_logic_vector(C_DATA_WIDTH-1 downto 0);

  signal buffer_in : std_logic_vector(C_DATA_WIDTH*2-1 downto 0);
  signal buffer_out : std_logic_vector(C_DATA_WIDTH*2-1 downto 0);

  signal e11_i, e12_i : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e21_i, e22_i : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e31_i, e32_i : std_logic_vector(C_DATA_WIDTH-1 downto 0);

  signal valid_counter : unsigned(RequiredBits(VALID_COUNT_DONE)-1 downto 0);
  
begin

  e11 <= e11_i;
  e12 <= e12_i;
  e13 <= r2_dout;
  e21 <= e21_i;
  e22 <= e22_i;
  e23 <= r1_dout;
  e31 <= e31_i;
  e32 <= e32_i;
  e33 <= r0_dout;

  -- Simply delays the input by one cycle.
  -- Also saves wr_en so we know a new word has been received.
  ROW_0_REG_PROC: process (clk, rst, wr_en, din)
  begin
    if clk'event and clk = '1' then
      if rst = '1' then
        r0_dout <= (others => '0');
      elsif wr_en = '1' then
        r0_dout <= din;
      end if;
    end if;
  end process;

  -- This process handles the shifting of data as the window moves from left to
  -- right.
  ELEMENT_REG_PROC: process (clk, rst, wr_en,
                             e32_i, r0_dout,
                             e22_i, r1_dout,
                             e12_i, r2_dout)
  begin
    if clk'event and clk = '1' then
      if rst = '1' then
        e11_i <= (others => '0');
        e12_i <= (others => '0');
        e21_i <= (others => '0');
        e22_i <= (others => '0');
        e31_i <= (others => '0');
        e32_i <= (others => '0');
      elsif wr_en = '1' then
        -- Slide rows of data left (i.e., move window right)
        e31_i <= e32_i;
        e32_i <= r0_dout;
        e21_i <= e22_i;
        e22_i <= r1_dout;
        e11_i <= e12_i;
        e12_i <= r2_dout;
      end if;
    end if;
  end process;

  
  -- Tracks input and indicates when the first valid NxN region is output
  -- (i.e., indicating the center data element is the first that was written).
  VALID_COUNTER_PROC : process (clk, rst, valid_counter)
  begin
    if clk'event and clk = '1' then
      if rst = '1' then
        valid_counter <= (others => '0');
        valid <= '0';
      else
        if wr_en = '1' then
          if valid_counter /= VALID_COUNT_DONE then
            valid_counter <= valid_counter + 1;
          else
            valid <= '1';
          end if;
        else
          valid <= '0';
        end if;
      end if;
    end if;
  end process;

  
  -- To keep timing correct, the rows get incrementally shorter. This is
  -- because the passing of data from one row buffer to the next is itself a
  -- delay.
  
  -- Row delay buffers to align current row with future rows. This
  -- implementation uses a single wide buffer, since all rows are written to
  -- and read from simultaneously.
  row_buffers : delay_buffer_ramb16
    generic map (
        C_DEPTH => C_ROW_WIDTH-1,
        C_WIDTH => C_DATA_WIDTH*2)
    port map (
        clk   => clk,
        rst   => rst,
        din   => buffer_in,
        wr_en => wr_en,
        dout  => buffer_out,
        valid => open);

  buffer_in <= r0_dout & r1_dout;
  r1_dout <= buffer_out(C_DATA_WIDTH*2-1 downto C_DATA_WIDTH);
  r2_dout <= buffer_out(C_DATA_WIDTH-1 downto 0);
  
end imp;


