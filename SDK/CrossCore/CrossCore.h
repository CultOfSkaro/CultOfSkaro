#ifndef SKARO_CROSS_CORE
#define SKARO_CROSS_CORE

#define VISION_DATA_ADDRESS 0x100000
#define MAX_BLOBS           100

#define BLOB_TYPE_BLUE			0
#define BLOB_TYPE_RED			1
#define NUM_BLOB_TYPES			2

//#define DEBUG_USB_VISION

typedef struct {
	int type;
	int left;
	int top;
	int width;
	int height;
	float distance;
	int center;
	float angle;
} Blob;

typedef struct {
	int numBlobs;
	Blob blobs[MAX_BLOBS];
} VisionData;

typedef struct {
	int frameRate;
	VisionData * data;
	//pointers into vision_data
	VisionData ** live_vision_data;
	VisionData ** snap_vision_data;
} Vision;

extern Vision vision;

extern char * shared_debug_buffer;


void Vision_Init(Vision * vision);

VisionData * Vision_GetBuffer(Vision * vision);

void inline Vision_UpdateData(Vision * vision, VisionData * tmp);

#endif
