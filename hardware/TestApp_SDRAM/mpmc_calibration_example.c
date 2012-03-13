#include "mpmc_calibration_example.h"
/************************** Variable Definitions *****************************/

static XMpmc	Mpmc;            /* Instance of the Mpmc driver */

/*
 * Buffers for storing and reading the test pattern.
 */
u32 WritePattern[MPMC_TEST_PATTERN_SIZE];
u32 ReadPattern[MPMC_TEST_PATTERN_SIZE];

/*****************************************************************************/
/**
*
* This is the main function for the MPMC example.
*
* @param	None.
*
* @return	XST_SUCCESS to indicate success, otherwise XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int mpmc_calibration_example_main(void)
{
	int Status;

	/*
	 * Run the MPMC Calibration example, specify the Device ID generated in
	 * xparameters.h.
	 */
	Status = MpmcCalibrationExample(MPMC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;

}

/*****************************************************************************/
/**
*
* The purpose of this function is to illustrate how to use the MPMC
* driver for the Calibration of the Static Phy.
*
* @param	DeviceId is device ID of the XMpmc Device, typically
*		XPAR_<MPMC_instance>_DEVICE_ID value from xparameters.h.
*
* @return	XST_SUCCESS to indicate success, otherwise XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int MpmcCalibrationExample(u16 DeviceId)
{
	XMpmc_Config *CfgPtr;
	int Status;
	MpmcCalibReturnValue CalibStatus;

	/*
	 * Disable the data cache and reinitialize it.
	 */
#ifdef __MICROBLAZE__
#if XPAR_MICROBLAZE_0_USE_DCACHE
	microblaze_disable_dcache();
	microblaze_init_dcache_range(0,
			XPAR_MICROBLAZE_0_DCACHE_BYTE_SIZE);
#endif
#endif

#ifdef __PPC__
	XCache_DisableDCache();
	XCache_InvalidateDCacheLine(MPMC_CALIBRATON_STARTADDR);
#endif

	xil_printf("\r\n Starting the Calibration Example \n\n");

	/*
	 * Initialize the MPMC device.
	 */
	CfgPtr = XMpmc_LookupConfig(DeviceId);
	if (CfgPtr == XNULL) {
		return XST_FAILURE;
	}

	Status = XMpmc_CfgInitialize(&Mpmc, CfgPtr, CfgPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait for the initial initialization sequence to be complete.
	 */
	while ((XMpmc_GetStaticPhyReg(&Mpmc) & XMPMC_SPIR_INIT_DONE_MASK) !=
						XMPMC_SPIR_INIT_DONE_MASK);

	/*
	 * Begin Calibration.
	 */
	Status = MpmcGetCalibrate(&Mpmc, MPMC_CALIBRATON_STARTADDR,
					&CalibStatus);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	/*
	 * Set the calibrated value in the Static Phy Register.
	 */
	if (CalibStatus.FoundValid == TRUE) {
		xil_printf("\r Setting the phase shift to %0d. ",
					CalibStatus.CalibTapValue);
		xil_printf("Min Value = %d. Max Value = %d.\r\n",
					CalibStatus.MinValue,
					CalibStatus.MaxValue);

		MpmcSetCalibrate(&Mpmc, CalibStatus.CalibTapValue,
					CalibStatus.RegValue);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

  	}

	xil_printf("\r\n Calibration is Successful \r\n");

	return XST_SUCCESS;
}


/****************************************************************************/
/**
*
* This function increments the DCM Phase shift by 1 tap.
*
* @param	InstancePtr is a pointer to an XMpmc instance to be worked on.
* @param	StaticRegValue is the initial value to be written to the Static
*		Phy register before the DCM PhaseShift decrement is done.
*
* @return	XST_SUCCESS if the DCM Phase shift increment is completed
*		else XST_FAILURE.
*
* @note		None.
*
*****************************************************************************/
int MpmcIncDcmPhaseShift(XMpmc * InstancePtr, u32 StaticRegValue)
{
	u32 RegValue;

	/*
	 * Write the given initial value into the Static Phy Register.
	 */
	RegValue = XMpmc_GetStaticPhyReg(InstancePtr) &
					(~(XMPMC_SPIR_RDEN_DELAY_MASK |
					   XMPMC_SPIR_RDDATA_CLK_SEL_MASK |
					   XMPMC_SPIR_RDDATA_SWAP_RISE_MASK));
	XMpmc_SetStaticPhyReg(InstancePtr, RegValue | StaticRegValue);



	/*
	 * Write back register settings with DCM PSEN and PSINCDEC enabled.
	 */
	RegValue = XMpmc_GetStaticPhyReg(InstancePtr);
	XMpmc_SetStaticPhyReg(InstancePtr, RegValue |
					XMPMC_SPIR_DCM_PSEN_MASK |
					XMPMC_SPIR_DCM_PSINCDEC_MASK);
	/*
	 * Wait for DCM Done.
	 */
	while ((XMpmc_GetStaticPhyReg(InstancePtr) &
					XMPMC_SPIR_DCM_DONE_MASK) == 0);

	/*
	 * Clear the DCM Done bit.
	 */
	RegValue = XMpmc_GetStaticPhyReg(InstancePtr);
	XMpmc_SetStaticPhyReg(InstancePtr , RegValue &
						(~ XMPMC_SPIR_DCM_DONE_MASK));

	/*
	 * Check if the DCM done bit is cleared.
	 */
	if ((XMpmc_GetStaticPhyReg(InstancePtr) &
					XMPMC_SPIR_DCM_DONE_MASK) != 0) {
		return XST_FAILURE;
	}


	return XST_SUCCESS;
}

/****************************************************************************/
/**
*
* This function decrements the DCM Phase shift by 1 tap.
*
* @param	InstancePtr is a pointer to an XMpmc instance to be worked on.
*
* @return	XST_SUCCESS if the DCM Phase shift decrement is completed
*		else XST_FAILURE.
*
* @note		None.
*
*****************************************************************************/
int MpmcDecDcmPhaseShift(XMpmc *InstancePtr)
{
	u32 RegValue;


	/*
	 * Set the Static Phy register to do DCM phase shift decrements.
	 */
	RegValue = XMpmc_GetStaticPhyReg(InstancePtr);
	XMpmc_SetStaticPhyReg(InstancePtr, (RegValue &
					(~ XMPMC_SPIR_DCM_PSINCDEC_MASK)) |
					XMPMC_SPIR_DCM_PSEN_MASK );

	/*
	 * Wait for DCM Done.
	 */
	while ((XMpmc_GetStaticPhyReg(InstancePtr) &
					XMPMC_SPIR_DCM_DONE_MASK) == 0);

	/*
	 * Clear the DCM Done bit.
	 */
	RegValue = XMpmc_GetStaticPhyReg(InstancePtr);
 	XMpmc_SetStaticPhyReg(InstancePtr,
 				RegValue & (~ XMPMC_SPIR_DCM_DONE_MASK));

	/*
	 * Check if the DCM Done bit is cleared.
	 */
	if ((XMpmc_GetStaticPhyReg(InstancePtr) &
					XMPMC_SPIR_DCM_DONE_MASK) != 0) {
		return XST_FAILURE;
	}


	return XST_SUCCESS;
}


/****************************************************************************/
/**
*
* This function resets the DCM Phase shift.
*
* @param	InstancePtr is a pointer to an XMpmc instance to be worked on.
*
* @return	XST_SUCCESS if the DCM Phase shift decrement is completed
*		else XST_FAILURE.
*
* @note		None.
*
*****************************************************************************/
int MpmcResetDcmPhaseShift(XMpmc *InstancePtr)
{
	u32 RegValue;
	int TapValue;
	int Status;
	int Index;


	/*
	 * Read the Static Phy register.
	 */
	RegValue = XMpmc_GetStaticPhyReg(InstancePtr);
	TapValue =  RegValue & XMPMC_SPIR_DCM_TAP_VALUE_MASK;


	if (TapValue > MPMC_MAX_TAPS) {
		TapValue = TapValue - 0x100;
	}

	for (Index = MPMC_MIN_TAPS; Index < TapValue;Index++) {
		Status = MpmcDecDcmPhaseShift(InstancePtr);
		if (Status != XST_SUCCESS){
			return XST_FAILURE;
		}
	}


	return XST_SUCCESS;
}

/****************************************************************************/
/**
*
* This function sets the value of the Static Phy Reg to the calculated
* calibration value..
*
* @param	InstancePtr is a pointer to an XMpmc instance to be worked on.
*
* @return	XST_SUCCESS if the DCM Phase shift decrement is completed
*		else XST_FAILURE.
*
* @note		None.
*
*****************************************************************************/
int MpmcSetCalibrate(XMpmc *InstancePtr, int TapValue, u32 RegValue)
{
	int OrigTapValue;
	int Index;
	int Status;

	OrigTapValue = XMpmc_GetStaticPhyReg(InstancePtr) &
				XMPMC_SPIR_DCM_TAP_VALUE_MASK;

	if (OrigTapValue > MPMC_MAX_TAPS) {
		OrigTapValue = OrigTapValue - 0x100;
	}

	if (OrigTapValue > TapValue) {
		for (Index = TapValue; Index < OrigTapValue;Index++) {
			MpmcUpdateStaticPhyReg (InstancePtr, RegValue);
			Status = MpmcDecDcmPhaseShift(InstancePtr);
			if (Status != XST_SUCCESS) {
				return XST_FAILURE;
			}

		}
	} else {
		for (Index = OrigTapValue; Index < TapValue;Index++) {
			Status = MpmcIncDcmPhaseShift(InstancePtr, RegValue);
			if (Status != XST_SUCCESS) {
				return XST_FAILURE;
			}
		}
	}

	if ((XMpmc_GetStaticPhyReg(InstancePtr) &
				XMPMC_SPIR_DCM_TAP_VALUE_MASK) !=
				(TapValue & XMPMC_SPIR_DCM_TAP_VALUE_MASK)){
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}


/****************************************************************************/
/**
*
* This function gets the calibration value.
*
* @param	InstancePtr is a pointer to an XMpmc instance to be worked on.
*
* @return	XST_SUCCESS if the DCM Phase shift decrement is completed
*		else XST_FAILURE.
*
* @note		None.
*
*****************************************************************************/
int MpmcGetCalibrate(XMpmc *InstancePtr, u32 MemoryStartAddr,
			MpmcCalibReturnValue *CalibStatus)
{
	int Count;
	MpmcCalibValue LocalCalibValue;
	int ValidCount;
	u32 RegValue;
	u8 Toggle = 0;
	u32 Status;

	LocalCalibValue.FoundValid = FALSE;
	RegValue = MPMC_RDEN_DELAY_MIN_VAL;

	/*
	 * Get the initial value from the register.
	 */
	LocalCalibValue.OrigTapValue = XMpmc_GetStaticPhyReg(InstancePtr) &
						XMPMC_SPIR_DCM_TAP_VALUE_MASK;
	if (LocalCalibValue.OrigTapValue > MPMC_MAX_TAPS) {
		LocalCalibValue.OrigTapValue =
				LocalCalibValue.OrigTapValue - 0x100;
	}

	/*
	 * Try to find valid calibration settings.
	 */
	while (LocalCalibValue.FoundValid == FALSE) {

		LocalCalibValue.MinValue = MPMC_MAX_TAPS - 1;
		LocalCalibValue.MaxValue = MPMC_MIN_TAPS + 1;

		/*
		 * Reset DCM to minimum value
		 */
		Status = MpmcResetDcmPhaseShift(InstancePtr);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}


		/*
		 * Read the memory and check that data versus the expected
		 * value.
		 *
		 * If the value matches, read the current calibration value.
		 * Repeat this to ensure that this is not on a transition.
		 * If the value is incorrect in any of these increments, the
		 * value must be re-established.
		 *
		 * If the value at 0 does match then we are probably in a case
		 * where the window is a wrap case.  This means that we are
		 * looking for the upper bound not the lower bound.
		 */
		ValidCount = 0;
		for (Count = MPMC_MIN_TAPS; Count < MPMC_MAX_TAPS; Count++) {

			MpmcWriteTestPattern(MemoryStartAddr, Toggle);


			if (MpmcCheckPattern(MemoryStartAddr, Toggle) !=
						XST_SUCCESS) {

				/*
				 * Found invalid calibration setting.
				 */
				if (ValidCount > MPMC_EDGE_TAPS) {
					LocalCalibValue.MaxValue = Count;
				}
				ValidCount = 0;
			} else {

				/*
				 * Found valid calibration setting
				 * Check to make sure that cache reads work.
				 */

#ifdef __MICROBLAZE__
#if XPAR_MICROBLAZE_0_USE_DCACHE
				/*
				 * Initialize and Enable the data cache.
				 */
				microblaze_init_dcache_range(0,
					XPAR_MICROBLAZE_0_DCACHE_BYTE_SIZE);
				microblaze_enable_dcache();
#endif
#endif

#ifdef __PPC__
				/*
				 * Enable the Dcache for all memory regions
				 * except the boot region of the PPC.
				 */
				XCache_InvalidateDCacheLine(MemoryStartAddr);
				XCache_EnableDCache(0xFFFFFFFE);

#endif
				Status = MpmcCheckPattern(MemoryStartAddr,
								Toggle);

				/*
				 * Disable and reinitialize the data cache.
				 */
#ifdef __MICROBLAZE__
#if XPAR_MICROBLAZE_0_USE_DCACHE
				microblaze_disable_dcache();
				microblaze_init_dcache_range(0,
					XPAR_MICROBLAZE_0_DCACHE_BYTE_SIZE);
#endif
#endif

#ifdef __PPC__
				XCache_DisableDCache();
				XCache_InvalidateDCacheLine(MemoryStartAddr);
#endif


				if (Status != XST_SUCCESS) {
					/*
					 * Found invalid calibration setting
					 */
					if (ValidCount > MPMC_EDGE_TAPS) {
						LocalCalibValue.MaxValue =
								Count;
					}
					ValidCount = 0;
				} else {
					/*
					 * Found valid calibration setting
					 */
					if (ValidCount == MPMC_EDGE_TAPS) {
						LocalCalibValue.MinValue =
							Count - MPMC_EDGE_TAPS;
						LocalCalibValue.MaxValue =
									Count;
					}
					else if (ValidCount > MPMC_EDGE_TAPS) {
						LocalCalibValue.MaxValue =
									Count;
					}
					ValidCount++;
					if (ValidCount>MPMC_EDGE_TAPS) {
						LocalCalibValue.FoundValid =
								TRUE;
					}
				}
			}

			Status = MpmcIncDcmPhaseShift(InstancePtr, RegValue);
			if (Status != XST_SUCCESS) {
				return XST_FAILURE;
			}

			if (Toggle == 0) {
				Toggle = 1;
			} else {
				Toggle = 0;
			}
		}

		if ((LocalCalibValue.FoundValid == FALSE) &&
			(((RegValue & XMPMC_SPIR_RDEN_DELAY_MASK)
					!= MPMC_RDEN_DELAY_MAX_VAL) ||
			((RegValue & XMPMC_SPIR_RDDATA_CLK_SEL_MASK) !=
					XMPMC_SPIR_RDDATA_CLK_SEL_MASK) ||
			((RegValue & XMPMC_SPIR_RDDATA_SWAP_RISE_MASK) !=
					XMPMC_SPIR_RDDATA_SWAP_RISE_MASK))){
			if ((RegValue & XMPMC_SPIR_RDEN_DELAY_MASK) !=
					MPMC_RDEN_DELAY_MAX_VAL) {
				RegValue = RegValue + MPMC_RDEN_DELAY_INC;
			}
			else if ((RegValue & XMPMC_SPIR_RDDATA_CLK_SEL_MASK) !=
					XMPMC_SPIR_RDDATA_CLK_SEL_MASK) {
				RegValue =
					(RegValue |
					XMPMC_SPIR_RDEN_DELAY_MASK) -
					XMPMC_SPIR_RDEN_DELAY_MASK +
					MPMC_RDEN_DELAY_MIN_VAL +
					XMPMC_SPIR_RDDATA_CLK_SEL_MASK;
			}
			else if ((RegValue & XMPMC_SPIR_RDDATA_SWAP_RISE_MASK)
			!=
					XMPMC_SPIR_RDDATA_SWAP_RISE_MASK) {
				RegValue =
					(RegValue |
					XMPMC_SPIR_RDEN_DELAY_MASK) -
					XMPMC_SPIR_RDEN_DELAY_MASK +
					MPMC_RDEN_DELAY_MIN_VAL -
					XMPMC_SPIR_RDDATA_CLK_SEL_MASK +
					XMPMC_SPIR_RDDATA_SWAP_RISE_MASK;
			}
		} else if (LocalCalibValue.FoundValid == FALSE) {
			xil_printf("\r\n ERROR: Could not calibrate.\r\n");
				return XST_FAILURE;
		}
	}

	CalibStatus->FoundValid = LocalCalibValue.FoundValid;
	CalibStatus->MinValue = LocalCalibValue.MinValue;
	CalibStatus->MaxValue = LocalCalibValue.MaxValue;
	CalibStatus->RegValue = RegValue;

	if (LocalCalibValue.MaxValue >= LocalCalibValue.MinValue) {
		CalibStatus->CalibTapValue = (LocalCalibValue.MinValue +
						LocalCalibValue.MaxValue)/2;
	} else {
		CalibStatus->CalibTapValue = (LocalCalibValue.MinValue +
					LocalCalibValue.MaxValue +
					MPMC_NUMBER_TAPS)/2;

		if (CalibStatus->CalibTapValue > MPMC_MAX_TAPS) {
			CalibStatus->CalibTapValue =
				(CalibStatus->CalibTapValue - MPMC_NUMBER_TAPS);
		}
	}

	return XST_SUCCESS;
}

/****************************************************************************/
/**
*
* This function writes the test pattern to the Memory.
*
* @param	MemoryStartAddr specifies the starting address of the memory to
*		which the test pattern is written.
* @param	Pattern specifies one of the two test patterns that are written
*		to the memory.
*
* @return	None
*
* @note		None.
*
*****************************************************************************/
void MpmcWriteTestPattern(u32 MemoryStartAddr, u8 Pattern)
{
	u32 WritePattern[MPMC_TEST_PATTERN_SIZE];
	u32 Index =0;

	if (Pattern == 0) {
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_0;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_1;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_2;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_3;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_4;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_5;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_6;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_7;
	}

	if (Pattern == 1) {
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_8;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_9;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_10;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_11;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_12;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_13;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_14;
		WritePattern[Index++] = MPMC_TAP_PATTERN_DWORD_15;
	}

	for (Index=0;Index<8;Index++) {
		XIo_Out32(MemoryStartAddr + (Index * 4),WritePattern[Index]);
	}
}

/****************************************************************************/
/**
*
* This function reads the test pattern from the Memory and compares it
* with the pattern that was written to the memory.
*
* @param	MemoryStartAddr specifies the starting address of the memory to
*		from which the test pattern is read.
* @param	Pattern specifies one of the two test patterns that is
*		to be compared.
*
* @return	None
*
* @note		None.
*
*****************************************************************************/
int MpmcCheckPattern(u32 MemoryStartAddr, u8 Pattern)
{
	u32 ReadPattern[MPMC_TEST_PATTERN_SIZE];
	u32 Index =0;


	for (Index=0; Index < 8; Index++) {
		ReadPattern[Index] = XIo_In32(MemoryStartAddr + (Index*4));
	}

	if (Pattern == 0) {
		if ((((ReadPattern[0] & MPMC_CALIB0_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_0 &
					MPMC_CALIB0_MASK)) ||
			((ReadPattern[1] & MPMC_CALIB1_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_1 &
					MPMC_CALIB1_MASK)) ||
			((ReadPattern[2] & MPMC_CALIB0_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_2 &
					MPMC_CALIB0_MASK)) ||
			((ReadPattern[3] & MPMC_CALIB1_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_3 &
					MPMC_CALIB1_MASK)) ||
			((ReadPattern[4] & MPMC_CALIB0_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_4 &
					MPMC_CALIB0_MASK)) ||
			((ReadPattern[5] & MPMC_CALIB1_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_5 &
					MPMC_CALIB1_MASK)) ||
			((ReadPattern[6] & MPMC_CALIB0_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_6 &
					MPMC_CALIB0_MASK)) ||
			((ReadPattern[7] & MPMC_CALIB1_MASK) !=
					(MPMC_TAP_PATTERN_DWORD_7 &
					MPMC_CALIB1_MASK)))) {

			return XST_FAILURE;
		}
		else {
			return XST_SUCCESS;
		}

	}

	if (Pattern == 1) {
		if ((((ReadPattern[0] & MPMC_CALIB0_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_8 &
					MPMC_CALIB0_MASK)) ||
			((ReadPattern[1] & MPMC_CALIB1_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_9 &
					MPMC_CALIB1_MASK)) ||
			((ReadPattern[2] & MPMC_CALIB0_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_10 &
					MPMC_CALIB0_MASK)) ||
			((ReadPattern[3] & MPMC_CALIB1_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_11 &
					MPMC_CALIB1_MASK)) ||
			((ReadPattern[4] & MPMC_CALIB0_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_12 &
					MPMC_CALIB0_MASK)) ||
			((ReadPattern[5] & MPMC_CALIB1_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_13 &
					MPMC_CALIB1_MASK)) ||
			((ReadPattern[6] & MPMC_CALIB0_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_14 &
					MPMC_CALIB0_MASK)) ||
			((ReadPattern[7] & MPMC_CALIB1_MASK) !=
				(MPMC_TAP_PATTERN_DWORD_15 &
					MPMC_CALIB1_MASK)))) {

			return XST_FAILURE;
		}
		else {
			return XST_SUCCESS;
		}
	}

	return XST_FAILURE;

}


/****************************************************************************/
/**
*
* This function updates the Static Phy Register with the RegValue provided.
*
* @param	InstancePtr is a pointer to an XMpmc instance to be worked on.
* @param	RegValue is the value to be written to the Static Phy Register.
*
* @return	None
*
* @note		None.
*
*****************************************************************************/
static void MpmcUpdateStaticPhyReg (XMpmc *InstancePtr, u32 RegValue)
{
  	u32  OrigValue;

	/*
	 * Write the given initial value into the Static Phy Register.
	 */
	OrigValue = XMpmc_GetStaticPhyReg(InstancePtr) &
					(~(XMPMC_SPIR_RDEN_DELAY_MASK |
					   XMPMC_SPIR_RDDATA_CLK_SEL_MASK |
					   XMPMC_SPIR_RDDATA_SWAP_RISE_MASK));
	XMpmc_SetStaticPhyReg(InstancePtr, OrigValue | RegValue);

}

