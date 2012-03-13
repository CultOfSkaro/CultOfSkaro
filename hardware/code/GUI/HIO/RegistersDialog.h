#if !defined(AFX_DLGSCROLLABLE_H__3586FDB6_FC0D_4FF7_BA09_D86692F0006A__INCLUDED_)
#define AFX_DLGSCROLLABLE_H__3586FDB6_FC0D_4FF7_BA09_D86692F0006A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScrollable.h : header file
//

#include "stdafx.h"



/////////////////////////////////////////////////////////////////////////////
// RegistersDialog dialog

class RegistersDialog : public CDialog
{
// Construction
public:
	RegistersDialog(CWnd* pParent = NULL);   // standard constructor

	// dialog size as you see in the resource view (original size)
	CRect	m_rcOriginalRect;

	// dragging
	BOOL	m_bDragging;
	CPoint	m_ptDragPoint;

	// actual scroll position
	int		m_nScrollPos;

	// actual dialog height
	int		m_nCurHeight;

// Dialog Data
	//{{AFX_DATA(RegistersDialog)
	enum { IDD = IDD_REG_SCROLLABLE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RegistersDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RegistersDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);		
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCROLLABLE_H__3586FDB6_FC0D_4FF7_BA09_D86692F0006A__INCLUDED_)
