/* $Id: mpmc_calibration_example.c,v 1.1.2.1 2009/07/14 07:56:45 svemula Exp $ */
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
*       (c) Copyright 2007 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
* @file mpmc_calibration_example.c
*
* This file contains a design example using the MPMC device driver and hardware
* device for Calibrating the Static Phy. This example runs on both PowerPC
* and MicroBlaze.
*
* This code needs to be executed out of BRAM at startup.
*
* Ideally all memory reads and writes should be requested from port 0 of MPMC.
* If this is not possible then MPMC_RDEN_DELAY_MIN_VAL and
* MPMC_RDEN_DELAY_MAX_VAL definitions in this file should be modified to
* restrict the min/max RDEN_DELAY settings. Having too large or small of a
* value could result in a processor hang that is only recoverable by resetting
* the system.
*
* The DCM Tap values will differ based on the FPGA FAMILY type and the Frequency
* of operation, the MPMC_MIN_TAPS and MPMC_MAX_TAPS defined in this file should
* be defined accordingly.  Refer the FPGA user guide of the specific family for
* these ranges.
*
* This calibration example has been tested on a S3E500 starter kit.
*
* @note		None
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a jl   09/07/07 First Release.
* 1.00a sv   11/07/07 Doxygen/Coding style changes.
* </pre>
*
******************************************************************************/

#include "xparameters.h"
#include "xmpmc.h"
#include "stdio.h"
#include "xutil.h"

#ifdef __MICROBLAZE__
#include "mb_interface.h"
#endif
#ifdef __PPC__
#include "xcache_l.h"
#endif

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define MPMC_DEVICE_ID		XPAR_MPMC_0_DEVICE_ID
#define MPMC_BASEADDR		XPAR_MPMC_0_MPMC_BASEADDR

/*
 * Start Address of the Memory used for Calibration. Normally it is
 * the starting address of the memory connected to MPMC.
 */
#define MPMC_CALIBRATON_STARTADDR	MPMC_BASEADDR

/*
 * Use a smaller footprint printf so that the program will fit in BRAM
 */
#define printf xil_printf


/*
 * Defines range of DCM Tap values. These values will differ based on the
 * FPGA FAMILY type and the frequency of operation.
 * Refer the FPGA user guide of the specific family for these ranges.
 * MPMC_NUMBER_TAPS needs to be less than 256.
 * MPMC_EDGE_TAPS should be set to the minimum acceptable range of valid taps.
 */
#define MPMC_MIN_TAPS			-90
#define MPMC_MAX_TAPS			90
#define MPMC_EDGE_TAPS			20
#define MPMC_NUMBER_TAPS		MPMC_MAX_TAPS - MPMC_MIN_TAPS

/*
 * This is the setting the should be used if calibration read/writes will be on
 * port 0 only.  It may be acceptable to have these settings if it is only
 * cache transfers that will be on a different port since cache transfers are
 * only executed once the word transfers have a valid setting.
 */
#define MPMC_RDEN_DELAY_MIN_VAL		0x00000000
#define MPMC_RDEN_DELAY_MAX_VAL		0xF0000000
#define MPMC_RDEN_DELAY_INC		0x10000000


/*
 * Specify byte lanes to calibrate over
 */
#define MPMC_CALIB0_MASK		0xFFFFFFFF
#define MPMC_CALIB1_MASK		0xFFFFFFFF


/*
 * Data patterns for calibration
 */
#define MPMC_TAP_PATTERN_DWORD_0    0x00000000
#define MPMC_TAP_PATTERN_DWORD_1    0xFFFFFFFF
#define MPMC_TAP_PATTERN_DWORD_2    0x55555555
#define MPMC_TAP_PATTERN_DWORD_3    0xAAAAAAAA
#define MPMC_TAP_PATTERN_DWORD_4    0x33333333
#define MPMC_TAP_PATTERN_DWORD_5    0xCCCCCCCC
#define MPMC_TAP_PATTERN_DWORD_6    0x11111111
#define MPMC_TAP_PATTERN_DWORD_7    0x22222222
#define MPMC_TAP_PATTERN_DWORD_8    0x17171717
#define MPMC_TAP_PATTERN_DWORD_9    0x71717171
#define MPMC_TAP_PATTERN_DWORD_10   0x2D2D2D2D
#define MPMC_TAP_PATTERN_DWORD_11   0xD2D2D2D2
#define MPMC_TAP_PATTERN_DWORD_12   0x4B4B4B4B
#define MPMC_TAP_PATTERN_DWORD_13   0xB4B4B4B4
#define MPMC_TAP_PATTERN_DWORD_14   0x87878787
#define MPMC_TAP_PATTERN_DWORD_15   0x78787878

/*
 * The number of words in the Test Pattern written to the memory.
 */
#define MPMC_TEST_PATTERN_SIZE		8

/**************************** Type Definitions *******************************/

typedef struct {
	int  FoundValid;	/* Valid Calibration */
	int  OrigTapValue;	/* Initial Tap Value */
	int  MinValue;		/* Minimum Tap Value */
	int  MaxValue;		/* Maximum Tap Value */
} MpmcCalibValue;

typedef struct {
	int  FoundValid;	/* Valid Calibration */
	int  MinValue;		/* Minimum Tap Value */
	int  MaxValue;		/* Maximum Tap Value */
	int  CalibTapValue;	/* Calibrated Tap Value */
	u32  RegValue;		/* Static Phy Value when calibration is done */
} MpmcCalibReturnValue;

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

int MpmcCalibrationExample(u16 DeviceId);

int MpmcIncDcmPhaseShift(XMpmc * InstancePtr, u32 StaticRegValue);

int MpmcDecDcmPhaseShift(XMpmc * InstancePtr);

int MpmcSetCalibrate(XMpmc *InstancePtr, int TapValue, u32 RegValue);

int MpmcGetCalibrate(XMpmc *InstancePtr, u32 MemoryStartAddr,
			MpmcCalibReturnValue *CalibStatus);

int MpmcCheckPattern(u32 MemoryStartAddr, u8 Pattern);

void MpmcWriteTestPattern(u32 MemoryStartAddr, u8 Pattern);

int MpmcResetDcmPhaseShift(XMpmc *InstancePtr);

static void MpmcUpdateStaticPhyReg (XMpmc *InstancePtr, u32 RegValue);
