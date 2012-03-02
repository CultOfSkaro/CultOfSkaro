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
#include <xparameters.h>

#include <xcache_l.h>

#include <stdio.h>

#include "xutil.h"
/* Add your core's .h files below this line */
#include "HeliosIO.h"
#include "Timer.h"

#include <xuartlite_l.h>
/* Add your core's .h files above this line */
//====================================================

int main (void) {
	XStatus status;
	u8 c;

   XCache_EnableICache(0x00000001);
   XCache_EnableDCache(0x00000001);
   print("-- Entering main() --\r\n");
   /* 
    * MemoryTest routine will not be run for the memory at 
    * 0xffff0000 (xps_bram_if_cntlr_1)
    * because it is being used to hold a part of this application program
    */
	/* Add your core's test function calls below this line */
	xil_printf("-- Starting GPIO Test Mode --\r\n");
	xil_printf("Initializing...\r\n");
		status = InitGameSystem();
	if(status != XST_SUCCESS){
		print("BAD GPIO!\r\n");
	}
	xil_printf("Use the keyboard to choose GPIO tests\r\n");
	xil_printf("1: read out channel 1 pins\r\n");
	xil_printf("2: read out channel 2 pins\r\n");
	xil_printf("3: toggle Helios user LED 1\r\n");
	xil_printf("4: toggle Helios user LED 2\r\n");
	xil_printf("5: fire a kill shot\r\n");
	xil_printf("t: read out tristate/direction registers\r\n");
	while(1)
	{
		/*while(XUartLite_IsReceiveEmpty(XPAR_WIRELESS_UART_BASEADDR))
		{
			xil_printf("Receive is empty\r\n");
			sleep(1);
		}
		c = XUartLite_RecvByte(XPAR_WIRELESS_UART_BASEADDR);*/
		read(0,&c,1);
		xil_printf("Got %s from you\r\n", &c);
		switch(c)
		{
			case '1':
				xil_printf("User button: %d\r\n", HeliosReadBTN());
				xil_printf("User switch 1: %d\r\n", HeliosReadSW1());
				xil_printf("User switch 2: %d\r\n", HeliosReadSW2());
				xil_printf("User LED 1: %d\r\n", HeliosReadLED1());
				xil_printf("User LED 2: %d\r\n", HeliosReadLED2());
				break;
			case '2':
				xil_printf("Truck Alive? : %d\r\n", Game_Enabled());
				xil_printf("Have Flag? : %d\r\n", Game_HaveFlag());
				//xil_printf("Waiting to shoot? : %d\r\n", Game_WaitingToShoot());
				xil_printf("Game state: %x\r\n", XGpio_ReadReg(HELIOS_IO_BASEADDR, XGPIO_DATA2_OFFSET));
				break;
			case '3':
				HeliosSetLED1(!HeliosReadLED1());
				break;
			case '4':
				HeliosSetLED2(!HeliosReadLED2());
				break;
			case '5':
				Game_Shoot(GAME_KILL_SHOT);
				break;
			case 't':
				Game_State();//
				break;
			default:
				xil_printf("Use the keyboard to choose GPIO tests\r\n");
				xil_printf("1: read out channel 1 pins\r\n");
				xil_printf("2: read out channel 2 pins\r\n");
				xil_printf("3: toggle Helios user LED 1\r\n");
				xil_printf("4: toggle Helios user LED 2\r\n");
				xil_printf("5: fire a kill shot\r\n");
				xil_printf("t: read out tristate/direction registers\r\n");
				break;
		}
	}
   print("-- Exiting main() --\r\n");
	/* Add your core's test function calls above this line */
	//print("Tests complete!  Exiting...\r\n");
   XCache_DisableDCache();
   XCache_DisableICache();
   return 0;
}

