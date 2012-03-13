------------------------------------------------------------------------------
-- Performs Standard Image Processing for a Raw Bayer Image Sensor
--
-- FILE: camera_processor.vhd
-- AUTHOR: Wade Fife
-- DATE: May 11, 2007
-- MODIFIED: May 11, 2007
--
--
-- DESCRIPTION
--
-- This core takes the raw Bayer output of an image sensor then performs
-- demosaicing and color correction to produce a quality RGB image. Optionally,
-- it can also round the resulting pixel values to an appropriate bit width.
--
-- Data is input into the core by asserting din with the next pixel value from
-- the camera and asserting wr_en for one clock cycle. Valid RGB pixels are
-- output on the r, g, and b outputs and rgb_valid is asserted for one clock
-- cycle to indicate that the r, g, and b outputs are valid.
--
--
-- GENERICS
--
--   C_IMAGE_WIDTH    The input/output image width, in pixel columns
--   C_IMAGE_HEIGHT   The input/output image height, in pixel rows
--   C_CAM_DATA_WIDTH The bit width of camera's output (i.e., width of din)
--   C_X_DATA_WIDTH   The desired widths of the camera's R, G, and B outputs
--   C_DEMOSAIC_TYPE  Demosaic method to use. At this writing, can be
--                    "BILINEAR", "EDGE_DIRECTED", or "MICROSOFT".
--   C_INSERT_SKIP    Number delay cycles to insert between each pixel when
--                    inserting data into the data stream.
--   C_AXX            Color correction matrix elements.
-- 
--                      | rc | = | A11 A12 A13 |   | r |
--                      | gc | = | A21 A22 A23 | X | g |
--                      | gb | = | A31 A32 A33 |   | b |
--
-- PORTS
--
--   clk       : Core clock input
--   rst       : Core reset input (active high)
--   bayer_pos : Indicates where in the Bayer pattern the first pixel occurs.
--               Can be "00" for green pixel on red/green row, "01" for red
--               pixel, "10" for blue pixel, and "11" for green pixel on
--               blue/green row. This input is read when new_frame asserted.
--   cc_bypass : Color correction bypass. When '1', color correction is not
--               performed. Change only between frames.
--   new_frame : Signal used to indicate a new frame is coming. This should be
--               asserted during vertical blanking and essentially resets the
--               core for the next frame.
--   din       : Raw data input, coming from camera.
--   wr_en     : Write enable for din
--   r         : Red pixel component output
--   g         : Green pixel component output
--   b         : Blue pixel component output
--   rgb_valid : Valid signal for r, g, and b
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity camera_processor is
  generic (
    C_IMAGE_WIDTH    : integer := 640;
    C_IMAGE_HEIGHT   : integer := 480;
    C_CAM_DATA_WIDTH : integer := 10;
    C_R_DATA_WIDTH   : integer := 8;
    C_G_DATA_WIDTH   : integer := 8;
    C_B_DATA_WIDTH   : integer := 8;
    C_DEMOSAIC_TYPE  : string  := "MICROSOFT";
    C_INSERT_SKIP    : integer := 1;
    C_CC_A11         : real :=  1.219;
    C_CC_A12         : real := -0.284;
    C_CC_A13         : real :=  0.066;
    C_CC_A21         : real := -0.422;
    C_CC_A22         : real :=  1.618;
    C_CC_A23         : real := -0.196;
    C_CC_A31         : real := -0.309;
    C_CC_A32         : real := -0.365;
    C_CC_A33         : real :=  1.674);
  port (
    clk       : in std_logic;
    rst       : in std_logic;

    bayer_pos : in std_logic_vector(1 downto 0);
    cc_bypass : in std_logic;
    
    new_frame : in std_logic;

    din       : in std_logic_vector(C_CAM_DATA_WIDTH-1 downto 0);
    wr_en     : in std_logic;

    -- Progressive scan, intermediate outputs
    r         : out std_logic_vector(C_R_DATA_WIDTH-1 downto 0);
    g         : out std_logic_vector(C_G_DATA_WIDTH-1 downto 0);
    b         : out std_logic_vector(C_B_DATA_WIDTH-1 downto 0);
    rgb_valid : out std_logic);
  
end camera_processor;


architecture imp of camera_processor is

  component demosaic_frame
    generic (
      C_IMAGE_WIDTH   : integer;
      C_IMAGE_HEIGHT  : integer;
      C_DATA_WIDTH    : integer;
      C_INSERT_SKIP   : integer;
      C_DEMOSAIC_TYPE : string);
    port (
      clk       : in  std_logic;
      rst       : in  std_logic;
      start_pos : in  std_logic_vector(1 downto 0);
      new_frame : in  std_logic;
      din       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      wr_en     : in  std_logic;
      r         : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      g         : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      b         : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      valid     : out std_logic);
  end component;

  component color_correction
    generic (
      C_A11         : real;
      C_A12         : real;
      C_A13         : real;
      C_A21         : real;
      C_A22         : real;
      C_A23         : real;
      C_A31         : real;
      C_A32         : real;
      C_A33         : real;
      C_DATA_WIDTH  : integer;
      C_WHOLE_WIDTH : integer;
      C_FRAC_WIDTH  : integer); 
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
  end component;

  component round
    generic (
      C_IN_WIDTH  : integer;
      C_OUT_WIDTH : integer;
      C_USE_REG   : boolean);
    port (
      clk   : in  std_logic;
      rst   : in  std_logic;
      wr_en : in  std_logic;
      din   : in  std_logic_vector(C_IN_WIDTH-1 downto 0);
      valid : out std_logic;
      dout  : out std_logic_vector(C_OUT_WIDTH-1 downto 0));
  end component;

  component rgb_sum
    generic (
      C_DATA_WIDTH   : integer;
      C_SUM_WIDTH    : integer;
      C_FRAME_WIDTH  : integer;
      C_FRAME_HEIGHT : integer);
    port (
      clk        : in  std_logic;
      rst        : in  std_logic;
      frame_done : in  std_logic;
      wr_en      : in  std_logic;
      r          : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      g          : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      b          : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      r_sum      : out std_logic_vector(C_SUM_WIDTH-1 downto 0);
      g_sum      : out std_logic_vector(C_SUM_WIDTH-1 downto 0);
      b_sum      : out std_logic_vector(C_SUM_WIDTH-1 downto 0));
  end component;

  -- Demosaic signals
  signal dem_r, dem_g, dem_b : std_logic_vector(C_CAM_DATA_WIDTH-1 downto 0);
  signal dem_valid : std_logic;

  -- Color corrected signals
  signal cc_r_t, cc_g_t, cc_b_t : std_logic_vector(C_CAM_DATA_WIDTH-1 downto 0);
  signal cc_valid_t : std_logic;
  signal cc_r, cc_g, cc_b : std_logic_vector(C_CAM_DATA_WIDTH-1 downto 0);
  signal cc_valid : std_logic;

  -- Interframe control registers
  signal new_frame_rst : std_logic;
  signal bayer_pos_reg : std_logic_vector(1 downto 0);

begin

  
  -----------------------------------------------------------------------------
  -- NEW FRAME RESET ----------------------------------------------------------

  -- This process controls the inter-frame reset signal and the bayer_pos
  -- register update.
  FRAME_RESET_PROC: process (clk, rst, new_frame, bayer_pos)
  begin
    if rst = '1' then
      new_frame_rst <= '1';
    elsif clk'event and clk = '1' then
      new_frame_rst <= new_frame;

      -- Update bayer position for this frame
      if new_frame = '1' then
        bayer_pos_reg <= bayer_pos;
      end if;
    end if;
  end process;

  
  -----------------------------------------------------------------------------
  -- DEMOSAIC -----------------------------------------------------------------

  demosaic_frame_i: demosaic_frame
    generic map (
      C_IMAGE_WIDTH   => C_IMAGE_WIDTH,
      C_IMAGE_HEIGHT  => C_IMAGE_HEIGHT,
      C_DATA_WIDTH    => C_CAM_DATA_WIDTH,
      C_INSERT_SKIP   => C_INSERT_SKIP,
      C_DEMOSAIC_TYPE => C_DEMOSAIC_TYPE)
    port map (
      clk       => clk,
      rst       => rst,
      start_pos => bayer_pos_reg,
      new_frame => new_frame_rst,
      din       => din,
      wr_en     => wr_en,
      r         => dem_r,
      g         => dem_g,
      b         => dem_b,
      valid     => dem_valid);

  
  -----------------------------------------------------------------------------
  -- COLOR CORRECTION ---------------------------------------------------------

  color_correction_i: color_correction
    generic map (
      C_A11 => C_CC_A11,
      C_A12 => C_CC_A12,
      C_A13 => C_CC_A13,
      C_A21 => C_CC_A21,
      C_A22 => C_CC_A22,
      C_A23 => C_CC_A23,
      C_A31 => C_CC_A31,
      C_A32 => C_CC_A32,
      C_A33 => C_CC_A33,
      C_DATA_WIDTH  => C_CAM_DATA_WIDTH,
      C_WHOLE_WIDTH => 2,
      C_FRAC_WIDTH  => C_CAM_DATA_WIDTH)
    port map (
      clk   => clk,
      rst   => rst,
      r     => dem_r,
      g     => dem_g,
      b     => dem_b,
      wr_en => dem_valid,
      rp    => cc_r_t,
      gp    => cc_g_t,
      bp    => cc_b_t,
      valid => cc_valid_t);

  BYPASS_CC: process (cc_bypass,
                      dem_r, dem_g, dem_b, dem_valid,
                      cc_r_t, cc_g_t, cc_b_t, cc_valid_t)
  begin
    if cc_bypass = '1' then
      cc_valid <= dem_valid;
      cc_r <= dem_r;
      cc_g <= dem_g;
      cc_b <= dem_b;
    else
      cc_valid <= cc_valid_t;
      cc_r <= cc_r_t;
      cc_g <= cc_g_t;
      cc_b <= cc_b_t;
    end if;
  end process BYPASS_CC;

  
  -----------------------------------------------------------------------------
  -- ROUNDING -----------------------------------------------------------------

  -- Make sure output is no bigger than input
  assert C_R_DATA_WIDTH <= C_CAM_DATA_WIDTH and
         C_G_DATA_WIDTH <= C_CAM_DATA_WIDTH and
         C_B_DATA_WIDTH <= C_CAM_DATA_WIDTH
    report "C_OUT_DATA_WIDTH cannot be greater than C_CAM_DATA_WIDTH."
    severity failure;

  -- Make that if any one output is smaller than the input that all outputs are
  -- smaller than the input. This is a limitation caused by round core, since
  -- it will eliminate any pipeline stages if the input size equals the output
  -- size. Were this to occure, the R, G, and B outputs might not be in phase.
  -- This is fixable, but is a limitation of the current implementation.
  assert (
           C_R_DATA_WIDTH = C_CAM_DATA_WIDTH and
           C_G_DATA_WIDTH = C_CAM_DATA_WIDTH and
           C_B_DATA_WIDTH = C_CAM_DATA_WIDTH
         )
         or
         (
           C_R_DATA_WIDTH < C_CAM_DATA_WIDTH and
           C_G_DATA_WIDTH < C_CAM_DATA_WIDTH and
           C_B_DATA_WIDTH < C_CAM_DATA_WIDTH
         )
    report "If any of C_X_DATA_WIDTH are smaller than C_CAM_DATA_WIDTH they must all be smaller."
    severity failure;

  -- Note that the round cores pass the data straight through if C_IN_WIDTH
  -- equals C_OUT_WIDTH.
  round_r: round
    generic map (
      C_IN_WIDTH  => C_CAM_DATA_WIDTH,
      C_OUT_WIDTH => C_R_DATA_WIDTH,
      C_USE_REG   => TRUE)
    port map (
      clk   => clk,
      rst   => rst,
      wr_en => cc_valid,
      din   => cc_r,
      valid => rgb_valid,
      dout  => r);
  round_g: round
    generic map (
      C_IN_WIDTH  => C_CAM_DATA_WIDTH,
      C_OUT_WIDTH => C_G_DATA_WIDTH,
      C_USE_REG   => TRUE)
    port map (
      clk   => clk,
      rst   => rst,
      wr_en => cc_valid,
      din   => cc_g,
      valid => open,
      dout  => g);
  round_b: round
    generic map (
      C_IN_WIDTH  => C_CAM_DATA_WIDTH,
      C_OUT_WIDTH => C_B_DATA_WIDTH,
      C_USE_REG   => TRUE)
    port map (
      clk   => clk,
      rst   => rst,
      wr_en => cc_valid,
      din   => cc_b,
      valid => open,
      dout  => b);

end imp;


