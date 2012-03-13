#pragma once

#include "stdafx.h"

class ImageUtilities
{
public:
	ImageUtilities(void){}

	static bool BufferToImage(HeliosPacketHeader* header, void* buffer, IplImage* image, int image_type);
	static bool DrawCorrelations(IplImage* image, CvMat* mat);

};
