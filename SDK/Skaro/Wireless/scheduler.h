#ifndef SKARO_SCHEDULER
#define SKARO_SCHEDULER
/*
 * Event_bits is a struct of bit fields.  Each bit field represents an event.
 * Add new events by adding a new bit field.
 */

#define MAX_EVENTS 8

typedef struct {
  int velocity_loop;
  int hello;
  int steering_loop;
  int vision;
  int process_wireless_commands;
  int timer1;
  int timer2;
  int timer3;
} Event_flags;

/*
 * Event_bits are wrapped up in a union so the entire thing can be set at once or
 * the individual flags can be set.
 */
typedef union {
  Event_flags flags;
  int all[MAX_EVENTS];
} Events;

void Events_Init(Events * e);

/*
 * Task_struct is used internally in the scheduler to manage the linked list.
 * You should never have to deal with this directly.
 */
struct Task_struct {
  void (* func)();
  int priority;
  struct Task_struct * next; 
};

typedef struct Task_struct Task;


/*
 * The Scheduler struct is used in the scheduler.  One instance should be declared globally,
 * and initialized during setup.
 */
typedef struct {
  Events events;  
  Task * tasks[32];
  void (* beforeHook)();
} Scheduler;

extern Scheduler scheduler;
/*
 * Call Scheduler_Init to initialize the scheduler.  It should be called during setup.
 */

void Scheduler_Init(Scheduler * scheduler);

/*
 * The main loop of the application should only call Scheduler_Run.  Everything
 * else should be done in separate tasks.
 */

void Scheduler_Run(Scheduler * scheduler);

/*
 * Scheduler_RegisterTask should be called at the beginning of the application.  The arguments
 * are a function pointer to the function to be called and an Events struct with the triggering
 * events set.
 */

void Scheduler_RegisterTask(Scheduler * scheduler, void (* func)(), Events events);

/*
 * Scheduler_SetBeforeHook sets a function to be called every time Scheduler_Run is called.  It runs
 * before the scheduling loop, and can be used to set events based on specific conditions.
 */

void Scheduler_SetBeforeHook(Scheduler * scheduler, void (* func)());
/*
 * TaskList_Append is used internally to add a task to the linked list for the given event.
 */
Task * TaskList_Append(Task * current, Task * newTask);

#endif
