/* $Id: xpseudo_asm_dcc.h,v 1.1.2.4 2009/05/01 17:49:21 moleres Exp $ */
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
* @file xpseudo_asm_dcc.h
*
* This header file contains macros for using inline assembler code. It is
* written specifically for the DIAB compiler.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a ch   06/18/02 First release
* 1.10a rpm  06/07/07 Moved from standalone BSP
* 1.11a ecm  04/15/09 Changed 'sync' to '__sync' to fix namespace collision
*					  with C++ streambuf.h include, CR511861
* </pre>
*
******************************************************************************/

#ifndef XPSEUDO_ASM_H  /* prevent circular inclusions */
#define XPSEUDO_ASM_H  /* by using protection macros */

/***************************** Include Files ********************************/

#include "xreg405.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

#define mtgpr(rn, v) MTGPR(rn, v)
#define MTGPR(rn, v) mtgpr_##rn(v)

#define mfgpr(rn) MFGPR(rn)
#define MFGPR(rn) mfgpr_##rn()

asm volatile void mtgpr_r0(v) {
% reg v;
!
 mr r0, v
}

asm volatile void mtgpr_r1(v) {
% reg v;
!
 mr r1, v
}

asm volatile void mtgpr_r2(v) {
% reg v;
!
 mr r2, v
}

asm volatile void mtgpr_r3(v) {
% reg v;
!
 mr r3, v
}

asm volatile void mtgpr_r4(v) {
% reg v;
!
 mr r4, v
}

asm volatile void mtgpr_r5(v) {
% reg v;
!
 mr r5, v
}

asm volatile void mtgpr_r6(v) {
% reg v;
!
 mr r6, v
}

asm volatile void mtgpr_r7(v) {
% reg v;
!
 mr r7, v
}

asm volatile void mtgpr_r8(v) {
% reg v;
!
 mr r8, v
}

asm volatile void mtgpr_r9(v) {
% reg v;
!
 mr r9, v
}

asm volatile void mtgpr_r10(v) {
% reg v;
!
 mr r10, v
}

asm volatile void mtgpr_r11(v) {
% reg v;
!
 mr r11, v
}

asm volatile void mtgpr_r12(v) {
% reg v;
!
 mr r12, v
}

asm volatile void mtgpr_r13(v) {
% reg v;
!
 mr r13, v
}

asm volatile void mtgpr_r14(v) {
% reg v;
!
 mr r14, v
}

asm volatile void mtgpr_r15(v) {
% reg v;
!
 mr r15, v
}

asm volatile void mtgpr_r16(v) {
% reg v;
!
 mr r16, v
}

asm volatile void mtgpr_r17(v) {
% reg v;
!
 mr r17, v
}

asm volatile void mtgpr_r18(v) {
% reg v;
!
 mr r18, v
}

asm volatile void mtgpr_r19(v) {
% reg v;
!
 mr r19, v
}

asm volatile void mtgpr_r20(v) {
% reg v;
!
 mr r20, v
}

asm volatile void mtgpr_r21(v) {
% reg v;
!
 mr r21, v
}

asm volatile void mtgpr_r22(v) {
% reg v;
!
 mr r22, v
}

asm volatile void mtgpr_r23(v) {
% reg v;
!
 mr r23, v
}

asm volatile void mtgpr_r24(v) {
% reg v;
!
 mr r24, v
}

asm volatile void mtgpr_r25(v) {
% reg v;
!
 mr r25, v
}

asm volatile void mtgpr_r26(v) {
% reg v;
!
 mr r26, v
}

asm volatile void mtgpr_r27(v) {
% reg v;
!
 mr r27, v
}

asm volatile void mtgpr_r28(v) {
% reg v;
!
 mr r28, v
}

asm volatile void mtgpr_r29(v) {
% reg v;
!
 mr r29, v
}

asm volatile void mtgpr_r30(v) {
% reg v;
!
 mr r30, v
}

asm volatile void mtgpr_r31(v) {
% reg v;
!
 mr r31, v
}

asm volatile int mfgpr_r0() {
! "r3"
  mr r3, r0
}

asm volatile int mfgpr_r1() {
! "r3"
  mr r3, r1
}

asm volatile int mfgpr_r2() {
! "r3"
  mr r3, r2
}

asm volatile int mfgpr_r3() {
! "r3"
  mr r3, r3
}

asm volatile int mfgpr_r4() {
! "r3"
  mr r3, r4
}

asm volatile int mfgpr_r5() {
! "r3"
  mr r3, r5
}

asm volatile int mfgpr_r6() {
! "r3"
  mr r3, r6
}

asm volatile int mfgpr_r7() {
! "r3"
  mr r3, r7
}

asm volatile int mfgpr_r8() {
! "r3"
  mr r3, r8
}

asm volatile int mfgpr_r9() {
! "r3"
  mr r3, r9
}

asm volatile int mfgpr_r10() {
! "r3"
  mr r3, r10
}

asm volatile int mfgpr_r11() {
! "r3"
  mr r3, r11
}

asm volatile int mfgpr_r12() {
! "r3"
  mr r3, r12
}

asm volatile int mfgpr_r13() {
! "r3"
  mr r3, r13
}

asm volatile int mfgpr_r14() {
! "r3"
  mr r3, r14
}

asm volatile int mfgpr_r15() {
! "r3"
  mr r3, r15
}

asm volatile int mfgpr_r16() {
! "r3"
  mr r3, r16
}

asm volatile int mfgpr_r17() {
! "r3"
  mr r3, r17
}

asm volatile int mfgpr_r18() {
! "r3"
  mr r3, r18
}

asm volatile int mfgpr_r19() {
! "r3"
  mr r3, r19
}

asm volatile int mfgpr_r20() {
! "r3"
  mr r3, r20
}

asm volatile int mfgpr_r21() {
! "r3"
  mr r3, r21
}

asm volatile int mfgpr_r22() {
! "r3"
  mr r3, r22
}

asm volatile int mfgpr_r23() {
! "r3"
  mr r3, r23
}

asm volatile int mfgpr_r24() {
! "r3"
  mr r3, r24
}

asm volatile int mfgpr_r25() {
! "r3"
  mr r3, r25
}

asm volatile int mfgpr_r26() {
! "r3"
  mr r3, r26
}

asm volatile int mfgpr_r27() {
! "r3"
  mr r3, r27
}

asm volatile int mfgpr_r28() {
! "r3"
  mr r3, r28
}

asm volatile int mfgpr_r29() {
! "r3"
  mr r3, r29
}

asm volatile int mfgpr_r30() {
! "r3"
  mr r3, r30
}

asm volatile int mfgpr_r31() {
! "r3"
  mr r3, r31
}

asm volatile void mtspr(rn, v) {
% reg v; con rn
!
  mtspr rn, v
}

asm volatile int mfspr(rn) {
% con rn
! "r3"
  mfspr r3, rn
}

asm volatile void mtdcr(rn, v) {
% reg v; con rn
!
  mtdcr rn, v
}

asm volatile int mfdcr(rn) {
% con rn
! "r3"
  mfdcr r3, rn
}

asm volatile void mtmsr(v) {
% reg v
!
  mtmsr v
}

asm volatile int mfmsr() {
! "r3"
  mfmsr r3
}

#define mtevpr(adr)     mtspr(XREG_SPR_EVPR, (adr))

#define iccci           asm volatile("iccci 0,0\n")
#define isync           asm volatile("isync\n")

asm volatile void icbi(adr) {
% reg adr
    icbi 0, adr
}

asm volatile void dccci(adr) {
% reg adr
    dccci 0, adr
}

asm volatile void dcbst(adr) {
% reg adr
    dcbst 0, adr
}

asm volatile void dcbf(adr) {
% reg adr
    dcbf 0, adr
}

asm volatile int dcread(adr) {
% reg adr
! "r3"
    dcread r3, 0, adr
}

#define __sync            asm volatile("sync\n")
/* memory operations */
#define eieio           asm volatile("eieio\n")

asm volatile char lbz(short adr) {
% reg adr
! "r3"
    lbz r3, 0(adr)
}

asm volatile short lhz(short adr) {
% reg adr
! "r3"
    lhz r3, 0(adr)
}

asm volatile int lwz(short adr) {
% reg adr
! "r3"
    lwz r3, 0(adr)
}

asm volatile void stb(short adr, char val) {
% reg adr; reg val;
!
    stb val, 0(adr)
}

asm volatile void sth(short adr, short val) {
% reg adr; reg val;
!
    sth val, 0(adr)
}

asm volatile void stw(short adr, int val) {
% reg adr; reg val;
!
    stw val, 0(adr)
}

asm volatile short lhbrx(int adr) {
% reg adr;
! "r3"
    lhbrx r3, 0, adr
}

asm volatile int lwbrx(int adr) {
% reg adr;
! "r3"
    lwbrx r3, 0, adr
}

asm volatile void sthbrx(int adr, short val) {
% reg adr; reg val;
!
    sthbrx val, 0, adr
}

asm volatile void stwbrx(int adr, int val) {
% reg adr; reg val;
!
    stwbrx val, 0, adr
}

#ifdef __cplusplus
}
#endif

#endif

