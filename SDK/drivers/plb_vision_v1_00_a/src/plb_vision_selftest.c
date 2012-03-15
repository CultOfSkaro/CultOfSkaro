/*****************************************************************************
* Filename:          /data/projects/SpencerBranch/drivers/plb_vision_v1_00_a/src/plb_vision_selftest.c
* Version:           1.00.a
* Description:       Contains a diagnostic self-test function for the plb_vision driver
* Date:              Mon Feb  8 12:51:11 2010 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/
#include <stdio.h>
#include <xbasic_types.h>
#include <xintc_l.h>
#include <xexception_l.h>
#include <xutil.h>
#include <xio.h>
#include <xparameters.h>
#include <USB_IO.h>
#include <mpmc_calibration.h>
#include "plb_vision.h"
#include "FrameTable.h"
#include "State.h"

#include "xtime_l.h"

/************************** Constant Definitions ***************************/

#define PIT_PER_SECOND 		1000
#define PIT_INTERVAL 		(XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ / PIT_PER_SECOND)

/************************** Variable Definitions ****************************/

/************************** Function Definitions ***************************/
typedef Xuint32 CPU_MSR;
#define DISABLE_INTERRUPTS() ({ Xuint32 msr; \
											asm volatile ("mfmsr %0" : "=r" (msr)); \
											asm volatile ("wrteei 0"); \
											msr; });
#define RESTORE_INTERRUPTS(MSR) asm volatile ("wrtee %0" :: "r" (MSR));

void CameraISR()
{
	CPU_MSR msr = DISABLE_INTERRUPTS();
	//acknowledge interrupts so the camera can start the next capture
	XIntc_AckIntr(XPAR_INTC_SINGLE_BASEADDR, XPAR_PLB_VISION_0_INTERRUPT_MASK);
	FT_InterruptHandlerFrameTable();
	RESTORE_INTERRUPTS(msr);
}

void pitHandler(){
	CPU_MSR msr;

	msr = DISABLE_INTERRUPTS();
	{
		M_uint[U_MS]++;
	}
	RESTORE_INTERRUPTS(msr);

	XTime_PITClearInterrupt();
}

/*
 * Header
 * bytes  description
 * ------------------
 * 64     0's
 * 64     255's
 * 4      size of total transmission - 128 (in bytes)
 * 4      size of frame (in bytes)
 * 4      size of detection data (in bytes)
 * *      frame
 * *      detection data
 */

//char headerBuf[64 + 64 + 4 + 4 + 4];
//char headerBuf[512];
char *headerBuf = (char*)0x10000;
int *transmitSize = (int*)(0x10000 + 64 + 64);
int *frameSize = (int*)(0x10000 + 64 + 64 + 4);
int *detectionSize = (int*)(0x10000 + 64 + 64 + 4 + 4);

void initHeader() {
	int i;
	for (i = 0; i < 64; i++) {
		headerBuf[i] = 0;
	}
	for (i = 64; i < 64 + 64; i++) {
		headerBuf[i] = 255;
	}
}

void transmitFrame(FrameTableEntry* frame) {
	uint32* bufAddr = frame->frame_address[VISION_FRAME_RGB565]->data.data32;
	int bufSize = frame->frame_address[VISION_FRAME_RGB565]->capacity;

	*frameSize = bufSize;
	*detectionSize = 0;
	*transmitSize = *frameSize + *detectionSize + 8;

	/*
	xil_printf("Frame id:%d\r\n", frame->id);
	xil_printf("Frame addr: %08x", bufAddr);
	xil_printf("Frame size: %d\r\n", bufSize);
	*/
	xil_printf("Transmit size: %d\r\n", 140 / 2);
	int i, j;
	for (i = 0; i < 140; i++) {
		for (j = 0; j < 8 && i < 140; i++, j++) {
			xil_printf("%d ", headerBuf[i]);
		}
		print("\r\n");
	}

	print("Writing header over usb...");
	while(!USB_writeReady());
	USB_blockWrite((u32*)headerBuf, 140 / 2);
	while(!USB_writeReady());
	print("Done\r\n");

	xil_printf("Writing frame over usb: %d...", frame->id);
	while(!USB_writeReady());
	USB_blockWrite((u32*)bufAddr, bufSize / 2);
	while(!USB_writeReady());
	print("Done\r\n");
}

void processFrame(FrameTableEntry* frame) {
	transmitFrame(frame);
}

int main()
{
	print("-------------------------Camera Stream App------------------------\r\n");


	print("Calibrating Memory Controller...\r\n");
	XCache_DisableDCache();
	XCache_DisableICache();
	MpmcCalibrationExample(XPAR_MPMC_0_DEVICE_ID);
	XCache_EnableICache(0x00000001);
	XCache_EnableDCache(0x00000001);

	print("Initializing Memory Buffers...\r\n");
	MemAllocInit((uint32*)0x100000);
	BSInit();
	STInit();

	//init header
	initHeader();

	print("Initializing USB...\r\n");
	USB_init();
	print("Waiting for USB...");
	while (!USB_writeReady());
	USB_setBurstSize(128);
	print("ready\r\n");

	print("Setting up interrupts\r\n");
	XExc_Init();
	XExc_RegisterHandler( XEXC_ID_NON_CRITICAL_INT, (XExceptionHandler)XIntc_DeviceInterruptHandler, (void*)XPAR_XPS_INTC_0_DEVICE_ID);
	XExc_mEnableExceptions(XEXC_NON_CRITICAL);
	XIntc_RegisterHandler( XPAR_XPS_INTC_0_BASEADDR, XPAR_XPS_INTC_0_PLB_VISION_0_INTERRUPT_INTR, (XInterruptHandler)CameraISR, (void*)NULL);
	XIntc_EnableIntr( XPAR_XPS_INTC_0_BASEADDR, XPAR_PLB_VISION_0_INTERRUPT_MASK);
	XIntc_MasterEnable( XPAR_XPS_INTC_0_BASEADDR );

	//PIT
	XExc_RegisterHandler(XEXC_ID_PIT_INT, &pitHandler, 0);
	XTime_PITEnableAutoReload();
	XTime_PITSetInterval(PIT_INTERVAL);
	XExc_mEnableExceptions(XEXC_ALL);
	XTime_PITEnableInterrupt();
	XTime_PITClearInterrupt();
	print("Done!\r\n");

	/* SET UP CAMERA */
	Init_Camera();
	ResetCameraSensor();
	print("Testing camera serial interface\r\n");
	Xuint32 data = 0;
	data = ReadCameraRegister(0xFF);
	xil_printf("Camera version: %0x\r\n", data);
	data = ReadCameraRegister(0x04);
	xil_printf("Window width: %0d\r\n", data);
	data = ReadCameraRegister(0x03);
	xil_printf("Window height: %0d\r\n", data);
	print("Done!\r\n");

	SetFrameSaveOptions(1);
	//SetFrameSaveOptions(VISION_SAVE_OPTIONS_CAM_BYTES_ALL);
	xil_printf("Frame save options: 0x%x\r\n", GetFrameSaveOptions);
	
	Xuint32 stat = CameraStatus();
	xil_printf("Status: 0x%x\r\n", stat);
	
	//wait a bit
	volatile int a;
	int i = 0;
	for(i=0;i<100000;i++){
		a += i;
	}

	print("Starting capture\r\n");
	FT_Init();
	FT_StartCapture(g_frametable[0]);
	
	//endless loop
	while(1) {
		FrameTableEntry* frame = NULL;

		//process frames
		frame = FT_CheckOutFrame();
		if (frame == NULL) {
			//print("Null frame!\r\n");
		} else {
			processFrame(frame);
			FT_CheckInFrame(frame);

			xil_printf("Process Rate: %d\r\n", rduint(U_FPS));
			xil_printf("Camera Rate: %d\r\n", rduint(U_FPS_CAM));
		}
	}

	xil_printf("Finished!\r\n");
	return 0;
}
