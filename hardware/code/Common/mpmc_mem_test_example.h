/******************************************************************************
*
*	XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*	AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*	SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*	OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*	APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*	THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*	AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*	FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*	WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*	IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*	REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*	INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*	FOR A PARTICULAR PURPOSE.
*
*	(c) Copyright 2008 Xilinx Inc.
*	All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
* @file mpmc_mem_test_example.c
*
* This file contains a memory test example using the MPMC hardware device.
* This example tests the memory by writing and reading test patterns to the
* specified region of the memory. This example tests all the instances of
* MPMC present in the system. The constant, MAX_INSTANCES, defines maximum
* number of instances this example can test. This constant is set to 6, by
* default. The user can modify this constant, to test as many instances as
* desired.
*
* The various memory tests that are done as a part of this example are :
*  - Write all zeroes, read and verify.
*  - Write all ones, read and verify.
*  - Test for stuck together Row/Column.
*  - Maximum Band Address/Row/Column noise.
*  - Data NOT EQUAL TO Address Test.
*
* @note
*
* The following definitions have to be changed by the user depending on
* his requirements :
*
* MPMC_TEST - This definition provides an option to disable or enable each
* individual memory test. All the memory tests are enabled by default.
*
* MPMC_CACHE_TEST - This definition specifies the combination of
* Instruction Cache and Data Cache that are tested a part of the memory
* tests. The different combinations that can be tested are
* - ICache is enabled and DCache is enabled
* - ICache is disabled and DCache is disabled
* - ICache is enabled and DCache is disabled
* - ICache is disabled and DCache is enabled
* MPMC_CACHE_TEST_0 and MPMC_CACHE_TEST_1 are enabled by default.
*
* MAX_INSTANCES - This definition specifies the maximum number of MPMC instances
* this example can test. The default value of this constant is set to 6.
*
* NUM_ITERATIONS - This definition specifies the number of times the test
* has to be run. If it is set to -1 the tests will run infinitely. The default
* value of this constant is set to 1.
*
* PPC_CACHABLE_MEM_REGIONS - This definition specifies the memory regions to
* be marked as cacheable for a PPC processor. The user needs to modify its value
* based on the address and size of the MPMC memory in the system.
* Refer to xcache_l.c for more information.
*
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- --------------------------------------------------------
* 1.00a cc   09/10/08 First release.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xmpmc.h"
#include <stdio.h>

#ifdef __PPC__
#include "xcache_l.h"
#endif /* __PPC__ */

#ifdef __MICROBLAZE__
#include "mb_interface.h"
#endif /* __MICROBLAZE__ */


/************************** Constant Definitions ******************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define MPMC_NUM_INSTANCES	XPAR_XMPMC_NUM_INSTANCES

/*
 * Bit masks for the tests that are to be done on the memory.
 */
#define MPMC_TEST_0	0x0001 /* MPMC_TEST_0 - Write all zeros and verify */
#define MPMC_TEST_1	0x0002 /* MPMC_TEST_1 - Write all ones and verify */
#define MPMC_TEST_2	0x0004 /* MPMC_TEST_2 - Test for stuck together row/col
						bits*/
#define MPMC_TEST_3	0x0008 /* MPMC_TEST_3 - MAX BA/ROW/COL NOISE */
#define MPMC_TEST_4	0x0010 /* MPMC_TEST_4 - DATA = !ADDR test */

/*
 * Tests to be done. All the tests are enabled by default.
 */
#define MPMC_TEST	(MPMC_TEST_0 | MPMC_TEST_1 | MPMC_TEST_2 | \
			 MPMC_TEST_3 | MPMC_TEST_4)

/*
 * Cache options for the tests.
 */
#define MPMC_CACHE_TEST_0	0x0001 /* ICache: ON , DCache: ON */
#define MPMC_CACHE_TEST_1	0x0002 /* ICache: OFF, DCache: OFF */
#define MPMC_CACHE_TEST_2	0x0004 /* ICache: ON, DCache: OFF */
#define MPMC_CACHE_TEST_3	0x0008 /* ICache: OFF, DCache: ON */

/*
 * Cache options for the tests to be done.
 * MPMC_CACHE_TEST_0 and MPMC_CACHE_TEST_1 are enabled by default.
 */
#define MPMC_CACHE_TEST		(MPMC_CACHE_TEST_0 | MPMC_CACHE_TEST_1)

#define MAX_INSTANCES		6
/*
 * Specifies how many passes of the test to run, -1 == infinite.
 */
#define NUM_ITERATIONS		1

/*
 * This constant defines the memory regions to be marked as cacheable for a PPC
 * processor. Refer to xcache_l.c for more information.
 */
#define PPC_CACHABLE_MEM_REGIONS	0x80000000

/**************************** Type Definitions ********************************/

/*
 * This type is a word address ptr.
 */
typedef volatile u32 *U32Ptr;

/***************** Macros (Inline Functions) Definitions **********************/

/*
 * Macros to read and write words directly to memory.
 */
#define WR_WORD(ADDR, DATA) (*(U32Ptr)(ADDR) = (DATA))
#define RD_WORD(ADDR, DATA) ((DATA) = *(U32Ptr)(ADDR))

/*
 * Macros to enable/disable caches on different processors.
 */
#ifdef __PPC__

#define ENABLE_ICACHE()  XCache_EnableICache(PPC_CACHABLE_MEM_REGIONS)
#define ENABLE_DCACHE()  XCache_EnableDCache(PPC_CACHABLE_MEM_REGIONS)
#define DISABLE_ICACHE() XCache_DisableICache()
#define DISABLE_DCACHE() XCache_DisableDCache()

#endif /* __PPC__ */

#ifdef __MICROBLAZE__

#define ENABLE_ICACHE()  microblaze_enable_icache()
#define ENABLE_DCACHE()  microblaze_enable_dcache()
#define DISABLE_ICACHE() microblaze_disable_icache()
#define DISABLE_DCACHE() microblaze_disable_dcache()

#endif /* __MICROBLAZE__ */

#define printf xil_printf /* Small foot print printf */

/************************** Function Prototypes *******************************/

static u32 MpmcMemTest(XMpmc *Mpmc);
int MpmcMemTestExample(u16 DeviceId);

