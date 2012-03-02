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


struct packed_struct {
		 unsigned test:4;
		 unsigned test2:10;
		 unsigned test3:9;
		 unsigned test4:9;
};

void print(char *str);

int main()
{

	struct packed_struct pack;

	print("\n\rBit field test\n\r");
	pack.test = 0xFF;
	xil_printf("0xFF stored in 4 bits: %x\n\r", pack.test);
	pack.test = 0xF;
	xil_printf("0xF store in 4 bits: %x\n\r", pack.test);
	pack.test2 = 0xFFF;
	xil_printf("0xFFF store in 10 bits: %x\n\r", pack.test2);
	pack.test2 = 0x3FF;
	xil_printf("0x3FF store in 10 bits: %x\n\r", pack.test2);
	pack.test3 = 0xFFF;
	xil_printf("0xFFF store in 9 bits: %x\n\r", pack.test3);
	pack.test3 = 0x1FF;
	xil_printf("0x1FF store in 9 bits: %x\n\r", pack.test3);
	pack.test4 = 0xFFF;
	xil_printf("0xFFF store in 9 bits: %x\n\r", pack.test4);
	pack.test4 = 0x1FF;
	xil_printf("0x1FF store in 9 bits: %x\n\r", pack.test4);
    return 0;
}
