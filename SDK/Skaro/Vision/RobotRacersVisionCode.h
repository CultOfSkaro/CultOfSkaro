#ifndef _ROBOTRACERSVISIONCODE_H_
#define _ROBOTRACERSVISIONCODE_H_

#include "image.h"
#include "vision.h"
#include "xparameters.h"

#define MAX_HUE 255
#define MAX_SAT 255

#define RED_SAT_T  30*MAX_SAT/100           // Red's Sat threshold
#define RED_CENTER  5*MAX_HUE/100          // Where I believe the red pylon is centered
#define RED_DELTA  7*MAX_HUE/100    // Red passband width
  
#define YELLOW_SAT_T  35*MAX_SAT/100        // Yellow's Sat threshold
#define YELLOW_CENTER  17*MAX_HUE/100      // Where I believe the Yellow pylon is centered
#define YELLOW_DELTA  4*MAX_HUE/100 // Yellow passband width
 
#define GREEN_SAT_T  30*MAX_SAT/100         // Green's Sat threshold
#define GREEN_CENTER  27*MAX_HUE/100       // Where I believe the green pylon is centered
#define GREEN_DELTA  9*MAX_HUE/100  // Green passband width
  
#define CYAN_SAT_T  11*MAX_SAT/100          // Cyan's Sat threshold
#define CYAN_CENTER  47*MAX_HUE/100        // Where I believe the Cyan pylon is centered
#define CYAN_DELTA  13*MAX_HUE/100  // Cyan passband width
  
#define BLUE_SAT_T  15*MAX_SAT/100          // Blue's Sat threshold
#define BLUE_CENTER  64*MAX_HUE/100        // Where I believe the blue pylon is centered
#define BLUE_DELTA  7*MAX_HUE/100   // Blue passband width
  
#define PURPLE_SAT_T  10*MAX_SAT/100        // Purple's Sat threshold
#define PURPLE_CENTER  84*MAX_HUE/100      // Where I believe the Purple pylon is centered
#define PURPLE_DELTA  19*MAX_HUE/100// Purple passband width


/**
 * @brief Truck
 *
 * This struct holds the basic information possessed by a truck such as its x-y
 * coordinates and its bearing angle.
 */
typedef struct {
  int32 angle;
  int32 x;
  int32 y;
} Truck;


/**
 * @brief Truck and tower information
 *
 * This struct is used to facilitate returning image processing results.
 */
typedef struct {
  TowerSearchResults towerResults;
  Truck truck;
  uint8 isValidPosition;
} TruckTowerInfo;

/**
 * @brief Process image
 *
 * This function takes image hue and saturation information and attempts to find
 * the tower and truck information.
 *
 * @param hue An array representing the hue of each pixel
 * @param sat An array representing the saturation of each pixel
 * @param findPosition A boolean representing whether we wish to find the
 *   the position of the current truck calling this function.
 * @return Information regarding the truck's position (if findPosition is set)
 *   and any towers found while processing the image.
 */
TruckTowerInfo processImage(Image hue, Image sat, uint8 findPosition);

#endif
