/**
\unit
\author Barrett Edwards

FILE:				Queue.h
AUTHOR:				Barrett Edwards
CREATED:			1 June 2007
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	Queue.c
DESCRIPTION:

	This is a simple implementation of a first-in-first-out (FIFO) 
	data storage Queue. This implementation only allows the storage 
	of void pointers (void*). It is expected that these void pointers 
	will be used to point to application specific data.
   
	This implementation #does not allocate any memory# for the Queue 
	structure or it's data array where the void pointers are actually 
	stored. It is expected that the data for the Queue will be allocated
	elsewhere.

HOWTO:

	A simple code example of Queue usage:
	\pre
	Queue myqueue;
	int queue_size = 10;
	void* data_ptr = malloc( queue_size * sizeof(void*) );
	QueueInit( &myqueue, data_ptr, queue_size );
	...
	QueuePush( &myqueue, new_data );
	...
	void *old_data = QueuePop( &myqueue );
	\endpre

Related Functions & Groups:
	- Initialization:
		- void QueueInit(Queue* Q, void** data, int MaxElements);
		- void QueueClear(Queue* Q);
	- Data access:
		- int   QueuePush(Queue* Q, void* X);
		- void* QueueFront(Queue* Q);
		- void  QueuePop(Queue* Q);
	- Info:
		- int QueueIsEmpty(Queue* Q);
		- int QueueIsFull(Queue* Q);

CHANGE LOG

*/

/* Conditional Inclusion ----------------------------------------------------*/
#ifndef QUEUE_H
#define QUEUE_H


/* Includes -----------------------------------------------------------------*/
#include "Header.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/

/** The storage struct for a simple Queue
	This implementation only stores void pointers. Therefore, any usage of 
	this Queue implementation should only push a pointer to data onto the 
	Queue rather than the data itself (unless it is a primative data type
	like an int).


	A simple code example of Queue usage:
	\pre
	Queue myqueue;
	int queue_size = 10;
	void* data_ptr = malloc( queue_size * sizeof(void*) );
	QueueInit( &myqueue, data_ptr, queue_size );
	...
	QueuePush( &myqueue, new_data );
	...
	void *old_data = QueuePop( &myqueue );
	\endpre

	Related Functions & Groups:
	- Initialization:
		- void QueueInit(Queue* Q, void** data, int MaxElements);
		- void QueueClear(Queue* Q);
	- Data access:
		- int   QueuePush(Queue* Q, void* X);
		- void* QueueFront(Queue* Q);
		- void  QueuePop(Queue* Q);
	- Info:
		- int QueueIsEmpty(Queue* Q);
		- int QueueIsFull(Queue* Q);
*/
struct Queue 
{
  int	 capacity;	///< Maximum number of elements this Queue can store
  int	 front;		///< Index of the front element in the Queue
  int	 back;		///< Index of the back element in the Queue
  int	 size;		///< Number of elements currently in the Queue
  void** data;		///< Data array for element storage
};
typedef struct Queue Queue; ///< typedef struct Queue Queue;


/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/

void  QueueInit(Queue* Q, void** data, int MaxElements);
void  QueueClear(Queue* Q);

int   QueuePush(Queue* Q, void* X);
void* QueueFront(Queue* Q);
void* QueuePop(Queue* Q);

int   QueueIsEmpty(Queue* Q);
int   QueueIsFull(Queue* Q);

int	  QueueTest();

#endif 
