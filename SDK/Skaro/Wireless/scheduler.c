#include "scheduler.h"
#include <stdlib.h>
#ifdef SCHEDULER_DEBUG
#define DEBUG_PRINT(...) Wireless_Debug(__VA_ARGS__);
#else
#define DEBUG_PRINT(...) ;
#endif

Scheduler scheduler;

void Events_Init(Events * e){
	int i;
	for(i=0; i < MAX_EVENTS; i++){
		e->all[i] = 0;
	}
}

void Scheduler_Init(Scheduler * scheduler){
  int i;
  for(i = 0; i < 32; i++){
    scheduler->tasks[i] = 0;
  }

  Events_Init(&scheduler->events);
}

void Scheduler_Destroy(Scheduler * scheduler){
  int i;
  for(i = 0; i < 32; i++){
    Task * current_task = scheduler->tasks[i];
    while(current_task){
      Task * next = current_task->next;
      free(current_task);
      current_task = next;
    }
  }
}

void Scheduler_Run(Scheduler * scheduler){
  int i;
  scheduler->beforeHook();
  for(i=0; i < MAX_EVENTS; i++){
    if(scheduler->events.all[i]){
      Task * current_task = scheduler->tasks[i];
      for(;current_task;current_task = current_task->next){
        current_task->func();
      }
      scheduler->events.all[i]=0;
      //return;
    }
  }
}

void Scheduler_RegisterTask(Scheduler * scheduler, void (* func)(), Events events){
  int i;
  int index;
  for(i=0; i < MAX_EVENTS; i++){
    index = 1 << i; 
    if(events.all[i]){
      Task * t = (Task *)malloc(sizeof(Task));
      t->func = func;
      t->next = 0;
      t->priority = 0;
      scheduler->tasks[i] = TaskList_Append(scheduler->tasks[i], t);
    }
  }
}


void inline Scheduler_SetBeforeHook(Scheduler * scheduler, void (* func)()){
	scheduler->beforeHook = func;
}

Task * TaskList_Append(Task * current, Task * newTask){
  if(current){
    current->next = TaskList_Append(current->next,newTask);
    return current;
  } else {
    return newTask;
  } 
}

