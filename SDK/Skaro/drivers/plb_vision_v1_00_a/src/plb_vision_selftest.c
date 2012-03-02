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
#include <xcache_l.h>
#include <xexception_l.h>
#include <xutil.h>
#include <xio.h>
#include <plb_vision.h>
#include <xparameters.h>
#include <USB_IO.h>
#include <mpmc_calibration.h>
/************************** Constant Definitions ***************************/

#define PLB_VISION_SELFTEST_BUFSIZE  512 /* Size of buffer (for transfer test) in bytes */

/************************** Variable Definitions ****************************/

static Xuint8 __attribute__((aligned (128))) SrcBuffer[PLB_VISION_SELFTEST_BUFSIZE];   /* Source buffer      */
static Xuint8 __attribute__((aligned (128))) DstBuffer[PLB_VISION_SELFTEST_BUFSIZE];   /* Destination buffer */

/************************** Function Definitions ***************************/
int gotInt = 0;
int frameCount = 0;
/* Camera ISR */
typedef Xuint32 CPU_MSR;
#define DISABLE_INTERRUPTS() ({ Xuint32 msr; \
											asm volatile ("mfmsr %0" : "=r" (msr)); \
											asm volatile ("wrteei 0"); \
											msr; });
#define RESTORE_INTERRUPTS(MSR) asm volatile ("wrtee %0" :: "r" (MSR));

#define MEMORY_ADDRESS 0x81d00000//0x007A2558 //XPAR_MPMC_0_MPMC_BASEADDR
#define MEMORY_ADDRESS2 0x00838558
typedef struct {
	Xuint8 magic; //0x00BE
	Xuint8 type;
	Xuint16 checksum_header;
	Xuint16 checksum_data;
	Xuint32 bufferSize;
	Xuint16 packed;
} __attribute__((__packed__)) HeliosCommHeader;
	char buffer[10];	

void SerialISR()
{
	Xuint32 status = XIntc_GetIntrStatus(XPAR_INTC_SINGLE_BASEADDR);
	print("pong\n\r");
}
void CameraISR()
{
	//Get IntC Status Register
	Xuint32 status = XIntc_GetIntrStatus(XPAR_INTC_SINGLE_BASEADDR);
	//confirm that this is the Camera Interrupt
	if(status & XPAR_PLB_VISION_0_INTERRUPT_MASK != XPAR_PLB_VISION_0_INTERRUPT_MASK){
		xil_printf("Interrupt was not camera interrupt\r\n");
		return;
	}
	if(gotInt == 1)
		return;
	frameCount++;
	CPU_MSR msr = DISABLE_INTERRUPTS();
	Xuint32 result;
	int i;
	//xil_printf("Got an interrupt!\r\n");
	if(frameCount == 100)
	{
	xil_printf("New contents of memory location 0x%x:\r\n", MEMORY_ADDRESS);
	xil_printf("P3\r\n640 480\r\n255\r\n");
	print("Here is the data\r\n");

	char c;
	for(i=0; i < (640*480)/2; i++)
	{
		/*
		if (i % 100 == 0) {
			read(0,&c,1);
		}
		 */

		result = XIo_In32((Xuint32*)(MEMORY_ADDRESS) + i);
		xil_printf("%x\r\n", result );

		//Xuint16 bit1 = (result >> 16)&0x0000FFFF;
		//Xuint16 bit2 = result & 0x0000FFFF;
		//Xuint8 r = ((bit1>>11)&0x1F)<<3;
		//Xuint8 g = ((bit1>>5)&0x3F)<<2;
		//Xuint8 b = (bit1 & 0x1F)<<3;
		//xil_printf("%d\r\n%d\r\n%d\r\n", r, g, b);
		//xil_printf("%d %d %d\r\n", r, g, b);
		//r = ((bit2>>11)&0x1F)<<3;
		//g = ((bit2>>5)&0x3F)<<2;
		//b = (bit2 & 0x1F)<<3;
		//xil_printf("%d %d %d\r\n", r, g, b);
		//xil_printf("%d\r\n%d\r\n%d\r\n", r, g, b);
	}
	xil_printf("Done!");
	gotInt = 1;
	return;
	}

	//xil_printf("%d\r", frameCount);
	
	if(frameCount == 99)
	  {
	    SetFrameAddress(VISION_FRAME_RGB565, MEMORY_ADDRESS);
	  }
	StartFrameCapture();
	//Clear interrupt #1 flag
	XIntc_AckIntr(XPAR_INTC_SINGLE_BASEADDR, "XPAR_PLB_VISION_0_INTERRUPT_MASK");
	//gotInt = 1;
	RESTORE_INTERRUPTS(msr);
}

int main()
{
	Xuint32 data = 0;
	int i = 0;
	gotInt = 0;
	Xuint32 stat;
	volatile int a;
	xil_printf("----Camera Test App----\r\n");
	/*USB_init();
	print("Testing status of USB\r\n");
	while (!USB_writeReady());
	print("TEST write ready : PASSED\r\n\r\n");
	*/
	xil_printf("Welcome to Helios\r\n");
	xil_printf("Setting up interrupts\r\n");
	XExc_Init();
	//Enable exceptions
	XExc_RegisterHandler( XEXC_ID_NON_CRITICAL_INT, (XExceptionHandler)XIntc_DeviceInterruptHandler, (void*)XPAR_XPS_INTC_0_DEVICE_ID);
	XExc_mEnableExceptions(XEXC_NON_CRITICAL);
	XIntc_RegisterHandler( XPAR_XPS_INTC_0_BASEADDR, XPAR_XPS_INTC_0_PLB_VISION_0_INTERRUPT_INTR, (XInterruptHandler)CameraISR, (void*)NULL);
	XIntc_EnableIntr( XPAR_XPS_INTC_0_BASEADDR, XPAR_PLB_VISION_0_INTERRUPT_MASK);
	XIntc_RegisterHandler( XPAR_XPS_INTC_0_BASEADDR, XPAR_XPS_INTC_0_WIRELESS_UART_INTERRUPT_INTR, (XInterruptHandler)SerialISR, (void*)NULL);
	XIntc_EnableIntr( XPAR_XPS_INTC_0_BASEADDR, XPAR_WIRELESS_UART_INTERRUPT_MASK);
	XIntc_MasterEnable( XPAR_XPS_INTC_0_BASEADDR );
	xil_printf("Done!\r\n");
	xil_printf("Calibrating SDRAM...\r\n");
	while(1){};
   XCache_DisableDCache();
   XCache_DisableICache();	
	MpmcCalibrationExample(XPAR_MPMC_0_DEVICE_ID);
   XCache_EnableICache(0x80000000);
   XCache_EnableDCache(0x80000000);
	
	xil_printf("Testing camera serial interface\r\n");
	
	/* SET UP CAMERA */
	Init_Camera();
	ResetCameraSensor();
//	xil_printf("Selecting Core register set\r\n");
//	SelectSensorRegisterBank();
	data = ReadCameraRegister(0xFF);
	xil_printf("Camera version: %0x\r\n", data);

	data = ReadCameraRegister(0x04);
	xil_printf("Window width: %0d\r\n", data);
	data = ReadCameraRegister(0x03);
	xil_printf("Window height: %0d\r\n", data);
//	SelectIPFRegisterBank();
//	xil_printf("Output Register1: %x", ReadCameraRegister(0x08));
//	xil_printf("Output Register2: %x", ReadCameraRegister(0x3A));
//	WriteCameraRegister(0x08,0xD800); //set output to RGB
//	WriteCameraRegister(0x3A,0x00);  //set output to RGB565
//	xil_printf("Output Register1: %x", ReadCameraRegister(0x08));
//	xil_printf("Output Register2: %x", ReadCameraRegister(0x3A));
	
	//DisplayTestPattern(XPAR_PLB_VISION_0_BASEADDR);
	//DisableTestPattern(XPAR_PLB_VISION_0_BASEADDR);
	/****************/
	xil_printf("Done!\r\n");
	xil_printf("Testing camera PLB interface\r\n");
	xil_printf("Saving images to SRAM\r\n");
	xil_printf("Erasing SRAM\r\n");
	int Status;
	u32 Index;
	u32 result = 0;
	for(i = 0; i < (640*480)/2; i++)
	{
		XIo_Out32((Xuint32*)MEMORY_ADDRESS + i, 0x0);
	}
	/*xil_printf("Current contents of memory location 0x00000000:\r\n");	
	for(i=0; i < 128; i++)
	{
		result = XIo_In32(XPAR_XPS_MCH_EMC_SRAM_MEM0_BASEADDR + i);
		xil_printf("0x%x 0x%x 0x%x 0x%x ", 0xF&(result>>24), 0xF&(result>>16), 0xF&(result>>8), 0xF&result );
	}
	*/
	xil_printf("Starting capture\r\n");
	
	SetFrameSaveOptions(1);
	//SetFrameSaveOptions(VISION_SAVE_OPTIONS_CAM_BYTES_ALL);
	xil_printf("Frame save options: 0x%x\r\n", GetFrameSaveOptions);
	
	stat = CameraStatus();
	xil_printf("Status: 0x%x\r\n", stat);
	
	
	for(i=0;i<100000;i++)
	{ //wait
		a += i;
	}
	SetFrameAddress(VISION_FRAME_RGB565, MEMORY_ADDRESS);
	stat = GetFrameAddress(VISION_FRAME_RGB565);
	xil_printf("Frame address: 0x%x\r\n", stat);
	StartFrameCapture();
	
	xil_printf("Frame capture started, writing to memory at 0x%x\r\n", MEMORY_ADDRESS);
	xil_printf("Waiting for interrupt...");
	gotInt = 0;
	
	
	while(!gotInt)
	{
		stat = CameraStatus();
		//xil_printf("Status: 0x%x\r\n", stat);
		for(i=0;i<100000;i++)
		{ //wait
			a += i;
		}
		if(stat == 0xD1) //capture done but FIFO not empty
		{
			for(i=0;i<100000;i++) //make sure no burst is going on currently
			{ //wait
				a += i;
			}
			//xil_printf("Status: 0x%x\r\n", stat);
			ResetCameraCore();
			StartFrameCapture();
		}
	}
	xil_printf("Finished!\r\n");
	return 0;
}
