#ifndef SKARO_WIRELESS
#define SKARO_WIRELESS

#include "queue.h"
#include "xuartlite.h"
#include "CrossCore.h"

#define WIRELESS_DEBUG        0x0
#define WIRELESS_CONTROL_LOG  0x1
#define WIRELESS_CONTROL_LOG_EXT  0x2
#define WIRELESS_VISION_FRAMERATE	0x3
#define	WIRELESS_BLOB_REPORT	0x4

#define WRITE_QUEUE_SIZE      800
#define READ_QUEUE_SIZE      50

typedef struct {
	unsigned char type;
	unsigned char length;
	unsigned char data[10];
} Wireless_Command;

typedef struct {
  /* Wireless sending */
  int send_in_progress;
  Queue write_queue;
  unsigned char write_data[WRITE_QUEUE_SIZE*sizeof(void*)];
  /* Wireless receiving */
  int receive_in_progress;
  Queue read_queue;
  void * read_data[READ_QUEUE_SIZE];
  Wireless_Command read_array[READ_QUEUE_SIZE];
  int read_array_position;
  /* UART device */
  XUartLite uart;
} Skaro_Wireless;

int Wireless_Init(Skaro_Wireless * w);
int Wireless_Destroy(Skaro_Wireless * w);

int Wireless_Send(Skaro_Wireless * w, char commmand, char length, char * data);

void Wireless_Debug(char * message);
int Wireless_ControlLog(float actual, float expected);
int Wireless_ControlLog_Ext(float actual, float expected, float saturated, float unsaturated, float refresh_rate);
int Wireless_Blob_Report(int length, Blob * blobs);

extern Skaro_Wireless wireless;
#endif
