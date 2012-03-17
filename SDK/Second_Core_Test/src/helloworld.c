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

typedef struct {
	int a;
	int b;
} VisionData;

volatile VisionData * vision_data = (VisionData *) 0x200000;
volatile VisionData ** live_vision_data = (volatile VisionData **) 0x100000;
volatile VisionData ** snap_vision_data = (volatile VisionData **) (0x100000 + sizeof(volatile VisionData **));


volatile VisionData * getVisionBuffer(){
	int i;
	for(i = 0; i < 3; i++){
		if((vision_data + i) == *live_vision_data){
			continue;
		} else if ((vision_data + i) == *snap_vision_data){
			continue;
		} else {
			return vision_data + i;
		}
	}
	return 0;
}

void updateVisionData(VisionData * tmp){
	*live_vision_data = (volatile VisionData *)tmp;
}

int main()
{
	usleep(100000);
	*snap_vision_data = 0;
	*live_vision_data = 0;
	volatile VisionData * tmp;
	while(1){
		tmp = getVisionBuffer();
		tmp->a = 20;
		tmp->b = 5;
		updateVisionData(tmp);
		tmp = getVisionBuffer();
		tmp->a = 10;
		tmp->b = 6;
		updateVisionData(tmp);
	}
}
