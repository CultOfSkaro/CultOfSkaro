
#include "CrossCore.h"

Vision vision;

void Vision_Init(Vision * vision){
	vision->data = (VisionData *) VISION_DATA_ADDRESS;
	vision->live_vision_data = (VisionData **) VISION_DATA_ADDRESS + 3*sizeof(VisionData);
	vision->snap_vision_data = ((VisionData **) VISION_DATA_ADDRESS + 3*sizeof(VisionData)) + 1;
	*vision->snap_vision_data = 0;
	*vision->live_vision_data = 0;
}

VisionData * Vision_GetBuffer(Vision * vision){
	int i;
	for(i = 0; i < 3; i++){
		if((vision->data + i) == *vision->live_vision_data){
			continue;
		} else if ((vision->data + i) == *vision->snap_vision_data){
			continue;
		} else {
			return vision->data + i;
		}
	}
	return 0;
}

void inline Vision_UpdateData(Vision * vision, VisionData * tmp){
	*vision->live_vision_data = tmp;
}

char * shared_debug_buffer = (char * ) 0x100;
