/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * Xilinx EDK 11.1 EDK_L.29.1
 *
 * This file is a sample test application
 *
 * This application is intended to test and/or illustrate some 
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * XPS project when you run the "Generate Libraries" menu item
 * in XPS.
 *
 * Your XPS project directory is at:
 *    C:\EDK11\Rebuild2\
 */


// Located in: ppc405_0/include/xparameters.h
#include "xparameters.h"

#include "xcache_l.h"

#include <stdio.h>

#include "xutil.h"
/* Add your core's .h files below this line */
#include "plb_quad_encoder.h"
#include "Timer.h"
/* Add your core's .h files above this line */
//====================================================

int main (void) {


	XCache_EnableICache(0x00000001);
	XCache_EnableDCache(0x00000001);
	Xuint32 ticks;
	print("-- Entering main() --\r\n");
	/*
	 * MemoryTest routine will not be run for the memory at
	 * 0xffff0000 (xps_bram_if_cntlr_1)
	 * because it is being used to hold a part of this application program
	 */
	/* Add your core's test function calls below this line */
	/* Basic uart test: */
	print("Hello world!\r\n");
	/* Encoder Test */
	print("Testing encoder...\r\n");
	// EncoderTest();
	clearEncoder();
	while (1) {
		ticks = getTicks();
		xil_printf("Ticks %d\r", ticks);
		ClockDelay(100000000);
	}
	print("-- Exiting main() --\r\n");
	/* Add your core's test function calls above this line */
	print("Tests complete!  Exiting...\r\n");
	XCache_DisableDCache();
	XCache_DisableICache();
	return 0;
}

