#ifndef _VISION_H_
#define _VISION_H_

#include "image.h"
#include "Header.h"

#define TOWER_SEARCH_MAX_COUNT 10

  /* Look-up table for status light distances
   * Note, I obviously need to add more values by doing more tests... */
  // perhaps distance in inches = 4240/pixels
/*#define TOWER_DIST_LUT_SIZE 2
const fixed TOWER_DIST_LUT[TOWER_DIST_LUT_SIZE][2] = // pixels, inches
			{{54,	79},
			{68,	62}}; 
*/
// Return values for TruckXYAngle() function
/*typedef struct {
  fixed x,y,a;
} XYAngle;*/


/**
 * @brief Tower
 *
 * This struct holds the information common to all towers. Each tower has a
 * team, state, and a distance and angle from the camera.
 */
typedef struct {
  int32 distance; // in inches
  int32 angle; // in tenths of degrees
  uint8 team;
  uint8 state;
} Tower;

/**
 * @brief Tower search results
 *
 * This struct is used to facilitate returning tower search results
 */
typedef struct {
  Tower towers[TOWER_SEARCH_MAX_COUNT];
  uint8 numTowersFound;
} TowerSearchResults;
/*
XYAngle TruckXYAngle(int corner, fixed distance, fixed angle, fixed theta); 
*/
/**
 * @brief Cross Clean
 *
 * This function facilitates the detection algorithm by "filling in" or
 * "de-speckling" the image in a crosshair fashion.
 *
 * @param im The original image
 */
void crossClean(Image im);

/**
 * @brief Find towers
 *
 * This function searches through a cleaned image and returns an integer
 * indicating the total number of towers found. It also returns the team,
 * state, distance, and angle of each tower. The maximum number of towers found
 * is limited to 5.
 *
 * @return The tower search results
 */
TowerSearchResults towers(Image Red, Image Yellow, Image Green, Image Cyan, Image Blue, Image Purple);


#endif
