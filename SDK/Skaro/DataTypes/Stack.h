/**
\unit
\author Barrett Edwards

FILE:				Stack.h
AUTHOR:				Barrett Edwards
CREATED:			20 July 2007
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	Stack.c
DESCRIPTION:		

	This is a simple implementation of a last-in-first-out (LIFO) 
	data storage Stack. This implementation only allows the storage 
	of void pointers (void*). It is expected that these void pointers 
	will be used to point to application specific data.
   
	This implementation #does not allocate any memory# for the Stack 
	structure or it's data array where the void pointers are actually 
	stored. It is expected that the data for the Stack will be allocated
	elsewhere. 

HOWTO:

	A simple code example of Stack usage:
	\pre
	Stack mystack;
	int stack_size = 10;
	void* data_ptr = malloc( stack_size * sizeof(void*) );
	StackInit( &mystack, data_ptr, stack_size );
	...
	StackPush( &mystack, new_data );
	...
	void *old_data = StackPop( &mystack );
	\endpre

	To create a Stack perform the following actions:
	-# Allocate memory for the Stack structure. This can be done on the 
	   heap or on the stack as show above.
	-# Allocate memory for the data table in the Stack. This must be 
	   atleast as large as: size * sizeof(void*). Where size is the 
	   maximum number of entries on the Stack.
	-# Initilize the stack using StackInit();
 
 Related Functions & Groups:
	- Initialization:
		- void StackInit(Stack* stack, void** data, int size);
		- void StackClear(Stack* stack);
	- Data access:
		- void* StackTop(Stack* stack);
		- void* StackPop(Stack* stack);
		- int StackPush(Stack* stack, void* data);
	- Info:
		- int StackIsEmpty(Stack* stack);
		- int StackNumAvailable(Stack* stack);
 
CHANGE LOG 

*/ 

/* Conditional Inclusion ----------------------------------------------------*/
#ifndef STACK_H
#define STACK_H


/* Includes -----------------------------------------------------------------*/
#include "Header.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/

/** The storage struct for a simple Stack
	This implementation only stores void pointers. Therefore, any usage of 
	this Stack implementation should only push a pointer to data onto the 
	Stack rather than the data itself (unless it is a primative data type
	like an int).


	A simple code example of Stack usage:
	\pre
	Stack mystack;
	int stack_size = 10;
	void* data_ptr = malloc( stack_size * sizeof(void*) );
	StackInit( &mystack, data_ptr, stack_size );
	...
	StackPush( &mystack, new_data );
	...
	void *old_data = StackPop( &mystack );
	\endpre

	Related Functions & Groups:
	- Initialization:
		- void StackInit(Stack* stack, void** data, int size);
		- void StackClear(Stack* stack);
	- Data access:
		- void* StackTop(Stack* stack);
		- void* StackPop(Stack* stack);
		- int StackPush(Stack* stack, void* data);
	- Info:
		- int StackIsEmpty(Stack* stack);
		- int StackNumAvailable(Stack* stack);
*/
struct Stack
{
	void**	data; ///< This is the data table array of the Stack
	int		size; ///< This is the maximum number of elemetns that can be stored on the stack
	int		top;  ///< This is the current number of elements on the stack
};
typedef struct Stack Stack; ///< typedef struct Stack Stack;


/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/

void	StackInit(Stack* stack, void** data, int size);
void	StackClear(Stack* stack);

void* 	StackTop(Stack* stack);
void* 	StackPop(Stack* stack);
int		StackPush(Stack* stack, void* data);

int		StackIsEmpty(Stack* stack);
int		StackNumAvailable(Stack* stack);

int		StackTest();

#endif
