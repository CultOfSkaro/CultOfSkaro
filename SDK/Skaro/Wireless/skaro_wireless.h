#ifndef SKARO_WIRELESS
#define SKARO_WIRELESS

#include "queue.h"
#include "xuartlite.h"

#define WIRELESS_DEBUG        0x0
#define WIRELESS_CONTROL_LOG  0x1
#define WIRELESS_CONTROL_LOG_EXT  0x2

#define WRITE_QUEUE_SIZE      400
#define READ_QUEUE_SIZE      400

typedef struct {
  int send_in_progress;
  Queue write_queue;
  unsigned char write_data[WRITE_QUEUE_SIZE*sizeof(void*)];
  Queue read_queue;
  unsigned char read_data[READ_QUEUE_SIZE*sizeof(void*)];
  XUartLite uart;
} Skaro_Wireless;

int Wireless_Init(Skaro_Wireless * w);
int Wireless_Destroy(Skaro_Wireless * w);

int Wireless_Send(Skaro_Wireless * w, char commmand, char length, char * data);

void Wireless_Debug(char * message);
int Wireless_ControlLog(float actual, float expected);
int Wireless_ControlLog_Ext(float actual, float expected, int saturated, int unsaturated, int refresh_rate);

#endif
