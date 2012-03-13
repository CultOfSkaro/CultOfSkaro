// BitmapViewer.cpp : implementation file
//

#include "stdafx.h"
#include "BitmapViewer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitmapViewer

CBitmapViewer::CBitmapViewer()
{
    RegisterWindowClass();	
}

CBitmapViewer::~CBitmapViewer()
{
	HDDDestroy();
}

// Register the window class if it has not already been registered.
BOOL CBitmapViewer::RegisterWindowClass()
{	
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, BITMAPVIEWER_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = BITMAPVIEWER_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}


BEGIN_MESSAGE_MAP(CBitmapViewer, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBitmapViewer message handlers

void CBitmapViewer::OnPaint() 
{
	// For some reason I (Barrett) was told that not to call CWnd::OnPaint() here for painting messages
	// But if we don't call it, then Toolbar doesn't get redrawn when we move the window. Why is this???
	CWnd::OnPaint();
	HDDRefresh();
}

BOOL CBitmapViewer::OnEraseBkgnd(CDC* pDC) 
{
    // Don't perform any erasing, since the OnPaint
    // function will simply draw over the background    
	return TRUE;
}

void CBitmapViewer::PreSubclassWindow() 
{
	// By now the window handle is good so we can create the DirectDraw surface 
	HDDInit(GetSafeHwnd(),640,480);
	

	CWnd::PreSubclassWindow();
}


/////////////////////////////////////////////


void CBitmapViewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	HSendMouseLClick(point.x, point.y);

	CWnd::OnLButtonDown(nFlags, point);
}
