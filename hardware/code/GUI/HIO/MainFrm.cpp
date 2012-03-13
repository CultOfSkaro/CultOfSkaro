// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "HIO.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_HELIOS_START, &CMainFrame::OnHeliosStart)
	ON_COMMAND(ID_HELIOS_STOP, &CMainFrame::OnHeliosStop)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{}

CMainFrame::~CMainFrame()
{}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::OnHeliosStart()
{
	HStart();
}

void CMainFrame::OnHeliosStop()
{
	HStop();
}

void CMainFrame::OnClose()
{
	// TODO: Preserve the position and size of my window 
	WINDOWPLACEMENT    wp;

    // before it is destroyed, save the position of the window
    wp.length = sizeof wp;

    if ( GetWindowPlacement(&wp) )
    {

        if ( IsIconic() )
          // never restore to Iconic state
          wp.showCmd = SW_SHOW ;

        if ((wp.flags & WPF_RESTORETOMAXIMIZED) != 0)
          // if maximized and maybe iconic restore maximized state
          wp.showCmd = SW_SHOWMAXIMIZED ;

        // and write it to the .INI file
        WriteWindowPlacement(&wp);
    }
    
    CFrameWnd::OnClose();
}

void CMainFrame::ActivateFrame(int nCmdShow)
{
	 // nCmdShow is the normal show mode this frame should be in
    // translate default nCmdShow (-1)
    if (nCmdShow == -1)
    {
        if (!IsWindowVisible())
            nCmdShow = SW_SHOWNORMAL;
        else if (IsIconic())
            nCmdShow = SW_RESTORE;
    }

    // bring to top before showing
    BringToTop(nCmdShow);

    if (nCmdShow != -1)
    {
        // show the window as specified
      WINDOWPLACEMENT wp;
      
      if ( !ReadWindowPlacement(&wp) )
      {
          ShowWindow(nCmdShow);
      }
      else
      {
         if ( nCmdShow != SW_SHOWNORMAL )  
           wp.showCmd = nCmdShow;

         SetWindowPlacement(&wp);
         // ShowWindow(wp.showCmd);
      }

      // and finally, bring to top after showing
      BringToTop(nCmdShow);
    }

    return ;
}


static char szSection[]   = "Settings";
static char szWindowPos[] = "WindowPos";
static char szFormat[] = "%u,%u,%d,%d,%d,%d,%d,%d,%d,%d";

BOOL CMainFrame::ReadWindowPlacement(WINDOWPLACEMENT *pwp)
{
    CString strBuffer;
    int    nRead ;

    strBuffer = AfxGetApp()->GetProfileString(szSection, szWindowPos);
    if ( strBuffer.IsEmpty() )  return FALSE;

    nRead = sscanf_s(strBuffer, szFormat,
                &pwp->flags, &pwp->showCmd,
                &pwp->ptMinPosition.x, &pwp->ptMinPosition.y,
                &pwp->ptMaxPosition.x, &pwp->ptMaxPosition.y,
                &pwp->rcNormalPosition.left,  &pwp->rcNormalPosition.top,
                &pwp->rcNormalPosition.right, &pwp->rcNormalPosition.bottom);
    if ( nRead != 10 )  return FALSE;
    pwp->length = sizeof(WINDOWPLACEMENT);

    return TRUE;
}

// Write a window placement to settings section of app's ini file.

void CMainFrame::WriteWindowPlacement(WINDOWPLACEMENT *pwp)
{	
    char szBuffer[sizeof("-32767")*8 + sizeof("65535")*2];
    int max = 1;
    CString s;

    sprintf_s(szBuffer, sizeof("-32767")*8 + sizeof("65535")*2, szFormat,
            pwp->flags, pwp->showCmd,
            pwp->ptMinPosition.x, pwp->ptMinPosition.y,
            pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
            pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
            pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
     AfxGetApp()->WriteProfileString(szSection, szWindowPos, szBuffer);

}