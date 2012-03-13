// ChildDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RegisterRowDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RegisterRowDialog dialog


#if 0
                RegisterRowDialog::RegisterRowDialog(CWnd* pParent /*=NULL*/)
	                : CDialog(RegisterRowDialog::IDD, pParent)
                {
                }
#endif


void RegisterRowDialog::DoDataExchange(CDataExchange* pDX)
{
	//printf("RegisterRowDialog::DoDataExchange()\r\n");
	
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RegisterRowDialog)	
	DDX_Text(pDX, IDC_REG_NAME , m_name);
	DDX_Text(pDX, IDC_REG_VALUE, m_value);
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(RegisterRowDialog, CDialog)		
	ON_EN_SETFOCUS(IDC_REG_VALUE, &RegisterRowDialog::OnGainFocus)
	ON_EN_KILLFOCUS(IDC_REG_VALUE, &RegisterRowDialog::OnLostFocus)
//	ON_WM_CHAR()
//ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RegisterRowDialog message handlers

RegisterRowDialog::RegisterRowDialog()
{	
	m_name		= "empty";
	m_has_focus = false;
	m_value		= "0";
}


BOOL RegisterRowDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}





void RegisterRowDialog::SetName(CString str, int register_type, int register_index)
{
	//printf("RegisterRowDialog::SetName()\r\n");
	
	m_name			= str;	
	m_register_type = register_type;
	m_register_index= register_index;

	UpdateData(false);  // set the controls from the values we've just written	
}

void RegisterRowDialog::SetValue(CString str)
// called by parent
{
	//printf("RegisterRowDialog::SetValue()\r\n");
	
	if(!m_has_focus)
	{   // If the cell is not currently selected, then update the value in the cell with the new value
		m_value = str;
		UpdateData(false);  // set the controls from the values we've just written
	}
}

/////////////////////////////////////////////////////////////////////////////


void RegisterRowDialog::OnGainFocus()
{
	//printf("RegisterRowDialog::OnGainFocus()\r\n");

	m_has_focus = true;	
}

void RegisterRowDialog::OnLostFocus()
{
	printf("RegisterRowDialog::OnLostFocus()\r\n");
	CString tmp = m_value;

	UpdateData(true); // Get the new value and store in m_value

	int rv = 0;	
	
	int				value_int   = 0;
	unsigned int	value_uint  = 0;
	unsigned int	value_hex   = 0;
	float			value_float = 0;
	
	char* str = m_value.GetBuffer();
	
	// Convert string value into a value and then 
	// Set Register in HeliosLibrary Backend
	switch(m_register_type){
		case REG_TYPE_CHAR:		rv = sscanf_s(str, "%u", &value_uint);
								if(rv) // Only set the Register if sscanf succeeded
									HSetRegisterChar( m_register_index, value_uint); 
								break;

		case REG_TYPE_INT:		rv = sscanf_s(str, "%i", &value_int);
								if(rv) // Only set the Register if sscanf succeeded
									HSetRegisterInt(  m_register_index, value_int); 
								break;

		case REG_TYPE_UINT:		rv = sscanf_s(str, "%u", &value_uint);
								if(rv) // Only set the Register if sscanf succeeded
									HSetRegisterUInt( m_register_index, value_uint); 
								break;

		case REG_TYPE_HEX:		rv = sscanf_s(str, "0x%x", &value_hex);
								if(rv) // Only set the Register if sscanf succeeded							
									HSetRegisterHex( m_register_index, value_hex); 
								else
								{   // if the first sscanf failed.. then try without the leading 0x
									rv = sscanf_s(str, "%x", &value_hex);
									if(rv) // Only set the Register if sscanf succeeded								
										HSetRegisterHex( m_register_index, value_hex); 
								}
								break;

		case REG_TYPE_FLOAT:	rv = sscanf_s(str, "%f", &value_float);
								if(rv) // Only set the Register if sscanf succeeded
									HSetRegisterFloat(m_register_index, value_float); 
								break;
		default: break;
	}
	m_value.ReleaseBuffer();

	m_has_focus = false;
}

//void RegisterRowDialog::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: Add your message handler code here and/or call default
//	if(nChar == 10)
//		printf("enter\r\n");
//	else 
//		printf("nope\r\n");
//	CRHGenericChildDialog::OnChar(nChar, nRepCnt, nFlags);
//}

//void RegisterRowDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: Add your message handler code here and/or call default
//	if(nChar == 10)
//		printf("enter\r\n");
//	else 
//		printf("nope\r\n");
//	CRHGenericChildDialog::OnKeyDown(nChar, nRepCnt, nFlags);
//}
