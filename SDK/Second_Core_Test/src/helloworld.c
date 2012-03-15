/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

#include <stdio.h>
#include "platform.h"
#include "sleep.h"
#include "string.h"

void print(char *str);

int main()
{
	int i = 0;
	volatile char * test_string = (char *)0x100000;
	while(1){
		usleep(1000000);
		strcpy(test_string, "Hello Peter!");
		/*
		XIo_Out8(test_string,'P');
		XIo_Out8(test_string+1,'e');
		XIo_Out8(test_string+2,'t');
		XIo_Out8(test_string+3,'e');
		XIo_Out8(test_string+4,'r');
		XIo_Out8(test_string+5,'i');
		XIo_Out8(test_string+6, 0);
		*/

		usleep(1000000);
		strcpy(test_string, "Yay, strcpy!!!!!");
		/*
		test_string[0] = 'Y';
		test_string[1] = 'o';
		test_string[2] = 'g';
		test_string[3] = 'i';
		test_string[4] = 'a';
		test_string[5] = 0;
		*/
	}
}
