-------------------------------------------------------------------------------
-- Buffer Data of One Size for Output as Another
--
-- FILE: size_bufer.vhd
-- AUTHOR: Wade Fife
-- DATE: March 1, 2005
-- MODIFIED: March 28, 2005
--
-- DESCRIPTION
--
-- This module reads in data of one size to output them in larger chunks.
--
-- For example, buffer bytes and output them as words (4-byte chunks) you would
-- set C_IN_WIDTH to 8 and C_OUT_WIDTH to 32.
--
-- This is used to take a byte or halfword based stream of data and convert it
-- to a word based stream for word-size DMA writes.
--
-------------------------------------------------------------------------------


library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


entity size_buffer is
  generic (
    C_IN_WIDTH  : integer := 8;
    C_OUT_WIDTH : integer := 32);
  port (
    clk          : in  std_logic;
    reset        : in  std_logic;
    in_valid     : in  std_logic;
    data_in      : in  std_logic_vector(C_IN_WIDTH-1 downto 0);
    out_valid    : out std_logic;
    data_out     : out std_logic_vector(C_OUT_WIDTH-1 downto 0));
end size_buffer;


architecture imp of size_buffer is
  constant MAX_COUNT : integer := (C_OUT_WIDTH/C_IN_WIDTH) - 1;
  subtype chunk_count_type is integer range 0 to MAX_COUNT;

  signal chunk_count : chunk_count_type;

  signal data_reg : std_logic_vector(C_OUT_WIDTH-1 downto 0);
  
begin

  data_out <= data_reg;
  
  process (clk, reset, in_valid, data_reg, data_in, chunk_count)
  begin
    if reset = '1' then
      out_valid <= '0';
    elsif clk'event and clk = '1' then
      out_valid <= '0';
      if in_valid = '1' then
        data_reg <= data_reg(C_OUT_WIDTH-C_IN_WIDTH-1 downto 0) & data_in;
        if chunk_count = MAX_COUNT then
          chunk_count <= 0;
          out_valid <= '1';
        else
          chunk_count <= chunk_count + 1;
        end if;
      end if;
    end if;
  end process;

end imp;


