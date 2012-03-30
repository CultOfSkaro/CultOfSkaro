#ifndef SKARO_AI
#define SKARO_AI
#include "Navigation.h"
#include "CrossCore.h"

#define DRIVE_TO_TOWER 0
#define ENTER_LOOP 1
#define EXIT_LOOP 2
#define DRIVE_AROUND_TOWER 3
#define DRIVE_AWAY_FROM_TOWER 4
#define IDLE 5
#define START 6
#define SEARCHING 7
#define CENTERING 8

typedef struct {
	int state;
	int destination;
	Navigation * navigation;
	Vision * vision;
	int search_direction;
} AI;

extern AI ai;

void AI_TwoTowers(AI * ai);

void AI_LostTower(AI * ai);
#endif