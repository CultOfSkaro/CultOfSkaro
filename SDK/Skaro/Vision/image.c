#include "image.h"
#include "Header.h"

void Image_setAt(Image img, uint16 y, uint16 x, uint8 value) {
  img[y][x] = value;
}

uint8 Image_getAt(Image img, uint16 y, uint16 x) {
  return img[y][x];
}

void Image_init(Image img){
	uint16 i;
	uint16 j;
  for (i = 0; i < IMAGE_HEIGHT; ++i)
    for (j = 0; j < IMAGE_WIDTH; ++j)
      img[i][j] = 0;
}

void Image_copy(Image src, Image dest){
	uint16 i;
	uint16 j;
  for (i = 0; i < IMAGE_HEIGHT; ++i)
    for (j = 0; j < IMAGE_WIDTH; ++j)
      dest[i][j] = src[i][j];
}
