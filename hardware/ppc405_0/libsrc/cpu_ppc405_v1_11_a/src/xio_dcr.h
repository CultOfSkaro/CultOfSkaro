/* $Id: xio_dcr.h,v 1.1.2.3 2009/05/01 17:49:21 moleres Exp $ */
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
* @file xio_dcr.h
*
* The DCR I/O access functions.
*
* @note
*
* These access functions are specific to the PPC405 CPU. Changes might be
* necessary for other members of the IBM PPC Family.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a ecm  10/18/01 First release
* 1.10a wgr  03/22/07 Converted to new coding style.
* </pre>
*
* @internal
*
* The C functions which subsequently call into either the assembly code
* or into the provided table of functions are required since the registers
* assigned to the calling and return from functions are strictly defined
* in the ABI and that definition is used in the low-level functions
* directly. The use of macros is not recommended since the temporary
* registers in the ABI are defined but there is no way to force
* the compiler to use a specific register in a block of code.
*
* Also, the function table created at compile time is in the data space,
* not the text space. This leads to the following possible issues:
* 1. If the MMU is used, the data and instruction spaces must be mapped
*    the same. If this condition is not met, the 405 could start executing
*    random code.
* 2. If the MMU is used, the EXECUTE bit must be set in the table used
*    for the data space. If this is not the case, an exception will occur.
* 3. If the memory is configured as a true Harvard where instruction and
*    data spaces are on independent busses, the DCR code will fail since
*    the function table is not resident in the instruction space.
*
* There are macros available to solve these issues. They are located in
* xpseudo_asm_gcc.h (or xpseudo_asm_dcc.h if Diab is used).
*
******************************************************************************/

#ifndef XDCRIO_H		/* prevent circular inclusions */
#define XDCRIO_H		/* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif


/***************************** Include Files *********************************/
#include "xbasic_types.h"

/************************** Constant Definitions *****************************/


#define MAX_DCR_REGISTERS           1024
#define MAX_DCR_REGISTER            MAX_DCR_REGISTERS - 1
#define MIN_DCR_REGISTER            0

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

/*
 * Access Functions
 */
void XIo_DcrOut(u32 DcrRegister, u32 Data);
u32 XIo_DcrIn(u32 DcrRegister);

#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */
