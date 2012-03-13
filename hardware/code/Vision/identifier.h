#ifndef _IDENTIFIER_H_
#define _IDENTIFIER_H_

#include "image.h"
#include "Header.h"

#define LIGHT_ID_MAX_COUNT 10
#define TOWER_RATIO 184 //.18 * 1024
#define TOWER_RATIO_ERROR 51 //.05 * 1024
/*
*	Details has 5 fields for each possible match
*		0: ID number, given when search is made, to help identify the color searched
*		1: verical coordinate of top left corner
*		2: horizontal coordinate of top left corner
*		3: height in pixels
*		4: width in pixels
*/
typedef struct {
	uint32 ID;
	uint32 y;
	uint32 x;
	uint32 height;
	uint32 width;
} LightDetail;

typedef struct {
  uint32 NumFound;
  LightDetail Details[LIGHT_ID_MAX_COUNT];
} LightId;

LightId LightIdentifier(Image im, uint32 idNum);

#endif
