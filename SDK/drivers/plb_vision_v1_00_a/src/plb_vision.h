/*****************************************************************************
* Filename:          /data/projects/SpencerBranch/drivers/plb_vision_v1_00_a/src/plb_vision.h
* Version:           1.00.a
* Description:       plb_vision Driver Header File
* Date:              Mon Feb  8 12:51:11 2010 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef PLB_VISION_H
#define PLB_VISION_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xio.h"
#include <xparameters.h>

/************************** Constant Definitions ***************************/


/** This is the default image width aquired from the camera
	Changing this value #WILL# cause the software to attempt to configure the 
	camera to output images according to this characteristic. 
	
	#BUT# changing this value #WILL NOT# have any effect on any custom image 
	processing FPGA hardware, such as the camera core or the correlator core.
	These cores sometimes contain processing cores that require the image 
	width and height to be set at systensis time. (i.e. a GENERIC parameter)
	Modify the confured image size in the FPGA hardware can be done by modifying
	the MHS file instance of the camera core or correlator core or any other 
	core that performs image processing.
*/
#define FRAME_WIDTH 	640			



/** This is the default image height aquired from the camera
	Changing this value #WILL# cause the software to attempt to configure the 
	camera to output images according to this characteristic. 
	
	#BUT# changing this value #WILL NOT# have any effect on any custom image 
	processing FPGA hardware, such as the camera core or the correlator core.
	These cores sometimes contain processing cores that require the image 
	width and height to be set at systensis time. (i.e. a GENERIC parameter)
	Modify the confured image size in the FPGA hardware can be done by modifying
	the MHS file instance of the camera core or correlator core or any other 
	core that performs image processing.
*/
#define FRAME_HEIGHT	480


/** These are the identifiers for the different types of Helios Daughter Boards
	These should be used to configure the #software# to interract appropriately
	with the attached daughter board. The main use of these macros is to define 
	which type of camera is attached and which code instructions should be called
	to initialize the camera.
*/
#define DAUGHTER_BOARD_GBV	0
#define DAUGHTER_BOARD_AVT	1
#define DAUGHTER_BOARD_GAME 3



#ifndef PACKED
#define PACKED __attribute__((__packed__))
#endif

#define BASEADDR XPAR_PLB_VISION_0_BASEADDR
/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : Status/Start Capture
 * -- SLV_REG1 : CamSer Ready/Write Request
 * -- SLV_REG2 : CamSer Read Request/Output Data
 * -- SLV_REG3 : PLB Burst Size
 * -- SLV_REG4 : Save Options
 * -- SLV_REG5 : Original frame memory address
 * -- SLV_REG6 : Frame 2 memory address
 * -- SLV_REG7 : UNUSED
 * -- SLV_REG8 : UNUSED
 * -- SLV_REG9 : UNUSED
 */
#define OFFSET_START_CAPTURE				0x0
#define OFFSET_RESET_CAM_CORE      	 	0x0
#define OFFSET_WR_CAM_REG					0x4
#define OFFSET_START_CAM_REG_READ		0x8

#define OFFSET_STATUS						0x0
#define OFFSET_CAM_SER_READY				0x4
#define OFFSET_CAM_SER_DATA				0x8

#define OFFSET_PLB_BURST_SIZE				0xC
#define OFFSET_SAVE_OPTIONS				0x10
#define OFFSET_ORG_FRAME_MEM_ADDR		0x14
#define OFFSET_SEG_FRAME_MEM_ADDR		0x18

#define VISION_SAVE_OPTIONS_NONE					0x00000000
#define VISION_SAVE_OPTIONS_ORIGINAL			0x00000001
#define VISION_SAVE_OPTIONS_CAM_BYTES_ALL		0x00000000
#define VISION_SAVE_OPTIONS_CAM_BYTES_EVEN	0x00000001
#define VISION_SAVE_OPTIONS_CAM_BYTES_ODD		0x00000002
#define VISION_CAM_BYTES_OFFSET					4 		//input to frame sync of which bytes to save
#define VISION_CAM_I2C_ADDR_OFFSET				6		//input to CamSer core for slave address
#define VISION_CAM_COLOR_CORRECT_OFFSET		14		//input to camera_processor to en/disable color correction
#define VISION_CAM_TYPE_OFFSET					15    //input to mux to pipe out camera_processor output or size_buffer output

#define VISION_MAX_NUM_FRAMES				2	//the number of TYPES of frames, not the number of frames we're saving, that is in FrameTable.h
/**************************** Enumerations *********************************/
enum CameraOptions
{
	VISION_FRAME_NOT_AVAILABLE,
	//VISION_FRAME_GRAYSCALE8				= 1,
	VISION_FRAME_RGB565,
	VISION_FRAME_COUNT
};

/**************************** Type Definitions *****************************/
struct CameraInfo
{
	Xuint8		id;
	Xuint8		status;
	Xuint32		width;
	Xuint32		height;
	Xuint32		base_address;
	Xuint32		version;
	Xuint32		subversion;
	Xuint32		revision;
	Xuint8		frame_type[VISION_MAX_NUM_FRAMES];
	signed char	frame_index[VISION_FRAME_COUNT];
	Xuint32		frame_save_index[VISION_FRAME_COUNT];
} PACKED;
typedef struct CameraInfo CameraInfo;
/**************************** Global Memory ********************************/
extern CameraInfo camera0;
/// This macro defines which daughter board is currently attached to the Helios
extern Xuint8 DAUGHTER_BOARD;
/***************** Macros (Inline Functions) Definitions *******************/
#define PTR(X) ((volatile Xuint32 *)(X))
/**
 *
 * Write a value to a PLB_VISION register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the PLB_VISION device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void PLB_VISION_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define PLB_VISION_mWriteReg(RegOffset, Data) \
 	XIo_Out32((BASEADDR) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a PLB_VISION register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the PLB_VISION device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 PLB_VISION_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define PLB_VISION_mReadReg(RegOffset) \
 	XIo_In32((BASEADDR) + (RegOffset))
/******** Camera Serial inline functions ***************/
/* Resets  the camera core components.  This includes the PLB fifos, frame_sync_hs core, and size_buffer,
 * but does not reset the image sensor or the plb ipif 
 */
#define ResetCameraCore() \
	*PTR(BASEADDR + OFFSET_RESET_CAM_CORE)		= 0x2;
/* Sending this command causes the fame_sync_hs core to enable for one entire frame, outputting pixels and
 * associated valid data signals in order to capture the frame data.  After one full frame, the frame_sync_hs
 * core will shut itself off (and assert one of the two signals tied into the camera interrupt), so to continually
 * capture frames, this command must be exectued repeatedly (every time the interrupt fires).  This currently happens
 * inside the frame table ISR, which is tied to the vision core's interrupt
 */
#define StartFrameCapture() \
	*PTR(BASEADDR + OFFSET_START_CAPTURE)		= 0x1;
/* Sending this command asserts a logical 0 on the image sensor's reset pin, causing a hard reset of the camera image sensor.
 * This does not reset the camera core, so it should only happen when the camera is not capturing data to avoid halting conditions
 */
#define CameraHardReset() \
	*PTR(BASEADDR + OFFSET_RESET_CAM_CORE)		= 0x4;
/* These two are only used with the MT9V111 (GBV daughter board) 
 SelectSensorRegisterBank uses I2C to select the core register bank of the MT9V111,
 SelectIPFRegisterBank uses I2C to select the IPF register bank of the MT9V111 
 Any functions setting or reading registers should be sure to call these inlines first to
 make certain you are reading/writing the correct register!
*/
#define SelectSensorRegisterBank() \
	WriteCameraRegister(0x01, 4);
#define SelectIPFRegisterBank() \
	WriteCameraRegister(0x01, 1);
/* Frame Save Options:
 * BIT 0: save original frame, 1 - save, 0 - ignore
 * BIT 1: save ____ frame (unimplemented)
 * BIT 2: save ____ frame (unimplemented)
 * BIT 3: save ____ frame (unimplemented)
 * BIT 4-5: camera pixel output format
 * expects value of 0, 1, or 2 (0x00, 0x01, or 0x10 according to frame_sync
 *  This connects directly to cam_bytes of the frame_synch core, so
 *   "The cam_bytes input indicates which bytes of the camera data are transfered.
 *    All camera color modes output two bytes per pixel. The cam_bytes can be
 *    configured to transfer all bytes bytes (cam_bytes = "00"), only odd bytes
 *    (byte_ comp = "01", and only even bytes (cam_bytes = "10"). This is useful,
 *    for example, if you only want the Y component (intensity) transfered for
 *    grayscale images. To be clear, the first byte (byte 0) is considered an even
 *    byte.
 * BIT 6-13: camera I2C Serial address
 *   This connects to the camSer core and set the I2C slave address for the camera, default should be 0xB8
 *   according to micron datasheet
 * BIT 14: Camera type
 *   0 - MT9V111 (don't use camera_processor, output is RGB565, so just serialize words in HW
 *   1 - MT9V023/24 output is Bayer RGB, process via camera_processor into RGB565 words
*/
#define SetFrameSaveOptions(options)\
	*PTR(BASEADDR + OFFSET_SAVE_OPTIONS) = *PTR(BASEADDR + OFFSET_SAVE_OPTIONS) & ~(0xF);\
	*PTR(BASEADDR + OFFSET_SAVE_OPTIONS) = *PTR(BASEADDR + OFFSET_SAVE_OPTIONS) | (0xF&options);
#define SetPixelOutputFormat(value) \
	*PTR(BASEADDR + OFFSET_SAVE_OPTIONS) = *PTR(BASEADDR + OFFSET_SAVE_OPTIONS) & ~(0x3<<VISION_CAM_BYTES_OFFSET);\
	*PTR(BASEADDR + OFFSET_SAVE_OPTIONS) = *PTR(BASEADDR + OFFSET_SAVE_OPTIONS) | ((0x3&value) << VISION_CAM_BYTES_OFFSET);
#define SetCameraSerialAddress(address) \
	*PTR(BASEADDR + OFFSET_SAVE_OPTIONS) = *PTR(BASEADDR + OFFSET_SAVE_OPTIONS) & ~(0xFF<<VISION_CAM_I2C_ADDR_OFFSET);\
	*PTR(BASEADDR + OFFSET_SAVE_OPTIONS) = *PTR(BASEADDR + OFFSET_SAVE_OPTIONS) |  ((0xFF&address)<<VISION_CAM_I2C_ADDR_OFFSET);
#define SetCameraType(type) \
	*PTR(BASEADDR + OFFSET_SAVE_OPTIONS) = *PTR(BASEADDR + OFFSET_SAVE_OPTIONS) & ~(0x1<<VISION_CAM_TYPE_OFFSET);\
	*PTR(BASEADDR + OFFSET_SAVE_OPTIONS) = *PTR(BASEADDR + OFFSET_SAVE_OPTIONS) |  ((0x1&type)<<VISION_CAM_TYPE_OFFSET);
#define DisableColorCorrection() \
	*PTR(BASEADDR + OFFSET_SAVE_OPTIONS) = *PTR(BASEADDR + OFFSET_SAVE_OPTIONS) & ~(0x1<<VISION_CAM_COLOR_CORRECT_OFFSET);\
	*PTR(BASEADDR + OFFSET_SAVE_OPTIONS) = *PTR(BASEADDR + OFFSET_SAVE_OPTIONS) |  (0x1<<VISION_CAM_COLOR_CORRECT_OFFSET);
#define EnableColorCorrection() \
	*PTR(BASEADDR + OFFSET_SAVE_OPTIONS) = *PTR(BASEADDR + OFFSET_SAVE_OPTIONS) & ~(0x1<<VISION_CAM_COLOR_CORRECT_OFFSET);
/* Sets the PLB burst transaction size
 * value - transaction size in BYTES.  THIS MUST BE A MULTIPLE OF 64
 *       - defaults to 1024
 * I have had success using burst sizes of 1024 and 512.  Sizes lower than 512 gave me some frame shearing,
 * but my sensor is a little whacky, so that might just be my application.  All multiples of 64 _should_ work.
 * 
 * Why should you adjust the PLB burst size?  Burst size changes the length of each PLB transaction.  At 1024,
 * each transaction is tranferring 1k of image data.  On a 64-bit bus, that equates to 128 PLB beats.  That means
 * the entire PLB is unusable for 128 beats (and uninterruptable).  Since EVERYTHING is on the same PLB (Memory, UARTs,
 * GPIO's, Camera), anything else that needs to transfer must wait.  By reducing the burst size you shrink the number of
 * beats that each transaction locks the PLB for, giving other components more opportunities to use the PLB bus.
 */
#define SetPLBBurstSize(value) \
	*PTR(BASEADDR + OFFSET_PLB_BURST_SIZE) = value;
	
// FUNCTION PROTOTYPES ////////////////////////////////////////////////////////

void ResetCameraSensor();
void DisplayTestPattern();
void DisableTestPattern();
void SetFrameAddress(Xuint32 frame_id, Xuint32 frameAddr);
Xuint32 GetFrameAddress(Xuint32 frame_id);
Xuint32 CameraStatus();
void WriteCameraRegister(Xuint8 addr, Xuint16 data);
Xuint32 ReadCameraRegister(Xuint8 addr);
Xuint32 CalculateImageBufferSize(CameraInfo* camera, Xuint32 type);
Xuint32 GetFrameSaveOptions(void);
Xuint32 GetPLBBurstSize(void);
void Init_Camera();
#endif /** PLB_VISION_H */
