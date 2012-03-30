#include "AI.h"

AI ai;


void AI_Init(AI * ai){
	ai->state = IDLE;
	ai->navigation = &navigation;
	ai->vision = &vision;
}

void AI_LostTower(AI * ai){
	Blob * biggest = ai->vision->target;
	switch(ai->state){
	case START:
		ai->search_direction = RIGHT;
		PID_SetVelocity(&ai->navigation->pid, 3500);
		PID_SetRadius(&ai->navigation->pid, ai->search_direction, 1000);
		Navigation_SetSteeringMode(ai->navigation,RADIUS_MODE);
		Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
	case SEARCHING:
		if(biggest){
			ai->state = CENTERING;
			PID_SetVelocity(&ai->navigation->pid, 3500);
			Navigation_SetSteeringMode(ai->navigation,CENTROID_MODE);
			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
			Wireless_Debug("FOUND IT!!!!!");
		}
		break;
	case CENTERING:
		if(biggest == 0){
			ai->search_direction *= -1;
			PID_SetVelocity(&ai->navigation->pid, 3500);
			PID_SetRadius(&ai->navigation->pid, ai->search_direction, 1000);
			Navigation_SetSteeringMode(ai->navigation,RADIUS_MODE);
			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
			ai->state = SEARCHING;
			Wireless_Debug("LOST IT!!!!!");
			break;
		}
		if(biggest->distance*2000 <= 4000){
			ai->state = IDLE;
			Wireless_Debug("ARRIVED IT!!!!!");
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
		if(!ai->vision->target)
			break;
		toGo = ai->vision->target->distance*2000 - 4000;
		if(toGo <= 0){
			ai->state = ENTER_LOOP;
			PID_SetRadius(&ai->navigation->pid, RIGHT, 1000);
			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
			Navigation_SetSteeringMode(ai->navigation, RADIUS_MODE);
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
