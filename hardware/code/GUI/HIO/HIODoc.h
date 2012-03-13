// HIODoc.h : interface of the CHIODoc class
//


#pragma once


class CHIODoc : public CDocument
{
protected: // create from serialization only
	CHIODoc();
	DECLARE_DYNCREATE(CHIODoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CHIODoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


