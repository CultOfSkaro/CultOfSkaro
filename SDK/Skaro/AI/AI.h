#ifndef SKARO_AI
#define SKARO_AI
#include "Navigation.h"
#include "CrossCore.h"
#include "GameBoard.h"

#define DRIVE_TO_TOWER 0
#define ENTER_LOOP 1
#define EXIT_LOOP 2
#define DRIVE_AROUND_TOWER 3
#define DRIVE_AWAY_FROM_TOWER 4
#define IDLE 5
#define START 6
#define SEARCHING 7
#define CENTERING 8
#define CENTERING_FULL_SPEED 9
#define CENTERING_HALF_SPEED 10
#define CROSS_FIELD 11

// MODES
#define GET_THE_FLAG		0
#define SHOOT_OPPONENT		1
#define RETURN_THE_FLAG		2
#define REVIVE_SELF			3
#define NOOP_AI				4

typedef struct {
	int state;
	int mode;
	int destination;
	Navigation * navigation;
	Vision * vision;
	GameBoard * gb;
	int search_direction;
	int max_velocity;
	int lost_tower;
} AI;

extern AI ai;


//FUNCTIONS
void AI_Init(AI * ai);
void AI_RunAI(AI * ai);
void AI_ShootTarget(AI * ai, Object * target, int shot_type);
void AI_ChangeMode(AI * ai, int new_mode);
void AI_Search(AI * ai, int direction_LEFT_RIGHT, int radius);
void inline AI_SetTarget(AI * ai, Object * target);
void AI_CenterTarget(AI * ai, Object * target);

//MODES
void AI_TwoTowers(AI * ai);
void AI_LostTower(AI * ai);

#endif
