// HIOView.h : interface of the CHIOView class
//
#pragma once

using std::vector;

#include "RegistersDialog.h"
#include "BitmapViewer.h"
#include "RegisterRowDialog.h"
#include "afxwin.h"


class CHIOView : public CFormView
{
protected: // create from serialization only
	CHIOView();
	DECLARE_DYNCREATE(CHIOView)

public:
	enum{ IDD = IDD_HIO_FORM };
	

// Attributes
public:
	CHIODoc* GetDocument() const;

	void InitialResize();

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CHIOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CBitmapViewer				m_BitmapViewer;
	RegistersDialog*			m_pdlgScroll;
	//CString						m_text_terminal;
	vector<RegisterRowDialog*>	m_RegisterRowDialogs_char;
	vector<RegisterRowDialog*>	m_RegisterRowDialogs_int;
	vector<RegisterRowDialog*>	m_RegisterRowDialogs_uint;
	vector<RegisterRowDialog*>	m_RegisterRowDialogs_hex;
	vector<RegisterRowDialog*>	m_RegisterRowDialogs_float;
	

// Generated message map functions
protected:	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnClickedSystemStart();
public:
	afx_msg void OnClickedSystemStop();
public:
	afx_msg void OnClickedVideoON();
public:
	afx_msg void OnClickedVideoOFF();
public:
	afx_msg void OnClickedStateON();
public:
	afx_msg void OnClickedStateOFF();
public:
	afx_msg void OnIndexChangedVideoTXType();
public:
	CComboBox m_VideoTXType;
public:
	CString m_text_terminal;
public:
	CEdit m_text_box;
public:
	afx_msg void OnSelectAll();
public:
	afx_msg void OnEditCopy();
public:
	afx_msg void OnEditCut();
public:
	afx_msg void OnEditPaste();
public:
	afx_msg void OnBnClickedEnableDrawCorrolations();
public:
	BOOL m_checkbox_draw_corrolations;
public:
	afx_msg void OnBnClickedAVIStart();
public:
	afx_msg void OnBnClickedAVIStop();
public:
	BOOL m_check_aviasimages;
public:
	afx_msg void OnBnClickedEnableAviWriteAsImage();
public:
	CComboBox m_CBoxAVICompression;
public:
	afx_msg void OnSelchangeCboxAviCompression();
public:
	afx_msg void OnClickedShutter();
public:
	afx_msg void OnMenuSetAVIFilename();
public:
	afx_msg void OnViewRefreshddraw();
};

#ifndef _DEBUG  // debug version in HIOView.cpp
inline CHIODoc* CHIOView::GetDocument() const
   { return reinterpret_cast<CHIODoc*>(m_pDocument); }
#endif

