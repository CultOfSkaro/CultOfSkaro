-------------------------------------------------------------------------------
-- High Speed Frame Grabber for Micron Cameras
--
-- FILE: frame_sync_hf.vhd
-- AUTHOR: Wade S. Fife
-- DATE: August, 2004
-- MODIFIED: June 9, 2007
--
--
-- DESCRIPTION
--
-- This core serves as an interface to a typical CMOS image sensor, with frame
-- and line valid or sync signals. This core synchronizes with the camera and
-- transfers the image data, indicating the start and end of frames.
--
-- This version of the core was designed as a replacement to the frame_synch
-- entity, which could not handle high-speed cameras or situations where the
-- clk frequency was too slow relative to the pixel clock. This core only
-- requires that the clk frequency be at least as fast as the camera's pix_clk
-- frequency.
--
-- The output of this core is a FWFT (first word fall through) FIFO. If the
-- FIFO behavior is not desired, the dout_valid can be connected directly to
-- the dout_rd_en. This enables automatic reading of the FIFO. If the FIFO
-- behavior is used, care must be taken to ensure that the core is read quickly
-- enough to keep up with the camera data.
--
--
-- GENERICS
--
--   C_POS_EDGE_PIX_CLK : Some cameras align the positive clock edge with the
--                        middle of the data, others align the negative edge
--                        with the middle of the data. Set to TRUE if your
--                        camera uses the positive clock edge. See the camera's
--                        data sheet for more information.
--   C_DONE_STICKY : When TRUE, causes the done_capture signal to remain high
--                   until start_capture is signaled again. When false,
--                   capture_done goes high for only one clk cycle.
--   C_DATA_WIDTH  : Bit width of camera's data port.
--
--
-- PORTS
--
--   start_capture : Assert high for one clock cycle to capture the next
--                   available frame.
--   capture_done  : Indicates that the last capture was completed. Optionally
--                   remains high until the next start_capture signal,
--                   depending on C_DONE_STICKY.
--   dout          : Captured data word output
--   dout_valid    : Indicates dout is currently valid.
--   dout_rd_en    : Assert high for one cycle to signal a read of dout.
--   new_frame     : Goes high for one clock cycle to indicate a new frame will
--                   be starting soon.
--   frame_active  : Goes high on the cycle after new_frame and stays high for
--                   the duration of the frame (until cam_frame_valid goes
--                   low).
--   cam_bytes : Indicates which bytes to receive from the camera. Use
--               "00" for all bytes, "01" for odd bytes, and "10" for even
--               bytes. The first byte of the frame is considered to be byte
--               0, and is therefore even.
--
--   cam_data        : Connect to camera's data port.
--   cam_frame_valid : Connect to the camera's frame valid pin.
--   cam_line_valid  : Connect to the camera's line valid pin.
--   cam_pix_clk     : Connect to the camera's pixel clock.
--
--
--  NOTES
--
--  * This core assumes the line and frame valid are high during the active 
--    frame.
--
--  * If cam_bytes is not "00" then each line of the camera's image must have
--    an even number of bytes.
--
--  * There should be several cam_pix_clk cycles between when frame valid goes
--    high (or low) and when line valid goes high (or low).
--
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity frame_sync_hs is
  generic (
    C_POS_EDGE_PIX_CLK : boolean := TRUE;
    C_DONE_STICKY      : boolean := FALSE;
    C_DATA_WIDTH       : integer := 8);
  port (
    -- Asynchronous reset
    rst : in std_logic;

    -- Camera interface (Input side, cam_pix_clk domain)
    cam_data        : in std_logic_vector(C_DATA_WIDTH-1 downto 0);
    cam_frame_valid : in std_logic;
    cam_line_valid  : in std_logic;
    cam_pix_clk     : in std_logic;

    -- Frame capture interface (Output side, clk domain)
    clk           : in std_logic;
    cam_bytes     : in std_logic_vector(1 downto 0);
    start_capture : in  std_logic;
    capture_done  : out std_logic;
    dout          : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    dout_valid    : out std_logic;
    dout_rd_en    : in  std_logic;
    new_frame     : out std_logic;
    frame_active  : out std_logic);
end frame_sync_hs;


architecture imp of frame_sync_hs is

  component frame_sync_fifo
    port (
      rst    : IN  std_logic;
      wr_clk : IN  std_logic;
      rd_clk : IN  std_logic;
      din    : IN  std_logic_VECTOR(7 downto 0);
      wr_en  : IN  std_logic;
      rd_en  : IN  std_logic;
      dout   : OUT std_logic_VECTOR(7 downto 0);
      full   : OUT std_logic;
      empty  : OUT std_logic);
  end component;
  
  type state_type is (IDLE_STATE,
                      WAIT_END_STATE,
                      WAIT_START_STATE,
                      FRAME_START_STATE,
                      FRAME_VALID_STATE,
                      FRAME_END_STATE);


  -----------------------------------------------------------------------------
  -- cam_pix_clk clock domain signals

  -- Camera pixel clock, possible inverted
  signal pix_clk : std_logic;

  signal frame_active_pcd, frame_active_pcd_t : std_logic;

  -- FIFO signals
  signal fifo_empty, fifo_wr_en : std_logic;

  
  -----------------------------------------------------------------------------
  -- clk clock domain signals
  
  -- FIFO signals
  signal fifo_rd_en : std_logic;

  -- Camera frame valid signals (clk domain)
  signal frame_valid_cd, frame_valid_cd_t : std_logic;

  -- High when frame valid is high and the current frame has been requested
  -- for capture
  signal frame_active_t : std_logic;

  -- Signal to go high for one cycle at beginning of a frame
  signal frame_start : std_logic;

  -- State bit indicating if the next byte is odd or even
  signal comp_reg : std_logic;

  -- Signal indicating the next read should be allows, based on comp_reg and
  -- cam_bytes
  signal read_ok : std_logic;

  -- State machine signals
  signal current_state, next_state : state_type;
  
begin

  -- Optionally use positive edge or negative edge of pixel clock
  NOT_INVERT_GEN: if C_POS_EDGE_PIX_CLK generate
    pix_clk <= cam_pix_clk;
  end generate;
  INVERT_GEN: if not C_POS_EDGE_PIX_CLK generate
    pix_clk <= not cam_pix_clk;
  end generate;

  -- First word fall through fifo. The depth is not necissarily important.
  frame_sync_fifo_1: frame_sync_fifo
    port map (
      rst    => rst,
      wr_clk => pix_clk,
      rd_clk => clk,
      din    => cam_data,
      wr_en  => fifo_wr_en,
      rd_en  => fifo_rd_en,
      dout   => dout,
      full   => open,
      empty  => fifo_empty);

  
  -----------------------------------------------------------------------------
  -- CAM_PIX_CLK CLOCK DOMAIN -------------------------------------------------

  
  new_frame <= frame_start;
  frame_active <= frame_active_t;

  -- Use cam_line_valid as data valid signal, but ignore if we're not actively
  -- capturing.
  fifo_wr_en <= cam_line_valid and frame_active_pcd;

  -- Bring frame_active signal to the cam_pix_clk domain.
  PCD_FRAME_ACTIVE_REG: process (pix_clk, rst, frame_active_t)
  begin
    if rst = '1' then
      frame_active_pcd <= '0';
    elsif rising_edge(pix_clk) then
      frame_active_pcd_t <= frame_active_t;
      frame_active_pcd <= frame_active_pcd_t;
    end if;
  end process;
  

  
  -----------------------------------------------------------------------------
  -- CLK CLOCK DOMAIN ---------------------------------------------------------

  
  -- Output valid if FIFO's not empty and output is current cam_byte selection.
  dout_valid <= (not fifo_empty) and read_ok;

  -- Read from FIFO if user selects or if current data isn't selected by
  -- cam_bytes.
  fifo_rd_en <= dout_rd_en or ((not fifo_empty) and (not read_ok));

  -- Bring frame_valid to the clk domain.
  FRAM_VALID_REG: process (clk, rst, cam_frame_valid, frame_valid_cd_t)
  begin
    if rst = '1' then
      frame_valid_cd <= '0';
    elsif rising_edge(clk) then
      frame_valid_cd_t <= cam_frame_valid;
      frame_valid_cd <= frame_valid_cd_t;
    end if;
  end process;

  
  -----------------------------------------------------------------------------
  -- Logic to determine if the next read should be allowed. Reading is only
  -- allowed on the bytes selected by cam_bytes. If cam_bytes is "00" then all
  -- bytes are passed through, so no state machine is necessary.
  
  READ_SELECT_PROC: process (clk, rst, cam_bytes, fifo_rd_en,
                             comp_reg, frame_start)
  begin
    -- Toggle comp_reg bit after each read.
    if rst = '1' then
      comp_reg <= '0';
    elsif rising_edge(clk) then
      if frame_start = '1' then
        -- Reset comp_reg at beginning of frame to increase robustness
        comp_reg <= '0';
      elsif fifo_rd_en = '1' then
        comp_reg <= not comp_reg;
      end if;
    end if;

    -- Determine value of read_ok, asynchronously
    case cam_bytes is
      when "00" =>
        read_ok <= '1';
      when "01" =>
        read_ok <= comp_reg;
      when "10" =>
        read_ok <= not comp_reg;
      when others =>
        read_ok <= '1';
    end case;
  end process;
  
  
  -----------------------------------------------------------------------------
  -- State machine to monitor the frame and line valid signals in order to give
  -- state information.
  
  FSM_REG_PROC: process (clk, rst, next_state)
  begin
    if rst = '1' then
      current_state <= IDLE_STATE;
    elsif rising_edge(clk) then
      current_state <= next_state;
    end if;
  end process;

  FSM_COMB_PROC: process (clk, current_state, start_capture, frame_valid_cd)
  begin
    -- Default signal values
    next_state <= current_state;
    frame_active_t <= '0';
    capture_done <= '0';
    frame_start <= '0';
    
    case current_state is
      when IDLE_STATE =>
        if C_DONE_STICKY then
          capture_done <= '1';
        end if;
        if start_capture = '1' then
          next_state <= WAIT_END_STATE;
        end if;

      -- Wait for current frame to end
      when WAIT_END_STATE =>
        if frame_valid_cd = '0' then
          next_state <= WAIT_START_STATE;
        end if;

      -- Wait for frame to start
      when WAIT_START_STATE =>
        if frame_valid_cd = '1' then
          next_state <= FRAME_START_STATE;
        end if;

      -- Signal that frame is starting
      when FRAME_START_STATE =>
        frame_start <= '1';
        next_state <= FRAME_VALID_STATE;

      -- Active frame capture
      when FRAME_VALID_STATE =>
        frame_active_t <= '1';
        if frame_valid_cd = '0' then
          next_state <= FRAME_END_STATE;
        end if;

      -- Signal end of frame
      when FRAME_END_STATE =>
        capture_done <= '1';
        next_state <= IDLE_STATE;

      when others =>
        next_state <= IDLE_STATE;
        
    end case;
  end process;
  
end imp;
