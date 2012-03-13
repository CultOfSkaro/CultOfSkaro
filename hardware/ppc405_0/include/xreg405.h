/* $Id: xreg405.h,v 1.1.2.3 2009/05/01 17:49:21 moleres Exp $ */
/******************************************************************************
*
* (c) Copyright 2004-2009 Xilinx, Inc. All rights reserved.
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
* @file xreg405.h
*
* This header file contains definitions for using inline assembler code. It is
* written specifically for the GNU compiler.
*
* All of the PPC405 GPR's, SPR's, Timers, Core Configuration Registers, and
* Debug Registers are defined along with the positions of the bits within each
* register.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.10a rpm  06/07/07 Moved from standalone BSP and updated to standards
* </pre>
*
******************************************************************************/
#ifndef XREG405_H
#define XREG405_H

#ifdef __cplusplus
extern "C" {
#endif

#define XREG_GPR0                           r0
#define XREG_GPR1                           r1
#define XREG_GPR2                           r2
#define XREG_GPR3                           r3
#define XREG_GPR4                           r4
#define XREG_GPR5                           r5
#define XREG_GPR6                           r6
#define XREG_GPR7                           r7
#define XREG_GPR8                           r8
#define XREG_GPR9                           r9
#define XREG_GPR10                          r10
#define XREG_GPR11                          r11
#define XREG_GPR12                          r12
#define XREG_GPR13                          r13
#define XREG_GPR14                          r14
#define XREG_GPR15                          r15
#define XREG_GPR16                          r16
#define XREG_GPR17                          r17
#define XREG_GPR18                          r18
#define XREG_GPR19                          r19
#define XREG_GPR20                          r20
#define XREG_GPR21                          r21
#define XREG_GPR22                          r22
#define XREG_GPR23                          r23
#define XREG_GPR24                          r24
#define XREG_GPR25                          r25
#define XREG_GPR26                          r26
#define XREG_GPR27                          r27
#define XREG_GPR28                          r28
#define XREG_GPR29                          r29
#define XREG_GPR30                          r30
#define XREG_GPR31                          r31

#define XREG_SPR_TBL_READ                   0x10c
#define XREG_SPR_TBU_READ                   0x10d
#define XREG_SPR_TBL_WRITE                  0x11c
#define XREG_SPR_TBU_WRITE                  0x11d
#define XREG_SPR_TSR                        0x3d8
#define XREG_SPR_TCR                        0x3da
#define XREG_SPR_PIT                        0x3db
#define XREG_SPR_ICCR                       0x3fb
#define XREG_SPR_DCCR                       0x3fa
#define XREG_SPR_DCWR                       0x3ba
#define XREG_SPR_SGR                        0x3b9
#define XREG_SPR_SU0R                       0x3bc
#define XREG_SPR_SLER                       0x3bb
#define XREG_SPR_CCR0                       0x3b3
#define XREG_SPR_EVPR                       0x3d6

/* Machine Status Register (MSR) Bits */
#define XREG_MSR_APU_AVAILABLE                  0x02000000
#define XREG_MSR_APU_ENABLE                     0x00080000
#define XREG_MSR_WAIT_STATE_ENABLE              0x00040000
#define XREG_MSR_WAIT_STATE_ENABLE_BIT_POS      13
#define XREG_MSR_CRITICAL_INTERRUPT_ENABLE      0x00020000
#define XREG_MSR_NON_CRITICAL_INTERRUPT_ENABLE  0x00008000
#define XREG_MSR_USER_MODE                      0x00004000
#define XREG_MSR_FLOATING_POINT_AVAILABLE       0x00002000
#define XREG_MSR_MACHINE_CHECK_ENABLE           0x00001000
#define XREG_MSR_FLOATING_POINT_EXCEPTION_MODE0 0x00000800
#define XREG_MSR_DEBUG_WAIT_ENABLE              0x00000400
#define XREG_MSR_DEBUG_INTERRUPT_ENABLE         0x00000200
#define XREG_MSR_FLOATING_POINT_EXCEPTION_MODE1 0x00000100
#define XREG_MSR_INSTRUCTION_RELOCATION_ENABLE  0x00000020
#define XREG_MSR_DATA_RELOCATION_ENABLE         0x00000010

/* Timer Control Register (TCR) Bits */
#define XREG_TCR_WDT_INTERRUPT_ENABLE           0x08000000
#define XREG_TCR_PIT_INTERRUPT_ENABLE           0x04000000
#define XREG_TCR_FIT_INTERRUPT_ENABLE           0x00800000
#define XREG_TCR_AUTORELOAD_ENABLE              0x00400000

/* WTD Timer Period Settings */
#define XREG_TCR_WDT_PERIOD_11                  0xc0000000
#define XREG_TCR_WDT_PERIOD_10                  0x80000000
#define XREG_TCR_WDT_PERIOD_01                  0x40000000
#define XREG_TCR_WDT_PERIOD_00                  0x00000000

/* WTD Reset Control Settings */
#define XREG_TCR_WDT_RESET_CONTROL_11           0x30000000
#define XREG_TCR_WDT_RESET_CONTROL_10           0x20000000
#define XREG_TCR_WDT_RESET_CONTROL_01           0x10000000
#define XREG_TCR_WDT_RESET_CONTROL_00           0x00000000

/* FIT Timer Period Settings */
#define XREG_TCR_FIT_PERIOD_11                  0x03000000
#define XREG_TCR_FIT_PERIOD_10                  0x02000000
#define XREG_TCR_FIT_PERIOD_01                  0x01000000
#define XREG_TCR_FIT_PERIOD_00                  0x00000000

/* Timer Status Register (TSR) Bits */
#define XREG_TSR_WDT_ENABLE_NEXT_WATCHDOG       0x80000000
#define XREG_TSR_WDT_INTERRUPT_STATUS           0x40000000
#define XREG_TSR_WDT_RESET_STATUS_11            0x30000000
#define XREG_TSR_WDT_RESET_STATUS_10            0x20000000
#define XREG_TSR_WDT_RESET_STATUS_01            0x10000000
#define XREG_TSR_WDT_RESET_STATUS_00            0x00000000
#define XREG_TSR_PIT_INTERRUPT_STATUS           0x08000000
#define XREG_TSR_FIT_INTERRUPT_STATUS           0x04000000
#define XREG_TSR_CLEAR_ALL                      0xffffffff

/* Core-Configuration Register (CCR0) Bits */
#define XREG_CCR0_LOAD_WORD_AS_LINE             0x02000000
#define XREG_CCR0_LOAD_WITHOUT_ALLOCATE         0x01000000
#define XREG_CCR0_STORE_WITHOUT_ALLOCATE        0x00800000
#define XREG_CCR0_DCU_PLB_PRIORITY_1            0x00400000
#define XREG_CCR0_ICU_PLB_PRIORITY_0            0x00200000
#define XREG_CCR0_ICU_PLB_PRIORITY_1            0x00100000
#define XREG_CCR0_ENABLE_U0_EXCEPTION           0x00020000
#define XREG_CCR0_LOAD_DEBUG_ENABLE             0x00010000
#define XREG_CCR0_PREFETCH_CACHEABLE            0x00000800
#define XREG_CCR0_PREFETCH_NON_CACHEABLE        0x00000400
#define XREG_CCR0_NON_CACHEABLE_REQ_SIZE        0x00000200
#define XREG_CCR0_FETCH_WITHOUT_ALLOCATE        0x00000100
#define XREG_CCR0_CACHE_INFORMATION_SELECT      0x00000010
#define XREG_CCR0_CACHE_WAY_SELECT              0x00000001

/* 405GP specific registers (for Walnut) */
#define XREG_DCR_CPC0_CR1               0xb2

#ifdef __cplusplus
}
#endif

#endif
