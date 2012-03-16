------------------------------------------------------------------------------
-- user_logic.vhd - entity/architecture pair
------------------------------------------------------------------------------
--
-- ***************************************************************************
-- ** Copyright (c) 1995-2009 Xilinx, Inc.  All rights reserved.            **
-- **                                                                       **
-- ** Xilinx, Inc.                                                          **
-- ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
-- ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
-- ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
-- ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
-- ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
-- ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
-- ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
-- ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
-- ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
-- ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
-- ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
-- ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
-- ** FOR A PARTICULAR PURPOSE.                                             **
-- **                                                                       **
-- ***************************************************************************
--
------------------------------------------------------------------------------
-- Filename:          user_logic.vhd
-- Version:           1.00.a
-- Description:       User logic.
-- Date:              Mon Feb  8 12:50:50 2010 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Naming Conventions:
--   active low signals:                    "*_n"
--   clock signals:                         "clk", "clk_div#", "clk_#x"
--   reset signals:                         "rst", "rst_n"
--   generics:                              "C_*"
--   user defined types:                    "*_TYPE"
--   state machine next state:              "*_ns"
--   state machine current state:           "*_cs"
--   combinatorial signals:                 "*_com"
--   pipelined or register delay signals:   "*_d#"
--   counter signals:                       "*cnt*"
--   clock enable signals:                  "*_ce"
--   internal version of output port:       "*_i"
--   device pins:                           "*_pin"
--   ports:                                 "- Names begin with Uppercase"
--   processes:                             "*_PROCESS"
--   component instantiations:              "<ENTITY_>I_<#|FUNC>"
------------------------------------------------------------------------------

-- DO NOT EDIT BELOW THIS LINE --------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;
--use proc_common_v3_00_a.srl_fifo_f;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--   C_MST_AWIDTH                 -- Master interface address bus width
--   C_MST_DWIDTH                 -- Master interface data bus width
--   C_NUM_REG                    -- Number of software accessible registers
--   C_RDFIFO_DEPTH               -- Read FIFO depth
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Reset                 -- Bus to IP reset
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
--   IP2Bus_MstRd_Req             -- IP to Bus master read request
--   IP2Bus_MstWr_Req             -- IP to Bus master write request
--   IP2Bus_Mst_Addr              -- IP to Bus master address bus
--   IP2Bus_Mst_BE                -- IP to Bus master byte enables
--   IP2Bus_Mst_Length            -- IP to Bus master transfer length
--   IP2Bus_Mst_Type              -- IP to Bus master transfer type
--   IP2Bus_Mst_Lock              -- IP to Bus master lock
--   IP2Bus_Mst_Reset             -- IP to Bus master reset
--   Bus2IP_Mst_CmdAck            -- Bus to IP master command acknowledgement
--   Bus2IP_Mst_Cmplt             -- Bus to IP master transfer completion
--   Bus2IP_Mst_Error             -- Bus to IP master error response
--   Bus2IP_Mst_Rearbitrate       -- Bus to IP master re-arbitrate
--   Bus2IP_Mst_Cmd_Timeout       -- Bus to IP master command timeout
--   Bus2IP_MstRd_d               -- Bus to IP master read data bus
--   Bus2IP_MstRd_rem             -- Bus to IP master read remainder
--   Bus2IP_MstRd_sof_n           -- Bus to IP master read start of frame
--   Bus2IP_MstRd_eof_n           -- Bus to IP master read end of frame
--   Bus2IP_MstRd_src_rdy_n       -- Bus to IP master read source ready
--   Bus2IP_MstRd_src_dsc_n       -- Bus to IP master read source discontinue
--   IP2Bus_MstRd_dst_rdy_n       -- IP to Bus master read destination ready
--   IP2Bus_MstRd_dst_dsc_n       -- IP to Bus master read destination discontinue
--   IP2Bus_MstWr_d               -- IP to Bus master write data bus
--   IP2Bus_MstWr_rem             -- IP to Bus master write remainder
--   IP2Bus_MstWr_sof_n           -- IP to Bus master write start of frame
--   IP2Bus_MstWr_eof_n           -- IP to Bus master write end of frame
--   IP2Bus_MstWr_src_rdy_n       -- IP to Bus master write source ready
--   IP2Bus_MstWr_src_dsc_n       -- IP to Bus master write source discontinue
--   Bus2IP_MstWr_dst_rdy_n       -- Bus to IP master write destination ready
--   Bus2IP_MstWr_dst_dsc_n       -- Bus to IP master write destination discontinue
--   IP2RFIFO_WrReq               -- IP to RFIFO : IP write request
--   IP2RFIFO_Data                -- IP to RFIFO : IP write data bus
--   IP2RFIFO_WrMark              -- IP to RFIFO : mark beginning of packet being written
--   IP2RFIFO_WrRelease           -- IP to RFIFO : return RFIFO to normal FIFO operation
--   IP2RFIFO_WrRestore           -- IP to RFIFO : restore the RFIFO to the last packet mark
--   RFIFO2IP_WrAck               -- RFIFO to IP : RFIFO write acknowledge
--   RFIFO2IP_AlmostFull          -- RFIFO to IP : RFIFO almost full
--   RFIFO2IP_Full                -- RFIFO to IP : RFIFO full
--   RFIFO2IP_Vacancy             -- RFIFO to IP : RFIFO vacancy
------------------------------------------------------------------------------

entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    C_BASEADDR                          : std_logic_vector      := X"00000000";
    C_IMAGE_WIDTH                       : integer               := 640;
    C_IMAGE_HEIGHT                      : integer               := 480;
    C_R_DATA_WIDTH                      : integer               := 5;
    C_G_DATA_WIDTH                      : integer               := 6;
    C_B_DATA_WIDTH                      : integer               := 5;
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_SLV_DWIDTH                   : integer              := 32;
    C_MST_AWIDTH                   : integer              := 32;
    C_MST_DWIDTH                   : integer              := 32;
    C_NUM_REG                      : integer              := 12;
    C_RDFIFO_DEPTH                 : integer              := 512
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    cam_data                            : in std_logic_vector(0 to 7);
    cam_frame_valid                     : in std_logic;
    cam_line_valid                      : in std_logic;
    cam_pix_clk                         : in std_logic;
    cam_sdata_I                         : in std_logic;
    cam_sdata_O                         : out std_logic;
    cam_sdata_T                         : out std_logic;
    cam_sclk                            : out std_logic;
    cam_reset_n                         : out std_logic;
    Interrupt                           : out std_logic;
    -- ADD USER PORTS ABOVE THIS LINE ------------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  std_logic;
    Bus2IP_Reset                   : in  std_logic;
    Bus2IP_Data                    : in  std_logic_vector(0 to C_SLV_DWIDTH-1);
    Bus2IP_BE                      : in  std_logic_vector(0 to C_SLV_DWIDTH/8-1);
    Bus2IP_RdCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    Bus2IP_WrCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    IP2Bus_Data                    : out std_logic_vector(0 to C_SLV_DWIDTH-1);
    IP2Bus_RdAck                   : out std_logic;
    IP2Bus_WrAck                   : out std_logic;
    IP2Bus_Error                   : out std_logic;
    IP2Bus_MstRd_Req               : out std_logic;
    IP2Bus_MstWr_Req               : out std_logic;
    IP2Bus_Mst_Addr                : out std_logic_vector(0 to C_MST_AWIDTH-1);
    IP2Bus_Mst_BE                  : out std_logic_vector(0 to C_MST_DWIDTH/8-1);
    IP2Bus_Mst_Length              : out std_logic_vector(0 to 11);
    IP2Bus_Mst_Type                : out std_logic;
    IP2Bus_Mst_Lock                : out std_logic;
    IP2Bus_Mst_Reset               : out std_logic;
    Bus2IP_Mst_CmdAck              : in  std_logic;
    Bus2IP_Mst_Cmplt               : in  std_logic;
    Bus2IP_Mst_Error               : in  std_logic;
    Bus2IP_Mst_Rearbitrate         : in  std_logic;
    Bus2IP_Mst_Cmd_Timeout         : in  std_logic;
    Bus2IP_MstRd_d                 : in  std_logic_vector(0 to C_MST_DWIDTH-1);
    Bus2IP_MstRd_rem               : in  std_logic_vector(0 to C_MST_DWIDTH/8-1);
    Bus2IP_MstRd_sof_n             : in  std_logic;
    Bus2IP_MstRd_eof_n             : in  std_logic;
    Bus2IP_MstRd_src_rdy_n         : in  std_logic;
    Bus2IP_MstRd_src_dsc_n         : in  std_logic;
    IP2Bus_MstRd_dst_rdy_n         : out std_logic;
    IP2Bus_MstRd_dst_dsc_n         : out std_logic;
    IP2Bus_MstWr_d                 : out std_logic_vector(0 to C_MST_DWIDTH-1);
    IP2Bus_MstWr_rem               : out std_logic_vector(0 to C_MST_DWIDTH/8-1);
    IP2Bus_MstWr_sof_n             : out std_logic;
    IP2Bus_MstWr_eof_n             : out std_logic;
    IP2Bus_MstWr_src_rdy_n         : out std_logic;
    IP2Bus_MstWr_src_dsc_n         : out std_logic;
    Bus2IP_MstWr_dst_rdy_n         : in  std_logic;
    Bus2IP_MstWr_dst_dsc_n         : in  std_logic;
    IP2RFIFO_WrReq                 : out std_logic;
    IP2RFIFO_Data                  : out std_logic_vector(0 to C_SLV_DWIDTH-1);
    IP2RFIFO_WrMark                : out std_logic;
    IP2RFIFO_WrRelease             : out std_logic;
    IP2RFIFO_WrRestore             : out std_logic;
    RFIFO2IP_WrAck                 : in  std_logic;
    RFIFO2IP_AlmostFull            : in  std_logic;
    RFIFO2IP_Full                  : in  std_logic;
    RFIFO2IP_Vacancy               : in  std_logic_vector(0 to log2(C_RDFIFO_DEPTH))
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute SIGIS : string;
  attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
  attribute SIGIS of Bus2IP_Reset  : signal is "RST";
  attribute SIGIS of IP2Bus_Mst_Reset: signal is "RST";

end entity user_logic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic is

  --USER signal declarations added here, as needed for user logic

-------------------------------------------------------------------------------
-- ----------------------------------------------------------------------------
-- Section 1: Component Declarations
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
  -- For grabbing frames from the camera

  component frame_sync_hs
    generic (
      C_POS_EDGE_PIX_CLK : boolean;
      C_DONE_STICKY      : boolean;
      C_DATA_WIDTH       : integer);
    port (
      rst             : in  std_logic;
      cam_data        : in  std_logic_vector(C_DATA_WIDTH-1 downto 0);
      cam_frame_valid : in  std_logic;
      cam_line_valid  : in  std_logic;
      cam_pix_clk     : in  std_logic;
      clk             : in  std_logic;
      cam_bytes       : in  std_logic_vector(1 downto 0);
      start_capture   : in  std_logic;
      capture_done    : out std_logic;
      dout            : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
      dout_valid      : out std_logic;
      dout_rd_en      : in  std_logic;
      new_frame       : out std_logic;
      frame_active    : out std_logic);
  end component;

  -- For communicating over the camera's serial interface
  component cam_ser
    generic (
      C_CLK_DIV : integer := 112);
    port (
      clk               : in    std_logic;
      reset             : in    std_logic;
      wr_en             : in    std_logic;
      rd_en             : in    std_logic;
      dev_addr          : in    std_logic_vector(0 to 7);
      reg_addr          : in    std_logic_vector(0 to 7);
      data_in           : in    std_logic_vector(0 to 15);
      data_out          : out   std_logic_vector(0 to 15);
      ready             : out   std_logic;
      sdata_I           : in    std_logic;
      sdata_O           : out   std_logic;
      sdata_T           : out   std_logic;
      sclk              : out   std_logic);
  end component;

  component frame_fifo
    port (
      din              : IN  std_logic_VECTOR(15 downto 0);
      prog_full_thresh : IN  std_logic_VECTOR(9 downto 0);
      rd_clk           : IN  std_logic;
      rd_en            : IN  std_logic;
      rst              : IN  std_logic;
      wr_clk           : IN  std_logic;
      wr_en            : IN  std_logic;
      dout             : OUT std_logic_VECTOR(63 downto 0);
      empty            : OUT std_logic;
      full             : OUT std_logic;
      prog_full        : OUT std_logic);
  end component;

  component size_buffer
    generic (
      C_IN_WIDTH        : integer;
      C_OUT_WIDTH       : integer);
    port (
      clk               : in  std_logic;
      reset             : in  std_logic;
      in_valid          : in  std_logic;
      data_in           : in  std_logic_vector(C_IN_WIDTH-1 downto 0);
      out_valid         : out std_logic;
      data_out          : out std_logic_vector(C_OUT_WIDTH-1 downto 0));
  end component;

-------------------------------------------------------------------------------
-- Components for the MT9V024 (Needs bayer demosaicing).  If these are not
-- needed, comment them out and redirect pixel to the size_buffer and output of
-- size_buffer to the frame fifo
-------------------------------------------------------------------------------

  
  component camera_processor
    generic (
      C_IMAGE_WIDTH    : integer;
      C_IMAGE_HEIGHT   : integer;
      C_CAM_DATA_WIDTH : integer;
      C_R_DATA_WIDTH   : integer;
      C_G_DATA_WIDTH   : integer;
      C_B_DATA_WIDTH   : integer;
      C_DEMOSAIC_TYPE  : string;
      C_INSERT_SKIP    : integer;
      C_CC_A11         : real;
      C_CC_A12         : real;
      C_CC_A13         : real;
      C_CC_A21         : real;
      C_CC_A22         : real;
      C_CC_A23         : real;
      C_CC_A31         : real;
      C_CC_A32         : real;
      C_CC_A33         : real);
    port (
      clk       : in  std_logic;
      rst       : in  std_logic;
      bayer_pos : in  std_logic_vector(1 downto 0);
      cc_bypass : in  std_logic;
      new_frame : in  std_logic;
      din       : in  std_logic_vector(C_CAM_DATA_WIDTH-1 downto 0);
      wr_en     : in  std_logic;
      r         : out std_logic_vector(C_R_DATA_WIDTH-1 downto 0);
      g         : out std_logic_vector(C_G_DATA_WIDTH-1 downto 0);
      b         : out std_logic_vector(C_B_DATA_WIDTH-1 downto 0);
      rgb_valid : out std_logic);
  end component;

  component rgb2hsv  
  port (
    clk         : in  std_logic;
    reset       : in  std_logic;

    R           : in  std_logic_vector(7 downto 0); 
    G           : in  std_logic_vector(7 downto 0);  
    B           : in  std_logic_vector(7 downto 0);  
    en          : in  std_logic;

    H           : out std_logic_vector(8 downto 0);
    S           : out std_logic_vector(7 downto 0);
    V           : out std_logic_vector(7 downto 0);
    valid       : out std_logic);

  end component;
-------------------------------------------------------------------------------
-- ----------------------------------------------------------------------------
-- Section 2: Signal Declarations
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

  signal status                         : std_logic_vector(10 downto 0);

  signal addr_sel                       : std_logic_vector(3 downto 0);
  
  signal cam_ser_we                     : std_logic;
  signal cam_ser_re                     : std_logic;
  signal cam_ser_addr                   : std_logic_vector(7 downto 0);
  signal cam_ser_data_in                : std_logic_vector(15 downto 0);
  signal cam_ser_data_out               : std_logic_vector(15 downto 0);
  signal cam_ser_ready                  : std_logic;

  
  -- State Machine control
  type   fg_state_type is               ( IDLE,
                                          START_OF_ORG_FRAME,
                                          START_OF_SEG_FRAME,                                          
                                          BURSTING_ORG,
                                          BURSTING_SEG,                                          
                                          END_OF_ORG_FRAME,
                                          END_OF_SEG_FRAME                                          
                                          );
  signal current_state, next_state      : fg_state_type;
  type fg_ctrl_state_type is ( CTRL_IDLE, CTRL_WRITE_REQUEST, CTRL_TRANFER_IN_PROGRESS );
  signal ctrl_current_state, ctrl_next_state : fg_ctrl_state_type;
  signal burst_state                    : std_logic_vector(1 downto 0);
  --signal burst_size : std_logic_vector(7 downto 0);
  
  signal int : std_logic;
  -- Component Instantiation
  signal start_capture                  : std_logic;
  signal capture_done                   : std_logic;
  signal core_soft_reset 	        : std_logic;


  signal pixel                          : std_logic_vector(7 downto 0);
  signal pixel_valid                    : std_logic; 
  
  signal pix_word                       : std_logic_vector(15 downto 0);
  signal pix_word_valid                 : std_logic;

  signal pixel_reg : std_logic_vector(7 downto 0);
  signal pixel_valid_reg : std_logic;
  
  signal seg_word                       : std_logic_vector(7 downto 0); 
  signal seg_word_valid                 : std_logic;

  signal org_frm_mem_addr_wr_en : std_logic;
  signal seg_frm_mem_addr_wr_en : std_logic;
  signal org_frm_mem_addr : std_logic_vector(31 downto 0);
  signal seg_frm_mem_addr : std_logic_vector(31 downto 0);

  signal bursting_org_frame : std_logic;
  signal bursting_seg_frame : std_logic;

  signal burst_count : unsigned(0 to 7);
  signal next_burst_count : unsigned(0 to 7);
  
  signal org_fifo_prog_full             : std_logic_vector(9 downto 0);
  signal seg_fifo_prog_full             : std_logic_vector(9 downto 0);
  
  signal org_frame_fifo_out             : std_logic_vector(63 downto 0);
  signal seg_frame_fifo_out             : std_logic_vector(63 downto 0);

  signal org_frame_fifo_wr_en           : std_logic;
  signal seg_frame_fifo_wr_en           : std_logic;
  
  signal org_frame_fifo_rd_en           : std_logic;  
  signal seg_frame_fifo_rd_en           : std_logic;
  
  signal org_frame_fifo_empty           : std_logic;
  signal seg_frame_fifo_empty           : std_logic;

  signal org_frame_fifo_ready           : std_logic;
  signal seg_frame_fifo_ready           : std_logic;

  signal r         : std_logic_vector(C_R_DATA_WIDTH-1 downto 0);
  signal g         : std_logic_vector(C_G_DATA_WIDTH-1 downto 0);
  signal b         : std_logic_vector(C_B_DATA_WIDTH-1 downto 0);
  signal rgb_valid : std_logic;
  signal new_frame : std_logic;
  
  signal hsv_r : std_logic_vector(7 downto 0);
  signal hsv_g : std_logic_vector(7 downto 0);
  signal hsv_b : std_logic_vector(7 downto 0);
  
  signal h	:	std_logic_vector(8 downto 0);
  signal s  :	std_logic_vector(7 downto 0);
  signal v  :	std_logic_vector(7 downto 0);
  signal hsv_valid : std_logic;

  signal size_buffer_valid : std_logic;
  signal size_buffer_dout : std_logic_vector(15 downto 0);
  
  ------------------------------------------
  -- Signals for user logic slave model s/w accessible register example
  ------------------------------------------
  signal sr0_status                             : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal sr1_cam_ser_ready                      : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal sr2_cam_ser_data_out                   : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal sr3_burst_size                          : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal sr4_save_options                       : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal sr5_org_frm_mem_addr                   : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal sr6_seg_frm_mem_addr                   : std_logic_vector(0 to C_SLV_DWIDTH-1);
  -- these slave registers are currently unused, but left for easy expansion
  signal sr7                       : std_logic_vector(0 to C_SLV_DWIDTH-1);  --unused
  signal sr8                       : std_logic_vector(0 to C_SLV_DWIDTH-1);  --unused
  signal sr9                       : std_logic_vector(0 to C_SLV_DWIDTH-1);  --unused
  -----------------------------------------------------------------------------
  signal slv_reg_write_sel              : std_logic_vector(0 to 9);
  signal slv_reg_read_sel               : std_logic_vector(0 to 9);
  signal slv_ip2bus_data                : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal slv_read_ack                   : std_logic;
  signal slv_write_ack                  : std_logic;

  ------------------------------------------
  -- Signals for user logic master model example
  ------------------------------------------
  -- signals for master model control/status registers write/read
  signal mst_ip2bus_data                : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal mst_reg_write_req              : std_logic;
  signal mst_reg_read_req               : std_logic;
  signal mst_reg_write_sel              : std_logic_vector(0 to 3);
  signal mst_reg_read_sel               : std_logic_vector(0 to 3);
  signal mst_write_ack                  : std_logic;
  signal mst_read_ack                   : std_logic;
  -- signals for master model control/status registers
  type BYTE_REG_TYPE is array(0 to 15) of std_logic_vector(0 to 7);
  signal mst_reg                        : BYTE_REG_TYPE;
  signal mst_byte_we                    : std_logic_vector(0 to 15);
  signal mst_cntl_rd_req                : std_logic;
  signal mst_cntl_wr_req                : std_logic;
  signal mst_cntl_bus_lock              : std_logic;
  signal mst_cntl_burst                 : std_logic;
  signal mst_ip2bus_addr                : std_logic_vector(0 to C_MST_AWIDTH-1);
  signal mst_xfer_length                : std_logic_vector(0 to 11);
  signal mst_ip2bus_be                  : std_logic_vector(0 to 15);
  signal mst_go                         : std_logic;
  -- signals for master model command interface state machine
  type CMD_CNTL_SM_TYPE is (CMD_IDLE, CMD_RUN, CMD_WAIT_FOR_DATA, CMD_DONE);
  signal mst_cmd_sm_state               : CMD_CNTL_SM_TYPE;
  signal mst_cmd_sm_set_done            : std_logic;
  signal mst_cmd_sm_set_error           : std_logic;
  signal mst_cmd_sm_set_timeout         : std_logic;
  signal mst_cmd_sm_busy                : std_logic;
  signal mst_cmd_sm_clr_go              : std_logic;
  signal mst_cmd_sm_rd_req              : std_logic;
  signal mst_cmd_sm_wr_req              : std_logic;
  signal mst_cmd_sm_reset               : std_logic;
  signal mst_cmd_sm_bus_lock            : std_logic;
  signal mst_cmd_sm_ip2bus_addr         : std_logic_vector(0 to C_MST_AWIDTH-1);
  signal mst_cmd_sm_ip2bus_be           : std_logic_vector(0 to C_MST_DWIDTH/8-1);
  signal mst_cmd_sm_xfer_type           : std_logic;
  signal mst_cmd_sm_xfer_length         : std_logic_vector(0 to 11);
  signal mst_cmd_sm_start_rd_llink      : std_logic;
  signal mst_cmd_sm_start_wr_llink      : std_logic;
  -- signals for master model read locallink interface state machine
  type RD_LLINK_SM_TYPE is (LLRD_IDLE, LLRD_GO);
  signal mst_llrd_sm_state              : RD_LLINK_SM_TYPE;
  signal mst_llrd_sm_dst_rdy            : std_logic;
  -- signals for master model write locallink interface state machine
  type WR_LLINK_SM_TYPE is (LLWR_IDLE, LLWR_SNGL_INIT, LLWR_SNGL, LLWR_BRST_INIT, LLWR_BRST, LLWR_BRST_LAST_BEAT);
  signal mst_llwr_sm_state              : WR_LLINK_SM_TYPE;
  signal mst_llwr_sm_src_rdy            : std_logic;
  signal mst_llwr_sm_sof                : std_logic;
  signal mst_llwr_sm_eof                : std_logic;
  signal mst_llwr_byte_cnt              : integer;
  signal mst_fifo_valid_write_xfer      : std_logic;
  signal mst_fifo_valid_read_xfer       : std_logic;

begin

  --USER logic implementation added here
  -----------------------------------------------------------------------------
  -- --------------------------------------------------------------------------
  -- Section 3: Combinational signal routing (non-process)
  -----------------------------------------------------------------------------
  -----------------------------------------------------------------------------
  ----------------------------------------------------------------------------
  Interrupt <= capture_done and org_frame_fifo_empty;-- and seg_frame_fifo_empty;
  int <= capture_done and org_frame_fifo_empty;
  status    <= bursting_org_frame
               & org_frame_fifo_rd_en & Bus2IP_Mst_Cmplt
               & sr4_save_options(31) & Bus2IP_MstWr_dst_rdy_n
               & int                  & capture_done
               & org_frame_fifo_ready & org_frame_fifo_empty
               & seg_frame_fifo_ready & seg_frame_fifo_empty;
  
  ------------------------------------------
  -- Example code to read/write user logic slave model s/w accessible registers
  -- 
  -- Note:
  -- The example code presented here is to show you one way of reading/writing
  -- software accessible registers implemented in the user logic slave model.
  -- Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  -- to one software accessible register by the top level template. For example,
  -- if you have four 32 bit software accessible registers in the user logic,
  -- you are basically operating on the following memory mapped registers:
  -- 
  --    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  --                     "1000"   C_BASEADDR + 0x0
  --                     "0100"   C_BASEADDR + 0x4
  --                     "0010"   C_BASEADDR + 0x8
  --                     "0001"   C_BASEADDR + 0xC
  -- 
  ------------------------------------------
  slv_reg_write_sel <= Bus2IP_WrCE(0 to 9);
  slv_reg_read_sel  <= Bus2IP_RdCE(0 to 9);
  slv_write_ack     <= Bus2IP_WrCE(0) or Bus2IP_WrCE(1) or Bus2IP_WrCE(2) or Bus2IP_WrCE(3) or Bus2IP_WrCE(4) or Bus2IP_WrCE(5) or Bus2IP_WrCE(6) or Bus2IP_WrCE(7) or Bus2IP_WrCE(8) or Bus2IP_WrCE(9);
  slv_read_ack      <= Bus2IP_RdCE(0) or Bus2IP_RdCE(1) or Bus2IP_RdCE(2) or Bus2IP_RdCE(3) or Bus2IP_RdCE(4) or Bus2IP_RdCE(5) or Bus2IP_RdCE(6) or Bus2IP_RdCE(7) or Bus2IP_RdCE(8) or Bus2IP_RdCE(9);

  -- implement slave model software accessible register(s)
  SLAVE_REG_WRITE_PROC : process( Bus2IP_Clk ) is
  begin
    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      -- defaults.  These values are set in this process, but should only stay set for one clock cycle
		-- and more importantly, should default to 0 if they are not being accessed
      cam_ser_we <= '0';
      cam_ser_re <= '0';
      cam_ser_addr <= (others => '0');
      cam_ser_data_in <= (others => '0');
      start_capture <= '0';
      core_soft_reset <= '0';
      cam_reset_n <= '1';
      org_frm_mem_addr_wr_en <= '0';
      seg_frm_mem_addr_wr_en <= '0';
      if Bus2IP_Reset = '1' then
        sr0_status <= (others => '0');
        sr1_cam_ser_ready <= (others => '0');
        sr2_cam_ser_data_out <= (others => '0');
        sr3_burst_size <= "00000000000000000000010000000000";   --default to
                                                                --1024 (bytes)
        sr4_save_options <= (others => '0');
        sr5_org_frm_mem_addr <= (others => '0');
        sr6_seg_frm_mem_addr <= (others => '0');
        sr7 <= (others => '0');
        sr8 <= (others => '0');
        sr9 <= (others => '0');
        core_soft_reset <= '1';
        cam_reset_n <= '0';
      else
        -- these are actually for the read process, but because my signals are
        -- coming into registers, they need to be clocked and the read process
        -- is not a clocked process.  Signals will be clocked here, but used on
        -- the read process
        sr0_status <= std_logic_vector(resize(unsigned(status), 32));
        sr1_cam_ser_ready <= "0000000000000000000000000000000" & cam_ser_ready;
        sr2_cam_ser_data_out <= "0000000000000000" & cam_ser_data_out;
        case slv_reg_write_sel is
          -- Status Register
          when "1000000000" =>          -- status reg is read only, if a write
                                        -- happens here
            if Bus2IP_Data(31) = '1' then -- bit 0 means start frame capture,
                                          -- write 0x01 in software here to
                                          -- start capture
              start_capture <= '1';
            elsif Bus2IP_Data(30) = '1' then --bit 1 means do a sensor reset,
                                             --write 0x02 in software here to
                                             --do a core soft reset
              core_soft_reset <= '1';
            elsif Bus2IP_Data(29) = '1' then   --bit 2 means do a
                                                   --chip-level reset of the
                                                   --image sensor, write a 0x04
                                                   --in software here to do a
                                                   --chip-level reset
              cam_reset_n <= '0';       --low-asserted
            end if;
          -- CamSer Ready/Write
          when "0100000000" =>          -- cam_ser_ready is read only, if a
                                        -- write happens here, it means write
                                        -- to the camera register
            cam_ser_we          <= '1';
            cam_ser_addr        <= Bus2IP_Data(8 to 15);
            cam_ser_data_in     <= Bus2IP_Data(16 to 31);
          -- CamSer Data Out/Read
          when "0010000000" =>          -- cam_ser_data_out is read only, if a
                                        -- write happens here, it means read
                                        -- from register by writing into the register
            cam_ser_re          <= '1';
            cam_ser_addr        <= Bus2IP_Data(24 to 31);
          -- Burst Size Register
          when "0001000000" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                sr3_burst_size(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
              end if;
            end loop;
          -- Save Options Register
          when "0000100000" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                sr4_save_options(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
              end if;
            end loop;
          -- Original Frame Memory Address Register
          when "0000010000" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                sr5_org_frm_mem_addr(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
              end if;
            end loop;
            org_frm_mem_addr_wr_en <= '1';
          -- Segmented Frame Memory Address Register
          when "0000001000" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                sr6_seg_frm_mem_addr(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
              end if;
            end loop;
            seg_frm_mem_addr_wr_en <= '1';
          -- UNUSED
          when "0000000100" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                sr7(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
              end if;
            end loop;
          -- UNUSED
          when "0000000010" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                sr8(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
              end if;
            end loop;
          -- UNUSED
          when "0000000001" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                sr9(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
              end if;
            end loop;
          when others => null;
        end case;
      end if;
    end if;

  end process SLAVE_REG_WRITE_PROC;

  -- implement slave model software accessible register(s) read mux
  SLAVE_REG_READ_PROC : process( slv_reg_read_sel, sr0_status, sr1_cam_ser_ready, sr2_cam_ser_data_out, sr3_burst_size, sr4_save_options, sr5_org_frm_mem_addr, sr6_seg_frm_mem_addr, sr7, sr8, sr9 ) is
  begin

    case slv_reg_read_sel is
      -- Status Register
      when "1000000000" => slv_ip2bus_data <= sr0_status;
      -- CamSer Ready/Write Register
      when "0100000000" => slv_ip2bus_data <= sr1_cam_ser_ready;
      -- CamSer Data Out/Read Request Register
      when "0010000000" => slv_ip2bus_data <= sr2_cam_ser_data_out;
      -- Burst Size Register
      when "0001000000" => slv_ip2bus_data <= sr3_burst_size;
      -- Save Options Register
      when "0000100000" => slv_ip2bus_data <= sr4_save_options;
      -- Original frame memory address register
      when "0000010000" => slv_ip2bus_data <= org_frm_mem_addr;  --report back
                                                                 --the actual
                                                                 --current value
      -- Segmented frame memory address register
      when "0000001000" => slv_ip2bus_data <= seg_frm_mem_addr;
      -- UNUSED
      when "0000000100" => slv_ip2bus_data <= sr7;
      -- UNUSED
      when "0000000010" => slv_ip2bus_data <= sr8;
      -- UNUSED
      when "0000000001" => slv_ip2bus_data <= sr9;
      when others => slv_ip2bus_data <= (others => '0');
    end case;

  end process SLAVE_REG_READ_PROC;

-------------------------------------------------------------------------------
-- Store the memory address for the original frame in a register
-- The memory address is written to PLB slave register 5, but I need a
-- different location where I can keep the incrementing value as I perform
-- bursts.  Plus I need to trigger a write enable, so this is here in a new
-- register, org_frm_mem_addr
-------------------------------------------------------------------------------
  process(Bus2IP_Clk,Bus2IP_Reset,org_frm_mem_addr_wr_en,bursting_org_frame,Bus2IP_Mst_Cmplt)
  begin
    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Reset = '1' then
        org_frm_mem_addr <= (others => '0');
      else
        if org_frm_mem_addr_wr_en = '1' then
          org_frm_mem_addr <= sr5_org_frm_mem_addr;
        else
          if bursting_org_frame = '1' and Bus2IP_Mst_Cmplt = '1' then
            org_frm_mem_addr <= std_logic_vector(unsigned(org_frm_mem_addr) + unsigned(sr3_burst_size(21 to 31)));
          end if;
        end if;
      end if;
    end if;
  end process; 
  
-------------------------------------------------------------------------------
-- Store the memory address for the segmented frame in a register
-------------------------------------------------------------------------------
  process(Bus2IP_Clk,Bus2IP_Reset,bursting_seg_frame,Bus2IP_Mst_Cmplt)
  begin
    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Reset = '1' then
        seg_frm_mem_addr <= (others => '0');
      else
        if seg_frm_mem_addr_wr_en = '1' then
          seg_frm_mem_addr <= sr6_seg_frm_mem_addr;
        else
          if bursting_seg_frame = '1' and Bus2IP_Mst_Cmplt = '1' then
            seg_frm_mem_addr <= std_logic_vector(unsigned(seg_frm_mem_addr) + unsigned(sr3_burst_size(21 to 31)));
          end if;
        end if;
      end if;
    end if;
  end process; 


 ------------------------------------------------------------------------------
 -- ---------------------------------------------------------------------------
 -- Section 7: PLB Burst State Machine
 -- 
 -- STATES:
 --  1: Idle   - This state waits until the dma_session_active is 1
 --  2: Assert_OFrame - This state waits until the Bus2IP_MstLastAck is 1
 --  3: Assert_HSVFrame
 --  4: Assert_SegFrame
 --  
 --
 --
 --
 -- ---------------------------------------------------------------------------
 ------------------------------------------------------------------------------

  -- user logic master command interface assignments
  IP2Bus_MstRd_Req  <= mst_cmd_sm_rd_req;
  IP2Bus_MstWr_Req  <= mst_cmd_sm_wr_req;
  IP2Bus_Mst_Addr   <= mst_cmd_sm_ip2bus_addr;
  IP2Bus_Mst_BE     <= mst_cmd_sm_ip2bus_be;
  IP2Bus_Mst_Type   <= mst_cmd_sm_xfer_type;
  IP2Bus_Mst_Length <= mst_cmd_sm_xfer_length;
  IP2Bus_Mst_Lock   <= mst_cmd_sm_bus_lock;
  IP2Bus_Mst_Reset  <= mst_cmd_sm_reset;

  -----------------------------------------------------------------------------
  -- I'm going to manipulate go signals entirely in hardware
  -----------------------------------------------------------------------------
  mst_go <= org_frame_fifo_ready and sr4_save_options(31);
  mst_cntl_burst <= '1';                --always doing burst
  mst_cntl_rd_req <= '0';
  mst_cntl_wr_req <= '1';
  mst_xfer_length <= sr3_burst_size(20 to 31);
  --implement master command interface state machine
  MASTER_CMD_SM_PROC : process( Bus2IP_Clk ) is
  begin

    if ( Bus2IP_Clk'event and Bus2IP_Clk = '1' ) then
      if ( Bus2IP_Reset = '1' ) then

        -- reset condition
        mst_cmd_sm_state          <= CMD_IDLE;
        mst_cmd_sm_clr_go         <= '0';
        mst_cmd_sm_rd_req         <= '0';
        mst_cmd_sm_wr_req         <= '0';
        mst_cmd_sm_bus_lock       <= '0';
        mst_cmd_sm_reset          <= '0';
        mst_cmd_sm_ip2bus_addr    <= (others => '0');
        mst_cmd_sm_ip2bus_be      <= (others => '0');
        mst_cmd_sm_xfer_type      <= '0';
        mst_cmd_sm_xfer_length    <= (others => '0');
        mst_cmd_sm_set_done       <= '0';
        mst_cmd_sm_set_error      <= '0';
        mst_cmd_sm_set_timeout    <= '0';
        mst_cmd_sm_busy           <= '0';
        mst_cmd_sm_start_rd_llink <= '0';
        mst_cmd_sm_start_wr_llink <= '0';

      else

        -- default condition
        mst_cmd_sm_clr_go         <= '0';
        mst_cmd_sm_rd_req         <= '0';
        mst_cmd_sm_wr_req         <= '0';
        mst_cmd_sm_bus_lock       <= '0';
        mst_cmd_sm_reset          <= '0';
        mst_cmd_sm_ip2bus_addr    <= (others => '0');
        mst_cmd_sm_ip2bus_be      <= (others => '0');
        mst_cmd_sm_xfer_type      <= '0';
        mst_cmd_sm_xfer_length    <= (others => '0');
        mst_cmd_sm_set_done       <= '0';
        mst_cmd_sm_set_error      <= '0';
        mst_cmd_sm_set_timeout    <= '0';
        mst_cmd_sm_busy           <= '1';
        mst_cmd_sm_start_rd_llink <= '0';
        mst_cmd_sm_start_wr_llink <= '0';
        bursting_org_frame <= '0';

        -- state transition
        case mst_cmd_sm_state is

          when CMD_IDLE =>
            if ( mst_go = '1' ) then
              mst_cmd_sm_state  <= CMD_RUN;
              mst_cmd_sm_clr_go <= '1';
              mst_cmd_sm_start_wr_llink <= '1';
            else
              mst_cmd_sm_state  <= CMD_IDLE;
              mst_cmd_sm_busy   <= '0';
            end if;

          when CMD_RUN =>
            if ( Bus2IP_Mst_CmdAck = '1' and Bus2IP_Mst_Cmplt = '0' ) then
              mst_cmd_sm_state <= CMD_WAIT_FOR_DATA;
            elsif ( Bus2IP_Mst_Cmplt = '1' ) then
              mst_cmd_sm_state <= CMD_DONE;
              if ( Bus2IP_Mst_Cmd_Timeout = '1' ) then
                -- PLB address phase timeout
                mst_cmd_sm_set_error   <= '1';
                mst_cmd_sm_set_timeout <= '1';
              elsif ( Bus2IP_Mst_Error = '1' ) then
                -- PLB data transfer error
                mst_cmd_sm_set_error   <= '1';
              end if;
            else
              mst_cmd_sm_state       <= CMD_RUN;
              mst_cmd_sm_rd_req      <= mst_cntl_rd_req;
              mst_cmd_sm_wr_req      <= mst_cntl_wr_req;
              mst_cmd_sm_ip2bus_addr <= org_frm_mem_addr;
              mst_cmd_sm_ip2bus_be   <= (others => '1');
              mst_cmd_sm_xfer_type   <= mst_cntl_burst;
              mst_cmd_sm_xfer_length <= mst_xfer_length;
              mst_cmd_sm_bus_lock    <= '0';
            end if;

          when CMD_WAIT_FOR_DATA =>
            bursting_org_frame <= '1';
            if ( Bus2IP_Mst_Cmplt = '1' ) then
              mst_cmd_sm_state <= CMD_DONE;
            else
              mst_cmd_sm_state <= CMD_WAIT_FOR_DATA;
            end if;

          when CMD_DONE =>
            mst_cmd_sm_state    <= CMD_IDLE;
            mst_cmd_sm_set_done <= '1';
            mst_cmd_sm_busy     <= '0';

          when others =>
            mst_cmd_sm_state    <= CMD_IDLE;
            mst_cmd_sm_busy     <= '0';

        end case;

      end if;
    end if;

  end process MASTER_CMD_SM_PROC;
  -- user logic master write locallink interface assignments
  IP2Bus_MstWr_src_rdy_n <= not(mst_llwr_sm_src_rdy);
  IP2Bus_MstWr_src_dsc_n <= '1'; -- do not throttle data
  IP2Bus_MstWr_rem       <= (others => '0');
  IP2Bus_MstWr_sof_n     <= not(mst_llwr_sm_sof);
  IP2Bus_MstWr_eof_n     <= not(mst_llwr_sm_eof);

  -- implement a simple state machine to enable the
  -- write locallink interface to transfer data
  LLINK_WR_SM_PROC : process( Bus2IP_Clk ) is
    constant BYTES_PER_BEAT : integer := C_MST_DWIDTH/8;
  begin

    if ( Bus2IP_Clk'event and Bus2IP_Clk = '1' ) then
      if ( Bus2IP_Reset = '1' ) then

        -- reset condition
        mst_llwr_sm_state   <= LLWR_IDLE;
        mst_llwr_sm_src_rdy <= '0';
        mst_llwr_sm_sof     <= '0';
        mst_llwr_sm_eof     <= '0';
        mst_llwr_byte_cnt   <= 0;

      else

        -- default condition
        mst_llwr_sm_state   <= LLWR_IDLE;
        mst_llwr_sm_src_rdy <= '0';
        mst_llwr_sm_sof     <= '0';
        mst_llwr_sm_eof     <= '0';
        mst_llwr_byte_cnt   <= 0;

        -- state transition
        case mst_llwr_sm_state is

          when LLWR_IDLE =>
            if ( mst_cmd_sm_start_wr_llink = '1' ) then --always bursting so
                                                           --I don't need: and mst_cntl_burst = '1' ) then
              mst_llwr_sm_state <= LLWR_BRST_INIT;
            else
              mst_llwr_sm_state <= LLWR_IDLE;
            end if;

            -------------------------------------------------------------------
            -- it would be nice to enable single transfers also, and have them
            -- initiated by capture_done and not fifo_prog_full and not
            -- fifo_empty, that way this halting state where the data is not a
            -- full burst size wouldn't break the core
            -------------------------------------------------------------------
          --when LLWR_SNGL_INIT =>
          --  mst_llwr_sm_state   <= LLWR_SNGL;
          --  mst_llwr_sm_src_rdy <= '1';
          --  mst_llwr_sm_sof     <= '1';
          --  mst_llwr_sm_eof     <= '1';

          --when LLWR_SNGL =>
          --  -- destination discontinue write
          --  if ( Bus2IP_MstWr_dst_dsc_n = '0' and Bus2IP_MstWr_dst_rdy_n = '0' ) then
          --    mst_llwr_sm_state   <= LLWR_IDLE;
          --    mst_llwr_sm_src_rdy <= '0';
          --    mst_llwr_sm_eof     <= '0';
          --  -- single data beat transfer complete
          --  elsif ( mst_fifo_valid_read_xfer = '1' ) then
          --    mst_llwr_sm_state   <= LLWR_IDLE;
          --    mst_llwr_sm_src_rdy <= '0';
          --    mst_llwr_sm_sof     <= '0';
          --    mst_llwr_sm_eof     <= '0';
          --  -- wait on destination
          --  else
          --    mst_llwr_sm_state   <= LLWR_SNGL;
          --    mst_llwr_sm_src_rdy <= '1';
          --    mst_llwr_sm_sof     <= '1';
          --    mst_llwr_sm_eof     <= '1';
          --  end if;

          when LLWR_BRST_INIT =>
            mst_llwr_sm_state   <= LLWR_BRST;
            mst_llwr_sm_src_rdy <= '1';
            mst_llwr_sm_sof     <= '1';
            mst_llwr_byte_cnt   <= to_integer(unsigned(mst_xfer_length));

          when LLWR_BRST =>
            if ( mst_fifo_valid_read_xfer = '1' ) then
              mst_llwr_sm_sof <= '0';
            else
              mst_llwr_sm_sof <= mst_llwr_sm_sof;
            end if;
            -- destination discontinue write
            if ( Bus2IP_MstWr_dst_dsc_n = '0' and
                 Bus2IP_MstWr_dst_rdy_n = '0' ) then
              mst_llwr_sm_state   <= LLWR_IDLE;
              mst_llwr_sm_src_rdy <= '1';
              mst_llwr_sm_eof     <= '1';
            -- last data beat write
            elsif ( mst_fifo_valid_read_xfer = '1' and
                   (mst_llwr_byte_cnt-BYTES_PER_BEAT) <= BYTES_PER_BEAT ) then
              mst_llwr_sm_state   <= LLWR_BRST_LAST_BEAT;
              mst_llwr_sm_src_rdy <= '1';
              mst_llwr_sm_eof     <= '1';
            -- wait on destination
            else
              mst_llwr_sm_state   <= LLWR_BRST;
              mst_llwr_sm_src_rdy <= '1';
              -- decrement write transfer counter if it's a valid write
              if ( mst_fifo_valid_read_xfer = '1' ) then
                mst_llwr_byte_cnt <= mst_llwr_byte_cnt - BYTES_PER_BEAT;
              else
                mst_llwr_byte_cnt <= mst_llwr_byte_cnt;
              end if;
            end if;

          when LLWR_BRST_LAST_BEAT =>
            -- destination discontinue write
            if ( Bus2IP_MstWr_dst_dsc_n = '0' and
                 Bus2IP_MstWr_dst_rdy_n = '0' ) then
              mst_llwr_sm_state   <= LLWR_IDLE;
              mst_llwr_sm_src_rdy <= '0';
            -- last data beat done
            elsif ( mst_fifo_valid_read_xfer = '1' ) then
              mst_llwr_sm_state   <= LLWR_IDLE;
              mst_llwr_sm_src_rdy <= '0';
            -- wait on destination
            else
              mst_llwr_sm_state   <= LLWR_BRST_LAST_BEAT;
              mst_llwr_sm_src_rdy <= '1';
              mst_llwr_sm_eof     <= '1';
            end if;

          when others =>
            mst_llwr_sm_state <= LLWR_IDLE;

        end case;

      end if;
    else
      null;
    end if;

  end process LLINK_WR_SM_PROC;


  -- local srl fifo for data storage
  mst_fifo_valid_write_xfer <= not(Bus2IP_MstRd_src_rdy_n) and mst_llrd_sm_dst_rdy;
  mst_fifo_valid_read_xfer  <= not(Bus2IP_MstWr_dst_rdy_n) and mst_llwr_sm_src_rdy;
  org_frame_fifo_rd_en <= mst_fifo_valid_read_xfer;  --this might be 1 clock
                                                     --too slow???

  IP2Bus_MstWr_d <= org_frame_fifo_out;
  
 -- try registering the pixel valid signal by one clock cycle
  process(Bus2IP_Clk,Bus2IP_Reset,pixel,pixel_valid)
  begin
    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Reset = '1' then
        --pixel_reg <= (others => '0');
        pixel_valid_reg <= '0';
      else
        --pixel_reg <= pixel;
        pixel_valid_reg <= pixel_valid;
      end if;
    end if;
  end process; 
  pixel_reg <= pixel;
-------------------------------------------------------------------------------
-- ----------------------------------------------------------------------------
-- Section 8: Component Instantions
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
  frame_sync_hs_1: frame_sync_hs
    generic map (
      C_POS_EDGE_PIX_CLK => TRUE,
      C_DONE_STICKY      => TRUE,
      C_DATA_WIDTH       => 8)
    port map (
      rst             => core_soft_reset,
      cam_data        => cam_data,
      cam_frame_valid => cam_frame_valid,
      cam_line_valid  => cam_line_valid,
      cam_pix_clk     => cam_pix_clk,
      clk             => Bus2IP_Clk,
      cam_bytes       => sr4_save_options(26 to 27),
      start_capture   => start_capture,
      capture_done    => capture_done,
      dout            => pixel,
      dout_valid      => pixel_valid,
      dout_rd_en      => pixel_valid,
      new_frame       => new_frame,
      frame_active    => open);

  CAM_SER_I: cam_ser
    port map (
      clk                       => Bus2IP_Clk,
      reset                     => core_soft_reset,   
      wr_en                     => cam_ser_we,
      rd_en                     => cam_ser_re,
      dev_addr                  => sr4_save_options(18 to 25),--x"B8",
      reg_addr                  => cam_ser_addr,
      data_in                   => cam_ser_data_in,
      data_out                  => cam_ser_data_out,
      ready                     => cam_ser_ready, 
      sdata_I                   => cam_sdata_I,--
      sdata_O                   => cam_sdata_O,--
      sdata_T                   => cam_sdata_T,--
      sclk                      => cam_sclk);--
  
  frame_fifo_1: frame_fifo
    port map (
      din              => pix_word,
      prog_full_thresh => org_fifo_prog_full,
      rd_clk           => Bus2IP_Clk,
      rd_en            => org_frame_fifo_rd_en,
      rst              => core_soft_reset,
      wr_clk           => Bus2IP_Clk,
      wr_en            => org_frame_fifo_wr_en,
      dout             => org_frame_fifo_out,
      empty            => org_frame_fifo_empty,
      full             => open,
      prog_full        => org_frame_fifo_ready);
  
  org_fifo_prog_full <= sr3_burst_size(21 to 30);--size in bytes divided by 2
  org_frame_fifo_wr_en <= pix_word_valid and sr4_save_options(31);

  seg_frame_fifo_ready <= '0';
  seg_frame_fifo_wr_en <= '0';
  seg_frame_fifo_empty <= '1';
  seg_frame_fifo_out <= (others => '0');
  
  
  -- 1 byte to 2 byte converter (for bw output)
  -- Martial single bytes into full machine words
  PIXEL_BYTES_TO_WORD: size_buffer
   generic map (
       C_IN_WIDTH  => 8,
       C_OUT_WIDTH => 16)
   port map (
       clk                      => Bus2IP_Clk,
       reset                    => core_soft_reset,
       in_valid                 => pixel_valid,
       data_in                  => pixel,
       out_valid                => size_buffer_valid, --pix_word_valid,
       data_out                 => size_buffer_dout); --pix_word);
  
  -----------------------------------------------------------------------------
  -- Camera pre-processing for MT9V024.  If not using MT9V024 (or other
  -- BayerRGB output camera sensor), pipe
  -- size_buffer's data_out into the fifo (via pix_word) instead of the concatenated output of
  -- this core
  -----------------------------------------------------------------------------
  camera_processor_1: camera_processor
    generic map (
      C_IMAGE_WIDTH    => C_IMAGE_WIDTH,
      C_IMAGE_HEIGHT   => C_IMAGE_HEIGHT,
      C_CAM_DATA_WIDTH => 8,
      C_R_DATA_WIDTH   => 5,
      C_G_DATA_WIDTH   => 6,
      C_B_DATA_WIDTH   => 5,
      C_DEMOSAIC_TYPE  => "MICROSOFT",
      C_INSERT_SKIP    => 1,            
      C_CC_A11         => 1.30577759827834,
      C_CC_A12         => -0.106734009508285,
      C_CC_A13         => -0.211208972507290,
      C_CC_A21         => -0.648321727706288,
      C_CC_A22         => 1.88073099269499,
      C_CC_A23         => -0.291332927457631,
      C_CC_A31         => -0.467686035588454,
      C_CC_A32         => -0.490654659445285,
      C_CC_A33         => 3.87069855054429
      )
    port map (
      clk       => Bus2IP_Clk,
      rst       => core_soft_reset,
      bayer_pos => "10",
      cc_bypass => sr4_save_options(17),--bypassing color correction, change to
                                        --0 to enable (need to set generic
                                        --values above for good correction)
      new_frame => new_frame,
      din       => pixel_reg,
      wr_en     => pixel_valid_reg,
      r         => r,
      g         => g,
      b         => b,
      rgb_valid => rgb_valid);
	  
	  
  --signal routing
  hsv_r <= r & "000";
  hsv_g <= g & "00";
  hsv_b <= b & "000";
  
  rgb2hsv_1: rgb2hsv  
  port map(
    clk         => Bus2IP_Clk,
    reset       => core_soft_reset,

    R           => hsv_r,
    G           => hsv_g,
    B           => hsv_b,
    en          => rgb_valid,

    H           => h,
    S           => s,
    V           => v,
    valid       => hsv_valid);


  -----------------------------------------------------------------------------
  -- I added this mux so that either the MT9V111 or the MT9V024 could be used
  -- on the game system board (using the original Micron/Aptina header boards).
  --  The MT9V024 outputs BayerRGB, so it's output must be piped through the
  --  camera_processor core, whereas the MT9V111 outputs RGB565, so it can skip
  --  that step and just needs its output word-serialized.
  -----------------------------------------------------------------------------
  pix_word_valid <= hsv_valid when sr4_save_options(16) = '1' else
                    size_buffer_valid;
  pix_word <= h(8 downto 3) & s(7 downto 3) & v(7 downto 3) when sr4_save_options(16) = '1' else
              size_buffer_dout;
  
  ------------------------------------------
  -- Example code to drive IP to Bus signals
  ------------------------------------------
  IP2Bus_Data  <= slv_ip2bus_data when slv_read_ack = '1' else
                  mst_ip2bus_data when mst_read_ack = '1' else
                  (others => '0');

  IP2Bus_WrAck <= slv_write_ack or mst_write_ack;
  IP2Bus_RdAck <= slv_read_ack or mst_read_ack;
  IP2Bus_Error <= '0';

end IMP;
