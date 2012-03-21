/*****************************************************************************
* Filename:          /data/projects/SpencerBranch/drivers/plb_vision_v1_00_a/src/plb_vision.c
* Version:           1.00.a
* Description:       plb_vision Driver Source File
* Date:              Mon Feb  8 12:51:11 2010 (by Create and Import Peripheral Wizard)

 * Spencer Fowers
 * Imported code from Barrett Edwards' SAA plb_camera core for this new EDK 11 project
 * 2-23-2010

*****************************************************************************/


/***************************** Include Files *******************************/

#include <string.h>
#include <xcache_l.h>
#include <xpseudo_asm.h>
#include "plb_vision.h"


/************************** Structs ****************************************/
/************************** Global Memory **********************************/
CameraInfo camera0 = {
	0,									//ID #
	1,									//status, 1 = connected, 0 = not connected
	640,								//image width
	480,								//image height
	XPAR_PLB_VISION_0_BASEADDR,//base address
	1,									//version
	0,									//subversion
	'a',								//revision
	{									//frame types
		VISION_FRAME_NOT_AVAILABLE,
		VISION_FRAME_RGB565		// 1
		//VISION_FRAME_GRAYSCALE8,// 2
	},
	{				//  Index into the frame_type array for the possible frame
		-1 ,		// VISION_FRAME_NOT_AVAILABLE
		 //1 ,		// VISION_FRAME_GRAYSCALE8
		 1 ,		// VISION_FRAME_RGB565
	},
	{				// frame_save_index[VISION_FRAME_COUNT]
		0		,	// VISION_FRAME_NOT_AVAILABLE
		//0x0002,	// VISION_FRAME_GRAYSCALE8
		0x0001,	// VISIOn_FRAME_RGB565
	}
};
Xuint8 DAUGHTER_BOARD = DAUGHTER_BOARD_GAME;
/************************** Function Definitions ***************************/

/* Uses the I2C interface to cause a soft-reset of the image sensor.  I do not believe this erases
 * changes to register banks, but I'm not 100% sure
 */
void ResetCameraSensor()
{
	if (DAUGHTER_BOARD == DAUGHTER_BOARD_GBV)
	{
		SelectIPFRegisterBank();
		WriteCameraRegister(0x07, 1);
		WriteCameraRegister(0x07, 0);
	}
	if (DAUGHTER_BOARD == DAUGHTER_BOARD_AVT || DAUGHTER_BOARD == DAUGHTER_BOARD_GAME)
	{
		WriteCameraRegister(0x0C, 3);
		//WriteCameraRegister(0x0C, 0);
	}
}
/* Causes the image sensor to output a test pattern over the pixel data lines instead of the actual
 * data from the CMOS sensor.  This is a register setting, so to turn it off you cannot just power
 * down the chip, but you must call DisableTestPattern() once.
 */
void DisplayTestPattern() 
{
	if (DAUGHTER_BOARD == DAUGHTER_BOARD_GBV)
	{
		SelectIPFRegisterBank();
		WriteCameraRegister(0x48, 1); //last value picks pattern, 0-9
	}
	if (DAUGHTER_BOARD == DAUGHTER_BOARD_AVT || DAUGHTER_BOARD == DAUGHTER_BOARD_GAME)
	{
		WriteCameraRegister(0x70, ReadCameraRegister(0x70)&~(0x1<<5));
		WriteCameraRegister(0x7F, 0x3800);
	}
}
/* Disables the test pattern output.  Since the test pattern is a sensor register, enabling a test pattern
 * will cause it to be enabled until it is disabled (not just until power down).  You must call this function
 * once to resume normal operation
 */
void DisableTestPattern() 
{
	if (DAUGHTER_BOARD == DAUGHTER_BOARD_GBV)
	{
		SelectIPFRegisterBank(); 
		WriteCameraRegister(0x48, 0);
	}
	if (DAUGHTER_BOARD == DAUGHTER_BOARD_AVT || DAUGHTER_BOARD == DAUGHTER_BOARD_GAME)
	{
		WriteCameraRegister(0x70, ReadCameraRegister(0x70)|(0x1<<5));
		WriteCameraRegister(0x7F, ReadCameraRegister(0x7F)&~(0x1<<13));
	}
}
/* Sets the location in memory where a frame should be stored.  This value is written into a software register
 * in the vision core and then incremented as bursting takes place.  Because this value is used during a 
 * frame capture, it SHOULD NOT be changed while the core is in operation.
 * @param frame_id int type of frame address to be set.  Currently we only capture one frame, so there is only
                       one choice, VISION_FRAME_RGB565, but more frame types can be added here so multiple images
							  can be saved for each frame
 * @param frameAddr int memory address where this frame type should be saved.  Make sure this memory address
                        has been properly malloc'ed before passing it to the core.
*/
void SetFrameAddress(Xuint32 frame_id, Xuint32 frameAddr)
{
	int offset = 0x0;
	switch(frame_id)
	{
		case VISION_FRAME_RGB565:
			offset = OFFSET_ORG_FRAME_MEM_ADDR;
			break;
		/*case VISION_FRAME_GRAYSCALE8: //this is just an example, this fifo is not built into hardware
			offset = OFFSET_SEG_FRAME_MEM_ADDR;
			break;*/
		default:
			return;
	}
	*PTR(BASEADDR + offset) = frameAddr;
}
/* Returns the frame address in the software register for a given frame type.  This value may be different
 * from the value you gave the core in SetFrameAddress if a capture is currently in progress, because the
 * value returned here is the incremented, current pointer value (where the next PLB burst will be written,
 * NOT the starting location of the frame).
 */
Xuint32 GetFrameAddress(Xuint32 frame_id)
{
	int offset = 0x0;
	switch(frame_id)
	{
		case VISION_FRAME_RGB565:
			offset = OFFSET_ORG_FRAME_MEM_ADDR;
			break;
		/*case VISION_FRAME_GRAYSCALE8: //this is just an example, this fifo is not built into hardware
			offset = OFFSET_SEG_FRAME_MEM_ADDR;
			break;*/
		default:
			return -1;
	}
	return *PTR(BASEADDR + offset);
}
/* This generic function will write a camera register using the I2C interface.  It is used by a number of other
 * inline functions.
 * @param addr uint8 register address to be written
 * @param data uint16 two-byte data value to be written to register addr
 */
void WriteCameraRegister(Xuint8 addr, Xuint16 data){
	// Wait for any previous read/write to complete
	while(*PTR(BASEADDR + OFFSET_CAM_SER_READY) == 0);
	*PTR(BASEADDR  + OFFSET_WR_CAM_REG) = (((Xuint32)addr) << 16) | ((Xuint32)data);	
}
/* Reads out a camera register over the I2C interface
 * @param addr uint8 register address to read out
 * @return uint32 data read out of register.  Data is only 16 bits, so should be masked.
 */
Xuint32 ReadCameraRegister(Xuint8 addr){
	// Wait for any previous read/write to complete
	while(*PTR(BASEADDR + OFFSET_CAM_SER_READY) == 0);
	// Signal to start read
	*PTR(BASEADDR + OFFSET_START_CAM_REG_READ) = (Xuint32)addr;
	// Wait until read is complete
	while(*PTR(BASEADDR + OFFSET_CAM_SER_READY) == 0);
	return *PTR(BASEADDR + OFFSET_CAM_SER_DATA);
}
/* The vision core holds a status register, this reads out that 32 bit value, the bits of the value are defined as:
 * 31-11 - Unused
 * 10 - bursting_org_frame - signal asserted by the PLB burst state machine, bursting and Bus2IP_Mst_Cmplt cause the memory address to be incremented
 * 9 - org_frame_fifo_rd_en - this is set high by the PLB burst state machine to dequeue data onto the PLB bus
 * 8 - Bus2IP_Mst_Cmplt - PLB acknowledge bit that the PLB burst transaction was completed correctly
 * 7 - sr4_save_options(31) - this bit is the "save original (RGB565) frame" bit, if high, the original frame will be burst across the PLB
 * 6 - Bus2IP_MstWr_dst_rdy_n - this signal is lowered when the PLB burst destination (sdram) is ready to receive data
 * 5 - int - the interrupt signal (capture_done and org_frame_fifo_empty)
 * 4 - capture done - the frame_sync_hs core asserts this signal when it has finished one complete frame (stays high until a new frame is started)
 * 3 - Org frame fifo ready - the org frame fifo has reached the programmable full threshold and is ready to be burst across the plb
 * 2 - Org frame fifo empty - the RGB565 fifo is completely empty, this bit is anded with capture_done to trigger the vision core interrupt
 * 1 - Seg frame fifo ready (fifo currently does not exist, so this is always tied low)
 * 0 - Seg frame fifo is empty (fifo currently does not exist, so this is always tied high)
 * these values can be changed in user_logic to whatever you want
 */
Xuint32 CameraStatus(){
	return *PTR(BASEADDR + OFFSET_STATUS);
}
/* Utility function to return the actual size in bytes of a frame based on its type */
Xuint32 CalculateImageBufferSize(CameraInfo* camera, Xuint32 type)
{
	Xuint32 FrameSize = 0;
	switch(type)
	{
		case VISION_FRAME_NOT_AVAILABLE:					FrameSize = 0;														break;
		case VISION_FRAME_RGB565:							FrameSize = camera->width * camera->height * 2;			break;
		//case VISION_FRAME_GRAYSCALE8:						FrameSize = camera->width * camera->height;				break;
	}
	return FrameSize;
}
/* returns the value of the frame save options register (sr4), note that sr4 does not hold ONLY 
 * the frame save options.  The contents of sr4 are defined as:
 * 31-16  - Unused
 * 15   - Camera type.  A 1 here will cause the data inserted into the fifo to be the output of the camera_processor core
			 while a 0 will output the data from the image sensor directly (after being serialized from 8 bits to 16 bits).  
			 The MT9V111 outputs RGB565 natively, so it does not need to be processed, only serialized to 16 bits (RGB565)
			 The MT9V024 outputs bayerRGB which needs to go through the camera processor core (demosaicing and (optionally) color correction)
			 and then output from the camera processor core as RGB565 (16 bits).  So this bit should be changed based on which
			 camera is currently connected
 * 14   - Color correction enable/disable.  Writing a 1 here will bypass the color correction portion of the 
 *        camera processor core.  This only takes effect if you are using the output of the camera processor core (see bit 15)
 * 6-13 - Camera Serial I2C address.  The address should be B8, but if a new camera is inserted that uses a
          different I2C address, it can be set in software
 * 4-5  - Camera bytes - input to frame_sync_hs, a 00 will save all bytes, 01 odd bytes only, 10 even bytes only
          coming out of the camera.  This is so you can easily capture a grayscale image with the MT9V111 by 
			 setting the output to YCbCr and only grabbing the first byte (Y) and ignoring the CbCr byte
 * 3    - Unused (for saving other frames)
 * 2    - Unused (for saving other frames)
 * 1    - Unused (for saving other frames)
 * 0    - Save the RGB565 original frame to SDRAM (enables fifos and PLB bursting)
 * There are functions for setting each of these bits that do the masking for you so nothing accidentally gets
 * overwritten.  See SetFrameSaveOptions, SetPixelOutputFormat, SetCameraSerialAddress, 
 *                   DisableColorCorrection()/EnableColorCorrection(), SetCameraType
 */
Xuint32 GetFrameSaveOptions(void)
{
	return *PTR(BASEADDR + OFFSET_SAVE_OPTIONS);
}
/* Returns the current PLB burst size value (MUST be a multiple of 64, MAX 1024), size is in BYTES */
Xuint32 GetPLBBurstSize(void)
{
	return *PTR(BASEADDR + OFFSET_PLB_BURST_SIZE);
}
/* This is the main camera initialization function and must be called before StartFrameCapture is called, or any
 * attempts to write to the camera I2C interface.  This function will set the PLB burst size, perform a hard reset
 * test the I2C to discover which sensor is being used, and then configure the sensor appropriately.  This way,
 * the sensor can be swapped out without any software changes.  There is a section in this init code for both
 * types of sensors, so make sure it is configuring both sensors the way you want.  Once this init code is finished
 * the sensor is ready to capture data, but this code does NOT assert a start capture, that is left to the user */
void Init_Camera()
{
	/* camera-independent defaults */
	SetPLBBurstSize(512);
	SetPixelOutputFormat(VISION_SAVE_OPTIONS_CAM_BYTES_ALL);  //both bytes, this only changes if you want grayscale from the MT9V111
	SetCameraSerialAddress(0xB8);

	CameraHardReset();
	ClockDelay(100000000);  //wait for camera to come back online
	ClockDelay(100000000);  //wait for camera to come back online
	ClockDelay(100000000);  //wait for camera to come back online
	Xuint16 CameraVersion = ReadCameraRegister(0xFF);
	//Xuint16 CameraVersion = 0x1324;
	xil_printf("Chip Version: %x\n\r", CameraVersion);
	Xuint8 spiaddr = 0;
	int timeout = 0;
	xil_printf("waiting for camera I2C interface...");
	/*while(CameraVersion != 0x823A && CameraVersion != 0x1324)
	{
		SetCameraSerialAddress(spiaddr++);
		CameraVersion = ReadCameraRegister(0xFF);
		xil_printf("%x.%x.\n", spiaddr, CameraVersion);
		ClockDelay(100000);  //wait for camera to come back online
		timeout++;
		if( CameraVersion == 0 )
		{
			SetCameraSerialAddress(0xB8);
			CameraVersion = ReadCameraRegister(0xFF);
			break;
		}
	}*/
	SetCameraSerialAddress(0xE0);				//I have no idea why but this is important
	CameraVersion = ReadCameraRegister(0xFF);
	SetCameraSerialAddress(0xB8);
	CameraVersion = ReadCameraRegister(0xFF);
	//CameraVersion = 0x1324;		// we're currently having some trouble finding this camera
	if(CameraVersion == 0)	//probably the MT9V111, make sure...
	{
		SelectSensorRegisterBank(); //in case we're using the MT9V111
	}
	xil_printf("Found camera version %x\r\n", CameraVersion);
	
	switch(CameraVersion)
	{
		case 0x823A:
		case 0x0:
			xil_printf("Using MT9V111 image sensor\r\n");
			DAUGHTER_BOARD = DAUGHTER_BOARD_GBV;
		break;
		case 0x1324:
			xil_printf("Using MT9V024 image sensor\r\n");
			DAUGHTER_BOARD = DAUGHTER_BOARD_GAME;
		break;
		default:
			xil_printf("ERROR, camera version %x NOT SUPPORTED\r\n", CameraVersion);
	}		
	xil_printf("Done!\r\n");	
	SetCameraType(DAUGHTER_BOARD);
	ResetCameraCore();
	ResetCameraSensor();

if (DAUGHTER_BOARD == DAUGHTER_BOARD_GBV)
	{
		SelectSensorRegisterBank();
		SelectIPFRegisterBank();
		WriteCameraRegister(0xA7, FRAME_WIDTH);
		WriteCameraRegister(0xAA, FRAME_HEIGHT);					

		// Reset camera's image flow processor
		WriteCameraRegister(0x05, 0);     // Turn off sharpenning
		WriteCameraRegister(0x08,0xD800); //set output to RGB
		WriteCameraRegister(0x3A,0x00);   //set output to RGB565

		xil_printf("Selecting Core register set\r\n");
		SelectSensorRegisterBank();
		xil_printf("Camera version: %0x\r\n", ReadCameraRegister(0x36));
		SelectIPFRegisterBank();
		xil_printf("Window width: %0d\r\n", ReadCameraRegister(0xA7)); //seems to default to 647, 487???
		xil_printf("Window height: %0d\r\n", ReadCameraRegister(0xAA));
		xil_printf("Output Register1: %x\r\n", ReadCameraRegister(0x08));
		xil_printf("Output Register2: %x\r\n", ReadCameraRegister(0x3A));
		//DisplayTestPattern(XPAR_PLB_VISION_0_BASEADDR);
		//DisableTestPattern(XPAR_PLB_VISION_0_BASEADDR);
	}	
else if (DAUGHTER_BOARD == DAUGHTER_BOARD_GAME)
	{
		//EnableColorCorrection();
		DisableColorCorrection();
		WriteCameraRegister(0x04, FRAME_WIDTH); //set window width
		WriteCameraRegister(0x03, FRAME_HEIGHT); //set window height		

		// Set FPS
		/*
		 	 	 940 (0x3AC) = 30 fps
				 800 (0x320) = 32 fps
				 700 (0x2BC) = 35 fps
				 600 (0x258) = 38 fps
				 580 (0x244) = 38 fps
				 94  (0x05E) = 60 fps
		*/
		//WriteCameraRegister(0x05, 0x3AC);  //3AC set Horizontal blanking to 940(30 FPS)
		WriteCameraRegister(0x05, 0x05E);  //05E set Horizontal blanking to 94 (60 FPS)

		/********** COLOR MODE *******************/
		WriteCameraRegister(0x0F, 0x02);	//set sensor to default color mode
		//WriteCameraRegister(0x0F, 0x15); 	//set sensor to color & no high dynamic range
		//WriteCameraRegister(0x0F, 0x55);	//set sensor to color & high dynamic range
		//WriteCameraRegister(XPAR_PLB_VISION_0_BASEADDR, 0x0A, 0x0022);	//Control of 2-Knee High Dynamic range 
		//WriteCameraRegister(XPAR_PLB_VISION_0_BASEADDR, 0xC2, 0x0940);	// Enable .75 attenuation of pixel voltage before Analog-to-Digital conversion


		/********** AUTO EXPOSURE/AUTO GAIN CONTROL **********/	
		WriteCameraRegister(0xAF, 0x03); //turn on auto exposure and auto gain (Default)
		//WriteCameraRegister(XPAR_PLB_VISION_0_BASEADDR, 0xAF, 0x02); //turn off auto exposure 
		//WriteCameraRegister(XPAR_PLB_VISION_0_BASEADDR, 0xAF, 0x01); //turn off auto gain
		//WriteCameraRegister(XPAR_PLB_VISION_0_BASEADDR, 0xAF, 0x00); //turn off both
		/********** ANALOG GAIN  ****************/
		//WriteCameraRegister(XPAR_PLB_VISION_0_BASEADDR, 0x35, 0x10); //increase/decrease analog gain (default 1x = 0x10)
	
		//WriteCameraRegister(XPAR_PLB_VISION_0_BASEADDR, 0x47, 0x8081); //override black level calibration
		//WriteCameraRegister(XPAR_PLB_VISION_0_BASEADDR, 0x48, 0x007E); //black level calibration value (override only) max
		//WriteCameraRegister(XPAR_PLB_VISION_0_BASEADDR, 0x48, 0x0082); //black level calibration value (override only) min
		
	   /********** IMAGE MIRRORING *************/
		//WriteCameraRegister(XPAR_PLB_VISION_0_BASEADDR,0x0D, 0x310); //mirror the rows (flip the image vertically)
		//WriteCameraRegister(XPAR_PLB_VISION_0_BASEADDR,0x0D, 0x0330); //mirror the rows (flip the image vertically & horrizontally)

		//WriteCameraRegister(0x70, 0x0014); // Test Pattern: Disable Noise Correction
		//WriteCameraRegister(0x7F, 0x3000); // Test Pattern: Vertical shades 
		//DisableTestPattern();
		xil_printf("Camera Version %x, Window Size: %dx%d, Output Format: %x  (0x0 = Monochrome, 0x1 = Color)\r\n", 
			ReadCameraRegister(0xFF),
			ReadCameraRegister(0x04),
			ReadCameraRegister(0x03),
			ReadCameraRegister(0x0F)
			);
	}
}
