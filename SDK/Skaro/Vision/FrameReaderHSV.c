#include "FrameReaderHSV.h"

void makeHueTable(FrameTableEntry* fte, Image HueTable){
	uint16* HSVTable;
	int i, j;
	HSVTable = fte->frame_address[VISION_FRAME_RGB565]->data.data16;
	for(i = 0; i < IMAGE_HEIGHT; i++){
		for(j = 0; j < IMAGE_WIDTH; j++){
			HueTable[i][j] = (uint8)((HSVTable[IMAGE_WIDTH*i+j] >> 8) & 0x00FF);
		}
	}
}

void makeSatTable(FrameTableEntry* fte, Image SatTable){
	uint16* HSVTable;
	int i, j;
	HSVTable = fte->frame_address[VISION_FRAME_RGB565]->data.data16;
	for(i = 0; i < IMAGE_HEIGHT; i++){
		for(j = 0; j < IMAGE_WIDTH; j++){
		SatTable[i][j] = (uint8)((HSVTable[IMAGE_WIDTH*i+j] >> 2) & 0x00FF);
		}
	}
}



void makeHueSatTables(FrameTableEntry* fte, Image HueTable, Image SatTable){
	uint16* HSVTable;
	int i, j;
	HSVTable = fte->frame_address[VISION_FRAME_RGB565]->data.data16;
	for(i = 0; i < IMAGE_HEIGHT; i++){
		for(j = 0; j < IMAGE_WIDTH; j++){
		SatTable[i][j] = (uint8)((HSVTable[IMAGE_WIDTH*i+j] >> 2) & 0x00FF);
		HueTable[i][j] = (uint8)((HSVTable[IMAGE_WIDTH*i+j] >> 8) & 0x00FF);
		}
	}
}
