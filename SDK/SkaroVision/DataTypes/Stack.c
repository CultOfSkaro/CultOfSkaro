/**
\file
\author Barrett Edwards

FILE:				Stack.c
AUTHOR:				Barrett Edwards
CREATED:			20 July 2007
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	Stack.h
DESCRIPTION:		See Stack.h

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

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>

#include "Header.h"
#include "Stack.h"


/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/


/** Initialize a pre-alocated Stack struct to an empty state.
This Stack implementation requires that the user code allocate 
the memory space for both the Stack struct AND the data array
where the data pushed onto the Stack will be stored. 

\param stack A pointer to the pre-allocated Stack struct
\param data  A void pointer array that is size * sizeof(void*)
\param size  Maximum number of elements the stack can hold
*/
void StackInit(Stack* stack, void** data, int size) 
{
	stack->data	= data;
	stack->size	= size;
	stack->top	= -1;
}



/** Sets the top of Stack position to the empty position (-1)
This does not modify any data on the Stack.

\param stack The Stack that will be cleared. 
*/
void StackClear(Stack* stack) 
{
	stack->top = -1;
}




/** Returns the current element on the Top of the Stack
This #DOES NOT# remove the Top element from the Stack. 
This function only returns a copy of the element.
If the Stack is empty (size == 0), then this function
just returns zero. 

\param stack The stack to perform this function on
\return      The current element on the top of the stack
*/
void* StackTop(Stack *stack)
{
	if(stack->top == -1) 
		return 0;
		
	return stack->data[stack->top];
}



/** Pops the Top element off the Stack and returns it.
If the Stack is empty (size == 0), then this function
just returns zero. 

\param stack The Stack to perform this function on
\return      The Top element on the Stack
*/
void* StackPop(Stack* stack) 
{
	if(stack->top == -1) 
		return 0;

	return stack->data[stack->top--];
}



/** Pushes an element on to the Stack

\param stack The Stack to perfrom this function on
\param data  The element that should be pushed onto the stack
\return      Returns TRUE if successful, FALSE otherwise. 
*/
int StackPush(Stack* stack, void* data) 
{
	if(stack->top == stack->size - 1) 
		return FALSE;
	
	++stack->top;

	stack->data[stack->top] = data;

	return TRUE;
}



/** Returns TRUE if the Stack contains zero elements

\param stack The Stack to perform this function on
\return		 Returns TRUE if the Stack is Empty, FALSE otherwise
*/
int StackIsEmpty(Stack* stack) 
{
	return (stack->top == -1);
}



/** Returns the number of elements currently on the Stack

\param stack The Stack to perform this function on
\return      The number of elements currently on the stack
*/
int StackNumAvailable(Stack* stack)
{
	return stack->top+1;
}



/** Code to test this Stack implementation
This code is only included for example purposes and to ensure that 
this implementation works in all situations

This code at present is not exhaustive.

STEPS:
- Test 1
  -# Push some integers onto the Stack inplace of pointers.
  -# Pop all of the elements off the Stack to make sure that they match
  -# Then ensure that the Stack is empty
  -# Then ensure that the Stack has zero elements available
- Test2 
  -# TODO 

\todo Write a more exhaustive Stack test 

*/
int StackTest()
{
	Stack stack;
	void* ary[10];

	StackInit( &stack, ary, 10 );

	// Test 1
	{
		{
			int i;
			for( i = 0 ; i < 10 ; i++ )
				StackPush( &stack, (void*) i ); 
		}

		{
			int i;
			for( i = 10-1 ; i >= 0 ; i-- )
			{
				int val = (int) StackPop( &stack );
				if(val != i)
					return 1;
			}		
		}
		
		if(StackIsEmpty(&stack) != TRUE)
			return 2;

		if(StackNumAvailable(&stack) != 0)
			return 3;
	}


	// Test 2
	{
		StackPush( &stack, (void*) 1234); 
		if((int)StackTop(&stack) != 1234)
			return 4;
	}


	// Test 3 
	{
		{
			int i;
			for( i = 0 ; i < 10 ; i++ )
				StackPush( &stack, (void*) i ); 
		}
		
		StackClear(&stack);

		if(StackIsEmpty(&stack) != TRUE)
			return 5;

		if(StackNumAvailable(&stack) != 0)
			return 6;

		if(StackTop(&stack) != 0)
			return 7;

		if(StackTop(&stack) != 0)
			return 8;
	}

	return 0;
}
