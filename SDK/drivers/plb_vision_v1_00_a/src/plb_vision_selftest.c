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

//#define PLB_VISION_SELFTEST_BUFSIZE  512 /* Size of buffer (for transfer test) in bytes */

/************************** Variable Definitions ****************************/
int frameCount = 0;

//static Xuint8 __attribute__((aligned (128))) SrcBuffer[PLB_VISION_SELFTEST_BUFSIZE];   /* Source buffer      */
//static Xuint8 __attribute__((aligned (128))) DstBuffer[PLB_VISION_SELFTEST_BUFSIZE];   /* Destination buffer */

/************************** Function Definitions ***************************/
/* Camera ISR */
typedef Xuint32 CPU_MSR;
#define DISABLE_INTERRUPTS() ({ Xuint32 msr; \
											asm volatile ("mfmsr %0" : "=r" (msr)); \
											asm volatile ("wrteei 0"); \
											msr; });
#define RESTORE_INTERRUPTS(MSR) asm volatile ("wrtee %0" :: "r" (MSR));

#define MEMORY_ADDRESS 0x81d00000//0x007A2558 //XPAR_MPMC_0_MPMC_BASEADDR
#define MEMORY_ADDRESS2 0x00838558

/*
typedef struct {
	Xuint8 magic; //0x00BE
	Xuint8 type;
	Xuint16 checksum_header;
	Xuint16 checksum_data;
	Xuint32 bufferSize;
	Xuint16 packed;
} __attribute__((__packed__)) HeliosCommHeader;
	char buffer[10];	


unsigned x = 0;
unsigned y = 0;
volatile char buf[11];
void IntToStr(Xuint8 * integer, Xuint8 len)
{
	Xuint32 temp;
	int i = 0;
	for(;i < 10; i++)
		buf[i] = 0x30;
	buf[10] = '\0';
	// Remember, it is big endian
	if(len == 1)//Xuint8 only three places
	{
		temp = (Xuint32) *integer;
		buf[9] = (char) (temp % 10 + 0x30);
		buf[8] = (char) ((temp % 100)/10 + 0x30);
		buf[7] = (char) (temp/100 + 0x30);
	}
	else if(len == 2)//Xuint16 only 5 places
	{
		temp = (Xuint32) *((Xuint16*)integer);
		buf[9] = (char) (temp % 10 + 0x30);
		buf[8] = (char) ((temp % 100)/10 + 0x30);
		buf[7] = (char) ((temp % 1000)/100 + 0x30);
		buf[6] = (char) ((temp % 10000)/1000 + 0x30);
		buf[5] = (char) (temp/10000 + 0x30);
	}
	else//Xuint32 only 10 places
	{
		temp = *((Xuint32*)integer);
		buf[9] = (char) (temp % 10 + 0x30);
		buf[8] = (char) ((temp % 100)/10 + 0x30);
		buf[7] = (char) ((temp % 1000)/100 + 0x30);
		buf[6] = (char) ((temp % 10000)/1000 + 0x30);
		buf[5] = (char) ((temp % 100000)/10000 + 0x30);
		buf[4] = (char) ((temp % 1000000)/100000 + 0x30);
		buf[3] = (char) ((temp % 10000000)/1000000 + 0x30);
		buf[2] = (char) ((temp % 100000000)/10000000 + 0x30);
		buf[1] = (char) ((temp % 1000000000)/100000000 + 0x30);
		buf[0] = (char) (temp/1000000000 + 0x30);
	}
}
*/

void CameraISR()
{
	//Get IntC Status Register
	//Xuint32 status = XIntc_GetIntrStatus(XPAR_INTC_SINGLE_BASEADDR);
	//confirm that this is the Camera Interrupt
	/*
	if((status & XPAR_PLB_VISION_0_INTERRUPT_MASK) != XPAR_PLB_VISION_0_INTERRUPT_MASK){
		xil_printf("Interrupt was not camera interrupt\r\n");
		return;
	}
	*/

	frameCount++;

	CPU_MSR msr = DISABLE_INTERRUPTS();
	FrameTableEntry* frame = FT_CheckOutFrame();
	Xuint32 add = GetFrameAddress(VISION_FRAME_RGB565);

	xil_printf("New contents of memory location 0x%x:\r\n", MEMORY_ADDRESS);
	xil_printf("P3\r\n640 480\r\n255\r\n");
	USB_setBurstSize(128);
	while(!USB_writeReady());

	//Buffer * b = frame->frame_address[VISION_FRAME_RGB565];
	//Xuint32 add = (b->data.data32);//GetFrameAddress(VISION_FRAME_RGB565) - 0x6A000;
	xil_printf("0x%x\r\n", frame->frame_address_end[VISION_FRAME_RGB565]);
	xil_printf("%x  %d\r\n", add, frame->id);

	USB_blockWrite(0,307200);
	FT_CheckInFrame(frame);

	//xil_printf("Done!");
	//xil_printf("%d\r", frameCount);
	
	XIntc_AckIntr(XPAR_INTC_SINGLE_BASEADDR, XPAR_PLB_VISION_0_INTERRUPT_MASK);
	RESTORE_INTERRUPTS(msr);
	FT_InterruptHandlerFrameTable();
}

int main()
{
	print("----Camera Stream App----\r\n");
	USB_init();
	print("Waiting for USB...");
	while (!USB_writeReady());
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
	print("Calibrating SDRAM...\r\n");
	
	XCache_DisableDCache();
	XCache_DisableICache();
	MpmcCalibrationExample(XPAR_MPMC_0_DEVICE_ID);
	XCache_EnableICache(0x80000000);
	XCache_EnableDCache(0x80000000);

	
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
	print("Erasing SRAM\r\n");
	int i = 0;
	for(i = 0; i < (640*480)/2; i++)
	{
		XIo_Out32(MEMORY_ADDRESS + i, 0x0);
	}
	
	SetFrameSaveOptions(1);
	//SetFrameSaveOptions(VISION_SAVE_OPTIONS_CAM_BYTES_ALL);
	xil_printf("Frame save options: 0x%x\r\n", GetFrameSaveOptions);
	
	Xuint32 stat = CameraStatus();
	xil_printf("Status: 0x%x\r\n", stat);
	
	//wait a bit
	volatile int a;
	for(i=0;i<100000;i++){
		a += i;
	}

	//SetFrameAddress(VISION_FRAME_RGB565, MEMORY_ADDRESS);
	stat = GetFrameAddress(VISION_FRAME_RGB565);
	xil_printf("Frame address: 0x%x\r\n", stat);
	print("Starting capture\r\n");
	FT_Init();
	FT_StartCapture(g_frametable[0]);

	//StartFrameCapture();
	
	xil_printf("Frame capture started, writing to memory at 0x%x\r\n", MEMORY_ADDRESS);
	print("Waiting for interrupt...");

	//endless loop
	while(1);

	xil_printf("Finished!\r\n");
	return 0;
}
