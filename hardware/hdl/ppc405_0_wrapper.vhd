-------------------------------------------------------------------------------
-- ppc405_0_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library ppc405_virtex4_v2_01_b;
use ppc405_virtex4_v2_01_b.all;

entity ppc405_0_wrapper is
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

  attribute x_core_info : STRING;
  attribute x_core_info of ppc405_0_wrapper : entity is "ppc405_virtex4_v2_01_b";

end ppc405_0_wrapper;

architecture STRUCTURE of ppc405_0_wrapper is

  component ppc405_virtex4 is
    generic (
      C_DPLB0_DWIDTH : INTEGER;
      C_DPLB0_NATIVE_DWIDTH : integer;
      C_IPLB0_DWIDTH : INTEGER;
      C_IPLB0_NATIVE_DWIDTH : integer;
      C_DPLB1_DWIDTH : INTEGER;
      C_DPLB1_NATIVE_DWIDTH : integer;
      C_IPLB1_DWIDTH : INTEGER;
      C_IPLB1_NATIVE_DWIDTH : integer;
      C_DPLB1_ADDR_BASE : std_logic_vector(0 to 31);
      C_DPLB1_ADDR_HIGH : std_logic_vector(0 to 31);
      C_IPLB1_ADDR_BASE : std_logic_vector(0 to 31);
      C_IPLB1_ADDR_HIGH : std_logic_vector(0 to 31);
      C_FASTEST_PLB_CLOCK : string;
      C_IDCR_BASEADDR : std_logic_vector(0 to 9);
      C_IDCR_HIGHADDR : std_logic_vector(0 to 9);
      C_DISABLE_OPERAND_FORWARDING : INTEGER;
      C_MMU_ENABLE : INTEGER;
      C_DETERMINISTIC_MULT : INTEGER;
      C_PLBSYNCBYPASS : INTEGER;
      C_APU_CONTROL : std_logic_vector(0 to 15);
      C_APU_UDI_1 : std_logic_vector(0 to 23);
      C_APU_UDI_2 : std_logic_vector(0 to 23);
      C_APU_UDI_3 : std_logic_vector(0 to 23);
      C_APU_UDI_4 : std_logic_vector(0 to 23);
      C_APU_UDI_5 : std_logic_vector(0 to 23);
      C_APU_UDI_6 : std_logic_vector(0 to 23);
      C_APU_UDI_7 : std_logic_vector(0 to 23);
      C_APU_UDI_8 : std_logic_vector(0 to 23);
      C_PVR_HIGH : std_logic_vector(0 to 3);
      C_PVR_LOW : std_logic_vector(0 to 3)
    );
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
      IPLB0_PLB_MRdDBus : in std_logic_vector(0 to (C_IPLB0_DWIDTH-1));
      IPLB0_PLB_MRearbitrate : in std_logic;
      IPLB0_PLB_MSSize : in std_logic_vector(0 to 1);
      IPLB0_PLB_MTimeout : in std_logic;
      IPLB0_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      IPLB0_M_ABus : out std_logic_vector(0 to 31);
      IPLB0_M_BE : out std_logic_vector(0 to ((C_IPLB0_DWIDTH/8)-1));
      IPLB0_M_MSize : out std_logic_vector(0 to 1);
      IPLB0_M_rdBurst : out std_logic;
      IPLB0_M_request : out std_logic;
      IPLB0_M_RNW : out std_logic;
      IPLB0_M_size : out std_logic_vector(0 to 3);
      IPLB0_M_wrBurst : out std_logic;
      IPLB0_M_wrDBus : out std_logic_vector(0 to (C_IPLB0_DWIDTH-1));
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
      DPLB0_PLB_MRdDBus : in std_logic_vector(0 to (C_DPLB0_DWIDTH-1));
      DPLB0_PLB_MRearbitrate : in std_logic;
      DPLB0_PLB_MSSize : in std_logic_vector(0 to 1);
      DPLB0_PLB_MTimeout : in std_logic;
      DPLB0_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      DPLB0_M_ABus : out std_logic_vector(0 to 31);
      DPLB0_M_BE : out std_logic_vector(0 to ((C_DPLB0_DWIDTH/8)-1));
      DPLB0_M_MSize : out std_logic_vector(0 to 1);
      DPLB0_M_rdBurst : out std_logic;
      DPLB0_M_request : out std_logic;
      DPLB0_M_RNW : out std_logic;
      DPLB0_M_size : out std_logic_vector(0 to 3);
      DPLB0_M_wrBurst : out std_logic;
      DPLB0_M_wrDBus : out std_logic_vector(0 to (C_DPLB0_DWIDTH-1));
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
      IPLB1_PLB_MRdDBus : in std_logic_vector(0 to (C_IPLB1_DWIDTH-1));
      IPLB1_PLB_MRearbitrate : in std_logic;
      IPLB1_PLB_MSSize : in std_logic_vector(0 to 1);
      IPLB1_PLB_MTimeout : in std_logic;
      IPLB1_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      IPLB1_M_ABus : out std_logic_vector(0 to 31);
      IPLB1_M_BE : out std_logic_vector(0 to ((C_IPLB1_DWIDTH/8)-1));
      IPLB1_M_MSize : out std_logic_vector(0 to 1);
      IPLB1_M_rdBurst : out std_logic;
      IPLB1_M_request : out std_logic;
      IPLB1_M_RNW : out std_logic;
      IPLB1_M_size : out std_logic_vector(0 to 3);
      IPLB1_M_wrBurst : out std_logic;
      IPLB1_M_wrDBus : out std_logic_vector(0 to (C_IPLB1_DWIDTH-1));
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
      DPLB1_PLB_MRdDBus : in std_logic_vector(0 to (C_DPLB1_DWIDTH-1));
      DPLB1_PLB_MRearbitrate : in std_logic;
      DPLB1_PLB_MSSize : in std_logic_vector(0 to 1);
      DPLB1_PLB_MTimeout : in std_logic;
      DPLB1_PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      DPLB1_M_ABus : out std_logic_vector(0 to 31);
      DPLB1_M_BE : out std_logic_vector(0 to ((C_DPLB1_DWIDTH/8)-1));
      DPLB1_M_MSize : out std_logic_vector(0 to 1);
      DPLB1_M_rdBurst : out std_logic;
      DPLB1_M_request : out std_logic;
      DPLB1_M_RNW : out std_logic;
      DPLB1_M_size : out std_logic_vector(0 to 3);
      DPLB1_M_wrBurst : out std_logic;
      DPLB1_M_wrDBus : out std_logic_vector(0 to (C_DPLB1_DWIDTH-1));
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

begin

  ppc405_0 : ppc405_virtex4
    generic map (
      C_DPLB0_DWIDTH => 64,
      C_DPLB0_NATIVE_DWIDTH => 64,
      C_IPLB0_DWIDTH => 64,
      C_IPLB0_NATIVE_DWIDTH => 64,
      C_DPLB1_DWIDTH => 64,
      C_DPLB1_NATIVE_DWIDTH => 64,
      C_IPLB1_DWIDTH => 64,
      C_IPLB1_NATIVE_DWIDTH => 64,
      C_DPLB1_ADDR_BASE => X"ffffffff",
      C_DPLB1_ADDR_HIGH => X"00000000",
      C_IPLB1_ADDR_BASE => X"ffffffff",
      C_IPLB1_ADDR_HIGH => X"00000000",
      C_FASTEST_PLB_CLOCK => "DPLB0",
      C_IDCR_BASEADDR => B"0100000000",
      C_IDCR_HIGHADDR => B"0111111111",
      C_DISABLE_OPERAND_FORWARDING => 1,
      C_MMU_ENABLE => 1,
      C_DETERMINISTIC_MULT => 0,
      C_PLBSYNCBYPASS => 1,
      C_APU_CONTROL => B"1101111000000000",
      C_APU_UDI_1 => B"101000011000100110000011",
      C_APU_UDI_2 => B"101000111000100110000011",
      C_APU_UDI_3 => B"101001011000100111000011",
      C_APU_UDI_4 => B"101001111000100111000011",
      C_APU_UDI_5 => B"101010011000110000000011",
      C_APU_UDI_6 => B"101010111000110000000011",
      C_APU_UDI_7 => B"101011011000110001000011",
      C_APU_UDI_8 => B"101011111000110001000011",
      C_PVR_HIGH => B"0000",
      C_PVR_LOW => B"0000"
    )
    port map (
      C405CPMCORESLEEPREQ => C405CPMCORESLEEPREQ,
      C405CPMMSRCE => C405CPMMSRCE,
      C405CPMMSREE => C405CPMMSREE,
      C405CPMTIMERIRQ => C405CPMTIMERIRQ,
      C405CPMTIMERRESETREQ => C405CPMTIMERRESETREQ,
      C405XXXMACHINECHECK => C405XXXMACHINECHECK,
      CPMC405CLOCK => CPMC405CLOCK,
      CPMC405CORECLKINACTIVE => CPMC405CORECLKINACTIVE,
      CPMC405CPUCLKEN => CPMC405CPUCLKEN,
      CPMC405JTAGCLKEN => CPMC405JTAGCLKEN,
      CPMC405TIMERCLKEN => CPMC405TIMERCLKEN,
      CPMC405TIMERTICK => CPMC405TIMERTICK,
      MCBCPUCLKEN => MCBCPUCLKEN,
      MCBTIMEREN => MCBTIMEREN,
      MCPPCRST => MCPPCRST,
      CPMDCRCLK => CPMDCRCLK,
      CPMFCMCLK => CPMFCMCLK,
      C405RSTCHIPRESETREQ => C405RSTCHIPRESETREQ,
      C405RSTCORERESETREQ => C405RSTCORERESETREQ,
      C405RSTSYSRESETREQ => C405RSTSYSRESETREQ,
      RSTC405RESETCHIP => RSTC405RESETCHIP,
      RSTC405RESETCORE => RSTC405RESETCORE,
      RSTC405RESETSYS => RSTC405RESETSYS,
      APUFCMDECODED => APUFCMDECODED,
      APUFCMDECUDI => APUFCMDECUDI,
      APUFCMDECUDIVALID => APUFCMDECUDIVALID,
      APUFCMENDIAN => APUFCMENDIAN,
      APUFCMFLUSH => APUFCMFLUSH,
      APUFCMINSTRUCTION => APUFCMINSTRUCTION,
      APUFCMINSTRVALID => APUFCMINSTRVALID,
      APUFCMLOADBYTEEN => APUFCMLOADBYTEEN,
      APUFCMLOADDATA => APUFCMLOADDATA,
      APUFCMLOADDVALID => APUFCMLOADDVALID,
      APUFCMOPERANDVALID => APUFCMOPERANDVALID,
      APUFCMRADATA => APUFCMRADATA,
      APUFCMRBDATA => APUFCMRBDATA,
      APUFCMWRITEBACKOK => APUFCMWRITEBACKOK,
      APUFCMXERCA => APUFCMXERCA,
      FCMAPUCR => FCMAPUCR,
      FCMAPUDCDCREN => FCMAPUDCDCREN,
      FCMAPUDCDFORCEALIGN => FCMAPUDCDFORCEALIGN,
      FCMAPUDCDFORCEBESTEERING => FCMAPUDCDFORCEBESTEERING,
      FCMAPUDCDFPUOP => FCMAPUDCDFPUOP,
      FCMAPUDCDGPRWRITE => FCMAPUDCDGPRWRITE,
      FCMAPUDCDLDSTBYTE => FCMAPUDCDLDSTBYTE,
      FCMAPUDCDLDSTDW => FCMAPUDCDLDSTDW,
      FCMAPUDCDLDSTHW => FCMAPUDCDLDSTHW,
      FCMAPUDCDLDSTQW => FCMAPUDCDLDSTQW,
      FCMAPUDCDLDSTWD => FCMAPUDCDLDSTWD,
      FCMAPUDCDLOAD => FCMAPUDCDLOAD,
      FCMAPUDCDPRIVOP => FCMAPUDCDPRIVOP,
      FCMAPUDCDRAEN => FCMAPUDCDRAEN,
      FCMAPUDCDRBEN => FCMAPUDCDRBEN,
      FCMAPUDCDSTORE => FCMAPUDCDSTORE,
      FCMAPUDCDTRAPBE => FCMAPUDCDTRAPBE,
      FCMAPUDCDTRAPLE => FCMAPUDCDTRAPLE,
      FCMAPUDCDUPDATE => FCMAPUDCDUPDATE,
      FCMAPUDCDXERCAEN => FCMAPUDCDXERCAEN,
      FCMAPUDCDXEROVEN => FCMAPUDCDXEROVEN,
      FCMAPUDECODEBUSY => FCMAPUDECODEBUSY,
      FCMAPUDONE => FCMAPUDONE,
      FCMAPUEXCEPTION => FCMAPUEXCEPTION,
      FCMAPUEXEBLOCKINGMCO => FCMAPUEXEBLOCKINGMCO,
      FCMAPUEXECRFIELD => FCMAPUEXECRFIELD,
      FCMAPUEXENONBLOCKINGMCO => FCMAPUEXENONBLOCKINGMCO,
      FCMAPUINSTRACK => FCMAPUINSTRACK,
      FCMAPULOADWAIT => FCMAPULOADWAIT,
      FCMAPURESULT => FCMAPURESULT,
      FCMAPURESULTVALID => FCMAPURESULTVALID,
      FCMAPUSLEEPNOTREADY => FCMAPUSLEEPNOTREADY,
      FCMAPUXERCA => FCMAPUXERCA,
      FCMAPUXEROV => FCMAPUXEROV,
      IPLB0_PLB_Clk => IPLB0_PLB_Clk,
      IPLB0_PLB_Rst => IPLB0_PLB_Rst,
      IPLB0_PLB_MBusy => IPLB0_PLB_MBusy,
      IPLB0_PLB_MRdErr => IPLB0_PLB_MRdErr,
      IPLB0_PLB_MWrErr => IPLB0_PLB_MWrErr,
      IPLB0_PLB_MWrBTerm => IPLB0_PLB_MWrBTerm,
      IPLB0_PLB_MWrDAck => IPLB0_PLB_MWrDAck,
      IPLB0_PLB_MAddrAck => IPLB0_PLB_MAddrAck,
      IPLB0_PLB_MRdBTerm => IPLB0_PLB_MRdBTerm,
      IPLB0_PLB_MRdDAck => IPLB0_PLB_MRdDAck,
      IPLB0_PLB_MRdDBus => IPLB0_PLB_MRdDBus,
      IPLB0_PLB_MRearbitrate => IPLB0_PLB_MRearbitrate,
      IPLB0_PLB_MSSize => IPLB0_PLB_MSSize,
      IPLB0_PLB_MTimeout => IPLB0_PLB_MTimeout,
      IPLB0_PLB_MRdWdAddr => IPLB0_PLB_MRdWdAddr,
      IPLB0_M_ABus => IPLB0_M_ABus,
      IPLB0_M_BE => IPLB0_M_BE,
      IPLB0_M_MSize => IPLB0_M_MSize,
      IPLB0_M_rdBurst => IPLB0_M_rdBurst,
      IPLB0_M_request => IPLB0_M_request,
      IPLB0_M_RNW => IPLB0_M_RNW,
      IPLB0_M_size => IPLB0_M_size,
      IPLB0_M_wrBurst => IPLB0_M_wrBurst,
      IPLB0_M_wrDBus => IPLB0_M_wrDBus,
      IPLB0_M_abort => IPLB0_M_abort,
      IPLB0_M_UABus => IPLB0_M_UABus,
      IPLB0_M_busLock => IPLB0_M_busLock,
      IPLB0_M_lockErr => IPLB0_M_lockErr,
      IPLB0_M_priority => IPLB0_M_priority,
      IPLB0_M_type => IPLB0_M_type,
      IPLB0_M_TAttribute => IPLB0_M_TAttribute,
      DPLB0_PLB_Clk => DPLB0_PLB_Clk,
      DPLB0_PLB_Rst => DPLB0_PLB_Rst,
      DPLB0_PLB_MBusy => DPLB0_PLB_MBusy,
      DPLB0_PLB_MRdErr => DPLB0_PLB_MRdErr,
      DPLB0_PLB_MWrErr => DPLB0_PLB_MWrErr,
      DPLB0_PLB_MWrBTerm => DPLB0_PLB_MWrBTerm,
      DPLB0_PLB_MWrDAck => DPLB0_PLB_MWrDAck,
      DPLB0_PLB_MAddrAck => DPLB0_PLB_MAddrAck,
      DPLB0_PLB_MRdBTerm => DPLB0_PLB_MRdBTerm,
      DPLB0_PLB_MRdDAck => DPLB0_PLB_MRdDAck,
      DPLB0_PLB_MRdDBus => DPLB0_PLB_MRdDBus,
      DPLB0_PLB_MRearbitrate => DPLB0_PLB_MRearbitrate,
      DPLB0_PLB_MSSize => DPLB0_PLB_MSSize,
      DPLB0_PLB_MTimeout => DPLB0_PLB_MTimeout,
      DPLB0_PLB_MRdWdAddr => DPLB0_PLB_MRdWdAddr,
      DPLB0_M_ABus => DPLB0_M_ABus,
      DPLB0_M_BE => DPLB0_M_BE,
      DPLB0_M_MSize => DPLB0_M_MSize,
      DPLB0_M_rdBurst => DPLB0_M_rdBurst,
      DPLB0_M_request => DPLB0_M_request,
      DPLB0_M_RNW => DPLB0_M_RNW,
      DPLB0_M_size => DPLB0_M_size,
      DPLB0_M_wrBurst => DPLB0_M_wrBurst,
      DPLB0_M_wrDBus => DPLB0_M_wrDBus,
      DPLB0_M_abort => DPLB0_M_abort,
      DPLB0_M_UABus => DPLB0_M_UABus,
      DPLB0_M_busLock => DPLB0_M_busLock,
      DPLB0_M_lockErr => DPLB0_M_lockErr,
      DPLB0_M_priority => DPLB0_M_priority,
      DPLB0_M_type => DPLB0_M_type,
      DPLB0_M_TAttribute => DPLB0_M_TAttribute,
      IPLB1_PLB_Clk => IPLB1_PLB_Clk,
      IPLB1_PLB_Rst => IPLB1_PLB_Rst,
      IPLB1_PLB_MBusy => IPLB1_PLB_MBusy,
      IPLB1_PLB_MRdErr => IPLB1_PLB_MRdErr,
      IPLB1_PLB_MWrErr => IPLB1_PLB_MWrErr,
      IPLB1_PLB_MWrBTerm => IPLB1_PLB_MWrBTerm,
      IPLB1_PLB_MWrDAck => IPLB1_PLB_MWrDAck,
      IPLB1_PLB_MAddrAck => IPLB1_PLB_MAddrAck,
      IPLB1_PLB_MRdBTerm => IPLB1_PLB_MRdBTerm,
      IPLB1_PLB_MRdDAck => IPLB1_PLB_MRdDAck,
      IPLB1_PLB_MRdDBus => IPLB1_PLB_MRdDBus,
      IPLB1_PLB_MRearbitrate => IPLB1_PLB_MRearbitrate,
      IPLB1_PLB_MSSize => IPLB1_PLB_MSSize,
      IPLB1_PLB_MTimeout => IPLB1_PLB_MTimeout,
      IPLB1_PLB_MRdWdAddr => IPLB1_PLB_MRdWdAddr,
      IPLB1_M_ABus => IPLB1_M_ABus,
      IPLB1_M_BE => IPLB1_M_BE,
      IPLB1_M_MSize => IPLB1_M_MSize,
      IPLB1_M_rdBurst => IPLB1_M_rdBurst,
      IPLB1_M_request => IPLB1_M_request,
      IPLB1_M_RNW => IPLB1_M_RNW,
      IPLB1_M_size => IPLB1_M_size,
      IPLB1_M_wrBurst => IPLB1_M_wrBurst,
      IPLB1_M_wrDBus => IPLB1_M_wrDBus,
      IPLB1_M_abort => IPLB1_M_abort,
      IPLB1_M_UABus => IPLB1_M_UABus,
      IPLB1_M_busLock => IPLB1_M_busLock,
      IPLB1_M_lockErr => IPLB1_M_lockErr,
      IPLB1_M_priority => IPLB1_M_priority,
      IPLB1_M_type => IPLB1_M_type,
      IPLB1_M_TAttribute => IPLB1_M_TAttribute,
      DPLB1_PLB_Clk => DPLB1_PLB_Clk,
      DPLB1_PLB_Rst => DPLB1_PLB_Rst,
      DPLB1_PLB_MBusy => DPLB1_PLB_MBusy,
      DPLB1_PLB_MRdErr => DPLB1_PLB_MRdErr,
      DPLB1_PLB_MWrErr => DPLB1_PLB_MWrErr,
      DPLB1_PLB_MWrBTerm => DPLB1_PLB_MWrBTerm,
      DPLB1_PLB_MWrDAck => DPLB1_PLB_MWrDAck,
      DPLB1_PLB_MAddrAck => DPLB1_PLB_MAddrAck,
      DPLB1_PLB_MRdBTerm => DPLB1_PLB_MRdBTerm,
      DPLB1_PLB_MRdDAck => DPLB1_PLB_MRdDAck,
      DPLB1_PLB_MRdDBus => DPLB1_PLB_MRdDBus,
      DPLB1_PLB_MRearbitrate => DPLB1_PLB_MRearbitrate,
      DPLB1_PLB_MSSize => DPLB1_PLB_MSSize,
      DPLB1_PLB_MTimeout => DPLB1_PLB_MTimeout,
      DPLB1_PLB_MRdWdAddr => DPLB1_PLB_MRdWdAddr,
      DPLB1_M_ABus => DPLB1_M_ABus,
      DPLB1_M_BE => DPLB1_M_BE,
      DPLB1_M_MSize => DPLB1_M_MSize,
      DPLB1_M_rdBurst => DPLB1_M_rdBurst,
      DPLB1_M_request => DPLB1_M_request,
      DPLB1_M_RNW => DPLB1_M_RNW,
      DPLB1_M_size => DPLB1_M_size,
      DPLB1_M_wrBurst => DPLB1_M_wrBurst,
      DPLB1_M_wrDBus => DPLB1_M_wrDBus,
      DPLB1_M_abort => DPLB1_M_abort,
      DPLB1_M_UABus => DPLB1_M_UABus,
      DPLB1_M_busLock => DPLB1_M_busLock,
      DPLB1_M_lockErr => DPLB1_M_lockErr,
      DPLB1_M_priority => DPLB1_M_priority,
      DPLB1_M_type => DPLB1_M_type,
      DPLB1_M_TAttribute => DPLB1_M_TAttribute,
      BRAMDSOCMCLK => BRAMDSOCMCLK,
      BRAMDSOCMRDDBUS => BRAMDSOCMRDDBUS,
      DSARCVALUE => DSARCVALUE,
      DSCNTLVALUE => DSCNTLVALUE,
      DSOCMBRAMABUS => DSOCMBRAMABUS,
      DSOCMBRAMBYTEWRITE => DSOCMBRAMBYTEWRITE,
      DSOCMBRAMEN => DSOCMBRAMEN,
      DSOCMBRAMWRDBUS => DSOCMBRAMWRDBUS,
      DSOCMBUSY => DSOCMBUSY,
      DSOCMRDADDRVALID => DSOCMRDADDRVALID,
      DSOCMWRADDRVALID => DSOCMWRADDRVALID,
      DSOCMRWCOMPLETE => DSOCMRWCOMPLETE,
      BRAMISOCMCLK => BRAMISOCMCLK,
      BRAMISOCMRDDBUS => BRAMISOCMRDDBUS,
      BRAMISOCMDCRRDDBUS => BRAMISOCMDCRRDDBUS,
      ISARCVALUE => ISARCVALUE,
      ISCNTLVALUE => ISCNTLVALUE,
      ISOCMBRAMEN => ISOCMBRAMEN,
      ISOCMBRAMEVENWRITEEN => ISOCMBRAMEVENWRITEEN,
      ISOCMBRAMODDWRITEEN => ISOCMBRAMODDWRITEEN,
      ISOCMBRAMRDABUS => ISOCMBRAMRDABUS,
      ISOCMBRAMWRABUS => ISOCMBRAMWRABUS,
      ISOCMBRAMWRDBUS => ISOCMBRAMWRDBUS,
      ISOCMDCRBRAMEVENEN => ISOCMDCRBRAMEVENEN,
      ISOCMDCRBRAMODDEN => ISOCMDCRBRAMODDEN,
      ISOCMDCRBRAMRDSELECT => ISOCMDCRBRAMRDSELECT,
      DCREMACABUS => DCREMACABUS,
      DCREMACCLK => DCREMACCLK,
      DCREMACDBUS => DCREMACDBUS,
      DCREMACENABLER => DCREMACENABLER,
      DCREMACREAD => DCREMACREAD,
      DCREMACWRITE => DCREMACWRITE,
      EMACDCRACK => EMACDCRACK,
      EMACDCRDBUS => EMACDCRDBUS,
      EXTDCRABUS => EXTDCRABUS,
      EXTDCRDBUSOUT => EXTDCRDBUSOUT,
      EXTDCRREAD => EXTDCRREAD,
      EXTDCRWRITE => EXTDCRWRITE,
      EXTDCRACK => EXTDCRACK,
      EXTDCRDBUSIN => EXTDCRDBUSIN,
      EICC405CRITINPUTIRQ => EICC405CRITINPUTIRQ,
      EICC405EXTINPUTIRQ => EICC405EXTINPUTIRQ,
      C405JTGCAPTUREDR => C405JTGCAPTUREDR,
      C405JTGEXTEST => C405JTGEXTEST,
      C405JTGPGMOUT => C405JTGPGMOUT,
      C405JTGSHIFTDR => C405JTGSHIFTDR,
      C405JTGTDO => C405JTGTDO,
      C405JTGTDOEN => C405JTGTDOEN,
      C405JTGUPDATEDR => C405JTGUPDATEDR,
      MCBJTAGEN => MCBJTAGEN,
      JTGC405BNDSCANTDO => JTGC405BNDSCANTDO,
      JTGC405TCK => JTGC405TCK,
      JTGC405TDI => JTGC405TDI,
      JTGC405TMS => JTGC405TMS,
      JTGC405TRSTNEG => JTGC405TRSTNEG,
      C405DBGMSRWE => C405DBGMSRWE,
      C405DBGSTOPACK => C405DBGSTOPACK,
      C405DBGWBCOMPLETE => C405DBGWBCOMPLETE,
      C405DBGWBFULL => C405DBGWBFULL,
      C405DBGWBIAR => C405DBGWBIAR,
      DBGC405DEBUGHALT => DBGC405DEBUGHALT,
      DBGC405DEBUGHALTNEG => DBGC405DEBUGHALTNEG,
      DBGC405EXTBUSHOLDACK => DBGC405EXTBUSHOLDACK,
      DBGC405UNCONDDEBUGEVENT => DBGC405UNCONDDEBUGEVENT,
      C405DBGLOADDATAONAPUDBUS => C405DBGLOADDATAONAPUDBUS,
      C405TRCCYCLE => C405TRCCYCLE,
      C405TRCEVENEXECUTIONSTATUS => C405TRCEVENEXECUTIONSTATUS,
      C405TRCODDEXECUTIONSTATUS => C405TRCODDEXECUTIONSTATUS,
      C405TRCTRACESTATUS => C405TRCTRACESTATUS,
      C405TRCTRIGGEREVENTOUT => C405TRCTRIGGEREVENTOUT,
      C405TRCTRIGGEREVENTTYPE => C405TRCTRIGGEREVENTTYPE,
      TRCC405TRACEDISABLE => TRCC405TRACEDISABLE,
      TRCC405TRIGGEREVENTIN => TRCC405TRIGGEREVENTIN
    );

end architecture STRUCTURE;

