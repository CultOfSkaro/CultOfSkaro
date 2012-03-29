#include "PIT.h"

PIT pit;

void PIT_Init(PIT * pit){
	// nothing to do here...
}

void PIT_IncrementCounters(PIT * pit){
	int i;
	for(i = 0; i < pit->active_counters; i++){
		Counter_Increment(&pit->counters[i]);
	}
}

void PIT_AddCounter(PIT * pit, Counter counter){
	counter.count = 0;
	pit->counters[pit->active_counters]=counter;
	pit->active_counters++;
}

void Counter_Increment(Counter * counter){
	counter->count++;
	if(counter->count > counter->max){
		*counter->event = 1;
		counter->count = 0;
	}
}

