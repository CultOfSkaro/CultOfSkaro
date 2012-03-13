
#include "stdafx.h"
#include "DirectDrawWrapper.h"
#include "ddraw.h"


CDirectDrawWrapper::CDirectDrawWrapper()
{	
	m_ddobject  = NULL; // Set initial values to NULL

	m_semaphore	= new CSemaphore(1,1,NULL,NULL); //To protect against multiple thread access
}

CDirectDrawWrapper::~CDirectDrawWrapper()
{
	Destroy();
}


//IplImage* image = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,4);
//for(int i = 0 ; i < 640*480 ; i++)
//	((DWORD*)image->imageData)[i] = 0xFFFF0000;
//Draw(image);
//cvReleaseImage(&image);



/////////////////////////////////////////////

void CDirectDrawWrapper::Destroy()
{
	m_semaphore->Lock();
	{
		if(m_ddobject != NULL) 
		{ 
			if(m_surface_primary != NULL) 
			{ 
				m_surface_primary->Release(); 
				m_surface_primary = NULL; 
			}	
			
			if(m_surface_backbuffer != NULL) 
			{ 
				m_surface_backbuffer->Release(); 
				m_surface_backbuffer = NULL; 
			}	

			if(m_clipper != NULL) 
			{ 
				m_clipper->Release(); 
				m_clipper = NULL; 
			}	
			
			m_ddobject->Release(); 
			m_ddobject = NULL; 
		}
	}
	m_semaphore->Unlock();
}

bool CDirectDrawWrapper::Init(HWND handle, int width, int height)
{
	bool return_value = true;

	m_semaphore->Lock();
	{
		do 
		{
			if(m_ddobject)
			{
				return_value = false;
				break;
			}
			
			m_window_handle = handle;
			m_lastWidth = width;
			m_lastHeight= height;

			/* STEP 1: Create DirectDraw object	*/
			m_result = DirectDrawCreate(NULL, &m_ddobject, NULL);
			if(m_result != DD_OK) 
			{
				m_ddobject = NULL;		
				return_value = false;
				break;
			}
			
			

			/* STEP 2: Determine the applications behavior */		
			m_result = m_ddobject->SetCooperativeLevel(m_window_handle, DDSCL_NORMAL); 
			if(m_result != DD_OK) 
			{		
				Destroy();
				return_value = false;
				break;
			}


			/* STEP 4: Set up the surface descriptor for the primary surface */	
			memset( &m_descriptor_primary, 
					0, 
					sizeof(m_descriptor_primary)); // zero out struct
			m_descriptor_primary.dwSize			= sizeof(m_descriptor_primary); 	
			m_descriptor_primary.dwFlags		= DDSD_CAPS;
			m_descriptor_primary.ddsCaps.dwCaps	= DDSCAPS_PRIMARYSURFACE;
			m_descriptor_primary.dwWidth		= width; // force these values since we cannot get the dialog to do them
			m_descriptor_primary.dwHeight		= height;
			

			
			/* STEP 5:  Create primary surface */
			m_result = m_ddobject->CreateSurface(&m_descriptor_primary, &m_surface_primary, NULL); 
			if(m_result != DD_OK) 
			{		
				m_ddobject->Release(); 
				m_surface_primary	= NULL; 				
				m_ddobject			= NULL; 
				return_value		= false;
				break;
			}


			
			/* STEP 6: Create a clipper to ensure that our drawing stays inside our window */
			m_result = m_ddobject->CreateClipper(0, &m_clipper, NULL);
			if(m_result != DD_OK ){
				m_surface_primary->Release();				
				m_ddobject->Release();
				m_surface_primary	= NULL; 
				m_ddobject			= NULL; 
				return_value		= false;
				break;
			}



			/* STEP 7: Set the clipper to our window handle */ 
			m_result = m_clipper->SetHWnd(0, m_window_handle);
			if(m_result != DD_OK)
			{
				m_clipper->Release();
				m_surface_primary->Release();
				m_ddobject->Release();
				m_clipper			= NULL;
				m_surface_primary	= NULL; 
				m_ddobject			= NULL; 
				return_value		= false;
				break;
			}
			


			/* STEP 8: Attach the clipper to the primary surface*/ 
			m_result = m_surface_primary->SetClipper(m_clipper);
			if(m_result != DD_OK)
			{
				m_clipper->Release();
				m_surface_primary->Release();
				m_ddobject->Release();
				m_clipper			= NULL;
				m_surface_primary	= NULL; 
				m_ddobject			= NULL; 
				return_value		= false;
				break;
			}



			/* STEP 9: Set up the Surface descriptor for the Backbuffer surface */
			memset( &m_descriptor_backbuffer, 
					0, 
					sizeof(m_descriptor_backbuffer));
			m_descriptor_backbuffer.dwSize			= sizeof( m_descriptor_backbuffer );
			m_descriptor_backbuffer.dwFlags			= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
			m_descriptor_backbuffer.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
			m_descriptor_backbuffer.dwWidth			= width;
			m_descriptor_backbuffer.dwHeight		= height;




			/* STEP 10: Create the backbuffer surface */ 
			m_result = m_ddobject->CreateSurface( &m_descriptor_backbuffer, &m_surface_backbuffer, NULL );
			if( m_result != DD_OK )
			{
				m_surface_backbuffer->Release();
				m_clipper->Release();
				m_surface_primary->Release();
				m_ddobject->Release();
				m_surface_backbuffer= NULL;
				m_clipper			= NULL;
				m_surface_primary	= NULL; 
				m_ddobject			= NULL; 
				return_value		= false;
				break;
			}
		} while(false);
	}	
	m_semaphore->Unlock();
	
	return return_value;
}



void CDirectDrawWrapper::Black()
{
	m_semaphore->Lock();
	{
		do
		{
			/* STEP 0: If Direct Draw object is null, then we can't draw to it*/
			if(!m_ddobject)
				break;				



			/* STEP 1: Initialize variables */
			int width	= m_lastWidth;
			int height	= m_lastHeight;
			int y;   
			DWORD *pScreen;
		   


			/* STEP 2: Initialize Direct draw surface descriptor struct*/ 
			memset(	&m_descriptor_copy, 
					0, 
					sizeof(m_descriptor_copy));
			m_descriptor_copy.dwSize = sizeof(m_descriptor_copy);



			/* STEP 3: Lock the back buffer so we can write a new image to it*/ 
			m_result = m_surface_backbuffer->Lock(NULL, &m_descriptor_copy, 0, NULL);
			if(m_result != DD_OK) 
				break;



			/* STEP 4: Grab the raw data pointers and store them in a more useful local variable*/
			pScreen = (DWORD*) m_descriptor_copy.lpSurface;    



			/* STEP 5: Copy image to screen, line by line. Is there a faster way of doing this?*/ 
			for(y = 0; y < height; y++) {
				//memcpy(pScreen, pFrame, width*4);
				memset(pScreen, 0, width*4);
				pScreen += m_descriptor_copy.lPitch >> 2;        
			}



			/* STEP 6: Unlock the backbuffer now that we have finished writing to it*/ 
			m_surface_backbuffer->Unlock(m_descriptor_copy.lpSurface);



			/* STEP 7: Store the size of this write to the back buffer so that we can use in the Refresh() function*/
			m_lastWidth  = width;
			m_lastHeight = height;
		} while(false);	
	}
	m_semaphore->Unlock();

	/* Step 8: Refresh will Blit (copy) the back buffer over to the primary surface */
	Refresh();
}



void CDirectDrawWrapper::DrawImage(IplImage *image)
{
	m_semaphore->Lock();
	{
		do
		{
			/* STEP 1: If Direct Draw object is null, then we can't draw to it*/
			if(!m_ddobject)
				break;

			int width	= image->width;
			int height	= image->height;
			int y;
			DWORD *pFrame;
			DWORD *pScreen;
		   


			/* STEP 2: Initialize Direct draw surface descriptor struct*/ 
			memset(	&m_descriptor_copy, 
					0, 
					sizeof(m_descriptor_copy));
			m_descriptor_copy.dwSize = sizeof(m_descriptor_copy);



			/* STEP 3: Lock the back buffer so we can write a new image to it*/ 
			m_result = m_surface_backbuffer->Lock(NULL, &m_descriptor_copy, 0, NULL);
			if(m_result != DD_OK) 
				break;


			/* STEP 4: Grab the raw data pointers and store them in a more useful local variable*/
			pScreen = (DWORD*) m_descriptor_copy.lpSurface;
			pFrame	= (DWORD*) image->imageData;



			/* STEP 5: Copy image to screen, line by line. Is there a faster way of doing this?*/ 
			for(y = 0; y < height; y++) {
				memcpy(pScreen, pFrame, width*4);
				pScreen += m_descriptor_copy.lPitch >> 2;
				pFrame  += width;
			}


			/* STEP 6: Unlock the backbuffer now that we have finished writing to it*/ 
			m_surface_backbuffer->Unlock(m_descriptor_copy.lpSurface);


			/* STEP 7: Store the size of this write to the back buffer so that we can use in the Refresh() function*/
			m_lastWidth  = width;
			m_lastHeight = height;	
		} while(false);
	}
	m_semaphore->Unlock();

	/* Step 8: Refresh will Blit (copy) the back buffer over to the primary surface */
	Refresh();
}

void CDirectDrawWrapper::Refresh()
{
	m_semaphore->Lock();
	{
		do
		{
			if(!m_ddobject)
				break;

			RECT	rcRectSrc;
			RECT	rcRectDest;
			POINT	p;

			/* STEP 1: We need to figure out where on the primary surface our window lives*/ 
			p.x = 0; 
			p.y = 0;
			::ClientToScreen(m_window_handle, &p);
			//ClientToScreen(&p);

			/* STEP 2: Then we need to find out how large the window we are writing to is*/
			::GetClientRect(m_window_handle, &rcRectDest);
			//GetClientRect(&rcRectDest);

			// To determine the *actual* size of the DirectDraw surface
			//printf("CDirectDrawWrapper::Refresh() w: %d h:%d\r\n",rcRectDest.right-rcRectDest.left,rcRectDest.bottom-rcRectDest.top);

			/* STEP 3: Now we need to shift the rectangle by the location of the upper left corner of the window*/
			OffsetRect(&rcRectDest, p.x, p.y);

			/* STEP 4: Define the source rectangle as the size of the back buffer [(0,0) to (width,height)]*/
			SetRect(&rcRectSrc, 0, 0, m_lastWidth , m_lastHeight);

			/* STEP 5: Blit (copy) the back buffer over to the primary surface*/
			m_result = m_surface_primary->Blt( &rcRectDest, m_surface_backbuffer, &rcRectSrc, DDBLT_WAIT, NULL);			
		} while(false);
	}	
	m_semaphore->Unlock();
}














