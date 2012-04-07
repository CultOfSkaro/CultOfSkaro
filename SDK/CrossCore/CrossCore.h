#ifndef SKARO_CROSS_CORE
#define SKARO_CROSS_CORE

#define VISION_DATA_ADDRESS 0x100000
#define MAX_BLOBS           100

#define BLOB_TYPE_BLUE			0
#define BLOB_TYPE_PINK			1
#define BLOB_TYPE_YELLOW		2
#define BLOB_TYPE_CYAN			3
#define BLOB_TYPE_RED			4
#define BLOB_TYPE_GREEN			5
#define NUM_BLOB_TYPES			6

#define UNKNOWN_OBJECT	0
#define TOWER_OBJECT	1
#define	TRUCK_OBJECT		2

#define DISABLED_OBJECT_STATUS 	0
#define FLAG_OBJECT_STATUS		1
#define ENABLED_OBJECT_STATUS	2

#define DEBUG_USB_VISION

#define RED_TEAM	0
#define	BLUE_TEAM	1

#define CURRENT_TEAM BLUE_TEAM

typedef struct {
	int type;
	int left;
	int top;
	int width;
	int height;
	int distance;
	int center;
	float angle;
} Blob;

typedef struct {
	Blob blob;
	int type;
	int status;
	int active;
} Object;

typedef struct {
	Object tower;
	Object truck;
} Team;

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
	Team us;
	Team them;
	Object * current_target;
} Vision;

extern Vision vision;

extern char * shared_debug_buffer;


void Vision_Init(Vision * vision);

VisionData * Vision_GetBuffer(Vision * vision);

void inline Vision_UpdateData(Vision * vision, VisionData * tmp);

#endif
