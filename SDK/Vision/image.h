#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "xparameters.h"
#include "Header.h"

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
/**
 * @brief Image
 *
 * This struct is used for representing a statically-sized integer array that is
 * the same size as the camera resolution. This simplifies function arguments
 * and return values.
 */
typedef uint8 Image[IMAGE_HEIGHT][IMAGE_WIDTH];

/**
 * @brief Image set pixel value
 *
 * This function sets the pixel in the image at the given x and y coordinates
 * to a given value.
 *
 * @param img The image containing the pixel to be set
 * @param y The vertical coordinate of the pixel
 * @param x The horizontal coordinate of the pixel
 * @param value The new value for the pixel
 */
void Image_setAt(Image img, uint16 y, uint16 x, uint8 value);

/**
 * @brief Image get pixel value
 *
 * This function retrieves the value of the pixel in the image at the given x
 * and y coordinates.
 *
 * @param img The image from which we retrieve the pixel value
 * @param y The vertical coordinate of the pixel
 * @param x The horizontal coordinate of the pixel
 * @return The value of the pixel
 */
uint8 Image_getAt(Image img, uint16 y, uint16 x);
 
/**
 * @brief Initialize Image
 *
 * This is a helper function used for initializing an image's pixels to 0.
 *
 * @param img The image to initialize
 */
void Image_init(Image img);

/**
 * @brief Copy Image
 *
 * This is a helper function used for copying the pixels from one image to
 * another.
 *
 * @param srcImg The image to copy the pixels from
 * @param destImg The destination image for the pixels
 */
void Image_copy(Image src, Image dest);

#endif
