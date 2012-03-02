#include "identifier.h"

/** Find Potential Shapes
 *  The shape this function finds depends on the ratio and error inputs. 
 *
 * I is the image - 2d array
 * name is the intuitive name of the current image - string
 * name is only used for matlab plotting.
 * ID has the same info as name, and it is what is used later. - integer
 * ratio is the ratio of height/length of rectangle - number
 * error is the error in ratio that we can be off and still pass - number
 *   ie: error = .1 means if ratio is .4 we will pass .3 to .5
 * NumFound is an integer of the number of possibles found
 * Details is a (NumFound x 6) array of the details for each possible
*/
LightId LightIdentifier(Image im, uint32 idNum) {
    Image analyzed;
    LightId ret_val;
    ret_val.NumFound = 0;
	
    uint32 i, j;
    uint32 x, y;
    uint32 xCount, yCount;
    uint32 R;
     
    // j is x; i is y   
    for (j = 0; j < IMAGE_WIDTH; j++) {
        for (i = 0; i < IMAGE_HEIGHT; i++) {
            if (1 == Image_getAt(im,i,j)) {
               if (0 == Image_getAt(analyzed,i,j)) {
                  x = j;
                  xCount = 1;
                  x = x + 1;
                  
                  // go along x
                  while (x < IMAGE_WIDTH && (1 == Image_getAt(im,i,x)) && (0 == Image_getAt(analyzed,i,x))) {
                        xCount = xCount + 1;
                        x = x + 1;
                  }
                  
                  y = i;
                  yCount = 1;
                  y = y + 1;
                  
                  // go along y
                  while (y < IMAGE_HEIGHT && (1 == Image_getAt(im,y,j))) {
                        yCount = yCount + 1;
                        y = y + 1;
                  }
                  
                  R = yCount*1024/xCount;
                  if (R >= (TOWER_RATIO - TOWER_RATIO_ERROR) && (R <= TOWER_RATIO + TOWER_RATIO_ERROR)) {
                     if (xCount >= 15) {
                        ret_val.Details[ret_val.NumFound].ID = idNum;
                        ret_val.Details[ret_val.NumFound].y = i;
                        ret_val.Details[ret_val.NumFound].x = j;
                        ret_val.Details[ret_val.NumFound].height = yCount; 
                        ret_val.Details[ret_val.NumFound].width = xCount;
						
						ret_val.NumFound = ret_val.NumFound + 1;
						if(ret_val.NumFound >= LIGHT_ID_MAX_COUNT){
							return ret_val;
						}
						for (x = j; x < j + xCount; x++){
							for (y = i; y < i + yCount; y++){
								Image_setAt(analyzed, y, x, 1);
							}
						}
                     }   
                  }
               }   
            }             
        }
    }
    
    return ret_val;
}
