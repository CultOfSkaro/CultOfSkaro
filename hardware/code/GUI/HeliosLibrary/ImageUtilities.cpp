#include "StdAfx.h"
#include "ImageUtilities.h"




bool ImageUtilities::BufferToImage(HeliosPacketHeader* header, void* buffer, IplImage* image, int image_type)
{	
	unsigned int	val, left, right, W,R,G,B,num;
	int				k				= 0;
	int				end				= image->width * image->height;
	unsigned int*	dest			= (unsigned int*) image->imageData;	
	bool			return_value	= true;
	unsigned char*  buf				= (unsigned char*) buffer;

	unsigned int frame_count	= (buf[0] << 24) | (buf[1] << 16) |  (buf[2] << 8) | buf[3];
	unsigned int camera_id		= (buf[4] << 8) | buf[5];
	image->alphaChannel = frame_count;
	image->align = camera_id;
	buf+=6;

	switch(image_type){
	  /*case PKT_IMG_NULL://				cvSet(image, cvScalar(128));  // Set Image to all Blue 
			  //						return_value = false;
			  //						break;

								       
	 case PKT_IMG_GRAYSCALE8:	   
									for(int i = 0 ; i < end ;i++)
									{
										val		= buf[k++];				
										dest[i] = val << 16 | val << 8 | val;				
									}			
									break;
									*/
	  /*case PKT_IMG_GRAYSCALE16:		cvSet(image, cvScalar(128));  // Set Image to all Blue 
	    break;*/
	case PKT_IMG_NULL:
	case PKT_IMG_RGB565:			/*RGB565*/		
									for(int i = 0 ; i < end ;i++)
									{
										left	= buf[k++];
										right	= buf[k++];
										R		= left & 0x000000F8;
										G		= (left & 0x07) << 5 | (right & 0x00E0) >> 3;
										B		= (right << 3) & 0x000000FF;				
										dest[i] = R << 16 | G << 8 | B;	
									}

									break;

	case PKT_IMG_BINARY:			/*Binary 0,1*/
									for(int i = 0 ; i < end ;i++)
									{
										if(buf[k++] > 0) dest[i] = 0x00FFFFFF;
										else				 dest[i] = 0x00000000;
									}	
									break;
									
	case PKT_IMG_COLOR_SEGMENTED:	/*8-bit Segmented image (each bit represents a color threshold match)*/
									for(int i = 0 ; i < end ;i++)
									{
										switch(buf[k++]){
										default:
										case 0x00: dest[i] = 0x00808080; break; // No match grey
										case 0x01: dest[i] = 0x00000000; break; // Black
										case 0x02: dest[i] = 0x00FF0000; break; // Red
										case 0x04: dest[i] = 0x00FFFF00; break; // Yellow
										case 0x08: dest[i] = 0x0000FF00; break; // Green
										case 0x10: dest[i] = 0x0000FFFF; break; // Cyan
										case 0x20: dest[i] = 0x000000FF; break; // Blue
										case 0x40: dest[i] = 0x00FF00FF; break; // Purple
										case 0x80: dest[i] = 0x00FFFFFF; break; // White														
										}
									}	
									break;

	case PKT_IMG_FEATURE8:			/* Feature Image 8-bit greyscale*/
									for(int i = 0 ; i < end ; i++)
									{
										val	= buf[k++];
										dest[i] = val << 16 | val << 8 | val;
									}
									break;	

	case PKT_IMG_FEATURE16:			/* Feature image 16-bit greyscale*/								
									for(int i = 0 ; i < end ; i++)
									{
										val	= buf[k] << 8  | buf[k+1];
										k+=2;
										W = (val >> 8) & 0x00F0;
										R = (val >> 4) & 0x00F0;
										G = (val     ) & 0x00F0;
										B = (val << 4) & 0x00F0;

										if(W != 0)
											dest[i] = W << 16 | W << 8 | W;
										else if(R != 0)
											dest[i] = R << 16;
										else if(G != 0)
											dest[i] = G << 8;
										else
											dest[i] = B;									
									}
											
									break;	

	case PKT_IMG_HSV1688:			cvSet(image, cvScalar(128));  // Set Image to all Blue 
									break;

	case PKT_IMG_YUV:				cvSet(image, cvScalar(128));  // Set Image to all Blue 
									break;

	case PKT_IMG_FEATURE8_LIST:		cvSet(image, cvScalar(128));  // Set Image to all Blue 
									break;

	case PKT_IMG_FEATURE16_LIST:	cvSet(image, cvScalar(0)); // Clear image. set to black
									num = (header->buffer_size-6) / 8; // number of features in list								
									for(uint32 i = 0 ; i < num ; i++)
									{
										uint32 y = buf[k+0] << 8 | buf[k+1];
										uint32 x = buf[k+2] << 8 | buf[k+3];
										uint32 v = buf[k+4] << 8 | buf[k+5];
										k+=8;
										
										W = (v >> 8) & 0x00F0;
										R = (v >> 4) & 0x00F0;
										G = (v     ) & 0x00F0;
										B = (v << 4) & 0x00F0;

										if     (W != 0) dest[y*640+x] = W << 16 | W << 8 | W;
										else if(R != 0)	dest[y*640+x] = R << 16;
										else if(G != 0)	dest[y*640+x] = G << 8;
										else			dest[y*640+x] = B;																				
									}								
									break;

	case PKT_IMG_RANK_6_BIT:		/* 6-Bit grayscale image */
									for(int i = 0 ; i < end ; i++)
									{
										val	= buf[k++] << 2;
										dest[i] = val << 16 | val << 8 | val;
									}										
									break;

	case PKT_IMG_RANK_8_BIT:		/* 8-Bit grayscale image */
									for(int i = 0 ; i < end ; i++)
									{
										val	= buf[k++];
										dest[i] = val << 16 | val << 8 | val;
									}
									break;

	case PKT_IMG_CCSTREAK_LIST:		cvSet(image, cvScalar(128,128,128,0));  // Set Image to all gray 
									num = (header->buffer_size-6) / 8; // number of streaks in list								
									for(uint32 i = 0 ; i < num ; i++)
									{
										uint32 y		= buf[k+0] << 8 | buf[k+1];
										uint32 xstart	= buf[k+2] << 8 | buf[k+3];
										uint32 xend		= buf[k+4] << 8 | buf[k+5];
										k+=8;

										for( ; xstart <= xend ; xstart++)
											dest[y*640+xstart] = 0x00ffffff; // just set to white										
									}	
									break;

	case PKT_IMG_CCSTREAK0_LIST:	cvSet(image, cvScalar(128,128,128,0));  // Set Image to all Gray									
									num = (header->buffer_size-6) / 8; // number of streaks in list								
									for(uint32 i = 0 ; i < num ; i++)
									{
										uint32 y		= buf[k+0] << 8 | buf[k+1];
										uint32 xstart	= buf[k+2] << 8 | buf[k+3];
										uint32 xend		= buf[k+4] << 8 | buf[k+5];
										k+=8;

										for( ; xstart <= xend ; xstart++)
											dest[y*640+xstart] = 0x00000000; // just set to Black
									}	
									break;
		
	case PKT_IMG_CCSTREAK1_LIST:	cvSet(image, cvScalar(128,128,128,0));  // Set Image to all Gray 
									num = (header->buffer_size-6) / 8; // number of streaks in list								
									for(uint32 i = 0 ; i < num ; i++)
									{
										uint32 y		= buf[k+0] << 8 | buf[k+1];
										uint32 xstart	= buf[k+2] << 8 | buf[k+3];
										uint32 xend		= buf[k+4] << 8 | buf[k+5];
										k+=8;

										for( ; xstart <= xend ; xstart++)
											dest[y*640+xstart] = 0x00FF0000; // just set to Red
									}	
									break;

	case PKT_IMG_CCSTREAK2_LIST:	cvSet(image, cvScalar(128,128,128,0));  // Set Image to all Gray 
									num = (header->buffer_size-6) / 8; // number of streaks in list								
									for(uint32 i = 0 ; i < num ; i++)
									{
										uint32 y		= buf[k+0] << 8 | buf[k+1];
										uint32 xstart	= buf[k+2] << 8 | buf[k+3];
										uint32 xend		= buf[k+4] << 8 | buf[k+5];
										k+=8;

										for( ; xstart <= xend ; xstart++)
											dest[y*640+xstart] = 0x00FFFF00; // just set to Yellow
									}	
									break;

	case PKT_IMG_CCSTREAK3_LIST:	cvSet(image, cvScalar(128,128,128,0));  // Set Image to all Gray 
									num = (header->buffer_size-6) / 8; // number of streaks in list								
									for(uint32 i = 0 ; i < num ; i++)
									{
										uint32 y		= buf[k+0] << 8 | buf[k+1];
										uint32 xstart	= buf[k+2] << 8 | buf[k+3];
										uint32 xend		= buf[k+4] << 8 | buf[k+5];
										k+=8;

										for( ; xstart <= xend ; xstart++)
											dest[y*640+xstart] = 0x0000FF00; // just set to Green
									}	
									break;

	case PKT_IMG_CCSTREAK4_LIST:	cvSet(image, cvScalar(128,128,128,0));  // Set Image to all Gray 
									num = (header->buffer_size-6) / 8; // number of streaks in list								
									for(uint32 i = 0 ; i < num ; i++)
									{
										uint32 y		= buf[k+0] << 8 | buf[k+1];
										uint32 xstart	= buf[k+2] << 8 | buf[k+3];
										uint32 xend		= buf[k+4] << 8 | buf[k+5];
										k+=8;

										for( ; xstart <= xend ; xstart++)
											dest[y*640+xstart] = 0x0000FFFF; // just set to Cyan
									}
									break;

	case PKT_IMG_CCSTREAK5_LIST:	cvSet(image, cvScalar(128,128,128,0));  // Set Image to all Gray 
									num = (header->buffer_size-6) / 8; // number of streaks in list								
									for(uint32 i = 0 ; i < num ; i++)
									{
										uint32 y		= buf[k+0] << 8 | buf[k+1];
										uint32 xstart	= buf[k+2] << 8 | buf[k+3];
										uint32 xend		= buf[k+4] << 8 | buf[k+5];
										k+=8;

										for( ; xstart <= xend ; xstart++)
											dest[y*640+xstart] = 0x000000FF; // just set to Blue
									}
									break;

	case PKT_IMG_CCSTREAK6_LIST:	cvSet(image, cvScalar(128,128,128,0));  // Set Image to all Gray 
									num = (header->buffer_size-6) / 8; // number of streaks in list								
									for(uint32 i = 0 ; i < num ; i++)
									{
										uint32 y		= buf[k+0] << 8 | buf[k+1];
										uint32 xstart	= buf[k+2] << 8 | buf[k+3];
										uint32 xend		= buf[k+4] << 8 | buf[k+5];
										k+=8;

										for( ; xstart <= xend ; xstart++)
											dest[y*640+xstart] = 0x00FF00FF; // just set to Purple
									}
									break;

	case PKT_IMG_CCSTREAK7_LIST:	cvSet(image, cvScalar(128,128,128,0));  // Set Image to all Gray 
									num = (header->buffer_size-6) / 8; // number of streaks in list								
									for(uint32 i = 0 ; i < num ; i++)
									{
										uint32 y		= buf[k+0] << 8 | buf[k+1];
										uint32 xstart	= buf[k+2] << 8 | buf[k+3];
										uint32 xend		= buf[k+4] << 8 | buf[k+5];
										k+=8;

										for( ; xstart <= xend ; xstart++)
											dest[y*640+xstart] = 0x00ffffff; // just set to white										
									}
									break;

	default:						cvSet(image, cvScalar(0));  // Set Image to all Black 
									return_value = false;
									break;
	}	

	return return_value;
}



bool ImageUtilities::DrawCorrelations(IplImage* image, CvMat* mat)
{
	int side				= 2;
	CvScalar colorred		= CV_RGB(255, 0, 0);
	CvScalar colorgreen		= CV_RGB(00, 255, 0);

	for(int i = 0 ; i < mat->rows ; i++)
	{
		int x  = (int) cvmGet(mat,i,0);
		int y  = (int) cvmGet(mat,i,1);
		int tx = (int) cvmGet(mat,i,2);
		int ty = (int) cvmGet(mat,i,3);

		//Draw source location	
		cvLine( image, cvPoint( x     , y-side ), cvPoint( x     , y+side ), colorred);
		cvLine( image, cvPoint( x-side, y      ), cvPoint( x+side, y      ), colorred);
		
		//Draw destination location
		cvLine( image, cvPoint( tx     , ty-side ), cvPoint( tx     , ty+side ), colorgreen);	
		cvLine( image, cvPoint( tx-side, ty      ), cvPoint( tx+side, ty      ), colorgreen);
	}

	return true;
}












