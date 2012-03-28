#include "skaro_wireless.h"
#include "Queue.h"
#include "xuartlite.h"
#include <string.h>
#include <stdio.h>
#include <InterruptControl.h>
#include <stdarg.h>
#include <ctype.h>

Skaro_Wireless wireless;

int Wireless_Init(Skaro_Wireless * w){
	w->send_in_progress = 0;
	w->receive_in_progress = 0;
	w->read_array_position = 0;
	QueueInit(&(w->write_queue), (void *)w->write_data, WRITE_QUEUE_SIZE);
	QueueInit(&(w->read_queue), (void *)w->read_data, READ_QUEUE_SIZE);
	return 1;
}

int Wireless_Destroy(Skaro_Wireless * w){
	return 1;
}

int Wireless_Send(Skaro_Wireless * w, char command, char length, char * data){
	unsigned char top;
	CPU_MSR msr = DISABLE_INTERRUPTS();
	QueuePush(&(w->write_queue), (void *)(uint32)command);
	QueuePush(&(w->write_queue),(void *)(uint32)length);

	int i;
	for(i = 0; i < length; i++){
		QueuePush(&w->write_queue, (void *)(uint32)data[i]);
	}

	// If there is no send in progress, initiate a send
	if (!(w->send_in_progress)) {
		// We are about to send it, so pop it off
		top = (unsigned char)(uint32)QueuePop(&(w->write_queue));
		w->send_in_progress = 1;
		XUartLite_Send(&(w->uart), &top, 1);
	}
	RESTORE_INTERRUPTS(msr);
	return 1;
}

void Wireless_Debug(char * message) {
	int length;

	length = strlen(message);
	// do more stuff here (like write to file)
	Wireless_Send(&wireless, WIRELESS_DEBUG, length, message);
}

int Wireless_ControlLog(float actual, float expected){
	char values[8];
	int i;
	for(i = 0; i <4; i++){
		values[i] = *((char *)(&actual)+i);
		values[4+i] = *((char *)(&expected)+i);
	}
	return Wireless_Send(&wireless, WIRELESS_CONTROL_LOG,8, values);
}

int Wireless_ControlLog_Ext(float actual, float expected, int saturated, int unsaturated, int refresh_rate){
	char values[20];
	int i;
	for(i = 0; i < 4; i ++){
		values[i] = *((char *)(&actual)+i);
		values[4+i] = *((char *)(&expected)+i);
		values[8+i] = *((char *)(&saturated)+i);
		values[12+i] = *((char *)(&unsaturated)+i);
		values[16+i] = *((char *)(&unsaturated)+i);
	}
	return Wireless_Send(&wireless, WIRELESS_CONTROL_LOG_EXT,20, values);
}

int Wireless_Blob_Report(int length, Blob * blobs){
	Wireless_Send(&wireless, WIRELESS_BLOB_REPORT, length*sizeof(Blob), blobs);
}

//int mains(){
//  Wireless_Init(&wireless);
//  Wireless_ControlLog(1200,100);
//  Wireless_Debug("How well does this work");
//  char c;
//  int i;
//  int length;
//  char format_string[10];
//  Queue_ReadByte(&wireless.write_queue, &c);
//  printf("%d\n",c);
//  if(c == WIRELESS_DEBUG){
//    strcpy(format_string,"%c\n");
//  } else if (c == WIRELESS_CONTROL_LOG){
//    strcpy(format_string,"%d\n");
//  }
//  Queue_ReadByte(&wireless.write_queue, &c);
//  printf("%d\n",c);
//  length = c;
//  for(i = 0; i < length; i++){
//    Queue_ReadByte(&wireless.write_queue, &c);
//    printf(format_string,c);
//  }
//  Wireless_Destroy(&wireless);
//}
