#ifndef SKARO_PIT
#define SKARO_PIT

#define MAX_PIT_COUNTERS 3

typedef struct{
	int count;
	int max;
	int * event;
} Counter;

typedef struct{
	Counter counters[MAX_PIT_COUNTERS];
	int active_counters;
} PIT;

extern PIT pit;

void PIT_Init(PIT * pit);

void PIT_IncrementCounters(PIT * pit);

void PIT_AddCounter(PIT * pit, Counter counter);

void Counter_Increment(Counter * counter);

#endif
