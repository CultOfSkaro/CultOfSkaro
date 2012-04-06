#include "AI.h"
#include "plb_quad_encoder.h"
#include "skaro_wireless.h"
#include "GameBoard.h"

AI ai;


void AI_Init(AI * ai){
	ai->state = IDLE;
	ai->navigation = &navigation;
	ai->vision = &vision;
}

#define AI_INTERVAL 10
#define STOPPED_THRESHOLD 500
#define BACKUP_RADIUS 1000
#define RUNNING_VELOCITY (ai->max_velocity)

int stoppedTime = 0;
int savedAIState = 0;
int backupTarget = 0;

void AI_Search(AI * ai, int direction_LEFT_RIGHT, int radius){
	ai->search_direction = direction_LEFT_RIGHT;
	PID_SetVelocity(&ai->navigation->pid, ai->max_velocity);
	PID_SetRadius(&ai->navigation->pid, ai->search_direction, radius);
	Navigation_SetSteeringMode(ai->navigation,RADIUS_MODE);
	Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
	ai->state = SEARCHING;
}

void AI_SetTarget(AI * ai, Blob * target){
	ai->vision->current_target = &target;
}

void AI_CenterTarget(AI * ai){
	ai->state = CENTERING;
	Navigation_SetSteeringMode(ai->navigation,CENTROID_MODE);
	Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
}

void AI_LostTower(AI * ai){
	//check to see if we hit something
	if (ai->navigation->pid.desiredVelocityPID != 0 &&
		ai->navigation->pid.currentVelocity < 50 &&
		ai->navigation->pid.currentVelocity > -50) {
		stoppedTime += AI_INTERVAL;

		if (stoppedTime >= STOPPED_THRESHOLD && ai->state != BACKUP) {
			Wireless_Debug("Backing up!!\r\n");
			savedAIState = ai->state;
			ai->state = BACKUP;
			PID_SetVelocity(&ai->navigation->pid, -1500);
			PID_SetRadius(&ai->navigation->pid, -(ai->search_direction), BACKUP_RADIUS);
			Navigation_SetSteeringMode(ai->navigation,RADIUS_MODE);
			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);

			backupTarget = getTicks() - PI*2*BACKUP_RADIUS/4;
		}
	} else {
		stoppedTime = 0;
	}

	Blob * target = *ai->vision->current_target;
	switch(ai->state) {
	case BACKUP:
		if(getTicks() <= backupTarget){
			ai->state = SEARCHING;
			PID_SetRadius(&ai->navigation->pid, ai->search_direction, 1000);
			Navigation_SetSteeringMode(ai->navigation,RADIUS_MODE);
			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
			PID_SetVelocity(&ai->navigation->pid, RUNNING_VELOCITY);
		}
		break;

	case START:
		AI_Search(ai, RIGHT, 1000);
		AI_SetTarget(ai,ai->vision->blue_tower);
		Wireless_Debug("SEARCHING!!!!!\r\n");
		//SUPPOSED TO FALL THROUGH TO SEARCHING!!!!
	case SEARCHING:
		target = *ai->vision->current_target;
		if(target) {
			AI_CenterTarget(ai);
			Wireless_Debug("FOUND IT!!!!!\r\n");
		}
		break;
	case CENTERING:
		target = *ai->vision->current_target;
		if(!target) {
			AI_Search(ai, ai->search_direction*-1, 2000);
			Wireless_Debug("LOST IT!!!!!\r\n");
			break;
		}
		if(target->distance <= 3500) {
			if(ai->vision->current_target == &ai->vision->pink_tower){
				Wireless_Debug("ARRIVED PINK!!!!! LOOKING FOR BLUE!!\r\n");
				GB_Shoot(GAME_PASS_SHOT);
				AI_Search(ai, RIGHT, 1500);
				AI_SetTarget(ai,ai->vision->blue_tower);
				break;
			} else {
				Wireless_Debug("ARRIVED BLUE!!!!! LOOKING FOR PINK\r\n");
				GB_Shoot(GAME_KILL_SHOT);
				GB_Shoot(GAME_PASS_SHOT);
				AI_Search(ai, RIGHT, 1500);
				AI_SetTarget(ai,ai->vision->pink_tower);
			}
		}
		break;
	case IDLE:
		PID_SetVelocity(&ai->navigation->pid, 0);
		Navigation_SetSteeringMode(ai->navigation,CENTROID_MODE);
		Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
		break;
	}
}

void AI_TwoTowers(AI * ai){
	int toGo;
	switch(ai->state){
	case START:
		ai->destination = getTicks() + 2000;
		PID_SetVelocity(&ai->navigation->pid, 2200);
		Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
		Navigation_SetSteeringMode(ai->navigation, CENTROID_MODE);
		ai->state = DRIVE_TO_TOWER;
		Wireless_Debug("In Start");
		Wireless_Debug("In DRIVE_TO_TOWER");
	case DRIVE_TO_TOWER:
		if(!ai->vision->current_target)
			break;
		toGo = (*ai->vision->current_target)->distance - 4000;
		if(toGo <= 0){
			ai->state = ENTER_LOOP;
			PID_SetRadius(&ai->navigation->pid, RIGHT, 1000);
			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
			ai->destination = getTicks() + PI*2*1000/4;
			Wireless_Debug("In ENTER_LOOP");
		}
		break;
	case ENTER_LOOP:
		if(getTicks() >= ai->destination){
			ai->state = DRIVE_AROUND_TOWER;
			PID_SetRadius(&ai->navigation->pid, LEFT, 1500);
			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
			Navigation_SetSteeringMode(ai->navigation, RADIUS_MODE);
			ai->destination = getTicks() + PI*2*1500;
			Wireless_Debug("In DRIVE_AROUND_TOWER");
		}
		break;
	case DRIVE_AROUND_TOWER:
		if(getTicks() >= ai->destination){
			ai->state = EXIT_LOOP;
			PID_SetRadius(&ai->navigation->pid, RIGHT, 1000);
			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
			Navigation_SetSteeringMode(ai->navigation, RADIUS_MODE);
			ai->destination = getTicks() + PI*2*1000/4;
			Wireless_Debug("In EXIT_LOOP");
		}
		break;
	case EXIT_LOOP:
		if(getTicks() >= ai->destination){
			ai->destination = getTicks() + 2000;
			PID_SetVelocity(&ai->navigation->pid, 1500);
			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
			Navigation_HoldServo(ai->navigation,0);
			ai->state = DRIVE_AWAY_FROM_TOWER;
			Wireless_Debug("In DRIVE_AWAY_FROM_TOWER");
		}
		break;
	case DRIVE_AWAY_FROM_TOWER:
		if(getTicks() >= ai->destination){
			PID_SetVelocity(&ai->navigation->pid, 0);
			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
			ai->state = IDLE;
		}
		break;
	case IDLE:
		break;
	}
}
