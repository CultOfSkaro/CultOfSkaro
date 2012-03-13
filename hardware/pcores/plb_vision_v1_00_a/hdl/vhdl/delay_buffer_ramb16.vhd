-------------------------------------------------------------------------------
-- Delay Buffer, Implemented Using RAMB16 Instances
--
-- Filename: delay_buffer_ramb16.vhd
-- Auther: Wade S. Fife
-- Date: 4/25/07
--
-- This delay buffer is intended to delay a data stream by the specified number
-- of cycles. This is useful, for example, in a 2D filter where pixel data is
-- sent in serially. The delay buffer allows us to buffer the first row so that
-- data leaves the buffer in sync with the second row, allowing vertical
-- processing to be performed.
--
-- Data is written by asserting din and wr_en for one clock cycle. As soon as
-- C_DEPTH more words have been buffered, the first word then appears on dout
-- and valid will go high for once clock cycle. Exactly C_DEPTH values can be
-- buffered simultaneously. See the timing diagram below for an example.
--
-- Note that this is not a true FIFO nor is it a pure delay. The actual number
-- of clock cycles of delay depends on wr_en. Basically, data can only be
-- output on dout on the cycle after when wr_en is asserted. This allows you to
-- keep the input stream (din) in sync with the output stream (dout). However,
-- if wr_en is always asserted then the delay will be exactly C_DEPTH+1 cycles,
-- and this entity is a pure delay. See the timing diagram below for an
-- example.
--
-- The C_WIDTH generic is the width of the din/dout ports.
--
-- A timing example is shown below where C_DEPTH is 2 and the buffer is
-- initially empty.
--
--    Cycle | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F |
--    ------|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
--      din | X | A | B | C | D | X | X | E | F | X | X | 2 | 3 | 4 | X | X |
--    wr_en | 0 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 1 | 0 | 0 |
--     dout | ? | ? | ? | ? | A | B | B | B | C | D | D | D | E | F | 2 | 2 |
--    valid | 0 | 0 | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 1 | 0 |
--
-- Note that dout holds its value until valid goes high again.
--
-- This module uses RAMB16 blocks (BRAMs) to generate the memory used for the
-- buffer. It attempts to choose an optimal number and size of BRAMs, but in
-- some situations it may not. The method for determining the number and width
-- of BRAMs is as follows. First, it determines which is the widest BRAM with a
-- depth sufficient to match C_DEPTH. It then combines as many of these BRAMs
-- as are needed to give a width of at least C_WIDTH. As a result, C_DEPTH
-- cannot exceed 16,384, the largest BRAM depth possible.
--
-- The reset of this entity is active-high and synchronous. Note that reset
-- does not clear the values in the block RAM. The valid signal must be used
-- to discriminate between latent and new values.
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

library unisim;
use unisim.vcomponents.all;


entity delay_buffer_ramb16 is
  generic (
    C_DEPTH : integer := 2;
    C_WIDTH : integer := 8);
  port (
    clk   : in  std_logic;
    rst   : in  std_logic;
    din   : in  std_logic_vector(C_WIDTH-1 downto 0);
    wr_en : in  std_logic;
    dout  : out std_logic_vector(C_WIDTH-1 downto 0);
    valid : out std_logic);
end delay_buffer_ramb16;


architecture imp of delay_buffer_ramb16 is

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

  -- Function to calculate the size of the BRAM to use
  function DetermineBramWidth(depth : integer)
  return integer is
    variable width : integer;
  begin
    assert depth <= 16384
      report "Depth cannot exceed 16,384!"
      severity failure;
                    
    if depth > 8192 then
      width := 1;
    elsif depth > 4096 then
      width := 2;
    elsif depth > 2048 then
      width := 4;
    elsif depth > 1024 then
      width := 9;
    elsif depth > 512 then
      width := 18;
    else
      width := 36;
    end if;
    return width;
  end function;

  -- Function to calculate how many BRAMs are needed
  function DetermineNumBrams(desired_width, bram_width : integer)
  return integer is
    variable num : integer;
  begin
    num := desired_width / bram_width;
    if desired_width mod bram_width /= 0 then
      num := num + 1;
    end if;
    return num;
  end function;

  -- Function to calculate address width for BRAM
  function DetermineRamAddrWidth(bram_width : integer)
  return integer is
    variable addr_width : integer;
  begin
    if bram_width = 1 then
      addr_width := 14;
    elsif bram_width = 2 then
      addr_width := 13;
    elsif bram_width = 4 then
      addr_width := 12;
    elsif bram_width = 9 then
      addr_width := 11;
    elsif bram_width = 18 then
      addr_width := 10;
    else
      addr_width := 9;
    end if;
    return addr_width;
  end function;

  -- The width BRAM to use
  constant BRAM_DATA_WIDTH : integer := DetermineBramWidth(C_DEPTH);

  -- The number of BRAMs required
  constant NUM_BRAMS : integer := DetermineNumBrams(C_WIDTH, BRAM_DATA_WIDTH);

  -- Address width of each BRAM or entire RAM
  constant RAM_ADDR_WIDTH : integer := DetermineRamAddrWidth(BRAM_DATA_WIDTH);

  -- Data width of entire RAM
  constant RAM_DATA_WIDTH : integer := BRAM_DATA_WIDTH * NUM_BRAMS;
  
  signal ram_in_addr  : std_logic_vector(RAM_ADDR_WIDTH-1 downto 0);
  signal ram_out_addr : std_logic_vector(RAM_ADDR_WIDTH-1 downto 0);
  signal ram_in_addr_reg  : unsigned(RAM_ADDR_WIDTH-1 downto 0);
  signal ram_out_addr_reg : unsigned(RAM_ADDR_WIDTH-1 downto 0);

  signal ram_data_in : std_logic_vector(RAM_DATA_WIDTH-1 downto 0);
  signal ram_data_out : std_logic_vector(RAM_DATA_WIDTH-1 downto 0);

  signal valid_count_reg : std_logic;
  signal valid_counter :
    unsigned(RequiredBits(C_DEPTH+1, RAM_ADDR_WIDTH)-1 downto 0);
  
  signal valid_reg : std_logic;

begin

  -- Convert unsigned registers to std_logic_vector signals
  ram_in_addr <= std_logic_vector(ram_in_addr_reg);
  ram_out_addr <= std_logic_vector(ram_out_addr_reg);

  -- Resize signals going to/from BRAM
  dout <= ram_data_out(C_WIDTH-1 downto 0);
  GEN_PADDING: if RAM_DATA_WIDTH /= C_WIDTH generate
    ram_data_in <= (RAM_DATA_WIDTH-1 downto C_WIDTH => '0') & din;
  end generate;
  GEN_NO_PADDING: if RAM_DATA_WIDTH = C_WIDTH generate
    ram_data_in <= din;
  end generate;

  valid <= valid_reg and valid_count_reg;

  
  ADDR_REG_PROC: process (clk, rst, ram_in_addr_reg, ram_out_addr_reg)
  begin
    if clk'event and clk = '1' then
      if rst = '1' then
        ram_in_addr_reg <= to_unsigned(C_DEPTH, RAM_ADDR_WIDTH);
        ram_out_addr_reg <= (others => '0');
      elsif wr_en = '1' then
        ram_in_addr_reg <= ram_in_addr_reg + 1;
        ram_out_addr_reg <= ram_out_addr_reg + 1;
      end if;
    end if;
  end process;


  VALID_COUNT_PROC: process (clk, rst, wr_en, valid_counter)
  begin
    if clk'event and clk = '1' then
      if rst = '1' then
        valid_counter <= (others => '0');
        valid_count_reg <= '0';
      elsif wr_en = '1' then
        if valid_counter /= C_DEPTH then
          valid_counter <= valid_counter + 1;
        else
          valid_count_reg <= '1';
        end if;
      end if;
    end if;
  end process;
  

  VALID_REG_PROC: process (clk, rst, wr_en)
  begin
    if clk'event and clk = '1' then
      if rst = '1' then
        valid_reg <= '0';
      else
        valid_reg <= wr_en;
      end if;
    end if;
  end process;
  
  GEN_BRAMS_36: if BRAM_DATA_WIDTH = 36 generate
    GENERATE_RAM: for i in 0 to NUM_BRAMS-1 generate
      BRAM_BUFFER_INST: RAMB16_S36_S36
        generic map (
          WRITE_MODE_A => "READ_FIRST",
          WRITE_MODE_B => "READ_FIRST")
        port map (
          CLKA  => clk,
          ENA   => '1',
          WEA   => wr_en,
          SSRA  => rst,
          ADDRA => ram_in_addr,
          DIA   => ram_data_in(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8 - 1
                               downto i*BRAM_DATA_WIDTH),
          DIPA  => ram_data_in(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - 1
                               downto i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8),
          DOA   => open,
          DOPA  => open,
          CLKB  => clk,
          ENB   => wr_en,
          WEB   => '0',
          SSRB  => rst,
          ADDRB => ram_out_addr,
          DIB   => (others => '0'),
          DIPB  => (others => '0'),
          DOB   => ram_data_out(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8 - 1
                                downto i*BRAM_DATA_WIDTH),
          DOPB  => ram_data_out(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH-1
                                downto i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8));
    end generate GENERATE_RAM;
  end generate;

  GEN_BRAMS_18: if BRAM_DATA_WIDTH = 18 generate
    GENERATE_RAM: for i in 0 to NUM_BRAMS-1 generate
      BRAM_BUFFER_INST: RAMB16_S18_S18
        generic map (
          WRITE_MODE_A => "READ_FIRST",
          WRITE_MODE_B => "READ_FIRST")
        port map (
          CLKA  => clk,
          ENA   => '1',
          WEA   => wr_en,
          SSRA  => rst,
          ADDRA => ram_in_addr,
          DIA   => ram_data_in(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8 - 1
                               downto i*BRAM_DATA_WIDTH),
          DIPA  => ram_data_in(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - 1
                               downto i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8),
          DOA   => open,
          DOPA  => open,
          CLKB  => clk,
          ENB   => wr_en,
          WEB   => '0',
          SSRB  => rst,
          ADDRB => ram_out_addr,
          DIB   => (others => '0'),
          DIPB  => (others => '0'),
          DOB   => ram_data_out(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8 - 1
                                downto i*BRAM_DATA_WIDTH),
          DOPB  => ram_data_out(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH-1
                                downto i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8));
    end generate GENERATE_RAM;
  end generate;

  GEN_BRAMS_9: if BRAM_DATA_WIDTH = 9 generate
    GENERATE_RAM: for i in 0 to NUM_BRAMS-1 generate
      BRAM_BUFFER_INST: RAMB16_S9_S9
        generic map (
          WRITE_MODE_A => "READ_FIRST",
          WRITE_MODE_B => "READ_FIRST")
        port map (
          CLKA  => clk,
          ENA   => '1',
          WEA   => wr_en,
          SSRA  => rst,
          ADDRA => ram_in_addr,
          DIA   => ram_data_in(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8 - 1
                               downto i*BRAM_DATA_WIDTH),
          DIPA  => ram_data_in(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - 1
                               downto i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8),
          DOA   => open,
          DOPA  => open,
          CLKB  => clk,
          ENB   => wr_en,
          WEB   => '0',
          SSRB  => rst,
          ADDRB => ram_out_addr,
          DIB   => (others => '0'),
          DIPB  => (others => '0'),
          DOB   => ram_data_out(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8 - 1
                                downto i*BRAM_DATA_WIDTH),
          DOPB  => ram_data_out(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH-1
                                downto i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - BRAM_DATA_WIDTH/8));
    end generate GENERATE_RAM;
  end generate;

  GEN_BRAMS_4: if BRAM_DATA_WIDTH = 4 generate
    GENERATE_RAM: for i in 0 to NUM_BRAMS-1 generate
      BRAM_BUFFER_INST: RAMB16_S4_S4
        generic map (
          WRITE_MODE_A => "READ_FIRST",
          WRITE_MODE_B => "READ_FIRST")
        port map (
          CLKA  => clk,
          ENA   => '1',
          WEA   => wr_en,
          SSRA  => rst,
          ADDRA => ram_in_addr,
          DIA   => ram_data_in(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - 1
                               downto i*BRAM_DATA_WIDTH),
          DOA   => open,
          CLKB  => clk,
          ENB   => wr_en,
          WEB   => '0',
          SSRB  => rst,
          ADDRB => ram_out_addr,
          DIB   => (others => '0'),
          DOB   => ram_data_out(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - 1
                                downto i*BRAM_DATA_WIDTH));
    end generate GENERATE_RAM;
  end generate;

  GEN_BRAMS_2: if BRAM_DATA_WIDTH = 2 generate
    GENERATE_RAM: for i in 0 to NUM_BRAMS-1 generate
      BRAM_BUFFER_INST: RAMB16_S2_S2
        generic map (
          WRITE_MODE_A => "READ_FIRST",
          WRITE_MODE_B => "READ_FIRST")
        port map (
          CLKA  => clk,
          ENA   => '1',
          WEA   => wr_en,
          SSRA  => rst,
          ADDRA => ram_in_addr,
          DIA   => ram_data_in(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - 1
                               downto i*BRAM_DATA_WIDTH),
          DOA   => open,
          CLKB  => clk,
          ENB   => wr_en,
          WEB   => '0',
          SSRB  => rst,
          ADDRB => ram_out_addr,
          DIB   => (others => '0'),
          DOB   => ram_data_out(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - 1
                                downto i*BRAM_DATA_WIDTH));
    end generate GENERATE_RAM;
  end generate;

  GEN_BRAMS_1: if BRAM_DATA_WIDTH = 1 generate
    GENERATE_RAM: for i in 0 to NUM_BRAMS-1 generate
      BRAM_BUFFER_INST: RAMB16_S1_S1
        generic map (
          WRITE_MODE_A => "READ_FIRST",
          WRITE_MODE_B => "READ_FIRST")
        port map (
          CLKA  => clk,
          ENA   => '1',
          WEA   => wr_en,
          SSRA  => rst,
          ADDRA => ram_in_addr,
          DIA   => ram_data_in(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - 1
                               downto i*BRAM_DATA_WIDTH),
          DOA   => open,
          CLKB  => clk,
          ENB   => wr_en,
          WEB   => '0',
          SSRB  => rst,
          ADDRB => ram_out_addr,
          DIB   => (others => '0'),
          DOB   => ram_data_out(i*BRAM_DATA_WIDTH + BRAM_DATA_WIDTH - 1
                                downto i*BRAM_DATA_WIDTH));
    end generate GENERATE_RAM;
  end generate;

         
end imp;
