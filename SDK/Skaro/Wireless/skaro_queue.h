#ifndef SKARO_QUEUE
#define SKARO_QUEUE
/* A queue a FIFO structure.  It is a region of memory with circular access.  As soon as the end of memory is reached, it starts at the beginning (provided the bytes at the beginning have already been read.
*/
typedef struct {
  char * read_pointer;
  char * write_pointer;
  char * buffer;
  char * end_pointer;
} Skaro_Queue;

#include "queue.h"
/* Queue_Initialize sets up a Queue struct.  It mallocs the buffer and sets up the pointers*/
void Queue_Init(Skaro_Queue * q, int size);
/* Queue_ReadByte reads one byte from the Queue and stores it in char c.  If the queue is empty it will return 0, otherwise it will return 1*/
int Queue_ReadByte(Skaro_Queue * q, char * c);
/* Queue_WrietByte writes char c to the Queue. If the queue is full it will return 0, otherwise it will return 1*/
int Queue_WriteByte(Skaro_Queue * q, char c);
int Queue_Write(Skaro_Queue * q, char length, char * data);
/*  Queue_Destroy frees up the buffer used by the queue */
int Queue_Destroy(Skaro_Queue * q);
#endif
