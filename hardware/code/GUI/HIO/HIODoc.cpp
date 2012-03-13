// HIODoc.cpp : implementation of the CHIODoc class
//

#include "stdafx.h"
#include "HIO.h"

#include "HIODoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHIODoc

IMPLEMENT_DYNCREATE(CHIODoc, CDocument)

BEGIN_MESSAGE_MAP(CHIODoc, CDocument)
END_MESSAGE_MAP()


// CHIODoc construction/destruction

CHIODoc::CHIODoc()
{
	// TODO: add one-time construction code here

}

CHIODoc::~CHIODoc()
{
}

BOOL CHIODoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CHIODoc serialization

void CHIODoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CHIODoc diagnostics

#ifdef _DEBUG
void CHIODoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHIODoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHIODoc commands
