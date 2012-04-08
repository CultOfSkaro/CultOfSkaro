/***************************** Include Files *******************************/
//Standard C Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Math.h>

//Xilinx Libraries
#include "xbasic_types.h"
#include "xintc_l.h"
#include "xexception_l.h"
#include "xutil.h"
#include "xio.h"
#include "xparameters.h"
#include "xtime_l.h"
#include "sleep.h"

//Custom Includes
#include "USB_IO.h"
#include "plb_vision.h"
#include "FrameTable.h"
#include "State.h"
#include "Stack.h"
#include "CrossCore.h"


/************************** Type Definitions ***************************/

typedef struct {
	int h;
	int s;
	int v;
} Color;

/************************** Constant Definitions ***************************/

#define PIT_PER_SECOND 		1000
#define PIT_INTERVAL 		(XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ / PIT_PER_SECOND)

#define HEADER_LOC     0x1000
#define HEADER_SIZE    (64 + 64 + 4 + 4 + 4)

#define NUM_BLOBS_LOC  (HEADER_LOC + HEADER_SIZE)
#define BLOB_LOC       (NUM_BLOBS_LOC + 4)
#define BLOB_DATA_LOC  NUM_BLOBS_LOC
#define BLOB_DATA_SIZE (4 + sizeof(Blob) * MAX_BLOBS)

#define FILL_STACK_LOC      (BLOB_DATA_LOC + BLOB_DATA_SIZE)
#define FILL_STACK_SIZE     (40000 * 4)
#define IMAGE_WIDTH         640
#define IMAGE_HEIGHT        480
#define CHECKED_LOC         0x81D00000 //root of the SRAM //BLOB_DATA_LOC + BLOB_DATA_SIZE
#define CHECKED_SIZE        (IMAGE_WIDTH * IMAGE_HEIGHT)

#define FRAME_LOC			(CHECKED_LOC + CHECKED_SIZE)

#define SEARCH_STRIDE_X  1
#define SEARCH_STRIDE_Y  1

#define FLOOD_STRIDE_X   1
#define FLOOD_STRIDE_Y   4

#define TARGET_COLOR_RED		0, 0, 10
#define TARGET_COLOR_PINK		40, 0, 10
#define TARGET_COLOR_BLUE       28, 0, 10
#define TARGET_COLOR_CYAN		23, 0, 10
#define TARGET_COLOR_GREEN		14, 0, 10
#define TARGET_COLOR_YELLOW		7, 0, 10
Color targetColors[NUM_BLOB_TYPES];

#define HUE_TOLERANCE_RED    2
#define HUE_TOLERANCE_PINK   3
#define HUE_TOLERANCE_BLUE   2
#define HUE_TOLERANCE_CYAN   3
#define HUE_TOLERANCE_GREEN  2
#define HUE_TOLERANCE_YELLOW 3
int HUE_TOLERANCES[NUM_BLOB_TYPES];

// Indices are in terms of 10's of pixels in the Y direction
//                                0    1    2    3    4    5    6    7    8    9
//                                10   11   12   13   14   15   16   17   18   19   20   21
char variableSearchStrideX[22] = {145, 140, 130, 125, 120, 110, 100, 95 , 90 , 80 ,
		                          75 , 70 , 60 , 50 , 45 , 35 , 30 , 10 , 5  , 5  , 5  , 5};
char variableSearchStrideY[22] = {10 , 10 , 10 , 10 , 10 , 10 , 10 , 10 , 10 , 10 ,
		                          5  , 5  , 5  , 5  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1};

/************************** Variable Definitions ****************************/

//Used during flood Fill
Stack toCheckMem;
Stack* toCheck = &toCheckMem;

//Stores detected blobs
VisionData *visionData;


//For transmitting over usb
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
char *headerBuf = (char*)HEADER_LOC;
int *transmitSize = (int*)(HEADER_LOC + 64 + 64);
int *frameSize = (int*)(HEADER_LOC + 64 + 64 + 4);
int *detectionSize = (int*)(HEADER_LOC + 64 + 64 + 4 + 4);

/************************** Function Declarations ***************************/
//void processFrame(ushort *pixels);

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

void initHeader() {
	int i;
	for (i = 0; i < 64; i++) {
		headerBuf[i] = 0;
	}
	for (i = 64; i < 64 + 64; i++) {
		headerBuf[i] = 255;
	}

	srand(12345621);
}

void initTargetColors() {
	targetColors[BLOB_TYPE_BLUE] = (Color){TARGET_COLOR_BLUE};
	targetColors[BLOB_TYPE_PINK] = (Color){TARGET_COLOR_PINK};
	targetColors[BLOB_TYPE_YELLOW] = (Color){TARGET_COLOR_YELLOW};
	targetColors[BLOB_TYPE_RED] = (Color){TARGET_COLOR_RED};
	targetColors[BLOB_TYPE_CYAN] = (Color){TARGET_COLOR_CYAN};
	targetColors[BLOB_TYPE_GREEN] = (Color){TARGET_COLOR_GREEN};

	HUE_TOLERANCES[BLOB_TYPE_BLUE] = HUE_TOLERANCE_BLUE;
	HUE_TOLERANCES[BLOB_TYPE_PINK] = HUE_TOLERANCE_PINK;
	HUE_TOLERANCES[BLOB_TYPE_YELLOW] = HUE_TOLERANCE_YELLOW;
	HUE_TOLERANCES[BLOB_TYPE_RED] = HUE_TOLERANCE_RED;
	HUE_TOLERANCES[BLOB_TYPE_CYAN] = HUE_TOLERANCE_CYAN;
	HUE_TOLERANCES[BLOB_TYPE_GREEN] = HUE_TOLERANCE_GREEN;
}

void transmitFrame(FrameTableEntry* frame) {
	uint32* bufAddr = frame->frame_address[VISION_FRAME_RGB565]->data.data32;
	int bufSize = frame->frame_address[VISION_FRAME_RGB565]->capacity;

	//set up transmission sizes
	*frameSize = bufSize;
	*detectionSize = 4 + (sizeof(Blob) * visionData->numBlobs);
	*transmitSize = *frameSize + *detectionSize + 8;

	//xil_printf("data size: %d\r\n", *detectionSize);

	/* Generates random blobs for testing purposes
	*numBlobs = rand() % MAX_BLOBS;
	int i;
	for (i = 0; i < *numBlobs; i++) {
		//blobs
		blobBuf[i].type = rand() % 10;
		blobBuf[i].left = rand() % 640;
		blobBuf[i].top = rand() % 480;
		blobBuf[i].width = rand() % (640 - blobBuf[i].left);
		blobBuf[i].height = rand() % (480 - blobBuf[i].top);
	}
	 */

	/*
	xil_printf("Frame id:%d\r\n", frame->id);
	xil_printf("Frame addr: %08x", bufAddr);
	xil_printf("Frame size: %d\r\n", bufSize);
	*/
	/*
	xil_printf("Transmit size: %d\r\n", 140 / 2);
	int i, j;
	for (i = 0; i < 140; i++) {
		for (j = 0; j < 8 && i < 140; i++, j++) {
			xil_printf("%d ", headerBuf[i]);
		}
		print("\r\n");
	}
	 */

	//print("Writing header over usb...");
	while(!USB_writeReady());
	USB_blockWrite((u32*)headerBuf, 140 / 2);
	while(!USB_writeReady());
	//print("Done\r\n");

	//xil_printf("Writing frame over usb: %d...", frame->id);
	while(!USB_writeReady());
	USB_blockWrite((u32*)((int)bufAddr + 4), bufSize / 2);
	while(!USB_writeReady());
	//print("Done\r\n");

	//print("Writing blobs over usb...");
	while(!USB_writeReady());
	USB_blockWrite((u32*)visionData, *detectionSize / 2);
	while(!USB_writeReady());
	//print("Done\r\n");
}


inline int colorMatch(short pixel, int targetColorIdx) {
	Color targetColor = targetColors[targetColorIdx];
	int hueTolerance = HUE_TOLERANCES[targetColorIdx];

	int h = (pixel >> 10) & 0x3F;
	//int s = (pixel >> 5) & 0x1F; //Currently not using saturation in color matching
	int v = pixel & 0x1F;

	return ((abs(h - targetColor.h) <= hueTolerance) && (v >= (targetColor.v)));
}

inline void* cTp(int x, int y) {
	return (void*)(x << 16 | y);
}
inline int pTx(void *p) {
	return ((int)p) >> 16;
}
inline int pTy(void *p) {
	return ((int)p) & 0xFFFF;
}

/*
Flood-fill (node, target-color, replacement-color):
 1. Set Q to the empty queue.
 2. Add node to the end of Q.
 4. While Q is not empty:
 5.     Set n equal to the last element of Q.
 7.     Remove last element from Q.
 8.     If the color of n is equal to target-color:
 9.         Set the color of n to replacement-color.
 10.        Add west node to end of Q.
 11.        Add east node to end of Q.
 12.        Add north node to end of Q.
 13.        Add south node to end of Q.
 14. Return.
 */

void floodFill(ushort *pixels, int xStart, int yStart, int blobType) {
	if (visionData->numBlobs >= MAX_BLOBS) return;

	StackClear(toCheck);
	StackPush(toCheck, cTp(xStart,yStart));
	int xMin = 100000;
	int xMax = 0;
	int yMin = 100000;
	int yMax = 0;

	while (!StackIsEmpty(toCheck)) {
		void* point = StackPop(toCheck);
		int x = pTx(point);
		int y = pTy(point);
		//xil_printf("x: %d, y:%d\r\n", x, y);

		if (pixels[y * IMAGE_WIDTH + x] == 0) continue;
		short pixel = pixels[y * IMAGE_WIDTH + x];


		if (colorMatch(pixel, blobType)) {
			pixels[y * IMAGE_WIDTH + x] = 0;
			if (x < xMin) xMin = x;
			if (x > xMax) xMax = x;
			if (y < yMin) yMin = y;
			if (y > yMax) yMax = y;

			if (x < IMAGE_WIDTH - FLOOD_STRIDE_X)
				StackPush(toCheck, cTp(x+FLOOD_STRIDE_X,y));
			if (x >= FLOOD_STRIDE_X)
				StackPush(toCheck, cTp(x-FLOOD_STRIDE_X,y));
			if (y < IMAGE_HEIGHT - FLOOD_STRIDE_Y)
				StackPush(toCheck, cTp(x,y+variableSearchStrideY[y/10]));
			if (y >= FLOOD_STRIDE_Y)
				StackPush(toCheck, cTp(x,y-variableSearchStrideY[y/10]));
		}
	}

	//create blob
	int width = xMax - xMin;
	int height = yMax - yMin;
	if (width > 0 && height > 0) {
		visionData->blobs[visionData->numBlobs].type = blobType;
		visionData->blobs[visionData->numBlobs].left = xMin;
		visionData->blobs[visionData->numBlobs].top = yMin;
		visionData->blobs[visionData->numBlobs].width = width;
		visionData->blobs[visionData->numBlobs].height = height;
		visionData->blobs[visionData->numBlobs].distance = 0;
		visionData->blobs[visionData->numBlobs].center = 0;
		visionData->blobs[visionData->numBlobs].angle = 0;
		visionData->blobs[visionData->numBlobs].valid = TRUE;

		(visionData->numBlobs)++;
	}
}

void processFrame(FrameTableEntry* frame) {
	ushort *pixels = frame->frame_address[VISION_FRAME_RGB565]->data.data16;

	//optimization : init the stack once
	StackInit(toCheck, (void**)FILL_STACK_LOC, FILL_STACK_SIZE);
	//StackInit(toCheck, stackMem, STACK_MEM_SIZE);

	visionData = Vision_GetBuffer(&vision);
	visionData->numBlobs = 0;

	//find blue blobs
	int x, y;
	for (y = 0; y < 200; y += variableSearchStrideY[y/10]) {
		for (x = 0; x < IMAGE_WIDTH; x += variableSearchStrideX[y/10]) {
			if (pixels[y * IMAGE_WIDTH + x] == 0) continue;
			short pixel = pixels[y * IMAGE_WIDTH + x];
			int i;
			for (i = 0; i < NUM_BLOB_TYPES; i++) {
				if (colorMatch(pixel, i)) {
					if (visionData->numBlobs >= MAX_BLOBS) continue;
					floodFill(pixels, x, y, i);
				}
			}
		}
	}

	//check below the blue blobs for red blobs

//	memset(pixelsChecked, 0, CHECKED_SIZE);
//	StackInit(toCheck, (void**)FILL_STACK_LOC, FILL_STACK_SIZE);
//	int i;
//	int max = visionData->numBlobs;
//	for (i = 0; i < max; i++) {
//		Blob *blob = &(visionData->blobs[i]);
//
//
//
//		/*
//		visionData->blobs[visionData->numBlobs].type = BLOB_TYPE_RED;
//		visionData->blobs[visionData->numBlobs].left = blob->left;
//		visionData->blobs[visionData->numBlobs].top = blob->top + blob->height;
//		visionData->blobs[visionData->numBlobs].width = blob->width;
//		visionData->blobs[visionData->numBlobs].height = blob->height;
//		(visionData->numBlobs)++;
//		*/
//
//		for (y = blob->top + blob->height; y < blob->top + blob->height * 4; y += 2) {
//			for (x = blob->left - blob->width / 2; x < blob->left + blob->width * 3 / 2; x += 2) {
//				short pixel = pixels[y * IMAGE_WIDTH + x];
//				if (colorMatch(pixel, targetColors[BLOB_TYPE_RED])) {
//					//if (visionData->numBlobs >= MAX_BLOBS) continue;
//					floodFill(pixels, x, y, targetColors[BLOB_TYPE_RED], BLOB_TYPE_RED);
//				}
//			}
//		}
//	}

	Vision_UpdateData(&vision,visionData);
#ifdef DEBUG_USB_VISION
	transmitFrame(frame);
#endif
}

int main()
{
	print("-------------------------Camera Stream App------------------------\r\n");

	//wait for other processor to run memory calibration
	usleep(100000);
	Vision_Init(&vision);
	print("Initializing Memory Buffers...\r\n");
	MemAllocInit((uint32*)0x300000);
	BSInit();
	STInit();

	//initialize constant memory
	initHeader();
	initTargetColors();
#ifdef DEBUG_USB_VISION
	print("Initializing USB...\r\n");
	// UNCOMMENT FOR USB VISION DATA
	USB_init();
	print("Waiting for USB...");
	while (!USB_writeReady());
	USB_setBurstSize(128);
	print("ready\r\n");
#endif
	print("Setting up interrupts\r\n");
	XExc_Init();
	XExc_RegisterHandler( XEXC_ID_NON_CRITICAL_INT, (XExceptionHandler)XIntc_DeviceInterruptHandler, (void*)XPAR_XPS_INTC_1_DEVICE_ID);
	XExc_mEnableExceptions(XEXC_NON_CRITICAL);
	XIntc_RegisterHandler( XPAR_XPS_INTC_1_BASEADDR, XPAR_XPS_INTC_1_PLB_VISION_0_INTERRUPT_INTR, (XInterruptHandler)CameraISR, (void*)NULL);
	XIntc_EnableIntr( XPAR_XPS_INTC_1_BASEADDR, XPAR_PLB_VISION_0_INTERRUPT_MASK);
	XIntc_MasterEnable( XPAR_XPS_INTC_1_BASEADDR );

	//PIT
	/*
	XExc_RegisterHandler(XEXC_ID_PIT_INT, &pitHandler, 0);
	XTime_PITEnableAutoReload();
	XTime_PITSetInterval(PIT_INTERVAL);
	XExc_mEnableExceptions(XEXC_ALL);
	XTime_PITEnableInterrupt();
	XTime_PITClearInterrupt();
	print("Done!\r\n");
	*/

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
	usleep(100000);

	print("Starting capture\r\n");


	FT_Init();
	FT_StartCapture(g_frametable[0]);

	//endless loop
	while(1) {
		FrameTableEntry* frame = NULL;

		//process frames
		//print("checking out frame\r\n");
		frame = FT_CheckOutFrame();
		if (frame == NULL) {
			//print("Null frame!\r\n");
		} else {
			processFrame(frame);
			FT_CheckInFrame(frame);

			//xil_printf("Process Rate: %d\r\n", rduint(U_FPS));
			//xil_printf("Camera Rate: %d\r\n", rduint(U_FPS_CAM));
		}
	}

	xil_printf("Finished!\r\n");


	//INFINITE LOOP
	while(1);

	return 0;
}
