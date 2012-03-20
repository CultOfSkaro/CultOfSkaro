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
} Blob;

typedef struct {
	int numBlobs;
	Blob blobs[MAX_BLOBS];
} VisionData;

//needs space for 3 structs
extern VisionData * vision_data;
extern VisionData ** live_vision_data;
extern VisionData ** snap_vision_data;

extern char * shared_debug_buffer;

//pointers into vision_data

VisionData * getVisionBuffer();

void inline updateVisionData(VisionData * tmp);

#endif
