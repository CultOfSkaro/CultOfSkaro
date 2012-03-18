
#include "CrossCore.h"

VisionData * vision_data = (VisionData *) VISION_DATA_ADDRESS;
VisionData ** live_vision_data = (VisionData **) VISION_DATA_ADDRESS + 3*sizeof(VisionData);
VisionData ** snap_vision_data = ((VisionData **) VISION_DATA_ADDRESS + 3*sizeof(VisionData)) + 1;

VisionData * getVisionBuffer(){
	int i;
	for(i = 0; i < 3; i++){
		if((vision_data + i) == *live_vision_data){
			continue;
		} else if ((vision_data + i) == *snap_vision_data){
			continue;
		} else {
			return vision_data + i;
		}
	}
	return 0;
}

void inline updateVisionData(VisionData * tmp){
	*live_vision_data = tmp;
}
