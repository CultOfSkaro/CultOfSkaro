/**
\file
\author Barrett Edwards

FILE:				Queue.c
AUTHOR:				Barrett Edwards
CREATED:			1 June 2007
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	Queue.h
DESCRIPTION:		See Queue.h

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

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>

#include "Header.h"
#include "Queue.h"
 

/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/


/** Initialize a pre-alocated Queue struct to an empty state.
This Queue implementation requires that the user code allocate 
the memory space for both the Queue struct AND the data array
where the data pushed onto the Queue will be stored. 


\param Q	 A pointer to the pre-allocated Queue struct
\param data  A void pointer array that is size * sizeof(void*)
\param size  Maximum number of elements the Queue can hold
*/
void QueueInit(Queue* Q, void** data, int MaxElements)
{
	//xil_printf("QueueInit, max elements: %d\r\n", MaxElements);
	Q->data = data;
	Q->capacity = MaxElements;
	QueueClear(Q);
}



/** Resets the Queue to contain zero elements		
	This does not modify any data on the Queue.

	The number of elements on the stack is reset to zero.
	The 'front' index of the queue is returned to its initial 
	position (0), which points to the first element in the Queue.
	The 'back' index is also returned to its initial empty 
	position (-1). 
 
\param Q The Queue that will be cleared. 
*/
void QueueClear(Queue* Q) 
{
  Q->size  = 0;
  Q->front = 0;
  Q->back  = -1;
}







/** Appends a data element onto the back of the Queue
	The current index of the back of the Queue is held 
	in the Queue::back field. To append a value onto the back
	of the Queue the Queue::back field is incremented by one 
	and then the new data value is inserted into the data
	array at that new index location. If the incrementing of 
	the Queue::back index causes it to roll off the end of the 
	array, then the Queue::back is set back to 0.

	This function also checks to see if the Queue is already 
	full. If it is full, then this function returns FALSE.
	This function will return TRUE if the insertion was 
	successful. 

\param Q The Queue to perform this function on
\param X The data to be appended to the back of the Queue
\return Returns TRUE if insertion was successful, FALSE otherwise
*/
int QueuePush(Queue* Q, void* X ) 
{
	if(QueueIsFull(Q)) 
		return FALSE;

	Q->back++;
	if(Q->back >= Q->capacity)
		Q->back = 0;

	Q->data[Q->back] = X;
	Q->size++;	
	
	return TRUE;
}




/** Returns the data element at the front of the Queue
	This #does not# pop the data off the front of the Queue.
	It only returns a copy of it.

	If the Queue is empty, then this function will return NULL. 

\param Q The Queue to perform this function on
\return Returns a copy of the first element of the Queue, returns NULL if Queue is empty
*/
void* QueueFront(Queue* Q) 
{
	if(!QueueIsEmpty(Q)) 
		return Q->data[Q->front];
    
	return NULL;
}



/** Removes and Returns the first element of the Queue.
	This function returns NULL if the Queue is empty.

\param Q The Queue to perform this function on
\return Returns the front element in the Queue
*/
void* QueuePop(Queue* Q) 
{
	void* X = QueueFront(Q);
	if(!QueueIsEmpty(Q)) 
	{
		Q->size--;
		Q->front++;
		if(Q->front >= Q->capacity)
			Q->front = 0;
	}
	else {
		//xil_printf("Queue is empty!\r\n");
	}

	return X;
}



/** Returns TRUE if the Queue is empty

\param Q The Queue to perform this function on
\return Returns TRUE if the Queue is empty, returns FALSE otherwise
*/
int QueueIsEmpty(Queue* Q)
{
	return Q->size == 0;
}



/** Returns TRUE if the Queue is full

\param Q The Queue to perform this function on
\return Returns TRUE if the Queue is full, returns FALSE otherwise
*/
int QueueIsFull(Queue* Q)
{
	return Q->size == Q->capacity;
}



/** Code to test this Queue implementation
This code is only included for example purposes and to ensure that 
this implementation works in all situations

This code at present is not exhaustive.

STEPS:
- Test 1
  -# Push some integers onto the Queue inplace of pointers.
  -# Pop all of the elements off the queue to make sure that they match
- Test2 
  -# TODO 

\todo Write a more exhaustive test 

*/
int QueueTest()
{
	Queue queue;
	void* ary[10];

	QueueInit( &queue, ary, 10 );

	// Test 1
	{
		{
			int i;
			for( i = 0 ; i < 10 ; i++ )
				QueuePush( &queue, (void*) i ); 
		}

		if(QueueIsFull(&queue) != TRUE)
			return 1;

		{
			int i;
			for( i = 0 ; i < 10 ; i++ )
			{
				int val = (int) QueuePop( &queue );
				if(val != i)
					return 2;
			}		
		}
		
		if(QueueIsEmpty(&queue) != TRUE)
			return 3;

		if(QueueIsFull(&queue) != FALSE)
			return 4;
	}


	// Test 2
	{
		QueuePush( &queue, (void*) 1234); 
		if((int)QueueFront(&queue) != 1234)
			return 5;
	}


	return 0;
}
