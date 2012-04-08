#include "AI.h"
#include "plb_quad_encoder.h"
#include "skaro_wireless.h"
#include "GameBoard.h"
#include "Timer.h"
#include "Serial.h"

AI ai;


void AI_Init(AI * ai){
	ai->state = IDLE;
	ai->mode = NOOP_AI;
	ai->navigation = &navigation;
	ai->vision = &vision;
	ai->gb = &gameBoard;
}

void AI_WhatToDo(AI * ai){
	if (ai->mode == NOOP_AI) {
		return;
	}

	int next_mode = ai->mode;
	/// we just hit something.
	if(ai->gb->hit){
		Wireless_Debug("We hit something!\n\r");
		switch(ai->mode){
		case GET_THE_FLAG:
			next_mode = RETURN_THE_FLAG;
			break;
		case SHOOT_OPPONENT:
			if (ai->gb->hasFlag) {
				next_mode = RETURN_THE_FLAG;
			} else {
				next_mode = GET_THE_FLAG;
			}
			break;
		case RETURN_THE_FLAG:
			next_mode = GET_THE_FLAG;
			break;
		case REVIVE_SELF:
			next_mode = GET_THE_FLAG;
			break;
		default:
			break;
		}
	}
	ai->gb->hit = 0;

	//check for anomalies in current state.
	switch(ai->mode){
		case GET_THE_FLAG:
			if (ai->gb->hasFlag) {
				next_mode = RETURN_THE_FLAG;
			}
			if(ai->vision->them.truck.active && (!ai->vision->them.tower.active) &&
					ai->vision->them.truck.status != DISABLED_OBJECT_STATUS){
				next_mode = SHOOT_OPPONENT;
			}
			break;
		case RETURN_THE_FLAG:
			if(ai->vision->us.tower.active){
				if(!ai->gb->hasFlag){
					next_mode = GET_THE_FLAG;
				}
				if(ai->vision->us.tower.status == ENABLED_OBJECT_STATUS){
					next_mode = SHOOT_OPPONENT;
				}
			}
			break;
		case SHOOT_OPPONENT:
			if(!ai->gb->hasFlag){
				if(ai->vision->them.truck.active){
					if(ai->vision->them.truck.status == FLAG_OBJECT_STATUS){
						// STAY ON COURSE
					} else {
						// KEEP SHOOTING THEM UNLESS WE CAN SEE THEIR TOWER
						if(ai->vision->them.tower.active){
							next_mode = GET_THE_FLAG;
						}
					}
				} else {
					next_mode = GET_THE_FLAG;
				}
			} else {
				/// If we have the flag and are chasing our opponent, that means that
				/// they have our flag and we must duel until the death.  There is no
				/// reason to every change modes this point.  See you in hell.
			}
			break;
		case REVIVE_SELF:
			if (ai->gb->alive) {
				next_mode = GET_THE_FLAG;
			}
			break;
		default:
			break;
	}
	// NO EXCEPTIONS.. if we are disabled we have to revive ourselves.
	if(!ai->gb->alive && !ai->gb->gameNotInPlay){
		next_mode = REVIVE_SELF;
	}
	if(next_mode != ai->mode)
		AI_ChangeMode(ai, next_mode);
}

void AI_RunAI(AI * ai){
	AI_WhatToDo(ai);
	switch(ai->mode){
	case GET_THE_FLAG:
		AI_ShootTarget(ai,&ai->vision->them.tower, GAME_KILL_SHOT);
		break;
	case RETURN_THE_FLAG:
		AI_ShootTarget(ai,&ai->vision->us.tower, GAME_PASS_SHOT);
		break;
	case SHOOT_OPPONENT:
		AI_ShootTarget(ai,&ai->vision->them.truck, GAME_KILL_SHOT);
		break;
	case REVIVE_SELF:
		AI_ShootTarget(ai,&ai->vision->us.tower, GAME_REVIVE_SHOT);
		break;
	case NOOP_AI:
		break;
	default:
		break;
	}
}

void AI_ChangeMode(AI * ai, int new_mode){
	ai->state = START;
	ai->mode = new_mode;
	switch(ai->mode){
	case GET_THE_FLAG:
		Wireless_Debug("GOING FOR THE FLAG\n\r;");
		break;
	case RETURN_THE_FLAG:
		Wireless_Debug("RETURNING THE FLAG\n\r");
		break;
	case SHOOT_OPPONENT:
		Wireless_Debug("SHOOTING OPPONENT\n\r");
		break;
	case REVIVE_SELF:
		Wireless_Debug("REVIVING SELF\n\r");
		break;
	default:
		break;
	}
}

void AI_Search(AI * ai, int direction_LEFT_RIGHT, int radius){
	ai->search_direction = direction_LEFT_RIGHT;
	PID_SetVelocity(&ai->navigation->pid, ai->max_velocity);
	PID_SetRadius(&ai->navigation->pid, ai->search_direction, radius);
	Navigation_SetSteeringMode(ai->navigation,RADIUS_MODE);
	Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
	ai->state = SEARCHING;
}

void inline AI_SetTarget(AI * ai,Object * target){
	ai->vision->current_target = target;
}

void AI_CenterTarget(AI * ai, Object * target, int velocity){
	navigation.pid.currentCentroid = target->blob.center;
	PID_SetVelocity(&ai->navigation->pid, velocity);
	Navigation_SetSteeringMode(ai->navigation,CENTROID_MODE);
	Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
}

void AI_Shooting(AI * ai, Object * target, int shot_type){
	if(!target->active) {
		ai->lost_tower++;
	} else {
		ai->lost_tower = 0;
	}

	if(ai->lost_tower > 5){
		PID_SetVelocity(&ai->navigation->pid, ai->max_velocity);
		AI_Search(ai, ai->search_direction*-1, 0.75 * ai->max_velocity);
		Wireless_Debug("Lost The Tower!\r\n");
	} else {
		GB_Shoot(shot_type);
	}
}

#define STOPPED_THRESHOLD 250
#define BACKUP_RADIUS 1000
#define RUNNING_VELOCITY (ai->max_velocity)
#define PI 	3.14159265

int stoppedTime = 0;
int savedAIState = 0;
int backupTarget = 0;
int doneBackingUp = 0;

void AI_ShootTarget(AI * ai, Object * target, int shot_type){
	static uint32 lastClockTicks = 0;

	//calculate the time delta in milliseconds
	uint32 nowClocks = ClockTime();
	int timeDelta = (refresh_rate(nowClocks, lastClockTicks) * 1000);
	lastClockTicks = nowClocks;

	//check to see if we hit something
	if (ai->navigation->pid.desiredVelocityPID != 0 &&
		ai->navigation->pid.currentVelocity < 50 &&
		ai->navigation->pid.currentVelocity > -50) {
		stoppedTime += timeDelta;
		if (stoppedTime >= STOPPED_THRESHOLD) {
			if (ai->state == BACKUP) {
				doneBackingUp = 1;
			} else {
				stoppedTime = 0;
				Wireless_Debug("Backing up!!\r\n");
				ai->state = BACKUP;
				PID_SetVelocity(&ai->navigation->pid, -2000);
				PID_SetRadius(&ai->navigation->pid, -(ai->search_direction), BACKUP_RADIUS);
				Navigation_SetSteeringMode(ai->navigation,RADIUS_MODE);
				Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);

				backupTarget = getTicks() - PI*2*BACKUP_RADIUS/4;
				Wireless_Debug("Backup target: ");
				PrintInt(backupTarget);
				Wireless_Debug("\r\n");
			}
		}
	} else {
		stoppedTime = 0;
	}

	vision.current_target = target;
	int ticks;
	switch(ai->state) {
	case BACKUP:
		ticks = getTicks();
		if(ticks <= backupTarget || doneBackingUp){
			doneBackingUp = 0;
			stoppedTime = 0;
			Wireless_Debug("Finished Backing up.\r\n");
			Wireless_Debug("Ticks: ");
			PrintInt(ticks);
			Wireless_Debug("\r\n");

			ai->state = SEARCHING;
			PID_SetVelocity(&ai->navigation->pid, ai->max_velocity);
			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
			ai->lost_tower = 0;
			AI_Search(ai, ai->search_direction, 1000);
		}
		break;

	case START:
		ai->lost_tower = 0;
		PID_SetVelocity(&ai->navigation->pid, ai->max_velocity);
		AI_Search(ai, RIGHT, 1000);
		/// SUPPOSED TO FALL THROUGH
	case SEARCHING:
		if(target->active) {
			ai->state = CENTERING_FULL_SPEED;
			AI_CenterTarget(ai, target, ai->max_velocity * 1.5);
			Wireless_Debug("Found Tower!\r\n");
			Wireless_Debug("Full Speed Ahead!\r\n");
		}
		break;
	case CENTERING_FULL_SPEED:
		if(target->active && target->blob.distance < 6000) {
			ai->state = CENTERING;
			AI_CenterTarget(ai, target, ai->max_velocity);
			Wireless_Debug("Slowing Down!\r\n");
		}
		AI_Shooting(ai, target, shot_type);
		break;
	case CENTERING:
		if(target->active && target->blob.distance < 5000){
			ai->state = CENTERING_HALF_SPEED;
			AI_CenterTarget(ai, target, ai->max_velocity / 2);
			Wireless_Debug("Slowing Down Even More!\r\n");
		}
		AI_Shooting(ai, target, shot_type);
		break;
	case CENTERING_HALF_SPEED:
		if(target->active && (target->blob.distance < 4000) && ai->gb->gameNotInPlay) {
			ai->state = WAITING_DISABLED;
			AI_CenterTarget(ai, target, 0);
			Wireless_Debug("Stopping because disabled!\r\n");
		}
		AI_Shooting(ai, target, shot_type);
		break;

	case WAITING_DISABLED:
		if(!ai->gb->gameNotInPlay) {
			ai->state = CENTERING_HALF_SPEED;
			AI_CenterTarget(ai, target, ai->max_velocity / 2);
			Wireless_Debug("Resuming.\r\n");
		}
		break;

	case IDLE:
		PID_SetVelocity(&ai->navigation->pid, 0);
		Navigation_SetSteeringMode(ai->navigation,CENTROID_MODE);
		Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
		break;
	default:
		break;
	}
}

//void AI_LostTower(AI * ai){
//	//check to see if we hit something
//	if (ai->navigation->pid.desiredVelocityPID != 0 &&
//			ai->navigation->pid.currentVelocity < 50 &&
//			ai->navigation->pid.currentVelocity > -50) {
//		stoppedTime += AI_INTERVAL;
//
//		if (stoppedTime >= STOPPED_THRESHOLD && ai->state != BACKUP) {
//			Wireless_Debug("Backing up!!\r\n");
//			savedAIState = ai->state;
//			ai->state = BACKUP;
//			PID_SetVelocity(&ai->navigation->pid, -1500);
//			PID_SetRadius(&ai->navigation->pid, -(ai->search_direction), BACKUP_RADIUS);
//			Navigation_SetSteeringMode(ai->navigation,RADIUS_MODE);
//			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
//
//			backupTarget = getTicks() - PI*2*BACKUP_RADIUS/4;
//		}
//	} else {
//		stoppedTime = 0;
//	}
//
//	Object * target = ai->vision->current_target;
//	switch(ai->state) {
//	case BACKUP:
//		if(getTicks() <= backupTarget){
//			ai->state = SEARCHING;
//			PID_SetRadius(&ai->navigation->pid, ai->search_direction, 1000);
//			Navigation_SetSteeringMode(ai->navigation,RADIUS_MODE);
//			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
//			PID_SetVelocity(&ai->navigation->pid, RUNNING_VELOCITY);
//		}
//		break;
//
//	case START:
//		AI_Search(ai, RIGHT, 1000);
//		AI_SetTarget(ai,&ai->vision->blue.tower);
//		Wireless_Debug("SEARCHING!!!!!\r\n");
//		//SUPPOSED TO FALL THROUGH TO SEARCHING!!!!
//	case SEARCHING:
//		target = ai->vision->current_target;
//		if(target) {
//			AI_CenterTarget(ai);
//			Wireless_Debug("FOUND IT!!!!!\r\n");
//		}
//		break;
//	case CENTERING:
//		target = ai->vision->current_target;
//		if(!target) {
//			AI_Search(ai, ai->search_direction*-1, 2000);
//			Wireless_Debug("LOST IT!!!!!\r\n");
//			break;
//		}
//		if(target->blob.distance <= 3500) {
//			if(ai->vision->current_target == &ai->vision->red.tower){
//				Wireless_Debug("ARRIVED PINK!!!!! LOOKING FOR BLUE!!\r\n");
//				GB_Shoot(GAME_PASS_SHOT);
//				AI_Search(ai, RIGHT, 1500);
//				AI_SetTarget(ai,&ai->vision->blue.tower);
//				break;
//			} else {
//				Wireless_Debug("ARRIVED BLUE!!!!! LOOKING FOR PINK\r\n");
//				GB_Shoot(GAME_KILL_SHOT);
//				GB_Shoot(GAME_PASS_SHOT);
//				AI_Search(ai, RIGHT, 1500);
//				AI_SetTarget(ai,&ai->vision->red.tower);
//			}
//		}
//		break;
//	case IDLE:
//		PID_SetVelocity(&ai->navigation->pid, 0);
//		Navigation_SetSteeringMode(ai->navigation,CENTROID_MODE);
//		Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
//		break;
//	}
//}

//void AI_TwoTowers(AI * ai){
//	int toGo;
//	switch(ai->state){
//	case START:
//		ai->destination = getTicks() + 2000;
//		PID_SetVelocity(&ai->navigation->pid, 2200);
//		Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
//		Navigation_SetSteeringMode(ai->navigation, CENTROID_MODE);
//		ai->state = DRIVE_TO_TOWER;
//		Wireless_Debug("In Start");
//		Wireless_Debug("In DRIVE_TO_TOWER");
//	case DRIVE_TO_TOWER:
//		if(!ai->vision->current_target)
//			break;
//		toGo = (*ai->vision->current_target)->distance - 4000;
//		if(toGo <= 0){
//			ai->state = ENTER_LOOP;
//			PID_SetRadius(&ai->navigation->pid, RIGHT, 1000);
//			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
//			ai->destination = getTicks() + PI*2*1000/4;
//			Wireless_Debug("In ENTER_LOOP");
//		}
//		break;
//	case ENTER_LOOP:
//		if(getTicks() >= ai->destination){
//			ai->state = DRIVE_AROUND_TOWER;
//			PID_SetRadius(&ai->navigation->pid, LEFT, 1500);
//			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
//			Navigation_SetSteeringMode(ai->navigation, RADIUS_MODE);
//			ai->destination = getTicks() + PI*2*1500;
//			Wireless_Debug("In DRIVE_AROUND_TOWER");
//		}
//		break;
//	case DRIVE_AROUND_TOWER:
//		if(getTicks() >= ai->destination){
//			ai->state = EXIT_LOOP;
//			PID_SetRadius(&ai->navigation->pid, RIGHT, 1000);
//			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
//			Navigation_SetSteeringMode(ai->navigation, RADIUS_MODE);
//			ai->destination = getTicks() + PI*2*1000/4;
//			Wireless_Debug("In EXIT_LOOP");
//		}
//		break;
//	case EXIT_LOOP:
//		if(getTicks() >= ai->destination){
//			ai->destination = getTicks() + 2000;
//			PID_SetVelocity(&ai->navigation->pid, 1500);
//			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
//			Navigation_HoldServo(ai->navigation,0);
//			ai->state = DRIVE_AWAY_FROM_TOWER;
//			Wireless_Debug("In DRIVE_AWAY_FROM_TOWER");
//		}
//		break;
//	case DRIVE_AWAY_FROM_TOWER:
//		if(getTicks() >= ai->destination){
//			PID_SetVelocity(&ai->navigation->pid, 0);
//			Navigation_SetVelocityMode(ai->navigation,VELOCITY_MODE);
//			ai->state = IDLE;
//		}
//		break;
//	case IDLE:
//		break;
//	}
//}
