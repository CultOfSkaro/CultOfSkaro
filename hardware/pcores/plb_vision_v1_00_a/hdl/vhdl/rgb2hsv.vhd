-------------------------------------------------------------------------------
-- 
-- FILE:    rgb2hsv.vhd
-- AUTHOR:  Barrett Edwards
-- CREATED: 12 April 2007
--
-- DESCRIPTION:
-- This module takes a RGB pixel input and converts it to HSV
-- Range: R : 0-255 8-bit unsgined
--        G : 0-255 8-bit unsgined
--        B : 0-255 8-bit unsgined
--        H : 0-360 9-bit unsgined
--        S : 0-255 8-bit unsgined
--        V : 0-255 8-bit unsgined
--
-- Equation: see wikipedia:"HSV color space"
-- V = max(r,g,b);
-- S = 255 - 255 * min(r,g,b) / max(r,g,b)
-- H =|
--    | if max(r,g,b) == b then h = 60 * (r-g)/(max(r,g,b) - min(r,g,b)) + 240
--    | if max(r,g,b) == g then h = 60 * (b-r)/(max(r,g,b) - min(r,g,b)) + 120
--    | if max(r,g,b) == b then
--    |   if G < B              h = 60 * (g-b)/(max(r,g,b) - min(r,g,b)) + 0
--    |   else                  h = 60 * (g-b)/(max(r,g,b) - min(r,g,b)) + 360
--    |
--
-- These equations are implemented in a 5 stage pipeline. The computations of
-- each stage are listed as follows:
--  Stage0: Stores the input RGB values into a register. This stage is included
--          to aid in helping the core make timing. This stage can be
--          execluded if desired.
--
--  Stage1: Determins the max and min of the R,G,B input values
--
--  Stage2: Computes the numerator of the hue equation (i.e. R-G).
--          Determines if G is less than B.
--          Computes an address for the saturation lookup table
--          Computes an address for the hue lookup table
--
--          The saturation lookup table is used to avoid the need to
--          implement a division in FPGA hardware. So, we compute the
--          division in C code before hand and then store it in a BRAM
--          lookup table. By doing this we can also get some extra
--          precision by using an 16 fixed point value as the output. The fixed
--          point output value is [U8.8] an unsigned 8-bit integer, 8-bit
--          fraction number. To gain this extra precision, the BRAM does not
--          store 255 / max(r,g,b) but stores (255 << 8) / max(r,g,b).
--
--          The hue lookup table is similar in construction to the saturation
--          lookup table. The hue lookup table implements the following
--          equation: (60 << 9) / (max(r,g,b) - min(r,g,b)). The max-min is
--          used as the address into the table. This address will always be
--          between 0-255. This results in a [S7.9] signed 7-bit integer, 9-bit
--          fractional number.
--
--  Stage3: Computes the multiply of (255/max(r,g,b)) * min(r,g,b), where
--          (255/max(r,g,b)) is computed using the lookup table.
--
--          Computes the multiply of (60 / (max(r,g,b)-min(r,g,b))) * (Numerator)
--          where (60 / (max(r,g,b)-min(r,g,b))) is computed using the lookup
--          table. Also the Numerator value can be one of the following: (R-G),
--          (B-R), (G-B) depending on which of the RGB values is the max.
--           
--  Stage4: Adds an offset to the Hue value based on which of the RGB values is
--          the max.
--          Thresholds the Saturation value to 0 if the max(R,G,B)= min(R,G,B).
--          This is to make sure the Saturation is 0 if all three of the R,G,B
--          values are the same (Grayscale). When this occures the hue is also
--          set to 0.
--          Computes Saturation value = 255 - (255*min/max)
--          
--  RESOURCES: for Virtex4 FX60 speed grade -11:
--   Slices: 132
--   FF    : 108
--   LUTs  : 194
--   BRAMs : 1
--   DSPs  : 2 
--   maxfq : 190.549 MHz (5.248ns period)
--
--  ACCURACY:
--   This core is not *perfectly* accurate so there is a little error noise. To
--   quantify this error in the calculated HSV values, all possible RGB values
--   were passed through this core. *All Possible* is defined as all the RGB
--   combinates that are possible using the RGB565 format. The true HSV values
--   of these RGB565 input values as calcualted in matlab using rgb2hsv() were
--   subtracted from the resulting HSV values that were calculated using this
--   core.
--                    difference = core_calculated_hsv - true_hsv
--   The resulting difference values had these characteristics:
--                     mean       std      min_error    max_error
--   Hue        :    -0.3790     0.3313     -1.3333      0.3396
--   Saturation :     0.4954     0.3415        0         1.5769
--   Value      :       0          0           0            0
--
-- 
-- 

  

-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library unisim;
use unisim.vcomponents.all;

entity rgb2hsv is   
  port (
    clk         : in  std_logic;
    reset       : in  std_logic;

    R           : in  std_logic_vector(7 downto 0) := X"59"; 
    G           : in  std_logic_vector(7 downto 0) := X"32";  
    B           : in  std_logic_vector(7 downto 0) := X"81";  
    en          : in  std_logic;

    H           : out std_logic_vector(8 downto 0);
    S           : out std_logic_vector(7 downto 0);
    V           : out std_logic_vector(7 downto 0);
    valid       : out std_logic);

end rgb2hsv;

architecture behavioral of rgb2hsv is
-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------
  component rgb2hsvlut
    port (
      addra : IN  std_logic_VECTOR(9 downto 0);
      addrb : IN  std_logic_VECTOR(9 downto 0);
      clka  : IN  std_logic;
      clkb  : IN  std_logic;
      douta : OUT std_logic_VECTOR(15 downto 0);
      doutb : OUT std_logic_VECTOR(15 downto 0));
  end component;

  component dspmult16_regout_aubu
    port (
      clk : IN  std_logic;
      a   : IN  std_logic_VECTOR(15 downto 0);
      b   : IN  std_logic_VECTOR(15 downto 0);
      q   : OUT std_logic_VECTOR(31 downto 0));
  end component;
  
  component dspmult16_regout_asbs
    port (
      clk : IN  std_logic;
      a   : IN  std_logic_VECTOR(15 downto 0);
      b   : IN  std_logic_VECTOR(15 downto 0);
      q   : OUT std_logic_VECTOR(31 downto 0));
  end component;
  
-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------  
  signal stage0                         : std_logic;
  signal stage0_R                       : unsigned(7 downto 0); 
  signal stage0_G                       : unsigned(7 downto 0); 
  signal stage0_B                       : unsigned(7 downto 0);

  signal stage1                         : std_logic;
  signal stage1_R                       : unsigned(7 downto 0); 
  signal stage1_G                       : unsigned(7 downto 0); 
  signal stage1_B                       : unsigned(7 downto 0);
  signal stage1_max                     : std_logic_vector(1 downto 0);
  signal stage1_min                     : std_logic_vector(1 downto 0);

  signal max_value                      : unsigned(7 downto 0);
  signal min_value                      : unsigned(7 downto 0);
  signal max_minus_min                  : unsigned(7 downto 0);

  signal sat_addr                       : std_logic_vector( 9 downto 0);
  signal hue_addr                       : std_logic_vector( 9 downto 0);

  signal sat_lookup                     : std_logic_vector(15 downto 0);
  signal hue_lookup                     : std_logic_vector(15 downto 0);

  signal hue_numeratorL                 : unsigned(7 downto 0);
  signal hue_numeratorR                 : unsigned(7 downto 0);
  
  signal stage2                         : std_logic;
  signal stage2_max                     : std_logic_vector(1 downto 0);
  signal stage2_min                     : std_logic_vector(1 downto 0);
  signal stage2_max_value               : unsigned(7 downto 0);
  signal stage2_min_value               : unsigned(7 downto 0);
  signal stage2_hue_numerator           : signed(8 downto 0);
  signal stage2_GltB                    : std_logic;

  signal satmultb                       : std_logic_vector(15 downto 0);
  signal huemultb                       : std_logic_vector(15 downto 0);
  
  signal satmultout                     : std_logic_vector(31 downto 0);
  signal huemultout                     : std_logic_vector(31 downto 0);
  
  signal stage3                         : std_logic;
  signal stage3_max                     : std_logic_vector(1 downto 0);
  signal stage3_min                     : std_logic_vector(1 downto 0);
  signal stage3_max_value               : unsigned(7 downto 0);
  signal stage3_GltB                    : std_logic;  


  signal hueoffset                      : signed(9 downto 0);
  signal stage4                         : std_logic;
  signal stage4_H                       : std_logic_vector(9 downto 0);
  signal stage4_S                       : std_logic_vector(7 downto 0);
  signal stage4_V                       : std_logic_vector(7 downto 0);

begin

-------------------------------------------------------------------------------
-- Stage 0
-- Purpose: latch in the incomming data
-------------------------------------------------------------------------------
  process(clk,reset,R,B,G)
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        stage0          <= '0';                
--        stage0_R        <= (others => '0');
--        stage0_G        <= (others => '0');
--        stage0_B        <= (others => '0');
      else
        stage0          <= en;        
        stage0_R        <= unsigned(R);
        stage0_G        <= unsigned(G);
        stage0_B        <= unsigned(B);
      end if;
    end if;
  end process;
  







  
-------------------------------------------------------------------------------
-- Stage 1
-- Purpose: 
--  1: Identify which of the 3 channels is the max
--  2: Identify which of the 3 channels is the min
-------------------------------------------------------------------------------
  
  process(clk,reset,stage0,stage0_R,stage0_G,stage0_B)
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        stage1          <= '0';                
--         stage1_R        <= (others => '0');
--         stage1_G        <= (others => '0');
--         stage1_B        <= (others => '0');
--         stage1_max      <= "00";
--         stage1_min      <= "00";
      else
        stage1          <= stage0;        
        stage1_R        <= stage0_R;
        stage1_G        <= stage0_G;
        stage1_B        <= stage0_B;

        if    (stage0_R >= stage0_B) and (stage0_R >= stage0_G) then stage1_max <= "00"; -- red
        elsif (stage0_G >= stage0_R) and (stage0_G >= stage0_B) then stage1_max <= "01"; -- green 
        else                                                         stage1_max <= "10"; -- blue  
        end if;

        if    (stage0_R <= stage0_B) and (stage0_R <= stage0_G) then stage1_min <= "00"; -- red 
        elsif (stage0_G <= stage0_R) and (stage0_G <= stage0_B) then stage1_min <= "01"; -- green
        else                                                         stage1_min <= "10"; -- blue
        end if;
      end if; 
    end if;
  end process;







  
  
-------------------------------------------------------------------------------
-- Stage 2:
--   
-------------------------------------------------------------------------------

  with stage1_max select
    max_value <=
    stage1_B when "10",   -- max == blue
    stage1_G when "01",   -- max == green
    stage1_R when others; -- max == red

  with stage1_min select
    min_value <=
    stage1_B when "10",   -- min == blue
    stage1_G when "01",   -- min == green
    stage1_R when others; -- min == red

  max_minus_min <=  max_value
                  - min_value;  

  sat_addr <= "00" & std_logic_vector(max_value);
  hue_addr <= "10" & std_logic_vector(max_minus_min);
  
  LookUpTable: rgb2hsvlut
    port map (
      addra => sat_addr,
      addrb => hue_addr,
      clka  => clk,
      clkb  => clk,
      douta => sat_lookup,  -- Saturation: (255<<8)/max   
      doutb => hue_lookup); -- Hue:        (60<<??)/(max - min)    
 

 
  with stage1_max select
       hue_numeratorL <=
       stage1_R when "10",   -- max == blue
       stage1_B when "01",   -- max == green
       stage1_G when others; -- max == red

  with stage1_max select
       hue_numeratorR <=
       stage1_G when "10",   -- max == blue
       stage1_R when "01",   -- max == green
       stage1_B when others; -- max == red
  
  
  process(clk,reset,stage1,stage1_R,stage1_G,stage1_B,stage1_max,stage1_min)
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        stage2                  <= '0';                       
--         stage2_max              <= "00";
--         stage2_min              <= "00";        
--         stage2_max_value        <= (others => '0');
--         stage2_min_value        <= (others => '0');
--         stage2_hue_numerator    <= (others => '0');
--         stage2_GltB             <= '0';
      else
        stage2                  <= stage1;        
        stage2_max              <= stage1_max;
        stage2_min              <= stage1_min;        
        stage2_max_value        <= max_value;
        stage2_min_value        <= min_value;
        stage2_hue_numerator    <=   signed(resize(hue_numeratorL,9))
                                   - signed(resize(hue_numeratorR,9));
        
        if stage1_G < stage1_B then stage2_GltB <= '1';
        else                        stage2_GltB <= '0';
        end if;
      end if;
    end if;
  end process; 
  









  
-------------------------------------------------------------------------------
-- Stage 3
-------------------------------------------------------------------------------  
  --douta * stage2_min_value
  --doutb * stage2_numerator;

  --This should have dual unsigned ports
  satmultb <= std_logic_vector(shift_left(resize(stage2_min_value,16),8));
  SatMult: dspmult16_regout_aubu
    port map (
        clk => clk,
        a   => sat_lookup,
        b   => satmultb,
        q   => satmultout);

  huemultb <= std_logic_vector(shift_left(resize(stage2_hue_numerator,16),7));
  HueMult: dspmult16_regout_asbs
    port map (
        clk => clk,
        a   => hue_lookup,
        b   => huemultb,
        q   => huemultout);
  
  
  process(clk,reset,stage2,stage2_min,stage2_max,stage2_min_value)
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        stage3                  <= '0';                       
--         stage3_max              <= "00";
--         stage3_min              <= "00";        
--         stage3_max_value        <= (others => '0');
--         stage3_GltB             <= '0';
      else
        stage3                  <= stage2;                       
        stage3_max              <= stage2_max;
        stage3_min              <= stage2_min;       
        stage3_max_value        <= stage2_max_value;
        stage3_GltB             <= stage2_GltB;
      end if;
    end if;
  end process; 


-------------------------------------------------------------------------------
-- Stage 4:
-------------------------------------------------------------------------------

  hueoffset <= to_signed(240,10) when stage3_max(1) = '1' else -- max == blue
               to_signed(120,10) when stage3_max(0) = '1' else -- max == green
               to_signed(360,10) when stage3_GltB   = '1' else -- max == red (G<B)
               to_signed(0,10);                                -- max == red (G>=B)   

               
  process(clk,reset,stage3,huemultout,hueoffset,stage3_max,stage3_min,satmultout,stage3_max_value)
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        stage4          <= '0';        
--         stage4_H        <= (others => '0');
--         stage4_S        <= (others => '0');
--         stage4_V        <= (others => '0');        
      else
        stage4          <= stage3;

        if stage3_max = stage3_min then
          stage4_H <= (others => '0');
          stage4_S <= (others => '0'); 
        else
          stage4_H <= std_logic_vector(signed(huemultout(25 downto 16)) + hueoffset);
          stage4_S <= std_logic_vector(to_unsigned(255,8) - unsigned(satmultout(23 downto 16)));
        end if;
        
        stage4_V   <= std_logic_vector(stage3_max_value);
      end if;
    end if;
  end process; 


-------------------------------------------------------------------------------
-- Output Asignment
-------------------------------------------------------------------------------
  H     <= stage4_H( 8 downto 0);
  S     <= stage4_S;
  V     <= stage4_V;
  valid <= stage4;
  
end behavioral;



