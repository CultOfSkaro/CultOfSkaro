-------------------------------------------------------------------------------
-- Color Correction
-- (multiplication of RGB values by a color correction matrix)
--
-- FILE: ccm.vhd
-- AUTHOR: Aaron Dennis
-- DATE: March 12, 2007
-- MODIFIED:
--
--
-- DESCRIPTION: 
--
-- Multiplies RGB values by a color correction matrix. The color correction
-- matrix modifies the color values in order to compensate for the
-- characteristics of an image sensor's filters and the lighting conditions
-- while acquiring an image.  New coefficients for the matrix are needed if the
-- image sensor or lighting conditions change.
--
-- Data is written to the core by asserting r, g, and b with the RGB pixel data
-- and holding wr_en high for one clock cycle. After conversion, the corrected
-- RGB data is asserted on the rc, gc, and bc outputs and valid is held high
-- for one clock cycle. No flow control is supported. You can adjust the pixel
-- input/output width by changing the C_DATA_WIDTH generic. The precision of
-- the fixed point computation can be adjusted by changing the C_FRAC_WIDTH
-- generic. Up to 30-bit precision is supported, but high precision comes at a
-- great cost.
--
-- The color correction matrix is a 3x3 matrix.  Its coefficents are labeled
-- a11 throught a33.  It is multiplied by r, g, and b input values to produce
-- rc, gc, and bc output values.  In matrix form the arithmetic is as follows:
--
--         CCM         INPUT   OUTPUT
--         ===         =====   ======
--   | a11 a12 a13 |   | r |   | rc |
--   | a21 a22 a23 | X | g | = | gc |
--   | a31 a32 a33 |   | b |   | bc |
--
-- The equations in algebraic form are as follows:
--
-- rc = a11*r + a12*g + a13*b
-- gc = a21*r + a22*g + a23*b
-- bc = a31*r + a32*g + a33*b
--
-- After the multiplication, the equations are referred to as follows:
--
-- rc = r_r + r_g + r_b
-- gc = g_r + g_g + g_b
-- bc = b_r + b_g + b_b
--
--
-- MATRIX COEFFICIENTS
--
-- The color correction matrix coefficients can be set by setting the generics
-- C_A11 through C_A33.  C_AIJ are real-valued generics that the core converts
-- to fixed-point format.
--
--
-- VALUE RANGES
--
-- The input r, g, and b values can range from 0-255.
--
-- In practice the color correction matrix diagonal coefficients are close to a
-- value of 1 and the other coefficients are close to 0.  This core assumes
-- that the coefficients are less than 2 and greater than -1.  In other terms:
--
-- -1 < aij < 2, where i and j range from 1-3.
--
-- The coefficients will be fixed-point numbers with the whole part taking two
-- bits (which includes the "sign" bit.)  Being a two's complement number, the
-- coefficient will have a range of -2 <= aij <= 1.xx where the ".xx" refers to
-- the maximum possible contribution of the fraction part of the coefficient.
-- This range is larger than the range stated above; we won't use the full
-- range allowed by the number of bits we are using.
--
--
-- PRECISION
--
-- The color correction matrix coefficients will be fixed-point numbers. The
-- whole part will take two bits.  The fraction part will take C_FRAC_WIDTH
-- bits.
--
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

entity color_correction is
  generic (
    C_A11         : real    := 1.0970;   -- @DKW put in values measured from V024
    C_A12         : real    := -0.3300;  --
    C_A13         : real    := 0.3837;   --
    C_A21         : real    := -0.0943;
    C_A22         : real    := 0.6429;
    C_A23         : real    := 0.6722;
    C_A31         : real    := -0.0360;
    C_A32         : real    := -0.7999;
    C_A33         : real    := 3.4709;
    C_DATA_WIDTH  : integer := 8;       -- Width of input/output color values
    C_WHOLE_WIDTH : integer := 3;	-- @DKW Extended for a larger range
    C_FRAC_WIDTH  : integer := 8);

  port (
    clk   : in  std_logic;
    rst   : in  std_logic;
    r     : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    g     : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    b     : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    wr_en : in  std_logic;
    rp    : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    gp    : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    bp    : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    valid : out std_logic);
end color_correction;

architecture Behavioral of color_correction is

  constant C_FIXED_WIDTH : integer := C_WHOLE_WIDTH + C_FRAC_WIDTH;

  -- Constants used in converting the real-valued matrix coefficents
  -- (C_A11, C_A12, etc.) to fixed-point values (a11, a12, etc.)
  constant FLOAT2FIXED_BITS : unsigned(C_FRAC_WIDTH downto 0) := '1' & to_unsigned(0, C_FRAC_WIDTH);	--1.0
  constant FLOAT2FIXED_REAL : real                            := real(to_integer(FLOAT2FIXED_BITS));

  -- Color correction matrix coefficients
  constant a11 : signed(C_FIXED_WIDTH-1 downto 0) := to_signed(integer(C_A11 * FLOAT2FIXED_REAL), C_FIXED_WIDTH);
  constant a12 : signed(C_FIXED_WIDTH-1 downto 0) := to_signed(integer(C_A12 * FLOAT2FIXED_REAL), C_FIXED_WIDTH);
  constant a13 : signed(C_FIXED_WIDTH-1 downto 0) := to_signed(integer(C_A13 * FLOAT2FIXED_REAL), C_FIXED_WIDTH);
  constant a21 : signed(C_FIXED_WIDTH-1 downto 0) := to_signed(integer(C_A21 * FLOAT2FIXED_REAL), C_FIXED_WIDTH);
  constant a22 : signed(C_FIXED_WIDTH-1 downto 0) := to_signed(integer(C_A22 * FLOAT2FIXED_REAL), C_FIXED_WIDTH);
  constant a23 : signed(C_FIXED_WIDTH-1 downto 0) := to_signed(integer(C_A23 * FLOAT2FIXED_REAL), C_FIXED_WIDTH);
  constant a31 : signed(C_FIXED_WIDTH-1 downto 0) := to_signed(integer(C_A31 * FLOAT2FIXED_REAL), C_FIXED_WIDTH);
  constant a32 : signed(C_FIXED_WIDTH-1 downto 0) := to_signed(integer(C_A32 * FLOAT2FIXED_REAL), C_FIXED_WIDTH);
  constant a33 : signed(C_FIXED_WIDTH-1 downto 0) := to_signed(integer(C_A33 * FLOAT2FIXED_REAL), C_FIXED_WIDTH);

  -- Constants for assigning signal widths
  constant DATA_EXT_WIDTH     : natural := C_DATA_WIDTH + 1;
  constant FRAC_BITS_TO_KEEP  : natural := 3;  -- @DKW extended the number of extra bits to keep from the multiplier
  constant MULT_RESULT_WIDTH  : natural := DATA_EXT_WIDTH + C_WHOLE_WIDTH + FRAC_BITS_TO_KEEP;
  constant ADD_RESULT_WIDTH   : natural := MULT_RESULT_WIDTH + 1;
  constant ROUND_RESULT_WIDTH : natural := ADD_RESULT_WIDTH - FRAC_BITS_TO_KEEP;
  constant ROUND : signed(ADD_RESULT_WIDTH-1 downto 0) := to_signed(0, ADD_RESULT_WIDTH-FRAC_BITS_TO_KEEP)&'1' & to_signed(0, FRAC_BITS_TO_KEEP-1);  -- 0.5  @DKW added

  -- Stage 0 signals (Store inputs in registers)
  signal r_0, g_0, b_0 : signed(DATA_EXT_WIDTH-1 downto 0);
  signal en_0          : std_logic;

  -- Stage 1 signals (Do multiplication)
  signal r_r, r_g, r_b : signed(MULT_RESULT_WIDTH-1 downto 0);
  signal g_r, g_g, g_b : signed(MULT_RESULT_WIDTH-1 downto 0);
  signal b_r, b_g, b_b : signed(MULT_RESULT_WIDTH-1 downto 0);
  signal en_1          : std_logic;

  -- Stage 2 signals (Do addition and rounding)
  signal r_2, g_2, b_2 : signed(ROUND_RESULT_WIDTH-1 downto 0);
  signal en_2          : std_logic;

  -- Stage 3 signals (Do saturation)
  signal r_3, g_3, b_3 : signed(C_DATA_WIDTH-1 downto 0);
  signal en_3          : std_logic;
  

begin
  assert C_A11 < 3.99 and C_A11 > 0.0			-- @DKW extended ranges
    report "Invalid color correction matrix coefficient"
    severity error;
  assert C_A12 < 1.0 and C_A12 > -1.0
    report "Invalid color correction matrix coefficient"
    severity error;
  assert C_A13 < 1.0 and C_A13 > -1.0
    report "Invalid color correction matrix coefficient"
    severity error;
  assert C_A21 < 1.0 and C_A21 > -1.0
    report "Invalid color correction matrix coefficient"
    severity error;
  assert C_A22 < 3.99 and C_A22 > 0.0
    report "Invalid color correction matrix coefficient"
    severity error;
  assert C_A23 < 1.0 and C_A23 > -1.0
    report "Invalid color correction matrix coefficient"
    severity error;
  assert C_A31 < 1.0 and C_A31 > -1.0
    report "Invalid color correction matrix coefficient"
    severity error;
  assert C_A32 < 1.0 and C_A32 > -1.0
    report "Invalid color correction matrix coefficient"
    severity error;
  assert C_A33 < 3.99 and C_A33 > 0.0
    report "Invalid color correction matrix coefficient"
    severity error;


  -----------------------------------------------------------------------------
  -- STAGE 0: Input registers
  -----------------------------------------------------------------------------

  STAGE_0_PROC : process (clk, rst, r, g, b, wr_en)
  begin
    if rst = '1' then
      en_0 <= '0';
      r_0  <= (others => '0');
      g_0  <= (others => '0');
      b_0  <= (others => '0');
    elsif clk'event and clk = '1' then
      r_0  <= signed('0' & r);
      g_0  <= signed('0' & g);
      b_0  <= signed('0' & b);
      en_0 <= wr_en;
    end if;
  end process;


  -----------------------------------------------------------------------------
  -- STAGE 1: Multiplication
  -----------------------------------------------------------------------------

  STAGE_1_PROC : process (clk, rst, en_0, r_0, g_0, b_0)
    variable temp : signed(DATA_EXT_WIDTH+C_FIXED_WIDTH-1 downto 0);
  begin
    if rst = '1' then
      en_1 <= '0';
      r_r  <= (others => '0');
      r_g  <= (others => '0');
      r_b  <= (others => '0');
      g_r  <= (others => '0');
      g_g  <= (others => '0');
      g_b  <= (others => '0');
      b_r  <= (others => '0');
      b_g  <= (others => '0');
      b_b  <= (others => '0');
    elsif clk'event and clk = '1' then
      en_1 <= en_0;

      -- Perform multiplication
      -- OPERAND SIZES
      -- aij is a C_FIXED_WIDTH-bit vector
      -- r_0, g_0, b_0 are (C_DATA_WIDTH+1)-bit vectors
      -- RESULT SIZE
      -- The result of the multiplication is a
      -- (C_FIXED_WIDTH + (C_DATA_WIDTH+1) + 1)-bit vector.
      -- TRUNCATION
      -- To save hardware we throw away all but FRAC_BITS_TO_KEEP of the
      -- fractional-part bits.

      temp := a11 * r_0;
      r_r  <= temp(temp'left downto C_FRAC_WIDTH-FRAC_BITS_TO_KEEP);

      temp := a12 * g_0;
      r_g  <= temp(temp'left downto C_FRAC_WIDTH-FRAC_BITS_TO_KEEP);

      temp := a13 * b_0;
      r_b  <= temp(temp'left downto C_FRAC_WIDTH-FRAC_BITS_TO_KEEP);

      temp := a21 * r_0;
      g_r  <= temp(temp'left downto C_FRAC_WIDTH-FRAC_BITS_TO_KEEP);

      temp := a22 * g_0;
      g_g  <= temp(temp'left downto C_FRAC_WIDTH-FRAC_BITS_TO_KEEP);

      temp := a23 * b_0;
      g_b  <= temp(temp'left downto C_FRAC_WIDTH-FRAC_BITS_TO_KEEP);

      temp := a31 * r_0;
      b_r  <= temp(temp'left downto C_FRAC_WIDTH-FRAC_BITS_TO_KEEP);

      temp := a32 * g_0;
      b_g  <= temp(temp'left downto C_FRAC_WIDTH-FRAC_BITS_TO_KEEP);

      temp := a33 * b_0;
      b_b  <= temp(temp'left downto C_FRAC_WIDTH-FRAC_BITS_TO_KEEP);

    end if;
  end process;


  -----------------------------------------------------------------------------
  -- STAGE 2: Addition and Rounding
  -----------------------------------------------------------------------------

  STAGE_2_PROC : process (clk, rst, en_1,
                         r_r, r_g, r_b,
                         g_r, g_g, g_b,
                         b_r, b_g, b_b)
    variable sum_r : signed(ADD_RESULT_WIDTH-1 downto 0);
    variable sum_g : signed(ADD_RESULT_WIDTH-1 downto 0);
    variable sum_b : signed(ADD_RESULT_WIDTH-1 downto 0);
  begin
    if rst = '1' then
      en_2 <= '0';
      r_2  <= (others => '0');
      g_2  <= (others => '0');
      b_2  <= (others => '0');
    elsif clk'event and clk = '1' then
      en_2 <= en_1;

      -- Do addition and rounding
      sum_r := resize(r_r, ADD_RESULT_WIDTH) +
               resize(r_g, ADD_RESULT_WIDTH) +
               resize(r_b, ADD_RESULT_WIDTH) + ROUND;
      sum_g := resize(g_r, ADD_RESULT_WIDTH) +
               resize(g_g, ADD_RESULT_WIDTH) +
               resize(g_b, ADD_RESULT_WIDTH) + ROUND;
      sum_b := resize(b_r, ADD_RESULT_WIDTH) +
               resize(b_g, ADD_RESULT_WIDTH) +
               resize(b_b, ADD_RESULT_WIDTH) + ROUND;

      -- truncate after rounding
      r_2 <= sum_r(sum_r'left downto FRAC_BITS_TO_KEEP);
      g_2 <= sum_g(sum_g'left downto FRAC_BITS_TO_KEEP);
      b_2 <= sum_b(sum_b'left downto FRAC_BITS_TO_KEEP);

    end if;
  end process;


  -----------------------------------------------------------------------------
  -- STAGE 3: Saturation Block
  -----------------------------------------------------------------------------

  STAGE_3_PROC : process (clk, rst, en_2,
                         r_2, g_2, b_2)
  begin
    if rst = '1' then
      en_3 <= '0';
      r_3  <= (others => '0');
      g_3  <= (others => '0');
      b_3  <= (others => '0');
    elsif clk'event and clk = '1' then
      en_3 <= en_2;

      if r_2(ROUND_RESULT_WIDTH-1) = '1' then	-- negative underflow --> 0
        r_3 <= (others => '0');
      elsif unsigned(r_2(ROUND_RESULT_WIDTH-2 downto C_DATA_WIDTH)) /= 0 then	-- positive overflow --> 255
        r_3 <= (others => '1');
      else
        r_3 <= r_2(C_DATA_WIDTH-1 downto 0);
      end if;

      if g_2(ROUND_RESULT_WIDTH-1) = '1' then
        g_3 <= (others => '0');
      elsif unsigned(g_2(ROUND_RESULT_WIDTH-2 downto C_DATA_WIDTH)) /= 0 then
        g_3 <= (others => '1');
      else
        g_3 <= g_2(C_DATA_WIDTH-1 downto 0);
      end if;

      if b_2(ROUND_RESULT_WIDTH-1) = '1' then
        b_3 <= (others => '0');
      elsif unsigned(b_2(ROUND_RESULT_WIDTH-2 downto C_DATA_WIDTH)) /= 0 then
        b_3 <= (others => '1');
      else
        b_3 <= b_2(C_DATA_WIDTH-1 downto 0);
      end if;
      
    end if;
  end process;

  -- Send results to the output
  rp    <= std_logic_vector(r_3);
  gp    <= std_logic_vector(g_3);
  bp    <= std_logic_vector(b_3);
  valid <= en_3;

end Behavioral;

