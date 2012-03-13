-------------------------------------------------------------------------------
-- Bayer RGB Demosaicing - Linear Interpolation
--
-- FILE: demosaic_linar.vhd
-- AUTHOR: Wade Fife
-- DATE: May 29, 2006
-- MODIFIED: April 26, 2007
--
-- DESCRIPTION
--
-- Demosaics a standard Bayer RGB patter. The raw Bayer data should be input on
-- the e11..e33 inputs (a 3x3 window) in order to calculate the demosaiced
-- value of the center pixel. The wr_en signal should be held high for one
-- clock cycle at the same time e11..e33 are valid in order to input the
-- data. The valid signal indicates that the r, g, and b output values are
-- valid. The eXX numbering parallels matrix element numbering; that is, e11 is
-- top left, e13 is top right, e33 is bottom right, etc.
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
-- C_EDGE_DIRECTED indicates whether the edge directed method should be used,
-- instead of the traditional biliear or nearest-neighbor averaging. Setting
-- this to true gives slightly better results, but requires a longer clock
-- cycle and takes up more resources. It is slower because both the edge
-- directed and non edge directed versions are mapped to the same pipeline. A
-- faster edge directed version could be created by making a custom
-- implementation for it.
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


entity demosaic_linear is
  generic (
    C_EDGE_DIRECTED : boolean := TRUE;  -- Use edge directed techniqe
    C_IMAGE_WIDTH   : integer := 640;
    C_DATA_WIDTH    : integer := 8);
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
end demosaic_linear;


architecture imp of demosaic_linear is

  constant START_GR : std_logic_vector := "00";
  constant START_R  : std_logic_vector := "01";
  constant START_B  : std_logic_vector := "10";
  constant START_GB : std_logic_vector := "11";

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
  signal e11_0, e12_0, e13_0 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal e21_0, e22_0, e23_0 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal e31_0, e32_0, e33_0 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal valid_0 : std_logic;
  signal new_frame_0 : std_logic;

  -- Stage 1 Signals
  signal center_1 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal h_avg_1 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal v_avg_1 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal p_avg_a_1 : unsigned(C_DATA_WIDTH downto 0);
  signal p_avg_b_1 : unsigned(C_DATA_WIDTH downto 0);
  signal x_avg_a_1 : unsigned(C_DATA_WIDTH downto 0);
  signal x_avg_b_1 : unsigned(C_DATA_WIDTH downto 0);
  signal p_avg_a_grad_1 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal p_avg_b_grad_1 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal x_avg_a_grad_1 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal x_avg_b_grad_1 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal valid_1 : std_logic;
  signal new_frame_1 : std_logic;

  -- Stage 2 Signals
  signal center_2 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal h_avg_2 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal v_avg_2 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal p_avg_2 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal x_avg_2 : unsigned(C_DATA_WIDTH-1 downto 0);
  signal valid_2 : std_logic;
  signal new_frame_2 : std_logic;

  -- Stage 3 Signals

  -- State Machine Signals
  signal next_state, current_state : state_type;

  -- Width Monitor Signals
  signal width_counter : unsigned(RequiredBits(C_IMAGE_WIDTH-2,32)-1 downto 0);
  signal eol : std_logic;               -- End of line indicator
  
begin

  -----------------------------------------------------------------------------
  -- Stage 0
  --
  -- Input registers.
  -----------------------------------------------------------------------------

  -- Pipeline stage 0. Consists of input registers.
  STAGE_0_PROC: process(clk, rst, wr_en, new_frame, start_pos,
                        e11, e12, e13,
                        e21, e22, e23,
                        e31, e32, e33)
  begin
    if rst = '1' then
      valid_0 <= '0';
      new_frame_0 <= '0';
      first_row_start <= GR_STATE;
      red_row_start   <= GR_STATE;
      blue_row_start  <= GR_STATE;
    elsif clk'event and clk = '1' then
      new_frame_0 <= new_frame;
      
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

      -- Don't do anythinng until pixel skip has completed, if enabled.
      valid_0 <= wr_en;
      if wr_en = '1' then
        e11_0 <= unsigned(e11);
        e12_0 <= unsigned(e12);
        e13_0 <= unsigned(e13);
        e21_0 <= unsigned(e21);
        e22_0 <= unsigned(e22);
        e23_0 <= unsigned(e23);
        e31_0 <= unsigned(e31);
        e32_0 <= unsigned(e32);
        e33_0 <= unsigned(e33);
      end if;
    end if;
  end process;


  
  -----------------------------------------------------------------------------
  -- Stage 1
  --
  -- First adder stage.
  -----------------------------------------------------------------------------
 
  STAGE_1_PROC: process (clk, rst, new_frame_0, valid_0,
                         e11_0, e12_0, e13_0,
                         e21_0, e22_0, e23_0,
                         e31_0, e32_0, e33_0)
    variable p_avg_a_diff : signed(C_DATA_WIDTH downto 0);
    variable p_avg_b_diff : signed(C_DATA_WIDTH downto 0);
    variable x_avg_a_diff : signed(C_DATA_WIDTH downto 0);
    variable x_avg_b_diff : signed(C_DATA_WIDTH downto 0);
  begin
    if rst = '1' then
      new_frame_1 <= '0';
      valid_1 <= '0';
    elsif clk'event and clk = '1' then
      new_frame_1 <= new_frame_0;
      valid_1 <= valid_0;

      -- Determine values of simple signals
      center_1 <= e22_0;
      h_avg_1 <=
        resize(shift_right((('0' & e21_0) + ('0' & e23_0)), 1), C_DATA_WIDTH);
      v_avg_1 <=
        resize(shift_right((('0' & e12_0) + ('0' & e32_0)), 1), C_DATA_WIDTH);

      -- Evaluate partial somes for the 'plus' and 'cross' arranged elements.
      p_avg_a_1 <= ('0' & e12_0) + ('0' & e32_0);
      p_avg_b_1 <= ('0' & e21_0) + ('0' & e23_0);
      x_avg_a_1 <= ('0' & e11_0) + ('0' & e33_0);
      x_avg_b_1 <= ('0' & e13_0) + ('0' & e31_0);

      -- If in edge directed mode, computer gradients help in deciding which
      -- pixels to average in the next stage.
      if C_EDGE_DIRECTED then
        -- Calculate gradients
        p_avg_a_diff := ('0' & signed(e12_0)) - ('0' & signed(e32_0));
        p_avg_b_diff := ('0' & signed(e21_0)) - ('0' & signed(e23_0));
        x_avg_a_diff := ('0' & signed(e11_0)) - ('0' & signed(e33_0));
        x_avg_b_diff := ('0' & signed(e13_0)) - ('0' & signed(e31_0));

        -- Take absolute values of the gradients
        
        if p_avg_a_diff(C_DATA_WIDTH) = '1' then
          p_avg_a_grad_1 <= unsigned(resize(-p_avg_a_diff, C_DATA_WIDTH));
        else
          p_avg_a_grad_1 <= unsigned(resize(p_avg_a_diff, C_DATA_WIDTH));
        end if;

        if p_avg_b_diff(C_DATA_WIDTH) = '1' then
          p_avg_b_grad_1 <= unsigned(resize(-p_avg_b_diff, C_DATA_WIDTH));
        else
          p_avg_b_grad_1 <= unsigned(resize(p_avg_b_diff, C_DATA_WIDTH));
        end if;

        if x_avg_a_diff(C_DATA_WIDTH) = '1' then
          x_avg_a_grad_1 <= unsigned(resize(-x_avg_a_diff, C_DATA_WIDTH));
        else
          x_avg_a_grad_1 <= unsigned(resize(x_avg_a_diff, C_DATA_WIDTH));
        end if;

        if x_avg_b_diff(C_DATA_WIDTH) = '1' then
          x_avg_b_grad_1 <= unsigned(resize(-x_avg_b_diff, C_DATA_WIDTH));
        else
          x_avg_b_grad_1 <= unsigned(resize(x_avg_b_diff, C_DATA_WIDTH));
        end if;

      end if;
      
    end if;
  end process STAGE_1_PROC;

  
  
  -----------------------------------------------------------------------------
  -- Stage 2
  --
  -- Second adder stage.
  -----------------------------------------------------------------------------

  STAGE_2_PROC: process(clk, rst, valid_1, center_1,
                        h_avg_1, p_avg_a_1, p_avg_b_1,
                        x_avg_a_1, x_avg_b_1, new_frame_1)
  begin
    if rst = '1' then
      new_frame_2 <= '0';
      valid_2 <= '0';
    elsif clk'event and clk = '1' then
      new_frame_2 <= new_frame_1;
      valid_2 <= valid_1;

      -- These signals fully calculated in previous stage
      center_2 <= center_1;
      h_avg_2 <= h_avg_1;
      v_avg_2 <= v_avg_1;

      if (not C_EDGE_DIRECTED) then
        -- If not edge directed, just take the average for the 'plus' and
        -- 'cross' arranged elements.
        p_avg_2 <= resize(
          shift_right(('0' & p_avg_a_1) + ('0' & p_avg_b_1), 2), C_DATA_WIDTH
          );
        x_avg_2 <= resize(
          shift_right(('0' & x_avg_a_1) + ('0' & x_avg_b_1), 2), C_DATA_WIDTH
          );

      else
        -- If edge directed, choose the pair of pixels with the smallest
        -- gradient and only average that pair.
        
        if p_avg_a_grad_1 < p_avg_b_grad_1 then
          p_avg_2 <= resize(shift_right(p_avg_a_1, 1), C_DATA_WIDTH);
        else
          p_avg_2 <= resize(shift_right(p_avg_b_1, 1), C_DATA_WIDTH);
        end if;

        if x_avg_a_grad_1 < x_avg_b_grad_1 then
          x_avg_2 <= resize(shift_right(x_avg_a_1, 1), C_DATA_WIDTH);
        else
          x_avg_2 <= resize(shift_right(x_avg_b_1, 1), C_DATA_WIDTH);
        end if;
      end if;
      
    end if;
  end process;

  -----------------------------------------------------------------------------
  -- Stage 3
  --
  -- Output selection
  -----------------------------------------------------------------------------
  
  STAGE_3_PROC: process(clk, rst, current_state, center_2,
                        h_avg_2, v_avg_2, p_avg_2, x_avg_2)
  begin
    if rst = '1' then
      valid <= '0';
    elsif clk'event and clk = '1' then
      valid <= valid_2;

      -- Select correct rgb output based on current Bayer patter position.
      case current_state is
        when GR_STATE =>
          r <= std_logic_vector(h_avg_2);
          g <= std_logic_vector(center_2);
          b <= std_logic_vector(v_avg_2);
        when R_STATE =>
          r <= std_logic_vector(center_2);
          g <= std_logic_vector(p_avg_2);
          b <= std_logic_vector(x_avg_2);
        when B_STATE =>
          r <= std_logic_vector(x_avg_2);
          g <= std_logic_vector(p_avg_2);
          b <= std_logic_vector(center_2);
        when GB_STATE =>
          r <= std_logic_vector(v_avg_2);
          g <= std_logic_vector(center_2);
          b <= std_logic_vector(h_avg_2);
        when others => null;
      end case;
    end if;
  end process;

  -- The following process tracks the column position accross the image so that
  -- we know when to switch from the Green/Red row to a Blue/Green row, or vice
  -- versa.
  WIDTH_COUNTER_PROC: process(clk, rst, width_counter, eol, new_frame_2)
  begin
    if rst = '1' then
      eol <= '0';
      width_counter <= (others => '0');
    elsif clk'event and clk = '1' then

      -- If start of new frame, reset logic
      if new_frame_2 = '1' then
        eol <= '0';
        width_counter <= (others => '0');

      elsif valid_2 = '1' then
        -- If we're reaching the end of the row on the next cycle, signal this
        if width_counter = to_unsigned(
                             C_IMAGE_WIDTH-2, width_counter'length
                           ) then
          eol <= '1';
        else
          eol <= '0';
        end if;

        -- If we're at the end of the row, reset counter, otherwise incrment
        if eol  = '1' then
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
  OUT_SELECT_FSM_REG: process(clk, rst, next_state, first_row_start,
                              new_frame_2)
  begin
    if rst = '1' then
      current_state <= GR_STATE;
    elsif clk'event and clk = '1' then
      -- If new frame, reset state to what it should be at start of first row
      if new_frame_2 = '1' then
        current_state <= first_row_start;
      elsif valid_2 = '1' then
        current_state <= next_state;
      end if;
    end if;
  end process;

  -- Combinational logic for state machine
  OUT_SELECT_FSM_COMB: process(clk, rst, current_state, eol,
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


