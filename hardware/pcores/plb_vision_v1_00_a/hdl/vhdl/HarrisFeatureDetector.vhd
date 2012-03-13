
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
--use STD.TEXTIO.all;


------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------

entity HarrisFeatureDetector is
    generic (
      C_ROW_WIDTH  : integer := 640;
      C_DATA_WIDTH : integer := 8);
    port
      (
        clk         : in  std_logic;
        reset       : in  std_logic;

        pixel       : in  std_logic_vector(7 downto 0);
        pixel_valid : in  std_logic;
        
        result      : out std_logic_vector(15 downto 0);
        result_valid: out std_logic    
        );
end entity HarrisFeatureDetector;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of HarrisFeatureDetector is
attribute keep_hierarchy : string;
attribute keep_hierarchy of IMP: architecture is "true";
  
  
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

  component dspmult8
    port (
      clk : IN  std_logic;
      a   : IN  std_logic_VECTOR(7 downto 0);
      b   : IN  std_logic_VECTOR(7 downto 0);
      q   : OUT std_logic_VECTOR(15 downto 0));
  end component;

  signal p12                    : std_logic_vector(7 downto 0);
  signal p21                    : std_logic_vector(7 downto 0);
  signal p23                    : std_logic_vector(7 downto 0);
  signal p32                    : std_logic_vector(7 downto 0);
  signal pixel_buffer_valid     : std_logic;
  
  signal dx                     : std_logic_vector(8 downto 0);
  signal dy                     : std_logic_vector(8 downto 0);
  signal dx_dy                  : std_logic_vector(15 downto 0);
  signal grad_valid             : std_logic;

  signal d11                   : std_logic_vector(15 downto 0);
  signal d12                   : std_logic_vector(15 downto 0);
  signal d13                   : std_logic_vector(15 downto 0);
  signal d21                   : std_logic_vector(15 downto 0);
  signal d22                   : std_logic_vector(15 downto 0);
  signal d23                   : std_logic_vector(15 downto 0);
  signal d31                   : std_logic_vector(15 downto 0);
  signal d32                   : std_logic_vector(15 downto 0);
  signal d33                   : std_logic_vector(15 downto 0);
  signal grid_valid            : std_logic;
  
  signal dx11                   : std_logic_vector(7 downto 0);
  signal dx12                   : std_logic_vector(7 downto 0);
  signal dx13                   : std_logic_vector(7 downto 0);
  signal dx21                   : std_logic_vector(7 downto 0);
  signal dx22                   : std_logic_vector(7 downto 0);
  signal dx23                   : std_logic_vector(7 downto 0);
  signal dx31                   : std_logic_vector(7 downto 0);
  signal dx32                   : std_logic_vector(7 downto 0);
  signal dx33                   : std_logic_vector(7 downto 0);
  
  signal dy11                   : std_logic_vector(7 downto 0);
  signal dy12                   : std_logic_vector(7 downto 0);
  signal dy13                   : std_logic_vector(7 downto 0);
  signal dy21                   : std_logic_vector(7 downto 0);
  signal dy22                   : std_logic_vector(7 downto 0);
  signal dy23                   : std_logic_vector(7 downto 0);
  signal dy31                   : std_logic_vector(7 downto 0);
  signal dy32                   : std_logic_vector(7 downto 0);
  signal dy33                   : std_logic_vector(7 downto 0);
  
  signal sqr_dx11             : std_logic_vector(15 downto 0);
  signal sqr_dx12             : std_logic_vector(15 downto 0);
  signal sqr_dx13             : std_logic_vector(15 downto 0);
  signal sqr_dx21             : std_logic_vector(15 downto 0);
  signal sqr_dx22             : std_logic_vector(15 downto 0);
  signal sqr_dx23             : std_logic_vector(15 downto 0);
  signal sqr_dx31             : std_logic_vector(15 downto 0);
  signal sqr_dx32             : std_logic_vector(15 downto 0);
  signal sqr_dx33             : std_logic_vector(15 downto 0);

  signal sqr_dy11             : std_logic_vector(15 downto 0);
  signal sqr_dy12             : std_logic_vector(15 downto 0);
  signal sqr_dy13             : std_logic_vector(15 downto 0);
  signal sqr_dy21             : std_logic_vector(15 downto 0);
  signal sqr_dy22             : std_logic_vector(15 downto 0);
  signal sqr_dy23             : std_logic_vector(15 downto 0);
  signal sqr_dy31             : std_logic_vector(15 downto 0);
  signal sqr_dy32             : std_logic_vector(15 downto 0);
  signal sqr_dy33             : std_logic_vector(15 downto 0);

  signal dxdy11               : std_logic_vector(15 downto 0);
  signal dxdy12               : std_logic_vector(15 downto 0);
  signal dxdy13               : std_logic_vector(15 downto 0);
  signal dxdy21               : std_logic_vector(15 downto 0);
  signal dxdy22               : std_logic_vector(15 downto 0);
  signal dxdy23               : std_logic_vector(15 downto 0);
  signal dxdy31               : std_logic_vector(15 downto 0);
  signal dxdy32               : std_logic_vector(15 downto 0);
  signal dxdy33               : std_logic_vector(15 downto 0);
  signal squares_valid        : std_logic;


  -- Added Stages to pipeline
  signal SumEx_1              : std_logic_vector(12 downto 0);   
  signal SumEx_2              : std_logic_vector(12 downto 0);   
  --signal SumEx_3              : std_logic_vector(12 downto 0);   
  signal SumEy_1              : std_logic_vector(12 downto 0);   
  signal SumEy_2              : std_logic_vector(12 downto 0);   
  --signal SumEy_3              : std_logic_vector(12 downto 0);   
  signal SumExEy_1            : std_logic_vector(12 downto 0);   
  signal SumExEy_2            : std_logic_vector(12 downto 0);   
  --signal SumExEy_3            : std_logic_vector(12 downto 0);   

  signal reg_SumEx_1          : std_logic_vector(12 downto 0);   
  signal reg_SumEx_2          : std_logic_vector(12 downto 0);   
  --signal reg_SumEx_3          : std_logic_vector(12 downto 0);   
  signal reg_SumEy_1          : std_logic_vector(12 downto 0);   
  signal reg_SumEy_2          : std_logic_vector(12 downto 0);   
  --signal reg_SumEy_3          : std_logic_vector(12 downto 0);   
  signal reg_SumExEy_1        : std_logic_vector(12 downto 0);   
  signal reg_SumExEy_2        : std_logic_vector(12 downto 0);   
  --signal reg_SumExEy_3        : std_logic_vector(12 downto 0);   
  signal sum1_valid           : std_logic;

  -- Reduced Sized Computation >>7
  signal SumEx                : std_logic_vector(12 downto 0);  -- original size:22 
  signal SumEy                : std_logic_vector(12 downto 0);  -- original size:22 
  signal SumExEy              : std_logic_vector(12 downto 0);  -- original size:22 

  signal reg_SumEx            : std_logic_vector(12 downto 0); -- original size:22 
  signal reg_SumEy            : std_logic_vector(12 downto 0); -- original size:22 
  signal reg_SumExEy          : std_logic_vector(12 downto 0); -- original size:22 
  signal sums_valid           : std_logic;


  signal AD                   : std_logic_vector(25 downto 0);  -- original size:44 
  signal BC                   : std_logic_vector(25 downto 0);  -- original size:44
  signal AplsD                : std_logic_vector(13 downto 0);  -- original size:23

  signal AD_reg               : std_logic_vector(25 downto 0);  -- original size:44 
  signal BC_reg               : std_logic_vector(25 downto 0);  -- original size:44 
  signal AplsD_reg            : std_logic_vector(13 downto 0);  -- original size:44 
  signal cross_valid          : std_logic;
  
  signal det                  : std_logic_vector(25 downto 0);  -- original size:44
  signal trace                : std_logic_vector(27 downto 0);  -- original size:46

  signal reg_det              : std_logic_vector(25 downto 0);  -- original size:44
  signal reg_trace            : std_logic_vector(23 downto 0);  -- original size:42
  signal det_valid            : std_logic;


  signal value                : std_logic_vector(25 downto 0);  -- bits
  signal reg_result           : std_logic_vector(25 downto 0);  -- bits Sized Computation >>7
    
    
 begin

-------------------------------------------------------------------------------
-- Stage 1: Buffer pixels in 3x3 buffer core
-------------------------------------------------------------------------------
  PIXEL_BUFFER: filter_buffer_3x3
    generic map (
        C_ROW_WIDTH  => C_ROW_WIDTH,
        C_DATA_WIDTH => 8)
    port map (
        clk   => clk,
        rst   => reset,
        din   => pixel,
        wr_en => pixel_valid,
        e11   => open,
        e12   => p12,
        e13   => open,
        e21   => p21,
        e22   => open,
        e23   => p23,
        e31   => open,
        e32   => p32,
        e33   => open,
        valid => pixel_buffer_valid);

  
-------------------------------------------------------------------------------
-- Stage 2: Buffer gradients into 3x3 buffer core
-------------------------------------------------------------------------------

  -- Stage 2: Computation
  dx <= ieee.std_logic_arith.signed('0'&p23) - ieee.std_logic_arith.signed('0'&p21);
  dy <= ieee.std_logic_arith.signed('0'&p12) - ieee.std_logic_arith.signed('0'&p32);       
  dx_dy <= dx(8 downto 1) & dy(8 downto 1);

  -- Stage 2: Registers  
  DX_BUFFER: filter_buffer_3x3
    generic map (
        C_ROW_WIDTH  => C_ROW_WIDTH,
        C_DATA_WIDTH => 16)
    port map (
        clk   => clk,
        rst   => reset,
        din   => dx_dy,
        wr_en => pixel_buffer_valid,
        e11   => d11,
        e12   => d12,
        e13   => d13,
        e21   => d21,
        e22   => d22,
        e23   => d23,
        e31   => d31,
        e32   => d32,
        e33   => d33,
        valid => grid_valid);

  
-------------------------------------------------------------------------------
-- Stage 3: Compute Squares of gradients 
-------------------------------------------------------------------------------

  --Stage 3: Computation
  dx11 <= d11(15 downto 8);
  dx12 <= d12(15 downto 8);
  dx13 <= d13(15 downto 8);
  dx21 <= d21(15 downto 8);
  dx22 <= d22(15 downto 8);
  dx23 <= d23(15 downto 8);
  dx31 <= d31(15 downto 8);
  dx32 <= d32(15 downto 8);
  dx33 <= d33(15 downto 8);
  
  dy11 <= d11(7 downto 0);
  dy12 <= d12(7 downto 0);
  dy13 <= d13(7 downto 0);
  dy21 <= d21(7 downto 0);
  dy22 <= d22(7 downto 0);
  dy23 <= d23(7 downto 0);
  dy31 <= d31(7 downto 0);
  dy32 <= d32(7 downto 0);
  dy33 <= d33(7 downto 0);

  -- The DSP multiply slices register their inputs so we don't need to
  -- store the resulting value in a slice register
  dsp_dx11: dspmult8 port map (clk => clk, a => dx11, b => dx11, q => sqr_dx11);
  dsp_dx12: dspmult8 port map (clk => clk, a => dx12, b => dx12, q => sqr_dx12);
  dsp_dx13: dspmult8 port map (clk => clk, a => dx13, b => dx13, q => sqr_dx13);
  dsp_dx21: dspmult8 port map (clk => clk, a => dx21, b => dx21, q => sqr_dx21);
  dsp_dx22: dspmult8 port map (clk => clk, a => dx22, b => dx22, q => sqr_dx22);
  dsp_dx23: dspmult8 port map (clk => clk, a => dx23, b => dx23, q => sqr_dx23);
  dsp_dx31: dspmult8 port map (clk => clk, a => dx31, b => dx31, q => sqr_dx31);
  dsp_dx32: dspmult8 port map (clk => clk, a => dx32, b => dx32, q => sqr_dx32);
  dsp_dx33: dspmult8 port map (clk => clk, a => dx33, b => dx33, q => sqr_dx33);
  
  dsp_dy11: dspmult8 port map (clk => clk, a => dy11, b => dy11, q => sqr_dy11);
  dsp_dy12: dspmult8 port map (clk => clk, a => dy12, b => dy12, q => sqr_dy12);
  dsp_dy13: dspmult8 port map (clk => clk, a => dy13, b => dy13, q => sqr_dy13);
  dsp_dy21: dspmult8 port map (clk => clk, a => dy21, b => dy21, q => sqr_dy21);
  dsp_dy22: dspmult8 port map (clk => clk, a => dy22, b => dy22, q => sqr_dy22);
  dsp_dy23: dspmult8 port map (clk => clk, a => dy23, b => dy23, q => sqr_dy23);
  dsp_dy31: dspmult8 port map (clk => clk, a => dy31, b => dy31, q => sqr_dy31);
  dsp_dy32: dspmult8 port map (clk => clk, a => dy32, b => dy32, q => sqr_dy32);
  dsp_dy33: dspmult8 port map (clk => clk, a => dy33, b => dy33, q => sqr_dy33);

  dsp_dxdy11: dspmult8 port map (clk => clk, a => dx11, b => dy11, q => dxdy11);
  dsp_dxdy12: dspmult8 port map (clk => clk, a => dx12, b => dy12, q => dxdy12);
  dsp_dxdy13: dspmult8 port map (clk => clk, a => dx13, b => dy13, q => dxdy13);
  dsp_dxdy21: dspmult8 port map (clk => clk, a => dx21, b => dy21, q => dxdy21);
  dsp_dxdy22: dspmult8 port map (clk => clk, a => dx22, b => dy22, q => dxdy22);
  dsp_dxdy23: dspmult8 port map (clk => clk, a => dx23, b => dy23, q => dxdy23);
  dsp_dxdy31: dspmult8 port map (clk => clk, a => dx31, b => dy31, q => dxdy31);
  dsp_dxdy32: dspmult8 port map (clk => clk, a => dx32, b => dy32, q => dxdy32);
  dsp_dxdy33: dspmult8 port map (clk => clk, a => dx33, b => dy33, q => dxdy33);


  -- Stage 3: Register valid signal
  process(clk,reset,squares_valid)   
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        squares_valid <= '0';
      else
        squares_valid <= grid_valid;
      end if;
 
    end if;
  end process;
  
-------------------------------------------------------------------------------
-- Stage 4: Sum squares 
-------------------------------------------------------------------------------

  -- Stage 4: computation
  SumEx_1<=   conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dx11(15 downto sqr_dx11'length-(SumEx'length-4))),SumEx'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dx12(15 downto sqr_dx12'length-(SumEx'length-4))),SumEx'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dx13(15 downto sqr_dx13'length-(SumEx'length-4))),SumEx'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dx21(15 downto sqr_dx21'length-(SumEx'length-4))),SumEx'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dx22(15 downto sqr_dx22'length-(SumEx'length-4))),SumEx'length);
  
  SumEx_2<=   conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dx23(15 downto sqr_dx23'length-(SumEx'length-4))),SumEx'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dx31(15 downto sqr_dx31'length-(SumEx'length-4))),SumEx'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dx32(15 downto sqr_dx32'length-(SumEx'length-4))),SumEx'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dx33(15 downto sqr_dx33'length-(SumEx'length-4))),SumEx'length);
  
  SumEy_1<=   conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dy11(15 downto sqr_dy11'length-(SumEy'length-4))),SumEy'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dy12(15 downto sqr_dy12'length-(SumEy'length-4))),SumEy'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dy13(15 downto sqr_dy13'length-(SumEy'length-4))),SumEy'length) + 
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dy21(15 downto sqr_dy21'length-(SumEy'length-4))),SumEy'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dy22(15 downto sqr_dy22'length-(SumEy'length-4))),SumEy'length);
  
  SumEy_2<=   conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dy23(15 downto sqr_dy23'length-(SumEy'length-4))),SumEy'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dy31(15 downto sqr_dy31'length-(SumEy'length-4))),SumEy'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dy32(15 downto sqr_dy32'length-(SumEy'length-4))),SumEy'length) +
              conv_unsigned(ieee.std_logic_arith.unsigned(sqr_dy33(15 downto sqr_dy33'length-(SumEy'length-4))),SumEy'length);

  SumExEy_1<= conv_signed(ieee.std_logic_arith.signed(dxdy11(15 downto dxdy11'length-(SumExEy'length-4))),SumExEy'length) +
              conv_signed(ieee.std_logic_arith.signed(dxdy12(15 downto dxdy12'length-(SumExEy'length-4))),SumExEy'length) +
              conv_signed(ieee.std_logic_arith.signed(dxdy13(15 downto dxdy13'length-(SumExEy'length-4))),SumExEy'length) +
              conv_signed(ieee.std_logic_arith.signed(dxdy21(15 downto dxdy21'length-(SumExEy'length-4))),SumExEy'length) +
              conv_signed(ieee.std_logic_arith.signed(dxdy22(15 downto dxdy22'length-(SumExEy'length-4))),SumExEy'length);
  
  SumExEy_2<= conv_signed(ieee.std_logic_arith.signed(dxdy23(15 downto dxdy23'length-(SumExEy'length-4))),SumExEy'length) +  
              conv_signed(ieee.std_logic_arith.signed(dxdy31(15 downto dxdy31'length-(SumExEy'length-4))),SumExEy'length) +
              conv_signed(ieee.std_logic_arith.signed(dxdy32(15 downto dxdy32'length-(SumExEy'length-4))),SumExEy'length) +
              conv_signed(ieee.std_logic_arith.signed(dxdy33(15 downto dxdy33'length-(SumExEy'length-4))),SumExEy'length);

  
  -- Stage 4: Registers 
  process(clk,reset,squares_valid,SumEx_1,SumEx_2,SumEy_1,SumEy_2,SumExEy_1,SumExEy_2)   
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        sum1_valid          <= '0';
        reg_SumEx_1         <= (others => '0');
        reg_SumEx_2         <= (others => '0');
        reg_SumEy_1         <= (others => '0');
        reg_SumEy_2         <= (others => '0');        
        reg_SumExEy_1       <= (others => '0');
        reg_SumExEy_2       <= (others => '0');
      else
        sum1_valid          <= squares_valid;        
        reg_SumEx_1         <= SumEx_1;
        reg_SumEx_2         <= SumEx_2;
        reg_SumEy_1         <= SumEy_1;
        reg_SumEy_2         <= SumEy_2;
        reg_SumExEy_1       <= SumExEy_1;
        reg_SumExEy_2       <= SumExEy_2;
      end if;
    end if;
  end process;

-------------------------------------------------------------------------------
-- Stage 5: Sum the Sums into the 4 matrix elemets A,B,C,D
-------------------------------------------------------------------------------

  -- Stage 5: Computation
  SumEx   <= reg_SumEx_1   + reg_SumEx_2;
  SumEy   <= reg_SumEy_1   + reg_SumEy_2;
  SumExEy <= reg_SumExEy_1 + reg_SumExEy_2;  

  -- Stage 5: Registers
  process(clk,reset,sum1_valid,SumEx,SumEy,SumExEy)   
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        sums_valid      <= '0';
        reg_SumEx       <= (others => '0');
        reg_SumEy       <= (others => '0');
        reg_SumExEy     <= (others => '0');
      else
        sums_valid      <= sum1_valid;
        reg_SumEx       <= SumEx; 
        reg_SumEy       <= SumEy; 
        reg_SumExEy     <= SumExEy; 
      end if;
    end if;
  end process;

-------------------------------------------------------------------------------
-- Stage 6: Compute Cross multiplications
-------------------------------------------------------------------------------

  -- Stage 6: Computation
  AD    <= reg_SumEx * reg_SumEy;     -- 12bits * 12bits = 24bits
  BC    <= ieee.std_logic_arith.signed(reg_SumExEy)   * ieee.std_logic_arith.signed(reg_SumExEy);   -- 12bits * 12bits = 24bits
  AplsD <= ('0'&reg_SumEx) + ('0' & reg_SumEy);     -- 12bits + 12bits = 13bits

  -- Stage 6: Registers
  process(clk,reset,sums_valid,AD,BC,AplsD)
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        cross_valid  <= '0';
        AD_reg       <= (others => '0');
        BC_reg       <= (others => '0');
        AplsD_reg    <= (others => '0');
      else
        cross_valid  <= sums_valid;
        AD_reg       <= AD;
        BC_reg       <= BC;
        AplsD_reg    <= AplsD;
      end if;
    end if;
  end process;


-------------------------------------------------------------------------------
-- Stage 7: Compute determinate and trace of matrix
-------------------------------------------------------------------------------  

  -- Stage 7: computation 
  det   <= AD_reg - BC_reg;                     -- 24bits - 24bits  = 24bits
  trace <= AplsD_reg * AplsD_reg;               -- 13bits * 13bits  = 26bits

  -- Stage 7: Registers
  process(clk,reset,cross_valid,det,trace)   
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        det_valid       <= '0';
        reg_det         <= (others => '0');
        reg_trace       <= (others => '0');
      else
        det_valid       <= cross_valid;
        reg_det         <= det;
        reg_trace       <= trace(trace'length-1 downto 4);  -- .04*trace^2 ~= trace^2 >> 4                                                             
      end if;
    end if;
  end process;

-------------------------------------------------------------------------------
-- Stage 8: Compute harris value using determinate and trace
-------------------------------------------------------------------------------

  -- Stage 8: Computation
  value <= ieee.std_logic_arith.signed(reg_det) - conv_signed(ieee.std_logic_arith.signed(reg_trace),reg_det'length);

  -- Stage 8: Registers
  process(clk,reset,det_valid,value)   
  begin
    if clk'event and clk = '1' then
      if reset = '1' then
        result_valid    <= '0';
        reg_result      <= (others => '0');
      else
        result_valid    <= det_valid;
        reg_result      <= value;        
      end if;
    end if;
  end process;

-------------------------------------------------------------------------------
-- Stage 9: Saturate & Register the result
-------------------------------------------------------------------------------

  process(clk,reg_result)
  begin
    if reg_result(reg_result'length-1) = '1' then
      result <= (others => '0');
    else
      if reg_result(24 downto 16) /= conv_std_logic_vector('0',9) then
        result <= (others => '1');
      else
        result <= reg_result(15 downto 0);      
      end if;        
    end if;
  end process;  
 
  
-------------------------------------------------------------------------------
end IMP;






  
-- Template Clocked Process

-------------------------------------------------------------------------------
--  
-------------------------------------------------------------------------------
--  process(clk,reset)
--  begin
--    if clk'event and clk = '1' then
--      if reset = '1' then
--      else
--      end if;
--    end if;
--  end process; 


