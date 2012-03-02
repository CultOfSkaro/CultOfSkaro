#include "skaro_queue.h"
#include <stdlib.h>

void Queue_Init(Skaro_Queue * q, int size){
  q->buffer = (char *) malloc(sizeof(char)*size);
  q->end_pointer = q->buffer + size;
  q->read_pointer = q->buffer;
  q->write_pointer = q->buffer;
}

int Queue_ReadByte(Skaro_Queue * q, char * c){
  if(q->read_pointer == q->write_pointer){
    return 0;
  }else{
    *c = *(q->read_pointer);
    q->read_pointer++;
    if(q->read_pointer == q->end_pointer){
    	print("wrapping around");
      q->read_pointer = q->buffer;
    } 
    return 1;
  }
}

int Queue_WriteByte(Skaro_Queue * q, char c){
  char * next = q->write_pointer + 1;
  if(next == q->end_pointer)
    next = q->buffer;
  if(next == q->read_pointer){
    return 0;
  }else{
    *q->write_pointer = c;
    q->write_pointer = next;
    return 1;
  }
} 

int Queue_Write(Skaro_Queue * q, char length, char * data){
  int i;
  for(i = 0; i < length; i++){
    Queue_WriteByte(q,data[i]);
  }  
}

int Queue_Destroy(Skaro_Queue * q){
  free(q->buffer);
  return 0;
}
