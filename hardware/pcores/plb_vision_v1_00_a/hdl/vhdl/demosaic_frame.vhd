-------------------------------------------------------------------------------
-- Demosaic Video Frame
--
-- FILE: demosaic_frame.vhd
-- AUTHOR: Wade Fife
-- DATE: June 6, 2006
-- MODIFIED: May 24, 2007
--
-- DESCRIPTION
--
-- This core consists of hardware to demosaic the Bayer RGB output of a digital
-- camera. It contains the 3x3 filter buffer for buffering rows for
-- demosaicing, the demosaicing core, and a core to insert data to compensate
-- for the 3x3 filter buffering.
--
-- The Bayer RGB data is input into this core by asserting the a Bayer pixel on
-- din and asserting wr_en for one clock cycle. The Bayer pattern start
-- position must be input on start_pos when new_frame is asserted. This
-- new_frame input resets the core for a new frame. The RGB pixels are output
-- on the r, b, and b outputs, on cycles in which valid is high.
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity demosaic_frame is
  generic (
    C_IMAGE_WIDTH   : integer := 640;
    C_IMAGE_HEIGHT  : integer := 480;
    C_DATA_WIDTH    : integer := 8;
    C_INSERT_SKIP   : integer := 0;
    C_DEMOSAIC_TYPE : string := "MICROSOFT"
    );     -- Can be 0 or 1
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
end demosaic_frame;


architecture imp of demosaic_frame is

  component filter_buffer_3x3
    generic (
      C_ROW_WIDTH  : integer;
      C_DATA_WIDTH : integer);
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
  end component;

  component filter_buffer_5x5
    generic (
      C_ROW_WIDTH  : integer;
      C_DATA_WIDTH : integer);
    port (
      clk   : in  std_logic;
      rst   : in  std_logic;
      din   : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      wr_en : in  std_logic;
      e11   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e12   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e13   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e14   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e15   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e21   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e22   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e23   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e24   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e25   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e31   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e32   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e33   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e34   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e35   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e41   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e42   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e43   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e44   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e45   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e51   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e52   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e53   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e54   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e55   : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      valid : out std_logic);
  end component;

  component demosaic_linear
    generic (
      C_EDGE_DIRECTED : boolean;
      C_IMAGE_WIDTH   : integer;
      C_DATA_WIDTH    : integer);
    port (
      clk       : in  std_logic;
      rst       : in  std_logic;
      new_frame : in  std_logic;
      start_pos : in  std_logic_vector(1 downto 0);
      e11       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e12       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e13       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e21       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e22       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e23       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e31       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e32       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      e33       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      wr_en     : in  std_logic;
      r         : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      g         : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      b         : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      valid     : out std_logic);
  end component;

  component demosaic_microsoft
    generic (
      C_IMAGE_WIDTH : integer;
      C_DATA_WIDTH  : integer);
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
  end component;

  component filter_output_adjust
    generic (
      C_DATA_WIDTH        : integer;
      C_IMAGE_WIDTH       : integer;
      C_SKIP_SIZE         : integer;
      C_PASS_SIZE         : integer;
      C_INSERT_SIZE       : integer;
      C_BORDER_SIZE       : integer;
      C_INSERT_VALUE      : std_logic_vector;
      C_INSERT_INTERLEAVE : integer);
    port (
      clk   : in  std_logic;
      rst   : in  std_logic;
      busy  : out std_logic;
      din   : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      wr_en : in  std_logic;
      dout  : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      valid : out std_logic);
  end component;

  signal e11 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e12 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e13 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e21 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e22 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e23 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e24 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e31 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e32 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e33 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e34 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e35 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e42 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e43 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e44 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal e53 : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal dem_rgb : std_logic_vector(C_DATA_WIDTH*3-1 downto 0);
  signal dem_r, dem_g, dem_b : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal dem_valid : std_logic;
  signal adjust_dout : std_logic_vector(C_DATA_WIDTH*3-1 downto 0);
  signal adjust_valid : std_logic;
  signal buffer_valid : std_logic;

  signal frame_reset : std_logic;

begin

  frame_reset <= rst or new_frame;
  
  -----------------------------------------------------------------------------
  -- Chose the approriate NxN buffer and demosaic core.
  
  assert C_DEMOSAIC_TYPE = "BILINEAR" or
         C_DEMOSAIC_TYPE = "EDGE_DIRECTED" or
         C_DEMOSAIC_TYPE = "MICROSOFT"
    report "You did not select a valid demosaic mode (C_DEMOSAIC_TYPE). Valid modes are LINEAR and MICROSOFT."
    severity failure;


  GEN_LINEAR : if C_DEMOSAIC_TYPE = "BILINEAR" or
                C_DEMOSAIC_TYPE = "EDGE_DIRECTED" generate

    filter_buffer_3x3_i : filter_buffer_3x3
      generic map (
        C_ROW_WIDTH  => C_IMAGE_WIDTH,
        C_DATA_WIDTH => C_DATA_WIDTH)
      port map (
        clk   => clk,
        rst   => frame_reset,
        din   => din,
        wr_en => wr_en,
        e11   => e11,
        e12   => e12,
        e13   => e13,
        e21   => e21,
        e22   => e22,
        e23   => e23,
        e31   => e31,
        e32   => e32,
        e33   => e33,
        valid => buffer_valid);

    demosaic_linear_i : demosaic_linear
      generic map (
        C_EDGE_DIRECTED => (C_DEMOSAIC_TYPE = "EDGE_DIRECTED"),
        C_IMAGE_WIDTH   => C_IMAGE_WIDTH,
        C_DATA_WIDTH    => C_DATA_WIDTH)
      port map (
        clk       => clk,
        rst       => rst,
        new_frame => new_frame,
        start_pos => start_pos,
        e11       => e11,
        e12       => e12,
        e13       => e13,
        e21       => e21,
        e22       => e22,
        e23       => e23,
        e31       => e31,
        e32       => e32,
        e33       => e33,
        wr_en     => buffer_valid,
        r         => dem_r,
        g         => dem_g,
        b         => dem_b,
        valid     => dem_valid);

    dem_rgb <= dem_r & dem_g & dem_b;

    filter_output_adjust_1: filter_output_adjust
      generic map (
        C_DATA_WIDTH   => C_DATA_WIDTH*3,
        C_IMAGE_WIDTH  => C_IMAGE_WIDTH,
        C_SKIP_SIZE    => 0,
        C_PASS_SIZE    => C_IMAGE_WIDTH*C_IMAGE_HEIGHT-(C_IMAGE_WIDTH+1),
        C_INSERT_SIZE  => C_IMAGE_WIDTH+1,
        C_BORDER_SIZE  => 1,
        C_INSERT_VALUE => std_logic_vector(to_unsigned(0, C_DATA_WIDTH*3)),
        C_INSERT_INTERLEAVE => C_INSERT_SKIP)
      port map (
        clk   => clk,
        rst   => frame_reset,
        busy  => open,
        din   => dem_rgb,
        wr_en => dem_valid,
        dout  => adjust_dout,
        valid => adjust_valid);
    
    r     <= adjust_dout(C_DATA_WIDTH*3-1 downto C_DATA_WIDTH*2);
    g     <= adjust_dout(C_DATA_WIDTH*2-1 downto C_DATA_WIDTH);
    b     <= adjust_dout(C_DATA_WIDTH-1 downto 0);
    valid <= adjust_valid;

  end generate GEN_LINEAR;


  GEN_MICROSOFT : if C_DEMOSAIC_TYPE = "MICROSOFT" generate

    filter_buffer_5x5_i : filter_buffer_5x5
      generic map (
        C_ROW_WIDTH  => C_IMAGE_WIDTH,
        C_DATA_WIDTH => C_DATA_WIDTH)
      port map (
        clk   => clk,
        rst   => frame_reset,
        din   => din,
        wr_en => wr_en,
        e11   => open,
        e12   => open,
        e13   => e13,
        e14   => open,
        e15   => open,
        e21   => open,
        e22   => e22,
        e23   => e23,
        e24   => e24,
        e25   => open,
        e31   => e31,
        e32   => e32,
        e33   => e33,
        e34   => e34,
        e35   => e35,
        e41   => open,
        e42   => e42,
        e43   => e43,
        e44   => e44,
        e45   => open,
        e51   => open,
        e52   => open,
        e53   => e53,
        e54   => open,
        e55   => open,
        valid => buffer_valid);

    demosaic_microsoft_i : demosaic_microsoft
      generic map (
        C_IMAGE_WIDTH => C_IMAGE_WIDTH,
        C_DATA_WIDTH  => C_DATA_WIDTH)
      port map (
        clk       => clk,
        rst       => rst,
        new_frame => new_frame,
        start_pos => start_pos,
        e13       => e13,
        e22       => e22,
        e23       => e23,
        e24       => e24,
        e31       => e31,
        e32       => e32,
        e33       => e33,
        e34       => e34,
        e35       => e35,
        e42       => e42,
        e43       => e43,
        e44       => e44,
        e53       => e53,
        wr_en     => buffer_valid,
        r         => dem_r,
        g         => dem_g,
        b         => dem_b,
        valid     => dem_valid);

    dem_rgb <= dem_r & dem_g & dem_b;

    filter_output_adjust_2: filter_output_adjust
      generic map (
        C_DATA_WIDTH   => C_DATA_WIDTH*3,
        C_IMAGE_WIDTH  => C_IMAGE_WIDTH,
        C_SKIP_SIZE    => 0,
        C_PASS_SIZE    => C_IMAGE_WIDTH*C_IMAGE_HEIGHT-(2*C_IMAGE_WIDTH+2),
        C_INSERT_SIZE  => 2*C_IMAGE_WIDTH+2,
        C_BORDER_SIZE  => 2,
        C_INSERT_VALUE => std_logic_vector(to_unsigned(0, C_DATA_WIDTH*3)),
        C_INSERT_INTERLEAVE => C_INSERT_SKIP)
      port map (
        clk   => clk,
        rst   => frame_reset,
        busy  => open,
        din   => dem_rgb,
        wr_en => dem_valid,
        dout  => adjust_dout,
        valid => adjust_valid);
    
    r     <= adjust_dout(C_DATA_WIDTH*3-1 downto C_DATA_WIDTH*2);
    g     <= adjust_dout(C_DATA_WIDTH*2-1 downto C_DATA_WIDTH);
    b     <= adjust_dout(C_DATA_WIDTH-1 downto 0);
    valid <= adjust_valid;


  end generate GEN_MICROSOFT;

end imp;


