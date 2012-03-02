#include "RobotRacersVisionCode.h"
#include "vision.h"



TruckTowerInfo processImage(Image Hue, Image Sat, uint8 findPosition) {
  Image ImRed, ImYellow, ImGreen, ImCyan, ImBlue, ImPurple;
  TruckTowerInfo ret_val;
  TowerSearchResults results;
  uint32 i, j;
  ret_val.isValidPosition = 0;

  // Red segmentation
  for (i = 0; i < IMAGE_HEIGHT; i++) {
    for (j = 0; j < IMAGE_WIDTH; j++) {
      //wrap around red
      if (((Hue[i][j] >= (256 + RED_CENTER - RED_DELTA)) || (Hue[i][j] <= (RED_CENTER + RED_DELTA))) && (Sat[i][j] > RED_SAT_T)) {
        ImRed[i][j] = 1;
      } else { 
        ImRed[i][j] = 0;
      }
    }
  }
  
  // Yellow segmentation
  for (i = 0; i < IMAGE_HEIGHT; i++) {
    for (j = 0; j < IMAGE_WIDTH; j++) {
      if ((Hue[i][j] >= (YELLOW_CENTER - YELLOW_DELTA)) && (Hue[i][j] <= (YELLOW_CENTER + YELLOW_DELTA)) && (Sat[i][j] > YELLOW_SAT_T)) {
        ImYellow[i][j] = 1;
      } else { 
        ImYellow[i][j] = 0;
      }
    }
  }
  
  // Green segmentation
  for (i = 0; i < IMAGE_HEIGHT; i++) {
    for (j = 0; j < IMAGE_WIDTH; j++) {
      if ((Hue[i][j] >= (GREEN_CENTER - GREEN_DELTA)) && (Hue[i][j] <= (GREEN_CENTER + GREEN_DELTA)) && (Sat[i][j] > GREEN_SAT_T)) {
        ImGreen[i][j] = 1;
      } else { 
        ImGreen[i][j] = 0;
      }
    }
  }
  
  // Cyan segmentation
  for (i = 0; i < IMAGE_HEIGHT; i++) {
    for (j = 0; j < IMAGE_WIDTH; j++) {
      if ((Hue[i][j] >= (CYAN_CENTER - CYAN_DELTA)) && (Hue[i][j] <= (CYAN_CENTER + CYAN_DELTA)) && (Sat[i][j] > CYAN_SAT_T)) {
        ImCyan[i][j] = 1;
      } else { 
        ImCyan[i][j] = 0;
      }
    }
  }

  // Blue segmentation
  for (i = 0; i < IMAGE_HEIGHT; i++) {
    for (j = 0; j < IMAGE_WIDTH; j++) {
      if ((Hue[i][j] >= (BLUE_CENTER - BLUE_DELTA)) && (Hue[i][j] <= (BLUE_CENTER + BLUE_DELTA)) && (Sat[i][j] > BLUE_SAT_T)) {
        ImBlue[i][j] = 1;
      } else { 
        ImBlue[i][j] = 0;
      }
    }
  }
  
  // Purple segmentation
  for (i = 0; i < IMAGE_HEIGHT; i++) {
    for (j = 0; j < IMAGE_WIDTH; j++) {
      //wrap around purple
      if (((Hue[i][j] >= (PURPLE_CENTER - PURPLE_DELTA)) || (Hue[i][j] <= (PURPLE_CENTER + PURPLE_DELTA - 256))) && (Sat[i][j] > PURPLE_SAT_T)) {
        ImPurple[i][j] = 1;
      } else { 
        ImPurple[i][j] = 0;
      }
    }
  }
  
  // Done color segmenting
  // Now clean up the pictures vertically (actually not vertically, but keep the name :)
  // TODO: figure out vertClean in C
  crossClean(ImRed);
  crossClean(ImYellow);
  crossClean(ImGreen);
  crossClean(ImCyan);
  crossClean(ImBlue);
  crossClean(ImPurple);
  
  /*if (1 == findPosition) {
    // Call the pylon truck xy code
    // note, the pylons are red, green and cyan
    // note, location is used to compare agains the wheel encoders, to help us
    // if we are lost.  I don't know if location needs to be called every time.
    ret_val.isValidPosition = location(Red, Green, Cyan, ret_val.TruckX, ret_val.TruckY, ret_val.TruckAngle); 
  } else {
    ret_val.isValidPosition = 0;

    ret_val.TruckX = 0;
    ret_val.TruckY = 0;
    ret_val.TruckAngle = 0;       
  }ret_val.towerResults.*/
  
  // Call the towers code
  // note, all colors are used here
  // note, towers returns the distance and angle to each tower, which is the
  // information that would be the most useful if our desire is to track, zero in,
  // and shoot that tower.
  results = towers(ImRed, ImYellow, ImGreen, ImCyan, ImBlue, ImPurple);
  ret_val.towerResults = results;
  return ret_val;  
}
