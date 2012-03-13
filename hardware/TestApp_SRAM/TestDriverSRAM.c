/* $Id: xilflash_devicecontrol_example.c,v 1.2 2008/04/11 10:12:40 svemula Exp $ */
/******************************************************************************
*
*       XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*       AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*       SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*       OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*       APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*       THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*       AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*       FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*       WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*       IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*       REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*       INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*       FOR A PARTICULAR PURPOSE.
*
*       (c) Copyright 2008 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
* @file xilflash_devicecontrol_example.c
*
* This file contains a design example using the Generic Flash Library.
* This example displays the Flash device geometry and properties. The geometry
* and properties are fetched using Device Control API.
*
*
* @note		None.
*
*<pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- ---------------------------------------------------------
* 1.01a mta   10/09/07 First release
*</pre>
******************************************************************************/

/***************************** Include Files *********************************/

#include <stdio.h>
#include <xbasic_types.h>
#include "xparameters.h"
#include "xutil.h"
#include "xio.h"


/************************** Constant Definitions *****************************/



/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/



/************************** Function Definitions ******************************/

/*****************************************************************************/
/**
*
* Main function to execute the Flash device control example.
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int main(void)
{
	xil_printf("\r\n\r\nMain function started\r\n");
	xil_printf("Starting SRAM read/write test\r\n");
	
	int result;
	result = SRAMReadWriteTest();
	
	if (result == XST_FAILURE){
		xil_printf("SRAM read/write test failed\r\n");
		return XST_FAILURE;
	}else {
		xil_printf("SRAM read/write test suceeded\r\n");
	}
	
	xil_printf("Main function ended\r\n");
	
	
	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function attempts to read and write to all locations on the SRAM.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int SRAMReadWriteTest(void)
{
	u32 i = 0;
	u32 result = 0;
	u32 TOP_OF_MEM = XPAR_XPS_MCH_EMC_SRAM_MEM0_HIGHADDR - XPAR_XPS_MCH_EMC_SRAM_MEM0_BASEADDR; //1 MB
	#define VALUE_FORMULA UINT_MAX - i //Start using larger numbers to test more bits
	
	xil_printf("\tTest is filling memory values from 0x%08x to 0x%08x.\r\n",
					XPAR_XPS_MCH_EMC_SRAM_MEM0_BASEADDR,
					XPAR_XPS_MCH_EMC_SRAM_MEM0_BASEADDR + TOP_OF_MEM);
	
	for(i=0; i < TOP_OF_MEM; i+=4){
		XIo_Out32(XPAR_XPS_MCH_EMC_SRAM_MEM0_BASEADDR + i, VALUE_FORMULA);
	}
	
	xil_printf("\tTest is checking memory values.\r\n");
	
	for(i=0; i < TOP_OF_MEM; i+=4){
		result = XIo_In32(XPAR_XPS_MCH_EMC_SRAM_MEM0_BASEADDR + i);
		if(result != (VALUE_FORMULA) ){
			xil_printf("Memory Location 0x%08x: Expected = %d, Actual = %d\r\n",XPAR_XPS_MCH_EMC_SRAM_MEM0_BASEADDR + i, VALUE_FORMULA, result);
			return XST_FAILURE;
		}
	}
	
	
	return XST_SUCCESS;
}






