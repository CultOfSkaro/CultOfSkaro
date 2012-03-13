#if !defined(AFX_GENERICCHILDDIALOG_H__5C5B8861_EC8F_11D7_A638_002018641C4D__INCLUDED_)
#define AFX_GENERICCHILDDIALOG_H__5C5B8861_EC8F_11D7_A638_002018641C4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenericChildDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRHGenericChildDialog dialog

class CRHGenericChildDialog : public CDialog
{
// Construction
public:
	//CRHGenericChildDialog(CWnd* pParent = NULL);   // standard constructor
	//CRHGenericChildDialog( UINT nIDTemplate, CWnd* pParentWnd = NULL );
	CRHGenericChildDialog();

	virtual int CRHGetDialogID() = 0;  // must be provided by derived classes

	void CRHCreateGenericChildDialog(CWnd *pParent, 
                                         int PlaceMarkerCtrlID, 
                                         int Id, 
                                         CRect *ABorderRect = NULL);
        void CRHCreateGenericChildDialog(CWnd *pParent, 
                                         CRect *pPlaceMarkerRect, 
                                         int Id, 
                                         CRect *ABorderRect = NULL);

        static CRect CRHRectForGroupBox;

// Dialog Data
	//{{AFX_DATA(CRHGenericChildDialog)
	//enum { IDD = _UNKNOWN_RESOURCE_ID_ }; <<< not needed, and produces a compiler error
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRHGenericChildDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRHGenericChildDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void CRHPostMessageToParent(UINT message, LPARAM lParam);
private:
        WPARAM CRHId; // used by CRHPostMessageToParent(), sent in any messages to the parent to indicate where it's come from
        CWnd* CRHpParent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERICCHILDDIALOG_H__5C5B8861_EC8F_11D7_A638_002018641C4D__INCLUDED_)
