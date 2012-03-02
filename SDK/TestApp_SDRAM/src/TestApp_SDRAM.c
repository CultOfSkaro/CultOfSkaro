/*****************************************************************************

 FILE: TestApp_SDRAM.c
 AUTHOR: Kyle Hancock
 CREATED: January 27, 2010
 
 DESCRIPTION: Tests the SDRAM controller core by performing a simple write
					and read to and from the SDRAM.

*****************************************************************************/


// Located in: ppc405_0/include/xparameters.h
#include "xparameters.h"
#include "xcache_l.h"
#include "stdio.h"
#include "xutil.h"
#include "xio.h"
#include "xmpmc.h"
#include "mpmc_mem_test_example.h"
#include "mpmc_calibration.h"
#include "MemoryTest.h"

//====================================================

#define TEST_OFFSET 0x00000004

XMpmc mySDRAM;
XMpmc_Config* mySDRAMConfig = NULL;
uint32* myAddress = (uint32*)XPAR_MPMC_0_MPMC_BASEADDR;
uint32 memSize = XPAR_MPMC_0_MPMC_HIGHADDR - XPAR_MPMC_0_MPMC_BASEADDR + 1; //get correct SDRAM size


int main (void) {


   XCache_EnableICache(0x00000001);
   XCache_EnableDCache(0x00000001);
	
   XCache_DisableDCache();
   XCache_DisableICache();

   xil_printf("\r\n-- Entering main() --\r\n");
	
	/************************ SDRAM SelfTest **************************/
	xil_printf("Performing SelfTest of SDRAM controller...\r\n");
	
	int iteration = 0;
	while(1)
	{
		xil_printf("*Iteration %d\r\n", iteration++);
	int Status;
	int i;
	volatile a;
	volatile b;
	int TotalErrors;
	Xuint8 dcm_psen, dcm_psincdec, dcm_done;
	
	mySDRAMConfig = XMpmc_LookupConfig(XPAR_MPMC_0_DEVICE_ID);
	if(mySDRAMConfig == NULL)
	{
		xil_printf("Could not find SDRAM based on dev id\r\n");
		return XST_FAILURE;
	}
	else {
		xil_printf("Got SDRAM config\r\n");
	}
	Status = XMpmc_CfgInitialize(&mySDRAM,mySDRAMConfig, XPAR_MPMC_0_MPMC_BASEADDR);
	if(Status != XST_SUCCESS) {
		xil_printf("Initialization Failed!\r\n");
		return XST_FAILURE;
	}
	else {
		xil_printf("Initialization Complete!\r\n");
	}
	Status = XMpmc_SelfTest(&mySDRAM);
	if(Status != XST_SUCCESS) {
		xil_printf("SelfTest Failed!\r\n");
		return XST_FAILURE;
	}
	else {
		xil_printf("SelfTest Complete!\r\n");
	}
	
	
	/***************** START CALIBRATION ***************/
	Status = MpmcCalibrationExample(XPAR_MPMC_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	/***************** END CALIBRATION *****************/
	
	TotalErrors = MpmcMemTestExample(XPAR_MPMC_0_DEVICE_ID);

	if (TotalErrors) {
		xil_printf("\r\n### Program finished with errors ###\r\n");
	} else {
		xil_printf("\r\n### Program finished successfully ###\r\n");
	}
	
	
	ENABLE_ICACHE();
	ENABLE_DCACHE();
	
	Xuint32 memSize = XPAR_MPMC_0_MPMC_HIGHADDR - XPAR_MPMC_0_MPMC_BASEADDR + 1; //get correct SDRAM size
	Xuint32* myAddress = (Xuint32*)XPAR_MPMC_0_MPMC_BASEADDR;
	
	xil_printf("\r\nStarting Wade's special Test...\r\n");
	char wadeSuccess = RunAllMemoryTests(myAddress, memSize);
	
	if (wadeSuccess){
		xil_printf("Wade's test PASSED!");
	}
	else{
		xil_printf("Wade's test FAILED!");
	}	
	
	xil_printf("Total Errors: %d\r\n", TotalErrors);
	}
}
