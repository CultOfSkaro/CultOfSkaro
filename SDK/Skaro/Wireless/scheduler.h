/*
 * Event_bits is a struct of bit fields.  Each bit field represents an event.
 * Add new events by adding a new bit field.
 */

typedef struct {
  int timer:1;
  int hello:1;
  int vision:1;
} Event_bits;

/*
 * Event_bits are wrapped up in a union so the entire thing can be set at once or
 * the individual flags can be set.
 */
typedef union {
  Event_bits flags;
  int reg;
} Events;

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
} Scheduler;


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
 * TaskList_Append is used internally to add a task to the linked list for the given event.
 */
Task * TaskList_Append(Task * current, Task * newTask);
