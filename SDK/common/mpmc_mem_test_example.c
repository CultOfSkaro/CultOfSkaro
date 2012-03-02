#include "mpmc_mem_test_example.h"
/************************** Variable Definitions ******************************/

static XMpmc Mpmc;			/* Instance of the Mpmc driver */

static u32 PassCount = 1;		/* Current iteration */

/*
 * This array specifies the offset from the Memory Baseaddress, for each MPMC
 * instance, from which the memory tests have to be done. By default these
 * values are set to 0.
 * Offset can be used to run the program out of the memory to test and not
 * clobber our software. Set this value to power of 2 larger than the size of
 * this program.
 * Offset should be set to a value less than half the size of the
 * MPMC memory for 'Maximum Bank Address/Row/Column noise' test (MPMC_TEST_3)
 * and to a value less than the size of the MPMC memory for remaining tests.
 */
const u32 MemOffset[MAX_INSTANCES] = {
	0x00200000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/*
 * The following variables are used to read from the MPMC memory, these are
 * defined as global to avoid having large buffers on the stack.
 */
#if (MPMC_TEST & MPMC_TEST_3)
 u32 DataBuffer[32];
#endif /* MPMC_TEST & MPMC_TEST_3 */

/******************************************************************************/
/**
*
* Main function to call the MPMC memory test example.
*
* @param	None.
*
* @return	Total number of errors for all iterations.
*
* @note		None.
*
*******************************************************************************/
int mpmc_mem_test_example_main(void)
{
	u16 DeviceId = 0;
	u32 TotalErrors = 0;

	while (DeviceId < MPMC_NUM_INSTANCES) {
		TotalErrors += MpmcMemTestExample(DeviceId++);
	}

	if (TotalErrors) {
		xil_printf("\r\n### Program finished with errors ###\r\n");
	} else {
		xil_printf("\r\n### Program finished successfully ###\r\n");
	}
	return TotalErrors;
}

/******************************************************************************/
/**
*
* This function runs the memory tests on a Mpmc instance specified by DeviceId.
*
* @param	DeviceId is device ID of the XMpmc Device, typically
*		XPAR_<MPMC_instance>_DEVICE_ID value from xparameters.h.
*
* @return	Total number of errors for all iterations, for a single Mpmc
*		instance.
*
* @note		None.
*
*******************************************************************************/
int MpmcMemTestExample(u16 DeviceId)
{
	XMpmc_Config *CfgPtr;
	u32 MemError = 0;
	u32 TotalErrors = 0;
	int Status;

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
	 * If NUM_ITERATIONS set to -1, loop will be infinite.
	 */
	while (PassCount <= NUM_ITERATIONS || NUM_ITERATIONS == -1) {
		xil_printf("\n\r\nMulti-Port Memory Controller Memory Test");
		xil_printf("\r\nTesting address range 0x%08X-0x%08X.",
			Mpmc.ConfigPtr.MemBaseAddress + MemOffset[DeviceId],
			Mpmc.ConfigPtr.MemHighAddress);
		xil_printf("\r\nIteration %d", PassCount);
		if (NUM_ITERATIONS != -1) {
			xil_printf(" of %d", NUM_ITERATIONS);
		}

#if (MPMC_CACHE_TEST & MPMC_CACHE_TEST_0)
		ENABLE_ICACHE();
		ENABLE_DCACHE();
		xil_printf("\r\nPass A) ICache:  On, DCache:  On");
		MemError = MpmcMemTest(&Mpmc);
		xil_printf("\r\n\t\tNumber of errors in this pass = %d\r\n",
			MemError);
		TotalErrors += MemError;
#endif /* MPMC_CACHE_TEST & MPMC_CACHE_TEST_0 */

#if (MPMC_CACHE_TEST & MPMC_CACHE_TEST_1)
		DISABLE_ICACHE();
		DISABLE_DCACHE();
		xil_printf("\r\nPass B) ICache: Off, DCache: Off");
		MemError = MpmcMemTest(&Mpmc);
		xil_printf("\r\n\t\tNumber of errors in this pass = %d\r\n",
			MemError);
		TotalErrors += MemError;
#endif /* MPMC_CACHE_TEST & MPMC_CACHE_TEST_1 */

#if (MPMC_CACHE_TEST & MPMC_CACHE_TEST_2)
		ENABLE_ICACHE();
		DISABLE_DCACHE();
		xil_printf("\r\nPass C) ICache:  On, DCache: Off");
		MemError = MpmcMemTest(&Mpmc);
		xil_printf("\r\n\tNumber of errors in this pass = %d\r\n",
			MemError);
		TotalErrors += MemError;
#endif /* MPMC_CACHE_TEST & MPMC_CACHE_TEST_2 */

#if (MPMC_CACHE_TEST & MPMC_CACHE_TEST_3)
		DISABLE_ICACHE();
		ENABLE_DCACHE();
		xil_printf("\r\nPass D) ICache: Off, DCache:  On");
		MemError = MpmcMemTest(&Mpmc);
		xil_printf("\r\n\tNumber of errors in this pass = %d\r\n",
			MemError);
		TotalErrors += MemError;
#endif /* MPMC_CACHE_TEST & MPMC_CACHE_TEST_0 */

		xil_printf("\r\nMPMC memory test ");
		xil_printf("iteration #%d ", PassCount);

		if (TotalErrors == 0) {
			xil_printf("has PASSED!");
		} else {
			xil_printf("has FAILED!");
		}
		PassCount++;
	}

	xil_printf("\r\nTotal number of errors for all iterations = %d",
		TotalErrors);

	return TotalErrors;
}

/******************************************************************************/
/**
*
* This function runs the MPMC memory tests for a MPMC instance. It returns back
* 0 if there were no errors, else it returns the number of memory errors.
*
* @param 	Mpmc is a pointer to the instance of MPMC to be worked on.
*
* @return
*		 - 0 - No Errors.
*		 - Greater than 1 indicates the number of errors detected.
*
* @note		None.
*
*******************************************************************************/
static u32 MpmcMemTest(XMpmc *Mpmc)
{
	u32 MyAdr;
	u32 StartAdr;
	u32 MemSize;
	u32 Range;
	u32 Offset;
	u32 Error = 0;
	u32 TotalErrors = 0;
	u32 DataRead;
	u32 WriteAdr;
	int i;
	int j;
#if (MPMC_TEST & MPMC_TEST_3)
	u32 BankAddr;
	u32 Row;
	u32 Col;
	u16 MemWidth;
	u16 StartBit;
	u32 RowAddrMask;
	u32 ColAddrMask;
	u32 BankAddrMask;
#endif /* MPMC_TEST & MPMC_TEST_3 */

	Offset   = MemOffset[Mpmc->ConfigPtr.DeviceId];
	StartAdr = Mpmc->ConfigPtr.MemBaseAddress + Offset;
	MemSize  = Mpmc->ConfigPtr.MemHighAddress -
		   Mpmc->ConfigPtr.MemBaseAddress + 1;
	Range    = MemSize - Offset;


	/*
	 * MPMC_TEST_0 - Write all zeros and verify.
	 */
#if (MPMC_TEST & MPMC_TEST_0)
	if (Offset > MemSize) {
		xil_printf("\n\r\tMemory Offset can't be greater than the size of" \
			"the memory");
		return ++Error;
	}

	xil_printf("\r\n\tTEST0: " \
		"Write all memory to 0x00000000 and check" \
		"\r\n\t\tWriting...");

	/*
	 * Clear the range of memory.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		WR_WORD(MyAdr, 0x00000000);
	}

	xil_printf("\r\n\t\tReading...");
	/*
	 * Read it all back.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		RD_WORD(MyAdr, DataRead);
		if (DataRead != 0x00000000) {
			Error++;
			xil_printf("\r\n\t\tTEST0 - ERROR #%d: Address = 0x%08x, " \
				"Data Expected was 0x00000000, Data Received " \
				"was 0x%08x", Error, MyAdr, DataRead);
		}
	}

	xil_printf("\r\n\tTest Complete Status = ");

	if (Error) {
		xil_printf("FAILURE");
	} else {
		xil_printf("SUCCESS");
	}

	TotalErrors += Error;
#endif /* MPMC_TEST & MPMC_TEST_0 */


	/*
	 * MPMC_TEST_1 - Write all ones and verify.
	 */
#if (MPMC_TEST & MPMC_TEST_1)
	if (Offset > MemSize) {
		xil_printf("\n\r\tMemory Offset can't be greater than the size of" \
			"the memory");
		return ++Error;
	}

	xil_printf("\r\n\tTEST1: " \
		"Write all memory to 0xFFFFFFFF and check" \
		"\r\n\t\tWriting...");

	/*
	 * Clear range of memory to 0xFFFFFFFF.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		WR_WORD(MyAdr, 0xFFFFFFFF);
	}

	xil_printf("\r\n\t\tReading...");

	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
	     RD_WORD(MyAdr, DataRead);

		if (DataRead != 0xFFFFFFFF) {
			Error++;
			xil_printf("\r\n\t\tTEST1 - ERROR #%d: Address = 0x%08x, " \
				"Data Expected was 0xFFFFFFFF, Data Received " \
				"was 0x%08x", Error, MyAdr, DataRead);
		}
	}
	xil_printf("\r\n\tTest Complete Status = ");
	if (Error) {
		xil_printf("FAILURE \r\n");
	} else {
		xil_printf("SUCCESS \r\n");
	}

	TotalErrors += Error;
#endif /* MPMC_TEST & MPMC_TEST_1 */

	/*
	 * MPMC_TEST_2 - Test for stuck together row/col bits.
	 */
#if (MPMC_TEST & MPMC_TEST_2)
	if (Offset > MemSize) {
		xil_printf("\n\r\tMemory Offset can't be greater than the size of" \
			"the memory");
		return ++Error;
	}

	xil_printf("\r\n\tTEST2: " \
		"Testing for stuck together bank/row/col bits" \
		"\r\n\t\tClearing memory to zeros...");

	/*
	 * Clear all of memory.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		WR_WORD(MyAdr, 0x00000000);
	}

	xil_printf("\r\n\t\tWriting and Reading...");

	for (i = 1, WriteAdr = StartAdr; WriteAdr < StartAdr + Range;
	     ++i, WriteAdr = StartAdr | (u32) (1 << i)) {

		/*
		 * Write out the value we'll be checking for later.
		 */
		WR_WORD(WriteAdr, 0xFFFFFFFF);

		for (j = 1, MyAdr = StartAdr; MyAdr < StartAdr + Range;
		     ++j, MyAdr = StartAdr | (u32)(1 << j)) {

			/*
			 * Read back a word of data.
			 */
			RD_WORD(MyAdr, DataRead);

			/*
			 * Check to make sure it is zero.
			 */
			if ((DataRead != 0) && (WriteAdr != MyAdr)) {
				Error++;
				xil_printf("\r\n\t\tTEST2 - ERROR #%d:, " \
					"Address = 0x%08x, " \
					"Data Expected was 0x00000000, " \
					"Data Received was 0x%08x",
					Error, MyAdr, DataRead);
			}
		}

		/*
		 * Clean out the old value before pointer update.
		 */
		WR_WORD(WriteAdr, 0x00000000);
	}
	xil_printf("\r\n\tTest Complete Status = ");

	if (Error) {
		xil_printf("FAILURE \r\n");
	} else {
		xil_printf("SUCCESS \r\n");
	}

	TotalErrors += Error;
#endif /* MPMC_TEST & MPMC_TEST_2 */

	/*
	 * MPMC_TEST_3 - MAX BA/ROW/COL NOISE.
	 */
#if (MPMC_TEST & MPMC_TEST_3)
	if (Offset > (MemSize / 2)) {
		xil_printf("\n\r\tMemory Offset can't be greater than half the" \
			"size of the memory");
		return ++Error;
	}

	xil_printf("\r\n\tTEST3: " \
		"Testing for maximum ba/row/col noise");
	xil_printf("\r\n\t\tThis test performs 16 word writes followed by 16 word" \
		" reads");
	xil_printf("\r\n\t\tEach 64 bytes inverts the ba/row/col address" \
		"\r\n\t\tInitializing Memory to 0xA5A5A5A5...");

	/*
	 * Calculate our bit masks for Column, Row and Bank addresses,
	 * we are assuming the memory is a power of 2 and even alignment.
	 */
	MemWidth = Mpmc->ConfigPtr.MemDataWidth;
	StartBit = (MemWidth == 8 ? 0 : MemWidth == 16 ? 1 :
		    MemWidth == 32 ? 2 : MemWidth == 64 ? 3 : 4);
	ColAddrMask = (((1 << Mpmc->ConfigPtr.MemColBits) - 1) << (StartBit)) &
		      (MemSize - 1);
	RowAddrMask = (((1 << Mpmc->ConfigPtr.MemRowBits) - 1) << (StartBit +
		       Mpmc->ConfigPtr.MemColBits)) & (MemSize - 1);
	BankAddrMask = (((1 << Mpmc->ConfigPtr.MemBankBits) - 1) <<
			 (StartBit + Mpmc->ConfigPtr.MemColBits +
			 Mpmc->ConfigPtr.MemRowBits)) & (MemSize - 1);

	/*
	 * Set the range of memory to 0xA5A5A5A5.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		WR_WORD(MyAdr, 0xA5A5A5A5);
	}

	/*
	 * Setup initial values.
	 */
	WriteAdr = StartAdr;
	MyAdr = WriteAdr;

	xil_printf("\r\n\t\tWriting and Reading...");

	/*
	 * Going to ping pong and meet in middle.
	 */
	for (i = 0; i < ((Range - Offset) / 128); i++) {
		BankAddr  = WriteAdr & BankAddrMask;
		Row = WriteAdr & RowAddrMask;
		Col = WriteAdr & ColAddrMask & 0xFFFFFFFC;

		/*
		 * Write out 64 bytes.
		 */
		for (j = 0; j < 8; j++) {
			WR_WORD(WriteAdr, 0xFFFFFFFF);
			WR_WORD(WriteAdr + 4, 0x00000000);
			WriteAdr += 8;
		}

		WriteAdr = MyAdr;

		/*
		 * Read back the 64 bytes.
		 */
		for (j = 0; j < 16; j++) {
			RD_WORD(WriteAdr, DataBuffer[j]);
			WriteAdr += 4;
		}

		WriteAdr = MyAdr;

		/*
		 * Lets compare the results to the expected values now....
		 */
		for (j = 0; j < 16; j += 2) {
			if (DataBuffer[j] != 0xFFFFFFFF) {
				Error++;
				xil_printf("\r\n\t\tTEST3.1 - ERROR #%d: " \
					"Address = 0x%08x, " \
					"Data Expected was 0xFFFFFFFF, " \
					"Data Received was 0x%08x", Error,
					(MyAdr + 4 * j), DataBuffer[j]);
			}

			if (DataBuffer[j + 1] != 0x00000000) {
				Error++;
				xil_printf("\r\n\t\tTEST3.2 - ERROR #%d: " \
					"Address = 0x%08x, " \
					"Data Expected was 0x00000000, " \
					"Data Received was 0x%08x", Error,
					MyAdr + (4 * j) + 4, DataBuffer[j + 1]);
			}
		}

		/*
		 * Calculate the inverse address for DDR SDRAM.
		 */
		BankAddr  = ~BankAddr  & BankAddrMask;
		Row = ~Row & RowAddrMask;
		Col = ~Col & ColAddrMask & 0xFFFFFFFC;
		WriteAdr = (BankAddr | Row | Col) - 60 +
			   Mpmc->ConfigPtr.MemBaseAddress;
		MyAdr = WriteAdr;

		/*
		 * Write out 64 bytes.
		 */
		for (j = 0; j < 8; j++) {
			WR_WORD(WriteAdr, 0xFFFFFFFF);
			WR_WORD(WriteAdr + 4, 0x00000000);
			WriteAdr += 8;
		}

		WriteAdr = MyAdr;

		/*
		 * Read back the 64 bytes.
		 */
		for (j = 16; j < 32; j++) {
			RD_WORD(WriteAdr, DataBuffer[j]);
			WriteAdr += 4;
		}

		/*
		 * Lets compare the results to the expected values now....
		 */
		for (j = 16; j < 32; j += 2) {
			if (DataBuffer[j] != 0xFFFFFFFF) {
				Error++;
				xil_printf("\r\n\t\tTEST3.3 - ERROR #%d: " \
					"Address = 0x%08x, " \
					"Data Expected was 0xFFFFFFFF, " \
					"Data Received was 0x%08x", Error,
					(MyAdr + 4 * (j - 16)), DataBuffer[j]);
			}
			if (DataBuffer[j + 1] != 0x00000000) {
				Error++;
				xil_printf("\r\n\t\tTEST3.4 - ERROR #%d: " \
					"Address = 0x%08x, " \
					"Data Expected was 0xFFFFFFFF, " \
					"Data Received was 0x%08x", Error,
					(MyAdr + (4 * (j - 16)) + 4),
					DataBuffer[j + 1]);
			}
		}

		/*
		 * Calculate the "normal" address for the next cycles.
		 */
		BankAddr  = ~BankAddr  & BankAddrMask;
		Row = ~Row & RowAddrMask;
		Col = ~Col & ColAddrMask & 0xFFFFFFFC;

		/*
		 * Increase by one 64 byte quantum.
		 */
		WriteAdr = (BankAddr | Row | Col) + 64 +
			   Mpmc->ConfigPtr.MemBaseAddress;
	}

	xil_printf("\r\n\tTest Complete Status = ");
	if (Error) {
		xil_printf("FAILURE");
	} else {
		xil_printf("SUCCESS");
	}

	TotalErrors += Error;
#endif /* MPMC_TEST & MPMC_TEST_3 */

	/*
	 * MPMC_TEST_4 - DATA = !ADDR test
	 */
#if (MPMC_TEST & MPMC_TEST_4)
	if (Offset > MemSize) {
		xil_printf("\n\r\tMemory Offset can't be greater than the size of" \
			"the memory");
		return ++Error;
	}

	xil_printf("\r\n\tTEST4: " \
		"Testing for Inverse Data at Address" \
		"\r\n\t\tWriting...");

	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		WR_WORD(MyAdr, ~MyAdr);
	}

	xil_printf("\r\n\t\tReading...");

	/*
	 * Read it all back.
	 */
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		RD_WORD(MyAdr, DataRead);

		if (DataRead != ~MyAdr) {
		Error++;
		xil_printf("\n\t\t\tTEST4 - ERROR #%d: Address = 0x%08x, " \
			"Data Expected was 0x%08x, Data Received was 0x%08x",
			Error, MyAdr, ~MyAdr, DataRead);
		}
	}

	xil_printf("\r\n\tTest Complete Status = ");
	if (Error) {
		xil_printf("FAILURE \r\n");
	} else {
		xil_printf("SUCCESS \r\n");
	}

	TotalErrors += Error;
#endif /* MPMC_TEST & MPMC_TEST_4 */

	return TotalErrors;
}

