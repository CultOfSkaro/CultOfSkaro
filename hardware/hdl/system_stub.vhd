-------------------------------------------------------------------------------
-- system_stub.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity system_stub is
  port (
    uart_rx : in std_logic;
    uart_tx : out std_logic;
    wireless_rx : in std_logic;
    wireless_tx : out std_logic;
    pwm_servo : out std_logic;
    pwm_esc : out std_logic;
    fpga_0_clk_1_sys_clk_pin : in std_logic;
    fpga_0_rst_1_sys_rst_pin : in std_logic;
    plb_quad_encoder_0_enc_a_in : in std_logic;
    plb_quad_encoder_0_enc_b_in : in std_logic;
    gpio_helios : inout std_logic_vector(4 downto 0);
    gpio_helios_game : inout std_logic_vector(15 downto 0);
    xps_mch_emc_flash_Mem_DQ : inout std_logic_vector(0 to 15);
    xps_mch_emc_flash_Mem_A_pin : out std_logic_vector(0 to 31);
    xps_mch_emc_flash_Mem_CEN_pin : out std_logic_vector(0 to 0);
    xps_mch_emc_flash_Mem_RPN_pin : out std_logic;
    xps_mch_emc_flash_Mem_BEN_pin : out std_logic_vector(0 to 1);
    xps_mch_emc_flash_Mem_QWEN_pin : out std_logic_vector(0 to 1);
    xps_mch_emc_flash_Mem_OEN_pin : out std_logic_vector(0 to 0);
    xps_mch_emc_sram_Mem_WEN_pin : out std_logic;
    xps_mch_emc_sram_Mem_A_pin : out std_logic_vector(0 to 31);
    xps_mch_emc_sram_Mem_CEN_pin : out std_logic_vector(0 to 0);
    xps_mch_emc_sram_Mem_OEN_pin : out std_logic_vector(0 to 0);
    xps_mch_emc_sram_Mem_BEN_pin : out std_logic_vector(0 to 3);
    xps_mch_emc_sram_Mem_DQ : inout std_logic_vector(0 to 31);
    xps_mch_emc_sram_Mem_ADV_LDN_pin : out std_logic;
    xps_mch_emc_sram_Mem_LBON_pin : out std_logic;
    sram_clk_pin : out std_logic;
    cam0_data : in std_logic_vector(7 downto 0);
    cam0_frame_valid : in std_logic;
    cam0_line_valid : in std_logic;
    cam0_pixclk : in std_logic;
    cam0_sclk : out std_logic;
    cam0_sdata : inout std_logic;
    cam0_reset_n : inout std_logic;
    cam_standby : out std_logic;
    usb_if_clk : in std_logic;
    usb_full_n : in std_logic;
    usb_empty_n : in std_logic;
    usb_alive : in std_logic;
    usb_sloe_n : out std_logic;
    usb_slrd_n : out std_logic;
    usb_slwr_n : out std_logic;
    usb_pktend_n : out std_logic;
    usb_fifoaddr : out std_logic_vector(1 downto 0);
    usb_fd : inout std_logic_vector(15 downto 0);
    sdram_clk : out std_logic;
    sdram_clk_fb : in std_logic;
    sdram_cke : out std_logic;
    sdram_cs_n : out std_logic;
    sdram_ras_n : out std_logic;
    sdram_cas_n : out std_logic;
    sdram_we_n : out std_logic;
    sdram_dqm : out std_logic_vector(3 downto 0);
    sdram_ba : out std_logic_vector(1 downto 0);
    sdram_addr : out std_logic_vector(11 downto 0);
    sdram_data : inout std_logic_vector(31 downto 0);
    gameboard_uart_rx : in std_logic;
    gameboard_uart_tx : out std_logic
  );
end system_stub;

architecture STRUCTURE of system_stub is

  component system is
    port (
      uart_rx : in std_logic;
      uart_tx : out std_logic;
      wireless_rx : in std_logic;
      wireless_tx : out std_logic;
      pwm_servo : out std_logic;
      pwm_esc : out std_logic;
      fpga_0_clk_1_sys_clk_pin : in std_logic;
      fpga_0_rst_1_sys_rst_pin : in std_logic;
      plb_quad_encoder_0_enc_a_in : in std_logic;
      plb_quad_encoder_0_enc_b_in : in std_logic;
      gpio_helios : inout std_logic_vector(4 downto 0);
      gpio_helios_game : inout std_logic_vector(15 downto 0);
      xps_mch_emc_flash_Mem_DQ : inout std_logic_vector(0 to 15);
      xps_mch_emc_flash_Mem_A_pin : out std_logic_vector(0 to 31);
      xps_mch_emc_flash_Mem_CEN_pin : out std_logic_vector(0 to 0);
      xps_mch_emc_flash_Mem_RPN_pin : out std_logic;
      xps_mch_emc_flash_Mem_BEN_pin : out std_logic_vector(0 to 1);
      xps_mch_emc_flash_Mem_QWEN_pin : out std_logic_vector(0 to 1);
      xps_mch_emc_flash_Mem_OEN_pin : out std_logic_vector(0 to 0);
      xps_mch_emc_sram_Mem_WEN_pin : out std_logic;
      xps_mch_emc_sram_Mem_A_pin : out std_logic_vector(0 to 31);
      xps_mch_emc_sram_Mem_CEN_pin : out std_logic_vector(0 to 0);
      xps_mch_emc_sram_Mem_OEN_pin : out std_logic_vector(0 to 0);
      xps_mch_emc_sram_Mem_BEN_pin : out std_logic_vector(0 to 3);
      xps_mch_emc_sram_Mem_DQ : inout std_logic_vector(0 to 31);
      xps_mch_emc_sram_Mem_ADV_LDN_pin : out std_logic;
      xps_mch_emc_sram_Mem_LBON_pin : out std_logic;
      sram_clk_pin : out std_logic;
      cam0_data : in std_logic_vector(7 downto 0);
      cam0_frame_valid : in std_logic;
      cam0_line_valid : in std_logic;
      cam0_pixclk : in std_logic;
      cam0_sclk : out std_logic;
      cam0_sdata : inout std_logic;
      cam0_reset_n : inout std_logic;
      cam_standby : out std_logic;
      usb_if_clk : in std_logic;
      usb_full_n : in std_logic;
      usb_empty_n : in std_logic;
      usb_alive : in std_logic;
      usb_sloe_n : out std_logic;
      usb_slrd_n : out std_logic;
      usb_slwr_n : out std_logic;
      usb_pktend_n : out std_logic;
      usb_fifoaddr : out std_logic_vector(1 downto 0);
      usb_fd : inout std_logic_vector(15 downto 0);
      sdram_clk : out std_logic;
      sdram_clk_fb : in std_logic;
      sdram_cke : out std_logic;
      sdram_cs_n : out std_logic;
      sdram_ras_n : out std_logic;
      sdram_cas_n : out std_logic;
      sdram_we_n : out std_logic;
      sdram_dqm : out std_logic_vector(3 downto 0);
      sdram_ba : out std_logic_vector(1 downto 0);
      sdram_addr : out std_logic_vector(11 downto 0);
      sdram_data : inout std_logic_vector(31 downto 0);
      gameboard_uart_rx : in std_logic;
      gameboard_uart_tx : out std_logic
    );
  end component;

  attribute BOX_TYPE : STRING;
  attribute BOX_TYPE of system : component is "user_black_box";

begin

  system_i : system
    port map (
      uart_rx => uart_rx,
      uart_tx => uart_tx,
      wireless_rx => wireless_rx,
      wireless_tx => wireless_tx,
      pwm_servo => pwm_servo,
      pwm_esc => pwm_esc,
      fpga_0_clk_1_sys_clk_pin => fpga_0_clk_1_sys_clk_pin,
      fpga_0_rst_1_sys_rst_pin => fpga_0_rst_1_sys_rst_pin,
      plb_quad_encoder_0_enc_a_in => plb_quad_encoder_0_enc_a_in,
      plb_quad_encoder_0_enc_b_in => plb_quad_encoder_0_enc_b_in,
      gpio_helios => gpio_helios,
      gpio_helios_game => gpio_helios_game,
      xps_mch_emc_flash_Mem_DQ => xps_mch_emc_flash_Mem_DQ,
      xps_mch_emc_flash_Mem_A_pin => xps_mch_emc_flash_Mem_A_pin,
      xps_mch_emc_flash_Mem_CEN_pin => xps_mch_emc_flash_Mem_CEN_pin(0 to 0),
      xps_mch_emc_flash_Mem_RPN_pin => xps_mch_emc_flash_Mem_RPN_pin,
      xps_mch_emc_flash_Mem_BEN_pin => xps_mch_emc_flash_Mem_BEN_pin,
      xps_mch_emc_flash_Mem_QWEN_pin => xps_mch_emc_flash_Mem_QWEN_pin,
      xps_mch_emc_flash_Mem_OEN_pin => xps_mch_emc_flash_Mem_OEN_pin(0 to 0),
      xps_mch_emc_sram_Mem_WEN_pin => xps_mch_emc_sram_Mem_WEN_pin,
      xps_mch_emc_sram_Mem_A_pin => xps_mch_emc_sram_Mem_A_pin,
      xps_mch_emc_sram_Mem_CEN_pin => xps_mch_emc_sram_Mem_CEN_pin(0 to 0),
      xps_mch_emc_sram_Mem_OEN_pin => xps_mch_emc_sram_Mem_OEN_pin(0 to 0),
      xps_mch_emc_sram_Mem_BEN_pin => xps_mch_emc_sram_Mem_BEN_pin,
      xps_mch_emc_sram_Mem_DQ => xps_mch_emc_sram_Mem_DQ,
      xps_mch_emc_sram_Mem_ADV_LDN_pin => xps_mch_emc_sram_Mem_ADV_LDN_pin,
      xps_mch_emc_sram_Mem_LBON_pin => xps_mch_emc_sram_Mem_LBON_pin,
      sram_clk_pin => sram_clk_pin,
      cam0_data => cam0_data,
      cam0_frame_valid => cam0_frame_valid,
      cam0_line_valid => cam0_line_valid,
      cam0_pixclk => cam0_pixclk,
      cam0_sclk => cam0_sclk,
      cam0_sdata => cam0_sdata,
      cam0_reset_n => cam0_reset_n,
      cam_standby => cam_standby,
      usb_if_clk => usb_if_clk,
      usb_full_n => usb_full_n,
      usb_empty_n => usb_empty_n,
      usb_alive => usb_alive,
      usb_sloe_n => usb_sloe_n,
      usb_slrd_n => usb_slrd_n,
      usb_slwr_n => usb_slwr_n,
      usb_pktend_n => usb_pktend_n,
      usb_fifoaddr => usb_fifoaddr,
      usb_fd => usb_fd,
      sdram_clk => sdram_clk,
      sdram_clk_fb => sdram_clk_fb,
      sdram_cke => sdram_cke,
      sdram_cs_n => sdram_cs_n,
      sdram_ras_n => sdram_ras_n,
      sdram_cas_n => sdram_cas_n,
      sdram_we_n => sdram_we_n,
      sdram_dqm => sdram_dqm,
      sdram_ba => sdram_ba,
      sdram_addr => sdram_addr,
      sdram_data => sdram_data,
      gameboard_uart_rx => gameboard_uart_rx,
      gameboard_uart_tx => gameboard_uart_tx
    );

end architecture STRUCTURE;

