#include "scheduler.h"
#include <stdlib.h>
#ifdef SCHEDULER_DEBUG
#define DEBUG_PRINT(...) Wireless_Debug(__VA_ARGS__);
#else
#define DEBUG_PRINT(...) ;
#endif

void Scheduler_Init(Scheduler * scheduler){
  int i;
  for(i = 0; i < 32; i++){
    scheduler->tasks[i] = 0;
  }
  scheduler->events.reg = 0;
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
  DEBUG_PRINT("Running Scheduler\n");
  int index;
  for(i=0; i < 32; i++){
    index = 1 << i;
    if((scheduler->events.reg >> i) & 1){
      Task * current_task = scheduler->tasks[i];
      for(;current_task;current_task = current_task->next){
        current_task->func();
      }
      scheduler->events.reg &= ~(1 << i);
    }
  }
}

void Scheduler_RegisterTask(Scheduler * scheduler, void (* func)(), Events events){
  int i;
  int index;
  for(i=0; i < 32; i++){
    index = 1 << i; 
    if((events.reg >> i) & 1){
      Task * t = (Task *)malloc(sizeof(Task));
      t->func = func;
      t->next = 0;
      t->priority = 0;
      scheduler->tasks[i] = TaskList_Append(scheduler->tasks[i], t);
    }
  }
}

Task * TaskList_Append(Task * current, Task * newTask){
  if(current){
    current->next = newTask;
    return current;
  } else {
    return newTask;
  } 
}
