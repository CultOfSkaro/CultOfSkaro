-------------------------------------------------------------------------------
-- system.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity system is
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
end system;

architecture STRUCTURE of system is

  component ppc405_0_wrapper is
    port (
      C405CPMCORESLEEPREQ : out std_logic;
      C405CPMMSRCE : out std_logic;
      C405CPMMSREE : out std_logic;
      C405CPMTIMERIRQ : out std_logic;
      C405CPMTIMERRESETREQ : out std_logic;
      C405XXXMACHINECHECK : out std_logic;
      CPMC405CLOCK : in std_logic;
      CPMC405CORECLKINACTIVE : in std_logic;
      CPMC405CPUCLKEN : in std_logic;
      CPMC405JTAGCLKEN : in std_logic;
      CPMC405TIMERCLKEN : in std_logic;
      CPMC405TIMERTICK : in std_logic;
      MCBCPUCLKEN : in std_logic;
      MCBTIMEREN : in std_logic;
      MCPPCRST : in std_logic;
      CPMDCRCLK : in std_logic;
      CPMFCMCLK : in std_logic;
      C405RSTCHIPRESETREQ : out std_logic;
      C405RSTCORERESETREQ : out std_logic;
      C405RSTSYSRESETREQ : out std_logic;
      RSTC405RESETCHIP : in std_logic;
      RSTC405RESETCORE : in std_logic;
      RSTC405RESETSYS : in std_logic;
      APUFCMDECODED : out std_logic;
      APUFCMDECUDI : out std_logic_vector(0 to 2);
      APUFCMDECUDIVALID : out std_logic;
      APUFCMENDIAN : out std_logic;
      APUFCMFLUSH : out std_logic;
      APUFCMINSTRUCTION : out std_logic_vector(0 to 31);
      APUFCMINSTRVALID : out std_logic;
      APUFCMLOADBYTEEN : out std_logic_vector(0 to 3);
      APUFCMLOADDATA : out std_logic_vector(0 to 31);
      APUFCMLOADDVALID : out std_logic;
      APUFCMOPERANDVALID : out std_logic;
      APUFCMRADATA : out std_logic_vector(0 to 31);
      APUFCMRBDATA : out std_logic_vector(0 to 31);
      APUFCMWRITEBACKOK : out std_logic;
      APUFCMXERCA : out std_logic;
      FCMAPUCR : in std_logic_vector(0 to 3);
      FCMAPUDCDCREN : in std_logic;
      FCMAPUDCDFORCEALIGN : in std_logic;
      FCMAPUDCDFORCEBESTEERING : in std_logic;
      FCMAPUDCDFPUOP : in std_logic;
      FCMAPUDCDGPRWRITE : in std_logic;
      FCMAPUDCDLDSTBYTE : in std_logic;
      FCMAPUDCDLDSTDW : in std_logic;
      FCMAPUDCDLDSTHW : in std_logic;
      FCMAPUDCDLDSTQW : in std_logic;
      FCMAPUDCDLDSTWD : in std_logic;
      FCMAPUDCDLOAD : in std_logic;
      FCMAPUDCDPRIVOP : in std_logic;
      FCMAPUDCDRAEN : in std_logic;
      FCMAPUDCDRBEN : in std_logic;
      FCMAPUDCDSTORE : in std_logic;
      FCMAPUDCDTRAPBE : in std_logic;
      FCMAPUDCDTRAPLE : in std_logic;
      FCMAPUDCDUPDATE : in std_logic;
      FCMAPUDCDXERCAEN : in std_logic;
      FCMAPUDCDXEROVEN : in std_logic;
      FCMAPUDECODEBUSY : in std_logic;
      FCMAPUDONE : in std_logic;
      FCMAPUEXCEPTION : in std_logic;
      FCMAPUEXEBLOCKINGMCO : in std_logic;
      FCMAPUEXECRFIELD : in std_logic_vector(0 to 2);
      FCMAPUEXENONBLOCKINGMCO : in std_logic;
      FCMAPUINSTRACK : in std_logic;
      FCMAPULOADWAIT : in std_logic;
      FCMAPURESULT : in std_logic_vector(0 to 31);
      FCMAPURESULTVALID : in std_logic;
      FCMAPUSLEEPNOTREADY : in std_logic;
      FCMAPUXERCA : in std_logic;
      FCMAPUXEROV : in std_logic;
      IPLB0_PLB_Clk : in std_logic;
      IPLB0_PLB_Rst : in std_logic;
      IPLB0_PLB_MBusy : in std_logic;
      IPLB0_PLB_MRdErr : in std_logic;
      IPLB0_PLB_MWrErr : in std_logic;
      IPLB0_PLB_MWrBTerm : in std_logic;
      IPLB0_PLB_MWrDAck : in std_logic;
      IPLB0_PLB_MAddrAck : in std_logic;
      IPLB0_PLB_MRdBTerm : in std_logic;
      IPLB0_PLB_MRdDAck : in std_logic;
      IPLB0_PLB_MRdDBus : in std_logic_vector(0 to 63);
      IPLB0_PLB_MRearbitrate : in std_logic;
      IPLB0_PLB_MSSize : in std_logic_vector(0 to 1);
      IPLB0_PLB_MTimeout : in std_logic;
      IPLB0_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      IPLB0_M_ABus : out std_logic_vector(0 to 31);
      IPLB0_M_BE : out std_logic_vector(0 to 7);
      IPLB0_M_MSize : out std_logic_vector(0 to 1);
      IPLB0_M_rdBurst : out std_logic;
      IPLB0_M_request : out std_logic;
      IPLB0_M_RNW : out std_logic;
      IPLB0_M_size : out std_logic_vector(0 to 3);
      IPLB0_M_wrBurst : out std_logic;
      IPLB0_M_wrDBus : out std_logic_vector(0 to 63);
      IPLB0_M_abort : out std_logic;
      IPLB0_M_UABus : out std_logic_vector(0 to 31);
      IPLB0_M_busLock : out std_logic;
      IPLB0_M_lockErr : out std_logic;
      IPLB0_M_priority : out std_logic_vector(0 to 1);
      IPLB0_M_type : out std_logic_vector(0 to 2);
      IPLB0_M_TAttribute : out std_logic_vector(0 to 15);
      DPLB0_PLB_Clk : in std_logic;
      DPLB0_PLB_Rst : in std_logic;
      DPLB0_PLB_MBusy : in std_logic;
      DPLB0_PLB_MRdErr : in std_logic;
      DPLB0_PLB_MWrErr : in std_logic;
      DPLB0_PLB_MWrBTerm : in std_logic;
      DPLB0_PLB_MWrDAck : in std_logic;
      DPLB0_PLB_MAddrAck : in std_logic;
      DPLB0_PLB_MRdBTerm : in std_logic;
      DPLB0_PLB_MRdDAck : in std_logic;
      DPLB0_PLB_MRdDBus : in std_logic_vector(0 to 63);
      DPLB0_PLB_MRearbitrate : in std_logic;
      DPLB0_PLB_MSSize : in std_logic_vector(0 to 1);
      DPLB0_PLB_MTimeout : in std_logic;
      DPLB0_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      DPLB0_M_ABus : out std_logic_vector(0 to 31);
      DPLB0_M_BE : out std_logic_vector(0 to 7);
      DPLB0_M_MSize : out std_logic_vector(0 to 1);
      DPLB0_M_rdBurst : out std_logic;
      DPLB0_M_request : out std_logic;
      DPLB0_M_RNW : out std_logic;
      DPLB0_M_size : out std_logic_vector(0 to 3);
      DPLB0_M_wrBurst : out std_logic;
      DPLB0_M_wrDBus : out std_logic_vector(0 to 63);
      DPLB0_M_abort : out std_logic;
      DPLB0_M_UABus : out std_logic_vector(0 to 31);
      DPLB0_M_busLock : out std_logic;
      DPLB0_M_lockErr : out std_logic;
      DPLB0_M_priority : out std_logic_vector(0 to 1);
      DPLB0_M_type : out std_logic_vector(0 to 2);
      DPLB0_M_TAttribute : out std_logic_vector(0 to 15);
      IPLB1_PLB_Clk : in std_logic;
      IPLB1_PLB_Rst : in std_logic;
      IPLB1_PLB_MBusy : in std_logic;
      IPLB1_PLB_MRdErr : in std_logic;
      IPLB1_PLB_MWrErr : in std_logic;
      IPLB1_PLB_MWrBTerm : in std_logic;
      IPLB1_PLB_MWrDAck : in std_logic;
      IPLB1_PLB_MAddrAck : in std_logic;
      IPLB1_PLB_MRdBTerm : in std_logic;
      IPLB1_PLB_MRdDAck : in std_logic;
      IPLB1_PLB_MRdDBus : in std_logic_vector(0 to 63);
      IPLB1_PLB_MRearbitrate : in std_logic;
      IPLB1_PLB_MSSize : in std_logic_vector(0 to 1);
      IPLB1_PLB_MTimeout : in std_logic;
      IPLB1_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      IPLB1_M_ABus : out std_logic_vector(0 to 31);
      IPLB1_M_BE : out std_logic_vector(0 to 7);
      IPLB1_M_MSize : out std_logic_vector(0 to 1);
      IPLB1_M_rdBurst : out std_logic;
      IPLB1_M_request : out std_logic;
      IPLB1_M_RNW : out std_logic;
      IPLB1_M_size : out std_logic_vector(0 to 3);
      IPLB1_M_wrBurst : out std_logic;
      IPLB1_M_wrDBus : out std_logic_vector(0 to 63);
      IPLB1_M_abort : out std_logic;
      IPLB1_M_UABus : out std_logic_vector(0 to 31);
      IPLB1_M_busLock : out std_logic;
      IPLB1_M_lockErr : out std_logic;
      IPLB1_M_priority : out std_logic_vector(0 to 1);
      IPLB1_M_type : out std_logic_vector(0 to 2);
      IPLB1_M_TAttribute : out std_logic_vector(0 to 15);
      DPLB1_PLB_Clk : in std_logic;
      DPLB1_PLB_Rst : in std_logic;
      DPLB1_PLB_MBusy : in std_logic;
      DPLB1_PLB_MRdErr : in std_logic;
      DPLB1_PLB_MWrErr : in std_logic;
      DPLB1_PLB_MWrBTerm : in std_logic;
      DPLB1_PLB_MWrDAck : in std_logic;
      DPLB1_PLB_MAddrAck : in std_logic;
      DPLB1_PLB_MRdBTerm : in std_logic;
      DPLB1_PLB_MRdDAck : in std_logic;
      DPLB1_PLB_MRdDBus : in std_logic_vector(0 to 63);
      DPLB1_PLB_MRearbitrate : in std_logic;
      DPLB1_PLB_MSSize : in std_logic_vector(0 to 1);
      DPLB1_PLB_MTimeout : in std_logic;
      DPLB1_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      DPLB1_M_ABus : out std_logic_vector(0 to 31);
      DPLB1_M_BE : out std_logic_vector(0 to 7);
      DPLB1_M_MSize : out std_logic_vector(0 to 1);
      DPLB1_M_rdBurst : out std_logic;
      DPLB1_M_request : out std_logic;
      DPLB1_M_RNW : out std_logic;
      DPLB1_M_size : out std_logic_vector(0 to 3);
      DPLB1_M_wrBurst : out std_logic;
      DPLB1_M_wrDBus : out std_logic_vector(0 to 63);
      DPLB1_M_abort : out std_logic;
      DPLB1_M_UABus : out std_logic_vector(0 to 31);
      DPLB1_M_busLock : out std_logic;
      DPLB1_M_lockErr : out std_logic;
      DPLB1_M_priority : out std_logic_vector(0 to 1);
      DPLB1_M_type : out std_logic_vector(0 to 2);
      DPLB1_M_TAttribute : out std_logic_vector(0 to 15);
      BRAMDSOCMCLK : in std_logic;
      BRAMDSOCMRDDBUS : in std_logic_vector(0 to 31);
      DSARCVALUE : in std_logic_vector(0 to 7);
      DSCNTLVALUE : in std_logic_vector(0 to 7);
      DSOCMBRAMABUS : out std_logic_vector(8 to 29);
      DSOCMBRAMBYTEWRITE : out std_logic_vector(0 to 3);
      DSOCMBRAMEN : out std_logic;
      DSOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
      DSOCMBUSY : out std_logic;
      DSOCMRDADDRVALID : out std_logic;
      DSOCMWRADDRVALID : out std_logic;
      DSOCMRWCOMPLETE : in std_logic;
      BRAMISOCMCLK : in std_logic;
      BRAMISOCMRDDBUS : in std_logic_vector(0 to 63);
      BRAMISOCMDCRRDDBUS : in std_logic_vector(0 to 31);
      ISARCVALUE : in std_logic_vector(0 to 7);
      ISCNTLVALUE : in std_logic_vector(0 to 7);
      ISOCMBRAMEN : out std_logic;
      ISOCMBRAMEVENWRITEEN : out std_logic;
      ISOCMBRAMODDWRITEEN : out std_logic;
      ISOCMBRAMRDABUS : out std_logic_vector(8 to 28);
      ISOCMBRAMWRABUS : out std_logic_vector(8 to 28);
      ISOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
      ISOCMDCRBRAMEVENEN : out std_logic;
      ISOCMDCRBRAMODDEN : out std_logic;
      ISOCMDCRBRAMRDSELECT : out std_logic;
      DCREMACABUS : out std_logic_vector(8 to 9);
      DCREMACCLK : out std_logic;
      DCREMACDBUS : out std_logic_vector(0 to 31);
      DCREMACENABLER : out std_logic;
      DCREMACREAD : out std_logic;
      DCREMACWRITE : out std_logic;
      EMACDCRACK : in std_logic;
      EMACDCRDBUS : in std_logic_vector(0 to 31);
      EXTDCRABUS : out std_logic_vector(0 to 9);
      EXTDCRDBUSOUT : out std_logic_vector(0 to 31);
      EXTDCRREAD : out std_logic;
      EXTDCRWRITE : out std_logic;
      EXTDCRACK : in std_logic;
      EXTDCRDBUSIN : in std_logic_vector(0 to 31);
      EICC405CRITINPUTIRQ : in std_logic;
      EICC405EXTINPUTIRQ : in std_logic;
      C405JTGCAPTUREDR : out std_logic;
      C405JTGEXTEST : out std_logic;
      C405JTGPGMOUT : out std_logic;
      C405JTGSHIFTDR : out std_logic;
      C405JTGTDO : out std_logic;
      C405JTGTDOEN : out std_logic;
      C405JTGUPDATEDR : out std_logic;
      MCBJTAGEN : in std_logic;
      JTGC405BNDSCANTDO : in std_logic;
      JTGC405TCK : in std_logic;
      JTGC405TDI : in std_logic;
      JTGC405TMS : in std_logic;
      JTGC405TRSTNEG : in std_logic;
      C405DBGMSRWE : out std_logic;
      C405DBGSTOPACK : out std_logic;
      C405DBGWBCOMPLETE : out std_logic;
      C405DBGWBFULL : out std_logic;
      C405DBGWBIAR : out std_logic_vector(0 to 29);
      DBGC405DEBUGHALT : in std_logic;
      DBGC405DEBUGHALTNEG : in std_logic;
      DBGC405EXTBUSHOLDACK : in std_logic;
      DBGC405UNCONDDEBUGEVENT : in std_logic;
      C405DBGLOADDATAONAPUDBUS : out std_logic;
      C405TRCCYCLE : out std_logic;
      C405TRCEVENEXECUTIONSTATUS : out std_logic_vector(0 to 1);
      C405TRCODDEXECUTIONSTATUS : out std_logic_vector(0 to 1);
      C405TRCTRACESTATUS : out std_logic_vector(0 to 3);
      C405TRCTRIGGEREVENTOUT : out std_logic;
      C405TRCTRIGGEREVENTTYPE : out std_logic_vector(0 to 10);
      TRCC405TRACEDISABLE : in std_logic;
      TRCC405TRIGGEREVENTIN : in std_logic
    );
  end component;

  component ppc405_1_wrapper is
    port (
      C405CPMCORESLEEPREQ : out std_logic;
      C405CPMMSRCE : out std_logic;
      C405CPMMSREE : out std_logic;
      C405CPMTIMERIRQ : out std_logic;
      C405CPMTIMERRESETREQ : out std_logic;
      C405XXXMACHINECHECK : out std_logic;
      CPMC405CLOCK : in std_logic;
      CPMC405CORECLKINACTIVE : in std_logic;
      CPMC405CPUCLKEN : in std_logic;
      CPMC405JTAGCLKEN : in std_logic;
      CPMC405TIMERCLKEN : in std_logic;
      CPMC405TIMERTICK : in std_logic;
      MCBCPUCLKEN : in std_logic;
      MCBTIMEREN : in std_logic;
      MCPPCRST : in std_logic;
      CPMDCRCLK : in std_logic;
      CPMFCMCLK : in std_logic;
      C405RSTCHIPRESETREQ : out std_logic;
      C405RSTCORERESETREQ : out std_logic;
      C405RSTSYSRESETREQ : out std_logic;
      RSTC405RESETCHIP : in std_logic;
      RSTC405RESETCORE : in std_logic;
      RSTC405RESETSYS : in std_logic;
      APUFCMDECODED : out std_logic;
      APUFCMDECUDI : out std_logic_vector(0 to 2);
      APUFCMDECUDIVALID : out std_logic;
      APUFCMENDIAN : out std_logic;
      APUFCMFLUSH : out std_logic;
      APUFCMINSTRUCTION : out std_logic_vector(0 to 31);
      APUFCMINSTRVALID : out std_logic;
      APUFCMLOADBYTEEN : out std_logic_vector(0 to 3);
      APUFCMLOADDATA : out std_logic_vector(0 to 31);
      APUFCMLOADDVALID : out std_logic;
      APUFCMOPERANDVALID : out std_logic;
      APUFCMRADATA : out std_logic_vector(0 to 31);
      APUFCMRBDATA : out std_logic_vector(0 to 31);
      APUFCMWRITEBACKOK : out std_logic;
      APUFCMXERCA : out std_logic;
      FCMAPUCR : in std_logic_vector(0 to 3);
      FCMAPUDCDCREN : in std_logic;
      FCMAPUDCDFORCEALIGN : in std_logic;
      FCMAPUDCDFORCEBESTEERING : in std_logic;
      FCMAPUDCDFPUOP : in std_logic;
      FCMAPUDCDGPRWRITE : in std_logic;
      FCMAPUDCDLDSTBYTE : in std_logic;
      FCMAPUDCDLDSTDW : in std_logic;
      FCMAPUDCDLDSTHW : in std_logic;
      FCMAPUDCDLDSTQW : in std_logic;
      FCMAPUDCDLDSTWD : in std_logic;
      FCMAPUDCDLOAD : in std_logic;
      FCMAPUDCDPRIVOP : in std_logic;
      FCMAPUDCDRAEN : in std_logic;
      FCMAPUDCDRBEN : in std_logic;
      FCMAPUDCDSTORE : in std_logic;
      FCMAPUDCDTRAPBE : in std_logic;
      FCMAPUDCDTRAPLE : in std_logic;
      FCMAPUDCDUPDATE : in std_logic;
      FCMAPUDCDXERCAEN : in std_logic;
      FCMAPUDCDXEROVEN : in std_logic;
      FCMAPUDECODEBUSY : in std_logic;
      FCMAPUDONE : in std_logic;
      FCMAPUEXCEPTION : in std_logic;
      FCMAPUEXEBLOCKINGMCO : in std_logic;
      FCMAPUEXECRFIELD : in std_logic_vector(0 to 2);
      FCMAPUEXENONBLOCKINGMCO : in std_logic;
      FCMAPUINSTRACK : in std_logic;
      FCMAPULOADWAIT : in std_logic;
      FCMAPURESULT : in std_logic_vector(0 to 31);
      FCMAPURESULTVALID : in std_logic;
      FCMAPUSLEEPNOTREADY : in std_logic;
      FCMAPUXERCA : in std_logic;
      FCMAPUXEROV : in std_logic;
      IPLB0_PLB_Clk : in std_logic;
      IPLB0_PLB_Rst : in std_logic;
      IPLB0_PLB_MBusy : in std_logic;
      IPLB0_PLB_MRdErr : in std_logic;
      IPLB0_PLB_MWrErr : in std_logic;
      IPLB0_PLB_MWrBTerm : in std_logic;
      IPLB0_PLB_MWrDAck : in std_logic;
      IPLB0_PLB_MAddrAck : in std_logic;
      IPLB0_PLB_MRdBTerm : in std_logic;
      IPLB0_PLB_MRdDAck : in std_logic;
      IPLB0_PLB_MRdDBus : in std_logic_vector(0 to 63);
      IPLB0_PLB_MRearbitrate : in std_logic;
      IPLB0_PLB_MSSize : in std_logic_vector(0 to 1);
      IPLB0_PLB_MTimeout : in std_logic;
      IPLB0_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      IPLB0_M_ABus : out std_logic_vector(0 to 31);
      IPLB0_M_BE : out std_logic_vector(0 to 7);
      IPLB0_M_MSize : out std_logic_vector(0 to 1);
      IPLB0_M_rdBurst : out std_logic;
      IPLB0_M_request : out std_logic;
      IPLB0_M_RNW : out std_logic;
      IPLB0_M_size : out std_logic_vector(0 to 3);
      IPLB0_M_wrBurst : out std_logic;
      IPLB0_M_wrDBus : out std_logic_vector(0 to 63);
      IPLB0_M_abort : out std_logic;
      IPLB0_M_UABus : out std_logic_vector(0 to 31);
      IPLB0_M_busLock : out std_logic;
      IPLB0_M_lockErr : out std_logic;
      IPLB0_M_priority : out std_logic_vector(0 to 1);
      IPLB0_M_type : out std_logic_vector(0 to 2);
      IPLB0_M_TAttribute : out std_logic_vector(0 to 15);
      DPLB0_PLB_Clk : in std_logic;
      DPLB0_PLB_Rst : in std_logic;
      DPLB0_PLB_MBusy : in std_logic;
      DPLB0_PLB_MRdErr : in std_logic;
      DPLB0_PLB_MWrErr : in std_logic;
      DPLB0_PLB_MWrBTerm : in std_logic;
      DPLB0_PLB_MWrDAck : in std_logic;
      DPLB0_PLB_MAddrAck : in std_logic;
      DPLB0_PLB_MRdBTerm : in std_logic;
      DPLB0_PLB_MRdDAck : in std_logic;
      DPLB0_PLB_MRdDBus : in std_logic_vector(0 to 63);
      DPLB0_PLB_MRearbitrate : in std_logic;
      DPLB0_PLB_MSSize : in std_logic_vector(0 to 1);
      DPLB0_PLB_MTimeout : in std_logic;
      DPLB0_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      DPLB0_M_ABus : out std_logic_vector(0 to 31);
      DPLB0_M_BE : out std_logic_vector(0 to 7);
      DPLB0_M_MSize : out std_logic_vector(0 to 1);
      DPLB0_M_rdBurst : out std_logic;
      DPLB0_M_request : out std_logic;
      DPLB0_M_RNW : out std_logic;
      DPLB0_M_size : out std_logic_vector(0 to 3);
      DPLB0_M_wrBurst : out std_logic;
      DPLB0_M_wrDBus : out std_logic_vector(0 to 63);
      DPLB0_M_abort : out std_logic;
      DPLB0_M_UABus : out std_logic_vector(0 to 31);
      DPLB0_M_busLock : out std_logic;
      DPLB0_M_lockErr : out std_logic;
      DPLB0_M_priority : out std_logic_vector(0 to 1);
      DPLB0_M_type : out std_logic_vector(0 to 2);
      DPLB0_M_TAttribute : out std_logic_vector(0 to 15);
      IPLB1_PLB_Clk : in std_logic;
      IPLB1_PLB_Rst : in std_logic;
      IPLB1_PLB_MBusy : in std_logic;
      IPLB1_PLB_MRdErr : in std_logic;
      IPLB1_PLB_MWrErr : in std_logic;
      IPLB1_PLB_MWrBTerm : in std_logic;
      IPLB1_PLB_MWrDAck : in std_logic;
      IPLB1_PLB_MAddrAck : in std_logic;
      IPLB1_PLB_MRdBTerm : in std_logic;
      IPLB1_PLB_MRdDAck : in std_logic;
      IPLB1_PLB_MRdDBus : in std_logic_vector(0 to 63);
      IPLB1_PLB_MRearbitrate : in std_logic;
      IPLB1_PLB_MSSize : in std_logic_vector(0 to 1);
      IPLB1_PLB_MTimeout : in std_logic;
      IPLB1_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      IPLB1_M_ABus : out std_logic_vector(0 to 31);
      IPLB1_M_BE : out std_logic_vector(0 to 7);
      IPLB1_M_MSize : out std_logic_vector(0 to 1);
      IPLB1_M_rdBurst : out std_logic;
      IPLB1_M_request : out std_logic;
      IPLB1_M_RNW : out std_logic;
      IPLB1_M_size : out std_logic_vector(0 to 3);
      IPLB1_M_wrBurst : out std_logic;
      IPLB1_M_wrDBus : out std_logic_vector(0 to 63);
      IPLB1_M_abort : out std_logic;
      IPLB1_M_UABus : out std_logic_vector(0 to 31);
      IPLB1_M_busLock : out std_logic;
      IPLB1_M_lockErr : out std_logic;
      IPLB1_M_priority : out std_logic_vector(0 to 1);
      IPLB1_M_type : out std_logic_vector(0 to 2);
      IPLB1_M_TAttribute : out std_logic_vector(0 to 15);
      DPLB1_PLB_Clk : in std_logic;
      DPLB1_PLB_Rst : in std_logic;
      DPLB1_PLB_MBusy : in std_logic;
      DPLB1_PLB_MRdErr : in std_logic;
      DPLB1_PLB_MWrErr : in std_logic;
      DPLB1_PLB_MWrBTerm : in std_logic;
      DPLB1_PLB_MWrDAck : in std_logic;
      DPLB1_PLB_MAddrAck : in std_logic;
      DPLB1_PLB_MRdBTerm : in std_logic;
      DPLB1_PLB_MRdDAck : in std_logic;
      DPLB1_PLB_MRdDBus : in std_logic_vector(0 to 63);
      DPLB1_PLB_MRearbitrate : in std_logic;
      DPLB1_PLB_MSSize : in std_logic_vector(0 to 1);
      DPLB1_PLB_MTimeout : in std_logic;
      DPLB1_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      DPLB1_M_ABus : out std_logic_vector(0 to 31);
      DPLB1_M_BE : out std_logic_vector(0 to 7);
      DPLB1_M_MSize : out std_logic_vector(0 to 1);
      DPLB1_M_rdBurst : out std_logic;
      DPLB1_M_request : out std_logic;
      DPLB1_M_RNW : out std_logic;
      DPLB1_M_size : out std_logic_vector(0 to 3);
      DPLB1_M_wrBurst : out std_logic;
      DPLB1_M_wrDBus : out std_logic_vector(0 to 63);
      DPLB1_M_abort : out std_logic;
      DPLB1_M_UABus : out std_logic_vector(0 to 31);
      DPLB1_M_busLock : out std_logic;
      DPLB1_M_lockErr : out std_logic;
      DPLB1_M_priority : out std_logic_vector(0 to 1);
      DPLB1_M_type : out std_logic_vector(0 to 2);
      DPLB1_M_TAttribute : out std_logic_vector(0 to 15);
      BRAMDSOCMCLK : in std_logic;
      BRAMDSOCMRDDBUS : in std_logic_vector(0 to 31);
      DSARCVALUE : in std_logic_vector(0 to 7);
      DSCNTLVALUE : in std_logic_vector(0 to 7);
      DSOCMBRAMABUS : out std_logic_vector(8 to 29);
      DSOCMBRAMBYTEWRITE : out std_logic_vector(0 to 3);
      DSOCMBRAMEN : out std_logic;
      DSOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
      DSOCMBUSY : out std_logic;
      DSOCMRDADDRVALID : out std_logic;
      DSOCMWRADDRVALID : out std_logic;
      DSOCMRWCOMPLETE : in std_logic;
      BRAMISOCMCLK : in std_logic;
      BRAMISOCMRDDBUS : in std_logic_vector(0 to 63);
      BRAMISOCMDCRRDDBUS : in std_logic_vector(0 to 31);
      ISARCVALUE : in std_logic_vector(0 to 7);
      ISCNTLVALUE : in std_logic_vector(0 to 7);
      ISOCMBRAMEN : out std_logic;
      ISOCMBRAMEVENWRITEEN : out std_logic;
      ISOCMBRAMODDWRITEEN : out std_logic;
      ISOCMBRAMRDABUS : out std_logic_vector(8 to 28);
      ISOCMBRAMWRABUS : out std_logic_vector(8 to 28);
      ISOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
      ISOCMDCRBRAMEVENEN : out std_logic;
      ISOCMDCRBRAMODDEN : out std_logic;
      ISOCMDCRBRAMRDSELECT : out std_logic;
      DCREMACABUS : out std_logic_vector(8 to 9);
      DCREMACCLK : out std_logic;
      DCREMACDBUS : out std_logic_vector(0 to 31);
      DCREMACENABLER : out std_logic;
      DCREMACREAD : out std_logic;
      DCREMACWRITE : out std_logic;
      EMACDCRACK : in std_logic;
      EMACDCRDBUS : in std_logic_vector(0 to 31);
      EXTDCRABUS : out std_logic_vector(0 to 9);
      EXTDCRDBUSOUT : out std_logic_vector(0 to 31);
      EXTDCRREAD : out std_logic;
      EXTDCRWRITE : out std_logic;
      EXTDCRACK : in std_logic;
      EXTDCRDBUSIN : in std_logic_vector(0 to 31);
      EICC405CRITINPUTIRQ : in std_logic;
      EICC405EXTINPUTIRQ : in std_logic;
      C405JTGCAPTUREDR : out std_logic;
      C405JTGEXTEST : out std_logic;
      C405JTGPGMOUT : out std_logic;
      C405JTGSHIFTDR : out std_logic;
      C405JTGTDO : out std_logic;
      C405JTGTDOEN : out std_logic;
      C405JTGUPDATEDR : out std_logic;
      MCBJTAGEN : in std_logic;
      JTGC405BNDSCANTDO : in std_logic;
      JTGC405TCK : in std_logic;
      JTGC405TDI : in std_logic;
      JTGC405TMS : in std_logic;
      JTGC405TRSTNEG : in std_logic;
      C405DBGMSRWE : out std_logic;
      C405DBGSTOPACK : out std_logic;
      C405DBGWBCOMPLETE : out std_logic;
      C405DBGWBFULL : out std_logic;
      C405DBGWBIAR : out std_logic_vector(0 to 29);
      DBGC405DEBUGHALT : in std_logic;
      DBGC405DEBUGHALTNEG : in std_logic;
      DBGC405EXTBUSHOLDACK : in std_logic;
      DBGC405UNCONDDEBUGEVENT : in std_logic;
      C405DBGLOADDATAONAPUDBUS : out std_logic;
      C405TRCCYCLE : out std_logic;
      C405TRCEVENEXECUTIONSTATUS : out std_logic_vector(0 to 1);
      C405TRCODDEXECUTIONSTATUS : out std_logic_vector(0 to 1);
      C405TRCTRACESTATUS : out std_logic_vector(0 to 3);
      C405TRCTRIGGEREVENTOUT : out std_logic;
      C405TRCTRIGGEREVENTTYPE : out std_logic_vector(0 to 10);
      TRCC405TRACEDISABLE : in std_logic;
      TRCC405TRIGGEREVENTIN : in std_logic
    );
  end component;

  component plb_0_wrapper is
    port (
      PLB_Clk : in std_logic;
      SYS_Rst : in std_logic;
      PLB_Rst : out std_logic;
      SPLB_Rst : out std_logic_vector(0 to 10);
      MPLB_Rst : out std_logic_vector(0 to 1);
      PLB_dcrAck : out std_logic;
      PLB_dcrDBus : out std_logic_vector(0 to 31);
      DCR_ABus : in std_logic_vector(0 to 9);
      DCR_DBus : in std_logic_vector(0 to 31);
      DCR_Read : in std_logic;
      DCR_Write : in std_logic;
      M_ABus : in std_logic_vector(0 to 63);
      M_UABus : in std_logic_vector(0 to 63);
      M_BE : in std_logic_vector(0 to 15);
      M_RNW : in std_logic_vector(0 to 1);
      M_abort : in std_logic_vector(0 to 1);
      M_busLock : in std_logic_vector(0 to 1);
      M_TAttribute : in std_logic_vector(0 to 31);
      M_lockErr : in std_logic_vector(0 to 1);
      M_MSize : in std_logic_vector(0 to 3);
      M_priority : in std_logic_vector(0 to 3);
      M_rdBurst : in std_logic_vector(0 to 1);
      M_request : in std_logic_vector(0 to 1);
      M_size : in std_logic_vector(0 to 7);
      M_type : in std_logic_vector(0 to 5);
      M_wrBurst : in std_logic_vector(0 to 1);
      M_wrDBus : in std_logic_vector(0 to 127);
      Sl_addrAck : in std_logic_vector(0 to 10);
      Sl_MRdErr : in std_logic_vector(0 to 21);
      Sl_MWrErr : in std_logic_vector(0 to 21);
      Sl_MBusy : in std_logic_vector(0 to 21);
      Sl_rdBTerm : in std_logic_vector(0 to 10);
      Sl_rdComp : in std_logic_vector(0 to 10);
      Sl_rdDAck : in std_logic_vector(0 to 10);
      Sl_rdDBus : in std_logic_vector(0 to 703);
      Sl_rdWdAddr : in std_logic_vector(0 to 43);
      Sl_rearbitrate : in std_logic_vector(0 to 10);
      Sl_SSize : in std_logic_vector(0 to 21);
      Sl_wait : in std_logic_vector(0 to 10);
      Sl_wrBTerm : in std_logic_vector(0 to 10);
      Sl_wrComp : in std_logic_vector(0 to 10);
      Sl_wrDAck : in std_logic_vector(0 to 10);
      Sl_MIRQ : in std_logic_vector(0 to 21);
      PLB_MIRQ : out std_logic_vector(0 to 1);
      PLB_ABus : out std_logic_vector(0 to 31);
      PLB_UABus : out std_logic_vector(0 to 31);
      PLB_BE : out std_logic_vector(0 to 7);
      PLB_MAddrAck : out std_logic_vector(0 to 1);
      PLB_MTimeout : out std_logic_vector(0 to 1);
      PLB_MBusy : out std_logic_vector(0 to 1);
      PLB_MRdErr : out std_logic_vector(0 to 1);
      PLB_MWrErr : out std_logic_vector(0 to 1);
      PLB_MRdBTerm : out std_logic_vector(0 to 1);
      PLB_MRdDAck : out std_logic_vector(0 to 1);
      PLB_MRdDBus : out std_logic_vector(0 to 127);
      PLB_MRdWdAddr : out std_logic_vector(0 to 7);
      PLB_MRearbitrate : out std_logic_vector(0 to 1);
      PLB_MWrBTerm : out std_logic_vector(0 to 1);
      PLB_MWrDAck : out std_logic_vector(0 to 1);
      PLB_MSSize : out std_logic_vector(0 to 3);
      PLB_PAValid : out std_logic;
      PLB_RNW : out std_logic;
      PLB_SAValid : out std_logic;
      PLB_abort : out std_logic;
      PLB_busLock : out std_logic;
      PLB_TAttribute : out std_logic_vector(0 to 15);
      PLB_lockErr : out std_logic;
      PLB_masterID : out std_logic_vector(0 to 0);
      PLB_MSize : out std_logic_vector(0 to 1);
      PLB_rdPendPri : out std_logic_vector(0 to 1);
      PLB_wrPendPri : out std_logic_vector(0 to 1);
      PLB_rdPendReq : out std_logic;
      PLB_wrPendReq : out std_logic;
      PLB_rdBurst : out std_logic;
      PLB_rdPrim : out std_logic_vector(0 to 10);
      PLB_reqPri : out std_logic_vector(0 to 1);
      PLB_size : out std_logic_vector(0 to 3);
      PLB_type : out std_logic_vector(0 to 2);
      PLB_wrBurst : out std_logic;
      PLB_wrDBus : out std_logic_vector(0 to 63);
      PLB_wrPrim : out std_logic_vector(0 to 10);
      PLB_SaddrAck : out std_logic;
      PLB_SMRdErr : out std_logic_vector(0 to 1);
      PLB_SMWrErr : out std_logic_vector(0 to 1);
      PLB_SMBusy : out std_logic_vector(0 to 1);
      PLB_SrdBTerm : out std_logic;
      PLB_SrdComp : out std_logic;
      PLB_SrdDAck : out std_logic;
      PLB_SrdDBus : out std_logic_vector(0 to 63);
      PLB_SrdWdAddr : out std_logic_vector(0 to 3);
      PLB_Srearbitrate : out std_logic;
      PLB_Sssize : out std_logic_vector(0 to 1);
      PLB_Swait : out std_logic;
      PLB_SwrBTerm : out std_logic;
      PLB_SwrComp : out std_logic;
      PLB_SwrDAck : out std_logic;
      Bus_Error_Det : out std_logic
    );
  end component;

  component plb_1_wrapper is
    port (
      PLB_Clk : in std_logic;
      SYS_Rst : in std_logic;
      PLB_Rst : out std_logic;
      SPLB_Rst : out std_logic_vector(0 to 6);
      MPLB_Rst : out std_logic_vector(0 to 3);
      PLB_dcrAck : out std_logic;
      PLB_dcrDBus : out std_logic_vector(0 to 31);
      DCR_ABus : in std_logic_vector(0 to 9);
      DCR_DBus : in std_logic_vector(0 to 31);
      DCR_Read : in std_logic;
      DCR_Write : in std_logic;
      M_ABus : in std_logic_vector(0 to 127);
      M_UABus : in std_logic_vector(0 to 127);
      M_BE : in std_logic_vector(0 to 31);
      M_RNW : in std_logic_vector(0 to 3);
      M_abort : in std_logic_vector(0 to 3);
      M_busLock : in std_logic_vector(0 to 3);
      M_TAttribute : in std_logic_vector(0 to 63);
      M_lockErr : in std_logic_vector(0 to 3);
      M_MSize : in std_logic_vector(0 to 7);
      M_priority : in std_logic_vector(0 to 7);
      M_rdBurst : in std_logic_vector(0 to 3);
      M_request : in std_logic_vector(0 to 3);
      M_size : in std_logic_vector(0 to 15);
      M_type : in std_logic_vector(0 to 11);
      M_wrBurst : in std_logic_vector(0 to 3);
      M_wrDBus : in std_logic_vector(0 to 255);
      Sl_addrAck : in std_logic_vector(0 to 6);
      Sl_MRdErr : in std_logic_vector(0 to 27);
      Sl_MWrErr : in std_logic_vector(0 to 27);
      Sl_MBusy : in std_logic_vector(0 to 27);
      Sl_rdBTerm : in std_logic_vector(0 to 6);
      Sl_rdComp : in std_logic_vector(0 to 6);
      Sl_rdDAck : in std_logic_vector(0 to 6);
      Sl_rdDBus : in std_logic_vector(0 to 447);
      Sl_rdWdAddr : in std_logic_vector(0 to 27);
      Sl_rearbitrate : in std_logic_vector(0 to 6);
      Sl_SSize : in std_logic_vector(0 to 13);
      Sl_wait : in std_logic_vector(0 to 6);
      Sl_wrBTerm : in std_logic_vector(0 to 6);
      Sl_wrComp : in std_logic_vector(0 to 6);
      Sl_wrDAck : in std_logic_vector(0 to 6);
      Sl_MIRQ : in std_logic_vector(0 to 27);
      PLB_MIRQ : out std_logic_vector(0 to 3);
      PLB_ABus : out std_logic_vector(0 to 31);
      PLB_UABus : out std_logic_vector(0 to 31);
      PLB_BE : out std_logic_vector(0 to 7);
      PLB_MAddrAck : out std_logic_vector(0 to 3);
      PLB_MTimeout : out std_logic_vector(0 to 3);
      PLB_MBusy : out std_logic_vector(0 to 3);
      PLB_MRdErr : out std_logic_vector(0 to 3);
      PLB_MWrErr : out std_logic_vector(0 to 3);
      PLB_MRdBTerm : out std_logic_vector(0 to 3);
      PLB_MRdDAck : out std_logic_vector(0 to 3);
      PLB_MRdDBus : out std_logic_vector(0 to 255);
      PLB_MRdWdAddr : out std_logic_vector(0 to 15);
      PLB_MRearbitrate : out std_logic_vector(0 to 3);
      PLB_MWrBTerm : out std_logic_vector(0 to 3);
      PLB_MWrDAck : out std_logic_vector(0 to 3);
      PLB_MSSize : out std_logic_vector(0 to 7);
      PLB_PAValid : out std_logic;
      PLB_RNW : out std_logic;
      PLB_SAValid : out std_logic;
      PLB_abort : out std_logic;
      PLB_busLock : out std_logic;
      PLB_TAttribute : out std_logic_vector(0 to 15);
      PLB_lockErr : out std_logic;
      PLB_masterID : out std_logic_vector(0 to 1);
      PLB_MSize : out std_logic_vector(0 to 1);
      PLB_rdPendPri : out std_logic_vector(0 to 1);
      PLB_wrPendPri : out std_logic_vector(0 to 1);
      PLB_rdPendReq : out std_logic;
      PLB_wrPendReq : out std_logic;
      PLB_rdBurst : out std_logic;
      PLB_rdPrim : out std_logic_vector(0 to 6);
      PLB_reqPri : out std_logic_vector(0 to 1);
      PLB_size : out std_logic_vector(0 to 3);
      PLB_type : out std_logic_vector(0 to 2);
      PLB_wrBurst : out std_logic;
      PLB_wrDBus : out std_logic_vector(0 to 63);
      PLB_wrPrim : out std_logic_vector(0 to 6);
      PLB_SaddrAck : out std_logic;
      PLB_SMRdErr : out std_logic_vector(0 to 3);
      PLB_SMWrErr : out std_logic_vector(0 to 3);
      PLB_SMBusy : out std_logic_vector(0 to 3);
      PLB_SrdBTerm : out std_logic;
      PLB_SrdComp : out std_logic;
      PLB_SrdDAck : out std_logic;
      PLB_SrdDBus : out std_logic_vector(0 to 63);
      PLB_SrdWdAddr : out std_logic_vector(0 to 3);
      PLB_Srearbitrate : out std_logic;
      PLB_Sssize : out std_logic_vector(0 to 1);
      PLB_Swait : out std_logic;
      PLB_SwrBTerm : out std_logic;
      PLB_SwrComp : out std_logic;
      PLB_SwrDAck : out std_logic;
      Bus_Error_Det : out std_logic
    );
  end component;

  component xps_bram_if_cntlr_0_wrapper is
    port (
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 0);
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_lockErr : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrBTerm : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 1);
      Sl_MWrErr : out std_logic_vector(0 to 1);
      Sl_MRdErr : out std_logic_vector(0 to 1);
      Sl_MIRQ : out std_logic_vector(0 to 1);
      BRAM_Rst : out std_logic;
      BRAM_Clk : out std_logic;
      BRAM_EN : out std_logic;
      BRAM_WEN : out std_logic_vector(0 to 7);
      BRAM_Addr : out std_logic_vector(0 to 31);
      BRAM_Din : in std_logic_vector(0 to 63);
      BRAM_Dout : out std_logic_vector(0 to 63)
    );
  end component;

  component xps_bram_if_cntlr_1_wrapper is
    port (
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 1);
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_lockErr : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrBTerm : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 3);
      Sl_MWrErr : out std_logic_vector(0 to 3);
      Sl_MRdErr : out std_logic_vector(0 to 3);
      Sl_MIRQ : out std_logic_vector(0 to 3);
      BRAM_Rst : out std_logic;
      BRAM_Clk : out std_logic;
      BRAM_EN : out std_logic;
      BRAM_WEN : out std_logic_vector(0 to 7);
      BRAM_Addr : out std_logic_vector(0 to 31);
      BRAM_Din : in std_logic_vector(0 to 63);
      BRAM_Dout : out std_logic_vector(0 to 63)
    );
  end component;

  component plb_bram_if_cntlr_0_bram_wrapper is
    port (
      BRAM_Rst_A : in std_logic;
      BRAM_Clk_A : in std_logic;
      BRAM_EN_A : in std_logic;
      BRAM_WEN_A : in std_logic_vector(0 to 7);
      BRAM_Addr_A : in std_logic_vector(0 to 31);
      BRAM_Din_A : out std_logic_vector(0 to 63);
      BRAM_Dout_A : in std_logic_vector(0 to 63);
      BRAM_Rst_B : in std_logic;
      BRAM_Clk_B : in std_logic;
      BRAM_EN_B : in std_logic;
      BRAM_WEN_B : in std_logic_vector(0 to 7);
      BRAM_Addr_B : in std_logic_vector(0 to 31);
      BRAM_Din_B : out std_logic_vector(0 to 63);
      BRAM_Dout_B : in std_logic_vector(0 to 63)
    );
  end component;

  component plb_bram_if_cntlr_1_bram_wrapper is
    port (
      BRAM_Rst_A : in std_logic;
      BRAM_Clk_A : in std_logic;
      BRAM_EN_A : in std_logic;
      BRAM_WEN_A : in std_logic_vector(0 to 7);
      BRAM_Addr_A : in std_logic_vector(0 to 31);
      BRAM_Din_A : out std_logic_vector(0 to 63);
      BRAM_Dout_A : in std_logic_vector(0 to 63);
      BRAM_Rst_B : in std_logic;
      BRAM_Clk_B : in std_logic;
      BRAM_EN_B : in std_logic;
      BRAM_WEN_B : in std_logic_vector(0 to 7);
      BRAM_Addr_B : in std_logic_vector(0 to 31);
      BRAM_Din_B : out std_logic_vector(0 to 63);
      BRAM_Dout_B : in std_logic_vector(0 to 63)
    );
  end component;

  component serial_uart_wrapper is
    port (
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 0);
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_lockErr : in std_logic;
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 1);
      Sl_MWrErr : out std_logic_vector(0 to 1);
      Sl_MRdErr : out std_logic_vector(0 to 1);
      Sl_wrBTerm : out std_logic;
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdBTerm : out std_logic;
      Sl_MIRQ : out std_logic_vector(0 to 1);
      RX : in std_logic;
      TX : out std_logic;
      Interrupt : out std_logic
    );
  end component;

  component wireless_uart_wrapper is
    port (
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 0);
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_lockErr : in std_logic;
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 1);
      Sl_MWrErr : out std_logic_vector(0 to 1);
      Sl_MRdErr : out std_logic_vector(0 to 1);
      Sl_wrBTerm : out std_logic;
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdBTerm : out std_logic;
      Sl_MIRQ : out std_logic_vector(0 to 1);
      RX : in std_logic;
      TX : out std_logic;
      Interrupt : out std_logic
    );
  end component;

  component clock_generator_0_wrapper is
    port (
      CLKIN : in std_logic;
      CLKOUT0 : out std_logic;
      CLKOUT1 : out std_logic;
      CLKOUT2 : out std_logic;
      CLKOUT3 : out std_logic;
      CLKOUT4 : out std_logic;
      CLKOUT5 : out std_logic;
      CLKOUT6 : out std_logic;
      CLKOUT7 : out std_logic;
      CLKOUT8 : out std_logic;
      CLKOUT9 : out std_logic;
      CLKOUT10 : out std_logic;
      CLKOUT11 : out std_logic;
      CLKOUT12 : out std_logic;
      CLKOUT13 : out std_logic;
      CLKOUT14 : out std_logic;
      CLKOUT15 : out std_logic;
      CLKFBIN : in std_logic;
      CLKFBOUT : out std_logic;
      PSCLK : in std_logic;
      PSEN : in std_logic;
      PSINCDEC : in std_logic;
      PSDONE : out std_logic;
      RST : in std_logic;
      LOCKED : out std_logic
    );
  end component;

  component jtagppc_cntlr_inst_wrapper is
    port (
      TRSTNEG : in std_logic;
      HALTNEG0 : in std_logic;
      DBGC405DEBUGHALT0 : out std_logic;
      HALTNEG1 : in std_logic;
      DBGC405DEBUGHALT1 : out std_logic;
      C405JTGTDO0 : in std_logic;
      C405JTGTDOEN0 : in std_logic;
      JTGC405TCK0 : out std_logic;
      JTGC405TDI0 : out std_logic;
      JTGC405TMS0 : out std_logic;
      JTGC405TRSTNEG0 : out std_logic;
      C405JTGTDO1 : in std_logic;
      C405JTGTDOEN1 : in std_logic;
      JTGC405TCK1 : out std_logic;
      JTGC405TDI1 : out std_logic;
      JTGC405TMS1 : out std_logic;
      JTGC405TRSTNEG1 : out std_logic
    );
  end component;

  component proc_sys_reset_0_wrapper is
    port (
      Slowest_sync_clk : in std_logic;
      Ext_Reset_In : in std_logic;
      Aux_Reset_In : in std_logic;
      MB_Debug_Sys_Rst : in std_logic;
      Core_Reset_Req_0 : in std_logic;
      Chip_Reset_Req_0 : in std_logic;
      System_Reset_Req_0 : in std_logic;
      Core_Reset_Req_1 : in std_logic;
      Chip_Reset_Req_1 : in std_logic;
      System_Reset_Req_1 : in std_logic;
      Dcm_locked : in std_logic;
      RstcPPCresetcore_0 : out std_logic;
      RstcPPCresetchip_0 : out std_logic;
      RstcPPCresetsys_0 : out std_logic;
      RstcPPCresetcore_1 : out std_logic;
      RstcPPCresetchip_1 : out std_logic;
      RstcPPCresetsys_1 : out std_logic;
      MB_Reset : out std_logic;
      Bus_Struct_Reset : out std_logic_vector(0 to 0);
      Peripheral_Reset : out std_logic_vector(0 to 0);
      Interconnect_aresetn : out std_logic_vector(0 to 0);
      Peripheral_aresetn : out std_logic_vector(0 to 0)
    );
  end component;

  component xps_intc_0_wrapper is
    port (
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 0);
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_lockErr : in std_logic;
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 1);
      Sl_MWrErr : out std_logic_vector(0 to 1);
      Sl_MRdErr : out std_logic_vector(0 to 1);
      Sl_wrBTerm : out std_logic;
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdBTerm : out std_logic;
      Sl_MIRQ : out std_logic_vector(0 to 1);
      Intr : in std_logic_vector(3 downto 0);
      Irq : out std_logic
    );
  end component;

  component plb_quad_encoder_0_wrapper is
    port (
      enc_a_in : in std_logic;
      enc_b_in : in std_logic;
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 0);
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_lockErr : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrBTerm : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 1);
      Sl_MWrErr : out std_logic_vector(0 to 1);
      Sl_MRdErr : out std_logic_vector(0 to 1);
      Sl_MIRQ : out std_logic_vector(0 to 1)
    );
  end component;

  component plb_pwm_ctrl_0_wrapper is
    port (
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 0);
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_lockErr : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrBTerm : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 1);
      Sl_MWrErr : out std_logic_vector(0 to 1);
      Sl_MRdErr : out std_logic_vector(0 to 1);
      Sl_MIRQ : out std_logic_vector(0 to 1);
      pwm : out std_logic_vector(0 to 1)
    );
  end component;

  component xps_mch_emc_flash_wrapper is
    port (
      MCH_SPLB_Clk : in std_logic;
      RdClk : in std_logic;
      MCH_SPLB_Rst : in std_logic;
      MCH0_Access_Control : in std_logic;
      MCH0_Access_Data : in std_logic_vector(0 to 31);
      MCH0_Access_Write : in std_logic;
      MCH0_Access_Full : out std_logic;
      MCH0_ReadData_Control : out std_logic;
      MCH0_ReadData_Data : out std_logic_vector(0 to 31);
      MCH0_ReadData_Read : in std_logic;
      MCH0_ReadData_Exists : out std_logic;
      MCH1_Access_Control : in std_logic;
      MCH1_Access_Data : in std_logic_vector(0 to 31);
      MCH1_Access_Write : in std_logic;
      MCH1_Access_Full : out std_logic;
      MCH1_ReadData_Control : out std_logic;
      MCH1_ReadData_Data : out std_logic_vector(0 to 31);
      MCH1_ReadData_Read : in std_logic;
      MCH1_ReadData_Exists : out std_logic;
      MCH2_Access_Control : in std_logic;
      MCH2_Access_Data : in std_logic_vector(0 to 31);
      MCH2_Access_Write : in std_logic;
      MCH2_Access_Full : out std_logic;
      MCH2_ReadData_Control : out std_logic;
      MCH2_ReadData_Data : out std_logic_vector(0 to 31);
      MCH2_ReadData_Read : in std_logic;
      MCH2_ReadData_Exists : out std_logic;
      MCH3_Access_Control : in std_logic;
      MCH3_Access_Data : in std_logic_vector(0 to 31);
      MCH3_Access_Write : in std_logic;
      MCH3_Access_Full : out std_logic;
      MCH3_ReadData_Control : out std_logic;
      MCH3_ReadData_Data : out std_logic_vector(0 to 31);
      MCH3_ReadData_Read : in std_logic;
      MCH3_ReadData_Exists : out std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 0);
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_lockErr : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrBTerm : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 1);
      Sl_MWrErr : out std_logic_vector(0 to 1);
      Sl_MRdErr : out std_logic_vector(0 to 1);
      Sl_MIRQ : out std_logic_vector(0 to 1);
      Mem_DQ_I : in std_logic_vector(0 to 15);
      Mem_DQ_O : out std_logic_vector(0 to 15);
      Mem_DQ_T : out std_logic_vector(0 to 15);
      Mem_A : out std_logic_vector(0 to 31);
      Mem_RPN : out std_logic;
      Mem_CEN : out std_logic_vector(0 to 0);
      Mem_OEN : out std_logic_vector(0 to 0);
      Mem_WEN : out std_logic;
      Mem_QWEN : out std_logic_vector(0 to 1);
      Mem_BEN : out std_logic_vector(0 to 1);
      Mem_CE : out std_logic_vector(0 to 0);
      Mem_ADV_LDN : out std_logic;
      Mem_LBON : out std_logic;
      Mem_CKEN : out std_logic;
      Mem_RNW : out std_logic
    );
  end component;

  component xps_mch_emc_sram_wrapper is
    port (
      MCH_SPLB_Clk : in std_logic;
      RdClk : in std_logic;
      MCH_SPLB_Rst : in std_logic;
      MCH0_Access_Control : in std_logic;
      MCH0_Access_Data : in std_logic_vector(0 to 31);
      MCH0_Access_Write : in std_logic;
      MCH0_Access_Full : out std_logic;
      MCH0_ReadData_Control : out std_logic;
      MCH0_ReadData_Data : out std_logic_vector(0 to 31);
      MCH0_ReadData_Read : in std_logic;
      MCH0_ReadData_Exists : out std_logic;
      MCH1_Access_Control : in std_logic;
      MCH1_Access_Data : in std_logic_vector(0 to 31);
      MCH1_Access_Write : in std_logic;
      MCH1_Access_Full : out std_logic;
      MCH1_ReadData_Control : out std_logic;
      MCH1_ReadData_Data : out std_logic_vector(0 to 31);
      MCH1_ReadData_Read : in std_logic;
      MCH1_ReadData_Exists : out std_logic;
      MCH2_Access_Control : in std_logic;
      MCH2_Access_Data : in std_logic_vector(0 to 31);
      MCH2_Access_Write : in std_logic;
      MCH2_Access_Full : out std_logic;
      MCH2_ReadData_Control : out std_logic;
      MCH2_ReadData_Data : out std_logic_vector(0 to 31);
      MCH2_ReadData_Read : in std_logic;
      MCH2_ReadData_Exists : out std_logic;
      MCH3_Access_Control : in std_logic;
      MCH3_Access_Data : in std_logic_vector(0 to 31);
      MCH3_Access_Write : in std_logic;
      MCH3_Access_Full : out std_logic;
      MCH3_ReadData_Control : out std_logic;
      MCH3_ReadData_Data : out std_logic_vector(0 to 31);
      MCH3_ReadData_Read : in std_logic;
      MCH3_ReadData_Exists : out std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 1);
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_lockErr : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrBTerm : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 3);
      Sl_MWrErr : out std_logic_vector(0 to 3);
      Sl_MRdErr : out std_logic_vector(0 to 3);
      Sl_MIRQ : out std_logic_vector(0 to 3);
      Mem_DQ_I : in std_logic_vector(0 to 31);
      Mem_DQ_O : out std_logic_vector(0 to 31);
      Mem_DQ_T : out std_logic_vector(0 to 31);
      Mem_A : out std_logic_vector(0 to 31);
      Mem_RPN : out std_logic;
      Mem_CEN : out std_logic_vector(0 to 0);
      Mem_OEN : out std_logic_vector(0 to 0);
      Mem_WEN : out std_logic;
      Mem_QWEN : out std_logic_vector(0 to 3);
      Mem_BEN : out std_logic_vector(0 to 3);
      Mem_CE : out std_logic_vector(0 to 0);
      Mem_ADV_LDN : out std_logic;
      Mem_LBON : out std_logic;
      Mem_CKEN : out std_logic;
      Mem_RNW : out std_logic
    );
  end component;

  component dcm_module_0_wrapper is
    port (
      RST : in std_logic;
      CLKIN : in std_logic;
      CLKFB : in std_logic;
      PSEN : in std_logic;
      PSINCDEC : in std_logic;
      PSCLK : in std_logic;
      DSSEN : in std_logic;
      CLK0 : out std_logic;
      CLK90 : out std_logic;
      CLK180 : out std_logic;
      CLK270 : out std_logic;
      CLKDV : out std_logic;
      CLK2X : out std_logic;
      CLK2X180 : out std_logic;
      CLKFX : out std_logic;
      CLKFX180 : out std_logic;
      STATUS : out std_logic_vector(7 downto 0);
      LOCKED : out std_logic;
      PSDONE : out std_logic
    );
  end component;

  component plb_usb_0_wrapper is
    port (
      dcm_locked : in std_logic;
      dcm_reset : out std_logic;
      if_clk : in std_logic;
      usb_full_n : in std_logic;
      usb_empty_n : in std_logic;
      usb_alive : in std_logic;
      Interrupt : out std_logic;
      sloe_n : out std_logic;
      slrd_n : out std_logic;
      slwr_n : out std_logic;
      pktend_n : out std_logic;
      fifoaddr : out std_logic_vector(1 downto 0);
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 1);
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_lockErr : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrBTerm : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 3);
      Sl_MWrErr : out std_logic_vector(0 to 3);
      Sl_MRdErr : out std_logic_vector(0 to 3);
      Sl_MIRQ : out std_logic_vector(0 to 3);
      MPLB_Clk : in std_logic;
      MPLB_Rst : in std_logic;
      MD_error : out std_logic;
      M_request : out std_logic;
      M_priority : out std_logic_vector(0 to 1);
      M_busLock : out std_logic;
      M_RNW : out std_logic;
      M_BE : out std_logic_vector(0 to 7);
      M_MSize : out std_logic_vector(0 to 1);
      M_size : out std_logic_vector(0 to 3);
      M_type : out std_logic_vector(0 to 2);
      M_TAttribute : out std_logic_vector(0 to 15);
      M_lockErr : out std_logic;
      M_abort : out std_logic;
      M_UABus : out std_logic_vector(0 to 31);
      M_ABus : out std_logic_vector(0 to 31);
      M_wrDBus : out std_logic_vector(0 to 63);
      M_wrBurst : out std_logic;
      M_rdBurst : out std_logic;
      PLB_MAddrAck : in std_logic;
      PLB_MSSize : in std_logic_vector(0 to 1);
      PLB_MRearbitrate : in std_logic;
      PLB_MTimeout : in std_logic;
      PLB_MBusy : in std_logic;
      PLB_MRdErr : in std_logic;
      PLB_MWrErr : in std_logic;
      PLB_MIRQ : in std_logic;
      PLB_MRdDBus : in std_logic_vector(0 to 63);
      PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      PLB_MRdDAck : in std_logic;
      PLB_MRdBTerm : in std_logic;
      PLB_MWrDAck : in std_logic;
      PLB_MWrBTerm : in std_logic;
      fd_I : in std_logic_vector(15 downto 0);
      fd_O : out std_logic_vector(15 downto 0);
      fd_T : out std_logic_vector(15 downto 0)
    );
  end component;

  component plb_vision_0_wrapper is
    port (
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 1);
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_lockErr : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrBTerm : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 3);
      Sl_MWrErr : out std_logic_vector(0 to 3);
      Sl_MRdErr : out std_logic_vector(0 to 3);
      Sl_MIRQ : out std_logic_vector(0 to 3);
      MPLB_Clk : in std_logic;
      MPLB_Rst : in std_logic;
      MD_error : out std_logic;
      M_request : out std_logic;
      M_priority : out std_logic_vector(0 to 1);
      M_busLock : out std_logic;
      M_RNW : out std_logic;
      M_BE : out std_logic_vector(0 to 7);
      M_MSize : out std_logic_vector(0 to 1);
      M_size : out std_logic_vector(0 to 3);
      M_type : out std_logic_vector(0 to 2);
      M_TAttribute : out std_logic_vector(0 to 15);
      M_lockErr : out std_logic;
      M_abort : out std_logic;
      M_UABus : out std_logic_vector(0 to 31);
      M_ABus : out std_logic_vector(0 to 31);
      M_wrDBus : out std_logic_vector(0 to 63);
      M_wrBurst : out std_logic;
      M_rdBurst : out std_logic;
      PLB_MAddrAck : in std_logic;
      PLB_MSSize : in std_logic_vector(0 to 1);
      PLB_MRearbitrate : in std_logic;
      PLB_MTimeout : in std_logic;
      PLB_MBusy : in std_logic;
      PLB_MRdErr : in std_logic;
      PLB_MWrErr : in std_logic;
      PLB_MIRQ : in std_logic;
      PLB_MRdDBus : in std_logic_vector(0 to 63);
      PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      PLB_MRdDAck : in std_logic;
      PLB_MRdBTerm : in std_logic;
      PLB_MWrDAck : in std_logic;
      PLB_MWrBTerm : in std_logic;
      cam_data : in std_logic_vector(0 to 7);
      cam_frame_valid : in std_logic;
      cam_line_valid : in std_logic;
      cam_pix_clk : in std_logic;
      cam_sclk : out std_logic;
      cam_reset_n : out std_logic;
      Interrupt : out std_logic;
      cam_sdata_I : in std_logic;
      cam_sdata_O : out std_logic;
      cam_sdata_T : out std_logic
    );
  end component;

  component mpmc_0_wrapper is
    port (
      FSL0_M_Clk : in std_logic;
      FSL0_M_Write : in std_logic;
      FSL0_M_Data : in std_logic_vector(0 to 31);
      FSL0_M_Control : in std_logic;
      FSL0_M_Full : out std_logic;
      FSL0_S_Clk : in std_logic;
      FSL0_S_Read : in std_logic;
      FSL0_S_Data : out std_logic_vector(0 to 31);
      FSL0_S_Control : out std_logic;
      FSL0_S_Exists : out std_logic;
      FSL0_B_M_Clk : in std_logic;
      FSL0_B_M_Write : in std_logic;
      FSL0_B_M_Data : in std_logic_vector(0 to 31);
      FSL0_B_M_Control : in std_logic;
      FSL0_B_M_Full : out std_logic;
      FSL0_B_S_Clk : in std_logic;
      FSL0_B_S_Read : in std_logic;
      FSL0_B_S_Data : out std_logic_vector(0 to 31);
      FSL0_B_S_Control : out std_logic;
      FSL0_B_S_Exists : out std_logic;
      SPLB0_Clk : in std_logic;
      SPLB0_Rst : in std_logic;
      SPLB0_PLB_ABus : in std_logic_vector(0 to 31);
      SPLB0_PLB_PAValid : in std_logic;
      SPLB0_PLB_SAValid : in std_logic;
      SPLB0_PLB_masterID : in std_logic_vector(0 to 0);
      SPLB0_PLB_RNW : in std_logic;
      SPLB0_PLB_BE : in std_logic_vector(0 to 7);
      SPLB0_PLB_UABus : in std_logic_vector(0 to 31);
      SPLB0_PLB_rdPrim : in std_logic;
      SPLB0_PLB_wrPrim : in std_logic;
      SPLB0_PLB_abort : in std_logic;
      SPLB0_PLB_busLock : in std_logic;
      SPLB0_PLB_MSize : in std_logic_vector(0 to 1);
      SPLB0_PLB_size : in std_logic_vector(0 to 3);
      SPLB0_PLB_type : in std_logic_vector(0 to 2);
      SPLB0_PLB_lockErr : in std_logic;
      SPLB0_PLB_wrPendReq : in std_logic;
      SPLB0_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SPLB0_PLB_rdPendReq : in std_logic;
      SPLB0_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SPLB0_PLB_reqPri : in std_logic_vector(0 to 1);
      SPLB0_PLB_TAttribute : in std_logic_vector(0 to 15);
      SPLB0_PLB_rdBurst : in std_logic;
      SPLB0_PLB_wrBurst : in std_logic;
      SPLB0_PLB_wrDBus : in std_logic_vector(0 to 63);
      SPLB0_Sl_addrAck : out std_logic;
      SPLB0_Sl_SSize : out std_logic_vector(0 to 1);
      SPLB0_Sl_wait : out std_logic;
      SPLB0_Sl_rearbitrate : out std_logic;
      SPLB0_Sl_wrDAck : out std_logic;
      SPLB0_Sl_wrComp : out std_logic;
      SPLB0_Sl_wrBTerm : out std_logic;
      SPLB0_Sl_rdDBus : out std_logic_vector(0 to 63);
      SPLB0_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SPLB0_Sl_rdDAck : out std_logic;
      SPLB0_Sl_rdComp : out std_logic;
      SPLB0_Sl_rdBTerm : out std_logic;
      SPLB0_Sl_MBusy : out std_logic_vector(0 to 1);
      SPLB0_Sl_MRdErr : out std_logic_vector(0 to 1);
      SPLB0_Sl_MWrErr : out std_logic_vector(0 to 1);
      SPLB0_Sl_MIRQ : out std_logic_vector(0 to 1);
      SDMA0_Clk : in std_logic;
      SDMA0_Rx_IntOut : out std_logic;
      SDMA0_Tx_IntOut : out std_logic;
      SDMA0_RstOut : out std_logic;
      SDMA0_TX_D : out std_logic_vector(0 to 31);
      SDMA0_TX_Rem : out std_logic_vector(0 to 3);
      SDMA0_TX_SOF : out std_logic;
      SDMA0_TX_EOF : out std_logic;
      SDMA0_TX_SOP : out std_logic;
      SDMA0_TX_EOP : out std_logic;
      SDMA0_TX_Src_Rdy : out std_logic;
      SDMA0_TX_Dst_Rdy : in std_logic;
      SDMA0_RX_D : in std_logic_vector(0 to 31);
      SDMA0_RX_Rem : in std_logic_vector(0 to 3);
      SDMA0_RX_SOF : in std_logic;
      SDMA0_RX_EOF : in std_logic;
      SDMA0_RX_SOP : in std_logic;
      SDMA0_RX_EOP : in std_logic;
      SDMA0_RX_Src_Rdy : in std_logic;
      SDMA0_RX_Dst_Rdy : out std_logic;
      SDMA_CTRL0_Clk : in std_logic;
      SDMA_CTRL0_Rst : in std_logic;
      SDMA_CTRL0_PLB_ABus : in std_logic_vector(0 to 31);
      SDMA_CTRL0_PLB_PAValid : in std_logic;
      SDMA_CTRL0_PLB_SAValid : in std_logic;
      SDMA_CTRL0_PLB_masterID : in std_logic_vector(0 to 0);
      SDMA_CTRL0_PLB_RNW : in std_logic;
      SDMA_CTRL0_PLB_BE : in std_logic_vector(0 to 7);
      SDMA_CTRL0_PLB_UABus : in std_logic_vector(0 to 31);
      SDMA_CTRL0_PLB_rdPrim : in std_logic;
      SDMA_CTRL0_PLB_wrPrim : in std_logic;
      SDMA_CTRL0_PLB_abort : in std_logic;
      SDMA_CTRL0_PLB_busLock : in std_logic;
      SDMA_CTRL0_PLB_MSize : in std_logic_vector(0 to 1);
      SDMA_CTRL0_PLB_size : in std_logic_vector(0 to 3);
      SDMA_CTRL0_PLB_type : in std_logic_vector(0 to 2);
      SDMA_CTRL0_PLB_lockErr : in std_logic;
      SDMA_CTRL0_PLB_wrPendReq : in std_logic;
      SDMA_CTRL0_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL0_PLB_rdPendReq : in std_logic;
      SDMA_CTRL0_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL0_PLB_reqPri : in std_logic_vector(0 to 1);
      SDMA_CTRL0_PLB_TAttribute : in std_logic_vector(0 to 15);
      SDMA_CTRL0_PLB_rdBurst : in std_logic;
      SDMA_CTRL0_PLB_wrBurst : in std_logic;
      SDMA_CTRL0_PLB_wrDBus : in std_logic_vector(0 to 63);
      SDMA_CTRL0_Sl_addrAck : out std_logic;
      SDMA_CTRL0_Sl_SSize : out std_logic_vector(0 to 1);
      SDMA_CTRL0_Sl_wait : out std_logic;
      SDMA_CTRL0_Sl_rearbitrate : out std_logic;
      SDMA_CTRL0_Sl_wrDAck : out std_logic;
      SDMA_CTRL0_Sl_wrComp : out std_logic;
      SDMA_CTRL0_Sl_wrBTerm : out std_logic;
      SDMA_CTRL0_Sl_rdDBus : out std_logic_vector(0 to 63);
      SDMA_CTRL0_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SDMA_CTRL0_Sl_rdDAck : out std_logic;
      SDMA_CTRL0_Sl_rdComp : out std_logic;
      SDMA_CTRL0_Sl_rdBTerm : out std_logic;
      SDMA_CTRL0_Sl_MBusy : out std_logic_vector(0 to 0);
      SDMA_CTRL0_Sl_MRdErr : out std_logic_vector(0 to 0);
      SDMA_CTRL0_Sl_MWrErr : out std_logic_vector(0 to 0);
      SDMA_CTRL0_Sl_MIRQ : out std_logic_vector(0 to 0);
      PIM0_Addr : in std_logic_vector(31 downto 0);
      PIM0_AddrReq : in std_logic;
      PIM0_AddrAck : out std_logic;
      PIM0_RNW : in std_logic;
      PIM0_Size : in std_logic_vector(3 downto 0);
      PIM0_RdModWr : in std_logic;
      PIM0_WrFIFO_Data : in std_logic_vector(63 downto 0);
      PIM0_WrFIFO_BE : in std_logic_vector(7 downto 0);
      PIM0_WrFIFO_Push : in std_logic;
      PIM0_RdFIFO_Data : out std_logic_vector(63 downto 0);
      PIM0_RdFIFO_Pop : in std_logic;
      PIM0_RdFIFO_RdWdAddr : out std_logic_vector(3 downto 0);
      PIM0_WrFIFO_Empty : out std_logic;
      PIM0_WrFIFO_AlmostFull : out std_logic;
      PIM0_WrFIFO_Flush : in std_logic;
      PIM0_RdFIFO_Empty : out std_logic;
      PIM0_RdFIFO_Flush : in std_logic;
      PIM0_RdFIFO_Latency : out std_logic_vector(1 downto 0);
      PIM0_InitDone : out std_logic;
      PPC440MC0_MIMCReadNotWrite : in std_logic;
      PPC440MC0_MIMCAddress : in std_logic_vector(0 to 35);
      PPC440MC0_MIMCAddressValid : in std_logic;
      PPC440MC0_MIMCWriteData : in std_logic_vector(0 to 127);
      PPC440MC0_MIMCWriteDataValid : in std_logic;
      PPC440MC0_MIMCByteEnable : in std_logic_vector(0 to 15);
      PPC440MC0_MIMCBankConflict : in std_logic;
      PPC440MC0_MIMCRowConflict : in std_logic;
      PPC440MC0_MCMIReadData : out std_logic_vector(0 to 127);
      PPC440MC0_MCMIReadDataValid : out std_logic;
      PPC440MC0_MCMIReadDataErr : out std_logic;
      PPC440MC0_MCMIAddrReadyToAccept : out std_logic;
      VFBC0_Cmd_Clk : in std_logic;
      VFBC0_Cmd_Reset : in std_logic;
      VFBC0_Cmd_Data : in std_logic_vector(31 downto 0);
      VFBC0_Cmd_Write : in std_logic;
      VFBC0_Cmd_End : in std_logic;
      VFBC0_Cmd_Full : out std_logic;
      VFBC0_Cmd_Almost_Full : out std_logic;
      VFBC0_Cmd_Idle : out std_logic;
      VFBC0_Wd_Clk : in std_logic;
      VFBC0_Wd_Reset : in std_logic;
      VFBC0_Wd_Write : in std_logic;
      VFBC0_Wd_End_Burst : in std_logic;
      VFBC0_Wd_Flush : in std_logic;
      VFBC0_Wd_Data : in std_logic_vector(31 downto 0);
      VFBC0_Wd_Data_BE : in std_logic_vector(3 downto 0);
      VFBC0_Wd_Full : out std_logic;
      VFBC0_Wd_Almost_Full : out std_logic;
      VFBC0_Rd_Clk : in std_logic;
      VFBC0_Rd_Reset : in std_logic;
      VFBC0_Rd_Read : in std_logic;
      VFBC0_Rd_End_Burst : in std_logic;
      VFBC0_Rd_Flush : in std_logic;
      VFBC0_Rd_Data : out std_logic_vector(31 downto 0);
      VFBC0_Rd_Empty : out std_logic;
      VFBC0_Rd_Almost_Empty : out std_logic;
      MCB0_cmd_clk : in std_logic;
      MCB0_cmd_en : in std_logic;
      MCB0_cmd_instr : in std_logic_vector(2 downto 0);
      MCB0_cmd_bl : in std_logic_vector(5 downto 0);
      MCB0_cmd_byte_addr : in std_logic_vector(29 downto 0);
      MCB0_cmd_empty : out std_logic;
      MCB0_cmd_full : out std_logic;
      MCB0_wr_clk : in std_logic;
      MCB0_wr_en : in std_logic;
      MCB0_wr_mask : in std_logic_vector(7 downto 0);
      MCB0_wr_data : in std_logic_vector(63 downto 0);
      MCB0_wr_full : out std_logic;
      MCB0_wr_empty : out std_logic;
      MCB0_wr_count : out std_logic_vector(6 downto 0);
      MCB0_wr_underrun : out std_logic;
      MCB0_wr_error : out std_logic;
      MCB0_rd_clk : in std_logic;
      MCB0_rd_en : in std_logic;
      MCB0_rd_data : out std_logic_vector(63 downto 0);
      MCB0_rd_full : out std_logic;
      MCB0_rd_empty : out std_logic;
      MCB0_rd_count : out std_logic_vector(6 downto 0);
      MCB0_rd_overflow : out std_logic;
      MCB0_rd_error : out std_logic;
      FSL1_M_Clk : in std_logic;
      FSL1_M_Write : in std_logic;
      FSL1_M_Data : in std_logic_vector(0 to 31);
      FSL1_M_Control : in std_logic;
      FSL1_M_Full : out std_logic;
      FSL1_S_Clk : in std_logic;
      FSL1_S_Read : in std_logic;
      FSL1_S_Data : out std_logic_vector(0 to 31);
      FSL1_S_Control : out std_logic;
      FSL1_S_Exists : out std_logic;
      FSL1_B_M_Clk : in std_logic;
      FSL1_B_M_Write : in std_logic;
      FSL1_B_M_Data : in std_logic_vector(0 to 31);
      FSL1_B_M_Control : in std_logic;
      FSL1_B_M_Full : out std_logic;
      FSL1_B_S_Clk : in std_logic;
      FSL1_B_S_Read : in std_logic;
      FSL1_B_S_Data : out std_logic_vector(0 to 31);
      FSL1_B_S_Control : out std_logic;
      FSL1_B_S_Exists : out std_logic;
      SPLB1_Clk : in std_logic;
      SPLB1_Rst : in std_logic;
      SPLB1_PLB_ABus : in std_logic_vector(0 to 31);
      SPLB1_PLB_PAValid : in std_logic;
      SPLB1_PLB_SAValid : in std_logic;
      SPLB1_PLB_masterID : in std_logic_vector(0 to 1);
      SPLB1_PLB_RNW : in std_logic;
      SPLB1_PLB_BE : in std_logic_vector(0 to 7);
      SPLB1_PLB_UABus : in std_logic_vector(0 to 31);
      SPLB1_PLB_rdPrim : in std_logic;
      SPLB1_PLB_wrPrim : in std_logic;
      SPLB1_PLB_abort : in std_logic;
      SPLB1_PLB_busLock : in std_logic;
      SPLB1_PLB_MSize : in std_logic_vector(0 to 1);
      SPLB1_PLB_size : in std_logic_vector(0 to 3);
      SPLB1_PLB_type : in std_logic_vector(0 to 2);
      SPLB1_PLB_lockErr : in std_logic;
      SPLB1_PLB_wrPendReq : in std_logic;
      SPLB1_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SPLB1_PLB_rdPendReq : in std_logic;
      SPLB1_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SPLB1_PLB_reqPri : in std_logic_vector(0 to 1);
      SPLB1_PLB_TAttribute : in std_logic_vector(0 to 15);
      SPLB1_PLB_rdBurst : in std_logic;
      SPLB1_PLB_wrBurst : in std_logic;
      SPLB1_PLB_wrDBus : in std_logic_vector(0 to 63);
      SPLB1_Sl_addrAck : out std_logic;
      SPLB1_Sl_SSize : out std_logic_vector(0 to 1);
      SPLB1_Sl_wait : out std_logic;
      SPLB1_Sl_rearbitrate : out std_logic;
      SPLB1_Sl_wrDAck : out std_logic;
      SPLB1_Sl_wrComp : out std_logic;
      SPLB1_Sl_wrBTerm : out std_logic;
      SPLB1_Sl_rdDBus : out std_logic_vector(0 to 63);
      SPLB1_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SPLB1_Sl_rdDAck : out std_logic;
      SPLB1_Sl_rdComp : out std_logic;
      SPLB1_Sl_rdBTerm : out std_logic;
      SPLB1_Sl_MBusy : out std_logic_vector(0 to 3);
      SPLB1_Sl_MRdErr : out std_logic_vector(0 to 3);
      SPLB1_Sl_MWrErr : out std_logic_vector(0 to 3);
      SPLB1_Sl_MIRQ : out std_logic_vector(0 to 3);
      SDMA1_Clk : in std_logic;
      SDMA1_Rx_IntOut : out std_logic;
      SDMA1_Tx_IntOut : out std_logic;
      SDMA1_RstOut : out std_logic;
      SDMA1_TX_D : out std_logic_vector(0 to 31);
      SDMA1_TX_Rem : out std_logic_vector(0 to 3);
      SDMA1_TX_SOF : out std_logic;
      SDMA1_TX_EOF : out std_logic;
      SDMA1_TX_SOP : out std_logic;
      SDMA1_TX_EOP : out std_logic;
      SDMA1_TX_Src_Rdy : out std_logic;
      SDMA1_TX_Dst_Rdy : in std_logic;
      SDMA1_RX_D : in std_logic_vector(0 to 31);
      SDMA1_RX_Rem : in std_logic_vector(0 to 3);
      SDMA1_RX_SOF : in std_logic;
      SDMA1_RX_EOF : in std_logic;
      SDMA1_RX_SOP : in std_logic;
      SDMA1_RX_EOP : in std_logic;
      SDMA1_RX_Src_Rdy : in std_logic;
      SDMA1_RX_Dst_Rdy : out std_logic;
      SDMA_CTRL1_Clk : in std_logic;
      SDMA_CTRL1_Rst : in std_logic;
      SDMA_CTRL1_PLB_ABus : in std_logic_vector(0 to 31);
      SDMA_CTRL1_PLB_PAValid : in std_logic;
      SDMA_CTRL1_PLB_SAValid : in std_logic;
      SDMA_CTRL1_PLB_masterID : in std_logic_vector(0 to 0);
      SDMA_CTRL1_PLB_RNW : in std_logic;
      SDMA_CTRL1_PLB_BE : in std_logic_vector(0 to 7);
      SDMA_CTRL1_PLB_UABus : in std_logic_vector(0 to 31);
      SDMA_CTRL1_PLB_rdPrim : in std_logic;
      SDMA_CTRL1_PLB_wrPrim : in std_logic;
      SDMA_CTRL1_PLB_abort : in std_logic;
      SDMA_CTRL1_PLB_busLock : in std_logic;
      SDMA_CTRL1_PLB_MSize : in std_logic_vector(0 to 1);
      SDMA_CTRL1_PLB_size : in std_logic_vector(0 to 3);
      SDMA_CTRL1_PLB_type : in std_logic_vector(0 to 2);
      SDMA_CTRL1_PLB_lockErr : in std_logic;
      SDMA_CTRL1_PLB_wrPendReq : in std_logic;
      SDMA_CTRL1_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL1_PLB_rdPendReq : in std_logic;
      SDMA_CTRL1_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL1_PLB_reqPri : in std_logic_vector(0 to 1);
      SDMA_CTRL1_PLB_TAttribute : in std_logic_vector(0 to 15);
      SDMA_CTRL1_PLB_rdBurst : in std_logic;
      SDMA_CTRL1_PLB_wrBurst : in std_logic;
      SDMA_CTRL1_PLB_wrDBus : in std_logic_vector(0 to 63);
      SDMA_CTRL1_Sl_addrAck : out std_logic;
      SDMA_CTRL1_Sl_SSize : out std_logic_vector(0 to 1);
      SDMA_CTRL1_Sl_wait : out std_logic;
      SDMA_CTRL1_Sl_rearbitrate : out std_logic;
      SDMA_CTRL1_Sl_wrDAck : out std_logic;
      SDMA_CTRL1_Sl_wrComp : out std_logic;
      SDMA_CTRL1_Sl_wrBTerm : out std_logic;
      SDMA_CTRL1_Sl_rdDBus : out std_logic_vector(0 to 63);
      SDMA_CTRL1_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SDMA_CTRL1_Sl_rdDAck : out std_logic;
      SDMA_CTRL1_Sl_rdComp : out std_logic;
      SDMA_CTRL1_Sl_rdBTerm : out std_logic;
      SDMA_CTRL1_Sl_MBusy : out std_logic_vector(0 to 0);
      SDMA_CTRL1_Sl_MRdErr : out std_logic_vector(0 to 0);
      SDMA_CTRL1_Sl_MWrErr : out std_logic_vector(0 to 0);
      SDMA_CTRL1_Sl_MIRQ : out std_logic_vector(0 to 0);
      PIM1_Addr : in std_logic_vector(31 downto 0);
      PIM1_AddrReq : in std_logic;
      PIM1_AddrAck : out std_logic;
      PIM1_RNW : in std_logic;
      PIM1_Size : in std_logic_vector(3 downto 0);
      PIM1_RdModWr : in std_logic;
      PIM1_WrFIFO_Data : in std_logic_vector(63 downto 0);
      PIM1_WrFIFO_BE : in std_logic_vector(7 downto 0);
      PIM1_WrFIFO_Push : in std_logic;
      PIM1_RdFIFO_Data : out std_logic_vector(63 downto 0);
      PIM1_RdFIFO_Pop : in std_logic;
      PIM1_RdFIFO_RdWdAddr : out std_logic_vector(3 downto 0);
      PIM1_WrFIFO_Empty : out std_logic;
      PIM1_WrFIFO_AlmostFull : out std_logic;
      PIM1_WrFIFO_Flush : in std_logic;
      PIM1_RdFIFO_Empty : out std_logic;
      PIM1_RdFIFO_Flush : in std_logic;
      PIM1_RdFIFO_Latency : out std_logic_vector(1 downto 0);
      PIM1_InitDone : out std_logic;
      PPC440MC1_MIMCReadNotWrite : in std_logic;
      PPC440MC1_MIMCAddress : in std_logic_vector(0 to 35);
      PPC440MC1_MIMCAddressValid : in std_logic;
      PPC440MC1_MIMCWriteData : in std_logic_vector(0 to 127);
      PPC440MC1_MIMCWriteDataValid : in std_logic;
      PPC440MC1_MIMCByteEnable : in std_logic_vector(0 to 15);
      PPC440MC1_MIMCBankConflict : in std_logic;
      PPC440MC1_MIMCRowConflict : in std_logic;
      PPC440MC1_MCMIReadData : out std_logic_vector(0 to 127);
      PPC440MC1_MCMIReadDataValid : out std_logic;
      PPC440MC1_MCMIReadDataErr : out std_logic;
      PPC440MC1_MCMIAddrReadyToAccept : out std_logic;
      VFBC1_Cmd_Clk : in std_logic;
      VFBC1_Cmd_Reset : in std_logic;
      VFBC1_Cmd_Data : in std_logic_vector(31 downto 0);
      VFBC1_Cmd_Write : in std_logic;
      VFBC1_Cmd_End : in std_logic;
      VFBC1_Cmd_Full : out std_logic;
      VFBC1_Cmd_Almost_Full : out std_logic;
      VFBC1_Cmd_Idle : out std_logic;
      VFBC1_Wd_Clk : in std_logic;
      VFBC1_Wd_Reset : in std_logic;
      VFBC1_Wd_Write : in std_logic;
      VFBC1_Wd_End_Burst : in std_logic;
      VFBC1_Wd_Flush : in std_logic;
      VFBC1_Wd_Data : in std_logic_vector(31 downto 0);
      VFBC1_Wd_Data_BE : in std_logic_vector(3 downto 0);
      VFBC1_Wd_Full : out std_logic;
      VFBC1_Wd_Almost_Full : out std_logic;
      VFBC1_Rd_Clk : in std_logic;
      VFBC1_Rd_Reset : in std_logic;
      VFBC1_Rd_Read : in std_logic;
      VFBC1_Rd_End_Burst : in std_logic;
      VFBC1_Rd_Flush : in std_logic;
      VFBC1_Rd_Data : out std_logic_vector(31 downto 0);
      VFBC1_Rd_Empty : out std_logic;
      VFBC1_Rd_Almost_Empty : out std_logic;
      MCB1_cmd_clk : in std_logic;
      MCB1_cmd_en : in std_logic;
      MCB1_cmd_instr : in std_logic_vector(2 downto 0);
      MCB1_cmd_bl : in std_logic_vector(5 downto 0);
      MCB1_cmd_byte_addr : in std_logic_vector(29 downto 0);
      MCB1_cmd_empty : out std_logic;
      MCB1_cmd_full : out std_logic;
      MCB1_wr_clk : in std_logic;
      MCB1_wr_en : in std_logic;
      MCB1_wr_mask : in std_logic_vector(7 downto 0);
      MCB1_wr_data : in std_logic_vector(63 downto 0);
      MCB1_wr_full : out std_logic;
      MCB1_wr_empty : out std_logic;
      MCB1_wr_count : out std_logic_vector(6 downto 0);
      MCB1_wr_underrun : out std_logic;
      MCB1_wr_error : out std_logic;
      MCB1_rd_clk : in std_logic;
      MCB1_rd_en : in std_logic;
      MCB1_rd_data : out std_logic_vector(63 downto 0);
      MCB1_rd_full : out std_logic;
      MCB1_rd_empty : out std_logic;
      MCB1_rd_count : out std_logic_vector(6 downto 0);
      MCB1_rd_overflow : out std_logic;
      MCB1_rd_error : out std_logic;
      FSL2_M_Clk : in std_logic;
      FSL2_M_Write : in std_logic;
      FSL2_M_Data : in std_logic_vector(0 to 31);
      FSL2_M_Control : in std_logic;
      FSL2_M_Full : out std_logic;
      FSL2_S_Clk : in std_logic;
      FSL2_S_Read : in std_logic;
      FSL2_S_Data : out std_logic_vector(0 to 31);
      FSL2_S_Control : out std_logic;
      FSL2_S_Exists : out std_logic;
      FSL2_B_M_Clk : in std_logic;
      FSL2_B_M_Write : in std_logic;
      FSL2_B_M_Data : in std_logic_vector(0 to 31);
      FSL2_B_M_Control : in std_logic;
      FSL2_B_M_Full : out std_logic;
      FSL2_B_S_Clk : in std_logic;
      FSL2_B_S_Read : in std_logic;
      FSL2_B_S_Data : out std_logic_vector(0 to 31);
      FSL2_B_S_Control : out std_logic;
      FSL2_B_S_Exists : out std_logic;
      SPLB2_Clk : in std_logic;
      SPLB2_Rst : in std_logic;
      SPLB2_PLB_ABus : in std_logic_vector(0 to 31);
      SPLB2_PLB_PAValid : in std_logic;
      SPLB2_PLB_SAValid : in std_logic;
      SPLB2_PLB_masterID : in std_logic_vector(0 to 0);
      SPLB2_PLB_RNW : in std_logic;
      SPLB2_PLB_BE : in std_logic_vector(0 to 7);
      SPLB2_PLB_UABus : in std_logic_vector(0 to 31);
      SPLB2_PLB_rdPrim : in std_logic;
      SPLB2_PLB_wrPrim : in std_logic;
      SPLB2_PLB_abort : in std_logic;
      SPLB2_PLB_busLock : in std_logic;
      SPLB2_PLB_MSize : in std_logic_vector(0 to 1);
      SPLB2_PLB_size : in std_logic_vector(0 to 3);
      SPLB2_PLB_type : in std_logic_vector(0 to 2);
      SPLB2_PLB_lockErr : in std_logic;
      SPLB2_PLB_wrPendReq : in std_logic;
      SPLB2_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SPLB2_PLB_rdPendReq : in std_logic;
      SPLB2_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SPLB2_PLB_reqPri : in std_logic_vector(0 to 1);
      SPLB2_PLB_TAttribute : in std_logic_vector(0 to 15);
      SPLB2_PLB_rdBurst : in std_logic;
      SPLB2_PLB_wrBurst : in std_logic;
      SPLB2_PLB_wrDBus : in std_logic_vector(0 to 63);
      SPLB2_Sl_addrAck : out std_logic;
      SPLB2_Sl_SSize : out std_logic_vector(0 to 1);
      SPLB2_Sl_wait : out std_logic;
      SPLB2_Sl_rearbitrate : out std_logic;
      SPLB2_Sl_wrDAck : out std_logic;
      SPLB2_Sl_wrComp : out std_logic;
      SPLB2_Sl_wrBTerm : out std_logic;
      SPLB2_Sl_rdDBus : out std_logic_vector(0 to 63);
      SPLB2_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SPLB2_Sl_rdDAck : out std_logic;
      SPLB2_Sl_rdComp : out std_logic;
      SPLB2_Sl_rdBTerm : out std_logic;
      SPLB2_Sl_MBusy : out std_logic_vector(0 to 0);
      SPLB2_Sl_MRdErr : out std_logic_vector(0 to 0);
      SPLB2_Sl_MWrErr : out std_logic_vector(0 to 0);
      SPLB2_Sl_MIRQ : out std_logic_vector(0 to 0);
      SDMA2_Clk : in std_logic;
      SDMA2_Rx_IntOut : out std_logic;
      SDMA2_Tx_IntOut : out std_logic;
      SDMA2_RstOut : out std_logic;
      SDMA2_TX_D : out std_logic_vector(0 to 31);
      SDMA2_TX_Rem : out std_logic_vector(0 to 3);
      SDMA2_TX_SOF : out std_logic;
      SDMA2_TX_EOF : out std_logic;
      SDMA2_TX_SOP : out std_logic;
      SDMA2_TX_EOP : out std_logic;
      SDMA2_TX_Src_Rdy : out std_logic;
      SDMA2_TX_Dst_Rdy : in std_logic;
      SDMA2_RX_D : in std_logic_vector(0 to 31);
      SDMA2_RX_Rem : in std_logic_vector(0 to 3);
      SDMA2_RX_SOF : in std_logic;
      SDMA2_RX_EOF : in std_logic;
      SDMA2_RX_SOP : in std_logic;
      SDMA2_RX_EOP : in std_logic;
      SDMA2_RX_Src_Rdy : in std_logic;
      SDMA2_RX_Dst_Rdy : out std_logic;
      SDMA_CTRL2_Clk : in std_logic;
      SDMA_CTRL2_Rst : in std_logic;
      SDMA_CTRL2_PLB_ABus : in std_logic_vector(0 to 31);
      SDMA_CTRL2_PLB_PAValid : in std_logic;
      SDMA_CTRL2_PLB_SAValid : in std_logic;
      SDMA_CTRL2_PLB_masterID : in std_logic_vector(0 to 0);
      SDMA_CTRL2_PLB_RNW : in std_logic;
      SDMA_CTRL2_PLB_BE : in std_logic_vector(0 to 7);
      SDMA_CTRL2_PLB_UABus : in std_logic_vector(0 to 31);
      SDMA_CTRL2_PLB_rdPrim : in std_logic;
      SDMA_CTRL2_PLB_wrPrim : in std_logic;
      SDMA_CTRL2_PLB_abort : in std_logic;
      SDMA_CTRL2_PLB_busLock : in std_logic;
      SDMA_CTRL2_PLB_MSize : in std_logic_vector(0 to 1);
      SDMA_CTRL2_PLB_size : in std_logic_vector(0 to 3);
      SDMA_CTRL2_PLB_type : in std_logic_vector(0 to 2);
      SDMA_CTRL2_PLB_lockErr : in std_logic;
      SDMA_CTRL2_PLB_wrPendReq : in std_logic;
      SDMA_CTRL2_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL2_PLB_rdPendReq : in std_logic;
      SDMA_CTRL2_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL2_PLB_reqPri : in std_logic_vector(0 to 1);
      SDMA_CTRL2_PLB_TAttribute : in std_logic_vector(0 to 15);
      SDMA_CTRL2_PLB_rdBurst : in std_logic;
      SDMA_CTRL2_PLB_wrBurst : in std_logic;
      SDMA_CTRL2_PLB_wrDBus : in std_logic_vector(0 to 63);
      SDMA_CTRL2_Sl_addrAck : out std_logic;
      SDMA_CTRL2_Sl_SSize : out std_logic_vector(0 to 1);
      SDMA_CTRL2_Sl_wait : out std_logic;
      SDMA_CTRL2_Sl_rearbitrate : out std_logic;
      SDMA_CTRL2_Sl_wrDAck : out std_logic;
      SDMA_CTRL2_Sl_wrComp : out std_logic;
      SDMA_CTRL2_Sl_wrBTerm : out std_logic;
      SDMA_CTRL2_Sl_rdDBus : out std_logic_vector(0 to 63);
      SDMA_CTRL2_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SDMA_CTRL2_Sl_rdDAck : out std_logic;
      SDMA_CTRL2_Sl_rdComp : out std_logic;
      SDMA_CTRL2_Sl_rdBTerm : out std_logic;
      SDMA_CTRL2_Sl_MBusy : out std_logic_vector(0 to 0);
      SDMA_CTRL2_Sl_MRdErr : out std_logic_vector(0 to 0);
      SDMA_CTRL2_Sl_MWrErr : out std_logic_vector(0 to 0);
      SDMA_CTRL2_Sl_MIRQ : out std_logic_vector(0 to 0);
      PIM2_Addr : in std_logic_vector(31 downto 0);
      PIM2_AddrReq : in std_logic;
      PIM2_AddrAck : out std_logic;
      PIM2_RNW : in std_logic;
      PIM2_Size : in std_logic_vector(3 downto 0);
      PIM2_RdModWr : in std_logic;
      PIM2_WrFIFO_Data : in std_logic_vector(63 downto 0);
      PIM2_WrFIFO_BE : in std_logic_vector(7 downto 0);
      PIM2_WrFIFO_Push : in std_logic;
      PIM2_RdFIFO_Data : out std_logic_vector(63 downto 0);
      PIM2_RdFIFO_Pop : in std_logic;
      PIM2_RdFIFO_RdWdAddr : out std_logic_vector(3 downto 0);
      PIM2_WrFIFO_Empty : out std_logic;
      PIM2_WrFIFO_AlmostFull : out std_logic;
      PIM2_WrFIFO_Flush : in std_logic;
      PIM2_RdFIFO_Empty : out std_logic;
      PIM2_RdFIFO_Flush : in std_logic;
      PIM2_RdFIFO_Latency : out std_logic_vector(1 downto 0);
      PIM2_InitDone : out std_logic;
      PPC440MC2_MIMCReadNotWrite : in std_logic;
      PPC440MC2_MIMCAddress : in std_logic_vector(0 to 35);
      PPC440MC2_MIMCAddressValid : in std_logic;
      PPC440MC2_MIMCWriteData : in std_logic_vector(0 to 127);
      PPC440MC2_MIMCWriteDataValid : in std_logic;
      PPC440MC2_MIMCByteEnable : in std_logic_vector(0 to 15);
      PPC440MC2_MIMCBankConflict : in std_logic;
      PPC440MC2_MIMCRowConflict : in std_logic;
      PPC440MC2_MCMIReadData : out std_logic_vector(0 to 127);
      PPC440MC2_MCMIReadDataValid : out std_logic;
      PPC440MC2_MCMIReadDataErr : out std_logic;
      PPC440MC2_MCMIAddrReadyToAccept : out std_logic;
      VFBC2_Cmd_Clk : in std_logic;
      VFBC2_Cmd_Reset : in std_logic;
      VFBC2_Cmd_Data : in std_logic_vector(31 downto 0);
      VFBC2_Cmd_Write : in std_logic;
      VFBC2_Cmd_End : in std_logic;
      VFBC2_Cmd_Full : out std_logic;
      VFBC2_Cmd_Almost_Full : out std_logic;
      VFBC2_Cmd_Idle : out std_logic;
      VFBC2_Wd_Clk : in std_logic;
      VFBC2_Wd_Reset : in std_logic;
      VFBC2_Wd_Write : in std_logic;
      VFBC2_Wd_End_Burst : in std_logic;
      VFBC2_Wd_Flush : in std_logic;
      VFBC2_Wd_Data : in std_logic_vector(31 downto 0);
      VFBC2_Wd_Data_BE : in std_logic_vector(3 downto 0);
      VFBC2_Wd_Full : out std_logic;
      VFBC2_Wd_Almost_Full : out std_logic;
      VFBC2_Rd_Clk : in std_logic;
      VFBC2_Rd_Reset : in std_logic;
      VFBC2_Rd_Read : in std_logic;
      VFBC2_Rd_End_Burst : in std_logic;
      VFBC2_Rd_Flush : in std_logic;
      VFBC2_Rd_Data : out std_logic_vector(31 downto 0);
      VFBC2_Rd_Empty : out std_logic;
      VFBC2_Rd_Almost_Empty : out std_logic;
      MCB2_cmd_clk : in std_logic;
      MCB2_cmd_en : in std_logic;
      MCB2_cmd_instr : in std_logic_vector(2 downto 0);
      MCB2_cmd_bl : in std_logic_vector(5 downto 0);
      MCB2_cmd_byte_addr : in std_logic_vector(29 downto 0);
      MCB2_cmd_empty : out std_logic;
      MCB2_cmd_full : out std_logic;
      MCB2_wr_clk : in std_logic;
      MCB2_wr_en : in std_logic;
      MCB2_wr_mask : in std_logic_vector(7 downto 0);
      MCB2_wr_data : in std_logic_vector(63 downto 0);
      MCB2_wr_full : out std_logic;
      MCB2_wr_empty : out std_logic;
      MCB2_wr_count : out std_logic_vector(6 downto 0);
      MCB2_wr_underrun : out std_logic;
      MCB2_wr_error : out std_logic;
      MCB2_rd_clk : in std_logic;
      MCB2_rd_en : in std_logic;
      MCB2_rd_data : out std_logic_vector(63 downto 0);
      MCB2_rd_full : out std_logic;
      MCB2_rd_empty : out std_logic;
      MCB2_rd_count : out std_logic_vector(6 downto 0);
      MCB2_rd_overflow : out std_logic;
      MCB2_rd_error : out std_logic;
      FSL3_M_Clk : in std_logic;
      FSL3_M_Write : in std_logic;
      FSL3_M_Data : in std_logic_vector(0 to 31);
      FSL3_M_Control : in std_logic;
      FSL3_M_Full : out std_logic;
      FSL3_S_Clk : in std_logic;
      FSL3_S_Read : in std_logic;
      FSL3_S_Data : out std_logic_vector(0 to 31);
      FSL3_S_Control : out std_logic;
      FSL3_S_Exists : out std_logic;
      FSL3_B_M_Clk : in std_logic;
      FSL3_B_M_Write : in std_logic;
      FSL3_B_M_Data : in std_logic_vector(0 to 31);
      FSL3_B_M_Control : in std_logic;
      FSL3_B_M_Full : out std_logic;
      FSL3_B_S_Clk : in std_logic;
      FSL3_B_S_Read : in std_logic;
      FSL3_B_S_Data : out std_logic_vector(0 to 31);
      FSL3_B_S_Control : out std_logic;
      FSL3_B_S_Exists : out std_logic;
      SPLB3_Clk : in std_logic;
      SPLB3_Rst : in std_logic;
      SPLB3_PLB_ABus : in std_logic_vector(0 to 31);
      SPLB3_PLB_PAValid : in std_logic;
      SPLB3_PLB_SAValid : in std_logic;
      SPLB3_PLB_masterID : in std_logic_vector(0 to 0);
      SPLB3_PLB_RNW : in std_logic;
      SPLB3_PLB_BE : in std_logic_vector(0 to 7);
      SPLB3_PLB_UABus : in std_logic_vector(0 to 31);
      SPLB3_PLB_rdPrim : in std_logic;
      SPLB3_PLB_wrPrim : in std_logic;
      SPLB3_PLB_abort : in std_logic;
      SPLB3_PLB_busLock : in std_logic;
      SPLB3_PLB_MSize : in std_logic_vector(0 to 1);
      SPLB3_PLB_size : in std_logic_vector(0 to 3);
      SPLB3_PLB_type : in std_logic_vector(0 to 2);
      SPLB3_PLB_lockErr : in std_logic;
      SPLB3_PLB_wrPendReq : in std_logic;
      SPLB3_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SPLB3_PLB_rdPendReq : in std_logic;
      SPLB3_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SPLB3_PLB_reqPri : in std_logic_vector(0 to 1);
      SPLB3_PLB_TAttribute : in std_logic_vector(0 to 15);
      SPLB3_PLB_rdBurst : in std_logic;
      SPLB3_PLB_wrBurst : in std_logic;
      SPLB3_PLB_wrDBus : in std_logic_vector(0 to 63);
      SPLB3_Sl_addrAck : out std_logic;
      SPLB3_Sl_SSize : out std_logic_vector(0 to 1);
      SPLB3_Sl_wait : out std_logic;
      SPLB3_Sl_rearbitrate : out std_logic;
      SPLB3_Sl_wrDAck : out std_logic;
      SPLB3_Sl_wrComp : out std_logic;
      SPLB3_Sl_wrBTerm : out std_logic;
      SPLB3_Sl_rdDBus : out std_logic_vector(0 to 63);
      SPLB3_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SPLB3_Sl_rdDAck : out std_logic;
      SPLB3_Sl_rdComp : out std_logic;
      SPLB3_Sl_rdBTerm : out std_logic;
      SPLB3_Sl_MBusy : out std_logic_vector(0 to 0);
      SPLB3_Sl_MRdErr : out std_logic_vector(0 to 0);
      SPLB3_Sl_MWrErr : out std_logic_vector(0 to 0);
      SPLB3_Sl_MIRQ : out std_logic_vector(0 to 0);
      SDMA3_Clk : in std_logic;
      SDMA3_Rx_IntOut : out std_logic;
      SDMA3_Tx_IntOut : out std_logic;
      SDMA3_RstOut : out std_logic;
      SDMA3_TX_D : out std_logic_vector(0 to 31);
      SDMA3_TX_Rem : out std_logic_vector(0 to 3);
      SDMA3_TX_SOF : out std_logic;
      SDMA3_TX_EOF : out std_logic;
      SDMA3_TX_SOP : out std_logic;
      SDMA3_TX_EOP : out std_logic;
      SDMA3_TX_Src_Rdy : out std_logic;
      SDMA3_TX_Dst_Rdy : in std_logic;
      SDMA3_RX_D : in std_logic_vector(0 to 31);
      SDMA3_RX_Rem : in std_logic_vector(0 to 3);
      SDMA3_RX_SOF : in std_logic;
      SDMA3_RX_EOF : in std_logic;
      SDMA3_RX_SOP : in std_logic;
      SDMA3_RX_EOP : in std_logic;
      SDMA3_RX_Src_Rdy : in std_logic;
      SDMA3_RX_Dst_Rdy : out std_logic;
      SDMA_CTRL3_Clk : in std_logic;
      SDMA_CTRL3_Rst : in std_logic;
      SDMA_CTRL3_PLB_ABus : in std_logic_vector(0 to 31);
      SDMA_CTRL3_PLB_PAValid : in std_logic;
      SDMA_CTRL3_PLB_SAValid : in std_logic;
      SDMA_CTRL3_PLB_masterID : in std_logic_vector(0 to 0);
      SDMA_CTRL3_PLB_RNW : in std_logic;
      SDMA_CTRL3_PLB_BE : in std_logic_vector(0 to 7);
      SDMA_CTRL3_PLB_UABus : in std_logic_vector(0 to 31);
      SDMA_CTRL3_PLB_rdPrim : in std_logic;
      SDMA_CTRL3_PLB_wrPrim : in std_logic;
      SDMA_CTRL3_PLB_abort : in std_logic;
      SDMA_CTRL3_PLB_busLock : in std_logic;
      SDMA_CTRL3_PLB_MSize : in std_logic_vector(0 to 1);
      SDMA_CTRL3_PLB_size : in std_logic_vector(0 to 3);
      SDMA_CTRL3_PLB_type : in std_logic_vector(0 to 2);
      SDMA_CTRL3_PLB_lockErr : in std_logic;
      SDMA_CTRL3_PLB_wrPendReq : in std_logic;
      SDMA_CTRL3_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL3_PLB_rdPendReq : in std_logic;
      SDMA_CTRL3_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL3_PLB_reqPri : in std_logic_vector(0 to 1);
      SDMA_CTRL3_PLB_TAttribute : in std_logic_vector(0 to 15);
      SDMA_CTRL3_PLB_rdBurst : in std_logic;
      SDMA_CTRL3_PLB_wrBurst : in std_logic;
      SDMA_CTRL3_PLB_wrDBus : in std_logic_vector(0 to 63);
      SDMA_CTRL3_Sl_addrAck : out std_logic;
      SDMA_CTRL3_Sl_SSize : out std_logic_vector(0 to 1);
      SDMA_CTRL3_Sl_wait : out std_logic;
      SDMA_CTRL3_Sl_rearbitrate : out std_logic;
      SDMA_CTRL3_Sl_wrDAck : out std_logic;
      SDMA_CTRL3_Sl_wrComp : out std_logic;
      SDMA_CTRL3_Sl_wrBTerm : out std_logic;
      SDMA_CTRL3_Sl_rdDBus : out std_logic_vector(0 to 63);
      SDMA_CTRL3_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SDMA_CTRL3_Sl_rdDAck : out std_logic;
      SDMA_CTRL3_Sl_rdComp : out std_logic;
      SDMA_CTRL3_Sl_rdBTerm : out std_logic;
      SDMA_CTRL3_Sl_MBusy : out std_logic_vector(0 to 0);
      SDMA_CTRL3_Sl_MRdErr : out std_logic_vector(0 to 0);
      SDMA_CTRL3_Sl_MWrErr : out std_logic_vector(0 to 0);
      SDMA_CTRL3_Sl_MIRQ : out std_logic_vector(0 to 0);
      PIM3_Addr : in std_logic_vector(31 downto 0);
      PIM3_AddrReq : in std_logic;
      PIM3_AddrAck : out std_logic;
      PIM3_RNW : in std_logic;
      PIM3_Size : in std_logic_vector(3 downto 0);
      PIM3_RdModWr : in std_logic;
      PIM3_WrFIFO_Data : in std_logic_vector(63 downto 0);
      PIM3_WrFIFO_BE : in std_logic_vector(7 downto 0);
      PIM3_WrFIFO_Push : in std_logic;
      PIM3_RdFIFO_Data : out std_logic_vector(63 downto 0);
      PIM3_RdFIFO_Pop : in std_logic;
      PIM3_RdFIFO_RdWdAddr : out std_logic_vector(3 downto 0);
      PIM3_WrFIFO_Empty : out std_logic;
      PIM3_WrFIFO_AlmostFull : out std_logic;
      PIM3_WrFIFO_Flush : in std_logic;
      PIM3_RdFIFO_Empty : out std_logic;
      PIM3_RdFIFO_Flush : in std_logic;
      PIM3_RdFIFO_Latency : out std_logic_vector(1 downto 0);
      PIM3_InitDone : out std_logic;
      PPC440MC3_MIMCReadNotWrite : in std_logic;
      PPC440MC3_MIMCAddress : in std_logic_vector(0 to 35);
      PPC440MC3_MIMCAddressValid : in std_logic;
      PPC440MC3_MIMCWriteData : in std_logic_vector(0 to 127);
      PPC440MC3_MIMCWriteDataValid : in std_logic;
      PPC440MC3_MIMCByteEnable : in std_logic_vector(0 to 15);
      PPC440MC3_MIMCBankConflict : in std_logic;
      PPC440MC3_MIMCRowConflict : in std_logic;
      PPC440MC3_MCMIReadData : out std_logic_vector(0 to 127);
      PPC440MC3_MCMIReadDataValid : out std_logic;
      PPC440MC3_MCMIReadDataErr : out std_logic;
      PPC440MC3_MCMIAddrReadyToAccept : out std_logic;
      VFBC3_Cmd_Clk : in std_logic;
      VFBC3_Cmd_Reset : in std_logic;
      VFBC3_Cmd_Data : in std_logic_vector(31 downto 0);
      VFBC3_Cmd_Write : in std_logic;
      VFBC3_Cmd_End : in std_logic;
      VFBC3_Cmd_Full : out std_logic;
      VFBC3_Cmd_Almost_Full : out std_logic;
      VFBC3_Cmd_Idle : out std_logic;
      VFBC3_Wd_Clk : in std_logic;
      VFBC3_Wd_Reset : in std_logic;
      VFBC3_Wd_Write : in std_logic;
      VFBC3_Wd_End_Burst : in std_logic;
      VFBC3_Wd_Flush : in std_logic;
      VFBC3_Wd_Data : in std_logic_vector(31 downto 0);
      VFBC3_Wd_Data_BE : in std_logic_vector(3 downto 0);
      VFBC3_Wd_Full : out std_logic;
      VFBC3_Wd_Almost_Full : out std_logic;
      VFBC3_Rd_Clk : in std_logic;
      VFBC3_Rd_Reset : in std_logic;
      VFBC3_Rd_Read : in std_logic;
      VFBC3_Rd_End_Burst : in std_logic;
      VFBC3_Rd_Flush : in std_logic;
      VFBC3_Rd_Data : out std_logic_vector(31 downto 0);
      VFBC3_Rd_Empty : out std_logic;
      VFBC3_Rd_Almost_Empty : out std_logic;
      MCB3_cmd_clk : in std_logic;
      MCB3_cmd_en : in std_logic;
      MCB3_cmd_instr : in std_logic_vector(2 downto 0);
      MCB3_cmd_bl : in std_logic_vector(5 downto 0);
      MCB3_cmd_byte_addr : in std_logic_vector(29 downto 0);
      MCB3_cmd_empty : out std_logic;
      MCB3_cmd_full : out std_logic;
      MCB3_wr_clk : in std_logic;
      MCB3_wr_en : in std_logic;
      MCB3_wr_mask : in std_logic_vector(7 downto 0);
      MCB3_wr_data : in std_logic_vector(63 downto 0);
      MCB3_wr_full : out std_logic;
      MCB3_wr_empty : out std_logic;
      MCB3_wr_count : out std_logic_vector(6 downto 0);
      MCB3_wr_underrun : out std_logic;
      MCB3_wr_error : out std_logic;
      MCB3_rd_clk : in std_logic;
      MCB3_rd_en : in std_logic;
      MCB3_rd_data : out std_logic_vector(63 downto 0);
      MCB3_rd_full : out std_logic;
      MCB3_rd_empty : out std_logic;
      MCB3_rd_count : out std_logic_vector(6 downto 0);
      MCB3_rd_overflow : out std_logic;
      MCB3_rd_error : out std_logic;
      FSL4_M_Clk : in std_logic;
      FSL4_M_Write : in std_logic;
      FSL4_M_Data : in std_logic_vector(0 to 31);
      FSL4_M_Control : in std_logic;
      FSL4_M_Full : out std_logic;
      FSL4_S_Clk : in std_logic;
      FSL4_S_Read : in std_logic;
      FSL4_S_Data : out std_logic_vector(0 to 31);
      FSL4_S_Control : out std_logic;
      FSL4_S_Exists : out std_logic;
      FSL4_B_M_Clk : in std_logic;
      FSL4_B_M_Write : in std_logic;
      FSL4_B_M_Data : in std_logic_vector(0 to 31);
      FSL4_B_M_Control : in std_logic;
      FSL4_B_M_Full : out std_logic;
      FSL4_B_S_Clk : in std_logic;
      FSL4_B_S_Read : in std_logic;
      FSL4_B_S_Data : out std_logic_vector(0 to 31);
      FSL4_B_S_Control : out std_logic;
      FSL4_B_S_Exists : out std_logic;
      SPLB4_Clk : in std_logic;
      SPLB4_Rst : in std_logic;
      SPLB4_PLB_ABus : in std_logic_vector(0 to 31);
      SPLB4_PLB_PAValid : in std_logic;
      SPLB4_PLB_SAValid : in std_logic;
      SPLB4_PLB_masterID : in std_logic_vector(0 to 0);
      SPLB4_PLB_RNW : in std_logic;
      SPLB4_PLB_BE : in std_logic_vector(0 to 7);
      SPLB4_PLB_UABus : in std_logic_vector(0 to 31);
      SPLB4_PLB_rdPrim : in std_logic;
      SPLB4_PLB_wrPrim : in std_logic;
      SPLB4_PLB_abort : in std_logic;
      SPLB4_PLB_busLock : in std_logic;
      SPLB4_PLB_MSize : in std_logic_vector(0 to 1);
      SPLB4_PLB_size : in std_logic_vector(0 to 3);
      SPLB4_PLB_type : in std_logic_vector(0 to 2);
      SPLB4_PLB_lockErr : in std_logic;
      SPLB4_PLB_wrPendReq : in std_logic;
      SPLB4_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SPLB4_PLB_rdPendReq : in std_logic;
      SPLB4_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SPLB4_PLB_reqPri : in std_logic_vector(0 to 1);
      SPLB4_PLB_TAttribute : in std_logic_vector(0 to 15);
      SPLB4_PLB_rdBurst : in std_logic;
      SPLB4_PLB_wrBurst : in std_logic;
      SPLB4_PLB_wrDBus : in std_logic_vector(0 to 63);
      SPLB4_Sl_addrAck : out std_logic;
      SPLB4_Sl_SSize : out std_logic_vector(0 to 1);
      SPLB4_Sl_wait : out std_logic;
      SPLB4_Sl_rearbitrate : out std_logic;
      SPLB4_Sl_wrDAck : out std_logic;
      SPLB4_Sl_wrComp : out std_logic;
      SPLB4_Sl_wrBTerm : out std_logic;
      SPLB4_Sl_rdDBus : out std_logic_vector(0 to 63);
      SPLB4_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SPLB4_Sl_rdDAck : out std_logic;
      SPLB4_Sl_rdComp : out std_logic;
      SPLB4_Sl_rdBTerm : out std_logic;
      SPLB4_Sl_MBusy : out std_logic_vector(0 to 0);
      SPLB4_Sl_MRdErr : out std_logic_vector(0 to 0);
      SPLB4_Sl_MWrErr : out std_logic_vector(0 to 0);
      SPLB4_Sl_MIRQ : out std_logic_vector(0 to 0);
      SDMA4_Clk : in std_logic;
      SDMA4_Rx_IntOut : out std_logic;
      SDMA4_Tx_IntOut : out std_logic;
      SDMA4_RstOut : out std_logic;
      SDMA4_TX_D : out std_logic_vector(0 to 31);
      SDMA4_TX_Rem : out std_logic_vector(0 to 3);
      SDMA4_TX_SOF : out std_logic;
      SDMA4_TX_EOF : out std_logic;
      SDMA4_TX_SOP : out std_logic;
      SDMA4_TX_EOP : out std_logic;
      SDMA4_TX_Src_Rdy : out std_logic;
      SDMA4_TX_Dst_Rdy : in std_logic;
      SDMA4_RX_D : in std_logic_vector(0 to 31);
      SDMA4_RX_Rem : in std_logic_vector(0 to 3);
      SDMA4_RX_SOF : in std_logic;
      SDMA4_RX_EOF : in std_logic;
      SDMA4_RX_SOP : in std_logic;
      SDMA4_RX_EOP : in std_logic;
      SDMA4_RX_Src_Rdy : in std_logic;
      SDMA4_RX_Dst_Rdy : out std_logic;
      SDMA_CTRL4_Clk : in std_logic;
      SDMA_CTRL4_Rst : in std_logic;
      SDMA_CTRL4_PLB_ABus : in std_logic_vector(0 to 31);
      SDMA_CTRL4_PLB_PAValid : in std_logic;
      SDMA_CTRL4_PLB_SAValid : in std_logic;
      SDMA_CTRL4_PLB_masterID : in std_logic_vector(0 to 0);
      SDMA_CTRL4_PLB_RNW : in std_logic;
      SDMA_CTRL4_PLB_BE : in std_logic_vector(0 to 7);
      SDMA_CTRL4_PLB_UABus : in std_logic_vector(0 to 31);
      SDMA_CTRL4_PLB_rdPrim : in std_logic;
      SDMA_CTRL4_PLB_wrPrim : in std_logic;
      SDMA_CTRL4_PLB_abort : in std_logic;
      SDMA_CTRL4_PLB_busLock : in std_logic;
      SDMA_CTRL4_PLB_MSize : in std_logic_vector(0 to 1);
      SDMA_CTRL4_PLB_size : in std_logic_vector(0 to 3);
      SDMA_CTRL4_PLB_type : in std_logic_vector(0 to 2);
      SDMA_CTRL4_PLB_lockErr : in std_logic;
      SDMA_CTRL4_PLB_wrPendReq : in std_logic;
      SDMA_CTRL4_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL4_PLB_rdPendReq : in std_logic;
      SDMA_CTRL4_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL4_PLB_reqPri : in std_logic_vector(0 to 1);
      SDMA_CTRL4_PLB_TAttribute : in std_logic_vector(0 to 15);
      SDMA_CTRL4_PLB_rdBurst : in std_logic;
      SDMA_CTRL4_PLB_wrBurst : in std_logic;
      SDMA_CTRL4_PLB_wrDBus : in std_logic_vector(0 to 63);
      SDMA_CTRL4_Sl_addrAck : out std_logic;
      SDMA_CTRL4_Sl_SSize : out std_logic_vector(0 to 1);
      SDMA_CTRL4_Sl_wait : out std_logic;
      SDMA_CTRL4_Sl_rearbitrate : out std_logic;
      SDMA_CTRL4_Sl_wrDAck : out std_logic;
      SDMA_CTRL4_Sl_wrComp : out std_logic;
      SDMA_CTRL4_Sl_wrBTerm : out std_logic;
      SDMA_CTRL4_Sl_rdDBus : out std_logic_vector(0 to 63);
      SDMA_CTRL4_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SDMA_CTRL4_Sl_rdDAck : out std_logic;
      SDMA_CTRL4_Sl_rdComp : out std_logic;
      SDMA_CTRL4_Sl_rdBTerm : out std_logic;
      SDMA_CTRL4_Sl_MBusy : out std_logic_vector(0 to 0);
      SDMA_CTRL4_Sl_MRdErr : out std_logic_vector(0 to 0);
      SDMA_CTRL4_Sl_MWrErr : out std_logic_vector(0 to 0);
      SDMA_CTRL4_Sl_MIRQ : out std_logic_vector(0 to 0);
      PIM4_Addr : in std_logic_vector(31 downto 0);
      PIM4_AddrReq : in std_logic;
      PIM4_AddrAck : out std_logic;
      PIM4_RNW : in std_logic;
      PIM4_Size : in std_logic_vector(3 downto 0);
      PIM4_RdModWr : in std_logic;
      PIM4_WrFIFO_Data : in std_logic_vector(63 downto 0);
      PIM4_WrFIFO_BE : in std_logic_vector(7 downto 0);
      PIM4_WrFIFO_Push : in std_logic;
      PIM4_RdFIFO_Data : out std_logic_vector(63 downto 0);
      PIM4_RdFIFO_Pop : in std_logic;
      PIM4_RdFIFO_RdWdAddr : out std_logic_vector(3 downto 0);
      PIM4_WrFIFO_Empty : out std_logic;
      PIM4_WrFIFO_AlmostFull : out std_logic;
      PIM4_WrFIFO_Flush : in std_logic;
      PIM4_RdFIFO_Empty : out std_logic;
      PIM4_RdFIFO_Flush : in std_logic;
      PIM4_RdFIFO_Latency : out std_logic_vector(1 downto 0);
      PIM4_InitDone : out std_logic;
      PPC440MC4_MIMCReadNotWrite : in std_logic;
      PPC440MC4_MIMCAddress : in std_logic_vector(0 to 35);
      PPC440MC4_MIMCAddressValid : in std_logic;
      PPC440MC4_MIMCWriteData : in std_logic_vector(0 to 127);
      PPC440MC4_MIMCWriteDataValid : in std_logic;
      PPC440MC4_MIMCByteEnable : in std_logic_vector(0 to 15);
      PPC440MC4_MIMCBankConflict : in std_logic;
      PPC440MC4_MIMCRowConflict : in std_logic;
      PPC440MC4_MCMIReadData : out std_logic_vector(0 to 127);
      PPC440MC4_MCMIReadDataValid : out std_logic;
      PPC440MC4_MCMIReadDataErr : out std_logic;
      PPC440MC4_MCMIAddrReadyToAccept : out std_logic;
      VFBC4_Cmd_Clk : in std_logic;
      VFBC4_Cmd_Reset : in std_logic;
      VFBC4_Cmd_Data : in std_logic_vector(31 downto 0);
      VFBC4_Cmd_Write : in std_logic;
      VFBC4_Cmd_End : in std_logic;
      VFBC4_Cmd_Full : out std_logic;
      VFBC4_Cmd_Almost_Full : out std_logic;
      VFBC4_Cmd_Idle : out std_logic;
      VFBC4_Wd_Clk : in std_logic;
      VFBC4_Wd_Reset : in std_logic;
      VFBC4_Wd_Write : in std_logic;
      VFBC4_Wd_End_Burst : in std_logic;
      VFBC4_Wd_Flush : in std_logic;
      VFBC4_Wd_Data : in std_logic_vector(31 downto 0);
      VFBC4_Wd_Data_BE : in std_logic_vector(3 downto 0);
      VFBC4_Wd_Full : out std_logic;
      VFBC4_Wd_Almost_Full : out std_logic;
      VFBC4_Rd_Clk : in std_logic;
      VFBC4_Rd_Reset : in std_logic;
      VFBC4_Rd_Read : in std_logic;
      VFBC4_Rd_End_Burst : in std_logic;
      VFBC4_Rd_Flush : in std_logic;
      VFBC4_Rd_Data : out std_logic_vector(31 downto 0);
      VFBC4_Rd_Empty : out std_logic;
      VFBC4_Rd_Almost_Empty : out std_logic;
      MCB4_cmd_clk : in std_logic;
      MCB4_cmd_en : in std_logic;
      MCB4_cmd_instr : in std_logic_vector(2 downto 0);
      MCB4_cmd_bl : in std_logic_vector(5 downto 0);
      MCB4_cmd_byte_addr : in std_logic_vector(29 downto 0);
      MCB4_cmd_empty : out std_logic;
      MCB4_cmd_full : out std_logic;
      MCB4_wr_clk : in std_logic;
      MCB4_wr_en : in std_logic;
      MCB4_wr_mask : in std_logic_vector(7 downto 0);
      MCB4_wr_data : in std_logic_vector(63 downto 0);
      MCB4_wr_full : out std_logic;
      MCB4_wr_empty : out std_logic;
      MCB4_wr_count : out std_logic_vector(6 downto 0);
      MCB4_wr_underrun : out std_logic;
      MCB4_wr_error : out std_logic;
      MCB4_rd_clk : in std_logic;
      MCB4_rd_en : in std_logic;
      MCB4_rd_data : out std_logic_vector(63 downto 0);
      MCB4_rd_full : out std_logic;
      MCB4_rd_empty : out std_logic;
      MCB4_rd_count : out std_logic_vector(6 downto 0);
      MCB4_rd_overflow : out std_logic;
      MCB4_rd_error : out std_logic;
      FSL5_M_Clk : in std_logic;
      FSL5_M_Write : in std_logic;
      FSL5_M_Data : in std_logic_vector(0 to 31);
      FSL5_M_Control : in std_logic;
      FSL5_M_Full : out std_logic;
      FSL5_S_Clk : in std_logic;
      FSL5_S_Read : in std_logic;
      FSL5_S_Data : out std_logic_vector(0 to 31);
      FSL5_S_Control : out std_logic;
      FSL5_S_Exists : out std_logic;
      FSL5_B_M_Clk : in std_logic;
      FSL5_B_M_Write : in std_logic;
      FSL5_B_M_Data : in std_logic_vector(0 to 31);
      FSL5_B_M_Control : in std_logic;
      FSL5_B_M_Full : out std_logic;
      FSL5_B_S_Clk : in std_logic;
      FSL5_B_S_Read : in std_logic;
      FSL5_B_S_Data : out std_logic_vector(0 to 31);
      FSL5_B_S_Control : out std_logic;
      FSL5_B_S_Exists : out std_logic;
      SPLB5_Clk : in std_logic;
      SPLB5_Rst : in std_logic;
      SPLB5_PLB_ABus : in std_logic_vector(0 to 31);
      SPLB5_PLB_PAValid : in std_logic;
      SPLB5_PLB_SAValid : in std_logic;
      SPLB5_PLB_masterID : in std_logic_vector(0 to 0);
      SPLB5_PLB_RNW : in std_logic;
      SPLB5_PLB_BE : in std_logic_vector(0 to 7);
      SPLB5_PLB_UABus : in std_logic_vector(0 to 31);
      SPLB5_PLB_rdPrim : in std_logic;
      SPLB5_PLB_wrPrim : in std_logic;
      SPLB5_PLB_abort : in std_logic;
      SPLB5_PLB_busLock : in std_logic;
      SPLB5_PLB_MSize : in std_logic_vector(0 to 1);
      SPLB5_PLB_size : in std_logic_vector(0 to 3);
      SPLB5_PLB_type : in std_logic_vector(0 to 2);
      SPLB5_PLB_lockErr : in std_logic;
      SPLB5_PLB_wrPendReq : in std_logic;
      SPLB5_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SPLB5_PLB_rdPendReq : in std_logic;
      SPLB5_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SPLB5_PLB_reqPri : in std_logic_vector(0 to 1);
      SPLB5_PLB_TAttribute : in std_logic_vector(0 to 15);
      SPLB5_PLB_rdBurst : in std_logic;
      SPLB5_PLB_wrBurst : in std_logic;
      SPLB5_PLB_wrDBus : in std_logic_vector(0 to 63);
      SPLB5_Sl_addrAck : out std_logic;
      SPLB5_Sl_SSize : out std_logic_vector(0 to 1);
      SPLB5_Sl_wait : out std_logic;
      SPLB5_Sl_rearbitrate : out std_logic;
      SPLB5_Sl_wrDAck : out std_logic;
      SPLB5_Sl_wrComp : out std_logic;
      SPLB5_Sl_wrBTerm : out std_logic;
      SPLB5_Sl_rdDBus : out std_logic_vector(0 to 63);
      SPLB5_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SPLB5_Sl_rdDAck : out std_logic;
      SPLB5_Sl_rdComp : out std_logic;
      SPLB5_Sl_rdBTerm : out std_logic;
      SPLB5_Sl_MBusy : out std_logic_vector(0 to 0);
      SPLB5_Sl_MRdErr : out std_logic_vector(0 to 0);
      SPLB5_Sl_MWrErr : out std_logic_vector(0 to 0);
      SPLB5_Sl_MIRQ : out std_logic_vector(0 to 0);
      SDMA5_Clk : in std_logic;
      SDMA5_Rx_IntOut : out std_logic;
      SDMA5_Tx_IntOut : out std_logic;
      SDMA5_RstOut : out std_logic;
      SDMA5_TX_D : out std_logic_vector(0 to 31);
      SDMA5_TX_Rem : out std_logic_vector(0 to 3);
      SDMA5_TX_SOF : out std_logic;
      SDMA5_TX_EOF : out std_logic;
      SDMA5_TX_SOP : out std_logic;
      SDMA5_TX_EOP : out std_logic;
      SDMA5_TX_Src_Rdy : out std_logic;
      SDMA5_TX_Dst_Rdy : in std_logic;
      SDMA5_RX_D : in std_logic_vector(0 to 31);
      SDMA5_RX_Rem : in std_logic_vector(0 to 3);
      SDMA5_RX_SOF : in std_logic;
      SDMA5_RX_EOF : in std_logic;
      SDMA5_RX_SOP : in std_logic;
      SDMA5_RX_EOP : in std_logic;
      SDMA5_RX_Src_Rdy : in std_logic;
      SDMA5_RX_Dst_Rdy : out std_logic;
      SDMA_CTRL5_Clk : in std_logic;
      SDMA_CTRL5_Rst : in std_logic;
      SDMA_CTRL5_PLB_ABus : in std_logic_vector(0 to 31);
      SDMA_CTRL5_PLB_PAValid : in std_logic;
      SDMA_CTRL5_PLB_SAValid : in std_logic;
      SDMA_CTRL5_PLB_masterID : in std_logic_vector(0 to 0);
      SDMA_CTRL5_PLB_RNW : in std_logic;
      SDMA_CTRL5_PLB_BE : in std_logic_vector(0 to 7);
      SDMA_CTRL5_PLB_UABus : in std_logic_vector(0 to 31);
      SDMA_CTRL5_PLB_rdPrim : in std_logic;
      SDMA_CTRL5_PLB_wrPrim : in std_logic;
      SDMA_CTRL5_PLB_abort : in std_logic;
      SDMA_CTRL5_PLB_busLock : in std_logic;
      SDMA_CTRL5_PLB_MSize : in std_logic_vector(0 to 1);
      SDMA_CTRL5_PLB_size : in std_logic_vector(0 to 3);
      SDMA_CTRL5_PLB_type : in std_logic_vector(0 to 2);
      SDMA_CTRL5_PLB_lockErr : in std_logic;
      SDMA_CTRL5_PLB_wrPendReq : in std_logic;
      SDMA_CTRL5_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL5_PLB_rdPendReq : in std_logic;
      SDMA_CTRL5_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL5_PLB_reqPri : in std_logic_vector(0 to 1);
      SDMA_CTRL5_PLB_TAttribute : in std_logic_vector(0 to 15);
      SDMA_CTRL5_PLB_rdBurst : in std_logic;
      SDMA_CTRL5_PLB_wrBurst : in std_logic;
      SDMA_CTRL5_PLB_wrDBus : in std_logic_vector(0 to 63);
      SDMA_CTRL5_Sl_addrAck : out std_logic;
      SDMA_CTRL5_Sl_SSize : out std_logic_vector(0 to 1);
      SDMA_CTRL5_Sl_wait : out std_logic;
      SDMA_CTRL5_Sl_rearbitrate : out std_logic;
      SDMA_CTRL5_Sl_wrDAck : out std_logic;
      SDMA_CTRL5_Sl_wrComp : out std_logic;
      SDMA_CTRL5_Sl_wrBTerm : out std_logic;
      SDMA_CTRL5_Sl_rdDBus : out std_logic_vector(0 to 63);
      SDMA_CTRL5_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SDMA_CTRL5_Sl_rdDAck : out std_logic;
      SDMA_CTRL5_Sl_rdComp : out std_logic;
      SDMA_CTRL5_Sl_rdBTerm : out std_logic;
      SDMA_CTRL5_Sl_MBusy : out std_logic_vector(0 to 0);
      SDMA_CTRL5_Sl_MRdErr : out std_logic_vector(0 to 0);
      SDMA_CTRL5_Sl_MWrErr : out std_logic_vector(0 to 0);
      SDMA_CTRL5_Sl_MIRQ : out std_logic_vector(0 to 0);
      PIM5_Addr : in std_logic_vector(31 downto 0);
      PIM5_AddrReq : in std_logic;
      PIM5_AddrAck : out std_logic;
      PIM5_RNW : in std_logic;
      PIM5_Size : in std_logic_vector(3 downto 0);
      PIM5_RdModWr : in std_logic;
      PIM5_WrFIFO_Data : in std_logic_vector(63 downto 0);
      PIM5_WrFIFO_BE : in std_logic_vector(7 downto 0);
      PIM5_WrFIFO_Push : in std_logic;
      PIM5_RdFIFO_Data : out std_logic_vector(63 downto 0);
      PIM5_RdFIFO_Pop : in std_logic;
      PIM5_RdFIFO_RdWdAddr : out std_logic_vector(3 downto 0);
      PIM5_WrFIFO_Empty : out std_logic;
      PIM5_WrFIFO_AlmostFull : out std_logic;
      PIM5_WrFIFO_Flush : in std_logic;
      PIM5_RdFIFO_Empty : out std_logic;
      PIM5_RdFIFO_Flush : in std_logic;
      PIM5_RdFIFO_Latency : out std_logic_vector(1 downto 0);
      PIM5_InitDone : out std_logic;
      PPC440MC5_MIMCReadNotWrite : in std_logic;
      PPC440MC5_MIMCAddress : in std_logic_vector(0 to 35);
      PPC440MC5_MIMCAddressValid : in std_logic;
      PPC440MC5_MIMCWriteData : in std_logic_vector(0 to 127);
      PPC440MC5_MIMCWriteDataValid : in std_logic;
      PPC440MC5_MIMCByteEnable : in std_logic_vector(0 to 15);
      PPC440MC5_MIMCBankConflict : in std_logic;
      PPC440MC5_MIMCRowConflict : in std_logic;
      PPC440MC5_MCMIReadData : out std_logic_vector(0 to 127);
      PPC440MC5_MCMIReadDataValid : out std_logic;
      PPC440MC5_MCMIReadDataErr : out std_logic;
      PPC440MC5_MCMIAddrReadyToAccept : out std_logic;
      VFBC5_Cmd_Clk : in std_logic;
      VFBC5_Cmd_Reset : in std_logic;
      VFBC5_Cmd_Data : in std_logic_vector(31 downto 0);
      VFBC5_Cmd_Write : in std_logic;
      VFBC5_Cmd_End : in std_logic;
      VFBC5_Cmd_Full : out std_logic;
      VFBC5_Cmd_Almost_Full : out std_logic;
      VFBC5_Cmd_Idle : out std_logic;
      VFBC5_Wd_Clk : in std_logic;
      VFBC5_Wd_Reset : in std_logic;
      VFBC5_Wd_Write : in std_logic;
      VFBC5_Wd_End_Burst : in std_logic;
      VFBC5_Wd_Flush : in std_logic;
      VFBC5_Wd_Data : in std_logic_vector(31 downto 0);
      VFBC5_Wd_Data_BE : in std_logic_vector(3 downto 0);
      VFBC5_Wd_Full : out std_logic;
      VFBC5_Wd_Almost_Full : out std_logic;
      VFBC5_Rd_Clk : in std_logic;
      VFBC5_Rd_Reset : in std_logic;
      VFBC5_Rd_Read : in std_logic;
      VFBC5_Rd_End_Burst : in std_logic;
      VFBC5_Rd_Flush : in std_logic;
      VFBC5_Rd_Data : out std_logic_vector(31 downto 0);
      VFBC5_Rd_Empty : out std_logic;
      VFBC5_Rd_Almost_Empty : out std_logic;
      MCB5_cmd_clk : in std_logic;
      MCB5_cmd_en : in std_logic;
      MCB5_cmd_instr : in std_logic_vector(2 downto 0);
      MCB5_cmd_bl : in std_logic_vector(5 downto 0);
      MCB5_cmd_byte_addr : in std_logic_vector(29 downto 0);
      MCB5_cmd_empty : out std_logic;
      MCB5_cmd_full : out std_logic;
      MCB5_wr_clk : in std_logic;
      MCB5_wr_en : in std_logic;
      MCB5_wr_mask : in std_logic_vector(7 downto 0);
      MCB5_wr_data : in std_logic_vector(63 downto 0);
      MCB5_wr_full : out std_logic;
      MCB5_wr_empty : out std_logic;
      MCB5_wr_count : out std_logic_vector(6 downto 0);
      MCB5_wr_underrun : out std_logic;
      MCB5_wr_error : out std_logic;
      MCB5_rd_clk : in std_logic;
      MCB5_rd_en : in std_logic;
      MCB5_rd_data : out std_logic_vector(63 downto 0);
      MCB5_rd_full : out std_logic;
      MCB5_rd_empty : out std_logic;
      MCB5_rd_count : out std_logic_vector(6 downto 0);
      MCB5_rd_overflow : out std_logic;
      MCB5_rd_error : out std_logic;
      FSL6_M_Clk : in std_logic;
      FSL6_M_Write : in std_logic;
      FSL6_M_Data : in std_logic_vector(0 to 31);
      FSL6_M_Control : in std_logic;
      FSL6_M_Full : out std_logic;
      FSL6_S_Clk : in std_logic;
      FSL6_S_Read : in std_logic;
      FSL6_S_Data : out std_logic_vector(0 to 31);
      FSL6_S_Control : out std_logic;
      FSL6_S_Exists : out std_logic;
      FSL6_B_M_Clk : in std_logic;
      FSL6_B_M_Write : in std_logic;
      FSL6_B_M_Data : in std_logic_vector(0 to 31);
      FSL6_B_M_Control : in std_logic;
      FSL6_B_M_Full : out std_logic;
      FSL6_B_S_Clk : in std_logic;
      FSL6_B_S_Read : in std_logic;
      FSL6_B_S_Data : out std_logic_vector(0 to 31);
      FSL6_B_S_Control : out std_logic;
      FSL6_B_S_Exists : out std_logic;
      SPLB6_Clk : in std_logic;
      SPLB6_Rst : in std_logic;
      SPLB6_PLB_ABus : in std_logic_vector(0 to 31);
      SPLB6_PLB_PAValid : in std_logic;
      SPLB6_PLB_SAValid : in std_logic;
      SPLB6_PLB_masterID : in std_logic_vector(0 to 0);
      SPLB6_PLB_RNW : in std_logic;
      SPLB6_PLB_BE : in std_logic_vector(0 to 7);
      SPLB6_PLB_UABus : in std_logic_vector(0 to 31);
      SPLB6_PLB_rdPrim : in std_logic;
      SPLB6_PLB_wrPrim : in std_logic;
      SPLB6_PLB_abort : in std_logic;
      SPLB6_PLB_busLock : in std_logic;
      SPLB6_PLB_MSize : in std_logic_vector(0 to 1);
      SPLB6_PLB_size : in std_logic_vector(0 to 3);
      SPLB6_PLB_type : in std_logic_vector(0 to 2);
      SPLB6_PLB_lockErr : in std_logic;
      SPLB6_PLB_wrPendReq : in std_logic;
      SPLB6_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SPLB6_PLB_rdPendReq : in std_logic;
      SPLB6_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SPLB6_PLB_reqPri : in std_logic_vector(0 to 1);
      SPLB6_PLB_TAttribute : in std_logic_vector(0 to 15);
      SPLB6_PLB_rdBurst : in std_logic;
      SPLB6_PLB_wrBurst : in std_logic;
      SPLB6_PLB_wrDBus : in std_logic_vector(0 to 63);
      SPLB6_Sl_addrAck : out std_logic;
      SPLB6_Sl_SSize : out std_logic_vector(0 to 1);
      SPLB6_Sl_wait : out std_logic;
      SPLB6_Sl_rearbitrate : out std_logic;
      SPLB6_Sl_wrDAck : out std_logic;
      SPLB6_Sl_wrComp : out std_logic;
      SPLB6_Sl_wrBTerm : out std_logic;
      SPLB6_Sl_rdDBus : out std_logic_vector(0 to 63);
      SPLB6_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SPLB6_Sl_rdDAck : out std_logic;
      SPLB6_Sl_rdComp : out std_logic;
      SPLB6_Sl_rdBTerm : out std_logic;
      SPLB6_Sl_MBusy : out std_logic_vector(0 to 0);
      SPLB6_Sl_MRdErr : out std_logic_vector(0 to 0);
      SPLB6_Sl_MWrErr : out std_logic_vector(0 to 0);
      SPLB6_Sl_MIRQ : out std_logic_vector(0 to 0);
      SDMA6_Clk : in std_logic;
      SDMA6_Rx_IntOut : out std_logic;
      SDMA6_Tx_IntOut : out std_logic;
      SDMA6_RstOut : out std_logic;
      SDMA6_TX_D : out std_logic_vector(0 to 31);
      SDMA6_TX_Rem : out std_logic_vector(0 to 3);
      SDMA6_TX_SOF : out std_logic;
      SDMA6_TX_EOF : out std_logic;
      SDMA6_TX_SOP : out std_logic;
      SDMA6_TX_EOP : out std_logic;
      SDMA6_TX_Src_Rdy : out std_logic;
      SDMA6_TX_Dst_Rdy : in std_logic;
      SDMA6_RX_D : in std_logic_vector(0 to 31);
      SDMA6_RX_Rem : in std_logic_vector(0 to 3);
      SDMA6_RX_SOF : in std_logic;
      SDMA6_RX_EOF : in std_logic;
      SDMA6_RX_SOP : in std_logic;
      SDMA6_RX_EOP : in std_logic;
      SDMA6_RX_Src_Rdy : in std_logic;
      SDMA6_RX_Dst_Rdy : out std_logic;
      SDMA_CTRL6_Clk : in std_logic;
      SDMA_CTRL6_Rst : in std_logic;
      SDMA_CTRL6_PLB_ABus : in std_logic_vector(0 to 31);
      SDMA_CTRL6_PLB_PAValid : in std_logic;
      SDMA_CTRL6_PLB_SAValid : in std_logic;
      SDMA_CTRL6_PLB_masterID : in std_logic_vector(0 to 0);
      SDMA_CTRL6_PLB_RNW : in std_logic;
      SDMA_CTRL6_PLB_BE : in std_logic_vector(0 to 7);
      SDMA_CTRL6_PLB_UABus : in std_logic_vector(0 to 31);
      SDMA_CTRL6_PLB_rdPrim : in std_logic;
      SDMA_CTRL6_PLB_wrPrim : in std_logic;
      SDMA_CTRL6_PLB_abort : in std_logic;
      SDMA_CTRL6_PLB_busLock : in std_logic;
      SDMA_CTRL6_PLB_MSize : in std_logic_vector(0 to 1);
      SDMA_CTRL6_PLB_size : in std_logic_vector(0 to 3);
      SDMA_CTRL6_PLB_type : in std_logic_vector(0 to 2);
      SDMA_CTRL6_PLB_lockErr : in std_logic;
      SDMA_CTRL6_PLB_wrPendReq : in std_logic;
      SDMA_CTRL6_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL6_PLB_rdPendReq : in std_logic;
      SDMA_CTRL6_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL6_PLB_reqPri : in std_logic_vector(0 to 1);
      SDMA_CTRL6_PLB_TAttribute : in std_logic_vector(0 to 15);
      SDMA_CTRL6_PLB_rdBurst : in std_logic;
      SDMA_CTRL6_PLB_wrBurst : in std_logic;
      SDMA_CTRL6_PLB_wrDBus : in std_logic_vector(0 to 63);
      SDMA_CTRL6_Sl_addrAck : out std_logic;
      SDMA_CTRL6_Sl_SSize : out std_logic_vector(0 to 1);
      SDMA_CTRL6_Sl_wait : out std_logic;
      SDMA_CTRL6_Sl_rearbitrate : out std_logic;
      SDMA_CTRL6_Sl_wrDAck : out std_logic;
      SDMA_CTRL6_Sl_wrComp : out std_logic;
      SDMA_CTRL6_Sl_wrBTerm : out std_logic;
      SDMA_CTRL6_Sl_rdDBus : out std_logic_vector(0 to 63);
      SDMA_CTRL6_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SDMA_CTRL6_Sl_rdDAck : out std_logic;
      SDMA_CTRL6_Sl_rdComp : out std_logic;
      SDMA_CTRL6_Sl_rdBTerm : out std_logic;
      SDMA_CTRL6_Sl_MBusy : out std_logic_vector(0 to 0);
      SDMA_CTRL6_Sl_MRdErr : out std_logic_vector(0 to 0);
      SDMA_CTRL6_Sl_MWrErr : out std_logic_vector(0 to 0);
      SDMA_CTRL6_Sl_MIRQ : out std_logic_vector(0 to 0);
      PIM6_Addr : in std_logic_vector(31 downto 0);
      PIM6_AddrReq : in std_logic;
      PIM6_AddrAck : out std_logic;
      PIM6_RNW : in std_logic;
      PIM6_Size : in std_logic_vector(3 downto 0);
      PIM6_RdModWr : in std_logic;
      PIM6_WrFIFO_Data : in std_logic_vector(63 downto 0);
      PIM6_WrFIFO_BE : in std_logic_vector(7 downto 0);
      PIM6_WrFIFO_Push : in std_logic;
      PIM6_RdFIFO_Data : out std_logic_vector(63 downto 0);
      PIM6_RdFIFO_Pop : in std_logic;
      PIM6_RdFIFO_RdWdAddr : out std_logic_vector(3 downto 0);
      PIM6_WrFIFO_Empty : out std_logic;
      PIM6_WrFIFO_AlmostFull : out std_logic;
      PIM6_WrFIFO_Flush : in std_logic;
      PIM6_RdFIFO_Empty : out std_logic;
      PIM6_RdFIFO_Flush : in std_logic;
      PIM6_RdFIFO_Latency : out std_logic_vector(1 downto 0);
      PIM6_InitDone : out std_logic;
      PPC440MC6_MIMCReadNotWrite : in std_logic;
      PPC440MC6_MIMCAddress : in std_logic_vector(0 to 35);
      PPC440MC6_MIMCAddressValid : in std_logic;
      PPC440MC6_MIMCWriteData : in std_logic_vector(0 to 127);
      PPC440MC6_MIMCWriteDataValid : in std_logic;
      PPC440MC6_MIMCByteEnable : in std_logic_vector(0 to 15);
      PPC440MC6_MIMCBankConflict : in std_logic;
      PPC440MC6_MIMCRowConflict : in std_logic;
      PPC440MC6_MCMIReadData : out std_logic_vector(0 to 127);
      PPC440MC6_MCMIReadDataValid : out std_logic;
      PPC440MC6_MCMIReadDataErr : out std_logic;
      PPC440MC6_MCMIAddrReadyToAccept : out std_logic;
      VFBC6_Cmd_Clk : in std_logic;
      VFBC6_Cmd_Reset : in std_logic;
      VFBC6_Cmd_Data : in std_logic_vector(31 downto 0);
      VFBC6_Cmd_Write : in std_logic;
      VFBC6_Cmd_End : in std_logic;
      VFBC6_Cmd_Full : out std_logic;
      VFBC6_Cmd_Almost_Full : out std_logic;
      VFBC6_Cmd_Idle : out std_logic;
      VFBC6_Wd_Clk : in std_logic;
      VFBC6_Wd_Reset : in std_logic;
      VFBC6_Wd_Write : in std_logic;
      VFBC6_Wd_End_Burst : in std_logic;
      VFBC6_Wd_Flush : in std_logic;
      VFBC6_Wd_Data : in std_logic_vector(31 downto 0);
      VFBC6_Wd_Data_BE : in std_logic_vector(3 downto 0);
      VFBC6_Wd_Full : out std_logic;
      VFBC6_Wd_Almost_Full : out std_logic;
      VFBC6_Rd_Clk : in std_logic;
      VFBC6_Rd_Reset : in std_logic;
      VFBC6_Rd_Read : in std_logic;
      VFBC6_Rd_End_Burst : in std_logic;
      VFBC6_Rd_Flush : in std_logic;
      VFBC6_Rd_Data : out std_logic_vector(31 downto 0);
      VFBC6_Rd_Empty : out std_logic;
      VFBC6_Rd_Almost_Empty : out std_logic;
      MCB6_cmd_clk : in std_logic;
      MCB6_cmd_en : in std_logic;
      MCB6_cmd_instr : in std_logic_vector(2 downto 0);
      MCB6_cmd_bl : in std_logic_vector(5 downto 0);
      MCB6_cmd_byte_addr : in std_logic_vector(29 downto 0);
      MCB6_cmd_empty : out std_logic;
      MCB6_cmd_full : out std_logic;
      MCB6_wr_clk : in std_logic;
      MCB6_wr_en : in std_logic;
      MCB6_wr_mask : in std_logic_vector(7 downto 0);
      MCB6_wr_data : in std_logic_vector(63 downto 0);
      MCB6_wr_full : out std_logic;
      MCB6_wr_empty : out std_logic;
      MCB6_wr_count : out std_logic_vector(6 downto 0);
      MCB6_wr_underrun : out std_logic;
      MCB6_wr_error : out std_logic;
      MCB6_rd_clk : in std_logic;
      MCB6_rd_en : in std_logic;
      MCB6_rd_data : out std_logic_vector(63 downto 0);
      MCB6_rd_full : out std_logic;
      MCB6_rd_empty : out std_logic;
      MCB6_rd_count : out std_logic_vector(6 downto 0);
      MCB6_rd_overflow : out std_logic;
      MCB6_rd_error : out std_logic;
      FSL7_M_Clk : in std_logic;
      FSL7_M_Write : in std_logic;
      FSL7_M_Data : in std_logic_vector(0 to 31);
      FSL7_M_Control : in std_logic;
      FSL7_M_Full : out std_logic;
      FSL7_S_Clk : in std_logic;
      FSL7_S_Read : in std_logic;
      FSL7_S_Data : out std_logic_vector(0 to 31);
      FSL7_S_Control : out std_logic;
      FSL7_S_Exists : out std_logic;
      FSL7_B_M_Clk : in std_logic;
      FSL7_B_M_Write : in std_logic;
      FSL7_B_M_Data : in std_logic_vector(0 to 31);
      FSL7_B_M_Control : in std_logic;
      FSL7_B_M_Full : out std_logic;
      FSL7_B_S_Clk : in std_logic;
      FSL7_B_S_Read : in std_logic;
      FSL7_B_S_Data : out std_logic_vector(0 to 31);
      FSL7_B_S_Control : out std_logic;
      FSL7_B_S_Exists : out std_logic;
      SPLB7_Clk : in std_logic;
      SPLB7_Rst : in std_logic;
      SPLB7_PLB_ABus : in std_logic_vector(0 to 31);
      SPLB7_PLB_PAValid : in std_logic;
      SPLB7_PLB_SAValid : in std_logic;
      SPLB7_PLB_masterID : in std_logic_vector(0 to 0);
      SPLB7_PLB_RNW : in std_logic;
      SPLB7_PLB_BE : in std_logic_vector(0 to 7);
      SPLB7_PLB_UABus : in std_logic_vector(0 to 31);
      SPLB7_PLB_rdPrim : in std_logic;
      SPLB7_PLB_wrPrim : in std_logic;
      SPLB7_PLB_abort : in std_logic;
      SPLB7_PLB_busLock : in std_logic;
      SPLB7_PLB_MSize : in std_logic_vector(0 to 1);
      SPLB7_PLB_size : in std_logic_vector(0 to 3);
      SPLB7_PLB_type : in std_logic_vector(0 to 2);
      SPLB7_PLB_lockErr : in std_logic;
      SPLB7_PLB_wrPendReq : in std_logic;
      SPLB7_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SPLB7_PLB_rdPendReq : in std_logic;
      SPLB7_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SPLB7_PLB_reqPri : in std_logic_vector(0 to 1);
      SPLB7_PLB_TAttribute : in std_logic_vector(0 to 15);
      SPLB7_PLB_rdBurst : in std_logic;
      SPLB7_PLB_wrBurst : in std_logic;
      SPLB7_PLB_wrDBus : in std_logic_vector(0 to 63);
      SPLB7_Sl_addrAck : out std_logic;
      SPLB7_Sl_SSize : out std_logic_vector(0 to 1);
      SPLB7_Sl_wait : out std_logic;
      SPLB7_Sl_rearbitrate : out std_logic;
      SPLB7_Sl_wrDAck : out std_logic;
      SPLB7_Sl_wrComp : out std_logic;
      SPLB7_Sl_wrBTerm : out std_logic;
      SPLB7_Sl_rdDBus : out std_logic_vector(0 to 63);
      SPLB7_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SPLB7_Sl_rdDAck : out std_logic;
      SPLB7_Sl_rdComp : out std_logic;
      SPLB7_Sl_rdBTerm : out std_logic;
      SPLB7_Sl_MBusy : out std_logic_vector(0 to 0);
      SPLB7_Sl_MRdErr : out std_logic_vector(0 to 0);
      SPLB7_Sl_MWrErr : out std_logic_vector(0 to 0);
      SPLB7_Sl_MIRQ : out std_logic_vector(0 to 0);
      SDMA7_Clk : in std_logic;
      SDMA7_Rx_IntOut : out std_logic;
      SDMA7_Tx_IntOut : out std_logic;
      SDMA7_RstOut : out std_logic;
      SDMA7_TX_D : out std_logic_vector(0 to 31);
      SDMA7_TX_Rem : out std_logic_vector(0 to 3);
      SDMA7_TX_SOF : out std_logic;
      SDMA7_TX_EOF : out std_logic;
      SDMA7_TX_SOP : out std_logic;
      SDMA7_TX_EOP : out std_logic;
      SDMA7_TX_Src_Rdy : out std_logic;
      SDMA7_TX_Dst_Rdy : in std_logic;
      SDMA7_RX_D : in std_logic_vector(0 to 31);
      SDMA7_RX_Rem : in std_logic_vector(0 to 3);
      SDMA7_RX_SOF : in std_logic;
      SDMA7_RX_EOF : in std_logic;
      SDMA7_RX_SOP : in std_logic;
      SDMA7_RX_EOP : in std_logic;
      SDMA7_RX_Src_Rdy : in std_logic;
      SDMA7_RX_Dst_Rdy : out std_logic;
      SDMA_CTRL7_Clk : in std_logic;
      SDMA_CTRL7_Rst : in std_logic;
      SDMA_CTRL7_PLB_ABus : in std_logic_vector(0 to 31);
      SDMA_CTRL7_PLB_PAValid : in std_logic;
      SDMA_CTRL7_PLB_SAValid : in std_logic;
      SDMA_CTRL7_PLB_masterID : in std_logic_vector(0 to 0);
      SDMA_CTRL7_PLB_RNW : in std_logic;
      SDMA_CTRL7_PLB_BE : in std_logic_vector(0 to 7);
      SDMA_CTRL7_PLB_UABus : in std_logic_vector(0 to 31);
      SDMA_CTRL7_PLB_rdPrim : in std_logic;
      SDMA_CTRL7_PLB_wrPrim : in std_logic;
      SDMA_CTRL7_PLB_abort : in std_logic;
      SDMA_CTRL7_PLB_busLock : in std_logic;
      SDMA_CTRL7_PLB_MSize : in std_logic_vector(0 to 1);
      SDMA_CTRL7_PLB_size : in std_logic_vector(0 to 3);
      SDMA_CTRL7_PLB_type : in std_logic_vector(0 to 2);
      SDMA_CTRL7_PLB_lockErr : in std_logic;
      SDMA_CTRL7_PLB_wrPendReq : in std_logic;
      SDMA_CTRL7_PLB_wrPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL7_PLB_rdPendReq : in std_logic;
      SDMA_CTRL7_PLB_rdPendPri : in std_logic_vector(0 to 1);
      SDMA_CTRL7_PLB_reqPri : in std_logic_vector(0 to 1);
      SDMA_CTRL7_PLB_TAttribute : in std_logic_vector(0 to 15);
      SDMA_CTRL7_PLB_rdBurst : in std_logic;
      SDMA_CTRL7_PLB_wrBurst : in std_logic;
      SDMA_CTRL7_PLB_wrDBus : in std_logic_vector(0 to 63);
      SDMA_CTRL7_Sl_addrAck : out std_logic;
      SDMA_CTRL7_Sl_SSize : out std_logic_vector(0 to 1);
      SDMA_CTRL7_Sl_wait : out std_logic;
      SDMA_CTRL7_Sl_rearbitrate : out std_logic;
      SDMA_CTRL7_Sl_wrDAck : out std_logic;
      SDMA_CTRL7_Sl_wrComp : out std_logic;
      SDMA_CTRL7_Sl_wrBTerm : out std_logic;
      SDMA_CTRL7_Sl_rdDBus : out std_logic_vector(0 to 63);
      SDMA_CTRL7_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      SDMA_CTRL7_Sl_rdDAck : out std_logic;
      SDMA_CTRL7_Sl_rdComp : out std_logic;
      SDMA_CTRL7_Sl_rdBTerm : out std_logic;
      SDMA_CTRL7_Sl_MBusy : out std_logic_vector(0 to 0);
      SDMA_CTRL7_Sl_MRdErr : out std_logic_vector(0 to 0);
      SDMA_CTRL7_Sl_MWrErr : out std_logic_vector(0 to 0);
      SDMA_CTRL7_Sl_MIRQ : out std_logic_vector(0 to 0);
      PIM7_Addr : in std_logic_vector(31 downto 0);
      PIM7_AddrReq : in std_logic;
      PIM7_AddrAck : out std_logic;
      PIM7_RNW : in std_logic;
      PIM7_Size : in std_logic_vector(3 downto 0);
      PIM7_RdModWr : in std_logic;
      PIM7_WrFIFO_Data : in std_logic_vector(63 downto 0);
      PIM7_WrFIFO_BE : in std_logic_vector(7 downto 0);
      PIM7_WrFIFO_Push : in std_logic;
      PIM7_RdFIFO_Data : out std_logic_vector(63 downto 0);
      PIM7_RdFIFO_Pop : in std_logic;
      PIM7_RdFIFO_RdWdAddr : out std_logic_vector(3 downto 0);
      PIM7_WrFIFO_Empty : out std_logic;
      PIM7_WrFIFO_AlmostFull : out std_logic;
      PIM7_WrFIFO_Flush : in std_logic;
      PIM7_RdFIFO_Empty : out std_logic;
      PIM7_RdFIFO_Flush : in std_logic;
      PIM7_RdFIFO_Latency : out std_logic_vector(1 downto 0);
      PIM7_InitDone : out std_logic;
      PPC440MC7_MIMCReadNotWrite : in std_logic;
      PPC440MC7_MIMCAddress : in std_logic_vector(0 to 35);
      PPC440MC7_MIMCAddressValid : in std_logic;
      PPC440MC7_MIMCWriteData : in std_logic_vector(0 to 127);
      PPC440MC7_MIMCWriteDataValid : in std_logic;
      PPC440MC7_MIMCByteEnable : in std_logic_vector(0 to 15);
      PPC440MC7_MIMCBankConflict : in std_logic;
      PPC440MC7_MIMCRowConflict : in std_logic;
      PPC440MC7_MCMIReadData : out std_logic_vector(0 to 127);
      PPC440MC7_MCMIReadDataValid : out std_logic;
      PPC440MC7_MCMIReadDataErr : out std_logic;
      PPC440MC7_MCMIAddrReadyToAccept : out std_logic;
      VFBC7_Cmd_Clk : in std_logic;
      VFBC7_Cmd_Reset : in std_logic;
      VFBC7_Cmd_Data : in std_logic_vector(31 downto 0);
      VFBC7_Cmd_Write : in std_logic;
      VFBC7_Cmd_End : in std_logic;
      VFBC7_Cmd_Full : out std_logic;
      VFBC7_Cmd_Almost_Full : out std_logic;
      VFBC7_Cmd_Idle : out std_logic;
      VFBC7_Wd_Clk : in std_logic;
      VFBC7_Wd_Reset : in std_logic;
      VFBC7_Wd_Write : in std_logic;
      VFBC7_Wd_End_Burst : in std_logic;
      VFBC7_Wd_Flush : in std_logic;
      VFBC7_Wd_Data : in std_logic_vector(31 downto 0);
      VFBC7_Wd_Data_BE : in std_logic_vector(3 downto 0);
      VFBC7_Wd_Full : out std_logic;
      VFBC7_Wd_Almost_Full : out std_logic;
      VFBC7_Rd_Clk : in std_logic;
      VFBC7_Rd_Reset : in std_logic;
      VFBC7_Rd_Read : in std_logic;
      VFBC7_Rd_End_Burst : in std_logic;
      VFBC7_Rd_Flush : in std_logic;
      VFBC7_Rd_Data : out std_logic_vector(31 downto 0);
      VFBC7_Rd_Empty : out std_logic;
      VFBC7_Rd_Almost_Empty : out std_logic;
      MCB7_cmd_clk : in std_logic;
      MCB7_cmd_en : in std_logic;
      MCB7_cmd_instr : in std_logic_vector(2 downto 0);
      MCB7_cmd_bl : in std_logic_vector(5 downto 0);
      MCB7_cmd_byte_addr : in std_logic_vector(29 downto 0);
      MCB7_cmd_empty : out std_logic;
      MCB7_cmd_full : out std_logic;
      MCB7_wr_clk : in std_logic;
      MCB7_wr_en : in std_logic;
      MCB7_wr_mask : in std_logic_vector(7 downto 0);
      MCB7_wr_data : in std_logic_vector(63 downto 0);
      MCB7_wr_full : out std_logic;
      MCB7_wr_empty : out std_logic;
      MCB7_wr_count : out std_logic_vector(6 downto 0);
      MCB7_wr_underrun : out std_logic;
      MCB7_wr_error : out std_logic;
      MCB7_rd_clk : in std_logic;
      MCB7_rd_en : in std_logic;
      MCB7_rd_data : out std_logic_vector(63 downto 0);
      MCB7_rd_full : out std_logic;
      MCB7_rd_empty : out std_logic;
      MCB7_rd_count : out std_logic_vector(6 downto 0);
      MCB7_rd_overflow : out std_logic;
      MCB7_rd_error : out std_logic;
      MPMC_CTRL_Clk : in std_logic;
      MPMC_CTRL_Rst : in std_logic;
      MPMC_CTRL_PLB_ABus : in std_logic_vector(0 to 31);
      MPMC_CTRL_PLB_PAValid : in std_logic;
      MPMC_CTRL_PLB_SAValid : in std_logic;
      MPMC_CTRL_PLB_masterID : in std_logic_vector(0 to 0);
      MPMC_CTRL_PLB_RNW : in std_logic;
      MPMC_CTRL_PLB_BE : in std_logic_vector(0 to 7);
      MPMC_CTRL_PLB_UABus : in std_logic_vector(0 to 31);
      MPMC_CTRL_PLB_rdPrim : in std_logic;
      MPMC_CTRL_PLB_wrPrim : in std_logic;
      MPMC_CTRL_PLB_abort : in std_logic;
      MPMC_CTRL_PLB_busLock : in std_logic;
      MPMC_CTRL_PLB_MSize : in std_logic_vector(0 to 1);
      MPMC_CTRL_PLB_size : in std_logic_vector(0 to 3);
      MPMC_CTRL_PLB_type : in std_logic_vector(0 to 2);
      MPMC_CTRL_PLB_lockErr : in std_logic;
      MPMC_CTRL_PLB_wrPendReq : in std_logic;
      MPMC_CTRL_PLB_wrPendPri : in std_logic_vector(0 to 1);
      MPMC_CTRL_PLB_rdPendReq : in std_logic;
      MPMC_CTRL_PLB_rdPendPri : in std_logic_vector(0 to 1);
      MPMC_CTRL_PLB_reqPri : in std_logic_vector(0 to 1);
      MPMC_CTRL_PLB_TAttribute : in std_logic_vector(0 to 15);
      MPMC_CTRL_PLB_rdBurst : in std_logic;
      MPMC_CTRL_PLB_wrBurst : in std_logic;
      MPMC_CTRL_PLB_wrDBus : in std_logic_vector(0 to 63);
      MPMC_CTRL_Sl_addrAck : out std_logic;
      MPMC_CTRL_Sl_SSize : out std_logic_vector(0 to 1);
      MPMC_CTRL_Sl_wait : out std_logic;
      MPMC_CTRL_Sl_rearbitrate : out std_logic;
      MPMC_CTRL_Sl_wrDAck : out std_logic;
      MPMC_CTRL_Sl_wrComp : out std_logic;
      MPMC_CTRL_Sl_wrBTerm : out std_logic;
      MPMC_CTRL_Sl_rdDBus : out std_logic_vector(0 to 63);
      MPMC_CTRL_Sl_rdWdAddr : out std_logic_vector(0 to 3);
      MPMC_CTRL_Sl_rdDAck : out std_logic;
      MPMC_CTRL_Sl_rdComp : out std_logic;
      MPMC_CTRL_Sl_rdBTerm : out std_logic;
      MPMC_CTRL_Sl_MBusy : out std_logic_vector(0 to 1);
      MPMC_CTRL_Sl_MRdErr : out std_logic_vector(0 to 1);
      MPMC_CTRL_Sl_MWrErr : out std_logic_vector(0 to 1);
      MPMC_CTRL_Sl_MIRQ : out std_logic_vector(0 to 1);
      MPMC_Clk0 : in std_logic;
      MPMC_Clk0_DIV2 : in std_logic;
      MPMC_Clk90 : in std_logic;
      MPMC_Clk_200MHz : in std_logic;
      MPMC_Rst : in std_logic;
      MPMC_Clk_Mem : in std_logic;
      MPMC_Clk_Mem_2x : in std_logic;
      MPMC_Clk_Mem_2x_180 : in std_logic;
      MPMC_Clk_Mem_2x_CE0 : in std_logic;
      MPMC_Clk_Mem_2x_CE90 : in std_logic;
      MPMC_Clk_Rd_Base : in std_logic;
      MPMC_Clk_Mem_2x_bufpll_o : out std_logic;
      MPMC_Clk_Mem_2x_180_bufpll_o : out std_logic;
      MPMC_Clk_Mem_2x_CE0_bufpll_o : out std_logic;
      MPMC_Clk_Mem_2x_CE90_bufpll_o : out std_logic;
      MPMC_PLL_Lock_bufpll_o : out std_logic;
      MPMC_PLL_Lock : in std_logic;
      MPMC_Idelayctrl_Rdy_I : in std_logic;
      MPMC_Idelayctrl_Rdy_O : out std_logic;
      MPMC_InitDone : out std_logic;
      MPMC_ECC_Intr : out std_logic;
      MPMC_DCM_PSEN : out std_logic;
      MPMC_DCM_PSINCDEC : out std_logic;
      MPMC_DCM_PSDONE : in std_logic;
      MPMC_MCB_DRP_Clk : in std_logic;
      SDRAM_Clk : out std_logic_vector(0 to 0);
      SDRAM_CE : out std_logic_vector(0 to 0);
      SDRAM_CS_n : out std_logic_vector(0 to 0);
      SDRAM_RAS_n : out std_logic;
      SDRAM_CAS_n : out std_logic;
      SDRAM_WE_n : out std_logic;
      SDRAM_BankAddr : out std_logic_vector(1 downto 0);
      SDRAM_Addr : out std_logic_vector(11 downto 0);
      SDRAM_DQ : inout std_logic_vector(31 downto 0);
      SDRAM_DM : out std_logic_vector(3 downto 0);
      DDR_Clk : out std_logic_vector(0 to 0);
      DDR_Clk_n : out std_logic_vector(0 to 0);
      DDR_CE : out std_logic_vector(0 to 0);
      DDR_CS_n : out std_logic_vector(0 to 0);
      DDR_RAS_n : out std_logic;
      DDR_CAS_n : out std_logic;
      DDR_WE_n : out std_logic;
      DDR_BankAddr : out std_logic_vector(1 downto 0);
      DDR_Addr : out std_logic_vector(11 downto 0);
      DDR_DQ : inout std_logic_vector(31 downto 0);
      DDR_DM : out std_logic_vector(3 downto 0);
      DDR_DQS : inout std_logic_vector(3 downto 0);
      DDR_DQS_Div_O : out std_logic;
      DDR_DQS_Div_I : in std_logic;
      DDR2_Clk : out std_logic_vector(0 to 0);
      DDR2_Clk_n : out std_logic_vector(0 to 0);
      DDR2_CE : out std_logic_vector(0 to 0);
      DDR2_CS_n : out std_logic_vector(0 to 0);
      DDR2_ODT : out std_logic_vector(0 to 0);
      DDR2_RAS_n : out std_logic;
      DDR2_CAS_n : out std_logic;
      DDR2_WE_n : out std_logic;
      DDR2_BankAddr : out std_logic_vector(1 downto 0);
      DDR2_Addr : out std_logic_vector(11 downto 0);
      DDR2_DQ : inout std_logic_vector(31 downto 0);
      DDR2_DM : out std_logic_vector(3 downto 0);
      DDR2_DQS : inout std_logic_vector(3 downto 0);
      DDR2_DQS_n : inout std_logic_vector(3 downto 0);
      DDR2_DQS_Div_O : out std_logic;
      DDR2_DQS_Div_I : in std_logic;
      DDR3_Clk : out std_logic_vector(0 to 0);
      DDR3_Clk_n : out std_logic_vector(0 to 0);
      DDR3_CE : out std_logic_vector(0 to 0);
      DDR3_CS_n : out std_logic_vector(0 to 0);
      DDR3_ODT : out std_logic_vector(0 to 0);
      DDR3_RAS_n : out std_logic;
      DDR3_CAS_n : out std_logic;
      DDR3_WE_n : out std_logic;
      DDR3_BankAddr : out std_logic_vector(1 downto 0);
      DDR3_Addr : out std_logic_vector(11 downto 0);
      DDR3_DQ : inout std_logic_vector(31 downto 0);
      DDR3_DM : out std_logic_vector(3 downto 0);
      DDR3_Reset_n : out std_logic;
      DDR3_DQS : inout std_logic_vector(3 downto 0);
      DDR3_DQS_n : inout std_logic_vector(3 downto 0);
      mcbx_dram_addr : out std_logic_vector(11 downto 0);
      mcbx_dram_ba : out std_logic_vector(1 downto 0);
      mcbx_dram_ras_n : out std_logic;
      mcbx_dram_cas_n : out std_logic;
      mcbx_dram_we_n : out std_logic;
      mcbx_dram_cke : out std_logic;
      mcbx_dram_clk : out std_logic;
      mcbx_dram_clk_n : out std_logic;
      mcbx_dram_dq : inout std_logic_vector(31 downto 0);
      mcbx_dram_dqs : inout std_logic;
      mcbx_dram_dqs_n : inout std_logic;
      mcbx_dram_udqs : inout std_logic;
      mcbx_dram_udqs_n : inout std_logic;
      mcbx_dram_udm : out std_logic;
      mcbx_dram_ldm : out std_logic;
      mcbx_dram_odt : out std_logic;
      mcbx_dram_ddr3_rst : out std_logic;
      selfrefresh_enter : in std_logic;
      selfrefresh_mode : out std_logic;
      calib_recal : in std_logic;
      rzq : inout std_logic;
      zio : inout std_logic
    );
  end component;

  component dcm_2_wrapper is
    port (
      RST : in std_logic;
      CLKIN : in std_logic;
      CLKFB : in std_logic;
      PSEN : in std_logic;
      PSINCDEC : in std_logic;
      PSCLK : in std_logic;
      DSSEN : in std_logic;
      CLK0 : out std_logic;
      CLK90 : out std_logic;
      CLK180 : out std_logic;
      CLK270 : out std_logic;
      CLKDV : out std_logic;
      CLK2X : out std_logic;
      CLK2X180 : out std_logic;
      CLKFX : out std_logic;
      CLKFX180 : out std_logic;
      STATUS : out std_logic_vector(7 downto 0);
      LOCKED : out std_logic;
      PSDONE : out std_logic
    );
  end component;

  component xps_gpio_1_wrapper is
    port (
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 0);
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_lockErr : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrBTerm : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 1);
      Sl_MWrErr : out std_logic_vector(0 to 1);
      Sl_MRdErr : out std_logic_vector(0 to 1);
      Sl_MIRQ : out std_logic_vector(0 to 1);
      IP2INTC_Irpt : out std_logic;
      GPIO_IO_I : in std_logic_vector(0 to 4);
      GPIO_IO_O : out std_logic_vector(0 to 4);
      GPIO_IO_T : out std_logic_vector(0 to 4);
      GPIO2_IO_I : in std_logic_vector(0 to 15);
      GPIO2_IO_O : out std_logic_vector(0 to 15);
      GPIO2_IO_T : out std_logic_vector(0 to 15)
    );
  end component;

  component debug_module_0_wrapper is
    port (
      Interrupt : out std_logic;
      Debug_SYS_Rst : out std_logic;
      Ext_BRK : out std_logic;
      Ext_NM_BRK : out std_logic;
      S_AXI_ACLK : in std_logic;
      S_AXI_ARESETN : in std_logic;
      S_AXI_AWADDR : in std_logic_vector(31 downto 0);
      S_AXI_AWVALID : in std_logic;
      S_AXI_AWREADY : out std_logic;
      S_AXI_WDATA : in std_logic_vector(31 downto 0);
      S_AXI_WSTRB : in std_logic_vector(3 downto 0);
      S_AXI_WVALID : in std_logic;
      S_AXI_WREADY : out std_logic;
      S_AXI_BRESP : out std_logic_vector(1 downto 0);
      S_AXI_BVALID : out std_logic;
      S_AXI_BREADY : in std_logic;
      S_AXI_ARADDR : in std_logic_vector(31 downto 0);
      S_AXI_ARVALID : in std_logic;
      S_AXI_ARREADY : out std_logic;
      S_AXI_RDATA : out std_logic_vector(31 downto 0);
      S_AXI_RRESP : out std_logic_vector(1 downto 0);
      S_AXI_RVALID : out std_logic;
      S_AXI_RREADY : in std_logic;
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 1);
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_lockErr : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrBTerm : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 3);
      Sl_MWrErr : out std_logic_vector(0 to 3);
      Sl_MRdErr : out std_logic_vector(0 to 3);
      Sl_MIRQ : out std_logic_vector(0 to 3);
      Dbg_Clk_0 : out std_logic;
      Dbg_TDI_0 : out std_logic;
      Dbg_TDO_0 : in std_logic;
      Dbg_Reg_En_0 : out std_logic_vector(0 to 7);
      Dbg_Capture_0 : out std_logic;
      Dbg_Shift_0 : out std_logic;
      Dbg_Update_0 : out std_logic;
      Dbg_Rst_0 : out std_logic;
      Dbg_Clk_1 : out std_logic;
      Dbg_TDI_1 : out std_logic;
      Dbg_TDO_1 : in std_logic;
      Dbg_Reg_En_1 : out std_logic_vector(0 to 7);
      Dbg_Capture_1 : out std_logic;
      Dbg_Shift_1 : out std_logic;
      Dbg_Update_1 : out std_logic;
      Dbg_Rst_1 : out std_logic;
      Dbg_Clk_2 : out std_logic;
      Dbg_TDI_2 : out std_logic;
      Dbg_TDO_2 : in std_logic;
      Dbg_Reg_En_2 : out std_logic_vector(0 to 7);
      Dbg_Capture_2 : out std_logic;
      Dbg_Shift_2 : out std_logic;
      Dbg_Update_2 : out std_logic;
      Dbg_Rst_2 : out std_logic;
      Dbg_Clk_3 : out std_logic;
      Dbg_TDI_3 : out std_logic;
      Dbg_TDO_3 : in std_logic;
      Dbg_Reg_En_3 : out std_logic_vector(0 to 7);
      Dbg_Capture_3 : out std_logic;
      Dbg_Shift_3 : out std_logic;
      Dbg_Update_3 : out std_logic;
      Dbg_Rst_3 : out std_logic;
      Dbg_Clk_4 : out std_logic;
      Dbg_TDI_4 : out std_logic;
      Dbg_TDO_4 : in std_logic;
      Dbg_Reg_En_4 : out std_logic_vector(0 to 7);
      Dbg_Capture_4 : out std_logic;
      Dbg_Shift_4 : out std_logic;
      Dbg_Update_4 : out std_logic;
      Dbg_Rst_4 : out std_logic;
      Dbg_Clk_5 : out std_logic;
      Dbg_TDI_5 : out std_logic;
      Dbg_TDO_5 : in std_logic;
      Dbg_Reg_En_5 : out std_logic_vector(0 to 7);
      Dbg_Capture_5 : out std_logic;
      Dbg_Shift_5 : out std_logic;
      Dbg_Update_5 : out std_logic;
      Dbg_Rst_5 : out std_logic;
      Dbg_Clk_6 : out std_logic;
      Dbg_TDI_6 : out std_logic;
      Dbg_TDO_6 : in std_logic;
      Dbg_Reg_En_6 : out std_logic_vector(0 to 7);
      Dbg_Capture_6 : out std_logic;
      Dbg_Shift_6 : out std_logic;
      Dbg_Update_6 : out std_logic;
      Dbg_Rst_6 : out std_logic;
      Dbg_Clk_7 : out std_logic;
      Dbg_TDI_7 : out std_logic;
      Dbg_TDO_7 : in std_logic;
      Dbg_Reg_En_7 : out std_logic_vector(0 to 7);
      Dbg_Capture_7 : out std_logic;
      Dbg_Shift_7 : out std_logic;
      Dbg_Update_7 : out std_logic;
      Dbg_Rst_7 : out std_logic;
      bscan_tdi : out std_logic;
      bscan_reset : out std_logic;
      bscan_shift : out std_logic;
      bscan_update : out std_logic;
      bscan_capture : out std_logic;
      bscan_sel1 : out std_logic;
      bscan_drck1 : out std_logic;
      bscan_tdo1 : in std_logic;
      Ext_JTAG_DRCK : out std_logic;
      Ext_JTAG_RESET : out std_logic;
      Ext_JTAG_SEL : out std_logic;
      Ext_JTAG_CAPTURE : out std_logic;
      Ext_JTAG_SHIFT : out std_logic;
      Ext_JTAG_UPDATE : out std_logic;
      Ext_JTAG_TDI : out std_logic;
      Ext_JTAG_TDO : in std_logic
    );
  end component;

  component gameboard_uart_wrapper is
    port (
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 0);
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_lockErr : in std_logic;
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 1);
      Sl_MWrErr : out std_logic_vector(0 to 1);
      Sl_MRdErr : out std_logic_vector(0 to 1);
      Sl_wrBTerm : out std_logic;
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdBTerm : out std_logic;
      Sl_MIRQ : out std_logic_vector(0 to 1);
      RX : in std_logic;
      TX : out std_logic;
      Interrupt : out std_logic
    );
  end component;

  component xps_intc_1_wrapper is
    port (
      SPLB_Clk : in std_logic;
      SPLB_Rst : in std_logic;
      PLB_ABus : in std_logic_vector(0 to 31);
      PLB_PAValid : in std_logic;
      PLB_masterID : in std_logic_vector(0 to 1);
      PLB_RNW : in std_logic;
      PLB_BE : in std_logic_vector(0 to 7);
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_wrDBus : in std_logic_vector(0 to 63);
      PLB_UABus : in std_logic_vector(0 to 31);
      PLB_SAValid : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_abort : in std_logic;
      PLB_busLock : in std_logic;
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_lockErr : in std_logic;
      PLB_wrBurst : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_wrPendReq : in std_logic;
      PLB_rdPendReq : in std_logic;
      PLB_wrPendPri : in std_logic_vector(0 to 1);
      PLB_rdPendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      PLB_TAttribute : in std_logic_vector(0 to 15);
      Sl_addrAck : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      Sl_rearbitrate : out std_logic;
      Sl_wrDAck : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdDAck : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 3);
      Sl_MWrErr : out std_logic_vector(0 to 3);
      Sl_MRdErr : out std_logic_vector(0 to 3);
      Sl_wrBTerm : out std_logic;
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      Sl_rdBTerm : out std_logic;
      Sl_MIRQ : out std_logic_vector(0 to 3);
      Intr : in std_logic_vector(1 downto 0);
      Irq : out std_logic
    );
  end component;

  component IOBUF is
    port (
      I : in std_logic;
      IO : inout std_logic;
      O : out std_logic;
      T : in std_logic
    );
  end component;

  -- Internal signals

  signal DCM_1_lock : std_logic;
  signal Dcm_all_locked : std_logic;
  signal cam0_sdata_I : std_logic;
  signal cam0_sdata_O : std_logic;
  signal cam0_sdata_T : std_logic;
  signal clk_300_0000MHzDCM0 : std_logic;
  signal dcm_clk_s : std_logic;
  signal ext_int_ppc405_0 : std_logic;
  signal ext_int_ppc405_1 : std_logic;
  signal gameboard_uart_Interrupt : std_logic;
  signal gpio_helios_I : std_logic_vector(4 downto 0);
  signal gpio_helios_O : std_logic_vector(4 downto 0);
  signal gpio_helios_T : std_logic_vector(4 downto 0);
  signal gpio_helios_game_I : std_logic_vector(15 downto 0);
  signal gpio_helios_game_O : std_logic_vector(15 downto 0);
  signal gpio_helios_game_T : std_logic_vector(15 downto 0);
  signal gpio_int : std_logic;
  signal mpmc_dcm_psdone : std_logic;
  signal mpmc_dcm_psen : std_logic;
  signal mpmc_dcm_psincdec : std_logic;
  signal net_gnd0 : std_logic;
  signal net_gnd1 : std_logic_vector(0 to 0);
  signal net_gnd2 : std_logic_vector(0 to 1);
  signal net_gnd3 : std_logic_vector(0 to 2);
  signal net_gnd4 : std_logic_vector(0 to 3);
  signal net_gnd6 : std_logic_vector(5 downto 0);
  signal net_gnd8 : std_logic_vector(0 to 7);
  signal net_gnd10 : std_logic_vector(0 to 9);
  signal net_gnd16 : std_logic_vector(0 to 15);
  signal net_gnd30 : std_logic_vector(29 downto 0);
  signal net_gnd32 : std_logic_vector(0 to 31);
  signal net_gnd36 : std_logic_vector(0 to 35);
  signal net_gnd64 : std_logic_vector(0 to 63);
  signal net_gnd128 : std_logic_vector(0 to 127);
  signal net_vcc0 : std_logic;
  signal net_vcc2 : std_logic_vector(0 to 1);
  signal net_vcc4 : std_logic_vector(0 to 3);
  signal pgassign1 : std_logic_vector(0 to 0);
  signal pgassign2 : std_logic_vector(0 to 0);
  signal pgassign3 : std_logic_vector(0 to 0);
  signal pgassign4 : std_logic_vector(3 downto 0);
  signal pgassign5 : std_logic_vector(0 to 1);
  signal pgassign6 : std_logic_vector(1 downto 0);
  signal plb_0_MPLB_Rst : std_logic_vector(0 to 1);
  signal plb_0_M_ABus : std_logic_vector(0 to 63);
  signal plb_0_M_BE : std_logic_vector(0 to 15);
  signal plb_0_M_MSize : std_logic_vector(0 to 3);
  signal plb_0_M_RNW : std_logic_vector(0 to 1);
  signal plb_0_M_TAttribute : std_logic_vector(0 to 31);
  signal plb_0_M_UABus : std_logic_vector(0 to 63);
  signal plb_0_M_abort : std_logic_vector(0 to 1);
  signal plb_0_M_busLock : std_logic_vector(0 to 1);
  signal plb_0_M_lockErr : std_logic_vector(0 to 1);
  signal plb_0_M_priority : std_logic_vector(0 to 3);
  signal plb_0_M_rdBurst : std_logic_vector(0 to 1);
  signal plb_0_M_request : std_logic_vector(0 to 1);
  signal plb_0_M_size : std_logic_vector(0 to 7);
  signal plb_0_M_type : std_logic_vector(0 to 5);
  signal plb_0_M_wrBurst : std_logic_vector(0 to 1);
  signal plb_0_M_wrDBus : std_logic_vector(0 to 127);
  signal plb_0_PLB_ABus : std_logic_vector(0 to 31);
  signal plb_0_PLB_BE : std_logic_vector(0 to 7);
  signal plb_0_PLB_MAddrAck : std_logic_vector(0 to 1);
  signal plb_0_PLB_MBusy : std_logic_vector(0 to 1);
  signal plb_0_PLB_MRdBTerm : std_logic_vector(0 to 1);
  signal plb_0_PLB_MRdDAck : std_logic_vector(0 to 1);
  signal plb_0_PLB_MRdDBus : std_logic_vector(0 to 127);
  signal plb_0_PLB_MRdErr : std_logic_vector(0 to 1);
  signal plb_0_PLB_MRdWdAddr : std_logic_vector(0 to 7);
  signal plb_0_PLB_MRearbitrate : std_logic_vector(0 to 1);
  signal plb_0_PLB_MSSize : std_logic_vector(0 to 3);
  signal plb_0_PLB_MSize : std_logic_vector(0 to 1);
  signal plb_0_PLB_MTimeout : std_logic_vector(0 to 1);
  signal plb_0_PLB_MWrBTerm : std_logic_vector(0 to 1);
  signal plb_0_PLB_MWrDAck : std_logic_vector(0 to 1);
  signal plb_0_PLB_MWrErr : std_logic_vector(0 to 1);
  signal plb_0_PLB_PAValid : std_logic;
  signal plb_0_PLB_RNW : std_logic;
  signal plb_0_PLB_SAValid : std_logic;
  signal plb_0_PLB_TAttribute : std_logic_vector(0 to 15);
  signal plb_0_PLB_UABus : std_logic_vector(0 to 31);
  signal plb_0_PLB_abort : std_logic;
  signal plb_0_PLB_busLock : std_logic;
  signal plb_0_PLB_lockErr : std_logic;
  signal plb_0_PLB_masterID : std_logic_vector(0 to 0);
  signal plb_0_PLB_rdBurst : std_logic;
  signal plb_0_PLB_rdPendPri : std_logic_vector(0 to 1);
  signal plb_0_PLB_rdPendReq : std_logic;
  signal plb_0_PLB_rdPrim : std_logic_vector(0 to 10);
  signal plb_0_PLB_reqPri : std_logic_vector(0 to 1);
  signal plb_0_PLB_size : std_logic_vector(0 to 3);
  signal plb_0_PLB_type : std_logic_vector(0 to 2);
  signal plb_0_PLB_wrBurst : std_logic;
  signal plb_0_PLB_wrDBus : std_logic_vector(0 to 63);
  signal plb_0_PLB_wrPendPri : std_logic_vector(0 to 1);
  signal plb_0_PLB_wrPendReq : std_logic;
  signal plb_0_PLB_wrPrim : std_logic_vector(0 to 10);
  signal plb_0_SPLB_Rst : std_logic_vector(0 to 10);
  signal plb_0_Sl_MBusy : std_logic_vector(0 to 21);
  signal plb_0_Sl_MIRQ : std_logic_vector(0 to 21);
  signal plb_0_Sl_MRdErr : std_logic_vector(0 to 21);
  signal plb_0_Sl_MWrErr : std_logic_vector(0 to 21);
  signal plb_0_Sl_SSize : std_logic_vector(0 to 21);
  signal plb_0_Sl_addrAck : std_logic_vector(0 to 10);
  signal plb_0_Sl_rdBTerm : std_logic_vector(0 to 10);
  signal plb_0_Sl_rdComp : std_logic_vector(0 to 10);
  signal plb_0_Sl_rdDAck : std_logic_vector(0 to 10);
  signal plb_0_Sl_rdDBus : std_logic_vector(0 to 703);
  signal plb_0_Sl_rdWdAddr : std_logic_vector(0 to 43);
  signal plb_0_Sl_rearbitrate : std_logic_vector(0 to 10);
  signal plb_0_Sl_wait : std_logic_vector(0 to 10);
  signal plb_0_Sl_wrBTerm : std_logic_vector(0 to 10);
  signal plb_0_Sl_wrComp : std_logic_vector(0 to 10);
  signal plb_0_Sl_wrDAck : std_logic_vector(0 to 10);
  signal plb_1_MPLB_Rst : std_logic_vector(0 to 3);
  signal plb_1_M_ABus : std_logic_vector(0 to 127);
  signal plb_1_M_BE : std_logic_vector(0 to 31);
  signal plb_1_M_MSize : std_logic_vector(0 to 7);
  signal plb_1_M_RNW : std_logic_vector(0 to 3);
  signal plb_1_M_TAttribute : std_logic_vector(0 to 63);
  signal plb_1_M_UABus : std_logic_vector(0 to 127);
  signal plb_1_M_abort : std_logic_vector(0 to 3);
  signal plb_1_M_busLock : std_logic_vector(0 to 3);
  signal plb_1_M_lockErr : std_logic_vector(0 to 3);
  signal plb_1_M_priority : std_logic_vector(0 to 7);
  signal plb_1_M_rdBurst : std_logic_vector(0 to 3);
  signal plb_1_M_request : std_logic_vector(0 to 3);
  signal plb_1_M_size : std_logic_vector(0 to 15);
  signal plb_1_M_type : std_logic_vector(0 to 11);
  signal plb_1_M_wrBurst : std_logic_vector(0 to 3);
  signal plb_1_M_wrDBus : std_logic_vector(0 to 255);
  signal plb_1_PLB_ABus : std_logic_vector(0 to 31);
  signal plb_1_PLB_BE : std_logic_vector(0 to 7);
  signal plb_1_PLB_MAddrAck : std_logic_vector(0 to 3);
  signal plb_1_PLB_MBusy : std_logic_vector(0 to 3);
  signal plb_1_PLB_MIRQ : std_logic_vector(0 to 3);
  signal plb_1_PLB_MRdBTerm : std_logic_vector(0 to 3);
  signal plb_1_PLB_MRdDAck : std_logic_vector(0 to 3);
  signal plb_1_PLB_MRdDBus : std_logic_vector(0 to 255);
  signal plb_1_PLB_MRdErr : std_logic_vector(0 to 3);
  signal plb_1_PLB_MRdWdAddr : std_logic_vector(0 to 15);
  signal plb_1_PLB_MRearbitrate : std_logic_vector(0 to 3);
  signal plb_1_PLB_MSSize : std_logic_vector(0 to 7);
  signal plb_1_PLB_MSize : std_logic_vector(0 to 1);
  signal plb_1_PLB_MTimeout : std_logic_vector(0 to 3);
  signal plb_1_PLB_MWrBTerm : std_logic_vector(0 to 3);
  signal plb_1_PLB_MWrDAck : std_logic_vector(0 to 3);
  signal plb_1_PLB_MWrErr : std_logic_vector(0 to 3);
  signal plb_1_PLB_PAValid : std_logic;
  signal plb_1_PLB_RNW : std_logic;
  signal plb_1_PLB_SAValid : std_logic;
  signal plb_1_PLB_TAttribute : std_logic_vector(0 to 15);
  signal plb_1_PLB_UABus : std_logic_vector(0 to 31);
  signal plb_1_PLB_abort : std_logic;
  signal plb_1_PLB_busLock : std_logic;
  signal plb_1_PLB_lockErr : std_logic;
  signal plb_1_PLB_masterID : std_logic_vector(0 to 1);
  signal plb_1_PLB_rdBurst : std_logic;
  signal plb_1_PLB_rdPendPri : std_logic_vector(0 to 1);
  signal plb_1_PLB_rdPendReq : std_logic;
  signal plb_1_PLB_rdPrim : std_logic_vector(0 to 6);
  signal plb_1_PLB_reqPri : std_logic_vector(0 to 1);
  signal plb_1_PLB_size : std_logic_vector(0 to 3);
  signal plb_1_PLB_type : std_logic_vector(0 to 2);
  signal plb_1_PLB_wrBurst : std_logic;
  signal plb_1_PLB_wrDBus : std_logic_vector(0 to 63);
  signal plb_1_PLB_wrPendPri : std_logic_vector(0 to 1);
  signal plb_1_PLB_wrPendReq : std_logic;
  signal plb_1_PLB_wrPrim : std_logic_vector(0 to 6);
  signal plb_1_SPLB_Rst : std_logic_vector(0 to 6);
  signal plb_1_Sl_MBusy : std_logic_vector(0 to 27);
  signal plb_1_Sl_MIRQ : std_logic_vector(0 to 27);
  signal plb_1_Sl_MRdErr : std_logic_vector(0 to 27);
  signal plb_1_Sl_MWrErr : std_logic_vector(0 to 27);
  signal plb_1_Sl_SSize : std_logic_vector(0 to 13);
  signal plb_1_Sl_addrAck : std_logic_vector(0 to 6);
  signal plb_1_Sl_rdBTerm : std_logic_vector(0 to 6);
  signal plb_1_Sl_rdComp : std_logic_vector(0 to 6);
  signal plb_1_Sl_rdDAck : std_logic_vector(0 to 6);
  signal plb_1_Sl_rdDBus : std_logic_vector(0 to 447);
  signal plb_1_Sl_rdWdAddr : std_logic_vector(0 to 27);
  signal plb_1_Sl_rearbitrate : std_logic_vector(0 to 6);
  signal plb_1_Sl_wait : std_logic_vector(0 to 6);
  signal plb_1_Sl_wrBTerm : std_logic_vector(0 to 6);
  signal plb_1_Sl_wrComp : std_logic_vector(0 to 6);
  signal plb_1_Sl_wrDAck : std_logic_vector(0 to 6);
  signal plb_clk_100MHz : std_logic;
  signal plb_vision_0_Interrupt : std_logic;
  signal ppc405_0_jtagppc_bus_C405JTGTDO : std_logic;
  signal ppc405_0_jtagppc_bus_C405JTGTDOEN : std_logic;
  signal ppc405_0_jtagppc_bus_JTGC405TCK : std_logic;
  signal ppc405_0_jtagppc_bus_JTGC405TDI : std_logic;
  signal ppc405_0_jtagppc_bus_JTGC405TMS : std_logic;
  signal ppc405_0_jtagppc_bus_JTGC405TRSTNEG : std_logic;
  signal ppc405_1_jtagppc_bus_C405JTGTDO : std_logic;
  signal ppc405_1_jtagppc_bus_C405JTGTDOEN : std_logic;
  signal ppc405_1_jtagppc_bus_JTGC405TCK : std_logic;
  signal ppc405_1_jtagppc_bus_JTGC405TDI : std_logic;
  signal ppc405_1_jtagppc_bus_JTGC405TMS : std_logic;
  signal ppc405_1_jtagppc_bus_JTGC405TRSTNEG : std_logic;
  signal ppc_reset_bus_0_Chip_Reset_Req : std_logic;
  signal ppc_reset_bus_0_Core_Reset_Req : std_logic;
  signal ppc_reset_bus_0_RstcPPCresetcore : std_logic;
  signal ppc_reset_bus_0_RstcPPCresetsys : std_logic;
  signal ppc_reset_bus_0_RstsPPCresetchip : std_logic;
  signal ppc_reset_bus_0_System_Reset_Req : std_logic;
  signal ppc_reset_bus_1_Chip_Reset_Req : std_logic;
  signal ppc_reset_bus_1_Core_Reset_Req : std_logic;
  signal ppc_reset_bus_1_RstcPPCresetcore : std_logic;
  signal ppc_reset_bus_1_RstcPPCresetsys : std_logic;
  signal ppc_reset_bus_1_RstsPPCresetchip : std_logic;
  signal ppc_reset_bus_1_System_Reset_Req : std_logic;
  signal sdram_clk_phy : std_logic;
  signal serial_int : std_logic;
  signal sys_bus_reset : std_logic_vector(0 to 0);
  signal sys_rst_s : std_logic;
  signal usb_dcm_locked : std_logic;
  signal usb_dcm_reset : std_logic;
  signal usb_fd_I : std_logic_vector(15 downto 0);
  signal usb_fd_O : std_logic_vector(15 downto 0);
  signal usb_fd_T : std_logic_vector(15 downto 0);
  signal usb_if_clk_shift : std_logic;
  signal usb_int : std_logic;
  signal wireless_int : std_logic;
  signal xps_bram_if_cntlr_0_port_BRAM_Addr : std_logic_vector(0 to 31);
  signal xps_bram_if_cntlr_0_port_BRAM_Clk : std_logic;
  signal xps_bram_if_cntlr_0_port_BRAM_Din : std_logic_vector(0 to 63);
  signal xps_bram_if_cntlr_0_port_BRAM_Dout : std_logic_vector(0 to 63);
  signal xps_bram_if_cntlr_0_port_BRAM_EN : std_logic;
  signal xps_bram_if_cntlr_0_port_BRAM_Rst : std_logic;
  signal xps_bram_if_cntlr_0_port_BRAM_WEN : std_logic_vector(0 to 7);
  signal xps_bram_if_cntlr_1_port_BRAM_Addr : std_logic_vector(0 to 31);
  signal xps_bram_if_cntlr_1_port_BRAM_Clk : std_logic;
  signal xps_bram_if_cntlr_1_port_BRAM_Din : std_logic_vector(0 to 63);
  signal xps_bram_if_cntlr_1_port_BRAM_Dout : std_logic_vector(0 to 63);
  signal xps_bram_if_cntlr_1_port_BRAM_EN : std_logic;
  signal xps_bram_if_cntlr_1_port_BRAM_Rst : std_logic;
  signal xps_bram_if_cntlr_1_port_BRAM_WEN : std_logic_vector(0 to 7);
  signal xps_mch_emc_0_Mem_A : std_logic_vector(0 to 31);
  signal xps_mch_emc_0_Mem_CEN : std_logic_vector(0 to 0);
  signal xps_mch_emc_0_Mem_DQ_I : std_logic_vector(0 to 15);
  signal xps_mch_emc_0_Mem_DQ_O : std_logic_vector(0 to 15);
  signal xps_mch_emc_0_Mem_DQ_T : std_logic_vector(0 to 15);
  signal xps_mch_emc_0_Mem_OEN : std_logic_vector(0 to 0);
  signal xps_mch_emc_0_Mem_QWEN : std_logic_vector(0 to 1);
  signal xps_mch_emc_0_Mem_RPN : std_logic;
  signal xps_mch_emc_sram_Mem_A : std_logic_vector(0 to 31);
  signal xps_mch_emc_sram_Mem_ADV_LDN : std_logic;
  signal xps_mch_emc_sram_Mem_BEN : std_logic_vector(0 to 3);
  signal xps_mch_emc_sram_Mem_CEN : std_logic_vector(0 to 0);
  signal xps_mch_emc_sram_Mem_DQ_I : std_logic_vector(0 to 31);
  signal xps_mch_emc_sram_Mem_DQ_O : std_logic_vector(0 to 31);
  signal xps_mch_emc_sram_Mem_DQ_T : std_logic_vector(0 to 31);
  signal xps_mch_emc_sram_Mem_LBON : std_logic;
  signal xps_mch_emc_sram_Mem_OEN : std_logic_vector(0 to 0);
  signal xps_mch_emc_sram_Mem_WEN : std_logic;

  attribute BOX_TYPE : STRING;
  attribute BOX_TYPE of ppc405_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of ppc405_1_wrapper : component is "user_black_box";
  attribute BOX_TYPE of plb_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of plb_1_wrapper : component is "user_black_box";
  attribute BOX_TYPE of xps_bram_if_cntlr_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of xps_bram_if_cntlr_1_wrapper : component is "user_black_box";
  attribute BOX_TYPE of plb_bram_if_cntlr_0_bram_wrapper : component is "user_black_box";
  attribute BOX_TYPE of plb_bram_if_cntlr_1_bram_wrapper : component is "user_black_box";
  attribute BOX_TYPE of serial_uart_wrapper : component is "user_black_box";
  attribute BOX_TYPE of wireless_uart_wrapper : component is "user_black_box";
  attribute BOX_TYPE of clock_generator_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of jtagppc_cntlr_inst_wrapper : component is "user_black_box";
  attribute BOX_TYPE of proc_sys_reset_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of xps_intc_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of plb_quad_encoder_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of plb_pwm_ctrl_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of xps_mch_emc_flash_wrapper : component is "user_black_box";
  attribute BOX_TYPE of xps_mch_emc_sram_wrapper : component is "user_black_box";
  attribute BOX_TYPE of dcm_module_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of plb_usb_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of plb_vision_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of mpmc_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of dcm_2_wrapper : component is "user_black_box";
  attribute BOX_TYPE of xps_gpio_1_wrapper : component is "user_black_box";
  attribute BOX_TYPE of debug_module_0_wrapper : component is "user_black_box";
  attribute BOX_TYPE of gameboard_uart_wrapper : component is "user_black_box";
  attribute BOX_TYPE of xps_intc_1_wrapper : component is "user_black_box";

begin

  -- Internal assignments

  dcm_clk_s <= fpga_0_clk_1_sys_clk_pin;
  sys_rst_s <= fpga_0_rst_1_sys_rst_pin;
  xps_mch_emc_flash_Mem_A_pin <= xps_mch_emc_0_Mem_A;
  xps_mch_emc_flash_Mem_CEN_pin(0 to 0) <= xps_mch_emc_0_Mem_CEN(0 to 0);
  xps_mch_emc_flash_Mem_RPN_pin <= xps_mch_emc_0_Mem_RPN;
  xps_mch_emc_flash_Mem_QWEN_pin <= xps_mch_emc_0_Mem_QWEN;
  xps_mch_emc_flash_Mem_OEN_pin(0 to 0) <= xps_mch_emc_0_Mem_OEN(0 to 0);
  xps_mch_emc_sram_Mem_WEN_pin <= xps_mch_emc_sram_Mem_WEN;
  xps_mch_emc_sram_Mem_A_pin <= xps_mch_emc_sram_Mem_A;
  xps_mch_emc_sram_Mem_CEN_pin(0 to 0) <= xps_mch_emc_sram_Mem_CEN(0 to 0);
  xps_mch_emc_sram_Mem_OEN_pin(0 to 0) <= xps_mch_emc_sram_Mem_OEN(0 to 0);
  xps_mch_emc_sram_Mem_BEN_pin <= xps_mch_emc_sram_Mem_BEN;
  xps_mch_emc_sram_Mem_ADV_LDN_pin <= xps_mch_emc_sram_Mem_ADV_LDN;
  xps_mch_emc_sram_Mem_LBON_pin <= xps_mch_emc_sram_Mem_LBON;
  sram_clk_pin <= plb_clk_100MHz;
  sdram_clk <= pgassign1(0);
  sdram_cke <= pgassign2(0);
  sdram_cs_n <= pgassign3(0);
  pgassign4(3) <= gpio_int;
  pgassign4(2) <= wireless_int;
  pgassign4(1) <= serial_int;
  pgassign4(0) <= gameboard_uart_Interrupt;
  pwm_servo <= pgassign5(0);
  pwm_esc <= pgassign5(1);
  pgassign6(1) <= usb_int;
  pgassign6(0) <= plb_vision_0_Interrupt;
  net_gnd0 <= '0';
  cam_standby <= net_gnd0;
  net_gnd1(0 to 0) <= B"0";
  net_gnd10(0 to 9) <= B"0000000000";
  net_gnd128(0 to 127) <= B"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
  net_gnd16(0 to 15) <= B"0000000000000000";
  net_gnd2(0 to 1) <= B"00";
  net_gnd3(0 to 2) <= B"000";
  net_gnd30(29 downto 0) <= B"000000000000000000000000000000";
  net_gnd32(0 to 31) <= B"00000000000000000000000000000000";
  net_gnd36(0 to 35) <= B"000000000000000000000000000000000000";
  net_gnd4(0 to 3) <= B"0000";
  net_gnd6(5 downto 0) <= B"000000";
  net_gnd64(0 to 63) <= B"0000000000000000000000000000000000000000000000000000000000000000";
  net_gnd8(0 to 7) <= B"00000000";
  net_vcc0 <= '1';
  net_vcc2(0 to 1) <= B"11";
  xps_mch_emc_flash_Mem_BEN_pin <= net_vcc2;
  net_vcc4(0 to 3) <= B"1111";

  ppc405_0 : ppc405_0_wrapper
    port map (
      C405CPMCORESLEEPREQ => open,
      C405CPMMSRCE => open,
      C405CPMMSREE => open,
      C405CPMTIMERIRQ => open,
      C405CPMTIMERRESETREQ => open,
      C405XXXMACHINECHECK => open,
      CPMC405CLOCK => clk_300_0000MHzDCM0,
      CPMC405CORECLKINACTIVE => net_gnd0,
      CPMC405CPUCLKEN => net_vcc0,
      CPMC405JTAGCLKEN => net_vcc0,
      CPMC405TIMERCLKEN => net_vcc0,
      CPMC405TIMERTICK => net_vcc0,
      MCBCPUCLKEN => net_vcc0,
      MCBTIMEREN => net_vcc0,
      MCPPCRST => net_vcc0,
      CPMDCRCLK => net_vcc0,
      CPMFCMCLK => net_vcc0,
      C405RSTCHIPRESETREQ => ppc_reset_bus_0_Chip_Reset_Req,
      C405RSTCORERESETREQ => ppc_reset_bus_0_Core_Reset_Req,
      C405RSTSYSRESETREQ => ppc_reset_bus_0_System_Reset_Req,
      RSTC405RESETCHIP => ppc_reset_bus_0_RstsPPCresetchip,
      RSTC405RESETCORE => ppc_reset_bus_0_RstcPPCresetcore,
      RSTC405RESETSYS => ppc_reset_bus_0_RstcPPCresetsys,
      APUFCMDECODED => open,
      APUFCMDECUDI => open,
      APUFCMDECUDIVALID => open,
      APUFCMENDIAN => open,
      APUFCMFLUSH => open,
      APUFCMINSTRUCTION => open,
      APUFCMINSTRVALID => open,
      APUFCMLOADBYTEEN => open,
      APUFCMLOADDATA => open,
      APUFCMLOADDVALID => open,
      APUFCMOPERANDVALID => open,
      APUFCMRADATA => open,
      APUFCMRBDATA => open,
      APUFCMWRITEBACKOK => open,
      APUFCMXERCA => open,
      FCMAPUCR => net_gnd4,
      FCMAPUDCDCREN => net_gnd0,
      FCMAPUDCDFORCEALIGN => net_gnd0,
      FCMAPUDCDFORCEBESTEERING => net_gnd0,
      FCMAPUDCDFPUOP => net_gnd0,
      FCMAPUDCDGPRWRITE => net_gnd0,
      FCMAPUDCDLDSTBYTE => net_gnd0,
      FCMAPUDCDLDSTDW => net_gnd0,
      FCMAPUDCDLDSTHW => net_gnd0,
      FCMAPUDCDLDSTQW => net_gnd0,
      FCMAPUDCDLDSTWD => net_gnd0,
      FCMAPUDCDLOAD => net_gnd0,
      FCMAPUDCDPRIVOP => net_gnd0,
      FCMAPUDCDRAEN => net_gnd0,
      FCMAPUDCDRBEN => net_gnd0,
      FCMAPUDCDSTORE => net_gnd0,
      FCMAPUDCDTRAPBE => net_gnd0,
      FCMAPUDCDTRAPLE => net_gnd0,
      FCMAPUDCDUPDATE => net_gnd0,
      FCMAPUDCDXERCAEN => net_gnd0,
      FCMAPUDCDXEROVEN => net_gnd0,
      FCMAPUDECODEBUSY => net_gnd0,
      FCMAPUDONE => net_gnd0,
      FCMAPUEXCEPTION => net_gnd0,
      FCMAPUEXEBLOCKINGMCO => net_gnd0,
      FCMAPUEXECRFIELD => net_gnd3,
      FCMAPUEXENONBLOCKINGMCO => net_gnd0,
      FCMAPUINSTRACK => net_gnd0,
      FCMAPULOADWAIT => net_gnd0,
      FCMAPURESULT => net_gnd32,
      FCMAPURESULTVALID => net_gnd0,
      FCMAPUSLEEPNOTREADY => net_gnd0,
      FCMAPUXERCA => net_gnd0,
      FCMAPUXEROV => net_gnd0,
      IPLB0_PLB_Clk => plb_clk_100MHz,
      IPLB0_PLB_Rst => plb_0_MPLB_Rst(1),
      IPLB0_PLB_MBusy => plb_0_PLB_MBusy(1),
      IPLB0_PLB_MRdErr => plb_0_PLB_MRdErr(1),
      IPLB0_PLB_MWrErr => plb_0_PLB_MWrErr(1),
      IPLB0_PLB_MWrBTerm => plb_0_PLB_MWrBTerm(1),
      IPLB0_PLB_MWrDAck => plb_0_PLB_MWrDAck(1),
      IPLB0_PLB_MAddrAck => plb_0_PLB_MAddrAck(1),
      IPLB0_PLB_MRdBTerm => plb_0_PLB_MRdBTerm(1),
      IPLB0_PLB_MRdDAck => plb_0_PLB_MRdDAck(1),
      IPLB0_PLB_MRdDBus => plb_0_PLB_MRdDBus(64 to 127),
      IPLB0_PLB_MRearbitrate => plb_0_PLB_MRearbitrate(1),
      IPLB0_PLB_MSSize => plb_0_PLB_MSSize(2 to 3),
      IPLB0_PLB_MTimeout => plb_0_PLB_MTimeout(1),
      IPLB0_PLB_MRdWdAddr => plb_0_PLB_MRdWdAddr(4 to 7),
      IPLB0_M_ABus => plb_0_M_ABus(32 to 63),
      IPLB0_M_BE => plb_0_M_BE(8 to 15),
      IPLB0_M_MSize => plb_0_M_MSize(2 to 3),
      IPLB0_M_rdBurst => plb_0_M_rdBurst(1),
      IPLB0_M_request => plb_0_M_request(1),
      IPLB0_M_RNW => plb_0_M_RNW(1),
      IPLB0_M_size => plb_0_M_size(4 to 7),
      IPLB0_M_wrBurst => plb_0_M_wrBurst(1),
      IPLB0_M_wrDBus => plb_0_M_wrDBus(64 to 127),
      IPLB0_M_abort => plb_0_M_abort(1),
      IPLB0_M_UABus => plb_0_M_UABus(32 to 63),
      IPLB0_M_busLock => plb_0_M_busLock(1),
      IPLB0_M_lockErr => plb_0_M_lockErr(1),
      IPLB0_M_priority => plb_0_M_priority(2 to 3),
      IPLB0_M_type => plb_0_M_type(3 to 5),
      IPLB0_M_TAttribute => plb_0_M_TAttribute(16 to 31),
      DPLB0_PLB_Clk => plb_clk_100MHz,
      DPLB0_PLB_Rst => plb_0_MPLB_Rst(0),
      DPLB0_PLB_MBusy => plb_0_PLB_MBusy(0),
      DPLB0_PLB_MRdErr => plb_0_PLB_MRdErr(0),
      DPLB0_PLB_MWrErr => plb_0_PLB_MWrErr(0),
      DPLB0_PLB_MWrBTerm => plb_0_PLB_MWrBTerm(0),
      DPLB0_PLB_MWrDAck => plb_0_PLB_MWrDAck(0),
      DPLB0_PLB_MAddrAck => plb_0_PLB_MAddrAck(0),
      DPLB0_PLB_MRdBTerm => plb_0_PLB_MRdBTerm(0),
      DPLB0_PLB_MRdDAck => plb_0_PLB_MRdDAck(0),
      DPLB0_PLB_MRdDBus => plb_0_PLB_MRdDBus(0 to 63),
      DPLB0_PLB_MRearbitrate => plb_0_PLB_MRearbitrate(0),
      DPLB0_PLB_MSSize => plb_0_PLB_MSSize(0 to 1),
      DPLB0_PLB_MTimeout => plb_0_PLB_MTimeout(0),
      DPLB0_PLB_MRdWdAddr => plb_0_PLB_MRdWdAddr(0 to 3),
      DPLB0_M_ABus => plb_0_M_ABus(0 to 31),
      DPLB0_M_BE => plb_0_M_BE(0 to 7),
      DPLB0_M_MSize => plb_0_M_MSize(0 to 1),
      DPLB0_M_rdBurst => plb_0_M_rdBurst(0),
      DPLB0_M_request => plb_0_M_request(0),
      DPLB0_M_RNW => plb_0_M_RNW(0),
      DPLB0_M_size => plb_0_M_size(0 to 3),
      DPLB0_M_wrBurst => plb_0_M_wrBurst(0),
      DPLB0_M_wrDBus => plb_0_M_wrDBus(0 to 63),
      DPLB0_M_abort => plb_0_M_abort(0),
      DPLB0_M_UABus => plb_0_M_UABus(0 to 31),
      DPLB0_M_busLock => plb_0_M_busLock(0),
      DPLB0_M_lockErr => plb_0_M_lockErr(0),
      DPLB0_M_priority => plb_0_M_priority(0 to 1),
      DPLB0_M_type => plb_0_M_type(0 to 2),
      DPLB0_M_TAttribute => plb_0_M_TAttribute(0 to 15),
      IPLB1_PLB_Clk => net_gnd0,
      IPLB1_PLB_Rst => net_gnd0,
      IPLB1_PLB_MBusy => net_gnd0,
      IPLB1_PLB_MRdErr => net_gnd0,
      IPLB1_PLB_MWrErr => net_gnd0,
      IPLB1_PLB_MWrBTerm => net_gnd0,
      IPLB1_PLB_MWrDAck => net_gnd0,
      IPLB1_PLB_MAddrAck => net_gnd0,
      IPLB1_PLB_MRdBTerm => net_gnd0,
      IPLB1_PLB_MRdDAck => net_gnd0,
      IPLB1_PLB_MRdDBus => net_gnd64,
      IPLB1_PLB_MRearbitrate => net_gnd0,
      IPLB1_PLB_MSSize => net_gnd2,
      IPLB1_PLB_MTimeout => net_gnd0,
      IPLB1_PLB_MRdWdAddr => net_gnd4,
      IPLB1_M_ABus => open,
      IPLB1_M_BE => open,
      IPLB1_M_MSize => open,
      IPLB1_M_rdBurst => open,
      IPLB1_M_request => open,
      IPLB1_M_RNW => open,
      IPLB1_M_size => open,
      IPLB1_M_wrBurst => open,
      IPLB1_M_wrDBus => open,
      IPLB1_M_abort => open,
      IPLB1_M_UABus => open,
      IPLB1_M_busLock => open,
      IPLB1_M_lockErr => open,
      IPLB1_M_priority => open,
      IPLB1_M_type => open,
      IPLB1_M_TAttribute => open,
      DPLB1_PLB_Clk => net_gnd0,
      DPLB1_PLB_Rst => net_gnd0,
      DPLB1_PLB_MBusy => net_gnd0,
      DPLB1_PLB_MRdErr => net_gnd0,
      DPLB1_PLB_MWrErr => net_gnd0,
      DPLB1_PLB_MWrBTerm => net_gnd0,
      DPLB1_PLB_MWrDAck => net_gnd0,
      DPLB1_PLB_MAddrAck => net_gnd0,
      DPLB1_PLB_MRdBTerm => net_gnd0,
      DPLB1_PLB_MRdDAck => net_gnd0,
      DPLB1_PLB_MRdDBus => net_gnd64,
      DPLB1_PLB_MRearbitrate => net_gnd0,
      DPLB1_PLB_MSSize => net_gnd2,
      DPLB1_PLB_MTimeout => net_gnd0,
      DPLB1_PLB_MRdWdAddr => net_gnd4,
      DPLB1_M_ABus => open,
      DPLB1_M_BE => open,
      DPLB1_M_MSize => open,
      DPLB1_M_rdBurst => open,
      DPLB1_M_request => open,
      DPLB1_M_RNW => open,
      DPLB1_M_size => open,
      DPLB1_M_wrBurst => open,
      DPLB1_M_wrDBus => open,
      DPLB1_M_abort => open,
      DPLB1_M_UABus => open,
      DPLB1_M_busLock => open,
      DPLB1_M_lockErr => open,
      DPLB1_M_priority => open,
      DPLB1_M_type => open,
      DPLB1_M_TAttribute => open,
      BRAMDSOCMCLK => net_vcc0,
      BRAMDSOCMRDDBUS => net_gnd32,
      DSARCVALUE => net_gnd8,
      DSCNTLVALUE => net_gnd8,
      DSOCMBRAMABUS => open,
      DSOCMBRAMBYTEWRITE => open,
      DSOCMBRAMEN => open,
      DSOCMBRAMWRDBUS => open,
      DSOCMBUSY => open,
      DSOCMRDADDRVALID => open,
      DSOCMWRADDRVALID => open,
      DSOCMRWCOMPLETE => net_vcc0,
      BRAMISOCMCLK => net_vcc0,
      BRAMISOCMRDDBUS => net_gnd64,
      BRAMISOCMDCRRDDBUS => net_gnd32,
      ISARCVALUE => net_gnd8,
      ISCNTLVALUE => net_gnd8,
      ISOCMBRAMEN => open,
      ISOCMBRAMEVENWRITEEN => open,
      ISOCMBRAMODDWRITEEN => open,
      ISOCMBRAMRDABUS => open,
      ISOCMBRAMWRABUS => open,
      ISOCMBRAMWRDBUS => open,
      ISOCMDCRBRAMEVENEN => open,
      ISOCMDCRBRAMODDEN => open,
      ISOCMDCRBRAMRDSELECT => open,
      DCREMACABUS => open,
      DCREMACCLK => open,
      DCREMACDBUS => open,
      DCREMACENABLER => open,
      DCREMACREAD => open,
      DCREMACWRITE => open,
      EMACDCRACK => net_gnd0,
      EMACDCRDBUS => net_gnd32,
      EXTDCRABUS => open,
      EXTDCRDBUSOUT => open,
      EXTDCRREAD => open,
      EXTDCRWRITE => open,
      EXTDCRACK => net_gnd0,
      EXTDCRDBUSIN => net_gnd32,
      EICC405CRITINPUTIRQ => net_gnd0,
      EICC405EXTINPUTIRQ => ext_int_ppc405_0,
      C405JTGCAPTUREDR => open,
      C405JTGEXTEST => open,
      C405JTGPGMOUT => open,
      C405JTGSHIFTDR => open,
      C405JTGTDO => ppc405_0_jtagppc_bus_C405JTGTDO,
      C405JTGTDOEN => ppc405_0_jtagppc_bus_C405JTGTDOEN,
      C405JTGUPDATEDR => open,
      MCBJTAGEN => net_vcc0,
      JTGC405BNDSCANTDO => net_gnd0,
      JTGC405TCK => ppc405_0_jtagppc_bus_JTGC405TCK,
      JTGC405TDI => ppc405_0_jtagppc_bus_JTGC405TDI,
      JTGC405TMS => ppc405_0_jtagppc_bus_JTGC405TMS,
      JTGC405TRSTNEG => ppc405_0_jtagppc_bus_JTGC405TRSTNEG,
      C405DBGMSRWE => open,
      C405DBGSTOPACK => open,
      C405DBGWBCOMPLETE => open,
      C405DBGWBFULL => open,
      C405DBGWBIAR => open,
      DBGC405DEBUGHALT => net_gnd0,
      DBGC405DEBUGHALTNEG => net_vcc0,
      DBGC405EXTBUSHOLDACK => net_gnd0,
      DBGC405UNCONDDEBUGEVENT => net_gnd0,
      C405DBGLOADDATAONAPUDBUS => open,
      C405TRCCYCLE => open,
      C405TRCEVENEXECUTIONSTATUS => open,
      C405TRCODDEXECUTIONSTATUS => open,
      C405TRCTRACESTATUS => open,
      C405TRCTRIGGEREVENTOUT => open,
      C405TRCTRIGGEREVENTTYPE => open,
      TRCC405TRACEDISABLE => net_gnd0,
      TRCC405TRIGGEREVENTIN => net_gnd0
    );

  ppc405_1 : ppc405_1_wrapper
    port map (
      C405CPMCORESLEEPREQ => open,
      C405CPMMSRCE => open,
      C405CPMMSREE => open,
      C405CPMTIMERIRQ => open,
      C405CPMTIMERRESETREQ => open,
      C405XXXMACHINECHECK => open,
      CPMC405CLOCK => clk_300_0000MHzDCM0,
      CPMC405CORECLKINACTIVE => net_gnd0,
      CPMC405CPUCLKEN => net_vcc0,
      CPMC405JTAGCLKEN => net_vcc0,
      CPMC405TIMERCLKEN => net_vcc0,
      CPMC405TIMERTICK => net_vcc0,
      MCBCPUCLKEN => net_vcc0,
      MCBTIMEREN => net_vcc0,
      MCPPCRST => net_vcc0,
      CPMDCRCLK => net_vcc0,
      CPMFCMCLK => net_vcc0,
      C405RSTCHIPRESETREQ => ppc_reset_bus_1_Chip_Reset_Req,
      C405RSTCORERESETREQ => ppc_reset_bus_1_Core_Reset_Req,
      C405RSTSYSRESETREQ => ppc_reset_bus_1_System_Reset_Req,
      RSTC405RESETCHIP => ppc_reset_bus_1_RstsPPCresetchip,
      RSTC405RESETCORE => ppc_reset_bus_1_RstcPPCresetcore,
      RSTC405RESETSYS => ppc_reset_bus_1_RstcPPCresetsys,
      APUFCMDECODED => open,
      APUFCMDECUDI => open,
      APUFCMDECUDIVALID => open,
      APUFCMENDIAN => open,
      APUFCMFLUSH => open,
      APUFCMINSTRUCTION => open,
      APUFCMINSTRVALID => open,
      APUFCMLOADBYTEEN => open,
      APUFCMLOADDATA => open,
      APUFCMLOADDVALID => open,
      APUFCMOPERANDVALID => open,
      APUFCMRADATA => open,
      APUFCMRBDATA => open,
      APUFCMWRITEBACKOK => open,
      APUFCMXERCA => open,
      FCMAPUCR => net_gnd4,
      FCMAPUDCDCREN => net_gnd0,
      FCMAPUDCDFORCEALIGN => net_gnd0,
      FCMAPUDCDFORCEBESTEERING => net_gnd0,
      FCMAPUDCDFPUOP => net_gnd0,
      FCMAPUDCDGPRWRITE => net_gnd0,
      FCMAPUDCDLDSTBYTE => net_gnd0,
      FCMAPUDCDLDSTDW => net_gnd0,
      FCMAPUDCDLDSTHW => net_gnd0,
      FCMAPUDCDLDSTQW => net_gnd0,
      FCMAPUDCDLDSTWD => net_gnd0,
      FCMAPUDCDLOAD => net_gnd0,
      FCMAPUDCDPRIVOP => net_gnd0,
      FCMAPUDCDRAEN => net_gnd0,
      FCMAPUDCDRBEN => net_gnd0,
      FCMAPUDCDSTORE => net_gnd0,
      FCMAPUDCDTRAPBE => net_gnd0,
      FCMAPUDCDTRAPLE => net_gnd0,
      FCMAPUDCDUPDATE => net_gnd0,
      FCMAPUDCDXERCAEN => net_gnd0,
      FCMAPUDCDXEROVEN => net_gnd0,
      FCMAPUDECODEBUSY => net_gnd0,
      FCMAPUDONE => net_gnd0,
      FCMAPUEXCEPTION => net_gnd0,
      FCMAPUEXEBLOCKINGMCO => net_gnd0,
      FCMAPUEXECRFIELD => net_gnd3,
      FCMAPUEXENONBLOCKINGMCO => net_gnd0,
      FCMAPUINSTRACK => net_gnd0,
      FCMAPULOADWAIT => net_gnd0,
      FCMAPURESULT => net_gnd32,
      FCMAPURESULTVALID => net_gnd0,
      FCMAPUSLEEPNOTREADY => net_gnd0,
      FCMAPUXERCA => net_gnd0,
      FCMAPUXEROV => net_gnd0,
      IPLB0_PLB_Clk => plb_clk_100MHz,
      IPLB0_PLB_Rst => plb_1_MPLB_Rst(1),
      IPLB0_PLB_MBusy => plb_1_PLB_MBusy(1),
      IPLB0_PLB_MRdErr => plb_1_PLB_MRdErr(1),
      IPLB0_PLB_MWrErr => plb_1_PLB_MWrErr(1),
      IPLB0_PLB_MWrBTerm => plb_1_PLB_MWrBTerm(1),
      IPLB0_PLB_MWrDAck => plb_1_PLB_MWrDAck(1),
      IPLB0_PLB_MAddrAck => plb_1_PLB_MAddrAck(1),
      IPLB0_PLB_MRdBTerm => plb_1_PLB_MRdBTerm(1),
      IPLB0_PLB_MRdDAck => plb_1_PLB_MRdDAck(1),
      IPLB0_PLB_MRdDBus => plb_1_PLB_MRdDBus(64 to 127),
      IPLB0_PLB_MRearbitrate => plb_1_PLB_MRearbitrate(1),
      IPLB0_PLB_MSSize => plb_1_PLB_MSSize(2 to 3),
      IPLB0_PLB_MTimeout => plb_1_PLB_MTimeout(1),
      IPLB0_PLB_MRdWdAddr => plb_1_PLB_MRdWdAddr(4 to 7),
      IPLB0_M_ABus => plb_1_M_ABus(32 to 63),
      IPLB0_M_BE => plb_1_M_BE(8 to 15),
      IPLB0_M_MSize => plb_1_M_MSize(2 to 3),
      IPLB0_M_rdBurst => plb_1_M_rdBurst(1),
      IPLB0_M_request => plb_1_M_request(1),
      IPLB0_M_RNW => plb_1_M_RNW(1),
      IPLB0_M_size => plb_1_M_size(4 to 7),
      IPLB0_M_wrBurst => plb_1_M_wrBurst(1),
      IPLB0_M_wrDBus => plb_1_M_wrDBus(64 to 127),
      IPLB0_M_abort => plb_1_M_abort(1),
      IPLB0_M_UABus => plb_1_M_UABus(32 to 63),
      IPLB0_M_busLock => plb_1_M_busLock(1),
      IPLB0_M_lockErr => plb_1_M_lockErr(1),
      IPLB0_M_priority => plb_1_M_priority(2 to 3),
      IPLB0_M_type => plb_1_M_type(3 to 5),
      IPLB0_M_TAttribute => plb_1_M_TAttribute(16 to 31),
      DPLB0_PLB_Clk => plb_clk_100MHz,
      DPLB0_PLB_Rst => plb_1_MPLB_Rst(0),
      DPLB0_PLB_MBusy => plb_1_PLB_MBusy(0),
      DPLB0_PLB_MRdErr => plb_1_PLB_MRdErr(0),
      DPLB0_PLB_MWrErr => plb_1_PLB_MWrErr(0),
      DPLB0_PLB_MWrBTerm => plb_1_PLB_MWrBTerm(0),
      DPLB0_PLB_MWrDAck => plb_1_PLB_MWrDAck(0),
      DPLB0_PLB_MAddrAck => plb_1_PLB_MAddrAck(0),
      DPLB0_PLB_MRdBTerm => plb_1_PLB_MRdBTerm(0),
      DPLB0_PLB_MRdDAck => plb_1_PLB_MRdDAck(0),
      DPLB0_PLB_MRdDBus => plb_1_PLB_MRdDBus(0 to 63),
      DPLB0_PLB_MRearbitrate => plb_1_PLB_MRearbitrate(0),
      DPLB0_PLB_MSSize => plb_1_PLB_MSSize(0 to 1),
      DPLB0_PLB_MTimeout => plb_1_PLB_MTimeout(0),
      DPLB0_PLB_MRdWdAddr => plb_1_PLB_MRdWdAddr(0 to 3),
      DPLB0_M_ABus => plb_1_M_ABus(0 to 31),
      DPLB0_M_BE => plb_1_M_BE(0 to 7),
      DPLB0_M_MSize => plb_1_M_MSize(0 to 1),
      DPLB0_M_rdBurst => plb_1_M_rdBurst(0),
      DPLB0_M_request => plb_1_M_request(0),
      DPLB0_M_RNW => plb_1_M_RNW(0),
      DPLB0_M_size => plb_1_M_size(0 to 3),
      DPLB0_M_wrBurst => plb_1_M_wrBurst(0),
      DPLB0_M_wrDBus => plb_1_M_wrDBus(0 to 63),
      DPLB0_M_abort => plb_1_M_abort(0),
      DPLB0_M_UABus => plb_1_M_UABus(0 to 31),
      DPLB0_M_busLock => plb_1_M_busLock(0),
      DPLB0_M_lockErr => plb_1_M_lockErr(0),
      DPLB0_M_priority => plb_1_M_priority(0 to 1),
      DPLB0_M_type => plb_1_M_type(0 to 2),
      DPLB0_M_TAttribute => plb_1_M_TAttribute(0 to 15),
      IPLB1_PLB_Clk => net_gnd0,
      IPLB1_PLB_Rst => net_gnd0,
      IPLB1_PLB_MBusy => net_gnd0,
      IPLB1_PLB_MRdErr => net_gnd0,
      IPLB1_PLB_MWrErr => net_gnd0,
      IPLB1_PLB_MWrBTerm => net_gnd0,
      IPLB1_PLB_MWrDAck => net_gnd0,
      IPLB1_PLB_MAddrAck => net_gnd0,
      IPLB1_PLB_MRdBTerm => net_gnd0,
      IPLB1_PLB_MRdDAck => net_gnd0,
      IPLB1_PLB_MRdDBus => net_gnd64,
      IPLB1_PLB_MRearbitrate => net_gnd0,
      IPLB1_PLB_MSSize => net_gnd2,
      IPLB1_PLB_MTimeout => net_gnd0,
      IPLB1_PLB_MRdWdAddr => net_gnd4,
      IPLB1_M_ABus => open,
      IPLB1_M_BE => open,
      IPLB1_M_MSize => open,
      IPLB1_M_rdBurst => open,
      IPLB1_M_request => open,
      IPLB1_M_RNW => open,
      IPLB1_M_size => open,
      IPLB1_M_wrBurst => open,
      IPLB1_M_wrDBus => open,
      IPLB1_M_abort => open,
      IPLB1_M_UABus => open,
      IPLB1_M_busLock => open,
      IPLB1_M_lockErr => open,
      IPLB1_M_priority => open,
      IPLB1_M_type => open,
      IPLB1_M_TAttribute => open,
      DPLB1_PLB_Clk => net_gnd0,
      DPLB1_PLB_Rst => net_gnd0,
      DPLB1_PLB_MBusy => net_gnd0,
      DPLB1_PLB_MRdErr => net_gnd0,
      DPLB1_PLB_MWrErr => net_gnd0,
      DPLB1_PLB_MWrBTerm => net_gnd0,
      DPLB1_PLB_MWrDAck => net_gnd0,
      DPLB1_PLB_MAddrAck => net_gnd0,
      DPLB1_PLB_MRdBTerm => net_gnd0,
      DPLB1_PLB_MRdDAck => net_gnd0,
      DPLB1_PLB_MRdDBus => net_gnd64,
      DPLB1_PLB_MRearbitrate => net_gnd0,
      DPLB1_PLB_MSSize => net_gnd2,
      DPLB1_PLB_MTimeout => net_gnd0,
      DPLB1_PLB_MRdWdAddr => net_gnd4,
      DPLB1_M_ABus => open,
      DPLB1_M_BE => open,
      DPLB1_M_MSize => open,
      DPLB1_M_rdBurst => open,
      DPLB1_M_request => open,
      DPLB1_M_RNW => open,
      DPLB1_M_size => open,
      DPLB1_M_wrBurst => open,
      DPLB1_M_wrDBus => open,
      DPLB1_M_abort => open,
      DPLB1_M_UABus => open,
      DPLB1_M_busLock => open,
      DPLB1_M_lockErr => open,
      DPLB1_M_priority => open,
      DPLB1_M_type => open,
      DPLB1_M_TAttribute => open,
      BRAMDSOCMCLK => net_vcc0,
      BRAMDSOCMRDDBUS => net_gnd32,
      DSARCVALUE => net_gnd8,
      DSCNTLVALUE => net_gnd8,
      DSOCMBRAMABUS => open,
      DSOCMBRAMBYTEWRITE => open,
      DSOCMBRAMEN => open,
      DSOCMBRAMWRDBUS => open,
      DSOCMBUSY => open,
      DSOCMRDADDRVALID => open,
      DSOCMWRADDRVALID => open,
      DSOCMRWCOMPLETE => net_vcc0,
      BRAMISOCMCLK => net_vcc0,
      BRAMISOCMRDDBUS => net_gnd64,
      BRAMISOCMDCRRDDBUS => net_gnd32,
      ISARCVALUE => net_gnd8,
      ISCNTLVALUE => net_gnd8,
      ISOCMBRAMEN => open,
      ISOCMBRAMEVENWRITEEN => open,
      ISOCMBRAMODDWRITEEN => open,
      ISOCMBRAMRDABUS => open,
      ISOCMBRAMWRABUS => open,
      ISOCMBRAMWRDBUS => open,
      ISOCMDCRBRAMEVENEN => open,
      ISOCMDCRBRAMODDEN => open,
      ISOCMDCRBRAMRDSELECT => open,
      DCREMACABUS => open,
      DCREMACCLK => open,
      DCREMACDBUS => open,
      DCREMACENABLER => open,
      DCREMACREAD => open,
      DCREMACWRITE => open,
      EMACDCRACK => net_gnd0,
      EMACDCRDBUS => net_gnd32,
      EXTDCRABUS => open,
      EXTDCRDBUSOUT => open,
      EXTDCRREAD => open,
      EXTDCRWRITE => open,
      EXTDCRACK => net_gnd0,
      EXTDCRDBUSIN => net_gnd32,
      EICC405CRITINPUTIRQ => net_gnd0,
      EICC405EXTINPUTIRQ => ext_int_ppc405_1,
      C405JTGCAPTUREDR => open,
      C405JTGEXTEST => open,
      C405JTGPGMOUT => open,
      C405JTGSHIFTDR => open,
      C405JTGTDO => ppc405_1_jtagppc_bus_C405JTGTDO,
      C405JTGTDOEN => ppc405_1_jtagppc_bus_C405JTGTDOEN,
      C405JTGUPDATEDR => open,
      MCBJTAGEN => net_vcc0,
      JTGC405BNDSCANTDO => net_gnd0,
      JTGC405TCK => ppc405_1_jtagppc_bus_JTGC405TCK,
      JTGC405TDI => ppc405_1_jtagppc_bus_JTGC405TDI,
      JTGC405TMS => ppc405_1_jtagppc_bus_JTGC405TMS,
      JTGC405TRSTNEG => ppc405_1_jtagppc_bus_JTGC405TRSTNEG,
      C405DBGMSRWE => open,
      C405DBGSTOPACK => open,
      C405DBGWBCOMPLETE => open,
      C405DBGWBFULL => open,
      C405DBGWBIAR => open,
      DBGC405DEBUGHALT => net_gnd0,
      DBGC405DEBUGHALTNEG => net_vcc0,
      DBGC405EXTBUSHOLDACK => net_gnd0,
      DBGC405UNCONDDEBUGEVENT => net_gnd0,
      C405DBGLOADDATAONAPUDBUS => open,
      C405TRCCYCLE => open,
      C405TRCEVENEXECUTIONSTATUS => open,
      C405TRCODDEXECUTIONSTATUS => open,
      C405TRCTRACESTATUS => open,
      C405TRCTRIGGEREVENTOUT => open,
      C405TRCTRIGGEREVENTTYPE => open,
      TRCC405TRACEDISABLE => net_gnd0,
      TRCC405TRIGGEREVENTIN => net_gnd0
    );

  plb_0 : plb_0_wrapper
    port map (
      PLB_Clk => plb_clk_100MHz,
      SYS_Rst => sys_bus_reset(0),
      PLB_Rst => open,
      SPLB_Rst => plb_0_SPLB_Rst,
      MPLB_Rst => plb_0_MPLB_Rst,
      PLB_dcrAck => open,
      PLB_dcrDBus => open,
      DCR_ABus => net_gnd10,
      DCR_DBus => net_gnd32,
      DCR_Read => net_gnd0,
      DCR_Write => net_gnd0,
      M_ABus => plb_0_M_ABus,
      M_UABus => plb_0_M_UABus,
      M_BE => plb_0_M_BE,
      M_RNW => plb_0_M_RNW,
      M_abort => plb_0_M_abort,
      M_busLock => plb_0_M_busLock,
      M_TAttribute => plb_0_M_TAttribute,
      M_lockErr => plb_0_M_lockErr,
      M_MSize => plb_0_M_MSize,
      M_priority => plb_0_M_priority,
      M_rdBurst => plb_0_M_rdBurst,
      M_request => plb_0_M_request,
      M_size => plb_0_M_size,
      M_type => plb_0_M_type,
      M_wrBurst => plb_0_M_wrBurst,
      M_wrDBus => plb_0_M_wrDBus,
      Sl_addrAck => plb_0_Sl_addrAck,
      Sl_MRdErr => plb_0_Sl_MRdErr,
      Sl_MWrErr => plb_0_Sl_MWrErr,
      Sl_MBusy => plb_0_Sl_MBusy,
      Sl_rdBTerm => plb_0_Sl_rdBTerm,
      Sl_rdComp => plb_0_Sl_rdComp,
      Sl_rdDAck => plb_0_Sl_rdDAck,
      Sl_rdDBus => plb_0_Sl_rdDBus,
      Sl_rdWdAddr => plb_0_Sl_rdWdAddr,
      Sl_rearbitrate => plb_0_Sl_rearbitrate,
      Sl_SSize => plb_0_Sl_SSize,
      Sl_wait => plb_0_Sl_wait,
      Sl_wrBTerm => plb_0_Sl_wrBTerm,
      Sl_wrComp => plb_0_Sl_wrComp,
      Sl_wrDAck => plb_0_Sl_wrDAck,
      Sl_MIRQ => plb_0_Sl_MIRQ,
      PLB_MIRQ => open,
      PLB_ABus => plb_0_PLB_ABus,
      PLB_UABus => plb_0_PLB_UABus,
      PLB_BE => plb_0_PLB_BE,
      PLB_MAddrAck => plb_0_PLB_MAddrAck,
      PLB_MTimeout => plb_0_PLB_MTimeout,
      PLB_MBusy => plb_0_PLB_MBusy,
      PLB_MRdErr => plb_0_PLB_MRdErr,
      PLB_MWrErr => plb_0_PLB_MWrErr,
      PLB_MRdBTerm => plb_0_PLB_MRdBTerm,
      PLB_MRdDAck => plb_0_PLB_MRdDAck,
      PLB_MRdDBus => plb_0_PLB_MRdDBus,
      PLB_MRdWdAddr => plb_0_PLB_MRdWdAddr,
      PLB_MRearbitrate => plb_0_PLB_MRearbitrate,
      PLB_MWrBTerm => plb_0_PLB_MWrBTerm,
      PLB_MWrDAck => plb_0_PLB_MWrDAck,
      PLB_MSSize => plb_0_PLB_MSSize,
      PLB_PAValid => plb_0_PLB_PAValid,
      PLB_RNW => plb_0_PLB_RNW,
      PLB_SAValid => plb_0_PLB_SAValid,
      PLB_abort => plb_0_PLB_abort,
      PLB_busLock => plb_0_PLB_busLock,
      PLB_TAttribute => plb_0_PLB_TAttribute,
      PLB_lockErr => plb_0_PLB_lockErr,
      PLB_masterID => plb_0_PLB_masterID(0 to 0),
      PLB_MSize => plb_0_PLB_MSize,
      PLB_rdPendPri => plb_0_PLB_rdPendPri,
      PLB_wrPendPri => plb_0_PLB_wrPendPri,
      PLB_rdPendReq => plb_0_PLB_rdPendReq,
      PLB_wrPendReq => plb_0_PLB_wrPendReq,
      PLB_rdBurst => plb_0_PLB_rdBurst,
      PLB_rdPrim => plb_0_PLB_rdPrim,
      PLB_reqPri => plb_0_PLB_reqPri,
      PLB_size => plb_0_PLB_size,
      PLB_type => plb_0_PLB_type,
      PLB_wrBurst => plb_0_PLB_wrBurst,
      PLB_wrDBus => plb_0_PLB_wrDBus,
      PLB_wrPrim => plb_0_PLB_wrPrim,
      PLB_SaddrAck => open,
      PLB_SMRdErr => open,
      PLB_SMWrErr => open,
      PLB_SMBusy => open,
      PLB_SrdBTerm => open,
      PLB_SrdComp => open,
      PLB_SrdDAck => open,
      PLB_SrdDBus => open,
      PLB_SrdWdAddr => open,
      PLB_Srearbitrate => open,
      PLB_Sssize => open,
      PLB_Swait => open,
      PLB_SwrBTerm => open,
      PLB_SwrComp => open,
      PLB_SwrDAck => open,
      Bus_Error_Det => open
    );

  plb_1 : plb_1_wrapper
    port map (
      PLB_Clk => plb_clk_100MHz,
      SYS_Rst => sys_bus_reset(0),
      PLB_Rst => open,
      SPLB_Rst => plb_1_SPLB_Rst,
      MPLB_Rst => plb_1_MPLB_Rst,
      PLB_dcrAck => open,
      PLB_dcrDBus => open,
      DCR_ABus => net_gnd10,
      DCR_DBus => net_gnd32,
      DCR_Read => net_gnd0,
      DCR_Write => net_gnd0,
      M_ABus => plb_1_M_ABus,
      M_UABus => plb_1_M_UABus,
      M_BE => plb_1_M_BE,
      M_RNW => plb_1_M_RNW,
      M_abort => plb_1_M_abort,
      M_busLock => plb_1_M_busLock,
      M_TAttribute => plb_1_M_TAttribute,
      M_lockErr => plb_1_M_lockErr,
      M_MSize => plb_1_M_MSize,
      M_priority => plb_1_M_priority,
      M_rdBurst => plb_1_M_rdBurst,
      M_request => plb_1_M_request,
      M_size => plb_1_M_size,
      M_type => plb_1_M_type,
      M_wrBurst => plb_1_M_wrBurst,
      M_wrDBus => plb_1_M_wrDBus,
      Sl_addrAck => plb_1_Sl_addrAck,
      Sl_MRdErr => plb_1_Sl_MRdErr,
      Sl_MWrErr => plb_1_Sl_MWrErr,
      Sl_MBusy => plb_1_Sl_MBusy,
      Sl_rdBTerm => plb_1_Sl_rdBTerm,
      Sl_rdComp => plb_1_Sl_rdComp,
      Sl_rdDAck => plb_1_Sl_rdDAck,
      Sl_rdDBus => plb_1_Sl_rdDBus,
      Sl_rdWdAddr => plb_1_Sl_rdWdAddr,
      Sl_rearbitrate => plb_1_Sl_rearbitrate,
      Sl_SSize => plb_1_Sl_SSize,
      Sl_wait => plb_1_Sl_wait,
      Sl_wrBTerm => plb_1_Sl_wrBTerm,
      Sl_wrComp => plb_1_Sl_wrComp,
      Sl_wrDAck => plb_1_Sl_wrDAck,
      Sl_MIRQ => plb_1_Sl_MIRQ,
      PLB_MIRQ => plb_1_PLB_MIRQ,
      PLB_ABus => plb_1_PLB_ABus,
      PLB_UABus => plb_1_PLB_UABus,
      PLB_BE => plb_1_PLB_BE,
      PLB_MAddrAck => plb_1_PLB_MAddrAck,
      PLB_MTimeout => plb_1_PLB_MTimeout,
      PLB_MBusy => plb_1_PLB_MBusy,
      PLB_MRdErr => plb_1_PLB_MRdErr,
      PLB_MWrErr => plb_1_PLB_MWrErr,
      PLB_MRdBTerm => plb_1_PLB_MRdBTerm,
      PLB_MRdDAck => plb_1_PLB_MRdDAck,
      PLB_MRdDBus => plb_1_PLB_MRdDBus,
      PLB_MRdWdAddr => plb_1_PLB_MRdWdAddr,
      PLB_MRearbitrate => plb_1_PLB_MRearbitrate,
      PLB_MWrBTerm => plb_1_PLB_MWrBTerm,
      PLB_MWrDAck => plb_1_PLB_MWrDAck,
      PLB_MSSize => plb_1_PLB_MSSize,
      PLB_PAValid => plb_1_PLB_PAValid,
      PLB_RNW => plb_1_PLB_RNW,
      PLB_SAValid => plb_1_PLB_SAValid,
      PLB_abort => plb_1_PLB_abort,
      PLB_busLock => plb_1_PLB_busLock,
      PLB_TAttribute => plb_1_PLB_TAttribute,
      PLB_lockErr => plb_1_PLB_lockErr,
      PLB_masterID => plb_1_PLB_masterID,
      PLB_MSize => plb_1_PLB_MSize,
      PLB_rdPendPri => plb_1_PLB_rdPendPri,
      PLB_wrPendPri => plb_1_PLB_wrPendPri,
      PLB_rdPendReq => plb_1_PLB_rdPendReq,
      PLB_wrPendReq => plb_1_PLB_wrPendReq,
      PLB_rdBurst => plb_1_PLB_rdBurst,
      PLB_rdPrim => plb_1_PLB_rdPrim,
      PLB_reqPri => plb_1_PLB_reqPri,
      PLB_size => plb_1_PLB_size,
      PLB_type => plb_1_PLB_type,
      PLB_wrBurst => plb_1_PLB_wrBurst,
      PLB_wrDBus => plb_1_PLB_wrDBus,
      PLB_wrPrim => plb_1_PLB_wrPrim,
      PLB_SaddrAck => open,
      PLB_SMRdErr => open,
      PLB_SMWrErr => open,
      PLB_SMBusy => open,
      PLB_SrdBTerm => open,
      PLB_SrdComp => open,
      PLB_SrdDAck => open,
      PLB_SrdDBus => open,
      PLB_SrdWdAddr => open,
      PLB_Srearbitrate => open,
      PLB_Sssize => open,
      PLB_Swait => open,
      PLB_SwrBTerm => open,
      PLB_SwrComp => open,
      PLB_SwrDAck => open,
      Bus_Error_Det => open
    );

  xps_bram_if_cntlr_0 : xps_bram_if_cntlr_0_wrapper
    port map (
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_0_SPLB_Rst(0),
      PLB_ABus => plb_0_PLB_ABus,
      PLB_UABus => plb_0_PLB_UABus,
      PLB_PAValid => plb_0_PLB_PAValid,
      PLB_SAValid => plb_0_PLB_SAValid,
      PLB_rdPrim => plb_0_PLB_rdPrim(0),
      PLB_wrPrim => plb_0_PLB_wrPrim(0),
      PLB_masterID => plb_0_PLB_masterID(0 to 0),
      PLB_abort => plb_0_PLB_abort,
      PLB_busLock => plb_0_PLB_busLock,
      PLB_RNW => plb_0_PLB_RNW,
      PLB_BE => plb_0_PLB_BE,
      PLB_MSize => plb_0_PLB_MSize,
      PLB_size => plb_0_PLB_size,
      PLB_type => plb_0_PLB_type,
      PLB_lockErr => plb_0_PLB_lockErr,
      PLB_wrDBus => plb_0_PLB_wrDBus,
      PLB_wrBurst => plb_0_PLB_wrBurst,
      PLB_rdBurst => plb_0_PLB_rdBurst,
      PLB_wrPendReq => plb_0_PLB_wrPendReq,
      PLB_rdPendReq => plb_0_PLB_rdPendReq,
      PLB_wrPendPri => plb_0_PLB_wrPendPri,
      PLB_rdPendPri => plb_0_PLB_rdPendPri,
      PLB_reqPri => plb_0_PLB_reqPri,
      PLB_TAttribute => plb_0_PLB_TAttribute,
      Sl_addrAck => plb_0_Sl_addrAck(0),
      Sl_SSize => plb_0_Sl_SSize(0 to 1),
      Sl_wait => plb_0_Sl_wait(0),
      Sl_rearbitrate => plb_0_Sl_rearbitrate(0),
      Sl_wrDAck => plb_0_Sl_wrDAck(0),
      Sl_wrComp => plb_0_Sl_wrComp(0),
      Sl_wrBTerm => plb_0_Sl_wrBTerm(0),
      Sl_rdDBus => plb_0_Sl_rdDBus(0 to 63),
      Sl_rdWdAddr => plb_0_Sl_rdWdAddr(0 to 3),
      Sl_rdDAck => plb_0_Sl_rdDAck(0),
      Sl_rdComp => plb_0_Sl_rdComp(0),
      Sl_rdBTerm => plb_0_Sl_rdBTerm(0),
      Sl_MBusy => plb_0_Sl_MBusy(0 to 1),
      Sl_MWrErr => plb_0_Sl_MWrErr(0 to 1),
      Sl_MRdErr => plb_0_Sl_MRdErr(0 to 1),
      Sl_MIRQ => plb_0_Sl_MIRQ(0 to 1),
      BRAM_Rst => xps_bram_if_cntlr_0_port_BRAM_Rst,
      BRAM_Clk => xps_bram_if_cntlr_0_port_BRAM_Clk,
      BRAM_EN => xps_bram_if_cntlr_0_port_BRAM_EN,
      BRAM_WEN => xps_bram_if_cntlr_0_port_BRAM_WEN,
      BRAM_Addr => xps_bram_if_cntlr_0_port_BRAM_Addr,
      BRAM_Din => xps_bram_if_cntlr_0_port_BRAM_Din,
      BRAM_Dout => xps_bram_if_cntlr_0_port_BRAM_Dout
    );

  xps_bram_if_cntlr_1 : xps_bram_if_cntlr_1_wrapper
    port map (
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_1_SPLB_Rst(0),
      PLB_ABus => plb_1_PLB_ABus,
      PLB_UABus => plb_1_PLB_UABus,
      PLB_PAValid => plb_1_PLB_PAValid,
      PLB_SAValid => plb_1_PLB_SAValid,
      PLB_rdPrim => plb_1_PLB_rdPrim(0),
      PLB_wrPrim => plb_1_PLB_wrPrim(0),
      PLB_masterID => plb_1_PLB_masterID,
      PLB_abort => plb_1_PLB_abort,
      PLB_busLock => plb_1_PLB_busLock,
      PLB_RNW => plb_1_PLB_RNW,
      PLB_BE => plb_1_PLB_BE,
      PLB_MSize => plb_1_PLB_MSize,
      PLB_size => plb_1_PLB_size,
      PLB_type => plb_1_PLB_type,
      PLB_lockErr => plb_1_PLB_lockErr,
      PLB_wrDBus => plb_1_PLB_wrDBus,
      PLB_wrBurst => plb_1_PLB_wrBurst,
      PLB_rdBurst => plb_1_PLB_rdBurst,
      PLB_wrPendReq => plb_1_PLB_wrPendReq,
      PLB_rdPendReq => plb_1_PLB_rdPendReq,
      PLB_wrPendPri => plb_1_PLB_wrPendPri,
      PLB_rdPendPri => plb_1_PLB_rdPendPri,
      PLB_reqPri => plb_1_PLB_reqPri,
      PLB_TAttribute => plb_1_PLB_TAttribute,
      Sl_addrAck => plb_1_Sl_addrAck(0),
      Sl_SSize => plb_1_Sl_SSize(0 to 1),
      Sl_wait => plb_1_Sl_wait(0),
      Sl_rearbitrate => plb_1_Sl_rearbitrate(0),
      Sl_wrDAck => plb_1_Sl_wrDAck(0),
      Sl_wrComp => plb_1_Sl_wrComp(0),
      Sl_wrBTerm => plb_1_Sl_wrBTerm(0),
      Sl_rdDBus => plb_1_Sl_rdDBus(0 to 63),
      Sl_rdWdAddr => plb_1_Sl_rdWdAddr(0 to 3),
      Sl_rdDAck => plb_1_Sl_rdDAck(0),
      Sl_rdComp => plb_1_Sl_rdComp(0),
      Sl_rdBTerm => plb_1_Sl_rdBTerm(0),
      Sl_MBusy => plb_1_Sl_MBusy(0 to 3),
      Sl_MWrErr => plb_1_Sl_MWrErr(0 to 3),
      Sl_MRdErr => plb_1_Sl_MRdErr(0 to 3),
      Sl_MIRQ => plb_1_Sl_MIRQ(0 to 3),
      BRAM_Rst => xps_bram_if_cntlr_1_port_BRAM_Rst,
      BRAM_Clk => xps_bram_if_cntlr_1_port_BRAM_Clk,
      BRAM_EN => xps_bram_if_cntlr_1_port_BRAM_EN,
      BRAM_WEN => xps_bram_if_cntlr_1_port_BRAM_WEN,
      BRAM_Addr => xps_bram_if_cntlr_1_port_BRAM_Addr,
      BRAM_Din => xps_bram_if_cntlr_1_port_BRAM_Din,
      BRAM_Dout => xps_bram_if_cntlr_1_port_BRAM_Dout
    );

  plb_bram_if_cntlr_0_bram : plb_bram_if_cntlr_0_bram_wrapper
    port map (
      BRAM_Rst_A => xps_bram_if_cntlr_0_port_BRAM_Rst,
      BRAM_Clk_A => xps_bram_if_cntlr_0_port_BRAM_Clk,
      BRAM_EN_A => xps_bram_if_cntlr_0_port_BRAM_EN,
      BRAM_WEN_A => xps_bram_if_cntlr_0_port_BRAM_WEN,
      BRAM_Addr_A => xps_bram_if_cntlr_0_port_BRAM_Addr,
      BRAM_Din_A => xps_bram_if_cntlr_0_port_BRAM_Din,
      BRAM_Dout_A => xps_bram_if_cntlr_0_port_BRAM_Dout,
      BRAM_Rst_B => net_gnd0,
      BRAM_Clk_B => net_gnd0,
      BRAM_EN_B => net_gnd0,
      BRAM_WEN_B => net_gnd8,
      BRAM_Addr_B => net_gnd32,
      BRAM_Din_B => open,
      BRAM_Dout_B => net_gnd64
    );

  plb_bram_if_cntlr_1_bram : plb_bram_if_cntlr_1_bram_wrapper
    port map (
      BRAM_Rst_A => xps_bram_if_cntlr_1_port_BRAM_Rst,
      BRAM_Clk_A => xps_bram_if_cntlr_1_port_BRAM_Clk,
      BRAM_EN_A => xps_bram_if_cntlr_1_port_BRAM_EN,
      BRAM_WEN_A => xps_bram_if_cntlr_1_port_BRAM_WEN,
      BRAM_Addr_A => xps_bram_if_cntlr_1_port_BRAM_Addr,
      BRAM_Din_A => xps_bram_if_cntlr_1_port_BRAM_Din,
      BRAM_Dout_A => xps_bram_if_cntlr_1_port_BRAM_Dout,
      BRAM_Rst_B => net_gnd0,
      BRAM_Clk_B => net_gnd0,
      BRAM_EN_B => net_gnd0,
      BRAM_WEN_B => net_gnd8,
      BRAM_Addr_B => net_gnd32,
      BRAM_Din_B => open,
      BRAM_Dout_B => net_gnd64
    );

  serial_uart : serial_uart_wrapper
    port map (
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_0_SPLB_Rst(1),
      PLB_ABus => plb_0_PLB_ABus,
      PLB_PAValid => plb_0_PLB_PAValid,
      PLB_masterID => plb_0_PLB_masterID(0 to 0),
      PLB_RNW => plb_0_PLB_RNW,
      PLB_BE => plb_0_PLB_BE,
      PLB_size => plb_0_PLB_size,
      PLB_type => plb_0_PLB_type,
      PLB_wrDBus => plb_0_PLB_wrDBus,
      PLB_UABus => plb_0_PLB_UABus,
      PLB_SAValid => plb_0_PLB_SAValid,
      PLB_rdPrim => plb_0_PLB_rdPrim(1),
      PLB_wrPrim => plb_0_PLB_wrPrim(1),
      PLB_abort => plb_0_PLB_abort,
      PLB_busLock => plb_0_PLB_busLock,
      PLB_MSize => plb_0_PLB_MSize,
      PLB_lockErr => plb_0_PLB_lockErr,
      PLB_wrBurst => plb_0_PLB_wrBurst,
      PLB_rdBurst => plb_0_PLB_rdBurst,
      PLB_wrPendReq => plb_0_PLB_wrPendReq,
      PLB_rdPendReq => plb_0_PLB_rdPendReq,
      PLB_wrPendPri => plb_0_PLB_wrPendPri,
      PLB_rdPendPri => plb_0_PLB_rdPendPri,
      PLB_reqPri => plb_0_PLB_reqPri,
      PLB_TAttribute => plb_0_PLB_TAttribute,
      Sl_addrAck => plb_0_Sl_addrAck(1),
      Sl_SSize => plb_0_Sl_SSize(2 to 3),
      Sl_wait => plb_0_Sl_wait(1),
      Sl_rearbitrate => plb_0_Sl_rearbitrate(1),
      Sl_wrDAck => plb_0_Sl_wrDAck(1),
      Sl_wrComp => plb_0_Sl_wrComp(1),
      Sl_rdDBus => plb_0_Sl_rdDBus(64 to 127),
      Sl_rdDAck => plb_0_Sl_rdDAck(1),
      Sl_rdComp => plb_0_Sl_rdComp(1),
      Sl_MBusy => plb_0_Sl_MBusy(2 to 3),
      Sl_MWrErr => plb_0_Sl_MWrErr(2 to 3),
      Sl_MRdErr => plb_0_Sl_MRdErr(2 to 3),
      Sl_wrBTerm => plb_0_Sl_wrBTerm(1),
      Sl_rdWdAddr => plb_0_Sl_rdWdAddr(4 to 7),
      Sl_rdBTerm => plb_0_Sl_rdBTerm(1),
      Sl_MIRQ => plb_0_Sl_MIRQ(2 to 3),
      RX => uart_rx,
      TX => uart_tx,
      Interrupt => serial_int
    );

  wireless_uart : wireless_uart_wrapper
    port map (
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_0_SPLB_Rst(2),
      PLB_ABus => plb_0_PLB_ABus,
      PLB_PAValid => plb_0_PLB_PAValid,
      PLB_masterID => plb_0_PLB_masterID(0 to 0),
      PLB_RNW => plb_0_PLB_RNW,
      PLB_BE => plb_0_PLB_BE,
      PLB_size => plb_0_PLB_size,
      PLB_type => plb_0_PLB_type,
      PLB_wrDBus => plb_0_PLB_wrDBus,
      PLB_UABus => plb_0_PLB_UABus,
      PLB_SAValid => plb_0_PLB_SAValid,
      PLB_rdPrim => plb_0_PLB_rdPrim(2),
      PLB_wrPrim => plb_0_PLB_wrPrim(2),
      PLB_abort => plb_0_PLB_abort,
      PLB_busLock => plb_0_PLB_busLock,
      PLB_MSize => plb_0_PLB_MSize,
      PLB_lockErr => plb_0_PLB_lockErr,
      PLB_wrBurst => plb_0_PLB_wrBurst,
      PLB_rdBurst => plb_0_PLB_rdBurst,
      PLB_wrPendReq => plb_0_PLB_wrPendReq,
      PLB_rdPendReq => plb_0_PLB_rdPendReq,
      PLB_wrPendPri => plb_0_PLB_wrPendPri,
      PLB_rdPendPri => plb_0_PLB_rdPendPri,
      PLB_reqPri => plb_0_PLB_reqPri,
      PLB_TAttribute => plb_0_PLB_TAttribute,
      Sl_addrAck => plb_0_Sl_addrAck(2),
      Sl_SSize => plb_0_Sl_SSize(4 to 5),
      Sl_wait => plb_0_Sl_wait(2),
      Sl_rearbitrate => plb_0_Sl_rearbitrate(2),
      Sl_wrDAck => plb_0_Sl_wrDAck(2),
      Sl_wrComp => plb_0_Sl_wrComp(2),
      Sl_rdDBus => plb_0_Sl_rdDBus(128 to 191),
      Sl_rdDAck => plb_0_Sl_rdDAck(2),
      Sl_rdComp => plb_0_Sl_rdComp(2),
      Sl_MBusy => plb_0_Sl_MBusy(4 to 5),
      Sl_MWrErr => plb_0_Sl_MWrErr(4 to 5),
      Sl_MRdErr => plb_0_Sl_MRdErr(4 to 5),
      Sl_wrBTerm => plb_0_Sl_wrBTerm(2),
      Sl_rdWdAddr => plb_0_Sl_rdWdAddr(8 to 11),
      Sl_rdBTerm => plb_0_Sl_rdBTerm(2),
      Sl_MIRQ => plb_0_Sl_MIRQ(4 to 5),
      RX => wireless_rx,
      TX => wireless_tx,
      Interrupt => wireless_int
    );

  clock_generator_0 : clock_generator_0_wrapper
    port map (
      CLKIN => dcm_clk_s,
      CLKOUT0 => plb_clk_100MHz,
      CLKOUT1 => clk_300_0000MHzDCM0,
      CLKOUT2 => open,
      CLKOUT3 => open,
      CLKOUT4 => open,
      CLKOUT5 => open,
      CLKOUT6 => open,
      CLKOUT7 => open,
      CLKOUT8 => open,
      CLKOUT9 => open,
      CLKOUT10 => open,
      CLKOUT11 => open,
      CLKOUT12 => open,
      CLKOUT13 => open,
      CLKOUT14 => open,
      CLKOUT15 => open,
      CLKFBIN => net_gnd0,
      CLKFBOUT => open,
      PSCLK => net_gnd0,
      PSEN => net_gnd0,
      PSINCDEC => net_gnd0,
      PSDONE => open,
      RST => net_gnd0,
      LOCKED => DCM_1_lock
    );

  jtagppc_cntlr_inst : jtagppc_cntlr_inst_wrapper
    port map (
      TRSTNEG => net_vcc0,
      HALTNEG0 => net_vcc0,
      DBGC405DEBUGHALT0 => open,
      HALTNEG1 => net_vcc0,
      DBGC405DEBUGHALT1 => open,
      C405JTGTDO0 => ppc405_0_jtagppc_bus_C405JTGTDO,
      C405JTGTDOEN0 => ppc405_0_jtagppc_bus_C405JTGTDOEN,
      JTGC405TCK0 => ppc405_0_jtagppc_bus_JTGC405TCK,
      JTGC405TDI0 => ppc405_0_jtagppc_bus_JTGC405TDI,
      JTGC405TMS0 => ppc405_0_jtagppc_bus_JTGC405TMS,
      JTGC405TRSTNEG0 => ppc405_0_jtagppc_bus_JTGC405TRSTNEG,
      C405JTGTDO1 => ppc405_1_jtagppc_bus_C405JTGTDO,
      C405JTGTDOEN1 => ppc405_1_jtagppc_bus_C405JTGTDOEN,
      JTGC405TCK1 => ppc405_1_jtagppc_bus_JTGC405TCK,
      JTGC405TDI1 => ppc405_1_jtagppc_bus_JTGC405TDI,
      JTGC405TMS1 => ppc405_1_jtagppc_bus_JTGC405TMS,
      JTGC405TRSTNEG1 => ppc405_1_jtagppc_bus_JTGC405TRSTNEG
    );

  proc_sys_reset_0 : proc_sys_reset_0_wrapper
    port map (
      Slowest_sync_clk => plb_clk_100MHz,
      Ext_Reset_In => sys_rst_s,
      Aux_Reset_In => net_gnd0,
      MB_Debug_Sys_Rst => net_gnd0,
      Core_Reset_Req_0 => ppc_reset_bus_0_Core_Reset_Req,
      Chip_Reset_Req_0 => ppc_reset_bus_0_Chip_Reset_Req,
      System_Reset_Req_0 => ppc_reset_bus_0_System_Reset_Req,
      Core_Reset_Req_1 => ppc_reset_bus_1_Core_Reset_Req,
      Chip_Reset_Req_1 => ppc_reset_bus_1_Chip_Reset_Req,
      System_Reset_Req_1 => ppc_reset_bus_1_System_Reset_Req,
      Dcm_locked => Dcm_all_locked,
      RstcPPCresetcore_0 => ppc_reset_bus_0_RstcPPCresetcore,
      RstcPPCresetchip_0 => ppc_reset_bus_0_RstsPPCresetchip,
      RstcPPCresetsys_0 => ppc_reset_bus_0_RstcPPCresetsys,
      RstcPPCresetcore_1 => ppc_reset_bus_1_RstcPPCresetcore,
      RstcPPCresetchip_1 => ppc_reset_bus_1_RstsPPCresetchip,
      RstcPPCresetsys_1 => ppc_reset_bus_1_RstcPPCresetsys,
      MB_Reset => open,
      Bus_Struct_Reset => sys_bus_reset(0 to 0),
      Peripheral_Reset => open,
      Interconnect_aresetn => open,
      Peripheral_aresetn => open
    );

  xps_intc_0 : xps_intc_0_wrapper
    port map (
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_0_SPLB_Rst(3),
      PLB_ABus => plb_0_PLB_ABus,
      PLB_PAValid => plb_0_PLB_PAValid,
      PLB_masterID => plb_0_PLB_masterID(0 to 0),
      PLB_RNW => plb_0_PLB_RNW,
      PLB_BE => plb_0_PLB_BE,
      PLB_size => plb_0_PLB_size,
      PLB_type => plb_0_PLB_type,
      PLB_wrDBus => plb_0_PLB_wrDBus,
      PLB_UABus => plb_0_PLB_UABus,
      PLB_SAValid => plb_0_PLB_SAValid,
      PLB_rdPrim => plb_0_PLB_rdPrim(3),
      PLB_wrPrim => plb_0_PLB_wrPrim(3),
      PLB_abort => plb_0_PLB_abort,
      PLB_busLock => plb_0_PLB_busLock,
      PLB_MSize => plb_0_PLB_MSize,
      PLB_lockErr => plb_0_PLB_lockErr,
      PLB_wrBurst => plb_0_PLB_wrBurst,
      PLB_rdBurst => plb_0_PLB_rdBurst,
      PLB_wrPendReq => plb_0_PLB_wrPendReq,
      PLB_rdPendReq => plb_0_PLB_rdPendReq,
      PLB_wrPendPri => plb_0_PLB_wrPendPri,
      PLB_rdPendPri => plb_0_PLB_rdPendPri,
      PLB_reqPri => plb_0_PLB_reqPri,
      PLB_TAttribute => plb_0_PLB_TAttribute,
      Sl_addrAck => plb_0_Sl_addrAck(3),
      Sl_SSize => plb_0_Sl_SSize(6 to 7),
      Sl_wait => plb_0_Sl_wait(3),
      Sl_rearbitrate => plb_0_Sl_rearbitrate(3),
      Sl_wrDAck => plb_0_Sl_wrDAck(3),
      Sl_wrComp => plb_0_Sl_wrComp(3),
      Sl_rdDBus => plb_0_Sl_rdDBus(192 to 255),
      Sl_rdDAck => plb_0_Sl_rdDAck(3),
      Sl_rdComp => plb_0_Sl_rdComp(3),
      Sl_MBusy => plb_0_Sl_MBusy(6 to 7),
      Sl_MWrErr => plb_0_Sl_MWrErr(6 to 7),
      Sl_MRdErr => plb_0_Sl_MRdErr(6 to 7),
      Sl_wrBTerm => plb_0_Sl_wrBTerm(3),
      Sl_rdWdAddr => plb_0_Sl_rdWdAddr(12 to 15),
      Sl_rdBTerm => plb_0_Sl_rdBTerm(3),
      Sl_MIRQ => plb_0_Sl_MIRQ(6 to 7),
      Intr => pgassign4,
      Irq => ext_int_ppc405_0
    );

  plb_quad_encoder_0 : plb_quad_encoder_0_wrapper
    port map (
      enc_a_in => plb_quad_encoder_0_enc_a_in,
      enc_b_in => plb_quad_encoder_0_enc_b_in,
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_0_SPLB_Rst(4),
      PLB_ABus => plb_0_PLB_ABus,
      PLB_UABus => plb_0_PLB_UABus,
      PLB_PAValid => plb_0_PLB_PAValid,
      PLB_SAValid => plb_0_PLB_SAValid,
      PLB_rdPrim => plb_0_PLB_rdPrim(4),
      PLB_wrPrim => plb_0_PLB_wrPrim(4),
      PLB_masterID => plb_0_PLB_masterID(0 to 0),
      PLB_abort => plb_0_PLB_abort,
      PLB_busLock => plb_0_PLB_busLock,
      PLB_RNW => plb_0_PLB_RNW,
      PLB_BE => plb_0_PLB_BE,
      PLB_MSize => plb_0_PLB_MSize,
      PLB_size => plb_0_PLB_size,
      PLB_type => plb_0_PLB_type,
      PLB_lockErr => plb_0_PLB_lockErr,
      PLB_wrDBus => plb_0_PLB_wrDBus,
      PLB_wrBurst => plb_0_PLB_wrBurst,
      PLB_rdBurst => plb_0_PLB_rdBurst,
      PLB_wrPendReq => plb_0_PLB_wrPendReq,
      PLB_rdPendReq => plb_0_PLB_rdPendReq,
      PLB_wrPendPri => plb_0_PLB_wrPendPri,
      PLB_rdPendPri => plb_0_PLB_rdPendPri,
      PLB_reqPri => plb_0_PLB_reqPri,
      PLB_TAttribute => plb_0_PLB_TAttribute,
      Sl_addrAck => plb_0_Sl_addrAck(4),
      Sl_SSize => plb_0_Sl_SSize(8 to 9),
      Sl_wait => plb_0_Sl_wait(4),
      Sl_rearbitrate => plb_0_Sl_rearbitrate(4),
      Sl_wrDAck => plb_0_Sl_wrDAck(4),
      Sl_wrComp => plb_0_Sl_wrComp(4),
      Sl_wrBTerm => plb_0_Sl_wrBTerm(4),
      Sl_rdDBus => plb_0_Sl_rdDBus(256 to 319),
      Sl_rdWdAddr => plb_0_Sl_rdWdAddr(16 to 19),
      Sl_rdDAck => plb_0_Sl_rdDAck(4),
      Sl_rdComp => plb_0_Sl_rdComp(4),
      Sl_rdBTerm => plb_0_Sl_rdBTerm(4),
      Sl_MBusy => plb_0_Sl_MBusy(8 to 9),
      Sl_MWrErr => plb_0_Sl_MWrErr(8 to 9),
      Sl_MRdErr => plb_0_Sl_MRdErr(8 to 9),
      Sl_MIRQ => plb_0_Sl_MIRQ(8 to 9)
    );

  plb_pwm_ctrl_0 : plb_pwm_ctrl_0_wrapper
    port map (
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_0_SPLB_Rst(5),
      PLB_ABus => plb_0_PLB_ABus,
      PLB_UABus => plb_0_PLB_UABus,
      PLB_PAValid => plb_0_PLB_PAValid,
      PLB_SAValid => plb_0_PLB_SAValid,
      PLB_rdPrim => plb_0_PLB_rdPrim(5),
      PLB_wrPrim => plb_0_PLB_wrPrim(5),
      PLB_masterID => plb_0_PLB_masterID(0 to 0),
      PLB_abort => plb_0_PLB_abort,
      PLB_busLock => plb_0_PLB_busLock,
      PLB_RNW => plb_0_PLB_RNW,
      PLB_BE => plb_0_PLB_BE,
      PLB_MSize => plb_0_PLB_MSize,
      PLB_size => plb_0_PLB_size,
      PLB_type => plb_0_PLB_type,
      PLB_lockErr => plb_0_PLB_lockErr,
      PLB_wrDBus => plb_0_PLB_wrDBus,
      PLB_wrBurst => plb_0_PLB_wrBurst,
      PLB_rdBurst => plb_0_PLB_rdBurst,
      PLB_wrPendReq => plb_0_PLB_wrPendReq,
      PLB_rdPendReq => plb_0_PLB_rdPendReq,
      PLB_wrPendPri => plb_0_PLB_wrPendPri,
      PLB_rdPendPri => plb_0_PLB_rdPendPri,
      PLB_reqPri => plb_0_PLB_reqPri,
      PLB_TAttribute => plb_0_PLB_TAttribute,
      Sl_addrAck => plb_0_Sl_addrAck(5),
      Sl_SSize => plb_0_Sl_SSize(10 to 11),
      Sl_wait => plb_0_Sl_wait(5),
      Sl_rearbitrate => plb_0_Sl_rearbitrate(5),
      Sl_wrDAck => plb_0_Sl_wrDAck(5),
      Sl_wrComp => plb_0_Sl_wrComp(5),
      Sl_wrBTerm => plb_0_Sl_wrBTerm(5),
      Sl_rdDBus => plb_0_Sl_rdDBus(320 to 383),
      Sl_rdWdAddr => plb_0_Sl_rdWdAddr(20 to 23),
      Sl_rdDAck => plb_0_Sl_rdDAck(5),
      Sl_rdComp => plb_0_Sl_rdComp(5),
      Sl_rdBTerm => plb_0_Sl_rdBTerm(5),
      Sl_MBusy => plb_0_Sl_MBusy(10 to 11),
      Sl_MWrErr => plb_0_Sl_MWrErr(10 to 11),
      Sl_MRdErr => plb_0_Sl_MRdErr(10 to 11),
      Sl_MIRQ => plb_0_Sl_MIRQ(10 to 11),
      pwm => pgassign5
    );

  xps_mch_emc_flash : xps_mch_emc_flash_wrapper
    port map (
      MCH_SPLB_Clk => plb_clk_100MHz,
      RdClk => plb_clk_100MHz,
      MCH_SPLB_Rst => plb_0_SPLB_Rst(6),
      MCH0_Access_Control => net_gnd0,
      MCH0_Access_Data => net_gnd32,
      MCH0_Access_Write => net_gnd0,
      MCH0_Access_Full => open,
      MCH0_ReadData_Control => open,
      MCH0_ReadData_Data => open,
      MCH0_ReadData_Read => net_gnd0,
      MCH0_ReadData_Exists => open,
      MCH1_Access_Control => net_gnd0,
      MCH1_Access_Data => net_gnd32,
      MCH1_Access_Write => net_gnd0,
      MCH1_Access_Full => open,
      MCH1_ReadData_Control => open,
      MCH1_ReadData_Data => open,
      MCH1_ReadData_Read => net_gnd0,
      MCH1_ReadData_Exists => open,
      MCH2_Access_Control => net_gnd0,
      MCH2_Access_Data => net_gnd32,
      MCH2_Access_Write => net_gnd0,
      MCH2_Access_Full => open,
      MCH2_ReadData_Control => open,
      MCH2_ReadData_Data => open,
      MCH2_ReadData_Read => net_gnd0,
      MCH2_ReadData_Exists => open,
      MCH3_Access_Control => net_gnd0,
      MCH3_Access_Data => net_gnd32,
      MCH3_Access_Write => net_gnd0,
      MCH3_Access_Full => open,
      MCH3_ReadData_Control => open,
      MCH3_ReadData_Data => open,
      MCH3_ReadData_Read => net_gnd0,
      MCH3_ReadData_Exists => open,
      PLB_ABus => plb_0_PLB_ABus,
      PLB_UABus => plb_0_PLB_UABus,
      PLB_PAValid => plb_0_PLB_PAValid,
      PLB_SAValid => plb_0_PLB_SAValid,
      PLB_rdPrim => plb_0_PLB_rdPrim(6),
      PLB_wrPrim => plb_0_PLB_wrPrim(6),
      PLB_masterID => plb_0_PLB_masterID(0 to 0),
      PLB_abort => plb_0_PLB_abort,
      PLB_busLock => plb_0_PLB_busLock,
      PLB_RNW => plb_0_PLB_RNW,
      PLB_BE => plb_0_PLB_BE,
      PLB_MSize => plb_0_PLB_MSize,
      PLB_size => plb_0_PLB_size,
      PLB_type => plb_0_PLB_type,
      PLB_lockErr => plb_0_PLB_lockErr,
      PLB_wrDBus => plb_0_PLB_wrDBus,
      PLB_wrBurst => plb_0_PLB_wrBurst,
      PLB_rdBurst => plb_0_PLB_rdBurst,
      PLB_wrPendReq => plb_0_PLB_wrPendReq,
      PLB_rdPendReq => plb_0_PLB_rdPendReq,
      PLB_wrPendPri => plb_0_PLB_wrPendPri,
      PLB_rdPendPri => plb_0_PLB_rdPendPri,
      PLB_reqPri => plb_0_PLB_reqPri,
      PLB_TAttribute => plb_0_PLB_TAttribute,
      Sl_addrAck => plb_0_Sl_addrAck(6),
      Sl_SSize => plb_0_Sl_SSize(12 to 13),
      Sl_wait => plb_0_Sl_wait(6),
      Sl_rearbitrate => plb_0_Sl_rearbitrate(6),
      Sl_wrDAck => plb_0_Sl_wrDAck(6),
      Sl_wrComp => plb_0_Sl_wrComp(6),
      Sl_wrBTerm => plb_0_Sl_wrBTerm(6),
      Sl_rdDBus => plb_0_Sl_rdDBus(384 to 447),
      Sl_rdWdAddr => plb_0_Sl_rdWdAddr(24 to 27),
      Sl_rdDAck => plb_0_Sl_rdDAck(6),
      Sl_rdComp => plb_0_Sl_rdComp(6),
      Sl_rdBTerm => plb_0_Sl_rdBTerm(6),
      Sl_MBusy => plb_0_Sl_MBusy(12 to 13),
      Sl_MWrErr => plb_0_Sl_MWrErr(12 to 13),
      Sl_MRdErr => plb_0_Sl_MRdErr(12 to 13),
      Sl_MIRQ => plb_0_Sl_MIRQ(12 to 13),
      Mem_DQ_I => xps_mch_emc_0_Mem_DQ_I,
      Mem_DQ_O => xps_mch_emc_0_Mem_DQ_O,
      Mem_DQ_T => xps_mch_emc_0_Mem_DQ_T,
      Mem_A => xps_mch_emc_0_Mem_A,
      Mem_RPN => xps_mch_emc_0_Mem_RPN,
      Mem_CEN => xps_mch_emc_0_Mem_CEN(0 to 0),
      Mem_OEN => xps_mch_emc_0_Mem_OEN(0 to 0),
      Mem_WEN => open,
      Mem_QWEN => xps_mch_emc_0_Mem_QWEN,
      Mem_BEN => open,
      Mem_CE => open,
      Mem_ADV_LDN => open,
      Mem_LBON => open,
      Mem_CKEN => open,
      Mem_RNW => open
    );

  xps_mch_emc_sram : xps_mch_emc_sram_wrapper
    port map (
      MCH_SPLB_Clk => plb_clk_100MHz,
      RdClk => plb_clk_100MHz,
      MCH_SPLB_Rst => plb_1_SPLB_Rst(1),
      MCH0_Access_Control => net_gnd0,
      MCH0_Access_Data => net_gnd32,
      MCH0_Access_Write => net_gnd0,
      MCH0_Access_Full => open,
      MCH0_ReadData_Control => open,
      MCH0_ReadData_Data => open,
      MCH0_ReadData_Read => net_gnd0,
      MCH0_ReadData_Exists => open,
      MCH1_Access_Control => net_gnd0,
      MCH1_Access_Data => net_gnd32,
      MCH1_Access_Write => net_gnd0,
      MCH1_Access_Full => open,
      MCH1_ReadData_Control => open,
      MCH1_ReadData_Data => open,
      MCH1_ReadData_Read => net_gnd0,
      MCH1_ReadData_Exists => open,
      MCH2_Access_Control => net_gnd0,
      MCH2_Access_Data => net_gnd32,
      MCH2_Access_Write => net_gnd0,
      MCH2_Access_Full => open,
      MCH2_ReadData_Control => open,
      MCH2_ReadData_Data => open,
      MCH2_ReadData_Read => net_gnd0,
      MCH2_ReadData_Exists => open,
      MCH3_Access_Control => net_gnd0,
      MCH3_Access_Data => net_gnd32,
      MCH3_Access_Write => net_gnd0,
      MCH3_Access_Full => open,
      MCH3_ReadData_Control => open,
      MCH3_ReadData_Data => open,
      MCH3_ReadData_Read => net_gnd0,
      MCH3_ReadData_Exists => open,
      PLB_ABus => plb_1_PLB_ABus,
      PLB_UABus => plb_1_PLB_UABus,
      PLB_PAValid => plb_1_PLB_PAValid,
      PLB_SAValid => plb_1_PLB_SAValid,
      PLB_rdPrim => plb_1_PLB_rdPrim(1),
      PLB_wrPrim => plb_1_PLB_wrPrim(1),
      PLB_masterID => plb_1_PLB_masterID,
      PLB_abort => plb_1_PLB_abort,
      PLB_busLock => plb_1_PLB_busLock,
      PLB_RNW => plb_1_PLB_RNW,
      PLB_BE => plb_1_PLB_BE,
      PLB_MSize => plb_1_PLB_MSize,
      PLB_size => plb_1_PLB_size,
      PLB_type => plb_1_PLB_type,
      PLB_lockErr => plb_1_PLB_lockErr,
      PLB_wrDBus => plb_1_PLB_wrDBus,
      PLB_wrBurst => plb_1_PLB_wrBurst,
      PLB_rdBurst => plb_1_PLB_rdBurst,
      PLB_wrPendReq => plb_1_PLB_wrPendReq,
      PLB_rdPendReq => plb_1_PLB_rdPendReq,
      PLB_wrPendPri => plb_1_PLB_wrPendPri,
      PLB_rdPendPri => plb_1_PLB_rdPendPri,
      PLB_reqPri => plb_1_PLB_reqPri,
      PLB_TAttribute => plb_1_PLB_TAttribute,
      Sl_addrAck => plb_1_Sl_addrAck(1),
      Sl_SSize => plb_1_Sl_SSize(2 to 3),
      Sl_wait => plb_1_Sl_wait(1),
      Sl_rearbitrate => plb_1_Sl_rearbitrate(1),
      Sl_wrDAck => plb_1_Sl_wrDAck(1),
      Sl_wrComp => plb_1_Sl_wrComp(1),
      Sl_wrBTerm => plb_1_Sl_wrBTerm(1),
      Sl_rdDBus => plb_1_Sl_rdDBus(64 to 127),
      Sl_rdWdAddr => plb_1_Sl_rdWdAddr(4 to 7),
      Sl_rdDAck => plb_1_Sl_rdDAck(1),
      Sl_rdComp => plb_1_Sl_rdComp(1),
      Sl_rdBTerm => plb_1_Sl_rdBTerm(1),
      Sl_MBusy => plb_1_Sl_MBusy(4 to 7),
      Sl_MWrErr => plb_1_Sl_MWrErr(4 to 7),
      Sl_MRdErr => plb_1_Sl_MRdErr(4 to 7),
      Sl_MIRQ => plb_1_Sl_MIRQ(4 to 7),
      Mem_DQ_I => xps_mch_emc_sram_Mem_DQ_I,
      Mem_DQ_O => xps_mch_emc_sram_Mem_DQ_O,
      Mem_DQ_T => xps_mch_emc_sram_Mem_DQ_T,
      Mem_A => xps_mch_emc_sram_Mem_A,
      Mem_RPN => open,
      Mem_CEN => xps_mch_emc_sram_Mem_CEN(0 to 0),
      Mem_OEN => xps_mch_emc_sram_Mem_OEN(0 to 0),
      Mem_WEN => xps_mch_emc_sram_Mem_WEN,
      Mem_QWEN => open,
      Mem_BEN => xps_mch_emc_sram_Mem_BEN,
      Mem_CE => open,
      Mem_ADV_LDN => xps_mch_emc_sram_Mem_ADV_LDN,
      Mem_LBON => xps_mch_emc_sram_Mem_LBON,
      Mem_CKEN => open,
      Mem_RNW => open
    );

  dcm_module_0 : dcm_module_0_wrapper
    port map (
      RST => usb_dcm_reset,
      CLKIN => usb_if_clk,
      CLKFB => usb_if_clk_shift,
      PSEN => net_gnd0,
      PSINCDEC => net_gnd0,
      PSCLK => net_gnd0,
      DSSEN => net_gnd0,
      CLK0 => usb_if_clk_shift,
      CLK90 => open,
      CLK180 => open,
      CLK270 => open,
      CLKDV => open,
      CLK2X => open,
      CLK2X180 => open,
      CLKFX => open,
      CLKFX180 => open,
      STATUS => open,
      LOCKED => usb_dcm_locked,
      PSDONE => open
    );

  plb_usb_0 : plb_usb_0_wrapper
    port map (
      dcm_locked => usb_dcm_locked,
      dcm_reset => usb_dcm_reset,
      if_clk => usb_if_clk_shift,
      usb_full_n => usb_full_n,
      usb_empty_n => usb_empty_n,
      usb_alive => usb_alive,
      Interrupt => usb_int,
      sloe_n => usb_sloe_n,
      slrd_n => usb_slrd_n,
      slwr_n => usb_slwr_n,
      pktend_n => usb_pktend_n,
      fifoaddr => usb_fifoaddr,
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_1_SPLB_Rst(2),
      PLB_ABus => plb_1_PLB_ABus,
      PLB_UABus => plb_1_PLB_UABus,
      PLB_PAValid => plb_1_PLB_PAValid,
      PLB_SAValid => plb_1_PLB_SAValid,
      PLB_rdPrim => plb_1_PLB_rdPrim(2),
      PLB_wrPrim => plb_1_PLB_wrPrim(2),
      PLB_masterID => plb_1_PLB_masterID,
      PLB_abort => plb_1_PLB_abort,
      PLB_busLock => plb_1_PLB_busLock,
      PLB_RNW => plb_1_PLB_RNW,
      PLB_BE => plb_1_PLB_BE,
      PLB_MSize => plb_1_PLB_MSize,
      PLB_size => plb_1_PLB_size,
      PLB_type => plb_1_PLB_type,
      PLB_lockErr => plb_1_PLB_lockErr,
      PLB_wrDBus => plb_1_PLB_wrDBus,
      PLB_wrBurst => plb_1_PLB_wrBurst,
      PLB_rdBurst => plb_1_PLB_rdBurst,
      PLB_wrPendReq => plb_1_PLB_wrPendReq,
      PLB_rdPendReq => plb_1_PLB_rdPendReq,
      PLB_wrPendPri => plb_1_PLB_wrPendPri,
      PLB_rdPendPri => plb_1_PLB_rdPendPri,
      PLB_reqPri => plb_1_PLB_reqPri,
      PLB_TAttribute => plb_1_PLB_TAttribute,
      Sl_addrAck => plb_1_Sl_addrAck(2),
      Sl_SSize => plb_1_Sl_SSize(4 to 5),
      Sl_wait => plb_1_Sl_wait(2),
      Sl_rearbitrate => plb_1_Sl_rearbitrate(2),
      Sl_wrDAck => plb_1_Sl_wrDAck(2),
      Sl_wrComp => plb_1_Sl_wrComp(2),
      Sl_wrBTerm => plb_1_Sl_wrBTerm(2),
      Sl_rdDBus => plb_1_Sl_rdDBus(128 to 191),
      Sl_rdWdAddr => plb_1_Sl_rdWdAddr(8 to 11),
      Sl_rdDAck => plb_1_Sl_rdDAck(2),
      Sl_rdComp => plb_1_Sl_rdComp(2),
      Sl_rdBTerm => plb_1_Sl_rdBTerm(2),
      Sl_MBusy => plb_1_Sl_MBusy(8 to 11),
      Sl_MWrErr => plb_1_Sl_MWrErr(8 to 11),
      Sl_MRdErr => plb_1_Sl_MRdErr(8 to 11),
      Sl_MIRQ => plb_1_Sl_MIRQ(8 to 11),
      MPLB_Clk => plb_clk_100MHz,
      MPLB_Rst => plb_1_MPLB_Rst(2),
      MD_error => open,
      M_request => plb_1_M_request(2),
      M_priority => plb_1_M_priority(4 to 5),
      M_busLock => plb_1_M_busLock(2),
      M_RNW => plb_1_M_RNW(2),
      M_BE => plb_1_M_BE(16 to 23),
      M_MSize => plb_1_M_MSize(4 to 5),
      M_size => plb_1_M_size(8 to 11),
      M_type => plb_1_M_type(6 to 8),
      M_TAttribute => plb_1_M_TAttribute(32 to 47),
      M_lockErr => plb_1_M_lockErr(2),
      M_abort => plb_1_M_abort(2),
      M_UABus => plb_1_M_UABus(64 to 95),
      M_ABus => plb_1_M_ABus(64 to 95),
      M_wrDBus => plb_1_M_wrDBus(128 to 191),
      M_wrBurst => plb_1_M_wrBurst(2),
      M_rdBurst => plb_1_M_rdBurst(2),
      PLB_MAddrAck => plb_1_PLB_MAddrAck(2),
      PLB_MSSize => plb_1_PLB_MSSize(4 to 5),
      PLB_MRearbitrate => plb_1_PLB_MRearbitrate(2),
      PLB_MTimeout => plb_1_PLB_MTimeout(2),
      PLB_MBusy => plb_1_PLB_MBusy(2),
      PLB_MRdErr => plb_1_PLB_MRdErr(2),
      PLB_MWrErr => plb_1_PLB_MWrErr(2),
      PLB_MIRQ => plb_1_PLB_MIRQ(2),
      PLB_MRdDBus => plb_1_PLB_MRdDBus(128 to 191),
      PLB_MRdWdAddr => plb_1_PLB_MRdWdAddr(8 to 11),
      PLB_MRdDAck => plb_1_PLB_MRdDAck(2),
      PLB_MRdBTerm => plb_1_PLB_MRdBTerm(2),
      PLB_MWrDAck => plb_1_PLB_MWrDAck(2),
      PLB_MWrBTerm => plb_1_PLB_MWrBTerm(2),
      fd_I => usb_fd_I,
      fd_O => usb_fd_O,
      fd_T => usb_fd_T
    );

  plb_vision_0 : plb_vision_0_wrapper
    port map (
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_1_SPLB_Rst(3),
      PLB_ABus => plb_1_PLB_ABus,
      PLB_UABus => plb_1_PLB_UABus,
      PLB_PAValid => plb_1_PLB_PAValid,
      PLB_SAValid => plb_1_PLB_SAValid,
      PLB_rdPrim => plb_1_PLB_rdPrim(3),
      PLB_wrPrim => plb_1_PLB_wrPrim(3),
      PLB_masterID => plb_1_PLB_masterID,
      PLB_abort => plb_1_PLB_abort,
      PLB_busLock => plb_1_PLB_busLock,
      PLB_RNW => plb_1_PLB_RNW,
      PLB_BE => plb_1_PLB_BE,
      PLB_MSize => plb_1_PLB_MSize,
      PLB_size => plb_1_PLB_size,
      PLB_type => plb_1_PLB_type,
      PLB_lockErr => plb_1_PLB_lockErr,
      PLB_wrDBus => plb_1_PLB_wrDBus,
      PLB_wrBurst => plb_1_PLB_wrBurst,
      PLB_rdBurst => plb_1_PLB_rdBurst,
      PLB_wrPendReq => plb_1_PLB_wrPendReq,
      PLB_rdPendReq => plb_1_PLB_rdPendReq,
      PLB_wrPendPri => plb_1_PLB_wrPendPri,
      PLB_rdPendPri => plb_1_PLB_rdPendPri,
      PLB_reqPri => plb_1_PLB_reqPri,
      PLB_TAttribute => plb_1_PLB_TAttribute,
      Sl_addrAck => plb_1_Sl_addrAck(3),
      Sl_SSize => plb_1_Sl_SSize(6 to 7),
      Sl_wait => plb_1_Sl_wait(3),
      Sl_rearbitrate => plb_1_Sl_rearbitrate(3),
      Sl_wrDAck => plb_1_Sl_wrDAck(3),
      Sl_wrComp => plb_1_Sl_wrComp(3),
      Sl_wrBTerm => plb_1_Sl_wrBTerm(3),
      Sl_rdDBus => plb_1_Sl_rdDBus(192 to 255),
      Sl_rdWdAddr => plb_1_Sl_rdWdAddr(12 to 15),
      Sl_rdDAck => plb_1_Sl_rdDAck(3),
      Sl_rdComp => plb_1_Sl_rdComp(3),
      Sl_rdBTerm => plb_1_Sl_rdBTerm(3),
      Sl_MBusy => plb_1_Sl_MBusy(12 to 15),
      Sl_MWrErr => plb_1_Sl_MWrErr(12 to 15),
      Sl_MRdErr => plb_1_Sl_MRdErr(12 to 15),
      Sl_MIRQ => plb_1_Sl_MIRQ(12 to 15),
      MPLB_Clk => plb_clk_100MHz,
      MPLB_Rst => plb_1_MPLB_Rst(3),
      MD_error => open,
      M_request => plb_1_M_request(3),
      M_priority => plb_1_M_priority(6 to 7),
      M_busLock => plb_1_M_busLock(3),
      M_RNW => plb_1_M_RNW(3),
      M_BE => plb_1_M_BE(24 to 31),
      M_MSize => plb_1_M_MSize(6 to 7),
      M_size => plb_1_M_size(12 to 15),
      M_type => plb_1_M_type(9 to 11),
      M_TAttribute => plb_1_M_TAttribute(48 to 63),
      M_lockErr => plb_1_M_lockErr(3),
      M_abort => plb_1_M_abort(3),
      M_UABus => plb_1_M_UABus(96 to 127),
      M_ABus => plb_1_M_ABus(96 to 127),
      M_wrDBus => plb_1_M_wrDBus(192 to 255),
      M_wrBurst => plb_1_M_wrBurst(3),
      M_rdBurst => plb_1_M_rdBurst(3),
      PLB_MAddrAck => plb_1_PLB_MAddrAck(3),
      PLB_MSSize => plb_1_PLB_MSSize(6 to 7),
      PLB_MRearbitrate => plb_1_PLB_MRearbitrate(3),
      PLB_MTimeout => plb_1_PLB_MTimeout(3),
      PLB_MBusy => plb_1_PLB_MBusy(3),
      PLB_MRdErr => plb_1_PLB_MRdErr(3),
      PLB_MWrErr => plb_1_PLB_MWrErr(3),
      PLB_MIRQ => plb_1_PLB_MIRQ(3),
      PLB_MRdDBus => plb_1_PLB_MRdDBus(192 to 255),
      PLB_MRdWdAddr => plb_1_PLB_MRdWdAddr(12 to 15),
      PLB_MRdDAck => plb_1_PLB_MRdDAck(3),
      PLB_MRdBTerm => plb_1_PLB_MRdBTerm(3),
      PLB_MWrDAck => plb_1_PLB_MWrDAck(3),
      PLB_MWrBTerm => plb_1_PLB_MWrBTerm(3),
      cam_data => cam0_data(7 downto 0),
      cam_frame_valid => cam0_frame_valid,
      cam_line_valid => cam0_line_valid,
      cam_pix_clk => cam0_pixclk,
      cam_sclk => cam0_sclk,
      cam_reset_n => cam0_reset_n,
      Interrupt => plb_vision_0_Interrupt,
      cam_sdata_I => cam0_sdata_I,
      cam_sdata_O => cam0_sdata_O,
      cam_sdata_T => cam0_sdata_T
    );

  mpmc_0 : mpmc_0_wrapper
    port map (
      FSL0_M_Clk => net_vcc0,
      FSL0_M_Write => net_gnd0,
      FSL0_M_Data => net_gnd32,
      FSL0_M_Control => net_gnd0,
      FSL0_M_Full => open,
      FSL0_S_Clk => net_gnd0,
      FSL0_S_Read => net_gnd0,
      FSL0_S_Data => open,
      FSL0_S_Control => open,
      FSL0_S_Exists => open,
      FSL0_B_M_Clk => net_vcc0,
      FSL0_B_M_Write => net_gnd0,
      FSL0_B_M_Data => net_gnd32,
      FSL0_B_M_Control => net_gnd0,
      FSL0_B_M_Full => open,
      FSL0_B_S_Clk => net_gnd0,
      FSL0_B_S_Read => net_gnd0,
      FSL0_B_S_Data => open,
      FSL0_B_S_Control => open,
      FSL0_B_S_Exists => open,
      SPLB0_Clk => plb_clk_100MHz,
      SPLB0_Rst => plb_0_SPLB_Rst(7),
      SPLB0_PLB_ABus => plb_0_PLB_ABus,
      SPLB0_PLB_PAValid => plb_0_PLB_PAValid,
      SPLB0_PLB_SAValid => plb_0_PLB_SAValid,
      SPLB0_PLB_masterID => plb_0_PLB_masterID(0 to 0),
      SPLB0_PLB_RNW => plb_0_PLB_RNW,
      SPLB0_PLB_BE => plb_0_PLB_BE,
      SPLB0_PLB_UABus => plb_0_PLB_UABus,
      SPLB0_PLB_rdPrim => plb_0_PLB_rdPrim(7),
      SPLB0_PLB_wrPrim => plb_0_PLB_wrPrim(7),
      SPLB0_PLB_abort => plb_0_PLB_abort,
      SPLB0_PLB_busLock => plb_0_PLB_busLock,
      SPLB0_PLB_MSize => plb_0_PLB_MSize,
      SPLB0_PLB_size => plb_0_PLB_size,
      SPLB0_PLB_type => plb_0_PLB_type,
      SPLB0_PLB_lockErr => plb_0_PLB_lockErr,
      SPLB0_PLB_wrPendReq => plb_0_PLB_wrPendReq,
      SPLB0_PLB_wrPendPri => plb_0_PLB_wrPendPri,
      SPLB0_PLB_rdPendReq => plb_0_PLB_rdPendReq,
      SPLB0_PLB_rdPendPri => plb_0_PLB_rdPendPri,
      SPLB0_PLB_reqPri => plb_0_PLB_reqPri,
      SPLB0_PLB_TAttribute => plb_0_PLB_TAttribute,
      SPLB0_PLB_rdBurst => plb_0_PLB_rdBurst,
      SPLB0_PLB_wrBurst => plb_0_PLB_wrBurst,
      SPLB0_PLB_wrDBus => plb_0_PLB_wrDBus,
      SPLB0_Sl_addrAck => plb_0_Sl_addrAck(7),
      SPLB0_Sl_SSize => plb_0_Sl_SSize(14 to 15),
      SPLB0_Sl_wait => plb_0_Sl_wait(7),
      SPLB0_Sl_rearbitrate => plb_0_Sl_rearbitrate(7),
      SPLB0_Sl_wrDAck => plb_0_Sl_wrDAck(7),
      SPLB0_Sl_wrComp => plb_0_Sl_wrComp(7),
      SPLB0_Sl_wrBTerm => plb_0_Sl_wrBTerm(7),
      SPLB0_Sl_rdDBus => plb_0_Sl_rdDBus(448 to 511),
      SPLB0_Sl_rdWdAddr => plb_0_Sl_rdWdAddr(28 to 31),
      SPLB0_Sl_rdDAck => plb_0_Sl_rdDAck(7),
      SPLB0_Sl_rdComp => plb_0_Sl_rdComp(7),
      SPLB0_Sl_rdBTerm => plb_0_Sl_rdBTerm(7),
      SPLB0_Sl_MBusy => plb_0_Sl_MBusy(14 to 15),
      SPLB0_Sl_MRdErr => plb_0_Sl_MRdErr(14 to 15),
      SPLB0_Sl_MWrErr => plb_0_Sl_MWrErr(14 to 15),
      SPLB0_Sl_MIRQ => plb_0_Sl_MIRQ(14 to 15),
      SDMA0_Clk => net_gnd0,
      SDMA0_Rx_IntOut => open,
      SDMA0_Tx_IntOut => open,
      SDMA0_RstOut => open,
      SDMA0_TX_D => open,
      SDMA0_TX_Rem => open,
      SDMA0_TX_SOF => open,
      SDMA0_TX_EOF => open,
      SDMA0_TX_SOP => open,
      SDMA0_TX_EOP => open,
      SDMA0_TX_Src_Rdy => open,
      SDMA0_TX_Dst_Rdy => net_vcc0,
      SDMA0_RX_D => net_gnd32,
      SDMA0_RX_Rem => net_vcc4,
      SDMA0_RX_SOF => net_vcc0,
      SDMA0_RX_EOF => net_vcc0,
      SDMA0_RX_SOP => net_vcc0,
      SDMA0_RX_EOP => net_vcc0,
      SDMA0_RX_Src_Rdy => net_vcc0,
      SDMA0_RX_Dst_Rdy => open,
      SDMA_CTRL0_Clk => net_vcc0,
      SDMA_CTRL0_Rst => net_gnd0,
      SDMA_CTRL0_PLB_ABus => net_gnd32,
      SDMA_CTRL0_PLB_PAValid => net_gnd0,
      SDMA_CTRL0_PLB_SAValid => net_gnd0,
      SDMA_CTRL0_PLB_masterID => net_gnd1(0 to 0),
      SDMA_CTRL0_PLB_RNW => net_gnd0,
      SDMA_CTRL0_PLB_BE => net_gnd8,
      SDMA_CTRL0_PLB_UABus => net_gnd32,
      SDMA_CTRL0_PLB_rdPrim => net_gnd0,
      SDMA_CTRL0_PLB_wrPrim => net_gnd0,
      SDMA_CTRL0_PLB_abort => net_gnd0,
      SDMA_CTRL0_PLB_busLock => net_gnd0,
      SDMA_CTRL0_PLB_MSize => net_gnd2,
      SDMA_CTRL0_PLB_size => net_gnd4,
      SDMA_CTRL0_PLB_type => net_gnd3,
      SDMA_CTRL0_PLB_lockErr => net_gnd0,
      SDMA_CTRL0_PLB_wrPendReq => net_gnd0,
      SDMA_CTRL0_PLB_wrPendPri => net_gnd2,
      SDMA_CTRL0_PLB_rdPendReq => net_gnd0,
      SDMA_CTRL0_PLB_rdPendPri => net_gnd2,
      SDMA_CTRL0_PLB_reqPri => net_gnd2,
      SDMA_CTRL0_PLB_TAttribute => net_gnd16,
      SDMA_CTRL0_PLB_rdBurst => net_gnd0,
      SDMA_CTRL0_PLB_wrBurst => net_gnd0,
      SDMA_CTRL0_PLB_wrDBus => net_gnd64,
      SDMA_CTRL0_Sl_addrAck => open,
      SDMA_CTRL0_Sl_SSize => open,
      SDMA_CTRL0_Sl_wait => open,
      SDMA_CTRL0_Sl_rearbitrate => open,
      SDMA_CTRL0_Sl_wrDAck => open,
      SDMA_CTRL0_Sl_wrComp => open,
      SDMA_CTRL0_Sl_wrBTerm => open,
      SDMA_CTRL0_Sl_rdDBus => open,
      SDMA_CTRL0_Sl_rdWdAddr => open,
      SDMA_CTRL0_Sl_rdDAck => open,
      SDMA_CTRL0_Sl_rdComp => open,
      SDMA_CTRL0_Sl_rdBTerm => open,
      SDMA_CTRL0_Sl_MBusy => open,
      SDMA_CTRL0_Sl_MRdErr => open,
      SDMA_CTRL0_Sl_MWrErr => open,
      SDMA_CTRL0_Sl_MIRQ => open,
      PIM0_Addr => net_gnd32(0 to 31),
      PIM0_AddrReq => net_gnd0,
      PIM0_AddrAck => open,
      PIM0_RNW => net_gnd0,
      PIM0_Size => net_gnd4(0 to 3),
      PIM0_RdModWr => net_gnd0,
      PIM0_WrFIFO_Data => net_gnd64(0 to 63),
      PIM0_WrFIFO_BE => net_gnd8(0 to 7),
      PIM0_WrFIFO_Push => net_gnd0,
      PIM0_RdFIFO_Data => open,
      PIM0_RdFIFO_Pop => net_gnd0,
      PIM0_RdFIFO_RdWdAddr => open,
      PIM0_WrFIFO_Empty => open,
      PIM0_WrFIFO_AlmostFull => open,
      PIM0_WrFIFO_Flush => net_gnd0,
      PIM0_RdFIFO_Empty => open,
      PIM0_RdFIFO_Flush => net_gnd0,
      PIM0_RdFIFO_Latency => open,
      PIM0_InitDone => open,
      PPC440MC0_MIMCReadNotWrite => net_gnd0,
      PPC440MC0_MIMCAddress => net_gnd36,
      PPC440MC0_MIMCAddressValid => net_gnd0,
      PPC440MC0_MIMCWriteData => net_gnd128,
      PPC440MC0_MIMCWriteDataValid => net_gnd0,
      PPC440MC0_MIMCByteEnable => net_gnd16,
      PPC440MC0_MIMCBankConflict => net_gnd0,
      PPC440MC0_MIMCRowConflict => net_gnd0,
      PPC440MC0_MCMIReadData => open,
      PPC440MC0_MCMIReadDataValid => open,
      PPC440MC0_MCMIReadDataErr => open,
      PPC440MC0_MCMIAddrReadyToAccept => open,
      VFBC0_Cmd_Clk => net_gnd0,
      VFBC0_Cmd_Reset => net_gnd0,
      VFBC0_Cmd_Data => net_gnd32(0 to 31),
      VFBC0_Cmd_Write => net_gnd0,
      VFBC0_Cmd_End => net_gnd0,
      VFBC0_Cmd_Full => open,
      VFBC0_Cmd_Almost_Full => open,
      VFBC0_Cmd_Idle => open,
      VFBC0_Wd_Clk => net_gnd0,
      VFBC0_Wd_Reset => net_gnd0,
      VFBC0_Wd_Write => net_gnd0,
      VFBC0_Wd_End_Burst => net_gnd0,
      VFBC0_Wd_Flush => net_gnd0,
      VFBC0_Wd_Data => net_gnd32(0 to 31),
      VFBC0_Wd_Data_BE => net_gnd4(0 to 3),
      VFBC0_Wd_Full => open,
      VFBC0_Wd_Almost_Full => open,
      VFBC0_Rd_Clk => net_gnd0,
      VFBC0_Rd_Reset => net_gnd0,
      VFBC0_Rd_Read => net_gnd0,
      VFBC0_Rd_End_Burst => net_gnd0,
      VFBC0_Rd_Flush => net_gnd0,
      VFBC0_Rd_Data => open,
      VFBC0_Rd_Empty => open,
      VFBC0_Rd_Almost_Empty => open,
      MCB0_cmd_clk => net_gnd0,
      MCB0_cmd_en => net_gnd0,
      MCB0_cmd_instr => net_gnd3(0 to 2),
      MCB0_cmd_bl => net_gnd6,
      MCB0_cmd_byte_addr => net_gnd30,
      MCB0_cmd_empty => open,
      MCB0_cmd_full => open,
      MCB0_wr_clk => net_gnd0,
      MCB0_wr_en => net_gnd0,
      MCB0_wr_mask => net_gnd8(0 to 7),
      MCB0_wr_data => net_gnd64(0 to 63),
      MCB0_wr_full => open,
      MCB0_wr_empty => open,
      MCB0_wr_count => open,
      MCB0_wr_underrun => open,
      MCB0_wr_error => open,
      MCB0_rd_clk => net_gnd0,
      MCB0_rd_en => net_gnd0,
      MCB0_rd_data => open,
      MCB0_rd_full => open,
      MCB0_rd_empty => open,
      MCB0_rd_count => open,
      MCB0_rd_overflow => open,
      MCB0_rd_error => open,
      FSL1_M_Clk => net_vcc0,
      FSL1_M_Write => net_gnd0,
      FSL1_M_Data => net_gnd32,
      FSL1_M_Control => net_gnd0,
      FSL1_M_Full => open,
      FSL1_S_Clk => net_gnd0,
      FSL1_S_Read => net_gnd0,
      FSL1_S_Data => open,
      FSL1_S_Control => open,
      FSL1_S_Exists => open,
      FSL1_B_M_Clk => net_vcc0,
      FSL1_B_M_Write => net_gnd0,
      FSL1_B_M_Data => net_gnd32,
      FSL1_B_M_Control => net_gnd0,
      FSL1_B_M_Full => open,
      FSL1_B_S_Clk => net_gnd0,
      FSL1_B_S_Read => net_gnd0,
      FSL1_B_S_Data => open,
      FSL1_B_S_Control => open,
      FSL1_B_S_Exists => open,
      SPLB1_Clk => plb_clk_100MHz,
      SPLB1_Rst => plb_1_SPLB_Rst(4),
      SPLB1_PLB_ABus => plb_1_PLB_ABus,
      SPLB1_PLB_PAValid => plb_1_PLB_PAValid,
      SPLB1_PLB_SAValid => plb_1_PLB_SAValid,
      SPLB1_PLB_masterID => plb_1_PLB_masterID,
      SPLB1_PLB_RNW => plb_1_PLB_RNW,
      SPLB1_PLB_BE => plb_1_PLB_BE,
      SPLB1_PLB_UABus => plb_1_PLB_UABus,
      SPLB1_PLB_rdPrim => plb_1_PLB_rdPrim(4),
      SPLB1_PLB_wrPrim => plb_1_PLB_wrPrim(4),
      SPLB1_PLB_abort => plb_1_PLB_abort,
      SPLB1_PLB_busLock => plb_1_PLB_busLock,
      SPLB1_PLB_MSize => plb_1_PLB_MSize,
      SPLB1_PLB_size => plb_1_PLB_size,
      SPLB1_PLB_type => plb_1_PLB_type,
      SPLB1_PLB_lockErr => plb_1_PLB_lockErr,
      SPLB1_PLB_wrPendReq => plb_1_PLB_wrPendReq,
      SPLB1_PLB_wrPendPri => plb_1_PLB_wrPendPri,
      SPLB1_PLB_rdPendReq => plb_1_PLB_rdPendReq,
      SPLB1_PLB_rdPendPri => plb_1_PLB_rdPendPri,
      SPLB1_PLB_reqPri => plb_1_PLB_reqPri,
      SPLB1_PLB_TAttribute => plb_1_PLB_TAttribute,
      SPLB1_PLB_rdBurst => plb_1_PLB_rdBurst,
      SPLB1_PLB_wrBurst => plb_1_PLB_wrBurst,
      SPLB1_PLB_wrDBus => plb_1_PLB_wrDBus,
      SPLB1_Sl_addrAck => plb_1_Sl_addrAck(4),
      SPLB1_Sl_SSize => plb_1_Sl_SSize(8 to 9),
      SPLB1_Sl_wait => plb_1_Sl_wait(4),
      SPLB1_Sl_rearbitrate => plb_1_Sl_rearbitrate(4),
      SPLB1_Sl_wrDAck => plb_1_Sl_wrDAck(4),
      SPLB1_Sl_wrComp => plb_1_Sl_wrComp(4),
      SPLB1_Sl_wrBTerm => plb_1_Sl_wrBTerm(4),
      SPLB1_Sl_rdDBus => plb_1_Sl_rdDBus(256 to 319),
      SPLB1_Sl_rdWdAddr => plb_1_Sl_rdWdAddr(16 to 19),
      SPLB1_Sl_rdDAck => plb_1_Sl_rdDAck(4),
      SPLB1_Sl_rdComp => plb_1_Sl_rdComp(4),
      SPLB1_Sl_rdBTerm => plb_1_Sl_rdBTerm(4),
      SPLB1_Sl_MBusy => plb_1_Sl_MBusy(16 to 19),
      SPLB1_Sl_MRdErr => plb_1_Sl_MRdErr(16 to 19),
      SPLB1_Sl_MWrErr => plb_1_Sl_MWrErr(16 to 19),
      SPLB1_Sl_MIRQ => plb_1_Sl_MIRQ(16 to 19),
      SDMA1_Clk => net_gnd0,
      SDMA1_Rx_IntOut => open,
      SDMA1_Tx_IntOut => open,
      SDMA1_RstOut => open,
      SDMA1_TX_D => open,
      SDMA1_TX_Rem => open,
      SDMA1_TX_SOF => open,
      SDMA1_TX_EOF => open,
      SDMA1_TX_SOP => open,
      SDMA1_TX_EOP => open,
      SDMA1_TX_Src_Rdy => open,
      SDMA1_TX_Dst_Rdy => net_vcc0,
      SDMA1_RX_D => net_gnd32,
      SDMA1_RX_Rem => net_vcc4,
      SDMA1_RX_SOF => net_vcc0,
      SDMA1_RX_EOF => net_vcc0,
      SDMA1_RX_SOP => net_vcc0,
      SDMA1_RX_EOP => net_vcc0,
      SDMA1_RX_Src_Rdy => net_vcc0,
      SDMA1_RX_Dst_Rdy => open,
      SDMA_CTRL1_Clk => net_vcc0,
      SDMA_CTRL1_Rst => net_gnd0,
      SDMA_CTRL1_PLB_ABus => net_gnd32,
      SDMA_CTRL1_PLB_PAValid => net_gnd0,
      SDMA_CTRL1_PLB_SAValid => net_gnd0,
      SDMA_CTRL1_PLB_masterID => net_gnd1(0 to 0),
      SDMA_CTRL1_PLB_RNW => net_gnd0,
      SDMA_CTRL1_PLB_BE => net_gnd8,
      SDMA_CTRL1_PLB_UABus => net_gnd32,
      SDMA_CTRL1_PLB_rdPrim => net_gnd0,
      SDMA_CTRL1_PLB_wrPrim => net_gnd0,
      SDMA_CTRL1_PLB_abort => net_gnd0,
      SDMA_CTRL1_PLB_busLock => net_gnd0,
      SDMA_CTRL1_PLB_MSize => net_gnd2,
      SDMA_CTRL1_PLB_size => net_gnd4,
      SDMA_CTRL1_PLB_type => net_gnd3,
      SDMA_CTRL1_PLB_lockErr => net_gnd0,
      SDMA_CTRL1_PLB_wrPendReq => net_gnd0,
      SDMA_CTRL1_PLB_wrPendPri => net_gnd2,
      SDMA_CTRL1_PLB_rdPendReq => net_gnd0,
      SDMA_CTRL1_PLB_rdPendPri => net_gnd2,
      SDMA_CTRL1_PLB_reqPri => net_gnd2,
      SDMA_CTRL1_PLB_TAttribute => net_gnd16,
      SDMA_CTRL1_PLB_rdBurst => net_gnd0,
      SDMA_CTRL1_PLB_wrBurst => net_gnd0,
      SDMA_CTRL1_PLB_wrDBus => net_gnd64,
      SDMA_CTRL1_Sl_addrAck => open,
      SDMA_CTRL1_Sl_SSize => open,
      SDMA_CTRL1_Sl_wait => open,
      SDMA_CTRL1_Sl_rearbitrate => open,
      SDMA_CTRL1_Sl_wrDAck => open,
      SDMA_CTRL1_Sl_wrComp => open,
      SDMA_CTRL1_Sl_wrBTerm => open,
      SDMA_CTRL1_Sl_rdDBus => open,
      SDMA_CTRL1_Sl_rdWdAddr => open,
      SDMA_CTRL1_Sl_rdDAck => open,
      SDMA_CTRL1_Sl_rdComp => open,
      SDMA_CTRL1_Sl_rdBTerm => open,
      SDMA_CTRL1_Sl_MBusy => open,
      SDMA_CTRL1_Sl_MRdErr => open,
      SDMA_CTRL1_Sl_MWrErr => open,
      SDMA_CTRL1_Sl_MIRQ => open,
      PIM1_Addr => net_gnd32(0 to 31),
      PIM1_AddrReq => net_gnd0,
      PIM1_AddrAck => open,
      PIM1_RNW => net_gnd0,
      PIM1_Size => net_gnd4(0 to 3),
      PIM1_RdModWr => net_gnd0,
      PIM1_WrFIFO_Data => net_gnd64(0 to 63),
      PIM1_WrFIFO_BE => net_gnd8(0 to 7),
      PIM1_WrFIFO_Push => net_gnd0,
      PIM1_RdFIFO_Data => open,
      PIM1_RdFIFO_Pop => net_gnd0,
      PIM1_RdFIFO_RdWdAddr => open,
      PIM1_WrFIFO_Empty => open,
      PIM1_WrFIFO_AlmostFull => open,
      PIM1_WrFIFO_Flush => net_gnd0,
      PIM1_RdFIFO_Empty => open,
      PIM1_RdFIFO_Flush => net_gnd0,
      PIM1_RdFIFO_Latency => open,
      PIM1_InitDone => open,
      PPC440MC1_MIMCReadNotWrite => net_gnd0,
      PPC440MC1_MIMCAddress => net_gnd36,
      PPC440MC1_MIMCAddressValid => net_gnd0,
      PPC440MC1_MIMCWriteData => net_gnd128,
      PPC440MC1_MIMCWriteDataValid => net_gnd0,
      PPC440MC1_MIMCByteEnable => net_gnd16,
      PPC440MC1_MIMCBankConflict => net_gnd0,
      PPC440MC1_MIMCRowConflict => net_gnd0,
      PPC440MC1_MCMIReadData => open,
      PPC440MC1_MCMIReadDataValid => open,
      PPC440MC1_MCMIReadDataErr => open,
      PPC440MC1_MCMIAddrReadyToAccept => open,
      VFBC1_Cmd_Clk => net_gnd0,
      VFBC1_Cmd_Reset => net_gnd0,
      VFBC1_Cmd_Data => net_gnd32(0 to 31),
      VFBC1_Cmd_Write => net_gnd0,
      VFBC1_Cmd_End => net_gnd0,
      VFBC1_Cmd_Full => open,
      VFBC1_Cmd_Almost_Full => open,
      VFBC1_Cmd_Idle => open,
      VFBC1_Wd_Clk => net_gnd0,
      VFBC1_Wd_Reset => net_gnd0,
      VFBC1_Wd_Write => net_gnd0,
      VFBC1_Wd_End_Burst => net_gnd0,
      VFBC1_Wd_Flush => net_gnd0,
      VFBC1_Wd_Data => net_gnd32(0 to 31),
      VFBC1_Wd_Data_BE => net_gnd4(0 to 3),
      VFBC1_Wd_Full => open,
      VFBC1_Wd_Almost_Full => open,
      VFBC1_Rd_Clk => net_gnd0,
      VFBC1_Rd_Reset => net_gnd0,
      VFBC1_Rd_Read => net_gnd0,
      VFBC1_Rd_End_Burst => net_gnd0,
      VFBC1_Rd_Flush => net_gnd0,
      VFBC1_Rd_Data => open,
      VFBC1_Rd_Empty => open,
      VFBC1_Rd_Almost_Empty => open,
      MCB1_cmd_clk => net_gnd0,
      MCB1_cmd_en => net_gnd0,
      MCB1_cmd_instr => net_gnd3(0 to 2),
      MCB1_cmd_bl => net_gnd6,
      MCB1_cmd_byte_addr => net_gnd30,
      MCB1_cmd_empty => open,
      MCB1_cmd_full => open,
      MCB1_wr_clk => net_gnd0,
      MCB1_wr_en => net_gnd0,
      MCB1_wr_mask => net_gnd8(0 to 7),
      MCB1_wr_data => net_gnd64(0 to 63),
      MCB1_wr_full => open,
      MCB1_wr_empty => open,
      MCB1_wr_count => open,
      MCB1_wr_underrun => open,
      MCB1_wr_error => open,
      MCB1_rd_clk => net_gnd0,
      MCB1_rd_en => net_gnd0,
      MCB1_rd_data => open,
      MCB1_rd_full => open,
      MCB1_rd_empty => open,
      MCB1_rd_count => open,
      MCB1_rd_overflow => open,
      MCB1_rd_error => open,
      FSL2_M_Clk => net_vcc0,
      FSL2_M_Write => net_gnd0,
      FSL2_M_Data => net_gnd32,
      FSL2_M_Control => net_gnd0,
      FSL2_M_Full => open,
      FSL2_S_Clk => net_gnd0,
      FSL2_S_Read => net_gnd0,
      FSL2_S_Data => open,
      FSL2_S_Control => open,
      FSL2_S_Exists => open,
      FSL2_B_M_Clk => net_vcc0,
      FSL2_B_M_Write => net_gnd0,
      FSL2_B_M_Data => net_gnd32,
      FSL2_B_M_Control => net_gnd0,
      FSL2_B_M_Full => open,
      FSL2_B_S_Clk => net_gnd0,
      FSL2_B_S_Read => net_gnd0,
      FSL2_B_S_Data => open,
      FSL2_B_S_Control => open,
      FSL2_B_S_Exists => open,
      SPLB2_Clk => net_vcc0,
      SPLB2_Rst => net_gnd0,
      SPLB2_PLB_ABus => net_gnd32,
      SPLB2_PLB_PAValid => net_gnd0,
      SPLB2_PLB_SAValid => net_gnd0,
      SPLB2_PLB_masterID => net_gnd1(0 to 0),
      SPLB2_PLB_RNW => net_gnd0,
      SPLB2_PLB_BE => net_gnd8,
      SPLB2_PLB_UABus => net_gnd32,
      SPLB2_PLB_rdPrim => net_gnd0,
      SPLB2_PLB_wrPrim => net_gnd0,
      SPLB2_PLB_abort => net_gnd0,
      SPLB2_PLB_busLock => net_gnd0,
      SPLB2_PLB_MSize => net_gnd2,
      SPLB2_PLB_size => net_gnd4,
      SPLB2_PLB_type => net_gnd3,
      SPLB2_PLB_lockErr => net_gnd0,
      SPLB2_PLB_wrPendReq => net_gnd0,
      SPLB2_PLB_wrPendPri => net_gnd2,
      SPLB2_PLB_rdPendReq => net_gnd0,
      SPLB2_PLB_rdPendPri => net_gnd2,
      SPLB2_PLB_reqPri => net_gnd2,
      SPLB2_PLB_TAttribute => net_gnd16,
      SPLB2_PLB_rdBurst => net_gnd0,
      SPLB2_PLB_wrBurst => net_gnd0,
      SPLB2_PLB_wrDBus => net_gnd64,
      SPLB2_Sl_addrAck => open,
      SPLB2_Sl_SSize => open,
      SPLB2_Sl_wait => open,
      SPLB2_Sl_rearbitrate => open,
      SPLB2_Sl_wrDAck => open,
      SPLB2_Sl_wrComp => open,
      SPLB2_Sl_wrBTerm => open,
      SPLB2_Sl_rdDBus => open,
      SPLB2_Sl_rdWdAddr => open,
      SPLB2_Sl_rdDAck => open,
      SPLB2_Sl_rdComp => open,
      SPLB2_Sl_rdBTerm => open,
      SPLB2_Sl_MBusy => open,
      SPLB2_Sl_MRdErr => open,
      SPLB2_Sl_MWrErr => open,
      SPLB2_Sl_MIRQ => open,
      SDMA2_Clk => net_gnd0,
      SDMA2_Rx_IntOut => open,
      SDMA2_Tx_IntOut => open,
      SDMA2_RstOut => open,
      SDMA2_TX_D => open,
      SDMA2_TX_Rem => open,
      SDMA2_TX_SOF => open,
      SDMA2_TX_EOF => open,
      SDMA2_TX_SOP => open,
      SDMA2_TX_EOP => open,
      SDMA2_TX_Src_Rdy => open,
      SDMA2_TX_Dst_Rdy => net_vcc0,
      SDMA2_RX_D => net_gnd32,
      SDMA2_RX_Rem => net_vcc4,
      SDMA2_RX_SOF => net_vcc0,
      SDMA2_RX_EOF => net_vcc0,
      SDMA2_RX_SOP => net_vcc0,
      SDMA2_RX_EOP => net_vcc0,
      SDMA2_RX_Src_Rdy => net_vcc0,
      SDMA2_RX_Dst_Rdy => open,
      SDMA_CTRL2_Clk => net_vcc0,
      SDMA_CTRL2_Rst => net_gnd0,
      SDMA_CTRL2_PLB_ABus => net_gnd32,
      SDMA_CTRL2_PLB_PAValid => net_gnd0,
      SDMA_CTRL2_PLB_SAValid => net_gnd0,
      SDMA_CTRL2_PLB_masterID => net_gnd1(0 to 0),
      SDMA_CTRL2_PLB_RNW => net_gnd0,
      SDMA_CTRL2_PLB_BE => net_gnd8,
      SDMA_CTRL2_PLB_UABus => net_gnd32,
      SDMA_CTRL2_PLB_rdPrim => net_gnd0,
      SDMA_CTRL2_PLB_wrPrim => net_gnd0,
      SDMA_CTRL2_PLB_abort => net_gnd0,
      SDMA_CTRL2_PLB_busLock => net_gnd0,
      SDMA_CTRL2_PLB_MSize => net_gnd2,
      SDMA_CTRL2_PLB_size => net_gnd4,
      SDMA_CTRL2_PLB_type => net_gnd3,
      SDMA_CTRL2_PLB_lockErr => net_gnd0,
      SDMA_CTRL2_PLB_wrPendReq => net_gnd0,
      SDMA_CTRL2_PLB_wrPendPri => net_gnd2,
      SDMA_CTRL2_PLB_rdPendReq => net_gnd0,
      SDMA_CTRL2_PLB_rdPendPri => net_gnd2,
      SDMA_CTRL2_PLB_reqPri => net_gnd2,
      SDMA_CTRL2_PLB_TAttribute => net_gnd16,
      SDMA_CTRL2_PLB_rdBurst => net_gnd0,
      SDMA_CTRL2_PLB_wrBurst => net_gnd0,
      SDMA_CTRL2_PLB_wrDBus => net_gnd64,
      SDMA_CTRL2_Sl_addrAck => open,
      SDMA_CTRL2_Sl_SSize => open,
      SDMA_CTRL2_Sl_wait => open,
      SDMA_CTRL2_Sl_rearbitrate => open,
      SDMA_CTRL2_Sl_wrDAck => open,
      SDMA_CTRL2_Sl_wrComp => open,
      SDMA_CTRL2_Sl_wrBTerm => open,
      SDMA_CTRL2_Sl_rdDBus => open,
      SDMA_CTRL2_Sl_rdWdAddr => open,
      SDMA_CTRL2_Sl_rdDAck => open,
      SDMA_CTRL2_Sl_rdComp => open,
      SDMA_CTRL2_Sl_rdBTerm => open,
      SDMA_CTRL2_Sl_MBusy => open,
      SDMA_CTRL2_Sl_MRdErr => open,
      SDMA_CTRL2_Sl_MWrErr => open,
      SDMA_CTRL2_Sl_MIRQ => open,
      PIM2_Addr => net_gnd32(0 to 31),
      PIM2_AddrReq => net_gnd0,
      PIM2_AddrAck => open,
      PIM2_RNW => net_gnd0,
      PIM2_Size => net_gnd4(0 to 3),
      PIM2_RdModWr => net_gnd0,
      PIM2_WrFIFO_Data => net_gnd64(0 to 63),
      PIM2_WrFIFO_BE => net_gnd8(0 to 7),
      PIM2_WrFIFO_Push => net_gnd0,
      PIM2_RdFIFO_Data => open,
      PIM2_RdFIFO_Pop => net_gnd0,
      PIM2_RdFIFO_RdWdAddr => open,
      PIM2_WrFIFO_Empty => open,
      PIM2_WrFIFO_AlmostFull => open,
      PIM2_WrFIFO_Flush => net_gnd0,
      PIM2_RdFIFO_Empty => open,
      PIM2_RdFIFO_Flush => net_gnd0,
      PIM2_RdFIFO_Latency => open,
      PIM2_InitDone => open,
      PPC440MC2_MIMCReadNotWrite => net_gnd0,
      PPC440MC2_MIMCAddress => net_gnd36,
      PPC440MC2_MIMCAddressValid => net_gnd0,
      PPC440MC2_MIMCWriteData => net_gnd128,
      PPC440MC2_MIMCWriteDataValid => net_gnd0,
      PPC440MC2_MIMCByteEnable => net_gnd16,
      PPC440MC2_MIMCBankConflict => net_gnd0,
      PPC440MC2_MIMCRowConflict => net_gnd0,
      PPC440MC2_MCMIReadData => open,
      PPC440MC2_MCMIReadDataValid => open,
      PPC440MC2_MCMIReadDataErr => open,
      PPC440MC2_MCMIAddrReadyToAccept => open,
      VFBC2_Cmd_Clk => net_gnd0,
      VFBC2_Cmd_Reset => net_gnd0,
      VFBC2_Cmd_Data => net_gnd32(0 to 31),
      VFBC2_Cmd_Write => net_gnd0,
      VFBC2_Cmd_End => net_gnd0,
      VFBC2_Cmd_Full => open,
      VFBC2_Cmd_Almost_Full => open,
      VFBC2_Cmd_Idle => open,
      VFBC2_Wd_Clk => net_gnd0,
      VFBC2_Wd_Reset => net_gnd0,
      VFBC2_Wd_Write => net_gnd0,
      VFBC2_Wd_End_Burst => net_gnd0,
      VFBC2_Wd_Flush => net_gnd0,
      VFBC2_Wd_Data => net_gnd32(0 to 31),
      VFBC2_Wd_Data_BE => net_gnd4(0 to 3),
      VFBC2_Wd_Full => open,
      VFBC2_Wd_Almost_Full => open,
      VFBC2_Rd_Clk => net_gnd0,
      VFBC2_Rd_Reset => net_gnd0,
      VFBC2_Rd_Read => net_gnd0,
      VFBC2_Rd_End_Burst => net_gnd0,
      VFBC2_Rd_Flush => net_gnd0,
      VFBC2_Rd_Data => open,
      VFBC2_Rd_Empty => open,
      VFBC2_Rd_Almost_Empty => open,
      MCB2_cmd_clk => net_gnd0,
      MCB2_cmd_en => net_gnd0,
      MCB2_cmd_instr => net_gnd3(0 to 2),
      MCB2_cmd_bl => net_gnd6,
      MCB2_cmd_byte_addr => net_gnd30,
      MCB2_cmd_empty => open,
      MCB2_cmd_full => open,
      MCB2_wr_clk => net_gnd0,
      MCB2_wr_en => net_gnd0,
      MCB2_wr_mask => net_gnd8(0 to 7),
      MCB2_wr_data => net_gnd64(0 to 63),
      MCB2_wr_full => open,
      MCB2_wr_empty => open,
      MCB2_wr_count => open,
      MCB2_wr_underrun => open,
      MCB2_wr_error => open,
      MCB2_rd_clk => net_gnd0,
      MCB2_rd_en => net_gnd0,
      MCB2_rd_data => open,
      MCB2_rd_full => open,
      MCB2_rd_empty => open,
      MCB2_rd_count => open,
      MCB2_rd_overflow => open,
      MCB2_rd_error => open,
      FSL3_M_Clk => net_vcc0,
      FSL3_M_Write => net_gnd0,
      FSL3_M_Data => net_gnd32,
      FSL3_M_Control => net_gnd0,
      FSL3_M_Full => open,
      FSL3_S_Clk => net_gnd0,
      FSL3_S_Read => net_gnd0,
      FSL3_S_Data => open,
      FSL3_S_Control => open,
      FSL3_S_Exists => open,
      FSL3_B_M_Clk => net_vcc0,
      FSL3_B_M_Write => net_gnd0,
      FSL3_B_M_Data => net_gnd32,
      FSL3_B_M_Control => net_gnd0,
      FSL3_B_M_Full => open,
      FSL3_B_S_Clk => net_gnd0,
      FSL3_B_S_Read => net_gnd0,
      FSL3_B_S_Data => open,
      FSL3_B_S_Control => open,
      FSL3_B_S_Exists => open,
      SPLB3_Clk => net_vcc0,
      SPLB3_Rst => net_gnd0,
      SPLB3_PLB_ABus => net_gnd32,
      SPLB3_PLB_PAValid => net_gnd0,
      SPLB3_PLB_SAValid => net_gnd0,
      SPLB3_PLB_masterID => net_gnd1(0 to 0),
      SPLB3_PLB_RNW => net_gnd0,
      SPLB3_PLB_BE => net_gnd8,
      SPLB3_PLB_UABus => net_gnd32,
      SPLB3_PLB_rdPrim => net_gnd0,
      SPLB3_PLB_wrPrim => net_gnd0,
      SPLB3_PLB_abort => net_gnd0,
      SPLB3_PLB_busLock => net_gnd0,
      SPLB3_PLB_MSize => net_gnd2,
      SPLB3_PLB_size => net_gnd4,
      SPLB3_PLB_type => net_gnd3,
      SPLB3_PLB_lockErr => net_gnd0,
      SPLB3_PLB_wrPendReq => net_gnd0,
      SPLB3_PLB_wrPendPri => net_gnd2,
      SPLB3_PLB_rdPendReq => net_gnd0,
      SPLB3_PLB_rdPendPri => net_gnd2,
      SPLB3_PLB_reqPri => net_gnd2,
      SPLB3_PLB_TAttribute => net_gnd16,
      SPLB3_PLB_rdBurst => net_gnd0,
      SPLB3_PLB_wrBurst => net_gnd0,
      SPLB3_PLB_wrDBus => net_gnd64,
      SPLB3_Sl_addrAck => open,
      SPLB3_Sl_SSize => open,
      SPLB3_Sl_wait => open,
      SPLB3_Sl_rearbitrate => open,
      SPLB3_Sl_wrDAck => open,
      SPLB3_Sl_wrComp => open,
      SPLB3_Sl_wrBTerm => open,
      SPLB3_Sl_rdDBus => open,
      SPLB3_Sl_rdWdAddr => open,
      SPLB3_Sl_rdDAck => open,
      SPLB3_Sl_rdComp => open,
      SPLB3_Sl_rdBTerm => open,
      SPLB3_Sl_MBusy => open,
      SPLB3_Sl_MRdErr => open,
      SPLB3_Sl_MWrErr => open,
      SPLB3_Sl_MIRQ => open,
      SDMA3_Clk => net_gnd0,
      SDMA3_Rx_IntOut => open,
      SDMA3_Tx_IntOut => open,
      SDMA3_RstOut => open,
      SDMA3_TX_D => open,
      SDMA3_TX_Rem => open,
      SDMA3_TX_SOF => open,
      SDMA3_TX_EOF => open,
      SDMA3_TX_SOP => open,
      SDMA3_TX_EOP => open,
      SDMA3_TX_Src_Rdy => open,
      SDMA3_TX_Dst_Rdy => net_vcc0,
      SDMA3_RX_D => net_gnd32,
      SDMA3_RX_Rem => net_vcc4,
      SDMA3_RX_SOF => net_vcc0,
      SDMA3_RX_EOF => net_vcc0,
      SDMA3_RX_SOP => net_vcc0,
      SDMA3_RX_EOP => net_vcc0,
      SDMA3_RX_Src_Rdy => net_vcc0,
      SDMA3_RX_Dst_Rdy => open,
      SDMA_CTRL3_Clk => net_vcc0,
      SDMA_CTRL3_Rst => net_gnd0,
      SDMA_CTRL3_PLB_ABus => net_gnd32,
      SDMA_CTRL3_PLB_PAValid => net_gnd0,
      SDMA_CTRL3_PLB_SAValid => net_gnd0,
      SDMA_CTRL3_PLB_masterID => net_gnd1(0 to 0),
      SDMA_CTRL3_PLB_RNW => net_gnd0,
      SDMA_CTRL3_PLB_BE => net_gnd8,
      SDMA_CTRL3_PLB_UABus => net_gnd32,
      SDMA_CTRL3_PLB_rdPrim => net_gnd0,
      SDMA_CTRL3_PLB_wrPrim => net_gnd0,
      SDMA_CTRL3_PLB_abort => net_gnd0,
      SDMA_CTRL3_PLB_busLock => net_gnd0,
      SDMA_CTRL3_PLB_MSize => net_gnd2,
      SDMA_CTRL3_PLB_size => net_gnd4,
      SDMA_CTRL3_PLB_type => net_gnd3,
      SDMA_CTRL3_PLB_lockErr => net_gnd0,
      SDMA_CTRL3_PLB_wrPendReq => net_gnd0,
      SDMA_CTRL3_PLB_wrPendPri => net_gnd2,
      SDMA_CTRL3_PLB_rdPendReq => net_gnd0,
      SDMA_CTRL3_PLB_rdPendPri => net_gnd2,
      SDMA_CTRL3_PLB_reqPri => net_gnd2,
      SDMA_CTRL3_PLB_TAttribute => net_gnd16,
      SDMA_CTRL3_PLB_rdBurst => net_gnd0,
      SDMA_CTRL3_PLB_wrBurst => net_gnd0,
      SDMA_CTRL3_PLB_wrDBus => net_gnd64,
      SDMA_CTRL3_Sl_addrAck => open,
      SDMA_CTRL3_Sl_SSize => open,
      SDMA_CTRL3_Sl_wait => open,
      SDMA_CTRL3_Sl_rearbitrate => open,
      SDMA_CTRL3_Sl_wrDAck => open,
      SDMA_CTRL3_Sl_wrComp => open,
      SDMA_CTRL3_Sl_wrBTerm => open,
      SDMA_CTRL3_Sl_rdDBus => open,
      SDMA_CTRL3_Sl_rdWdAddr => open,
      SDMA_CTRL3_Sl_rdDAck => open,
      SDMA_CTRL3_Sl_rdComp => open,
      SDMA_CTRL3_Sl_rdBTerm => open,
      SDMA_CTRL3_Sl_MBusy => open,
      SDMA_CTRL3_Sl_MRdErr => open,
      SDMA_CTRL3_Sl_MWrErr => open,
      SDMA_CTRL3_Sl_MIRQ => open,
      PIM3_Addr => net_gnd32(0 to 31),
      PIM3_AddrReq => net_gnd0,
      PIM3_AddrAck => open,
      PIM3_RNW => net_gnd0,
      PIM3_Size => net_gnd4(0 to 3),
      PIM3_RdModWr => net_gnd0,
      PIM3_WrFIFO_Data => net_gnd64(0 to 63),
      PIM3_WrFIFO_BE => net_gnd8(0 to 7),
      PIM3_WrFIFO_Push => net_gnd0,
      PIM3_RdFIFO_Data => open,
      PIM3_RdFIFO_Pop => net_gnd0,
      PIM3_RdFIFO_RdWdAddr => open,
      PIM3_WrFIFO_Empty => open,
      PIM3_WrFIFO_AlmostFull => open,
      PIM3_WrFIFO_Flush => net_gnd0,
      PIM3_RdFIFO_Empty => open,
      PIM3_RdFIFO_Flush => net_gnd0,
      PIM3_RdFIFO_Latency => open,
      PIM3_InitDone => open,
      PPC440MC3_MIMCReadNotWrite => net_gnd0,
      PPC440MC3_MIMCAddress => net_gnd36,
      PPC440MC3_MIMCAddressValid => net_gnd0,
      PPC440MC3_MIMCWriteData => net_gnd128,
      PPC440MC3_MIMCWriteDataValid => net_gnd0,
      PPC440MC3_MIMCByteEnable => net_gnd16,
      PPC440MC3_MIMCBankConflict => net_gnd0,
      PPC440MC3_MIMCRowConflict => net_gnd0,
      PPC440MC3_MCMIReadData => open,
      PPC440MC3_MCMIReadDataValid => open,
      PPC440MC3_MCMIReadDataErr => open,
      PPC440MC3_MCMIAddrReadyToAccept => open,
      VFBC3_Cmd_Clk => net_gnd0,
      VFBC3_Cmd_Reset => net_gnd0,
      VFBC3_Cmd_Data => net_gnd32(0 to 31),
      VFBC3_Cmd_Write => net_gnd0,
      VFBC3_Cmd_End => net_gnd0,
      VFBC3_Cmd_Full => open,
      VFBC3_Cmd_Almost_Full => open,
      VFBC3_Cmd_Idle => open,
      VFBC3_Wd_Clk => net_gnd0,
      VFBC3_Wd_Reset => net_gnd0,
      VFBC3_Wd_Write => net_gnd0,
      VFBC3_Wd_End_Burst => net_gnd0,
      VFBC3_Wd_Flush => net_gnd0,
      VFBC3_Wd_Data => net_gnd32(0 to 31),
      VFBC3_Wd_Data_BE => net_gnd4(0 to 3),
      VFBC3_Wd_Full => open,
      VFBC3_Wd_Almost_Full => open,
      VFBC3_Rd_Clk => net_gnd0,
      VFBC3_Rd_Reset => net_gnd0,
      VFBC3_Rd_Read => net_gnd0,
      VFBC3_Rd_End_Burst => net_gnd0,
      VFBC3_Rd_Flush => net_gnd0,
      VFBC3_Rd_Data => open,
      VFBC3_Rd_Empty => open,
      VFBC3_Rd_Almost_Empty => open,
      MCB3_cmd_clk => net_gnd0,
      MCB3_cmd_en => net_gnd0,
      MCB3_cmd_instr => net_gnd3(0 to 2),
      MCB3_cmd_bl => net_gnd6,
      MCB3_cmd_byte_addr => net_gnd30,
      MCB3_cmd_empty => open,
      MCB3_cmd_full => open,
      MCB3_wr_clk => net_gnd0,
      MCB3_wr_en => net_gnd0,
      MCB3_wr_mask => net_gnd8(0 to 7),
      MCB3_wr_data => net_gnd64(0 to 63),
      MCB3_wr_full => open,
      MCB3_wr_empty => open,
      MCB3_wr_count => open,
      MCB3_wr_underrun => open,
      MCB3_wr_error => open,
      MCB3_rd_clk => net_gnd0,
      MCB3_rd_en => net_gnd0,
      MCB3_rd_data => open,
      MCB3_rd_full => open,
      MCB3_rd_empty => open,
      MCB3_rd_count => open,
      MCB3_rd_overflow => open,
      MCB3_rd_error => open,
      FSL4_M_Clk => net_vcc0,
      FSL4_M_Write => net_gnd0,
      FSL4_M_Data => net_gnd32,
      FSL4_M_Control => net_gnd0,
      FSL4_M_Full => open,
      FSL4_S_Clk => net_gnd0,
      FSL4_S_Read => net_gnd0,
      FSL4_S_Data => open,
      FSL4_S_Control => open,
      FSL4_S_Exists => open,
      FSL4_B_M_Clk => net_vcc0,
      FSL4_B_M_Write => net_gnd0,
      FSL4_B_M_Data => net_gnd32,
      FSL4_B_M_Control => net_gnd0,
      FSL4_B_M_Full => open,
      FSL4_B_S_Clk => net_gnd0,
      FSL4_B_S_Read => net_gnd0,
      FSL4_B_S_Data => open,
      FSL4_B_S_Control => open,
      FSL4_B_S_Exists => open,
      SPLB4_Clk => net_vcc0,
      SPLB4_Rst => net_gnd0,
      SPLB4_PLB_ABus => net_gnd32,
      SPLB4_PLB_PAValid => net_gnd0,
      SPLB4_PLB_SAValid => net_gnd0,
      SPLB4_PLB_masterID => net_gnd1(0 to 0),
      SPLB4_PLB_RNW => net_gnd0,
      SPLB4_PLB_BE => net_gnd8,
      SPLB4_PLB_UABus => net_gnd32,
      SPLB4_PLB_rdPrim => net_gnd0,
      SPLB4_PLB_wrPrim => net_gnd0,
      SPLB4_PLB_abort => net_gnd0,
      SPLB4_PLB_busLock => net_gnd0,
      SPLB4_PLB_MSize => net_gnd2,
      SPLB4_PLB_size => net_gnd4,
      SPLB4_PLB_type => net_gnd3,
      SPLB4_PLB_lockErr => net_gnd0,
      SPLB4_PLB_wrPendReq => net_gnd0,
      SPLB4_PLB_wrPendPri => net_gnd2,
      SPLB4_PLB_rdPendReq => net_gnd0,
      SPLB4_PLB_rdPendPri => net_gnd2,
      SPLB4_PLB_reqPri => net_gnd2,
      SPLB4_PLB_TAttribute => net_gnd16,
      SPLB4_PLB_rdBurst => net_gnd0,
      SPLB4_PLB_wrBurst => net_gnd0,
      SPLB4_PLB_wrDBus => net_gnd64,
      SPLB4_Sl_addrAck => open,
      SPLB4_Sl_SSize => open,
      SPLB4_Sl_wait => open,
      SPLB4_Sl_rearbitrate => open,
      SPLB4_Sl_wrDAck => open,
      SPLB4_Sl_wrComp => open,
      SPLB4_Sl_wrBTerm => open,
      SPLB4_Sl_rdDBus => open,
      SPLB4_Sl_rdWdAddr => open,
      SPLB4_Sl_rdDAck => open,
      SPLB4_Sl_rdComp => open,
      SPLB4_Sl_rdBTerm => open,
      SPLB4_Sl_MBusy => open,
      SPLB4_Sl_MRdErr => open,
      SPLB4_Sl_MWrErr => open,
      SPLB4_Sl_MIRQ => open,
      SDMA4_Clk => net_gnd0,
      SDMA4_Rx_IntOut => open,
      SDMA4_Tx_IntOut => open,
      SDMA4_RstOut => open,
      SDMA4_TX_D => open,
      SDMA4_TX_Rem => open,
      SDMA4_TX_SOF => open,
      SDMA4_TX_EOF => open,
      SDMA4_TX_SOP => open,
      SDMA4_TX_EOP => open,
      SDMA4_TX_Src_Rdy => open,
      SDMA4_TX_Dst_Rdy => net_vcc0,
      SDMA4_RX_D => net_gnd32,
      SDMA4_RX_Rem => net_vcc4,
      SDMA4_RX_SOF => net_vcc0,
      SDMA4_RX_EOF => net_vcc0,
      SDMA4_RX_SOP => net_vcc0,
      SDMA4_RX_EOP => net_vcc0,
      SDMA4_RX_Src_Rdy => net_vcc0,
      SDMA4_RX_Dst_Rdy => open,
      SDMA_CTRL4_Clk => net_vcc0,
      SDMA_CTRL4_Rst => net_gnd0,
      SDMA_CTRL4_PLB_ABus => net_gnd32,
      SDMA_CTRL4_PLB_PAValid => net_gnd0,
      SDMA_CTRL4_PLB_SAValid => net_gnd0,
      SDMA_CTRL4_PLB_masterID => net_gnd1(0 to 0),
      SDMA_CTRL4_PLB_RNW => net_gnd0,
      SDMA_CTRL4_PLB_BE => net_gnd8,
      SDMA_CTRL4_PLB_UABus => net_gnd32,
      SDMA_CTRL4_PLB_rdPrim => net_gnd0,
      SDMA_CTRL4_PLB_wrPrim => net_gnd0,
      SDMA_CTRL4_PLB_abort => net_gnd0,
      SDMA_CTRL4_PLB_busLock => net_gnd0,
      SDMA_CTRL4_PLB_MSize => net_gnd2,
      SDMA_CTRL4_PLB_size => net_gnd4,
      SDMA_CTRL4_PLB_type => net_gnd3,
      SDMA_CTRL4_PLB_lockErr => net_gnd0,
      SDMA_CTRL4_PLB_wrPendReq => net_gnd0,
      SDMA_CTRL4_PLB_wrPendPri => net_gnd2,
      SDMA_CTRL4_PLB_rdPendReq => net_gnd0,
      SDMA_CTRL4_PLB_rdPendPri => net_gnd2,
      SDMA_CTRL4_PLB_reqPri => net_gnd2,
      SDMA_CTRL4_PLB_TAttribute => net_gnd16,
      SDMA_CTRL4_PLB_rdBurst => net_gnd0,
      SDMA_CTRL4_PLB_wrBurst => net_gnd0,
      SDMA_CTRL4_PLB_wrDBus => net_gnd64,
      SDMA_CTRL4_Sl_addrAck => open,
      SDMA_CTRL4_Sl_SSize => open,
      SDMA_CTRL4_Sl_wait => open,
      SDMA_CTRL4_Sl_rearbitrate => open,
      SDMA_CTRL4_Sl_wrDAck => open,
      SDMA_CTRL4_Sl_wrComp => open,
      SDMA_CTRL4_Sl_wrBTerm => open,
      SDMA_CTRL4_Sl_rdDBus => open,
      SDMA_CTRL4_Sl_rdWdAddr => open,
      SDMA_CTRL4_Sl_rdDAck => open,
      SDMA_CTRL4_Sl_rdComp => open,
      SDMA_CTRL4_Sl_rdBTerm => open,
      SDMA_CTRL4_Sl_MBusy => open,
      SDMA_CTRL4_Sl_MRdErr => open,
      SDMA_CTRL4_Sl_MWrErr => open,
      SDMA_CTRL4_Sl_MIRQ => open,
      PIM4_Addr => net_gnd32(0 to 31),
      PIM4_AddrReq => net_gnd0,
      PIM4_AddrAck => open,
      PIM4_RNW => net_gnd0,
      PIM4_Size => net_gnd4(0 to 3),
      PIM4_RdModWr => net_gnd0,
      PIM4_WrFIFO_Data => net_gnd64(0 to 63),
      PIM4_WrFIFO_BE => net_gnd8(0 to 7),
      PIM4_WrFIFO_Push => net_gnd0,
      PIM4_RdFIFO_Data => open,
      PIM4_RdFIFO_Pop => net_gnd0,
      PIM4_RdFIFO_RdWdAddr => open,
      PIM4_WrFIFO_Empty => open,
      PIM4_WrFIFO_AlmostFull => open,
      PIM4_WrFIFO_Flush => net_gnd0,
      PIM4_RdFIFO_Empty => open,
      PIM4_RdFIFO_Flush => net_gnd0,
      PIM4_RdFIFO_Latency => open,
      PIM4_InitDone => open,
      PPC440MC4_MIMCReadNotWrite => net_gnd0,
      PPC440MC4_MIMCAddress => net_gnd36,
      PPC440MC4_MIMCAddressValid => net_gnd0,
      PPC440MC4_MIMCWriteData => net_gnd128,
      PPC440MC4_MIMCWriteDataValid => net_gnd0,
      PPC440MC4_MIMCByteEnable => net_gnd16,
      PPC440MC4_MIMCBankConflict => net_gnd0,
      PPC440MC4_MIMCRowConflict => net_gnd0,
      PPC440MC4_MCMIReadData => open,
      PPC440MC4_MCMIReadDataValid => open,
      PPC440MC4_MCMIReadDataErr => open,
      PPC440MC4_MCMIAddrReadyToAccept => open,
      VFBC4_Cmd_Clk => net_gnd0,
      VFBC4_Cmd_Reset => net_gnd0,
      VFBC4_Cmd_Data => net_gnd32(0 to 31),
      VFBC4_Cmd_Write => net_gnd0,
      VFBC4_Cmd_End => net_gnd0,
      VFBC4_Cmd_Full => open,
      VFBC4_Cmd_Almost_Full => open,
      VFBC4_Cmd_Idle => open,
      VFBC4_Wd_Clk => net_gnd0,
      VFBC4_Wd_Reset => net_gnd0,
      VFBC4_Wd_Write => net_gnd0,
      VFBC4_Wd_End_Burst => net_gnd0,
      VFBC4_Wd_Flush => net_gnd0,
      VFBC4_Wd_Data => net_gnd32(0 to 31),
      VFBC4_Wd_Data_BE => net_gnd4(0 to 3),
      VFBC4_Wd_Full => open,
      VFBC4_Wd_Almost_Full => open,
      VFBC4_Rd_Clk => net_gnd0,
      VFBC4_Rd_Reset => net_gnd0,
      VFBC4_Rd_Read => net_gnd0,
      VFBC4_Rd_End_Burst => net_gnd0,
      VFBC4_Rd_Flush => net_gnd0,
      VFBC4_Rd_Data => open,
      VFBC4_Rd_Empty => open,
      VFBC4_Rd_Almost_Empty => open,
      MCB4_cmd_clk => net_gnd0,
      MCB4_cmd_en => net_gnd0,
      MCB4_cmd_instr => net_gnd3(0 to 2),
      MCB4_cmd_bl => net_gnd6,
      MCB4_cmd_byte_addr => net_gnd30,
      MCB4_cmd_empty => open,
      MCB4_cmd_full => open,
      MCB4_wr_clk => net_gnd0,
      MCB4_wr_en => net_gnd0,
      MCB4_wr_mask => net_gnd8(0 to 7),
      MCB4_wr_data => net_gnd64(0 to 63),
      MCB4_wr_full => open,
      MCB4_wr_empty => open,
      MCB4_wr_count => open,
      MCB4_wr_underrun => open,
      MCB4_wr_error => open,
      MCB4_rd_clk => net_gnd0,
      MCB4_rd_en => net_gnd0,
      MCB4_rd_data => open,
      MCB4_rd_full => open,
      MCB4_rd_empty => open,
      MCB4_rd_count => open,
      MCB4_rd_overflow => open,
      MCB4_rd_error => open,
      FSL5_M_Clk => net_vcc0,
      FSL5_M_Write => net_gnd0,
      FSL5_M_Data => net_gnd32,
      FSL5_M_Control => net_gnd0,
      FSL5_M_Full => open,
      FSL5_S_Clk => net_gnd0,
      FSL5_S_Read => net_gnd0,
      FSL5_S_Data => open,
      FSL5_S_Control => open,
      FSL5_S_Exists => open,
      FSL5_B_M_Clk => net_vcc0,
      FSL5_B_M_Write => net_gnd0,
      FSL5_B_M_Data => net_gnd32,
      FSL5_B_M_Control => net_gnd0,
      FSL5_B_M_Full => open,
      FSL5_B_S_Clk => net_gnd0,
      FSL5_B_S_Read => net_gnd0,
      FSL5_B_S_Data => open,
      FSL5_B_S_Control => open,
      FSL5_B_S_Exists => open,
      SPLB5_Clk => net_vcc0,
      SPLB5_Rst => net_gnd0,
      SPLB5_PLB_ABus => net_gnd32,
      SPLB5_PLB_PAValid => net_gnd0,
      SPLB5_PLB_SAValid => net_gnd0,
      SPLB5_PLB_masterID => net_gnd1(0 to 0),
      SPLB5_PLB_RNW => net_gnd0,
      SPLB5_PLB_BE => net_gnd8,
      SPLB5_PLB_UABus => net_gnd32,
      SPLB5_PLB_rdPrim => net_gnd0,
      SPLB5_PLB_wrPrim => net_gnd0,
      SPLB5_PLB_abort => net_gnd0,
      SPLB5_PLB_busLock => net_gnd0,
      SPLB5_PLB_MSize => net_gnd2,
      SPLB5_PLB_size => net_gnd4,
      SPLB5_PLB_type => net_gnd3,
      SPLB5_PLB_lockErr => net_gnd0,
      SPLB5_PLB_wrPendReq => net_gnd0,
      SPLB5_PLB_wrPendPri => net_gnd2,
      SPLB5_PLB_rdPendReq => net_gnd0,
      SPLB5_PLB_rdPendPri => net_gnd2,
      SPLB5_PLB_reqPri => net_gnd2,
      SPLB5_PLB_TAttribute => net_gnd16,
      SPLB5_PLB_rdBurst => net_gnd0,
      SPLB5_PLB_wrBurst => net_gnd0,
      SPLB5_PLB_wrDBus => net_gnd64,
      SPLB5_Sl_addrAck => open,
      SPLB5_Sl_SSize => open,
      SPLB5_Sl_wait => open,
      SPLB5_Sl_rearbitrate => open,
      SPLB5_Sl_wrDAck => open,
      SPLB5_Sl_wrComp => open,
      SPLB5_Sl_wrBTerm => open,
      SPLB5_Sl_rdDBus => open,
      SPLB5_Sl_rdWdAddr => open,
      SPLB5_Sl_rdDAck => open,
      SPLB5_Sl_rdComp => open,
      SPLB5_Sl_rdBTerm => open,
      SPLB5_Sl_MBusy => open,
      SPLB5_Sl_MRdErr => open,
      SPLB5_Sl_MWrErr => open,
      SPLB5_Sl_MIRQ => open,
      SDMA5_Clk => net_gnd0,
      SDMA5_Rx_IntOut => open,
      SDMA5_Tx_IntOut => open,
      SDMA5_RstOut => open,
      SDMA5_TX_D => open,
      SDMA5_TX_Rem => open,
      SDMA5_TX_SOF => open,
      SDMA5_TX_EOF => open,
      SDMA5_TX_SOP => open,
      SDMA5_TX_EOP => open,
      SDMA5_TX_Src_Rdy => open,
      SDMA5_TX_Dst_Rdy => net_vcc0,
      SDMA5_RX_D => net_gnd32,
      SDMA5_RX_Rem => net_vcc4,
      SDMA5_RX_SOF => net_vcc0,
      SDMA5_RX_EOF => net_vcc0,
      SDMA5_RX_SOP => net_vcc0,
      SDMA5_RX_EOP => net_vcc0,
      SDMA5_RX_Src_Rdy => net_vcc0,
      SDMA5_RX_Dst_Rdy => open,
      SDMA_CTRL5_Clk => net_vcc0,
      SDMA_CTRL5_Rst => net_gnd0,
      SDMA_CTRL5_PLB_ABus => net_gnd32,
      SDMA_CTRL5_PLB_PAValid => net_gnd0,
      SDMA_CTRL5_PLB_SAValid => net_gnd0,
      SDMA_CTRL5_PLB_masterID => net_gnd1(0 to 0),
      SDMA_CTRL5_PLB_RNW => net_gnd0,
      SDMA_CTRL5_PLB_BE => net_gnd8,
      SDMA_CTRL5_PLB_UABus => net_gnd32,
      SDMA_CTRL5_PLB_rdPrim => net_gnd0,
      SDMA_CTRL5_PLB_wrPrim => net_gnd0,
      SDMA_CTRL5_PLB_abort => net_gnd0,
      SDMA_CTRL5_PLB_busLock => net_gnd0,
      SDMA_CTRL5_PLB_MSize => net_gnd2,
      SDMA_CTRL5_PLB_size => net_gnd4,
      SDMA_CTRL5_PLB_type => net_gnd3,
      SDMA_CTRL5_PLB_lockErr => net_gnd0,
      SDMA_CTRL5_PLB_wrPendReq => net_gnd0,
      SDMA_CTRL5_PLB_wrPendPri => net_gnd2,
      SDMA_CTRL5_PLB_rdPendReq => net_gnd0,
      SDMA_CTRL5_PLB_rdPendPri => net_gnd2,
      SDMA_CTRL5_PLB_reqPri => net_gnd2,
      SDMA_CTRL5_PLB_TAttribute => net_gnd16,
      SDMA_CTRL5_PLB_rdBurst => net_gnd0,
      SDMA_CTRL5_PLB_wrBurst => net_gnd0,
      SDMA_CTRL5_PLB_wrDBus => net_gnd64,
      SDMA_CTRL5_Sl_addrAck => open,
      SDMA_CTRL5_Sl_SSize => open,
      SDMA_CTRL5_Sl_wait => open,
      SDMA_CTRL5_Sl_rearbitrate => open,
      SDMA_CTRL5_Sl_wrDAck => open,
      SDMA_CTRL5_Sl_wrComp => open,
      SDMA_CTRL5_Sl_wrBTerm => open,
      SDMA_CTRL5_Sl_rdDBus => open,
      SDMA_CTRL5_Sl_rdWdAddr => open,
      SDMA_CTRL5_Sl_rdDAck => open,
      SDMA_CTRL5_Sl_rdComp => open,
      SDMA_CTRL5_Sl_rdBTerm => open,
      SDMA_CTRL5_Sl_MBusy => open,
      SDMA_CTRL5_Sl_MRdErr => open,
      SDMA_CTRL5_Sl_MWrErr => open,
      SDMA_CTRL5_Sl_MIRQ => open,
      PIM5_Addr => net_gnd32(0 to 31),
      PIM5_AddrReq => net_gnd0,
      PIM5_AddrAck => open,
      PIM5_RNW => net_gnd0,
      PIM5_Size => net_gnd4(0 to 3),
      PIM5_RdModWr => net_gnd0,
      PIM5_WrFIFO_Data => net_gnd64(0 to 63),
      PIM5_WrFIFO_BE => net_gnd8(0 to 7),
      PIM5_WrFIFO_Push => net_gnd0,
      PIM5_RdFIFO_Data => open,
      PIM5_RdFIFO_Pop => net_gnd0,
      PIM5_RdFIFO_RdWdAddr => open,
      PIM5_WrFIFO_Empty => open,
      PIM5_WrFIFO_AlmostFull => open,
      PIM5_WrFIFO_Flush => net_gnd0,
      PIM5_RdFIFO_Empty => open,
      PIM5_RdFIFO_Flush => net_gnd0,
      PIM5_RdFIFO_Latency => open,
      PIM5_InitDone => open,
      PPC440MC5_MIMCReadNotWrite => net_gnd0,
      PPC440MC5_MIMCAddress => net_gnd36,
      PPC440MC5_MIMCAddressValid => net_gnd0,
      PPC440MC5_MIMCWriteData => net_gnd128,
      PPC440MC5_MIMCWriteDataValid => net_gnd0,
      PPC440MC5_MIMCByteEnable => net_gnd16,
      PPC440MC5_MIMCBankConflict => net_gnd0,
      PPC440MC5_MIMCRowConflict => net_gnd0,
      PPC440MC5_MCMIReadData => open,
      PPC440MC5_MCMIReadDataValid => open,
      PPC440MC5_MCMIReadDataErr => open,
      PPC440MC5_MCMIAddrReadyToAccept => open,
      VFBC5_Cmd_Clk => net_gnd0,
      VFBC5_Cmd_Reset => net_gnd0,
      VFBC5_Cmd_Data => net_gnd32(0 to 31),
      VFBC5_Cmd_Write => net_gnd0,
      VFBC5_Cmd_End => net_gnd0,
      VFBC5_Cmd_Full => open,
      VFBC5_Cmd_Almost_Full => open,
      VFBC5_Cmd_Idle => open,
      VFBC5_Wd_Clk => net_gnd0,
      VFBC5_Wd_Reset => net_gnd0,
      VFBC5_Wd_Write => net_gnd0,
      VFBC5_Wd_End_Burst => net_gnd0,
      VFBC5_Wd_Flush => net_gnd0,
      VFBC5_Wd_Data => net_gnd32(0 to 31),
      VFBC5_Wd_Data_BE => net_gnd4(0 to 3),
      VFBC5_Wd_Full => open,
      VFBC5_Wd_Almost_Full => open,
      VFBC5_Rd_Clk => net_gnd0,
      VFBC5_Rd_Reset => net_gnd0,
      VFBC5_Rd_Read => net_gnd0,
      VFBC5_Rd_End_Burst => net_gnd0,
      VFBC5_Rd_Flush => net_gnd0,
      VFBC5_Rd_Data => open,
      VFBC5_Rd_Empty => open,
      VFBC5_Rd_Almost_Empty => open,
      MCB5_cmd_clk => net_gnd0,
      MCB5_cmd_en => net_gnd0,
      MCB5_cmd_instr => net_gnd3(0 to 2),
      MCB5_cmd_bl => net_gnd6,
      MCB5_cmd_byte_addr => net_gnd30,
      MCB5_cmd_empty => open,
      MCB5_cmd_full => open,
      MCB5_wr_clk => net_gnd0,
      MCB5_wr_en => net_gnd0,
      MCB5_wr_mask => net_gnd8(0 to 7),
      MCB5_wr_data => net_gnd64(0 to 63),
      MCB5_wr_full => open,
      MCB5_wr_empty => open,
      MCB5_wr_count => open,
      MCB5_wr_underrun => open,
      MCB5_wr_error => open,
      MCB5_rd_clk => net_gnd0,
      MCB5_rd_en => net_gnd0,
      MCB5_rd_data => open,
      MCB5_rd_full => open,
      MCB5_rd_empty => open,
      MCB5_rd_count => open,
      MCB5_rd_overflow => open,
      MCB5_rd_error => open,
      FSL6_M_Clk => net_vcc0,
      FSL6_M_Write => net_gnd0,
      FSL6_M_Data => net_gnd32,
      FSL6_M_Control => net_gnd0,
      FSL6_M_Full => open,
      FSL6_S_Clk => net_gnd0,
      FSL6_S_Read => net_gnd0,
      FSL6_S_Data => open,
      FSL6_S_Control => open,
      FSL6_S_Exists => open,
      FSL6_B_M_Clk => net_vcc0,
      FSL6_B_M_Write => net_gnd0,
      FSL6_B_M_Data => net_gnd32,
      FSL6_B_M_Control => net_gnd0,
      FSL6_B_M_Full => open,
      FSL6_B_S_Clk => net_gnd0,
      FSL6_B_S_Read => net_gnd0,
      FSL6_B_S_Data => open,
      FSL6_B_S_Control => open,
      FSL6_B_S_Exists => open,
      SPLB6_Clk => net_vcc0,
      SPLB6_Rst => net_gnd0,
      SPLB6_PLB_ABus => net_gnd32,
      SPLB6_PLB_PAValid => net_gnd0,
      SPLB6_PLB_SAValid => net_gnd0,
      SPLB6_PLB_masterID => net_gnd1(0 to 0),
      SPLB6_PLB_RNW => net_gnd0,
      SPLB6_PLB_BE => net_gnd8,
      SPLB6_PLB_UABus => net_gnd32,
      SPLB6_PLB_rdPrim => net_gnd0,
      SPLB6_PLB_wrPrim => net_gnd0,
      SPLB6_PLB_abort => net_gnd0,
      SPLB6_PLB_busLock => net_gnd0,
      SPLB6_PLB_MSize => net_gnd2,
      SPLB6_PLB_size => net_gnd4,
      SPLB6_PLB_type => net_gnd3,
      SPLB6_PLB_lockErr => net_gnd0,
      SPLB6_PLB_wrPendReq => net_gnd0,
      SPLB6_PLB_wrPendPri => net_gnd2,
      SPLB6_PLB_rdPendReq => net_gnd0,
      SPLB6_PLB_rdPendPri => net_gnd2,
      SPLB6_PLB_reqPri => net_gnd2,
      SPLB6_PLB_TAttribute => net_gnd16,
      SPLB6_PLB_rdBurst => net_gnd0,
      SPLB6_PLB_wrBurst => net_gnd0,
      SPLB6_PLB_wrDBus => net_gnd64,
      SPLB6_Sl_addrAck => open,
      SPLB6_Sl_SSize => open,
      SPLB6_Sl_wait => open,
      SPLB6_Sl_rearbitrate => open,
      SPLB6_Sl_wrDAck => open,
      SPLB6_Sl_wrComp => open,
      SPLB6_Sl_wrBTerm => open,
      SPLB6_Sl_rdDBus => open,
      SPLB6_Sl_rdWdAddr => open,
      SPLB6_Sl_rdDAck => open,
      SPLB6_Sl_rdComp => open,
      SPLB6_Sl_rdBTerm => open,
      SPLB6_Sl_MBusy => open,
      SPLB6_Sl_MRdErr => open,
      SPLB6_Sl_MWrErr => open,
      SPLB6_Sl_MIRQ => open,
      SDMA6_Clk => net_gnd0,
      SDMA6_Rx_IntOut => open,
      SDMA6_Tx_IntOut => open,
      SDMA6_RstOut => open,
      SDMA6_TX_D => open,
      SDMA6_TX_Rem => open,
      SDMA6_TX_SOF => open,
      SDMA6_TX_EOF => open,
      SDMA6_TX_SOP => open,
      SDMA6_TX_EOP => open,
      SDMA6_TX_Src_Rdy => open,
      SDMA6_TX_Dst_Rdy => net_vcc0,
      SDMA6_RX_D => net_gnd32,
      SDMA6_RX_Rem => net_vcc4,
      SDMA6_RX_SOF => net_vcc0,
      SDMA6_RX_EOF => net_vcc0,
      SDMA6_RX_SOP => net_vcc0,
      SDMA6_RX_EOP => net_vcc0,
      SDMA6_RX_Src_Rdy => net_vcc0,
      SDMA6_RX_Dst_Rdy => open,
      SDMA_CTRL6_Clk => net_vcc0,
      SDMA_CTRL6_Rst => net_gnd0,
      SDMA_CTRL6_PLB_ABus => net_gnd32,
      SDMA_CTRL6_PLB_PAValid => net_gnd0,
      SDMA_CTRL6_PLB_SAValid => net_gnd0,
      SDMA_CTRL6_PLB_masterID => net_gnd1(0 to 0),
      SDMA_CTRL6_PLB_RNW => net_gnd0,
      SDMA_CTRL6_PLB_BE => net_gnd8,
      SDMA_CTRL6_PLB_UABus => net_gnd32,
      SDMA_CTRL6_PLB_rdPrim => net_gnd0,
      SDMA_CTRL6_PLB_wrPrim => net_gnd0,
      SDMA_CTRL6_PLB_abort => net_gnd0,
      SDMA_CTRL6_PLB_busLock => net_gnd0,
      SDMA_CTRL6_PLB_MSize => net_gnd2,
      SDMA_CTRL6_PLB_size => net_gnd4,
      SDMA_CTRL6_PLB_type => net_gnd3,
      SDMA_CTRL6_PLB_lockErr => net_gnd0,
      SDMA_CTRL6_PLB_wrPendReq => net_gnd0,
      SDMA_CTRL6_PLB_wrPendPri => net_gnd2,
      SDMA_CTRL6_PLB_rdPendReq => net_gnd0,
      SDMA_CTRL6_PLB_rdPendPri => net_gnd2,
      SDMA_CTRL6_PLB_reqPri => net_gnd2,
      SDMA_CTRL6_PLB_TAttribute => net_gnd16,
      SDMA_CTRL6_PLB_rdBurst => net_gnd0,
      SDMA_CTRL6_PLB_wrBurst => net_gnd0,
      SDMA_CTRL6_PLB_wrDBus => net_gnd64,
      SDMA_CTRL6_Sl_addrAck => open,
      SDMA_CTRL6_Sl_SSize => open,
      SDMA_CTRL6_Sl_wait => open,
      SDMA_CTRL6_Sl_rearbitrate => open,
      SDMA_CTRL6_Sl_wrDAck => open,
      SDMA_CTRL6_Sl_wrComp => open,
      SDMA_CTRL6_Sl_wrBTerm => open,
      SDMA_CTRL6_Sl_rdDBus => open,
      SDMA_CTRL6_Sl_rdWdAddr => open,
      SDMA_CTRL6_Sl_rdDAck => open,
      SDMA_CTRL6_Sl_rdComp => open,
      SDMA_CTRL6_Sl_rdBTerm => open,
      SDMA_CTRL6_Sl_MBusy => open,
      SDMA_CTRL6_Sl_MRdErr => open,
      SDMA_CTRL6_Sl_MWrErr => open,
      SDMA_CTRL6_Sl_MIRQ => open,
      PIM6_Addr => net_gnd32(0 to 31),
      PIM6_AddrReq => net_gnd0,
      PIM6_AddrAck => open,
      PIM6_RNW => net_gnd0,
      PIM6_Size => net_gnd4(0 to 3),
      PIM6_RdModWr => net_gnd0,
      PIM6_WrFIFO_Data => net_gnd64(0 to 63),
      PIM6_WrFIFO_BE => net_gnd8(0 to 7),
      PIM6_WrFIFO_Push => net_gnd0,
      PIM6_RdFIFO_Data => open,
      PIM6_RdFIFO_Pop => net_gnd0,
      PIM6_RdFIFO_RdWdAddr => open,
      PIM6_WrFIFO_Empty => open,
      PIM6_WrFIFO_AlmostFull => open,
      PIM6_WrFIFO_Flush => net_gnd0,
      PIM6_RdFIFO_Empty => open,
      PIM6_RdFIFO_Flush => net_gnd0,
      PIM6_RdFIFO_Latency => open,
      PIM6_InitDone => open,
      PPC440MC6_MIMCReadNotWrite => net_gnd0,
      PPC440MC6_MIMCAddress => net_gnd36,
      PPC440MC6_MIMCAddressValid => net_gnd0,
      PPC440MC6_MIMCWriteData => net_gnd128,
      PPC440MC6_MIMCWriteDataValid => net_gnd0,
      PPC440MC6_MIMCByteEnable => net_gnd16,
      PPC440MC6_MIMCBankConflict => net_gnd0,
      PPC440MC6_MIMCRowConflict => net_gnd0,
      PPC440MC6_MCMIReadData => open,
      PPC440MC6_MCMIReadDataValid => open,
      PPC440MC6_MCMIReadDataErr => open,
      PPC440MC6_MCMIAddrReadyToAccept => open,
      VFBC6_Cmd_Clk => net_gnd0,
      VFBC6_Cmd_Reset => net_gnd0,
      VFBC6_Cmd_Data => net_gnd32(0 to 31),
      VFBC6_Cmd_Write => net_gnd0,
      VFBC6_Cmd_End => net_gnd0,
      VFBC6_Cmd_Full => open,
      VFBC6_Cmd_Almost_Full => open,
      VFBC6_Cmd_Idle => open,
      VFBC6_Wd_Clk => net_gnd0,
      VFBC6_Wd_Reset => net_gnd0,
      VFBC6_Wd_Write => net_gnd0,
      VFBC6_Wd_End_Burst => net_gnd0,
      VFBC6_Wd_Flush => net_gnd0,
      VFBC6_Wd_Data => net_gnd32(0 to 31),
      VFBC6_Wd_Data_BE => net_gnd4(0 to 3),
      VFBC6_Wd_Full => open,
      VFBC6_Wd_Almost_Full => open,
      VFBC6_Rd_Clk => net_gnd0,
      VFBC6_Rd_Reset => net_gnd0,
      VFBC6_Rd_Read => net_gnd0,
      VFBC6_Rd_End_Burst => net_gnd0,
      VFBC6_Rd_Flush => net_gnd0,
      VFBC6_Rd_Data => open,
      VFBC6_Rd_Empty => open,
      VFBC6_Rd_Almost_Empty => open,
      MCB6_cmd_clk => net_gnd0,
      MCB6_cmd_en => net_gnd0,
      MCB6_cmd_instr => net_gnd3(0 to 2),
      MCB6_cmd_bl => net_gnd6,
      MCB6_cmd_byte_addr => net_gnd30,
      MCB6_cmd_empty => open,
      MCB6_cmd_full => open,
      MCB6_wr_clk => net_gnd0,
      MCB6_wr_en => net_gnd0,
      MCB6_wr_mask => net_gnd8(0 to 7),
      MCB6_wr_data => net_gnd64(0 to 63),
      MCB6_wr_full => open,
      MCB6_wr_empty => open,
      MCB6_wr_count => open,
      MCB6_wr_underrun => open,
      MCB6_wr_error => open,
      MCB6_rd_clk => net_gnd0,
      MCB6_rd_en => net_gnd0,
      MCB6_rd_data => open,
      MCB6_rd_full => open,
      MCB6_rd_empty => open,
      MCB6_rd_count => open,
      MCB6_rd_overflow => open,
      MCB6_rd_error => open,
      FSL7_M_Clk => net_vcc0,
      FSL7_M_Write => net_gnd0,
      FSL7_M_Data => net_gnd32,
      FSL7_M_Control => net_gnd0,
      FSL7_M_Full => open,
      FSL7_S_Clk => net_gnd0,
      FSL7_S_Read => net_gnd0,
      FSL7_S_Data => open,
      FSL7_S_Control => open,
      FSL7_S_Exists => open,
      FSL7_B_M_Clk => net_vcc0,
      FSL7_B_M_Write => net_gnd0,
      FSL7_B_M_Data => net_gnd32,
      FSL7_B_M_Control => net_gnd0,
      FSL7_B_M_Full => open,
      FSL7_B_S_Clk => net_gnd0,
      FSL7_B_S_Read => net_gnd0,
      FSL7_B_S_Data => open,
      FSL7_B_S_Control => open,
      FSL7_B_S_Exists => open,
      SPLB7_Clk => net_vcc0,
      SPLB7_Rst => net_gnd0,
      SPLB7_PLB_ABus => net_gnd32,
      SPLB7_PLB_PAValid => net_gnd0,
      SPLB7_PLB_SAValid => net_gnd0,
      SPLB7_PLB_masterID => net_gnd1(0 to 0),
      SPLB7_PLB_RNW => net_gnd0,
      SPLB7_PLB_BE => net_gnd8,
      SPLB7_PLB_UABus => net_gnd32,
      SPLB7_PLB_rdPrim => net_gnd0,
      SPLB7_PLB_wrPrim => net_gnd0,
      SPLB7_PLB_abort => net_gnd0,
      SPLB7_PLB_busLock => net_gnd0,
      SPLB7_PLB_MSize => net_gnd2,
      SPLB7_PLB_size => net_gnd4,
      SPLB7_PLB_type => net_gnd3,
      SPLB7_PLB_lockErr => net_gnd0,
      SPLB7_PLB_wrPendReq => net_gnd0,
      SPLB7_PLB_wrPendPri => net_gnd2,
      SPLB7_PLB_rdPendReq => net_gnd0,
      SPLB7_PLB_rdPendPri => net_gnd2,
      SPLB7_PLB_reqPri => net_gnd2,
      SPLB7_PLB_TAttribute => net_gnd16,
      SPLB7_PLB_rdBurst => net_gnd0,
      SPLB7_PLB_wrBurst => net_gnd0,
      SPLB7_PLB_wrDBus => net_gnd64,
      SPLB7_Sl_addrAck => open,
      SPLB7_Sl_SSize => open,
      SPLB7_Sl_wait => open,
      SPLB7_Sl_rearbitrate => open,
      SPLB7_Sl_wrDAck => open,
      SPLB7_Sl_wrComp => open,
      SPLB7_Sl_wrBTerm => open,
      SPLB7_Sl_rdDBus => open,
      SPLB7_Sl_rdWdAddr => open,
      SPLB7_Sl_rdDAck => open,
      SPLB7_Sl_rdComp => open,
      SPLB7_Sl_rdBTerm => open,
      SPLB7_Sl_MBusy => open,
      SPLB7_Sl_MRdErr => open,
      SPLB7_Sl_MWrErr => open,
      SPLB7_Sl_MIRQ => open,
      SDMA7_Clk => net_gnd0,
      SDMA7_Rx_IntOut => open,
      SDMA7_Tx_IntOut => open,
      SDMA7_RstOut => open,
      SDMA7_TX_D => open,
      SDMA7_TX_Rem => open,
      SDMA7_TX_SOF => open,
      SDMA7_TX_EOF => open,
      SDMA7_TX_SOP => open,
      SDMA7_TX_EOP => open,
      SDMA7_TX_Src_Rdy => open,
      SDMA7_TX_Dst_Rdy => net_vcc0,
      SDMA7_RX_D => net_gnd32,
      SDMA7_RX_Rem => net_vcc4,
      SDMA7_RX_SOF => net_vcc0,
      SDMA7_RX_EOF => net_vcc0,
      SDMA7_RX_SOP => net_vcc0,
      SDMA7_RX_EOP => net_vcc0,
      SDMA7_RX_Src_Rdy => net_vcc0,
      SDMA7_RX_Dst_Rdy => open,
      SDMA_CTRL7_Clk => net_vcc0,
      SDMA_CTRL7_Rst => net_gnd0,
      SDMA_CTRL7_PLB_ABus => net_gnd32,
      SDMA_CTRL7_PLB_PAValid => net_gnd0,
      SDMA_CTRL7_PLB_SAValid => net_gnd0,
      SDMA_CTRL7_PLB_masterID => net_gnd1(0 to 0),
      SDMA_CTRL7_PLB_RNW => net_gnd0,
      SDMA_CTRL7_PLB_BE => net_gnd8,
      SDMA_CTRL7_PLB_UABus => net_gnd32,
      SDMA_CTRL7_PLB_rdPrim => net_gnd0,
      SDMA_CTRL7_PLB_wrPrim => net_gnd0,
      SDMA_CTRL7_PLB_abort => net_gnd0,
      SDMA_CTRL7_PLB_busLock => net_gnd0,
      SDMA_CTRL7_PLB_MSize => net_gnd2,
      SDMA_CTRL7_PLB_size => net_gnd4,
      SDMA_CTRL7_PLB_type => net_gnd3,
      SDMA_CTRL7_PLB_lockErr => net_gnd0,
      SDMA_CTRL7_PLB_wrPendReq => net_gnd0,
      SDMA_CTRL7_PLB_wrPendPri => net_gnd2,
      SDMA_CTRL7_PLB_rdPendReq => net_gnd0,
      SDMA_CTRL7_PLB_rdPendPri => net_gnd2,
      SDMA_CTRL7_PLB_reqPri => net_gnd2,
      SDMA_CTRL7_PLB_TAttribute => net_gnd16,
      SDMA_CTRL7_PLB_rdBurst => net_gnd0,
      SDMA_CTRL7_PLB_wrBurst => net_gnd0,
      SDMA_CTRL7_PLB_wrDBus => net_gnd64,
      SDMA_CTRL7_Sl_addrAck => open,
      SDMA_CTRL7_Sl_SSize => open,
      SDMA_CTRL7_Sl_wait => open,
      SDMA_CTRL7_Sl_rearbitrate => open,
      SDMA_CTRL7_Sl_wrDAck => open,
      SDMA_CTRL7_Sl_wrComp => open,
      SDMA_CTRL7_Sl_wrBTerm => open,
      SDMA_CTRL7_Sl_rdDBus => open,
      SDMA_CTRL7_Sl_rdWdAddr => open,
      SDMA_CTRL7_Sl_rdDAck => open,
      SDMA_CTRL7_Sl_rdComp => open,
      SDMA_CTRL7_Sl_rdBTerm => open,
      SDMA_CTRL7_Sl_MBusy => open,
      SDMA_CTRL7_Sl_MRdErr => open,
      SDMA_CTRL7_Sl_MWrErr => open,
      SDMA_CTRL7_Sl_MIRQ => open,
      PIM7_Addr => net_gnd32(0 to 31),
      PIM7_AddrReq => net_gnd0,
      PIM7_AddrAck => open,
      PIM7_RNW => net_gnd0,
      PIM7_Size => net_gnd4(0 to 3),
      PIM7_RdModWr => net_gnd0,
      PIM7_WrFIFO_Data => net_gnd64(0 to 63),
      PIM7_WrFIFO_BE => net_gnd8(0 to 7),
      PIM7_WrFIFO_Push => net_gnd0,
      PIM7_RdFIFO_Data => open,
      PIM7_RdFIFO_Pop => net_gnd0,
      PIM7_RdFIFO_RdWdAddr => open,
      PIM7_WrFIFO_Empty => open,
      PIM7_WrFIFO_AlmostFull => open,
      PIM7_WrFIFO_Flush => net_gnd0,
      PIM7_RdFIFO_Empty => open,
      PIM7_RdFIFO_Flush => net_gnd0,
      PIM7_RdFIFO_Latency => open,
      PIM7_InitDone => open,
      PPC440MC7_MIMCReadNotWrite => net_gnd0,
      PPC440MC7_MIMCAddress => net_gnd36,
      PPC440MC7_MIMCAddressValid => net_gnd0,
      PPC440MC7_MIMCWriteData => net_gnd128,
      PPC440MC7_MIMCWriteDataValid => net_gnd0,
      PPC440MC7_MIMCByteEnable => net_gnd16,
      PPC440MC7_MIMCBankConflict => net_gnd0,
      PPC440MC7_MIMCRowConflict => net_gnd0,
      PPC440MC7_MCMIReadData => open,
      PPC440MC7_MCMIReadDataValid => open,
      PPC440MC7_MCMIReadDataErr => open,
      PPC440MC7_MCMIAddrReadyToAccept => open,
      VFBC7_Cmd_Clk => net_gnd0,
      VFBC7_Cmd_Reset => net_gnd0,
      VFBC7_Cmd_Data => net_gnd32(0 to 31),
      VFBC7_Cmd_Write => net_gnd0,
      VFBC7_Cmd_End => net_gnd0,
      VFBC7_Cmd_Full => open,
      VFBC7_Cmd_Almost_Full => open,
      VFBC7_Cmd_Idle => open,
      VFBC7_Wd_Clk => net_gnd0,
      VFBC7_Wd_Reset => net_gnd0,
      VFBC7_Wd_Write => net_gnd0,
      VFBC7_Wd_End_Burst => net_gnd0,
      VFBC7_Wd_Flush => net_gnd0,
      VFBC7_Wd_Data => net_gnd32(0 to 31),
      VFBC7_Wd_Data_BE => net_gnd4(0 to 3),
      VFBC7_Wd_Full => open,
      VFBC7_Wd_Almost_Full => open,
      VFBC7_Rd_Clk => net_gnd0,
      VFBC7_Rd_Reset => net_gnd0,
      VFBC7_Rd_Read => net_gnd0,
      VFBC7_Rd_End_Burst => net_gnd0,
      VFBC7_Rd_Flush => net_gnd0,
      VFBC7_Rd_Data => open,
      VFBC7_Rd_Empty => open,
      VFBC7_Rd_Almost_Empty => open,
      MCB7_cmd_clk => net_gnd0,
      MCB7_cmd_en => net_gnd0,
      MCB7_cmd_instr => net_gnd3(0 to 2),
      MCB7_cmd_bl => net_gnd6,
      MCB7_cmd_byte_addr => net_gnd30,
      MCB7_cmd_empty => open,
      MCB7_cmd_full => open,
      MCB7_wr_clk => net_gnd0,
      MCB7_wr_en => net_gnd0,
      MCB7_wr_mask => net_gnd8(0 to 7),
      MCB7_wr_data => net_gnd64(0 to 63),
      MCB7_wr_full => open,
      MCB7_wr_empty => open,
      MCB7_wr_count => open,
      MCB7_wr_underrun => open,
      MCB7_wr_error => open,
      MCB7_rd_clk => net_gnd0,
      MCB7_rd_en => net_gnd0,
      MCB7_rd_data => open,
      MCB7_rd_full => open,
      MCB7_rd_empty => open,
      MCB7_rd_count => open,
      MCB7_rd_overflow => open,
      MCB7_rd_error => open,
      MPMC_CTRL_Clk => plb_clk_100MHz,
      MPMC_CTRL_Rst => plb_0_SPLB_Rst(8),
      MPMC_CTRL_PLB_ABus => plb_0_PLB_ABus,
      MPMC_CTRL_PLB_PAValid => plb_0_PLB_PAValid,
      MPMC_CTRL_PLB_SAValid => plb_0_PLB_SAValid,
      MPMC_CTRL_PLB_masterID => plb_0_PLB_masterID(0 to 0),
      MPMC_CTRL_PLB_RNW => plb_0_PLB_RNW,
      MPMC_CTRL_PLB_BE => plb_0_PLB_BE,
      MPMC_CTRL_PLB_UABus => plb_0_PLB_UABus,
      MPMC_CTRL_PLB_rdPrim => plb_0_PLB_rdPrim(8),
      MPMC_CTRL_PLB_wrPrim => plb_0_PLB_wrPrim(8),
      MPMC_CTRL_PLB_abort => plb_0_PLB_abort,
      MPMC_CTRL_PLB_busLock => plb_0_PLB_busLock,
      MPMC_CTRL_PLB_MSize => plb_0_PLB_MSize,
      MPMC_CTRL_PLB_size => plb_0_PLB_size,
      MPMC_CTRL_PLB_type => plb_0_PLB_type,
      MPMC_CTRL_PLB_lockErr => plb_0_PLB_lockErr,
      MPMC_CTRL_PLB_wrPendReq => plb_0_PLB_wrPendReq,
      MPMC_CTRL_PLB_wrPendPri => plb_0_PLB_wrPendPri,
      MPMC_CTRL_PLB_rdPendReq => plb_0_PLB_rdPendReq,
      MPMC_CTRL_PLB_rdPendPri => plb_0_PLB_rdPendPri,
      MPMC_CTRL_PLB_reqPri => plb_0_PLB_reqPri,
      MPMC_CTRL_PLB_TAttribute => plb_0_PLB_TAttribute,
      MPMC_CTRL_PLB_rdBurst => plb_0_PLB_rdBurst,
      MPMC_CTRL_PLB_wrBurst => plb_0_PLB_wrBurst,
      MPMC_CTRL_PLB_wrDBus => plb_0_PLB_wrDBus,
      MPMC_CTRL_Sl_addrAck => plb_0_Sl_addrAck(8),
      MPMC_CTRL_Sl_SSize => plb_0_Sl_SSize(16 to 17),
      MPMC_CTRL_Sl_wait => plb_0_Sl_wait(8),
      MPMC_CTRL_Sl_rearbitrate => plb_0_Sl_rearbitrate(8),
      MPMC_CTRL_Sl_wrDAck => plb_0_Sl_wrDAck(8),
      MPMC_CTRL_Sl_wrComp => plb_0_Sl_wrComp(8),
      MPMC_CTRL_Sl_wrBTerm => plb_0_Sl_wrBTerm(8),
      MPMC_CTRL_Sl_rdDBus => plb_0_Sl_rdDBus(512 to 575),
      MPMC_CTRL_Sl_rdWdAddr => plb_0_Sl_rdWdAddr(32 to 35),
      MPMC_CTRL_Sl_rdDAck => plb_0_Sl_rdDAck(8),
      MPMC_CTRL_Sl_rdComp => plb_0_Sl_rdComp(8),
      MPMC_CTRL_Sl_rdBTerm => plb_0_Sl_rdBTerm(8),
      MPMC_CTRL_Sl_MBusy => plb_0_Sl_MBusy(16 to 17),
      MPMC_CTRL_Sl_MRdErr => plb_0_Sl_MRdErr(16 to 17),
      MPMC_CTRL_Sl_MWrErr => plb_0_Sl_MWrErr(16 to 17),
      MPMC_CTRL_Sl_MIRQ => plb_0_Sl_MIRQ(16 to 17),
      MPMC_Clk0 => plb_clk_100MHz,
      MPMC_Clk0_DIV2 => net_vcc0,
      MPMC_Clk90 => net_vcc0,
      MPMC_Clk_200MHz => net_vcc0,
      MPMC_Rst => sys_bus_reset(0),
      MPMC_Clk_Mem => sdram_clk_phy,
      MPMC_Clk_Mem_2x => net_vcc0,
      MPMC_Clk_Mem_2x_180 => net_vcc0,
      MPMC_Clk_Mem_2x_CE0 => net_vcc0,
      MPMC_Clk_Mem_2x_CE90 => net_vcc0,
      MPMC_Clk_Rd_Base => net_vcc0,
      MPMC_Clk_Mem_2x_bufpll_o => open,
      MPMC_Clk_Mem_2x_180_bufpll_o => open,
      MPMC_Clk_Mem_2x_CE0_bufpll_o => open,
      MPMC_Clk_Mem_2x_CE90_bufpll_o => open,
      MPMC_PLL_Lock_bufpll_o => open,
      MPMC_PLL_Lock => net_gnd0,
      MPMC_Idelayctrl_Rdy_I => net_vcc0,
      MPMC_Idelayctrl_Rdy_O => open,
      MPMC_InitDone => open,
      MPMC_ECC_Intr => open,
      MPMC_DCM_PSEN => mpmc_dcm_psen,
      MPMC_DCM_PSINCDEC => mpmc_dcm_psincdec,
      MPMC_DCM_PSDONE => mpmc_dcm_psdone,
      MPMC_MCB_DRP_Clk => net_vcc0,
      SDRAM_Clk => pgassign1(0 to 0),
      SDRAM_CE => pgassign2(0 to 0),
      SDRAM_CS_n => pgassign3(0 to 0),
      SDRAM_RAS_n => sdram_ras_n,
      SDRAM_CAS_n => sdram_cas_n,
      SDRAM_WE_n => sdram_we_n,
      SDRAM_BankAddr => sdram_ba,
      SDRAM_Addr => sdram_addr,
      SDRAM_DQ => sdram_data,
      SDRAM_DM => sdram_dqm,
      DDR_Clk => open,
      DDR_Clk_n => open,
      DDR_CE => open,
      DDR_CS_n => open,
      DDR_RAS_n => open,
      DDR_CAS_n => open,
      DDR_WE_n => open,
      DDR_BankAddr => open,
      DDR_Addr => open,
      DDR_DQ => open,
      DDR_DM => open,
      DDR_DQS => open,
      DDR_DQS_Div_O => open,
      DDR_DQS_Div_I => net_gnd0,
      DDR2_Clk => open,
      DDR2_Clk_n => open,
      DDR2_CE => open,
      DDR2_CS_n => open,
      DDR2_ODT => open,
      DDR2_RAS_n => open,
      DDR2_CAS_n => open,
      DDR2_WE_n => open,
      DDR2_BankAddr => open,
      DDR2_Addr => open,
      DDR2_DQ => open,
      DDR2_DM => open,
      DDR2_DQS => open,
      DDR2_DQS_n => open,
      DDR2_DQS_Div_O => open,
      DDR2_DQS_Div_I => net_gnd0,
      DDR3_Clk => open,
      DDR3_Clk_n => open,
      DDR3_CE => open,
      DDR3_CS_n => open,
      DDR3_ODT => open,
      DDR3_RAS_n => open,
      DDR3_CAS_n => open,
      DDR3_WE_n => open,
      DDR3_BankAddr => open,
      DDR3_Addr => open,
      DDR3_DQ => open,
      DDR3_DM => open,
      DDR3_Reset_n => open,
      DDR3_DQS => open,
      DDR3_DQS_n => open,
      mcbx_dram_addr => open,
      mcbx_dram_ba => open,
      mcbx_dram_ras_n => open,
      mcbx_dram_cas_n => open,
      mcbx_dram_we_n => open,
      mcbx_dram_cke => open,
      mcbx_dram_clk => open,
      mcbx_dram_clk_n => open,
      mcbx_dram_dq => open,
      mcbx_dram_dqs => open,
      mcbx_dram_dqs_n => open,
      mcbx_dram_udqs => open,
      mcbx_dram_udqs_n => open,
      mcbx_dram_udm => open,
      mcbx_dram_ldm => open,
      mcbx_dram_odt => open,
      mcbx_dram_ddr3_rst => open,
      selfrefresh_enter => net_gnd0,
      selfrefresh_mode => open,
      calib_recal => net_gnd0,
      rzq => open,
      zio => open
    );

  dcm_2 : dcm_2_wrapper
    port map (
      RST => DCM_1_lock,
      CLKIN => plb_clk_100MHz,
      CLKFB => sdram_clk_phy,
      PSEN => mpmc_dcm_psen,
      PSINCDEC => mpmc_dcm_psincdec,
      PSCLK => plb_clk_100MHz,
      DSSEN => net_gnd0,
      CLK0 => sdram_clk_phy,
      CLK90 => open,
      CLK180 => open,
      CLK270 => open,
      CLKDV => open,
      CLK2X => open,
      CLK2X180 => open,
      CLKFX => open,
      CLKFX180 => open,
      STATUS => open,
      LOCKED => Dcm_all_locked,
      PSDONE => mpmc_dcm_psdone
    );

  xps_gpio_1 : xps_gpio_1_wrapper
    port map (
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_0_SPLB_Rst(9),
      PLB_ABus => plb_0_PLB_ABus,
      PLB_UABus => plb_0_PLB_UABus,
      PLB_PAValid => plb_0_PLB_PAValid,
      PLB_SAValid => plb_0_PLB_SAValid,
      PLB_rdPrim => plb_0_PLB_rdPrim(9),
      PLB_wrPrim => plb_0_PLB_wrPrim(9),
      PLB_masterID => plb_0_PLB_masterID(0 to 0),
      PLB_abort => plb_0_PLB_abort,
      PLB_busLock => plb_0_PLB_busLock,
      PLB_RNW => plb_0_PLB_RNW,
      PLB_BE => plb_0_PLB_BE,
      PLB_MSize => plb_0_PLB_MSize,
      PLB_size => plb_0_PLB_size,
      PLB_type => plb_0_PLB_type,
      PLB_lockErr => plb_0_PLB_lockErr,
      PLB_wrDBus => plb_0_PLB_wrDBus,
      PLB_wrBurst => plb_0_PLB_wrBurst,
      PLB_rdBurst => plb_0_PLB_rdBurst,
      PLB_wrPendReq => plb_0_PLB_wrPendReq,
      PLB_rdPendReq => plb_0_PLB_rdPendReq,
      PLB_wrPendPri => plb_0_PLB_wrPendPri,
      PLB_rdPendPri => plb_0_PLB_rdPendPri,
      PLB_reqPri => plb_0_PLB_reqPri,
      PLB_TAttribute => plb_0_PLB_TAttribute,
      Sl_addrAck => plb_0_Sl_addrAck(9),
      Sl_SSize => plb_0_Sl_SSize(18 to 19),
      Sl_wait => plb_0_Sl_wait(9),
      Sl_rearbitrate => plb_0_Sl_rearbitrate(9),
      Sl_wrDAck => plb_0_Sl_wrDAck(9),
      Sl_wrComp => plb_0_Sl_wrComp(9),
      Sl_wrBTerm => plb_0_Sl_wrBTerm(9),
      Sl_rdDBus => plb_0_Sl_rdDBus(576 to 639),
      Sl_rdWdAddr => plb_0_Sl_rdWdAddr(36 to 39),
      Sl_rdDAck => plb_0_Sl_rdDAck(9),
      Sl_rdComp => plb_0_Sl_rdComp(9),
      Sl_rdBTerm => plb_0_Sl_rdBTerm(9),
      Sl_MBusy => plb_0_Sl_MBusy(18 to 19),
      Sl_MWrErr => plb_0_Sl_MWrErr(18 to 19),
      Sl_MRdErr => plb_0_Sl_MRdErr(18 to 19),
      Sl_MIRQ => plb_0_Sl_MIRQ(18 to 19),
      IP2INTC_Irpt => gpio_int,
      GPIO_IO_I => gpio_helios_I(4 downto 0),
      GPIO_IO_O => gpio_helios_O(4 downto 0),
      GPIO_IO_T => gpio_helios_T(4 downto 0),
      GPIO2_IO_I => gpio_helios_game_I(15 downto 0),
      GPIO2_IO_O => gpio_helios_game_O(15 downto 0),
      GPIO2_IO_T => gpio_helios_game_T(15 downto 0)
    );

  debug_module_0 : debug_module_0_wrapper
    port map (
      Interrupt => open,
      Debug_SYS_Rst => open,
      Ext_BRK => open,
      Ext_NM_BRK => open,
      S_AXI_ACLK => net_gnd0,
      S_AXI_ARESETN => net_gnd0,
      S_AXI_AWADDR => net_gnd32(0 to 31),
      S_AXI_AWVALID => net_gnd0,
      S_AXI_AWREADY => open,
      S_AXI_WDATA => net_gnd32(0 to 31),
      S_AXI_WSTRB => net_gnd4(0 to 3),
      S_AXI_WVALID => net_gnd0,
      S_AXI_WREADY => open,
      S_AXI_BRESP => open,
      S_AXI_BVALID => open,
      S_AXI_BREADY => net_gnd0,
      S_AXI_ARADDR => net_gnd32(0 to 31),
      S_AXI_ARVALID => net_gnd0,
      S_AXI_ARREADY => open,
      S_AXI_RDATA => open,
      S_AXI_RRESP => open,
      S_AXI_RVALID => open,
      S_AXI_RREADY => net_gnd0,
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_1_SPLB_Rst(5),
      PLB_ABus => plb_1_PLB_ABus,
      PLB_UABus => plb_1_PLB_UABus,
      PLB_PAValid => plb_1_PLB_PAValid,
      PLB_SAValid => plb_1_PLB_SAValid,
      PLB_rdPrim => plb_1_PLB_rdPrim(5),
      PLB_wrPrim => plb_1_PLB_wrPrim(5),
      PLB_masterID => plb_1_PLB_masterID,
      PLB_abort => plb_1_PLB_abort,
      PLB_busLock => plb_1_PLB_busLock,
      PLB_RNW => plb_1_PLB_RNW,
      PLB_BE => plb_1_PLB_BE,
      PLB_MSize => plb_1_PLB_MSize,
      PLB_size => plb_1_PLB_size,
      PLB_type => plb_1_PLB_type,
      PLB_lockErr => plb_1_PLB_lockErr,
      PLB_wrDBus => plb_1_PLB_wrDBus,
      PLB_wrBurst => plb_1_PLB_wrBurst,
      PLB_rdBurst => plb_1_PLB_rdBurst,
      PLB_wrPendReq => plb_1_PLB_wrPendReq,
      PLB_rdPendReq => plb_1_PLB_rdPendReq,
      PLB_wrPendPri => plb_1_PLB_wrPendPri,
      PLB_rdPendPri => plb_1_PLB_rdPendPri,
      PLB_reqPri => plb_1_PLB_reqPri,
      PLB_TAttribute => plb_1_PLB_TAttribute,
      Sl_addrAck => plb_1_Sl_addrAck(5),
      Sl_SSize => plb_1_Sl_SSize(10 to 11),
      Sl_wait => plb_1_Sl_wait(5),
      Sl_rearbitrate => plb_1_Sl_rearbitrate(5),
      Sl_wrDAck => plb_1_Sl_wrDAck(5),
      Sl_wrComp => plb_1_Sl_wrComp(5),
      Sl_wrBTerm => plb_1_Sl_wrBTerm(5),
      Sl_rdDBus => plb_1_Sl_rdDBus(320 to 383),
      Sl_rdWdAddr => plb_1_Sl_rdWdAddr(20 to 23),
      Sl_rdDAck => plb_1_Sl_rdDAck(5),
      Sl_rdComp => plb_1_Sl_rdComp(5),
      Sl_rdBTerm => plb_1_Sl_rdBTerm(5),
      Sl_MBusy => plb_1_Sl_MBusy(20 to 23),
      Sl_MWrErr => plb_1_Sl_MWrErr(20 to 23),
      Sl_MRdErr => plb_1_Sl_MRdErr(20 to 23),
      Sl_MIRQ => plb_1_Sl_MIRQ(20 to 23),
      Dbg_Clk_0 => open,
      Dbg_TDI_0 => open,
      Dbg_TDO_0 => net_gnd0,
      Dbg_Reg_En_0 => open,
      Dbg_Capture_0 => open,
      Dbg_Shift_0 => open,
      Dbg_Update_0 => open,
      Dbg_Rst_0 => open,
      Dbg_Clk_1 => open,
      Dbg_TDI_1 => open,
      Dbg_TDO_1 => net_gnd0,
      Dbg_Reg_En_1 => open,
      Dbg_Capture_1 => open,
      Dbg_Shift_1 => open,
      Dbg_Update_1 => open,
      Dbg_Rst_1 => open,
      Dbg_Clk_2 => open,
      Dbg_TDI_2 => open,
      Dbg_TDO_2 => net_gnd0,
      Dbg_Reg_En_2 => open,
      Dbg_Capture_2 => open,
      Dbg_Shift_2 => open,
      Dbg_Update_2 => open,
      Dbg_Rst_2 => open,
      Dbg_Clk_3 => open,
      Dbg_TDI_3 => open,
      Dbg_TDO_3 => net_gnd0,
      Dbg_Reg_En_3 => open,
      Dbg_Capture_3 => open,
      Dbg_Shift_3 => open,
      Dbg_Update_3 => open,
      Dbg_Rst_3 => open,
      Dbg_Clk_4 => open,
      Dbg_TDI_4 => open,
      Dbg_TDO_4 => net_gnd0,
      Dbg_Reg_En_4 => open,
      Dbg_Capture_4 => open,
      Dbg_Shift_4 => open,
      Dbg_Update_4 => open,
      Dbg_Rst_4 => open,
      Dbg_Clk_5 => open,
      Dbg_TDI_5 => open,
      Dbg_TDO_5 => net_gnd0,
      Dbg_Reg_En_5 => open,
      Dbg_Capture_5 => open,
      Dbg_Shift_5 => open,
      Dbg_Update_5 => open,
      Dbg_Rst_5 => open,
      Dbg_Clk_6 => open,
      Dbg_TDI_6 => open,
      Dbg_TDO_6 => net_gnd0,
      Dbg_Reg_En_6 => open,
      Dbg_Capture_6 => open,
      Dbg_Shift_6 => open,
      Dbg_Update_6 => open,
      Dbg_Rst_6 => open,
      Dbg_Clk_7 => open,
      Dbg_TDI_7 => open,
      Dbg_TDO_7 => net_gnd0,
      Dbg_Reg_En_7 => open,
      Dbg_Capture_7 => open,
      Dbg_Shift_7 => open,
      Dbg_Update_7 => open,
      Dbg_Rst_7 => open,
      bscan_tdi => open,
      bscan_reset => open,
      bscan_shift => open,
      bscan_update => open,
      bscan_capture => open,
      bscan_sel1 => open,
      bscan_drck1 => open,
      bscan_tdo1 => net_gnd0,
      Ext_JTAG_DRCK => open,
      Ext_JTAG_RESET => open,
      Ext_JTAG_SEL => open,
      Ext_JTAG_CAPTURE => open,
      Ext_JTAG_SHIFT => open,
      Ext_JTAG_UPDATE => open,
      Ext_JTAG_TDI => open,
      Ext_JTAG_TDO => net_gnd0
    );

  gameboard_uart : gameboard_uart_wrapper
    port map (
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_0_SPLB_Rst(10),
      PLB_ABus => plb_0_PLB_ABus,
      PLB_PAValid => plb_0_PLB_PAValid,
      PLB_masterID => plb_0_PLB_masterID(0 to 0),
      PLB_RNW => plb_0_PLB_RNW,
      PLB_BE => plb_0_PLB_BE,
      PLB_size => plb_0_PLB_size,
      PLB_type => plb_0_PLB_type,
      PLB_wrDBus => plb_0_PLB_wrDBus,
      PLB_UABus => plb_0_PLB_UABus,
      PLB_SAValid => plb_0_PLB_SAValid,
      PLB_rdPrim => plb_0_PLB_rdPrim(10),
      PLB_wrPrim => plb_0_PLB_wrPrim(10),
      PLB_abort => plb_0_PLB_abort,
      PLB_busLock => plb_0_PLB_busLock,
      PLB_MSize => plb_0_PLB_MSize,
      PLB_lockErr => plb_0_PLB_lockErr,
      PLB_wrBurst => plb_0_PLB_wrBurst,
      PLB_rdBurst => plb_0_PLB_rdBurst,
      PLB_wrPendReq => plb_0_PLB_wrPendReq,
      PLB_rdPendReq => plb_0_PLB_rdPendReq,
      PLB_wrPendPri => plb_0_PLB_wrPendPri,
      PLB_rdPendPri => plb_0_PLB_rdPendPri,
      PLB_reqPri => plb_0_PLB_reqPri,
      PLB_TAttribute => plb_0_PLB_TAttribute,
      Sl_addrAck => plb_0_Sl_addrAck(10),
      Sl_SSize => plb_0_Sl_SSize(20 to 21),
      Sl_wait => plb_0_Sl_wait(10),
      Sl_rearbitrate => plb_0_Sl_rearbitrate(10),
      Sl_wrDAck => plb_0_Sl_wrDAck(10),
      Sl_wrComp => plb_0_Sl_wrComp(10),
      Sl_rdDBus => plb_0_Sl_rdDBus(640 to 703),
      Sl_rdDAck => plb_0_Sl_rdDAck(10),
      Sl_rdComp => plb_0_Sl_rdComp(10),
      Sl_MBusy => plb_0_Sl_MBusy(20 to 21),
      Sl_MWrErr => plb_0_Sl_MWrErr(20 to 21),
      Sl_MRdErr => plb_0_Sl_MRdErr(20 to 21),
      Sl_wrBTerm => plb_0_Sl_wrBTerm(10),
      Sl_rdWdAddr => plb_0_Sl_rdWdAddr(40 to 43),
      Sl_rdBTerm => plb_0_Sl_rdBTerm(10),
      Sl_MIRQ => plb_0_Sl_MIRQ(20 to 21),
      RX => gameboard_uart_rx,
      TX => gameboard_uart_tx,
      Interrupt => gameboard_uart_Interrupt
    );

  xps_intc_1 : xps_intc_1_wrapper
    port map (
      SPLB_Clk => plb_clk_100MHz,
      SPLB_Rst => plb_1_SPLB_Rst(6),
      PLB_ABus => plb_1_PLB_ABus,
      PLB_PAValid => plb_1_PLB_PAValid,
      PLB_masterID => plb_1_PLB_masterID,
      PLB_RNW => plb_1_PLB_RNW,
      PLB_BE => plb_1_PLB_BE,
      PLB_size => plb_1_PLB_size,
      PLB_type => plb_1_PLB_type,
      PLB_wrDBus => plb_1_PLB_wrDBus,
      PLB_UABus => plb_1_PLB_UABus,
      PLB_SAValid => plb_1_PLB_SAValid,
      PLB_rdPrim => plb_1_PLB_rdPrim(6),
      PLB_wrPrim => plb_1_PLB_wrPrim(6),
      PLB_abort => plb_1_PLB_abort,
      PLB_busLock => plb_1_PLB_busLock,
      PLB_MSize => plb_1_PLB_MSize,
      PLB_lockErr => plb_1_PLB_lockErr,
      PLB_wrBurst => plb_1_PLB_wrBurst,
      PLB_rdBurst => plb_1_PLB_rdBurst,
      PLB_wrPendReq => plb_1_PLB_wrPendReq,
      PLB_rdPendReq => plb_1_PLB_rdPendReq,
      PLB_wrPendPri => plb_1_PLB_wrPendPri,
      PLB_rdPendPri => plb_1_PLB_rdPendPri,
      PLB_reqPri => plb_1_PLB_reqPri,
      PLB_TAttribute => plb_1_PLB_TAttribute,
      Sl_addrAck => plb_1_Sl_addrAck(6),
      Sl_SSize => plb_1_Sl_SSize(12 to 13),
      Sl_wait => plb_1_Sl_wait(6),
      Sl_rearbitrate => plb_1_Sl_rearbitrate(6),
      Sl_wrDAck => plb_1_Sl_wrDAck(6),
      Sl_wrComp => plb_1_Sl_wrComp(6),
      Sl_rdDBus => plb_1_Sl_rdDBus(384 to 447),
      Sl_rdDAck => plb_1_Sl_rdDAck(6),
      Sl_rdComp => plb_1_Sl_rdComp(6),
      Sl_MBusy => plb_1_Sl_MBusy(24 to 27),
      Sl_MWrErr => plb_1_Sl_MWrErr(24 to 27),
      Sl_MRdErr => plb_1_Sl_MRdErr(24 to 27),
      Sl_wrBTerm => plb_1_Sl_wrBTerm(6),
      Sl_rdWdAddr => plb_1_Sl_rdWdAddr(24 to 27),
      Sl_rdBTerm => plb_1_Sl_rdBTerm(6),
      Sl_MIRQ => plb_1_Sl_MIRQ(24 to 27),
      Intr => pgassign6,
      Irq => ext_int_ppc405_1
    );

  iobuf_0 : IOBUF
    port map (
      I => gpio_helios_O(4),
      IO => gpio_helios(4),
      O => gpio_helios_I(4),
      T => gpio_helios_T(4)
    );

  iobuf_1 : IOBUF
    port map (
      I => gpio_helios_O(3),
      IO => gpio_helios(3),
      O => gpio_helios_I(3),
      T => gpio_helios_T(3)
    );

  iobuf_2 : IOBUF
    port map (
      I => gpio_helios_O(2),
      IO => gpio_helios(2),
      O => gpio_helios_I(2),
      T => gpio_helios_T(2)
    );

  iobuf_3 : IOBUF
    port map (
      I => gpio_helios_O(1),
      IO => gpio_helios(1),
      O => gpio_helios_I(1),
      T => gpio_helios_T(1)
    );

  iobuf_4 : IOBUF
    port map (
      I => gpio_helios_O(0),
      IO => gpio_helios(0),
      O => gpio_helios_I(0),
      T => gpio_helios_T(0)
    );

  iobuf_5 : IOBUF
    port map (
      I => gpio_helios_game_O(15),
      IO => gpio_helios_game(15),
      O => gpio_helios_game_I(15),
      T => gpio_helios_game_T(15)
    );

  iobuf_6 : IOBUF
    port map (
      I => gpio_helios_game_O(14),
      IO => gpio_helios_game(14),
      O => gpio_helios_game_I(14),
      T => gpio_helios_game_T(14)
    );

  iobuf_7 : IOBUF
    port map (
      I => gpio_helios_game_O(13),
      IO => gpio_helios_game(13),
      O => gpio_helios_game_I(13),
      T => gpio_helios_game_T(13)
    );

  iobuf_8 : IOBUF
    port map (
      I => gpio_helios_game_O(12),
      IO => gpio_helios_game(12),
      O => gpio_helios_game_I(12),
      T => gpio_helios_game_T(12)
    );

  iobuf_9 : IOBUF
    port map (
      I => gpio_helios_game_O(11),
      IO => gpio_helios_game(11),
      O => gpio_helios_game_I(11),
      T => gpio_helios_game_T(11)
    );

  iobuf_10 : IOBUF
    port map (
      I => gpio_helios_game_O(10),
      IO => gpio_helios_game(10),
      O => gpio_helios_game_I(10),
      T => gpio_helios_game_T(10)
    );

  iobuf_11 : IOBUF
    port map (
      I => gpio_helios_game_O(9),
      IO => gpio_helios_game(9),
      O => gpio_helios_game_I(9),
      T => gpio_helios_game_T(9)
    );

  iobuf_12 : IOBUF
    port map (
      I => gpio_helios_game_O(8),
      IO => gpio_helios_game(8),
      O => gpio_helios_game_I(8),
      T => gpio_helios_game_T(8)
    );

  iobuf_13 : IOBUF
    port map (
      I => gpio_helios_game_O(7),
      IO => gpio_helios_game(7),
      O => gpio_helios_game_I(7),
      T => gpio_helios_game_T(7)
    );

  iobuf_14 : IOBUF
    port map (
      I => gpio_helios_game_O(6),
      IO => gpio_helios_game(6),
      O => gpio_helios_game_I(6),
      T => gpio_helios_game_T(6)
    );

  iobuf_15 : IOBUF
    port map (
      I => gpio_helios_game_O(5),
      IO => gpio_helios_game(5),
      O => gpio_helios_game_I(5),
      T => gpio_helios_game_T(5)
    );

  iobuf_16 : IOBUF
    port map (
      I => gpio_helios_game_O(4),
      IO => gpio_helios_game(4),
      O => gpio_helios_game_I(4),
      T => gpio_helios_game_T(4)
    );

  iobuf_17 : IOBUF
    port map (
      I => gpio_helios_game_O(3),
      IO => gpio_helios_game(3),
      O => gpio_helios_game_I(3),
      T => gpio_helios_game_T(3)
    );

  iobuf_18 : IOBUF
    port map (
      I => gpio_helios_game_O(2),
      IO => gpio_helios_game(2),
      O => gpio_helios_game_I(2),
      T => gpio_helios_game_T(2)
    );

  iobuf_19 : IOBUF
    port map (
      I => gpio_helios_game_O(1),
      IO => gpio_helios_game(1),
      O => gpio_helios_game_I(1),
      T => gpio_helios_game_T(1)
    );

  iobuf_20 : IOBUF
    port map (
      I => gpio_helios_game_O(0),
      IO => gpio_helios_game(0),
      O => gpio_helios_game_I(0),
      T => gpio_helios_game_T(0)
    );

  iobuf_21 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(0),
      IO => xps_mch_emc_flash_Mem_DQ(0),
      O => xps_mch_emc_0_Mem_DQ_I(0),
      T => xps_mch_emc_0_Mem_DQ_T(0)
    );

  iobuf_22 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(1),
      IO => xps_mch_emc_flash_Mem_DQ(1),
      O => xps_mch_emc_0_Mem_DQ_I(1),
      T => xps_mch_emc_0_Mem_DQ_T(1)
    );

  iobuf_23 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(2),
      IO => xps_mch_emc_flash_Mem_DQ(2),
      O => xps_mch_emc_0_Mem_DQ_I(2),
      T => xps_mch_emc_0_Mem_DQ_T(2)
    );

  iobuf_24 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(3),
      IO => xps_mch_emc_flash_Mem_DQ(3),
      O => xps_mch_emc_0_Mem_DQ_I(3),
      T => xps_mch_emc_0_Mem_DQ_T(3)
    );

  iobuf_25 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(4),
      IO => xps_mch_emc_flash_Mem_DQ(4),
      O => xps_mch_emc_0_Mem_DQ_I(4),
      T => xps_mch_emc_0_Mem_DQ_T(4)
    );

  iobuf_26 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(5),
      IO => xps_mch_emc_flash_Mem_DQ(5),
      O => xps_mch_emc_0_Mem_DQ_I(5),
      T => xps_mch_emc_0_Mem_DQ_T(5)
    );

  iobuf_27 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(6),
      IO => xps_mch_emc_flash_Mem_DQ(6),
      O => xps_mch_emc_0_Mem_DQ_I(6),
      T => xps_mch_emc_0_Mem_DQ_T(6)
    );

  iobuf_28 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(7),
      IO => xps_mch_emc_flash_Mem_DQ(7),
      O => xps_mch_emc_0_Mem_DQ_I(7),
      T => xps_mch_emc_0_Mem_DQ_T(7)
    );

  iobuf_29 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(8),
      IO => xps_mch_emc_flash_Mem_DQ(8),
      O => xps_mch_emc_0_Mem_DQ_I(8),
      T => xps_mch_emc_0_Mem_DQ_T(8)
    );

  iobuf_30 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(9),
      IO => xps_mch_emc_flash_Mem_DQ(9),
      O => xps_mch_emc_0_Mem_DQ_I(9),
      T => xps_mch_emc_0_Mem_DQ_T(9)
    );

  iobuf_31 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(10),
      IO => xps_mch_emc_flash_Mem_DQ(10),
      O => xps_mch_emc_0_Mem_DQ_I(10),
      T => xps_mch_emc_0_Mem_DQ_T(10)
    );

  iobuf_32 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(11),
      IO => xps_mch_emc_flash_Mem_DQ(11),
      O => xps_mch_emc_0_Mem_DQ_I(11),
      T => xps_mch_emc_0_Mem_DQ_T(11)
    );

  iobuf_33 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(12),
      IO => xps_mch_emc_flash_Mem_DQ(12),
      O => xps_mch_emc_0_Mem_DQ_I(12),
      T => xps_mch_emc_0_Mem_DQ_T(12)
    );

  iobuf_34 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(13),
      IO => xps_mch_emc_flash_Mem_DQ(13),
      O => xps_mch_emc_0_Mem_DQ_I(13),
      T => xps_mch_emc_0_Mem_DQ_T(13)
    );

  iobuf_35 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(14),
      IO => xps_mch_emc_flash_Mem_DQ(14),
      O => xps_mch_emc_0_Mem_DQ_I(14),
      T => xps_mch_emc_0_Mem_DQ_T(14)
    );

  iobuf_36 : IOBUF
    port map (
      I => xps_mch_emc_0_Mem_DQ_O(15),
      IO => xps_mch_emc_flash_Mem_DQ(15),
      O => xps_mch_emc_0_Mem_DQ_I(15),
      T => xps_mch_emc_0_Mem_DQ_T(15)
    );

  iobuf_37 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(0),
      IO => xps_mch_emc_sram_Mem_DQ(0),
      O => xps_mch_emc_sram_Mem_DQ_I(0),
      T => xps_mch_emc_sram_Mem_DQ_T(0)
    );

  iobuf_38 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(1),
      IO => xps_mch_emc_sram_Mem_DQ(1),
      O => xps_mch_emc_sram_Mem_DQ_I(1),
      T => xps_mch_emc_sram_Mem_DQ_T(1)
    );

  iobuf_39 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(2),
      IO => xps_mch_emc_sram_Mem_DQ(2),
      O => xps_mch_emc_sram_Mem_DQ_I(2),
      T => xps_mch_emc_sram_Mem_DQ_T(2)
    );

  iobuf_40 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(3),
      IO => xps_mch_emc_sram_Mem_DQ(3),
      O => xps_mch_emc_sram_Mem_DQ_I(3),
      T => xps_mch_emc_sram_Mem_DQ_T(3)
    );

  iobuf_41 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(4),
      IO => xps_mch_emc_sram_Mem_DQ(4),
      O => xps_mch_emc_sram_Mem_DQ_I(4),
      T => xps_mch_emc_sram_Mem_DQ_T(4)
    );

  iobuf_42 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(5),
      IO => xps_mch_emc_sram_Mem_DQ(5),
      O => xps_mch_emc_sram_Mem_DQ_I(5),
      T => xps_mch_emc_sram_Mem_DQ_T(5)
    );

  iobuf_43 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(6),
      IO => xps_mch_emc_sram_Mem_DQ(6),
      O => xps_mch_emc_sram_Mem_DQ_I(6),
      T => xps_mch_emc_sram_Mem_DQ_T(6)
    );

  iobuf_44 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(7),
      IO => xps_mch_emc_sram_Mem_DQ(7),
      O => xps_mch_emc_sram_Mem_DQ_I(7),
      T => xps_mch_emc_sram_Mem_DQ_T(7)
    );

  iobuf_45 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(8),
      IO => xps_mch_emc_sram_Mem_DQ(8),
      O => xps_mch_emc_sram_Mem_DQ_I(8),
      T => xps_mch_emc_sram_Mem_DQ_T(8)
    );

  iobuf_46 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(9),
      IO => xps_mch_emc_sram_Mem_DQ(9),
      O => xps_mch_emc_sram_Mem_DQ_I(9),
      T => xps_mch_emc_sram_Mem_DQ_T(9)
    );

  iobuf_47 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(10),
      IO => xps_mch_emc_sram_Mem_DQ(10),
      O => xps_mch_emc_sram_Mem_DQ_I(10),
      T => xps_mch_emc_sram_Mem_DQ_T(10)
    );

  iobuf_48 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(11),
      IO => xps_mch_emc_sram_Mem_DQ(11),
      O => xps_mch_emc_sram_Mem_DQ_I(11),
      T => xps_mch_emc_sram_Mem_DQ_T(11)
    );

  iobuf_49 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(12),
      IO => xps_mch_emc_sram_Mem_DQ(12),
      O => xps_mch_emc_sram_Mem_DQ_I(12),
      T => xps_mch_emc_sram_Mem_DQ_T(12)
    );

  iobuf_50 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(13),
      IO => xps_mch_emc_sram_Mem_DQ(13),
      O => xps_mch_emc_sram_Mem_DQ_I(13),
      T => xps_mch_emc_sram_Mem_DQ_T(13)
    );

  iobuf_51 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(14),
      IO => xps_mch_emc_sram_Mem_DQ(14),
      O => xps_mch_emc_sram_Mem_DQ_I(14),
      T => xps_mch_emc_sram_Mem_DQ_T(14)
    );

  iobuf_52 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(15),
      IO => xps_mch_emc_sram_Mem_DQ(15),
      O => xps_mch_emc_sram_Mem_DQ_I(15),
      T => xps_mch_emc_sram_Mem_DQ_T(15)
    );

  iobuf_53 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(16),
      IO => xps_mch_emc_sram_Mem_DQ(16),
      O => xps_mch_emc_sram_Mem_DQ_I(16),
      T => xps_mch_emc_sram_Mem_DQ_T(16)
    );

  iobuf_54 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(17),
      IO => xps_mch_emc_sram_Mem_DQ(17),
      O => xps_mch_emc_sram_Mem_DQ_I(17),
      T => xps_mch_emc_sram_Mem_DQ_T(17)
    );

  iobuf_55 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(18),
      IO => xps_mch_emc_sram_Mem_DQ(18),
      O => xps_mch_emc_sram_Mem_DQ_I(18),
      T => xps_mch_emc_sram_Mem_DQ_T(18)
    );

  iobuf_56 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(19),
      IO => xps_mch_emc_sram_Mem_DQ(19),
      O => xps_mch_emc_sram_Mem_DQ_I(19),
      T => xps_mch_emc_sram_Mem_DQ_T(19)
    );

  iobuf_57 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(20),
      IO => xps_mch_emc_sram_Mem_DQ(20),
      O => xps_mch_emc_sram_Mem_DQ_I(20),
      T => xps_mch_emc_sram_Mem_DQ_T(20)
    );

  iobuf_58 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(21),
      IO => xps_mch_emc_sram_Mem_DQ(21),
      O => xps_mch_emc_sram_Mem_DQ_I(21),
      T => xps_mch_emc_sram_Mem_DQ_T(21)
    );

  iobuf_59 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(22),
      IO => xps_mch_emc_sram_Mem_DQ(22),
      O => xps_mch_emc_sram_Mem_DQ_I(22),
      T => xps_mch_emc_sram_Mem_DQ_T(22)
    );

  iobuf_60 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(23),
      IO => xps_mch_emc_sram_Mem_DQ(23),
      O => xps_mch_emc_sram_Mem_DQ_I(23),
      T => xps_mch_emc_sram_Mem_DQ_T(23)
    );

  iobuf_61 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(24),
      IO => xps_mch_emc_sram_Mem_DQ(24),
      O => xps_mch_emc_sram_Mem_DQ_I(24),
      T => xps_mch_emc_sram_Mem_DQ_T(24)
    );

  iobuf_62 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(25),
      IO => xps_mch_emc_sram_Mem_DQ(25),
      O => xps_mch_emc_sram_Mem_DQ_I(25),
      T => xps_mch_emc_sram_Mem_DQ_T(25)
    );

  iobuf_63 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(26),
      IO => xps_mch_emc_sram_Mem_DQ(26),
      O => xps_mch_emc_sram_Mem_DQ_I(26),
      T => xps_mch_emc_sram_Mem_DQ_T(26)
    );

  iobuf_64 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(27),
      IO => xps_mch_emc_sram_Mem_DQ(27),
      O => xps_mch_emc_sram_Mem_DQ_I(27),
      T => xps_mch_emc_sram_Mem_DQ_T(27)
    );

  iobuf_65 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(28),
      IO => xps_mch_emc_sram_Mem_DQ(28),
      O => xps_mch_emc_sram_Mem_DQ_I(28),
      T => xps_mch_emc_sram_Mem_DQ_T(28)
    );

  iobuf_66 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(29),
      IO => xps_mch_emc_sram_Mem_DQ(29),
      O => xps_mch_emc_sram_Mem_DQ_I(29),
      T => xps_mch_emc_sram_Mem_DQ_T(29)
    );

  iobuf_67 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(30),
      IO => xps_mch_emc_sram_Mem_DQ(30),
      O => xps_mch_emc_sram_Mem_DQ_I(30),
      T => xps_mch_emc_sram_Mem_DQ_T(30)
    );

  iobuf_68 : IOBUF
    port map (
      I => xps_mch_emc_sram_Mem_DQ_O(31),
      IO => xps_mch_emc_sram_Mem_DQ(31),
      O => xps_mch_emc_sram_Mem_DQ_I(31),
      T => xps_mch_emc_sram_Mem_DQ_T(31)
    );

  iobuf_69 : IOBUF
    port map (
      I => cam0_sdata_O,
      IO => cam0_sdata,
      O => cam0_sdata_I,
      T => cam0_sdata_T
    );

  iobuf_70 : IOBUF
    port map (
      I => usb_fd_O(15),
      IO => usb_fd(15),
      O => usb_fd_I(15),
      T => usb_fd_T(15)
    );

  iobuf_71 : IOBUF
    port map (
      I => usb_fd_O(14),
      IO => usb_fd(14),
      O => usb_fd_I(14),
      T => usb_fd_T(14)
    );

  iobuf_72 : IOBUF
    port map (
      I => usb_fd_O(13),
      IO => usb_fd(13),
      O => usb_fd_I(13),
      T => usb_fd_T(13)
    );

  iobuf_73 : IOBUF
    port map (
      I => usb_fd_O(12),
      IO => usb_fd(12),
      O => usb_fd_I(12),
      T => usb_fd_T(12)
    );

  iobuf_74 : IOBUF
    port map (
      I => usb_fd_O(11),
      IO => usb_fd(11),
      O => usb_fd_I(11),
      T => usb_fd_T(11)
    );

  iobuf_75 : IOBUF
    port map (
      I => usb_fd_O(10),
      IO => usb_fd(10),
      O => usb_fd_I(10),
      T => usb_fd_T(10)
    );

  iobuf_76 : IOBUF
    port map (
      I => usb_fd_O(9),
      IO => usb_fd(9),
      O => usb_fd_I(9),
      T => usb_fd_T(9)
    );

  iobuf_77 : IOBUF
    port map (
      I => usb_fd_O(8),
      IO => usb_fd(8),
      O => usb_fd_I(8),
      T => usb_fd_T(8)
    );

  iobuf_78 : IOBUF
    port map (
      I => usb_fd_O(7),
      IO => usb_fd(7),
      O => usb_fd_I(7),
      T => usb_fd_T(7)
    );

  iobuf_79 : IOBUF
    port map (
      I => usb_fd_O(6),
      IO => usb_fd(6),
      O => usb_fd_I(6),
      T => usb_fd_T(6)
    );

  iobuf_80 : IOBUF
    port map (
      I => usb_fd_O(5),
      IO => usb_fd(5),
      O => usb_fd_I(5),
      T => usb_fd_T(5)
    );

  iobuf_81 : IOBUF
    port map (
      I => usb_fd_O(4),
      IO => usb_fd(4),
      O => usb_fd_I(4),
      T => usb_fd_T(4)
    );

  iobuf_82 : IOBUF
    port map (
      I => usb_fd_O(3),
      IO => usb_fd(3),
      O => usb_fd_I(3),
      T => usb_fd_T(3)
    );

  iobuf_83 : IOBUF
    port map (
      I => usb_fd_O(2),
      IO => usb_fd(2),
      O => usb_fd_I(2),
      T => usb_fd_T(2)
    );

  iobuf_84 : IOBUF
    port map (
      I => usb_fd_O(1),
      IO => usb_fd(1),
      O => usb_fd_I(1),
      T => usb_fd_T(1)
    );

  iobuf_85 : IOBUF
    port map (
      I => usb_fd_O(0),
      IO => usb_fd(0),
      O => usb_fd_I(0),
      T => usb_fd_T(0)
    );

end architecture STRUCTURE;

