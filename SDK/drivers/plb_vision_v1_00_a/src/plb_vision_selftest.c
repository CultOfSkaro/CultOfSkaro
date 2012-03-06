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
/************************** Constant Definitions ***************************/


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
	static FrameTableEntry* frame = NULL;

	CPU_MSR msr = DISABLE_INTERRUPTS();

	//check in and out frames
	if (frame != NULL) {
		FT_CheckInFrame(frame);
	}
	frame = FT_CheckOutFrame();

	//acknowledge interrupts so the camera can start the next capture
	XIntc_AckIntr(XPAR_INTC_SINGLE_BASEADDR, XPAR_PLB_VISION_0_INTERRUPT_MASK);
	FT_InterruptHandlerFrameTable();

	if (frame == NULL) {
		print("Null frame!\r\n");
	} else {
		uint32* bufAddr = frame->frame_address[VISION_FRAME_RGB565]->data.data32;
		/*
		int bufSize = frame->frame_address[VISION_FRAME_RGB565]->capacity;
		xil_printf("Frame id:%d\r\n", frame->id);
		xil_printf("Frame addr: %08x", bufAddr);
		xil_printf("Frame size: %d\r\n", bufSize);
		*/

		xil_printf("Writing frame over usb: %d\r\n", frame->id);
		while(!USB_writeReady());
		USB_blockWrite((u32*)bufAddr,307200);
	}

	RESTORE_INTERRUPTS(msr);
}

int main()
{
	print("-------------------------Camera Stream App------------------------\r\n");


	print("Calibrating Memory Controller...\r\n");
	XCache_DisableDCache();
	XCache_DisableICache();
	MpmcCalibrationExample(XPAR_MPMC_0_DEVICE_ID);
	XCache_EnableICache(0x80000000);
	XCache_EnableDCache(0x80000000);

	print("Initializing Memory Buffers...\r\n");
	MemAllocInit((uint32*)0x100000);	//root of the DDR (can't use 0, that's NULL)
	BSInit();

	print("Initializing USB...\r\n");
	USB_init();
	print("Waiting for USB...");
	while (!USB_writeReady());
	USB_setBurstSize(128);
	print("ready\r\n");

	print("Setting up interrupts\r\n");
	//Enable exceptions
	XExc_Init();
	XExc_RegisterHandler( XEXC_ID_NON_CRITICAL_INT, (XExceptionHandler)XIntc_DeviceInterruptHandler, (void*)XPAR_XPS_INTC_0_DEVICE_ID);
	XExc_mEnableExceptions(XEXC_NON_CRITICAL);
	XIntc_RegisterHandler( XPAR_XPS_INTC_0_BASEADDR, XPAR_XPS_INTC_0_PLB_VISION_0_INTERRUPT_INTR, (XInterruptHandler)CameraISR, (void*)NULL);
	XIntc_EnableIntr( XPAR_XPS_INTC_0_BASEADDR, XPAR_PLB_VISION_0_INTERRUPT_MASK);
	XIntc_MasterEnable( XPAR_XPS_INTC_0_BASEADDR );
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
	
	//xil_printf("Frame capture started, writing to memory at 0x%x\r\n", MEMORY_ADDRESS);
	print("Waiting for interrupt...\r\n");

	//endless loop
	while(1);

	xil_printf("Finished!\r\n");
	return 0;
}
