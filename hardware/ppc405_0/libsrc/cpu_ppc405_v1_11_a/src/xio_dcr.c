/* $Id: xio_dcr.c,v 1.1.2.3 2009/05/01 17:49:21 moleres Exp $ */
/******************************************************************************
*
* (c) Copyright 2002-2009 Xilinx, Inc. All rights reserved.
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
* @file xio_dcr.c
*
* The implementation of the XDcrIo interface. See xio_dcr.h for more
* information about the component.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a ecm  10/18/01 First release
*       ecm  01/29/03 Cleaned up the table and made it more readable.
*                     The DCR_REG_512 was 17 and the table used DCR_REG_32
*                     +15 which is actually the same as what DCR_REG_512
*                     should have been,16. I removed the blocks using
*                     DCR_REG_32+15 and added blocks using DCR_REG_512+14
*                     to make the table correct when DCR_REG_512 = 16.
*                     This is functionally exactly the same, just more
*                     readable hence no rev bump.
*       ecm  09/21/05 Fixed the function calls into the table to allow
*                     the code to be optimized. Also added the directive
*                     to force the code into the text space for Linux to
*                     function without causing a page fault.
* 1.10a wgr  03/22/07 Converted to new coding style.
* </pre>
*
* @internal
*
* The C functions which subsequently call into either the assembly code or into
* the provided table of functions are required since the registers assigned to
* the calling and return from functions are strictly defined in the ABI and that
* definition is used in the low-level functions directly. The use of macros is
* not recommended since the temporary registers in the ABI are defined but there
* is no way to force the compiler to use a specific register in a block of code.
*
*****************************************************************************/

/***************************** Include Files ********************************/

#include "xstatus.h"
#include "xbasic_types.h"
#include "xio.h"
#include "xio_dcr.h"

/************************** Constant Definitions ****************************/

#define SUB_RETURN                  0x4E800020	/* equals 'blr' */
#define MTDCR_FIXED                 0x7C000386	/* this is the basic template */
						/* equals 'mtdcr 0, 0x00' */
#define MFDCR_FIXED                 0x7C000286	/* this is the basic template */
						/* equals 'mfdcr 0, 0x00' */
#define GPR_SHIFT                   21
#define DCR_SHIFT_LH                16	/* bits 5 - 9 shift value */
#define DCR_SHIFT_UH                11	/* bits 0 - 4 shift value */
#define DCR_REG_32                   1	/* register 32 and above */
#define DCR_REG_512                 16	/* register 512 and above */

/* defines for the mfdcr and mfdcr structures */

#define GPR_SOURCE_REG              4<<GPR_SHIFT
#define GPR_DEST_REG                3<<GPR_SHIFT


/**************************** Type Definitions ******************************/
typedef u32 (*XIo_DcrOpCode) (u32 Register, u32 Data);


typedef struct DcrFunctionTableEntryTag {
	u32 OpCode;
	u32 Return;
} DcrFunctionTableEntry;


#if defined __GNUC__
#if defined XPAR_XIO_DCR_TEXT_SPACE
static DcrFunctionTableEntry
	__attribute__ ((section(".text"))) MtDcrTable[MAX_DCR_REGISTERS] =
#else
static DcrFunctionTableEntry MtDcrTable[MAX_DCR_REGISTERS] =
#endif
#else
static DcrFunctionTableEntry MtDcrTable[MAX_DCR_REGISTERS] =
#endif
{
	{MTDCR_FIXED | GPR_SOURCE_REG | (0) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (1) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (2) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (3) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (4) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (5) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (6) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (7) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (8) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (9) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (10) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (11) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (12) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (13) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (14) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (15) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (16) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (17) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (18) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (19) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (20) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (21) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (22) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (23) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (24) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (25) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (26) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (27) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (28) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (29) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (30) << DCR_SHIFT_LH, SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | (31) << DCR_SHIFT_LH, SUB_RETURN},
	/* this is where the DCR register numbering goes weird */
	/* 32 through 63 */

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},

	/* 64 through 95 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},

	/* 96 through 127 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},

	/* 128 through 159 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},

	/* 160 through 191 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},

	/* 192 through 223 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},

	/* 224 through 255 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},

	/* 256 through 287 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},

	/* 288 through 319 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},

	/* 320 through 351 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},

	/* 352 through 383 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},

	/* 384 through 415 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},

	/* 416 through 447 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},

	/* 448 through 479 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},

	/* 480 through 511 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},

	/* 512 through 543 */
	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},

	{MTDCR_FIXED | GPR_SOURCE_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MTDCR_FIXED | GPR_SOURCE_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},

};

#if defined __GNUC__
#if defined XPAR_XIO_DCR_TEXT_SPACE
static DcrFunctionTableEntry
	__attribute__ ((section(".text"))) MfDcrTable[MAX_DCR_REGISTERS] =
#else
static DcrFunctionTableEntry MfDcrTable[MAX_DCR_REGISTERS] =
#endif
#else
static DcrFunctionTableEntry MfDcrTable[MAX_DCR_REGISTERS] =
#endif
{
	{MFDCR_FIXED | GPR_DEST_REG | (0) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (1) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (2) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (3) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (4) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (5) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (6) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (7) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (8) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (9) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (10) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (11) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (12) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (13) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (14) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (15) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (16) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (17) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (18) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (19) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (20) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (21) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (22) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (23) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (24) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (25) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (26) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (27) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (28) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (29) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (30) << DCR_SHIFT_LH, SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | (31) << DCR_SHIFT_LH, SUB_RETURN},

	/* this is where the DCR register numbering goes weird */
	/* 32 through 63 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 0) << DCR_SHIFT_UH), SUB_RETURN},

	/* 64 through 95 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 1) << DCR_SHIFT_UH), SUB_RETURN},

	/* 96 through 127 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 2) << DCR_SHIFT_UH), SUB_RETURN},

	/* 128 through 159 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 3) << DCR_SHIFT_UH), SUB_RETURN},

	/* 160 through 191 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 4) << DCR_SHIFT_UH), SUB_RETURN},

	/* 192 through 223 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 5) << DCR_SHIFT_UH), SUB_RETURN},

	/* 224 through 255 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 6) << DCR_SHIFT_UH), SUB_RETURN},

	/* 256 through 287 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 7) << DCR_SHIFT_UH), SUB_RETURN},

	/* 288 through 319 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 8) << DCR_SHIFT_UH), SUB_RETURN},

	/* 320 through 351 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 9) << DCR_SHIFT_UH), SUB_RETURN},

	/* 352 through 383 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 10) << DCR_SHIFT_UH), SUB_RETURN},

	/* 384 through 415 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 11) << DCR_SHIFT_UH), SUB_RETURN},

	/* 416 through 447 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 12) << DCR_SHIFT_UH), SUB_RETURN},

	/* 448 through 479 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 13) << DCR_SHIFT_UH), SUB_RETURN},

	/* 480 through 511 */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_32 + 14) << DCR_SHIFT_UH), SUB_RETURN},

	/* 512 and above */
	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 0) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 1) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 2) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 3) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 4) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 5) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 6) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 7) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 8) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 9) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 10) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 11) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 12) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 13) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 14) << DCR_SHIFT_UH), SUB_RETURN},

	{MFDCR_FIXED | GPR_DEST_REG | ((0) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((1) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((2) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((3) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((4) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((5) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((6) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((7) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((8) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((9) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((10) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((11) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((12) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((13) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((14) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((15) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((16) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((17) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((18) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((19) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((20) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((21) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((22) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((23) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((24) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((25) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((26) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((27) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((28) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((29) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((30) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
	{MFDCR_FIXED | GPR_DEST_REG | ((31) << DCR_SHIFT_LH) |
	 ((DCR_REG_512 + 15) << DCR_SHIFT_UH), SUB_RETURN},
};


/***************** Macros (Inline Functions) Definitions ********************/

/************************** Variable Definitions ****************************/



/************************** Function Prototypes *****************************/

/*****************************************************************************/
/**
*
* Outputs value provided to specified register defined in the header file.
*
* @param    DcrRegister is the intended destination DCR register
* @param    Data is the value to be placed into the specified DCR register
*
* @return
*
* None.
*
* @note
*
* None.
*
* @internal
*
* Assumes that the data is stored in the indicated source register defined
* above. Looking at the assembly output is required to verify this if the
* tool chain environment changes
*
****************************************************************************/
void XIo_DcrOut(u32 DcrRegister, u32 Data)
{
	/*
	 * Assert validates the register number
	 */
	XASSERT_VOID(DcrRegister < MAX_DCR_REGISTERS);

	/*
	 * If MIN_DCR_REGISTER is 0, then the assert conditional is meaningless
	 * so don't check it.
	 */
#if (MIN_DCR_REGISTER != 0)
	XASSERT_VOID(DcrRegister >= MIN_DCR_REGISTER);
#endif

	/*
	 * pass the call on to the proper function
	 */
	(void) ((XIo_DcrOpCode)
		(&(MtDcrTable[DcrRegister - MIN_DCR_REGISTER].OpCode)))
		(DcrRegister, Data);
}

/*****************************************************************************/
/**
*
* Reads value from specified register.
*
* @param    DcrRegister is the intended source DCR register
*
* @return
*
* Contents of the specified DCR register.
*
* @note
*
* None.
*
* @internal
*
* Assumes that the data is stored in the indicated source register defined
* in the header file. Looking at the assembly output is required to verify
* this if the tool chain environment changes
*
****************************************************************************/
u32 XIo_DcrIn(u32 DcrRegister)
{
	/*
	 * Assert validates the register number
	 */
	XASSERT_NONVOID(DcrRegister < MAX_DCR_REGISTERS);

	/*
	 * If MIN_DCR_REGISTER is 0, then the assert conditional is meaningless
	 * so don't check it.
	 */
#if (MIN_DCR_REGISTER != 0)
	XASSERT_NONVOID(DcrRegister >= MIN_DCR_REGISTER);
#endif

	/*
	 * pass the call on to the proper function
	 */
	return ((XIo_DcrOpCode)
		(&(MfDcrTable[DcrRegister - MIN_DCR_REGISTER].OpCode)))
		(DcrRegister, 0);
}
