#if !defined(AFX_CHILDDIALOG_H__011FF183_FD84_11D7_A638_002018641C4D__INCLUDED_)
#define AFX_CHILDDIALOG_H__011FF183_FD84_11D7_A638_002018641C4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GenericChildDialog.h"

enum
{
    BASE /* not used */ = WM_USER,
    CHANGED_VALUE,
};

/////////////////////////////////////////////////////////////////////////////
// ChildDialog dialog

class RegisterRowDialog : public CRHGenericChildDialog   // was CDialog
{
public:
	RegisterRowDialog();
	virtual int CRHGetDialogID(){ return(IDD); }

	void SetName(CString str, int register_type, int register_index);
    void SetValue(CString str);


protected:
	enum { IDD = IDD_REGISTER_ROW };	
	int		m_register_type;
	int		m_register_index;
	CString	m_name;
	CString	m_value;	
	bool	m_has_focus;	

	
protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    //Override DDX/DDV support
	virtual BOOL OnInitDialog();	
	DECLARE_MESSAGE_MAP()

public: // Message map functions
	afx_msg void OnGainFocus();
	afx_msg void OnLostFocus();
public:
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDDIALOG_H__011FF183_FD84_11D7_A638_002018641C4D__INCLUDED_)
