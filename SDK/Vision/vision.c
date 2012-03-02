#include "vision.h"
#include "identifier.h"

//static const fixed pi = fixed(3.141592653589793);
//static const fixed twoPi = fixed(6.283185307179586);
//static const double pi = 3217;
//static const double twoPi = 6434;



/**
 * @brief Absolute Value
 *
 * This function returns the absolute value of the given number
 *
 * @param x The number from which to take the absolute value
 * @return The result of taking the absolute value
 */
/*double mathAbs(double x) {
  return (x >= 0.0) ? x : -x;
}*/

/**
 * @brief Trig Cosine
 *
 * This function computes the cosine of the angle, using Taylor series expansion.
 *
 * @param angle The angle (given in radians)
 * @return The cosine of the angle
 */
//double trigCos(double angle) {
//  double normAngle;
//  double result;
//  double fact;
//  double x;
//  double angleSquared;
//  int i;

  /* Reduce the angle to be within [-pi, pi] to allow fast series convergence */
//  normAngle = angle - twoPi*((int)(angle/twoPi));
//  if (normAngle < -pi)
//    normAngle += twoPi;
//  else if (normAngle > pi)
//    normAngle -= twoPi;
//  result = 1.0;
//  fact = 1.0;
//  x = 1.0;
//  angleSquared = normAngle*normAngle;

  /* 15 iterations gives us about 16 decimal digits of precision */
//  for (i = 1; i < 15; ++i) {
//    fact *= 2*i*(2*i-1);
//    x *= angleSquared;
//    result += (i&1) ? -x/fact : x/fact;
//  }
//  return result;
//}

/**
 * @brief Trig Sine
 *
 * This function computes the sine of the angle, using Taylor series expansion.
 *
 * @param angle The angle (given in radians)
 * @return The sine of the angle
 */
/*double trigSin(double angle) {
  double normAngle;
  double result;
  double fact;
  double x;
  double angleSquared;
  int i;*/

  /* Reduce the angle to be within [-pi, pi] to allow fast series convergence */
 /* normAngle = angle - twoPi*((int)(angle/twoPi));
  if (normAngle < -pi)
    normAngle += twoPi;
  else if (normAngle > pi)
    normAngle -= twoPi;
  result = normAngle;
  fact = 1.0;
  x = normAngle;
  angleSquared = normAngle*normAngle;
*/
  /* 15 iterations gives us about 16 decimal digits of precision */
  /*for (i = 1; i < 15; ++i) {
    fact *= 2*i*(2*i+1);
    x *= angleSquared;
    result += (i&1) ? -x/fact : x/fact;
  }
  return result;
}*/

/**
 * @brief Compute Truck X-Y-Angle
 *
 * This computes the X and Y coordinates of the truck, given it's distance and
 * angle from a corner. This function also computes the absolute heading angle
 * of the truck (indicating its direction).
 *
 * @param corner The ID number of the corner of interest
 * @param angle The angle (in degrees) that you are to the right of the left
 *   sideline. 0 means you are on the sideline to your left. 90 means you are
 *   on the sideline to your right. 45 means you are in the middle of the
 *   field.
 * @param theta The angle (in degrees) that the pylon is at relative to our
 *   camera. 0 means we are locking right at it. Negative is to the left, and
 *   positive is to the right.
 * @return A vector containing the x,y coordinates and the direction angle of
 *   the truck.
 */
/*XYAngle Truck(int corner, fixed distance, fixed angle, fixed theta) {
  XYAngle result;
  fixed w = fixed(20.0); // Width of the playing field 
  fixed l = fixed(30.0); // Length of the playing field 
  fixed rsl = fixed(distance * trigSin(angle/180.0*pi));
  fixed lsl = fixed(distance * trigCos(angle/180.0*pi));
  fixed r;

  if (corner == 1) {
    r = fixed(270.0);
    result.x = rsl;
    result.y = lsl;
  }
  else if (corner == 2) {
    r = fixed(180.0);
    result.x = w - lsl;
    result.y = rsl;
  }
  else if (corner == 3) {
    r = fixed(0.0);
    result.x = lsl;
    result.y = l - rsl;
  }
  else {
    r = fixed(90.0);
    result.x = w - rsl;
    result.y = l - lsl;
  }
  
  result.a = r - fixed(90.0) + angle + theta;
  if (result.a < fixed(0.0))
    result.a += fixed(360.0);
  return result;
}*/


void crossClean(Image im) {
  Image tempIm;
  uint32 score;
  uint32 i, j, tempy, tempx;


  /* Initialize resultIm to im */
  Image_copy(im, tempIm);

  for (i = 5; i < IMAGE_HEIGHT-5; ++i) {
    for (j = 5; i < IMAGE_WIDTH-5; ++j) {
      if (Image_getAt(tempIm, i, j) == 1) {
        for (tempy = i-1; tempy <= i+1; ++tempy) {
          for (tempx = j-1; tempx <= j+1; ++tempx) {
            if (Image_getAt(tempIm, tempy, tempx) == 0) {
              score = Image_getAt(tempIm, tempy, tempx-1) +
                Image_getAt(tempIm, tempy, tempx-2) +
                Image_getAt(tempIm, tempy, tempx-3) +
                Image_getAt(tempIm, tempy, tempx-4) +
                Image_getAt(tempIm, tempy, tempx+1) +
                Image_getAt(tempIm, tempy, tempx+2) +
                Image_getAt(tempIm, tempy, tempx+3) +
                Image_getAt(tempIm, tempy, tempx+4) +
                Image_getAt(tempIm, tempy-1, tempx) +
                Image_getAt(tempIm, tempy-2, tempx) +
                Image_getAt(tempIm, tempy-3, tempx) +
                Image_getAt(tempIm, tempy-4, tempx) +
                Image_getAt(tempIm, tempy+1, tempx) +
                Image_getAt(tempIm, tempy+2, tempx) +
                Image_getAt(tempIm, tempy+3, tempx) +
                Image_getAt(tempIm, tempy+4, tempx);
              if (score >= 6)
                Image_setAt(im, tempy, tempx, 1);
            }
          }
        }
      }
    }
  }
}

TowerSearchResults towers(Image Red, Image Yellow, Image Green, Image Cyan, Image Blue, Image Purple) {

	// local variables
	LightId redId, yellowId, greenId, cyanId, blueId, purpleId;
	LightDetail PossibleStatusLights[LIGHT_ID_MAX_COUNT*4];
	LightDetail PossibleTeamLights[LIGHT_ID_MAX_COUNT*2];
	TowerSearchResults result;
	int32 WinnerError, a;
	int32 offCenter, maxAngle;
	uint32 i, t, s, n;
	uint32 numStatusLights, numTeamLights;
	uint32 lowerBound, upperBound, Xerror, Werror;

	/* Initialize the search results to indicate none found */
	result.numTowersFound = 0;

	// look for possible lights of each color
	redId = LightIdentifier(Red, 1);
	yellowId = LightIdentifier(Yellow, 2);
	greenId = LightIdentifier(Green, 3);
	cyanId = LightIdentifier(Cyan, 4);
	blueId = LightIdentifier(Blue, 5);
	purpleId = LightIdentifier(Purple, 6);

	/* Combine status lights */
	for (i = 0; i < yellowId.NumFound; ++i)
	PossibleStatusLights[i] = yellowId.Details[i];
	n = yellowId.NumFound;
	for (i = 0; i < greenId.NumFound; ++i)
		PossibleStatusLights[n+i] = greenId.Details[i];
	n += greenId.NumFound;
	for (i = 0; i < cyanId.NumFound; ++i)
		PossibleStatusLights[n+i] = cyanId.Details[i];
	n += cyanId.NumFound;
	for (i = 0; i < purpleId.NumFound; ++i)
		PossibleStatusLights[n+i] = purpleId.Details[i];
	numStatusLights = n + purpleId.NumFound;
	
	/* Combine team lights */
	for (i = 0; i < redId.NumFound; ++i)
		PossibleTeamLights[i] = redId.Details[i];
	n = redId.NumFound;
	for (i = 0; i < blueId.NumFound; ++i)
		PossibleTeamLights[n+i] = blueId.Details[i];
	numTeamLights = n + blueId.NumFound;

  /*
   * Make sense of it
   *
   * First, we know that the only valid pairs are a team light underneath and
   * a status light on top.  Red and Blue (yes, blue, not cyan) are our team
   * colors.  So lets go through and try every possible red and blue and see
   * if there's a matching yellow, green, cyan, or purple on top of it.  And
   * yes, they have to be matching, meaning within some error of the same
   * width.
   */
	for (t = 0; t < numTeamLights; ++t) {
		for (s = 0; s < numStatusLights; ++s) {
			/* StatusLight above TeamLight, within 2 lightbar heights */
			lowerBound = PossibleStatusLights[s].y;
			upperBound = lowerBound + PossibleStatusLights[s].height * 2;
			if ((PossibleTeamLights[t].y > lowerBound) && (PossibleTeamLights[t].y < upperBound)){
				/* StatusLight width is very close to TeamLight X width */
				Werror = 3; //tenths
				lowerBound = PossibleTeamLights[t].width*(10-Werror)/10;
				upperBound = PossibleTeamLights[t].width*(10+Werror)/10;
				if ((PossibleStatusLights[s].width < upperBound) &&	(PossibleStatusLights[s].width > lowerBound)) {
					/* StatusLight X is very close to TeamLight X */
					Xerror = PossibleTeamLights[t].width/5; /* Xerror is a percentage of teamlight width */
					lowerBound = PossibleTeamLights[t].x - Xerror;
					upperBound = PossibleTeamLights[t].x + Xerror;
					if ((PossibleStatusLights[s].x < upperBound) &&	(PossibleStatusLights[s].x > lowerBound)) {
						/* We found one */
						++result.numTowersFound;

						/* Team decision */
						if (PossibleTeamLights[t].ID == 5)
							result.towers[result.numTowersFound-1].team = 1; /* team 1 is blue */
						else if (PossibleTeamLights[t].ID == 1)
							result.towers[result.numTowersFound-1].team = 2; /* team 2 is red */

						/* 
						* State decision
						* Enabled = 1 = green
						* Disabled = 2 = cyan
						* HaveFlag = 3 = purple
						*/
						if (PossibleStatusLights[s].ID == 3)
							result.towers[result.numTowersFound-1].state = 1; /* green, enabled */
						else if (PossibleStatusLights[s].ID == 4)
							result.towers[result.numTowersFound-1].state = 2; /* cyan, disabled */
						else if (PossibleStatusLights[s].ID == 6)
							result.towers[result.numTowersFound-1].state = 3; /* purple, have flag */

						/* Angle Decision */
						maxAngle = 150; /* Our camera sees 15 degrees to left and 15 degrees to right */
						offCenter = PossibleTeamLights[t].x + PossibleTeamLights[t].width/2 - IMAGE_WIDTH; /* this is the center of the light */
						result.towers[result.numTowersFound-1].angle = (offCenter*maxAngle)/(IMAGE_WIDTH/2);

						/* Distance Decision */

						/* figure out distance by going through LUT */
						/*WinnerError = 1000;
						result.towers[result.TotalTowersFound-1].distance = 240;
						for (n = 0; n < TOWER_DIST_LUT_SIZE; ++n) {
							a = LUT[n][0]-PossibleTeamLights[t].width;
							if (a < 0) a = -a;
							if (a < WinnerError) {
								WinnerError = a;
								result.towers[result.TotalTowersFound-1].distance = LUT[n][1];
							}
						}*/
						// figure out distance by using two points and assuming inverse proportional relationship
						result.towers[result.numTowersFound-1].distance = 4240/PossibleTeamLights[t].width;
					}
				}
			}
		}
	}
	return result;
}
