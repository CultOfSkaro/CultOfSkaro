-------------------------------------------------------------------------------
-- Bayer RGB Demosaicing - Microsoft Research's Linear Interpolation
--
-- FILE: demosaic_microsoft.vhd
-- AUTHOR: Aaron Dennis
-- DATE: May 3, 2007
-- MODIFIED:
--
-- In version 2 (v2) one additional adder stage was added to the pipeline.
-- Also, only the needed number of bits are used in representing the
-- various values that are computed.  Version 1 had extra bits.
--
-- NOTES
--
-- The file demosaic_linear.vhd, written by Wade Fife, was used as a
-- starting point/template for this file.
--
-- REFERENCES
--
-- The demosaicing algorithm implemented in this core is described in
-- the following paper:
--
-- Malvar, H. S., Wei He, L., and Cutler, R. 2004. High-quality linear
-- interpolation for demosaicing of bayer-patterned color images. Proceedings
-- of the IEEE International Conference on Speech, Acoustics, and
-- Signal Processing.
--
-- DESCRIPTION
--
-- Demosaics a standard Bayer RGB pattern.  A 5x5 window of Bayer pattern
-- values is used to compute demosaiced RGB values.  The RGB value's location
-- in the demosaiced image corresponds to the location of the center of the 5x5
-- window (e33).  This means that when calculating RGB values for the top-left
-- pixel, the top two rows and left two columns in the 5x5 window won't have
-- valid data.  When no valid data is available for an input signal it may be
-- set to zero.
--
-- Conceptually, the Bayer data will be input on the e11..e55 input signals (a
-- 5x5 window).  However, since in practice some of the values from the 5x5
-- window will never be used, not all of the input signals e11..e55 exist.
-- Specifically, the corner values and the values directly to the side, top,
-- and bottom of the corners won't be used. In other words, e11, e12, e21 won't
-- be used; e15, e14, e25 won't be used; e51, e52, e41 won't be used; and e55,
-- e54, e45 won't be used. The values that will be used are labeled below and
-- the unused values are labeled '***':
--
--                        ***   ***   e13   ***   ***
--                        ***   e22   e23   e24   ***
--                        e31   e32   e33   e34   e35
--                        ***   e42   e43   e44   ***
--                        ***   ***   e53   ***   ***
--
-- As seen in the above diagram, the eXX numbering parallels matrix element
-- numbering; that is, e11 is top left, e15 is top right, e55 is bottom right,
-- e33 is center, etc.
--
-- To feed data in, the wr_en signal should be held high for one clock cycle
-- at the same time that e11..e55 have valid data.
--
-- The valid signal indicates that the r, g, and b output values are valid.
--
-- The new_frame signal can be used to indicate the start of a new frame. This
-- essentially resets the core, but data already written will still result in
-- correct data on the output. Data should NOT be written (i.e., wr_en
-- shouldn't be high) on the same cycle that new_frame is high.
--
-- The start_pos signal specifies the Bayer pattern position of the first pixel
-- for the new frame. The signal start_pos is only sampled on the cycle that
-- new_frame is high. The start position can be changed on a per frame basis
-- for windowing purposes. "00" indicates green pixel on the green/red row,
-- "01" indicates a red pixel, "10" indicates a blue pixel, and "11" indicates
-- a green pixel on the green/blue row. The following Bayer pattern is assumed:
--
--      GRGRGRGR
--      BGBGBGBG
--      GRGRGRGR
--      BGBGBGBG
--
-- C_IMAGE_WIDTH specifies the width of the image, so that the core knows when
-- the Bayer pattern should change for the next row.
--
-- C_DATA_WIDTH specifies the bit width of the input Bayer pixels and the R, G,
-- and B output widths.
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity demosaic_microsoft is
  generic (
    C_IMAGE_WIDTH : integer := 640;
    C_DATA_WIDTH  : integer := 8);
  port (
    clk       : in  std_logic;
    rst       : in  std_logic;
    new_frame : in  std_logic;
    start_pos : in  std_logic_vector(1 downto 0);
    e13       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e22       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e23       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e24       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e31       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e32       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e33       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e34       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e35       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e42       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e43       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e44       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    e53       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    wr_en     : in  std_logic;
    r         : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    g         : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    b         : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    valid     : out std_logic);
end demosaic_microsoft;


architecture imp of demosaic_microsoft is

  constant START_GR : std_logic_vector := "00";
  constant START_R  : std_logic_vector := "01";
  constant START_B  : std_logic_vector := "10";
  constant START_GB : std_logic_vector := "11";

  constant IN_WIDTH    : integer := C_DATA_WIDTH+1;
  constant MULT1_WIDTH : integer := IN_WIDTH;
  constant MULT2_WIDTH : integer := IN_WIDTH+1;
  constant MULT4_WIDTH : integer := IN_WIDTH+2;
  constant MULT8_WIDTH : integer := IN_WIDTH+3;

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

  -- Bayer pattern states
  type state_type is (GR_STATE, R_STATE, B_STATE, GB_STATE);
  signal first_row_start : state_type;
  signal red_row_start   : state_type;
  signal blue_row_start  : state_type;

  -- Stage 0 Signals - Input registers
  signal e13_0                             : signed(IN_WIDTH-1 downto 0);
  signal e22_0, e23_0, e24_0               : signed(IN_WIDTH-1 downto 0);
  signal e31_0, e32_0, e33_0, e34_0, e35_0 : signed(IN_WIDTH-1 downto 0);
  signal e42_0, e43_0, e44_0               : signed(IN_WIDTH-1 downto 0);
  signal e53_0                             : signed(IN_WIDTH-1 downto 0);
  signal e13_n, e53_n, e31_n, e35_n        : signed(MULT1_WIDTH-1 downto 0);
  signal valid_0                           : std_logic;
  signal new_frame_0                       : std_logic;

  -- Stage 1 Signals                              Val
  signal m131        : signed(MULT1_WIDTH-1 downto 0);  -- e13 * -1
  signal m132        : signed(MULT1_WIDTH-1 downto 0);  --  e13 *  1/2
  signal m133        : signed(MULT4_WIDTH-1 downto 0);  --  e13 * -3/2
  signal m221        : signed(MULT1_WIDTH-1 downto 0);  --  e22 * -1
  signal m222        : signed(MULT2_WIDTH-1 downto 0);  --  e22 *  2
  signal m231        : signed(MULT2_WIDTH-1 downto 0);  --  e23 *  2
  signal m232        : signed(MULT4_WIDTH-1 downto 0);  --  e23 *  4
  signal m241        : signed(MULT1_WIDTH-1 downto 0);  --  e24 * -1
  signal m242        : signed(MULT2_WIDTH-1 downto 0);  --  e24 *  2
  signal m311        : signed(MULT1_WIDTH-1 downto 0);  -- e31 * -1
  signal m312        : signed(MULT1_WIDTH-1 downto 0);  -- e31 *  1/2
  signal m313        : signed(MULT4_WIDTH-1 downto 0);  --  e31 * -3/2
  signal m321        : signed(MULT2_WIDTH-1 downto 0);  --  e32 *  2
  signal m322        : signed(MULT4_WIDTH-1 downto 0);  --  e32 *  4
  signal m331        : signed(MULT4_WIDTH-1 downto 0);  --  e33 *  4
  signal m332        : signed(MULT8_WIDTH-1 downto 0);  --  e33 *  5
  signal m333        : signed(MULT8_WIDTH-1 downto 0);  --  e33 *  6
  signal m341        : signed(MULT2_WIDTH-1 downto 0);  --  e34 *  2
  signal m342        : signed(MULT4_WIDTH-1 downto 0);  --  e34 *  4
  signal m351        : signed(MULT1_WIDTH-1 downto 0);  -- e35 * -1
  signal m352        : signed(MULT1_WIDTH-1 downto 0);  --  e35 *  1/2
  signal m353        : signed(MULT4_WIDTH-1 downto 0);  --  e35 * -3/2
  signal m421        : signed(MULT1_WIDTH-1 downto 0);  --  e42 * -1
  signal m422        : signed(MULT2_WIDTH-1 downto 0);  --  e42 *  2
  signal m431        : signed(MULT2_WIDTH-1 downto 0);  --  e43 *  2
  signal m432        : signed(MULT4_WIDTH-1 downto 0);  --  e43 *  4
  signal m441        : signed(MULT1_WIDTH-1 downto 0);  --  e44 * -1
  signal m442        : signed(MULT2_WIDTH-1 downto 0);  --  e44 *  2
  signal m531        : signed(MULT1_WIDTH-1 downto 0);  -- e53 * -1
  signal m532        : signed(MULT1_WIDTH-1 downto 0);  --  e53 *  1/2
  signal m533        : signed(MULT4_WIDTH-1 downto 0);  --  e53 * -3/2
  signal e33_1       : unsigned(C_DATA_WIDTH-1 downto 0);
  signal valid_1     : std_logic;
  signal new_frame_1 : std_logic;

  -- Stage 2 Signals
  signal common_add_1  : signed(MULT1_WIDTH downto 0);
  signal common_add_2  : signed(MULT1_WIDTH downto 0);
  signal common_add_3a : signed(MULT8_WIDTH-1 downto 0);
  signal common_add_3b : signed(MULT8_WIDTH-1 downto 0);
  signal fp_add_1      : signed(MULT1_WIDTH downto 0);
  signal fp_add_2      : signed(MULT1_WIDTH downto 0);
  signal fp_add_3      : signed(MULT4_WIDTH+1 downto 0);
  signal gr_gb_adda    : signed(MULT4_WIDTH+1 downto 0);
  signal gr_gb_addb    : signed(MULT4_WIDTH+1 downto 0);
  signal rgr_bgb_add   : signed(MULT4_WIDTH downto 0);
  signal rgb_bgr_add   : signed(MULT4_WIDTH downto 0);
  signal rb_br_adda    : signed(MULT8_WIDTH downto 0);
  signal rb_br_addb    : signed(MULT8_WIDTH downto 0);
  signal e33_2         : unsigned(C_DATA_WIDTH-1 downto 0);
  signal valid_2       : std_logic;
  signal new_frame_2   : std_logic;

  -- Stage 3 Signals
  signal gr_gba      : signed(MULT2_WIDTH+2 downto 0);
  signal gr_gbb      : signed(MULT2_WIDTH+2 downto 0);
  signal rgr_bgba    : signed(MULT2_WIDTH+3 downto 0);
  signal rgr_bgbb    : signed(MULT2_WIDTH+3 downto 0);
  signal rgb_bgra    : signed(MULT2_WIDTH+3 downto 0);
  signal rgb_bgrb    : signed(MULT2_WIDTH+3 downto 0);
  signal rb_bra      : signed(MULT2_WIDTH+3 downto 0);
  signal rb_brb      : signed(MULT2_WIDTH+3 downto 0);
  signal e33_3       : unsigned(C_DATA_WIDTH-1 downto 0);
  signal valid_3     : std_logic;
  signal new_frame_3 : std_logic;

  -- Stage 4 Signals
  signal gr_gb       : signed(MULT2_WIDTH-1 downto 0);
  signal rgr_bgb     : signed(MULT2_WIDTH-1 downto 0);
  signal rgb_bgr     : signed(MULT2_WIDTH-1 downto 0);
  signal rb_br       : signed(MULT2_WIDTH-1 downto 0);
  signal e33_4       : unsigned(C_DATA_WIDTH-1 downto 0);
  signal valid_4     : std_logic;
  signal new_frame_4 : std_logic;

  -- Stage 5 Signals
  signal gr_gb_sat   : unsigned(C_DATA_WIDTH-1 downto 0);
  signal rgr_bgb_sat : unsigned(C_DATA_WIDTH-1 downto 0);
  signal rgb_bgr_sat : unsigned(C_DATA_WIDTH-1 downto 0);
  signal rb_br_sat   : unsigned(C_DATA_WIDTH-1 downto 0);
  signal e33_5       : unsigned(C_DATA_WIDTH-1 downto 0);
  signal valid_5     : std_logic;
  signal new_frame_5 : std_logic;

  -- State Machine Signals
  signal next_state, current_state : state_type;

  -- Width Monitor Signals
  signal width_counter : unsigned(RequiredBits(C_IMAGE_WIDTH-2, 32)-1 downto 0);
  signal eol           : std_logic;     -- End of line indicator
  
begin

  -----------------------------------------------------------------------------
  -- Stage 0
  --
  -- Input registers.
  -----------------------------------------------------------------------------

  STAGE_0_PROC : process(clk, rst, wr_en, new_frame, start_pos,
                        e13,
                        e22, e23, e24,
                        e31, e32, e33, e34, e35,
                        e42, e43, e44,
                        e53)
  begin
    if rst = '1' then
      valid_0         <= '0';
      new_frame_0     <= '0';
      first_row_start <= GR_STATE;
      red_row_start   <= GR_STATE;
      blue_row_start  <= GR_STATE;
      e13_0           <= (others => '0');
      e22_0           <= (others => '0');
      e23_0           <= (others => '0');
      e24_0           <= (others => '0');
      e31_0           <= (others => '0');
      e32_0           <= (others => '0');
      e33_0           <= (others => '0');
      e34_0           <= (others => '0');
      e35_0           <= (others => '0');
      e42_0           <= (others => '0');
      e43_0           <= (others => '0');
      e44_0           <= (others => '0');
      e53_0           <= (others => '0');
      e13_n           <= (others => '0');
      e53_n           <= (others => '0');
      e31_n           <= (others => '0');
      e35_n           <= (others => '0');

    elsif rising_edge(clk) then
      new_frame_0 <= new_frame;
      valid_0     <= wr_en;

      -- For new frame, reset essential registers and determine which state
      -- we should switch to for each row type, based on start position.
      if new_frame = '1' then
        case start_pos is
          when START_GR =>
            first_row_start <= GR_STATE;
            red_row_start   <= GR_STATE;
            blue_row_start  <= B_STATE;
          when START_R =>
            first_row_start <= R_STATE;
            red_row_start   <= R_STATE;
            blue_row_start  <= GB_STATE;
          when START_B =>
            first_row_start <= B_STATE;
            red_row_start   <= GR_STATE;
            blue_row_start  <= B_STATE;
          when START_GB =>
            first_row_start <= GB_STATE;
            red_row_start   <= R_STATE;
            blue_row_start  <= GB_STATE;
          when others => null;
        end case;
      end if;

      -- Read the inputs when wr_en goes high.
      if wr_en = '1' then
        e13_0 <= signed('0' & e13);
        e22_0 <= signed('0' & e22);
        e23_0 <= signed('0' & e23);
        e24_0 <= signed('0' & e24);
        e31_0 <= signed('0' & e31);
        e32_0 <= signed('0' & e32);
        e33_0 <= signed('0' & e33);
        e34_0 <= signed('0' & e34);
        e35_0 <= signed('0' & e35);
        e42_0 <= signed('0' & e42);
        e43_0 <= signed('0' & e43);
        e44_0 <= signed('0' & e44);
        e53_0 <= signed('0' & e53);

        e13_n <= -signed('0' & e13);
        e53_n <= -signed('0' & e53);
        e31_n <= -signed('0' & e31);
        e35_n <= -signed('0' & e35);
      end if;

    end if;
  end process;



  -----------------------------------------------------------------------------
  -- Stage 1
  --
  -- Element-wise multiplication of the mask matrices with the Bayer data.
  -----------------------------------------------------------------------------
  
  STAGE_1_PROC : process (clk, rst, new_frame_0, valid_0,
                         e13_0,
                         e22_0, e23_0, e24_0,
                         e31_0, e32_0, e33_0, e34_0, e35_0,
                         e42_0, e43_0, e44_0,
                         e53_0,
                         e13_n, e53_n, e31_n, e35_n)
    variable t33a : signed(MULT8_WIDTH-1 downto 0);
    variable t33b : signed(MULT8_WIDTH-1 downto 0);
  begin
    if rst = '1' then
      new_frame_1 <= '0';
      valid_1     <= '0';
      e33_1       <= (others => '0');
      m131        <= (others => '0');
      m132        <= (others => '0');
      m133        <= (others => '0');
      m221        <= (others => '0');
      m222        <= (others => '0');
      m231        <= (others => '0');
      m232        <= (others => '0');
      m241        <= (others => '0');
      m242        <= (others => '0');
      m311        <= (others => '0');
      m312        <= (others => '0');
      m313        <= (others => '0');
      m321        <= (others => '0');
      m322        <= (others => '0');
      m331        <= (others => '0');
      m332        <= (others => '0');
      m333        <= (others => '0');
      m341        <= (others => '0');
      m342        <= (others => '0');
      m351        <= (others => '0');
      m352        <= (others => '0');
      m353        <= (others => '0');
      m421        <= (others => '0');
      m422        <= (others => '0');
      m431        <= (others => '0');
      m432        <= (others => '0');
      m441        <= (others => '0');
      m442        <= (others => '0');
      m531        <= (others => '0');
      m532        <= (others => '0');
      m533        <= (others => '0');

    elsif rising_edge(clk) then
      new_frame_1 <= new_frame_0;
      valid_1     <= valid_0;
      e33_1       <= resize(unsigned(e33_0), C_DATA_WIDTH);

      -- First Row
      -- the least significant bit of m132 and m133 are interpreted as decimal bits
      m131 <= e13_n;                                 -- multiply by -1
      m132 <= e13_0;                                 -- multiply by 1/2
      m133 <= ('1' & e13_n & '0') - ("00" & e13_0);  -- multiply by -3/2

      -- Second Row
      m221 <= -e22_0;                                     -- multiply by -1
      m222 <= shift_left(resize(e22_0, MULT2_WIDTH), 1);  -- multiply by 2

      m231 <= shift_left(resize(e23_0, MULT2_WIDTH), 1);  -- multiply by 2
      m232 <= shift_left(resize(e23_0, MULT4_WIDTH), 2);  -- multiply by 4

      m241 <= -e24_0;                                     -- multiply by -1
      m242 <= shift_left(resize(e24_0, MULT2_WIDTH), 1);  -- multiply by 2

      -- Third Row
      -- the least significant bit of m312 and m313 are interpreted as decimal bits
      m311 <= e31_n;                                 -- multiply by -1
      m312 <= e31_0;                                 -- multiply by 1/2
      m313 <= ('1' & e31_n & '0') - ("00" & e31_0);  -- multiply by -3/2

      m321 <= shift_left(resize(e32_0, MULT2_WIDTH), 1);  -- multiply by 2
      m322 <= shift_left(resize(e32_0, MULT4_WIDTH), 2);  -- multiply by 4

      t33a := shift_left(resize(e33_0, MULT8_WIDTH), 1);
      t33b := shift_left(resize(e33_0, MULT8_WIDTH), 2);
      m331 <= resize(t33b, MULT4_WIDTH);          -- multiply by 4
      m332 <= t33b + resize(e33_0, MULT8_WIDTH);  -- multiply by 5
      m333 <= t33b + t33a;                        -- multiply by 6

      m341 <= shift_left(resize(e34_0, MULT2_WIDTH), 1);  -- multiply by 2
      m342 <= shift_left(resize(e34_0, MULT4_WIDTH), 2);  -- multiply by 4

      -- the least significant bit of m352 and m353 are interpreted as decimal bits
      m351 <= e35_n;                                 -- multiply by -1
      m352 <= e35_0;                                 -- multiply by 1/2
      m353 <= ('1' & e35_n & '0') - ("00" & e35_0);  -- multiply by -3/2

      -- Fourth Row
      m421 <= -e42_0;                                     -- multiply by -1
      m422 <= shift_left(resize(e42_0, MULT2_WIDTH), 1);  -- multiply by 2

      m431 <= shift_left(resize(e43_0, MULT2_WIDTH), 1);  -- multiply by 2
      m432 <= shift_left(resize(e43_0, MULT4_WIDTH), 2);  -- multiply by 4

      m441 <= -e44_0;                                     -- multiply by -1
      m442 <= shift_left(resize(e44_0, MULT2_WIDTH), 1);  -- multiply by 2

      -- Fifth Row
      -- the least significant bit of m532 and m533 are interpreted as decimal bits
      m531 <= e53_n;                                 -- multiply by -1
      m532 <= e53_0;                                 -- multiply by 1/2
      m533 <= ('1' & e53_n & '0') - ("00" & e53_0);  -- multiply by -3/2
      
    end if;
  end process STAGE_1_PROC;




  -----------------------------------------------------------------------------
  -- Stage 2
  --
  -- First adder stage.
  -----------------------------------------------------------------------------

  STAGE_2_PROC : process(clk, rst, new_frame_1, valid_1, e33_1,
                        m131, m132, m133,
                        m221, m222,
                        m231, m232,
                        m241, m242,
                        m311, m312, m313,
                        m321, m322,
                        m331, m332, m333,
                        m341, m342,
                        m351, m352, m353,
                        m421, m422,
                        m431, m432,
                        m441, m442,
                        m531, m532, m533)
  begin
    if rst = '1' then
      new_frame_2   <= '0';
      valid_2       <= '0';
      e33_2         <= (others => '0');
      common_add_1  <= (others => '0');
      common_add_2  <= (others => '0');
      common_add_3a <= (others => '0');
      common_add_3b <= (others => '0');
      fp_add_1      <= (others => '0');
      fp_add_2      <= (others => '0');
      fp_add_3      <= (others => '0');
      gr_gb_adda    <= (others => '0');
      gr_gb_addb    <= (others => '0');
      rgr_bgb_add   <= (others => '0');
      rgb_bgr_add   <= (others => '0');
      rb_br_adda    <= (others => '0');
      rb_br_addb    <= (others => '0');

    elsif rising_edge(clk) then
      new_frame_2 <= new_frame_1;
      valid_2     <= valid_1;
      e33_2       <= e33_1;

      -- Do some preliminary additions
      common_add_1 <= resize(m131, MULT1_WIDTH+1) +
                      resize(m531, MULT1_WIDTH+1);
      common_add_2 <= resize(m311, MULT1_WIDTH+1) +
                      resize(m351, MULT1_WIDTH+1);
      common_add_3a <= resize(m221, MULT8_WIDTH) +
                       resize(m241, MULT8_WIDTH) +
                       resize(m332, MULT8_WIDTH);
      common_add_3b <= resize(m421, MULT8_WIDTH) +
                       resize(m441, MULT8_WIDTH);

      -- the least significant digits of fp_add_1/2/3 are decimal digits
      fp_add_1 <= resize(m132, MULT1_WIDTH+1) +
                  resize(m532, MULT1_WIDTH+1);
      fp_add_2 <= resize(m312, MULT1_WIDTH+1) +
                  resize(m352, MULT1_WIDTH+1);
      fp_add_3 <= resize(m133, MULT4_WIDTH+2) +
                  resize(m533, MULT4_WIDTH+2) +
                  resize(m313, MULT4_WIDTH+2) +
                  resize(m353, MULT4_WIDTH+2);

      gr_gb_adda <= resize(m231, MULT4_WIDTH+2) +
                    resize(m321, MULT4_WIDTH+2) +
                    resize(m331, MULT4_WIDTH+2);
      gr_gb_addb <= resize(m341, MULT4_WIDTH+2) +
                    resize(m431, MULT4_WIDTH+2);

      rgr_bgb_add <= resize(m322, MULT4_WIDTH+1) +
                     resize(m342, MULT4_WIDTH+1);

      rgb_bgr_add <= resize(m232, MULT4_WIDTH+1) +
                     resize(m432, MULT4_WIDTH+1);

      rb_br_adda <= resize(m222, MULT8_WIDTH+1) +
                    resize(m242, MULT8_WIDTH+1);
      rb_br_addb <= resize(m333, MULT8_WIDTH+1) +
                    resize(m422, MULT8_WIDTH+1) +
                    resize(m442, MULT8_WIDTH+1);

    end if;
  end process STAGE_2_PROC;




  -----------------------------------------------------------------------------
  -- Stage 3
  --
  -- Second adder stage.
  -----------------------------------------------------------------------------

  STAGE_3_PROC : process(clk, rst, new_frame_2, valid_2, e33_2,
                        common_add_1, common_add_2, common_add_3a, common_add_3b,
                        fp_add_1, fp_add_2, fp_add_3,
                        gr_gb_adda, gr_gb_addb,
                        rgr_bgb_add, rgb_bgr_add,
                        rb_br_adda, rb_br_addb)

  begin
    if rst = '1' then
      new_frame_3 <= '0';
      valid_3     <= '0';
      e33_3       <= (others => '0');
      gr_gba      <= (others => '0');
      gr_gbb      <= (others => '0');
      rgr_bgba    <= (others => '0');
      rgr_bgbb    <= (others => '0');
      rgb_bgra    <= (others => '0');
      rgb_bgrb    <= (others => '0');
      rb_bra      <= (others => '0');
      rb_brb      <= (others => '0');
      
    elsif rising_edge(clk) then
      new_frame_3 <= new_frame_2;
      valid_3     <= valid_2;
      e33_3       <= e33_2;

      -- the lowest bit of fp_add_1, fp_add_2, and fp_add_3 are decimal bits.
      gr_gba <= resize(
        resize(common_add_1, MULT4_WIDTH+2) +
        resize(common_add_2, MULT4_WIDTH+2),
        MULT2_WIDTH+3);
      gr_gbb <= resize(
        resize(gr_gb_adda, MULT4_WIDTH+2) +
        resize(gr_gb_addb, MULT4_WIDTH+2),
        MULT2_WIDTH+3);
      rgr_bgba <= resize(
        resize(common_add_2 & '0', MULT8_WIDTH+2) +
        resize(common_add_3a & '0', MULT8_WIDTH+2) +
        resize(common_add_3b & '0', MULT8_WIDTH+2),
        MULT2_WIDTH+4);
      rgr_bgbb <= resize(
        resize(rgr_bgb_add & '0', MULT8_WIDTH+2) +
        resize(fp_add_1, MULT8_WIDTH+2) +
        resize('0' & fp_add_1(0), MULT8_WIDTH+2),
        MULT2_WIDTH+4);
      rgb_bgra <= resize(
        resize(common_add_1 & '0', MULT8_WIDTH+2) +
        resize(common_add_3a & '0', MULT8_WIDTH+2) +
        resize(common_add_3b & '0', MULT8_WIDTH+2),
        MULT2_WIDTH+4);
      rgb_bgrb <= resize(
        resize(rgb_bgr_add & '0', MULT8_WIDTH+2) +
        resize(fp_add_2, MULT8_WIDTH+2) +
        resize('0' & fp_add_2(0), MULT8_WIDTH+2),
        MULT2_WIDTH+4);
      rb_bra <= resize(
        resize(rb_br_adda & '0', MULT8_WIDTH+2) +
        resize(rb_br_addb & '0', MULT8_WIDTH+2) +
        resize(fp_add_3, MULT8_WIDTH+2) +
        resize('0' & fp_add_3(0), MULT8_WIDTH+2),
        MULT2_WIDTH+4);
      rb_brb <= (others => '0');
      
    end if;
  end process STAGE_3_PROC;



  -----------------------------------------------------------------------------
  -- Stage 4
  --
  -- Second adder stage.
  -----------------------------------------------------------------------------

  STAGE_4_PROC : process(clk, rst, new_frame_3, valid_3, e33_3,
                        gr_gba, gr_gbb,
                        rgr_bgba, rgr_bgbb,
                        rgb_bgra, rgb_bgrb,
                        rb_bra, rb_brb)

  begin
    if rst = '1' then
      new_frame_4 <= '0';
      valid_4     <= '0';
      e33_4       <= (others => '0');
      gr_gb       <= (others => '0');
      rgr_bgb     <= (others => '0');
      rgb_bgr     <= (others => '0');
      rb_br       <= (others => '0');

    elsif rising_edge(clk) then
      new_frame_4 <= new_frame_3;
      valid_4     <= valid_3;
      e33_4       <= e33_3;

      -- the lowest bit of fp_add_1, fp_add_2, and fp_add_3 are decimal bits.
      gr_gb <= resize(
        shift_right(                    -- normalize by 8            
          gr_gba +
          gr_gbb,
          3),
        MULT2_WIDTH);
      rgr_bgb <= resize(
        shift_right(  -- normalize by 8 and get rid of the decimal bit
          rgr_bgba +
          rgr_bgbb,
          4),
        MULT2_WIDTH);
      rgb_bgr <= resize(
        shift_right(  -- normalize by 8 and get rid of the decimal bit
          rgb_bgra +
          rgb_bgrb,
          4),
        MULT2_WIDTH);
      rb_br <= resize(
        shift_right(  -- normalize by 8 and get rid of the decimal bit
          rb_bra +
          rb_brb,
          4),
        MULT2_WIDTH);

    end if;
  end process STAGE_4_PROC;



  -----------------------------------------------------------------------------
  -- Stage 5
  --
  -- Satuarate the calculated values
  -----------------------------------------------------------------------------
  
  STAGE_5_PROC : process(clk, rst, new_frame_4, valid_4, e33_4,
                        gr_gb, rgr_bgb, rgb_bgr, rb_br)
  begin
    if rst = '1' then
      new_frame_5 <= '0';
      valid_5     <= '0';
      e33_5       <= (others => '0');
      gr_gb_sat   <= (others => '0');
      rgr_bgb_sat <= (others => '0');
      rgb_bgr_sat <= (others => '0');
      rb_br_sat   <= (others => '0');
      
    elsif rising_edge(clk) then
      new_frame_5 <= new_frame_4;
      valid_5     <= valid_4;
      e33_5       <= e33_4;

      if gr_gb(gr_gb'left) = '1' then
        gr_gb_sat <= (others => '0');   --Value went negative
      elsif signed(gr_gb(gr_gb'left-1 downto C_DATA_WIDTH)) /= integer(0) then
        gr_gb_sat <= (others => '1');  --Value exceeds representable positive value
      else
        gr_gb_sat <= resize(unsigned(gr_gb), C_DATA_WIDTH);
      end if;

      if rgr_bgb(rgr_bgb'left) = '1' then
        rgr_bgb_sat <= (others => '0');  --Value went negative
      elsif signed(rgr_bgb(rgr_bgb'left-1 downto C_DATA_WIDTH)) /= integer(0) then
        rgr_bgb_sat <= (others => '1');  --Value exceeds representable positive value
      else
        rgr_bgb_sat <= resize(unsigned(rgr_bgb), C_DATA_WIDTH);
      end if;

      if rgb_bgr(rgb_bgr'left) = '1' then
        rgb_bgr_sat <= (others => '0');  --Value went negative
      elsif signed(rgb_bgr(rgb_bgr'left-1 downto C_DATA_WIDTH)) /= integer(0) then
        rgb_bgr_sat <= (others => '1');  --Value exceeds representable positive value
      else
        rgb_bgr_sat <= resize(unsigned(rgb_bgr), C_DATA_WIDTH);
      end if;

      if rb_br(rb_br'left) = '1' then
        rb_br_sat <= (others => '0');   --Value went negative
      elsif signed(rb_br(rb_br'left-1 downto C_DATA_WIDTH)) /= integer(0) then
        rb_br_sat <= (others => '1');  --Value exceeds representable positive value
      else
        rb_br_sat <= resize(unsigned(rb_br), C_DATA_WIDTH);
      end if;

    end if;
  end process STAGE_5_PROC;


  -----------------------------------------------------------------------------
  -- Stage 6
  --
  -- Output Selection
  -----------------------------------------------------------------------------
  
  STAGE_6_PROC : process(clk, rst, new_frame_5, valid_5, current_state, e33_5,
                        gr_gb_sat, rgr_bgb_sat, rgb_bgr_sat, rb_br_sat)
  begin
    if rst = '1' then
      valid <= '0';
      r     <= (others => '0');
      g     <= (others => '0');
      b     <= (others => '0');
      
    elsif rising_edge(clk) then
      valid <= valid_5;

      -- Select correct rgb output based on current Bayer patter position.
      case current_state is
        when GR_STATE =>
          r <= std_logic_vector(rgr_bgb_sat);
          g <= std_logic_vector(e33_5);
          b <= std_logic_vector(rgb_bgr_sat);
        when R_STATE =>
          r <= std_logic_vector(e33_5);
          g <= std_logic_vector(gr_gb_sat);
          b <= std_logic_vector(rb_br_sat);
        when B_STATE =>
          r <= std_logic_vector(rb_br_sat);
          g <= std_logic_vector(gr_gb_sat);
          b <= std_logic_vector(e33_5);
        when GB_STATE =>
          r <= std_logic_vector(rgb_bgr_sat);
          g <= std_logic_vector(e33_5);
          b <= std_logic_vector(rgr_bgb_sat);
        when others => null;
      end case;
    end if;
  end process STAGE_6_PROC;



  -- The following process tracks the column position accross the image so that
  -- we know when to switch from the Green/Red row to a Blue/Green row, or vice
  -- versa.
  WIDTH_COUNTER_PROC : process(clk, rst, width_counter, eol, new_frame_3)
  begin
    if rst = '1' then
      eol           <= '0';
      width_counter <= (others => '0');
    elsif clk'event and clk = '1' then

      -- If start of new frame, reset logic
      if new_frame_5 = '1' then
        eol           <= '0';
        width_counter <= (others => '0');

      -- If data will be output on the next clock cycle
      elsif valid_5 = '1' then
        -- If we're reaching the end of the row on the next cycle, signal this
        if width_counter = to_unsigned(
          C_IMAGE_WIDTH-2, width_counter'length
          ) then
          eol <= '1';
        else
          eol <= '0';
        end if;

        -- If we're at the end of the row, reset counter, otherwise incrment
        if eol = '1' then
          width_counter <= (others => '0');
        else
          width_counter <= width_counter + 1;
        end if;
      end if;
    end if;
  end process;

  -----------------------------------------------------------------------------
  -- The following state machine tracks Bayer RGB position state.

  -- State machine register
  OUT_SELECT_FSM_REG : process(clk, rst, next_state, first_row_start,
                              new_frame_3)
  begin
    if rst = '1' then
      current_state <= GR_STATE;
    elsif clk'event and clk = '1' then
      -- If new frame, reset state to what it should be at start of first row
      if new_frame_5 = '1' then
        current_state <= first_row_start;
      elsif valid_5 = '1' then
        current_state <= next_state;
      end if;
    end if;
  end process;

  -- Combinational logic for state machine
  OUT_SELECT_FSM_COMB : process(clk, rst, current_state, eol,
                               red_row_start, blue_row_start)
  begin
    -- Default values
    next_state <= current_state;

    case current_state is

      when GR_STATE =>
        if eol = '1' then
          next_state <= blue_row_start;
        else
          next_state <= R_STATE;
        end if;

      when R_STATE =>
        if eol = '1' then
          next_state <= blue_row_start;
        else
          next_state <= GR_STATE;
        end if;

      when B_STATE =>
        if eol = '1' then
          next_state <= red_row_start;
        else
          next_state <= GB_STATE;
        end if;

      when GB_STATE =>
        if eol = '1' then
          next_state <= red_row_start;
        else
          next_state <= B_STATE;
        end if;

      when others => null;
                     
    end case;
  end process;

end imp;


