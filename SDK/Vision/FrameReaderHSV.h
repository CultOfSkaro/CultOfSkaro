#ifndef FRAME_READER_HSV_H
#define FRAME_READER_HSV_H

#include "FrameTable.h"
#include "image.h"

void makeHueTable(FrameTableEntry* fte, Image HueTable);
void makeSatTable(FrameTableEntry* fte, Image SatTable);
void makeHueSatTables(FrameTableEntry* fte, Image HueTable, Image SatTable);


#endif
