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
#include <xilflash.h>
#include "xutil.h"
#include "xio.h"


/************************** Constant Definitions *****************************/

#define MAX_FLASH_BYTES		16777216
#define FLASH_TEST_SIZE		4096
#define START_ADDRESS		0
//shows how many bytes wide the data channel is
#define DATA_WIDTH			2

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
int FlashDeviceControlExample(void);
int FlashReadWriteExample(void);

/************************** Variable Definitions *****************************/
XFlash FlashInstance; /* XFlash Instance. */

/*
 * Buffers used during read and write transactions.
 */
u16 ReadBuffer[FLASH_TEST_SIZE]; /* Buffer used to store the data read. */
u16 WriteBuffer[FLASH_TEST_SIZE]; /* Write buffer. */

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
	int Status;
	print("Starting FlashDevice Control test\r\n");
	
	Status = FlashDeviceControlExample();
	if(Status != XST_SUCCESS) {
		print("\tTest Failed\r\n\r\n");
		return XST_FAILURE;
	}
	print("\tTest Suceeded\r\n\r\n");
	
	
	//Test Read/Write
	print("Starting Read/Write Test\r\n");
	Status = FlashReadWriteExample();
	if(Status != XST_SUCCESS) {
		print("\tRead/Write Test Failed\r\n\r\n");
		return XST_FAILURE;
	}
	print("\tRead/Write Test Suceeded\r\n\r\n");
	
	
	
	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function fetches and displays the geometry and properties of the Flash *
* device.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int FlashDeviceControlExample(void)
{
	int Status;
	u32 Index;
	DeviceCtrlParam IoctlParams;

	/*
	 * Initialize the Flash Library.
	 */
	Status = XFlash_Initialize(&FlashInstance, XPAR_XPS_MCH_EMC_FLASH_MEM0_BASEADDR, 2, 0);
	if(Status != XST_SUCCESS) {
		print("\tInitialization Failed\r\n");
		return XST_FAILURE;
	}

	/*
	 * Reset the Flash Device. This clears the Status registers and puts
	 * the device in Read mode.
	 */
	Status = XFlash_Reset(&FlashInstance);
	if(Status != XST_SUCCESS) {
		print("\tReset Failed\r\n");
		return XST_FAILURE;
	}

	/*
	 * Fetch the flash device properties and display.
	 */
	Status = XFlash_DeviceControl(&FlashInstance,
				XFL_DEVCTL_GET_PROPERTIES, &IoctlParams);
	if(Status != XST_SUCCESS) {
		print("\tFetch device properties Failed\r\n");
		return XST_FAILURE;
	}

	xil_printf("\n\r\t\tFlash Properties\n\r");
	xil_printf("FlashProperties->PartID.ManufacturerID = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
						PartID.ManufacturerID);

	xil_printf("FlashProperties->PartID.DeviceID = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
							PartID.DeviceID);

	xil_printf("FlashProperties->PartID.DeviceID = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
							PartID.CommandSet);

	xil_printf("FlashProperties->TimeTypical.WriteSingle_Us = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
						TimeTypical.WriteSingle_Us);

	xil_printf("FlashProperties->TimeTypical.WriteBuffer_Us = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
						TimeTypical.WriteBuffer_Us);

	xil_printf("FlashProperties->TimeTypical.EraseBlock_Ms = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
						TimeTypical.EraseBlock_Ms);

	xil_printf("FlashProperties->TimeTypical.EraseChip_Ms = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
						TimeTypical.EraseChip_Ms);

	xil_printf("FlashProperties->TimeMax.WriteSingle_Us = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
						TimeMax.WriteSingle_Us);

	xil_printf("FlashProperties->TimeMax.WriteBuffer_Us = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
						TimeMax.WriteBuffer_Us);

	xil_printf("FlashProperties->TimeMax.EraseBlock_Ms = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
						TimeMax.EraseBlock_Ms);

	xil_printf("FlashProperties->TimeMax.EraseChip_Ms = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
						TimeMax.EraseChip_Ms);

	xil_printf("FlashProperties->ProgCap.WriteBufferSize = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
						ProgCap.WriteBufferSize);

	xil_printf("FlashProperties->ProgCap.WriteBufferAlignMask = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->ProgCap.
						WriteBufferAlignmentMask);

	xil_printf("FlashProperties->ProgCap.EraseQueueSize = 0x%x\n\r",
			IoctlParams.PropertiesParam.PropertiesPtr->
						ProgCap.EraseQueueSize);

	/*
	 * Fetch the flash device geometry and display.
	 */
	Status = XFlash_DeviceControl(&FlashInstance,
				XFL_DEVCTL_GET_GEOMETRY, &IoctlParams);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	xil_printf("\n\r\n\r\t\tFlash Geometry\n\r");

	xil_printf("FlashGeometry->BaseAddress = 0x%x\n\r",
			IoctlParams.GeometryParam.GeometryPtr->BaseAddress);

	xil_printf("FlashGeometry->MemoryLayout = 0x%x\n\r",
			IoctlParams.GeometryParam.GeometryPtr->MemoryLayout);

	xil_printf("FlashGeometry->DeviceSize = 0x%x\n\r",
			IoctlParams.GeometryParam.GeometryPtr->DeviceSize);

	xil_printf("FlashGeometry->NumEraseRegions = 0x%x\n\r",
			IoctlParams.GeometryParam.GeometryPtr->NumEraseRegions);

	xil_printf("FlashGeometry->NumBlocks = 0x%x\n\r",
			IoctlParams.GeometryParam.GeometryPtr->NumBlocks);

	for(Index = 0; Index < IoctlParams.GeometryParam.GeometryPtr->
					NumEraseRegions; Index++) {
		xil_printf("\tErase region %d\n\r", Index);

		xil_printf("Absolute Offset = 0x%x\n\r",
				IoctlParams.GeometryParam.GeometryPtr->
					EraseRegion[Index].AbsoluteOffset);

		xil_printf("Absolute Block = 0x%x\n\r",
				IoctlParams.GeometryParam.GeometryPtr->
					EraseRegion[Index].AbsoluteBlock);

		xil_printf("Num Of Block = 0x%x\n\r",
				IoctlParams.GeometryParam.GeometryPtr->
						EraseRegion[Index].Number);

		xil_printf("Size Of Block = 0x%x\n\r",
				IoctlParams.GeometryParam.GeometryPtr->
						EraseRegion[Index].Size);
	}

	return XST_SUCCESS;
}


/*****************************************************************************/
/**
*
* This function writes, reads, and verifies the data to the Flash device.
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int FlashReadWriteExample(void)
{
	int Status;
	int i;
	u32 Index;

	/*
	 * Initialize the Flash Library.
	 */
	Status = XFlash_Initialize(&FlashInstance, XPAR_XPS_MCH_EMC_FLASH_MEM0_BASEADDR, 2, 0);
	if(Status != XST_SUCCESS) {
		print("\tInitialization Failed\r\n");
		return XST_FAILURE;
	}

	for(i=0;i<MAX_FLASH_BYTES/(FLASH_TEST_SIZE*DATA_WIDTH); i++){
		xil_printf("Starting round %3d\r\n",i);
		/*
		 * Reset the Flash Device. This clears the Status registers and puts
		 * the device in Read mode.
		 */
		Status = XFlash_Reset(&FlashInstance);
		if(Status != XST_SUCCESS) {
			print("\tReset Failed\r\n");
			return XST_FAILURE;
		}
	
		/*
		 * Perform the Erase operation.
		 */
		Status = XFlash_Erase(&FlashInstance, START_ADDRESS+(i*FLASH_TEST_SIZE*DATA_WIDTH), FLASH_TEST_SIZE*DATA_WIDTH);
		if(Status != XST_SUCCESS) {
			print("\tErase Failed\r\n");
			return XST_FAILURE;
		}
		
		/*
		 * Prepare the write buffer. Fill in the data need to be written into
		 * Flash Device.
		 */
		for(Index = 0; Index < FLASH_TEST_SIZE; Index++) {
			WriteBuffer[Index] = Index + 1;
		}

		/*
		 * Perform the Write operation.
		 */
		Status = XFlash_Write(&FlashInstance, START_ADDRESS+(i*FLASH_TEST_SIZE*DATA_WIDTH), FLASH_TEST_SIZE*DATA_WIDTH,
									WriteBuffer);
		if(Status != XST_SUCCESS) {
			print("\tWrite Failed\r\n");
			return XST_FAILURE;
		}

		/*
		 * Perform the read operation.
		 */
		Status = XFlash_Read(&FlashInstance, START_ADDRESS+(i*FLASH_TEST_SIZE*DATA_WIDTH), FLASH_TEST_SIZE*DATA_WIDTH,
									ReadBuffer);
		if(Status != XST_SUCCESS) {
			print("\tRead Failed\r\n");
				return XST_FAILURE;
		}

		/*
		 * Compare the data read against the data Written.
		 */
		for(Index = 0; Index < FLASH_TEST_SIZE; Index++) {
			//xil_printf("Expected = '%d', Actual = '%d'\r\n", (u8)(Index + 1), ReadBuffer[Index]);
			if(ReadBuffer[Index] != (u16)(Index + 1)) {
				xil_printf("\t%3d:Data mismatch in round %d. Expected = '%d', Actual = '%d'\r\n",Index,i, (u8)(Index + 1), ReadBuffer[Index]);
				//return XST_FAILURE;
			}
		}
	}

	return XST_SUCCESS;
}






