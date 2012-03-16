/* $Id: xilflash_cfi.c,v 1.1.2.2 2010/06/30 05:40:50 sadanan Exp $ */
/******************************************************************************
*
* (c) Copyright 2007-2010 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xilflash_cfi.c
*
* The file implements the functions for retrieval and translation of CFI data
* from a compliant flash device. CFI contains data that defines part geometry,
* write/erase timing, and programming data.
*
* Data is retrieved using macros defined in this xflash_cfi.h file. The
* macros simplify data extraction because they have been written to take into
* account the layout of parts on the data bus. To the library, CFI data appears
* as if it were always being read from a single 8-bit part (XFL_LAYOUT_X8_X8_X1)
* Otherwise, the retrieval code would have to contend with all the formats
* illustrated below. The illustration shows how the first three bytes of the CFI
* query data "QRY" appear in flash memory space for various part layouts.
* <pre>
*
*			Byte Offset (Big-Endian)
*			0123456789ABCDEF
*			----------------
*			XFL_LAYOUT_X16_X16_X1	Q R Y
*			XFL_LAYOUT_X16_X16_X2	Q Q R R Y Y
* </pre>
*
* Where the holes between Q, R, and Y are NULL (all bits 0)
*
* @note
*
* This code is intended to be RTOS and processor independent.
* It works with physical addresses only. Any needs for dynamic memory
* management, threads, mutual exclusion, virtual memory, or cache control
* management must be satisfied by the layer above this library.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rmm  10/25/07 First release
* 1.00a mta  10/25/07 Updated to flash library
* 1.01a ksu  04/10/08 Added support for AMD CFI Interface
* 1.02a ksu  08/06/09 Added code to read the platform flash bank information
* 2.00a ktn  12/04/09 Updated to use the HAL processor APIs/macros
* 2.02a sdm  06/30/10 Updated to support AXI EMC with Little Endian Processor
* </pre>
*
*
****************************************************************************/

/***************************** Include Files *********************************/
#include "xil_types.h"
#include "include/xilflash.h"
#include "include/xilflash_cfi.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************** Function Definitions *****************************/

/*****************************************************************************/
/**
*
* Retrieves the standard CFI data from the part(s), interpret the data, and
* update the provided geometry and properties structures.
*
* Extended CFI data is part specific and ignored here. This data must be read
* by the specific flash device family library.
*
* @param	InstancePtr is the pointer to the XFlash instance.
*
* @return
*		- XST_SUCCESS if successful.
*		- XFLASH_CFI_QUERY_ERROR if an error occurred interpreting
*		  the data.
*		- XFLASH_PART_NOT_SUPPORTED if invalid Layout parameter.
*
* @note		None.
*
******************************************************************************/
int XFlashCFI_ReadCommon(XFlash *InstancePtr)
{
	void *DataPtr;
	u8 Data8;
	u8 Mode;
	u8 DataQRY[3];
	u16 Data16;
	u16 ExtendedQueryTblOffset;
	u32 SizeMultiplier;
	u32 CurrentAbsoluteOffset;
	u16 CurrentAbsoluteBlock;
	u32 Index;
	u32 Interleave;
	int Status = XST_SUCCESS;
#if (XPAR_XFL_PLATFORM_FLASH == 1)
	u16 NumBanks, Bank;
	u16 NumEraseRegions;
	u8 TypesEraseBlock;
	u16 NumBlockInBank;
	u32 SizeBlockInBank;
	XFlashGeometry *GeomPtr;
#endif /* XPAR_XFL_PLATFORM_FLASH */

	if(InstancePtr == NULL) {
		return XST_FAILURE;
	}

	switch (InstancePtr->Geometry.MemoryLayout) {
		case XFL_LAYOUT_X16_X8_X1:
			WRITE_FLASH_8(InstancePtr->Geometry.BaseAddress + 0xAA,
								0x98);
			break;

		case XFL_LAYOUT_X16_X16_X1:
			WRITE_FLASH_16(InstancePtr->Geometry.BaseAddress + 0xAA,
								0x9898);
			break;

		case XFL_LAYOUT_X16_X16_X2:
			WRITE_FLASH_32(InstancePtr->Geometry.BaseAddress + 0xAA,
								0x98989898);
			break;

		case XFL_LAYOUT_X16_X16_X4:
			WRITE_FLASH_64x2(InstancePtr->Geometry.BaseAddress +
						0xAA, 0x98989898, 0x98989898);
			break;

		default:
			return (XFLASH_NOT_SUPPORTED);
	}

	/*
	 * To stay consistent when retrieving the CFI data for all part layouts
	 * we use the XFL_CFI_READ macros supplying the correct interleave based
	 * on the layout.
	 *
	 * The size of a block for an instance is the block size reported
	 * by the device multiplied by the number of devices (SizeMultiplier).
	 */
	Interleave = (InstancePtr->Geometry.MemoryLayout &
			XFL_LAYOUT_CFI_INTERL_MASK) >> 24;
	SizeMultiplier = (InstancePtr->Geometry.MemoryLayout &
			  XFL_LAYOUT_NUM_PARTS_MASK);
	Mode = (InstancePtr->Geometry.MemoryLayout &
			      XFL_LAYOUT_PART_MODE_MASK) >> 8;

	/*
	 * Begin reading the data. Each datum is documented in comments with
	 * its offset range.
	 */

	/*
	 * 10h-12h : Contains the "QRY" string. Must be present.
	 */
	XFL_CFI_POSITION_PTR(DataPtr, InstancePtr->Geometry.BaseAddress,
				Interleave, 0x10);

	DataQRY[0] = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
	XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);
	DataQRY[1] = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
	XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);
	DataQRY[2] = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);

	if ((DataQRY[0] != 'Q') || (DataQRY[1] != 'R') || (DataQRY[2] != 'Y')) {
		Status = XFLASH_CFI_QUERY_ERROR;
	}
	else {
		/*
		 * 13h-14h : Primary vender command set.
		 */
		XFL_CFI_POSITION_PTR(DataPtr, InstancePtr->Geometry.BaseAddress,
					Interleave, 0x13);
		InstancePtr->Properties.PartID.CommandSet =
			XFlashCFI_Read16((u8*)DataPtr, Interleave, Mode);
		InstancePtr->CommandSet =
				InstancePtr->Properties.PartID.CommandSet;

		/*
		 * 15h-16h : Address for Primary Algorithm extended Query table.
		 */
		XFL_CFI_ADVANCE_PTR16(DataPtr, Interleave);
		ExtendedQueryTblOffset =
			XFlashCFI_Read16((u8*)DataPtr, Interleave, Mode);

		/*
		 * 17h-1Ah : Vendor data to be interpreted by part (ignored
		 * here).
		 * 1Bh-1Eh : Voltage requirements (ignored by this library).
		 *
		 * Interpret the timing requirements starting here.
		 *	1Fh : Typical timeout for single byte/word program cycle
		 * 		(2^N Us).
		 */
		XFL_CFI_POSITION_PTR(DataPtr, InstancePtr->Geometry.BaseAddress,
						Interleave, 0x1F);
		Data8 = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
		if (Data8 != 0) {
			InstancePtr->Properties.TimeTypical.WriteSingle_Us =
								1 << Data8;
		}

		XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);

		/*
		 * 20h:Typical timeout for max buffer program cycle (2^N Us)
		 * = 0 if not supported.
		 */

		Data8 = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
		if (Data8 != 0) {
			InstancePtr->Properties.TimeTypical.WriteBuffer_Us =
								1 << Data8;
		}

		XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);

		/*
		 * 21h : Typical timeout for single block erase (2^N Ms).
		 */
		Data8 = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
		if (Data8 != 0) {
			InstancePtr->Properties.TimeTypical.EraseBlock_Ms =
								1 << Data8;
		}

		XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);

		/*
		 * 22h : Typical timeout for full chip erase (2^N Ms)
		 * = 0 if not supported.
		 */
		Data8 = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
		if (Data8 != 0) {
			InstancePtr->Properties.TimeTypical.EraseChip_Ms =
								1 << Data8;
		}

		XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);

		/*
		 * 23h : Maximum timeout for single byte/word program cycle
		 *	(2^N * typical time).
		 */
		Data8 = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
		InstancePtr->Properties.TimeMax.WriteSingle_Us =
			InstancePtr->Properties.TimeTypical.WriteSingle_Us *
								(1 << Data8);
		XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);

		/*
		 * 24h : Maximum timeout for max buffer program cycle
		 *	(2^N * typical time)
		 *	= 0 if not supported.
		 */
		Data8 = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
		InstancePtr->Properties.TimeMax.WriteBuffer_Us =
			InstancePtr->Properties.TimeTypical.WriteBuffer_Us *
			(1 << Data8);
		XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);

		/*
		 * 25h : Maximum timeout for single block erase
		 *	(2^N * typical time).
		 */
		Data8 = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
		InstancePtr->Properties.TimeMax.EraseBlock_Ms =
			InstancePtr->Properties.TimeTypical.EraseBlock_Ms *
								(1 << Data8);
		XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);

		/*
		 * 26h : Maximum timeout for full chip erase
		 *	(2^N * typical time)
		 *	= 0 if not supported.
		 */
		Data8 = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
		InstancePtr->Properties.TimeMax.EraseChip_Ms =
			InstancePtr->Properties.TimeTypical.EraseChip_Ms *
								(1 << Data8);
		XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);

		/*
		 * 27h : Device size in bytes
		 *	= 2^N bytes * (Number of parts).
		 */
		Data8 = XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
		InstancePtr->Geometry.DeviceSize = (1 << Data8) *
								SizeMultiplier;
		XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);

		/*
		 * 28h-29h : Device interface description (ignored).
		 */
		XFL_CFI_ADVANCE_PTR16(DataPtr, Interleave);

		/*
		 * 2Ah-2Bh : Maximum number of bytes in write buffer
		 *		= 2^N bytes * (Number of parts).
		 */
		Data16 = XFlashCFI_Read16((u8*)DataPtr, Interleave, Mode);
		if (Data16 != 0) {
			InstancePtr->Properties.ProgCap.WriteBufferSize =
						(1 << Data16) * SizeMultiplier;
		}

		XFL_CFI_ADVANCE_PTR16(DataPtr, Interleave);

		/*
		 * 2Ch : Number of erase regions.
		 * Make sure there are not too many to contain in the instance.
		 * This will ensure the for loop below doesn't go out of bounds
		 * on the Geometry.EraseRegion array.
		 */
		InstancePtr->Geometry.NumEraseRegions = XFlashCFI_Read8(
								(u8*)DataPtr,
								Interleave,
								Mode);
		if (InstancePtr->Geometry.NumEraseRegions >
			XFL_MAX_ERASE_REGIONS) {
			return (XFLASH_TOO_MANY_REGIONS);
		}

		XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);

		/*
		 * 2Dh-30h : Erase region #1 definition
		 * 31h-34h : Erase region #2 definition
		 * 35h-39h : Erase region #3 definition,  etc.
		 */
		CurrentAbsoluteOffset = 0;
		CurrentAbsoluteBlock = 0;
		InstancePtr->Geometry.NumBlocks = 0;
		for (Index = 0; Index < InstancePtr->Geometry.NumEraseRegions;
		Index++) {

			/*
			 * Offset 0-1 : Number of blocks in the region
			 *		= N + 1.
			 */
			Data16 = XFlashCFI_Read16((u8*)DataPtr, Interleave,
							Mode);
			InstancePtr->Geometry.EraseRegion[Index].Number = Data16
									+ 1;
			InstancePtr->Geometry.NumBlocks +=
				InstancePtr->Geometry.EraseRegion[Index].Number;
			XFL_CFI_ADVANCE_PTR16(DataPtr, Interleave);

			/*
			 * Offset 2-3 : Size of erase blocks in the region
			 * = N * 256 * (Number of parts).
			 */
			Data16 = XFlashCFI_Read16((u8*)DataPtr, Interleave,
						Mode);
			InstancePtr->Geometry.EraseRegion[Index].Size =
						Data16 * 256 * SizeMultiplier;
			XFL_CFI_ADVANCE_PTR16(DataPtr, Interleave);

			/*
			 * Calculate the part offset where this region begins.
			 */
			InstancePtr->Geometry.EraseRegion[Index].AbsoluteOffset
							= CurrentAbsoluteOffset;
			InstancePtr->Geometry.EraseRegion[Index].AbsoluteBlock =
							CurrentAbsoluteBlock;

			/*
			 * Increment absolute counters.
			 */
			CurrentAbsoluteOffset +=
			(InstancePtr->Geometry.EraseRegion[Index].Size *
			InstancePtr->Geometry.EraseRegion[Index].Number);
			CurrentAbsoluteBlock +=
				InstancePtr->Geometry.EraseRegion[Index].Number;
		}

		/*
		 * Set the absolute offsets for NumEraseRegions+1. This is not a
		 * real region, but marks one unit past the part's addressable
		 * region. For example, if the device(s) are a total of 1000
		 * bytes in size with a total of 10 blocks then 1000 and 10 are
		 * written to the Absolute parameters. The Size & Number are
		 * left as zero.
		 */
		InstancePtr->Geometry.EraseRegion[Index].AbsoluteOffset =
							CurrentAbsoluteOffset;
		InstancePtr->Geometry.EraseRegion[Index].AbsoluteBlock =
							CurrentAbsoluteBlock;

		/*
		 * This ends the query. The following summarizes what attributes
		 * of the InstancePtr were initialized:
		 *
		 *	Properties.PartID
		 *	- CommandSet defined.
		 *	- ManufacturerID is defined by the part's Initialize
		 *	  function.
		 *	- DeviceID is defined by the part's Initialize function.
		 *
		 *	Properties.TimeTypical
		 *	Completely defined.
		 *
		 *	Properties.TimeMax
		 *	Completely defined.
		 *
		 *	Properties.ProgCap
		 *	- WriteBufferAlignment must be defined by the device.
		 *	  It defaults to 0 here.
		 *	- EraseQueueSize must be defined by the device. It
		 *	  defaults to 1 here.
		 *
		 * Geometry
		 *	Completely defined.
		 *
		 */
		InstancePtr->Properties.ProgCap.EraseQueueSize = 1;

		/*
		 * Some of AMD flash have different geometry based on
		 * type of boot mode. Read boot mode to identify
		 * location of boot block and parameter blocks.
		 */
		if (InstancePtr->CommandSet == 0x02) {
		 	/*
		 	 * Extended Query Table Offset + 0x0F: Boot mode.
		 	 */
			XFL_CFI_POSITION_PTR(DataPtr,
				InstancePtr->Geometry.BaseAddress, Interleave,
				(ExtendedQueryTblOffset + 0x0F));
			InstancePtr->Geometry.BootMode =
				XFlashCFI_Read8((u8*)DataPtr, Interleave, Mode);
		}

		/*
		 * The platform flash (Intel) have multiple banks in same erase
		 * region. Read number of identical banks in each erase region,
		 * number of erase blocks and size of blocks in each bank. For
		 * platfrom flash, library treats each bank as seperate region.
		 */
		if (InstancePtr->CommandSet == 0x01) {
#if (XPAR_XFL_PLATFORM_FLASH == 1)
			Index = 0;
			NumEraseRegions = InstancePtr->Geometry.NumEraseRegions;
			InstancePtr->Geometry.NumEraseRegions = 0;
			CurrentAbsoluteOffset = 0;
			CurrentAbsoluteBlock = 0;
			Bank = 0;
			GeomPtr = &InstancePtr->Geometry;
			while (Index < NumEraseRegions) {
				/*
				 * Extended Query Table Offset + 0x24/0x32:
				 * Number of banks in  region.
				 */
				XFL_CFI_POSITION_PTR(DataPtr,
					InstancePtr->Geometry.BaseAddress,
					Interleave,
					(ExtendedQueryTblOffset + 0x24 +
					(Index * 0x0E)));
				NumBanks = XFlashCFI_Read16((u8*)DataPtr,
						Interleave, Mode);
				/*
				 * Ignore the information about multiple
				 * operation in bank and region as it is not
				 * supported by the library
				 */
				XFL_CFI_ADVANCE_PTR16(DataPtr, Interleave);
				XFL_CFI_ADVANCE_PTR16(DataPtr, Interleave);
				XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);

				/*
				 * Extended Query Table Offset + 0x29/0x37:
				 * Types of erase block in the bank
				 */
				TypesEraseBlock =
					XFlashCFI_Read8((u8*)DataPtr,
						Interleave, Mode);

				XFL_CFI_ADVANCE_PTR8(DataPtr, Interleave);
				while (TypesEraseBlock--) {
					/*
					 * Number of erase block in bank
					 */
					NumBlockInBank =
						XFlashCFI_Read16((u8*)DataPtr,
							Interleave, Mode);
					NumBlockInBank += 1;
					XFL_CFI_ADVANCE_PTR16(DataPtr,
						Interleave);
					/*
					 * Size of each erase block in bank
					 */
					SizeBlockInBank =
						XFlashCFI_Read16((u8*)DataPtr,
							Interleave, Mode);
					SizeBlockInBank *= 256;
					/*
					 * Update flash instance structure
					 */
					GeomPtr->NumEraseRegions += NumBanks;
					while (Bank < GeomPtr->NumEraseRegions){
						GeomPtr->EraseRegion[Bank].
							Number = NumBlockInBank;
						GeomPtr->EraseRegion[Bank].Size
							= SizeBlockInBank;
						GeomPtr->EraseRegion[Bank].
							AbsoluteOffset =
							CurrentAbsoluteOffset;
						GeomPtr->EraseRegion[Bank].
							AbsoluteBlock =
							CurrentAbsoluteBlock;

						CurrentAbsoluteOffset +=
							(GeomPtr->EraseRegion
							[Bank].Size *
							GeomPtr->EraseRegion
							[Bank].Number);
						CurrentAbsoluteBlock +=
							GeomPtr->EraseRegion
							[Bank].Number;
						Bank++;
					}
					XFL_CFI_ADVANCE_PTR16(DataPtr,
						Interleave);
					XFL_CFI_ADVANCE_PTR16(DataPtr,
						Interleave);
					XFL_CFI_ADVANCE_PTR16(DataPtr,
						Interleave);
				}
				Index++;
			}
			GeomPtr->EraseRegion[Bank].AbsoluteOffset =
				CurrentAbsoluteOffset;
			GeomPtr->EraseRegion[Bank].AbsoluteBlock =
				CurrentAbsoluteBlock;
#endif /* XPAR_XFL_PLATFORM_FLASH */
	 	}
	}

	switch (InstancePtr->Geometry.MemoryLayout) {
		case XFL_LAYOUT_X16_X8_X1:
			WRITE_FLASH_8(InstancePtr->Geometry.BaseAddress + 0xAA,
							0xFF);
			break;

		case XFL_LAYOUT_X16_X16_X1:
			WRITE_FLASH_16(InstancePtr->Geometry.BaseAddress + 0xAA,
								0xFFFF);
		break;

		case XFL_LAYOUT_X16_X16_X2:
			WRITE_FLASH_32(InstancePtr->Geometry.BaseAddress + 0xAA,
								0xFFFFFFFF);
		break;

		case XFL_LAYOUT_X16_X16_X4:
			WRITE_FLASH_64x2(InstancePtr->Geometry.BaseAddress +
						0xAA, 0xFFFFFFFF, 0xFFFFFFFF);
		break;
	}

	return (Status);
}

#ifdef XPAR_AXI_EMC
/*****************************************************************************/
/**
*
* Reads 8-bits of data from the CFI data location into a local variable.
*
* @param	Ptr is the pointer to read. Can be a pointer to any type.
* @param	Interleave is the byte interleaving (based on part layout).
* @param	Mode is the mode of operation (based on part layout).
*
* @return	The byte at Ptr adjusted for the interleave factor.
*
*****************************************************************************/
int XFlashCFI_Read8(u8 *Ptr, u8 Interleave, u8 Mode)
{
	return (READ_FLASH_8((u32)Ptr));
}

/*****************************************************************************/
/**
*
* Reads 16-bits of data from the CFI data location into a local variable.
*
* @param	Ptr is the pointer to read. Can be a pointer to any type.
* @param	Interleave is the byte interleaving (based on part layout).
* @param	Mode is the mode of operation (based on part layout).
*
* @return	The 16-bit value at Ptr adjusted for the interleave factor.
*
*****************************************************************************/
int XFlashCFI_Read16(u8 *Ptr, u8 Interleave, u8 Mode)
{
	int Data = 0;

	(Data) = (u8)READ_FLASH_8((u8*)(Ptr) + Interleave);
	(Data) <<= 8;
	(Data) |= (u8)READ_FLASH_8((u8*)(Ptr));

	return Data;
}

#else /* XPAR_XPS_MCH_EMC */
/*****************************************************************************/
/**
*
* Reads 8-bits of data from the CFI data location into a local variable.
*
* @param	Ptr is the pointer to read. Can be a pointer to any type.
* @param	Interleave is the byte interleaving (based on part layout).
* @param	Mode is the mode of operation (based on part layout).
*
* @return	The byte at Ptr adjusted for the interleave factor.
*
*****************************************************************************/
int XFlashCFI_Read8(u8 *Ptr, u8 Interleave, u8 Mode)
{
	if (Mode == (u8)1) {
		Interleave = 1;
	}

	return (READ_FLASH_8((u32)Ptr + Interleave - 1));

}

/*****************************************************************************/
/**
*
* Reads 16-bits of data from the CFI data location into a local variable.
*
* @param	Ptr is the pointer to read. Can be a pointer to any type.
* @param	Interleave is the byte interleaving (based on part layout).
* @param	Mode is the mode of operation (based on part layout).
*
* @return	The 16-bit value at Ptr adjusted for the interleave factor.
*
*****************************************************************************/
int XFlashCFI_Read16(u8 *Ptr, u8 Interleave, u8 Mode)
{
	int Data = 0;

	if (Mode == (u8)1) {
		(Data) = (u8)READ_FLASH_8((u8*)(Ptr) + Interleave);
		(Data) <<= 8;
		(Data) |= (u8)READ_FLASH_8((u8*)(Ptr));
	}
	else if (Mode == (u8)2) {
		(Data) = (u16)READ_FLASH_8((u8*)(Ptr) + ((Interleave) * 2) - 1);
		(Data) <<= 8;
		(Data) |= (u16)READ_FLASH_8((u8*)(Ptr) + (Interleave) - 1);
	}

	return Data;
}
#endif /* XPAR_AXI_EMC */