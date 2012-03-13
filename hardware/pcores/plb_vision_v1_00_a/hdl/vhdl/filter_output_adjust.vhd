-------------------------------------------------------------------------------
-- 2D Filter Output Adjuster
--
-- FILE: filter_output_adjust.vhd
-- AUTHOR: Wade Fife
-- DATE: May 22, 2007
-- MODIFIED: May 22, 2007
--
-- DESCRIPTION
--
-- The purpose of this module is insert and replace data in the data stream
-- coming from a 2D filter to eliminate invalid data and insert missing data.
--
-- An NxN filter core has three potential shortcomings. The first is that the
-- first floor(N/2)*(ImageWidth+1) pixels can be invalid in that they represent
-- regions outside the image. The second is that the filter may stop outputting
-- data floor(N/2)*(ImageWidth+1) pixels too early. The third is that the
-- border pixels are also usually invalid, since the NxN kernel overlaps the
-- edge of the image and wraps around to the other side of the image.
--
-- This core address all three of these issues. It can be placed on the output
-- after the filter kernel application to skip the initial bogus pixels, add
-- empty pixels at the end of the image, and replace the border pixels with
-- empty pixels. The "empty" value is configurable.
--
-- In summary, the core skips the first C_SKIP_SIZE words of the frame, lets
-- the next C_PASS_SIZE words through (except the C_BORDER_PIXELS border pixels
-- on both ends of each line, which are replaced by C_INSERT_VALUE), then
-- inserts C_INSERT_SIZE words of value C_INSERT_VALUE into the data stream,
-- delaying C_INSERT_INTERLEAVE cycles between each insert.
--
-- The busy signal is used to indicate when data is being inserted into the
-- data stream. Writes to the input are ignored when busy is high.
--
-- GENERICS
--
-- C_DATA_WIDTH  : The bit width of din and dout.
-- C_IMAGE_WIDTH : The width of the image in din-sized words.
-- C_SKIP_SIZE   : Number of pixel words to skip at the beginning of a frame.
-- C_PASS_SIZE   : Number of image pixels to pass through unchanged for the
--                 bulk of the frame.
-- C_INSERT_SIZE : Number of pixels to insert into the data stream at the end
--                 of a frame.
-- C_BORDER_SIZE : Number of pixels that make up border to replace (e.g., 1
--                 for 3x3 filter, 2 for 5x5 filter)
-- C_INSERT_VALUE : Value of word to insert for border and at end of frame.
-- C_INSERT_INTERLEAVE : Number clock cycles to wait between each inserted
--                       pixel at the end of frame.
--
-- NOTES
--
-- * The core ignores input while it's inserting data.
--
-- * This core is really a bit complicated. Make sure you thoroughly test the
--   core after making any changes.
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity filter_output_adjust is
  generic (
    C_DATA_WIDTH        : integer := 24;
    C_IMAGE_WIDTH       : integer := 640;
    C_SKIP_SIZE         : integer := 1*(640+1);
    C_PASS_SIZE         : integer := 480*640-2*(640+1);
    C_INSERT_SIZE       : integer := 1*(640+1);
    C_BORDER_SIZE       : integer := 2;
    C_INSERT_VALUE      : std_logic_vector := x"000000";
    C_INSERT_INTERLEAVE : integer := 1);
  port (
    clk       : in  std_logic;
    rst       : in  std_logic;
    busy      : out std_logic;
    din       : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
    wr_en     : in  std_logic;
    dout      : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    valid     : out std_logic);
end filter_output_adjust;


architecture imp of filter_output_adjust is

  -- Function to calculate number of bits needed to represent a given positive
  -- integer (0 to 0x7FFFFFFF).
  function RequiredBits (int_num : integer)
  return integer is
    variable test_vector : unsigned(0 to 31);
  begin
    assert int_num >= 0
      report "RequiredBits() integer must be non-negative!" severity failure;
    assert int_num <= 2147483647
      report "Integer exceeds allwed range" severity failure;
    test_vector := (others => '0');
    for i in 1 to 31 loop
      test_vector := test_vector(1 to 31) & '1';
      if to_integer(test_vector) >= int_num then
        return i;
      end if;
    end loop;
    return 31;
  end function RequiredBits;

  -- Counter registers
  signal pixel_count :
    unsigned(RequiredBits(C_SKIP_SIZE+C_PASS_SIZE+C_INSERT_SIZE)-1 downto 0);
  signal col_count : unsigned(RequiredBits(C_IMAGE_WIDTH-1)-1 downto 0);
  signal interleave_count :
    unsigned(RequiredBits(C_INSERT_INTERLEAVE)-1 downto 0);

  -- State machine types and signals
  type state_type is (IDLE_STATE, SKIP_STATE, PASS_STATE, INSERT_STATE);
  signal state, next_state : state_type;

  -- Input registers
  signal data_reg : std_logic_vector(C_DATA_WIDTH-1 downto 0);
  signal valid_reg : std_logic;

  -- Output control signals
  signal insert_valid, do_insert : std_logic;
  signal border_insert : std_logic;

  signal col_count_clr : std_logic;
  signal ignore_input : std_logic;

begin

  assert C_BORDER_SIZE < C_IMAGE_WIDTH / 2
    report "C_BORDER_SIZE must be < C_IMAGE_WIDTH / 2"
    severity failure;


  busy <= ignore_input;
  
  -----------------------------------------------------------------------------
  -- Mux to select correct data based on state
  
  OUTPUT_MUX: process (state, data_reg, valid_reg, insert_valid, border_insert)
  begin
    case state is

      when SKIP_STATE =>
        dout <= C_INSERT_VALUE;
        valid <= '0';
      
      when PASS_STATE =>
        if border_insert = '1' then
          dout <= C_INSERT_VALUE;
          valid <= valid_reg;
        else
          dout <= data_reg;
          valid <= valid_reg;
        end if;

      when INSERT_STATE =>
        dout <= C_INSERT_VALUE;
        valid <= insert_valid;
        
      when others =>
        dout <= data_reg;
        valid <= valid_reg;
        
    end case;
  end process;

  
  -----------------------------------------------------------------------------
  -- Input register to load din and wr_en
  
  INPUT_REG_PROC: process (clk, rst, din, wr_en)
  begin
    if rising_edge(clk) then
      if rst = '1' then
        data_reg <= (others => '0');
        valid_reg <= '0';
      else
        if wr_en = '1' and ignore_input = '0' then
          data_reg <= din;
          valid_reg <= '1';
        else
          valid_reg <= '0';
        end if;
      end if;
    end if;
  end process;

  
  -----------------------------------------------------------------------------
  -- Pixel counter to track progress in frame
  
  PIXEL_COUNT_REG: process (clk, rst, wr_en, do_insert)
  begin
    if rising_edge(clk) then
      if rst = '1' then
        pixel_count <= (others => '0');
      else
        if (wr_en = '1' and ignore_input = '0')
          or do_insert ='1' then
          if pixel_count = C_SKIP_SIZE+C_PASS_SIZE+C_INSERT_SIZE-1 then
            pixel_count <= (others => '0');
          else
            pixel_count <= pixel_count + 1;
          end if;
        end if;
      end if;
    end if;
  end process;

  
  -----------------------------------------------------------------------------
  -- Border insert logic
  
  BORDER_INSERT_GEN : if C_BORDER_SIZE > 0 generate
    BORDER_PROC : process (col_count, state)
    begin
      if state = PASS_STATE and
        (col_count < C_BORDER_SIZE or
         col_count >= C_IMAGE_WIDTH-C_BORDER_SIZE) then
        border_insert <= '1';
      else
        border_insert <= '0';
      end if;
    end process;

    COL_COUNT_REG : process (clk, rst, wr_en, col_count_clr, col_count)
    begin
      if rising_edge(clk) then
        if rst = '1' then
          col_count <= (others => '0');
        else
          if col_count_clr = '1' then
            col_count <= (others => '0');
          elsif wr_en = '1' then
            if col_count = C_IMAGE_WIDTH-1 then
              col_count <= (others => '0');
            else
              col_count <= col_count + 1;
            end if;
          end if;
        end if;
      end if;
    end process;
  end generate;

  BORDER_INSERT_GEN_NOT : if C_BORDER_SIZE <= 0 generate
    border_insert <= '0';
  end generate BORDER_INSERT_GEN_NOT;

  
  -----------------------------------------------------------------------------
  -- Insert interleave counter
  
  GEN_INTERLEAVE_G0 : if C_INSERT_INTERLEAVE > 0 generate
    INTERLEAVE_COUNT_REG : process (clk, rst, interleave_count, state, do_insert)
    begin
      if state = INSERT_STATE and interleave_count = C_INSERT_INTERLEAVE then
        do_insert <= '1';
      else
        do_insert <= '0';
      end if;

      if rising_edge(clk) then
        if rst = '1' then
          insert_valid     <= '0';
          interleave_count <= to_unsigned(1, interleave_count'length);
        else
          insert_valid <= do_insert;

          if state = INSERT_STATE then
            if interleave_count = C_INSERT_INTERLEAVE then
              interleave_count <= (others => '0');
            else
              interleave_count <= interleave_count + 1;
            end if;
          end if;
        end if;
      end if;
      
    end process;
  end generate;
  
  GEN_INTERLEAVE_E0: if C_INSERT_INTERLEAVE = 0 generate
    INTERLEAVE_COUNT_REG: process (clk, rst, do_insert, ignore_input)
    begin
      -- I'm not really happy about using ignore_input for this, but it works.
      if ignore_input = '1' then
        do_insert <= '1';
      else
        do_insert <= '0';
      end if;

      if rising_edge(clk) then
        if rst = '1' then
          insert_valid <= '1';
        else
          insert_valid <= do_insert;
        end if;
      end if;
    end process;
  end generate;

  
  -----------------------------------------------------------------------------
  -- STATE MACHINE ------------------------------------------------------------
  
  -- State machine register
  FSM_REG_PROC: process (clk, rst, next_state)
  begin
    if rising_edge(clk) then
      if rst = '1' then
        state <= IDLE_STATE;
      else
        state <= next_state;        
      end if;
    end if;
  end process;

  -- State machine combinational logic
  FSM_COMB_PROC: process (clk, rst, state, wr_en, pixel_count)
  begin
    next_state <= state;
    col_count_clr <= '1';
    ignore_input <= '0';
    
    case state is
      when IDLE_STATE =>
        col_count_clr <= '1';
        if wr_en = '1' then
          if C_SKIP_SIZE > 0 then
            next_state <= SKIP_STATE;
          else
            next_state <= PASS_STATE;
          end if;
        end if;
      
      when SKIP_STATE =>
        col_count_clr <= '1';
        if wr_en = '1' and pixel_count >= C_SKIP_SIZE then
          next_state <= PASS_STATE;
        end if;
        
      when PASS_STATE =>
        col_count_clr <= '0';
        if pixel_count >= C_SKIP_SIZE+C_PASS_SIZE then
          if C_INSERT_SIZE > 0 then 
            ignore_input <= '1';
            next_state <= INSERT_STATE;
          else
            next_state <= IDLE_STATE;
          end if;
        end if;

      when INSERT_STATE =>
        ignore_input <= '1';
        if pixel_count = 0 then
          next_state <= IDLE_STATE;
        end if;

      when others => null;
    end case;

  end process;
 
end imp;


