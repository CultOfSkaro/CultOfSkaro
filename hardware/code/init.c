#include "init.h"
#include "HeliosIO.h"
#include "State.h"
#include "FrameTable.h"
#include "LoopScheduler.h"
#include "BufferStore.h"
#include "Comm.h"
#include "ISR.h"
#include <plb_vision.h>
#include <xparameters.h>
#include <xintc_l.h>
#include <xexception_l.h>
#include "mpmc_calibration.h"

/*  Initialize the computing platform 
 *	1: Initialize CPU Caches
 *	2: Initialize CPU interrupts
 *  3: Initialize software systems
 *  4: Initialize Camera
 *  5: Enable user cores
 *  6: Enable CPU interrupts
 */
 
extern Xuint8 DAUGHTER_BOARD;
void Init()
{
	int memory_test_results = 0; // bit 0 is 1 if the Data cache test was successful. bit 1 is 1 if the instruction cache test was succesful

	/****** SDRAM CALIBRATION CODE HERE *************/
	xil_printf("Calibrating SDRAM...\r\n");
	XCache_DisableDCache();
	XCache_DisableICache();	
	MpmcCalibrationExample(XPAR_MPMC_0_DEVICE_ID);
	XCache_EnableICache(0x80000000);
	XCache_EnableDCache(0x80000000);
	/*  I don't want to ALWAYS run a full memory test 
	xil_printf("Running memory test...\r\n");
	int TotalErrors = MpmcMemTestExample(XPAR_MPMC_0_DEVICE_ID);
	if (TotalErrors) {
	  xil_printf("\r\n### Program finished with errors ###\r\n");
	} else {
	  xil_printf("\r\n### Program finished successfully ###\r\n");
	}
	*/
   
	
	xil_printf("Initializing interrupts...");
	Init_Interrupts();
	xil_printf("Done!\r\nInitializing MemAlloc...");
	MemAllocInit((uint32 *)XPAR_MPMC_0_MPMC_BASEADDR + 32); // Initialize MemAlloc to use SDRAM (Add 32 (size of 1 cache line) to avoid null pointer problem)
	xil_printf("Done!\r\nInitializing State...");
	STInit();
	xil_printf("Done!\r\nInitializing Frame Table...");
	FT_Init();
	xil_printf("Done!\r\nInitializing ISR's...");
	ISR_Init();
	xil_printf("Done!\r\nInitializing Buffer Store...");
	BSInit();		
	xil_printf("Done!\r\nInitializing Camera...");
	Init_Camera();
	wruint(U_CAMERA_VERSION, ReadCameraRegister(0xFF)); // Read camera version
	{	//once camera is initialized, set up the frame save options
		uint32 save_options;
		save_options = 0;
		save_options |= camera0.frame_save_index[VISION_FRAME_RGB565];
		wrhex(X_CAM_SAVE_OPTIONS_BASE, save_options);
		//wrhex(X_CAM_SAVE_OPTIONS_EXTENDED, 0x01 << rduint(U_FRAME_TRANSMIT_OPTION));
		SetFrameSaveOptions(0x1);  //store original frame (burst to memory)
		xil_printf("Frame save options: 0x%x\r\n", GetFrameSaveOptions);
		xil_printf("Status: 0x%x\r\n", CameraStatus());
	}
	xil_printf("Done!\r\n");
	xil_printf("Starting first capture, memory address 0x%x\r\n", g_frametable[0]);
	FT_StartCapture(g_frametable[0]);	
	
	xil_printf("Initializing Game system...");
	InitGameSystem();
	xil_printf("Done!\r\n");
	wrhex(X_MEMORY_INIT_RESULTS, memory_test_results);	
	wrhex(X_MEM_ALLOC_ADDR, (uint32) MemAlloc(1));		// Record the final MemAlloc() address to ensure we didn't overflow
	XExc_mEnableExceptions(XEXC_NON_CRITICAL);			// Processor resets with interrupts disabled so Enable interrupts			
}




// STEP: Initialize Interrupts
void Init_Interrupts()
{
	/*
		Initialize Interrupts
		STEPS:
		1: Enable interrupt on all uartlite cores. 
		   This must be done BEFORE the uartlite interrupt is registerd with the PIC.
		   Or else the interrupt line will ALWAYS be on.
		2: Initialize table and register each interrupt handler with the PIC
		3: Enable the PIC to respond to the individual interrupts
		4: Enable the PIC


		Note the XExc_* and XIntc_* prefixes. XExc_* controls the CPU's internal 
		interrupts, XIntc_* are driver calls for the external interrupt controller.		
	*/

	/* InitializePIT - The Programmable-Interval Timer (PIT) is an interrupt that occurs at a 
	 * preset interval of time	 
	 */
	XTime_PITSetInterval(CPU_CLOCK_FREQ / SYSTEM_TICK_FREQ);	// Make it go off every so often
	XTime_PITEnableAutoReload();
	XTime_PITEnableInterrupt(); 								// Enable CPU's PIT interrupt feature
	// Enable the Serial Port Interrupt. This must be done BEFORE it is registered or else it will always stay on
	// No longer used, see below 
	// XUartLite_mEnableIntr(XPAR_SERIAL_UART_BASEADDR);
	// XUartLite_mEnableIntr(XPAR_WIRELESS_UART_BASEADDR);
	// Initialize PowerPC exception table
	XExc_Init();
	// Setup up PIT interrupt handler
	XExc_RegisterHandler(	XEXC_ID_PIT_INT,
							(XExceptionHandler)ISR_Timer, 
							(void *)NULL);
	// Setup external interrupt controller handler (defined in xintc_l.c/h)
	XExc_RegisterHandler( XEXC_ID_NON_CRITICAL_INT, 
							(XExceptionHandler)XIntc_DeviceInterruptHandler, 
							(void*)XPAR_XPS_INTC_0_DEVICE_ID);
	// DO NOT DO THIS
	// We now handle the uarts on a regular timed interval (every 1 ms)
	// this is done in the ISR_Timer() handler
	// Register Serial interrupt handler with interrupt controller driver
	//XIntc_RegisterHandler(	XPAR_OPB_INTC_0_BASEADDR,
	//						XPAR_OPB_INTC_0_OPB_UART_SERIAL_INTERRUPT_INTR,
	//						(XInterruptHandler)ISR_Serial,
	//						(void *)NULL);
		
	// DO NOT DO THIS
	// We now handle the uarts on a regular timed interval (every 1 ms)
	// this is done in the ISR_Timer() handler
	// Register Wireless interrupt handler with interrupt controller driver
	//XIntc_RegisterHandler(	XPAR_OPB_INTC_0_BASEADDR,
	//							XPAR_OPB_INTC_0_OPB_UART_WIRELESS_INTERRUPT_INTR,
	//							(XInterruptHandler)ISR_Wireless,
	//							(void *)NULL);

	// Register USB interrupt handler with interrupt controller driver
	XIntc_RegisterHandler(	XPAR_XPS_INTC_0_BASEADDR,
							XPAR_XPS_INTC_0_PLB_USB_0_INTERRUPT_INTR,
							(XInterruptHandler)ISR_USB,
							(void *)NULL);		

	// Register Camera Core interrupt handler with interrupt controller driver
	XIntc_RegisterHandler(	XPAR_XPS_INTC_0_BASEADDR,
							XPAR_XPS_INTC_0_PLB_VISION_0_INTERRUPT_INTR,
							(XInterruptHandler)FT_InterruptHandlerFrameTable,
							(void *)NULL);		

	// Register the GPIO interrupt handler for the Game System
	XIntc_RegisterHandler( XPAR_XPS_INTC_0_BASEADDR,
							XPAR_XPS_INTC_0_XPS_GPIO_1_IP2INTC_IRPT_INTR,
							(XInterruptHandler)ISR_GPIO,
							(void *)NULL);

	// Enable individual interrupts with interrupt controller
	XIntc_mEnableIntr(XPAR_XPS_INTC_0_BASEADDR, 
													XPAR_PLB_USB_0_INTERRUPT_MASK
													|XPAR_PLB_VISION_0_INTERRUPT_MASK
													|XPAR_XPS_GPIO_1_IP2INTC_IRPT_MASK
													);

	// Enable interrupt controller master
	XIntc_mMasterEnable(XPAR_XPS_INTC_0_BASEADDR);
}

