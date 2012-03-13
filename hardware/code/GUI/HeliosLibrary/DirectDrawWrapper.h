
#ifndef CDIRECTDRAWWRAPPER_H
#define CDIRECTDRAWWRAPPER_H

#include "ddraw.h"
#include "cv.h"

class CDirectDrawWrapper
{
public:
	CDirectDrawWrapper();
	~CDirectDrawWrapper();

	bool Init(HWND handle, int width, int height);
	void DrawImage(IplImage *image);
	void Refresh();
	void Destroy();
	bool Valid() {return (m_ddobject != NULL);}
	void Black();

	HRESULT Error() {return m_result;}

protected:
	HWND				m_window_handle;			// Window handle of window to draw on
	LPDIRECTDRAW		m_ddobject;					// Pointer to DirectDraw object

	DDSURFACEDESC		m_descriptor_copy;			// DirectDraw Surface Descriptor
	DDSURFACEDESC		m_descriptor_primary;		// DirectDraw Surface Descriptor
	DDSURFACEDESC		m_descriptor_backbuffer;	// DirectDraw Surface Descriptor
	LPDIRECTDRAWSURFACE	m_surface_primary;			// Pointer to primary surface
	LPDIRECTDRAWSURFACE	m_surface_backbuffer;		// Pointer to back buffer
	LPDIRECTDRAWCLIPPER m_clipper;					// Pointer to clipper that clips our surface to the window
	
	HRESULT				m_result;

	int					m_lastWidth;
	int					m_lastHeight;

	CSemaphore*			m_semaphore;				// To protect against multiple thread access
};


#endif