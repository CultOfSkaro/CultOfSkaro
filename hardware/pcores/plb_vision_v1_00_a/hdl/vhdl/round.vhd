-------------------------------------------------------------------------------
-- Round a Number to a Smaller Width
--
-- FILE: round.vhd
-- AUTHOR: Wade Fife
-- DATE: August 23, 2006
-- MODIFIED: August 23, 2006
--
--
-- DESCRIPTION
--
-- Resizes the unsigned input to the size of the output and rounds the
-- result. If the input is exactly half way between rounded up and rounded
-- down, the result is rounded up.
--
-- For example, assume C_IN_WIDTH = 5 and C_OUT_WIDTH = 3. The following are
-- rounding examples:
--
--    INPUT  |  OUTPUT
--   ------------------
--    01000  |  010
--    01001  |  010
--    01010  |  011
--    01011  |  011
--
-- The C_USE_REG determines whether or not an input register should be used. If
-- C_USE_REG is FALSE then this circuit is completely combinational. Otherwise
-- the input is registered before rounding.
--
-- NOTES
--
-- * If C_IN_WIDTH = C_OUT_WIDTH, no rounding is performed and all logic and
--   registers are bypassed.
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity round is
  generic (
    C_IN_WIDTH  : integer := 10;
    C_OUT_WIDTH : integer := 8;
    C_USE_REG   : boolean := TRUE);
  port (
    clk       : in  std_logic;
    rst       : in  std_logic;

    wr_en     : in  std_logic;
    din       : in  std_logic_vector(C_IN_WIDTH-1 downto 0);

    valid     : out std_logic;
    dout      : out std_logic_vector(C_OUT_WIDTH-1 downto 0));
end round;


architecture imp of round is

  -- This signal is the input, truncated from the right to 1 bit wider than the
  -- output size. The extra bit is the rounding bit.
  signal source : unsigned(C_OUT_WIDTH downto 0);

begin

  assert C_IN_WIDTH >= C_OUT_WIDTH
    report "C_IN_WIDTH must be >= C_OUT_WIDTH"
    severity failure;

  -- If the input is the same as the output, simply pass through
  BYPASS_GEN: if C_IN_WIDTH = C_OUT_WIDTH generate
    valid <= wr_en;
    dout <= din;
  end generate BYPASS_GEN;

  BYPASS_GEN_NOT : if C_IN_WIDTH /= C_OUT_WIDTH generate
    REG_GEN : if C_USE_REG generate
      process (clk, rst, wr_en, din)
      begin
        if rising_edge(clk) then
          if rst = '1' then
            valid <= '0';
            source  <= (others => '0');
          else
            if wr_en = '1' then
              source <= unsigned(din(C_IN_WIDTH-1
                                     downto C_IN_WIDTH-C_OUT_WIDTH-1));
              valid <= '1';
            else
              valid <= '0';
            end if;
          end if;
        end if;
      end process;
    end generate;

    REG_GEN_NOT : if not C_USE_REG generate
      source <= unsigned(din(C_IN_WIDTH-1 downto C_IN_WIDTH-C_OUT_WIDTH-1));
      valid <= wr_en;
    end generate;

    -- Do actual rounding logic. Simply add the bit just to the right of the
    -- least significant position of dout. Make sure it's not going to
    -- overflow.
    process (source)
    begin
      if source(C_OUT_WIDTH downto 1) =
         unsigned(to_signed(-1, C_OUT_WIDTH)) then
        dout <= (others => '1');
      else
        dout <= std_logic_vector(source(C_OUT_WIDTH downto 1) +
                       resize(source(0 downto 0), C_OUT_WIDTH));
      end if;
    end process;
        
  end generate BYPASS_GEN_NOT;


end imp;


