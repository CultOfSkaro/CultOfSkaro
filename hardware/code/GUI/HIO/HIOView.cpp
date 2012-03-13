// HIOView.cpp : implementation of the CHIOView class
//

#include "stdafx.h"
#include "HIO.h"

#include "HIODoc.h"
#include "HIOView.h"

//#include "VideoDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHIOView

IMPLEMENT_DYNCREATE(CHIOView, CFormView)

BEGIN_MESSAGE_MAP(CHIOView, CFormView)		
	ON_WM_TIMER()
	
	ON_BN_CLICKED(IDC_HIO_FORM_BTN_START,    &CHIOView::OnClickedSystemStart)
	ON_BN_CLICKED(IDC_HIO_FORM_BTN_STOP,     &CHIOView::OnClickedSystemStop)
	ON_BN_CLICKED(IDC_HIO_FORM_BTN_VIDEOON,  &CHIOView::OnClickedVideoON)
	ON_BN_CLICKED(IDC_HIO_FORM_BTN_VIDEOOFF, &CHIOView::OnClickedVideoOFF)
	ON_BN_CLICKED(IDC_HIO_FORM_BTN_STATEON,  &CHIOView::OnClickedStateON)
	ON_BN_CLICKED(IDC_HIO_FORM_BTN_STATEOFF, &CHIOView::OnClickedStateOFF)
	
	ON_CBN_SELCHANGE(IDC_HIO_FORM_CMBO_FRAME_TX_TYPE, &CHIOView::OnIndexChangedVideoTXType)
	
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CHIOView::OnSelectAll)
	ON_COMMAND(ID_EDIT_COPY, &CHIOView::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CHIOView::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CHIOView::OnEditPaste)
	ON_BN_CLICKED(IDC_ENABLE_DRAW_CORROLATIONS, &CHIOView::OnBnClickedEnableDrawCorrolations)
	ON_BN_CLICKED(IDC_AVISTART, &CHIOView::OnBnClickedAVIStart)
	ON_BN_CLICKED(IDC_AVISTOP, &CHIOView::OnBnClickedAVIStop)
	ON_BN_CLICKED(IDC_ENABLE_AVI_WRITE_AS_IMAGE, &CHIOView::OnBnClickedEnableAviWriteAsImage)
	ON_CBN_SELCHANGE(IDC_COMBOBOX_AVI_COMPRESSION, &CHIOView::OnSelchangeCboxAviCompression)
	ON_BN_CLICKED(IDC_BTN_SHUTTER, &CHIOView::OnClickedShutter)
	ON_COMMAND(ID_HELIOS_SETAVIFILENAME, &CHIOView::OnMenuSetAVIFilename)
	ON_COMMAND(ID_VIEW_REFRESHDDRAW, &CHIOView::OnViewRefreshddraw)
END_MESSAGE_MAP()

// CHIOView construction/destruction

CHIOView::CHIOView() : CFormView(CHIOView::IDD)	
, m_text_terminal(_T(""))
, m_checkbox_draw_corrolations(FALSE)
, m_check_aviasimages(FALSE)
{}

CHIOView::~CHIOView(){}

void CHIOView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(	pDX, IDC_VIDEO_DISPLAY,					m_BitmapViewer);
	DDX_Control(	pDX, IDC_HIO_FORM_CMBO_FRAME_TX_TYPE,	m_VideoTXType);	
	DDX_Text(		pDX, IDC_TEXT_TERMINAL,					m_text_terminal);
	DDX_Control(	pDX, IDC_TEXT_TERMINAL,					m_text_box);
	DDX_Check(		pDX, IDC_ENABLE_DRAW_CORROLATIONS,		m_checkbox_draw_corrolations);
	DDX_Check(		pDX, IDC_ENABLE_AVI_WRITE_AS_IMAGE,		m_check_aviasimages);
	DDX_Control(	pDX, IDC_COMBOBOX_AVI_COMPRESSION,		m_CBoxAVICompression);
}

BOOL CHIOView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CFormView::PreCreateWindow(cs);
}

void CHIOView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// Maybe here?
	m_pdlgScroll = new RegistersDialog(this);
	CRect rc;
	GetDlgItem(IDC_REG_PLACEHOLDER)->GetWindowRect(rc);
	ScreenToClient(&rc);
	m_pdlgScroll->MoveWindow(rc);

	// TODO: Add extra initialization here


	// Add video transmission types to the combo box
	m_VideoTXType.AddString("00: RGB565 ");	
	m_VideoTXType.AddString("01: Grayscale8");
	m_VideoTXType.AddString("02: Rank 6-bit");
	m_VideoTXType.AddString("03: Feature16");
	m_VideoTXType.AddString("04: Color Segmented");
	m_VideoTXType.AddString("05: Feature16 List");
	m_VideoTXType.AddString("06: CCStreak0 List (Black)");
	m_VideoTXType.AddString("07: CCStreak1 List (Red)");
	m_VideoTXType.AddString("08: CCStreak2 List (Yellow)");
	m_VideoTXType.AddString("09: CCStreak3 List (Green)");
	m_VideoTXType.AddString("10: CCStreak4 List (Cyan)");
	m_VideoTXType.AddString("11: CCStreak5 List (Blue)");
	m_VideoTXType.AddString("12: CCStreak6 List (Purple)");
	m_VideoTXType.AddString("13: CCStreak7 List (White)");
	m_VideoTXType.SetCurSel(0);

	m_CBoxAVICompression.AddString("0: Compression:");
	m_CBoxAVICompression.AddString("1: Menu Box");
	m_CBoxAVICompression.AddString("2: RAW");
	m_CBoxAVICompression.AddString("3: IV50");
	m_CBoxAVICompression.AddString("4: MSVC");
	m_CBoxAVICompression.SetCurSel(0);


	// Add register names to the register list
    CRect rect(10,10, 20,20);   // size of this rect doesn't matter
    CPoint point(0,22);        // offset between rectangles
	
	fstream fin;
	fin.open("Registers.txt");
	if(fin.fail())
	{
		printf("ifstream failed.\r\n");		

		fin.close();
		fin.clear();
		fin.open(string("..\\").append("Registers.txt").c_str());
		if(fin.fail())
			printf("ifstream failed again.\r\n");
	}
	string str;

	int index_all   = 0;
	int index_char  = 0;
	int index_int   = 0;
	int index_uint  = 0;
	int index_hex   = 0;
	int index_float = 0;
	while(getline(fin,str)) 
	{
		switch(str[0]){
		case 'C':	m_RegisterRowDialogs_char.push_back(new RegisterRowDialog);
					m_RegisterRowDialogs_char.back()->CRHCreateGenericChildDialog(m_pdlgScroll, &rect, index_all, NULL);			
					m_RegisterRowDialogs_char.back()->SetName(CString(str.c_str()),REG_TYPE_CHAR,index_char++);
					m_RegisterRowDialogs_char.back()->SetValue("0");
					break;

		case 'I':	m_RegisterRowDialogs_int.push_back(new RegisterRowDialog);
					m_RegisterRowDialogs_int.back()->CRHCreateGenericChildDialog(m_pdlgScroll, &rect, index_all, NULL);			
					m_RegisterRowDialogs_int.back()->SetName(CString(str.c_str()),REG_TYPE_INT,index_int++);
					m_RegisterRowDialogs_int.back()->SetValue("0");		
					break;

		case 'U':	m_RegisterRowDialogs_uint.push_back(new RegisterRowDialog);
					m_RegisterRowDialogs_uint.back()->CRHCreateGenericChildDialog(m_pdlgScroll, &rect, index_all, NULL);			
					m_RegisterRowDialogs_uint.back()->SetName(CString(str.c_str()),REG_TYPE_UINT,index_uint++);
					m_RegisterRowDialogs_uint.back()->SetValue("0");
					break;

		case 'X':	m_RegisterRowDialogs_hex.push_back(new RegisterRowDialog);
					m_RegisterRowDialogs_hex.back()->CRHCreateGenericChildDialog(m_pdlgScroll, &rect, index_all, NULL);			
					m_RegisterRowDialogs_hex.back()->SetName(CString(str.c_str()),REG_TYPE_HEX,index_hex++);
					m_RegisterRowDialogs_hex.back()->SetValue("0x00000000");
					break;

		case 'F':	m_RegisterRowDialogs_float.push_back(new RegisterRowDialog);
					m_RegisterRowDialogs_float.back()->CRHCreateGenericChildDialog(m_pdlgScroll, &rect, index_all, NULL);			
					m_RegisterRowDialogs_float.back()->SetName(CString(str.c_str()),REG_TYPE_FLOAT,index_float++);
					m_RegisterRowDialogs_float.back()->SetValue("0.0");
					break;

		default:	printf("Helios::ReadInRegisterNames() Error. Register name in Registers.txt does not start with C, I, U, X, or F"); break;
		}   
		index_all++;
		rect += point;
	}	    

	// Now set a 250 ms timer to update the registers
	::SetTimer( GetSafeHwnd(),			// handle to main window 
			    IDT_UPDATE_REGISTERS,   // timer identifier 
				200,                    // 250 milli-second interval 
				NULL);					// timer callback (handled through windows messaging WM_TIMER)	
}


// CHIOView diagnostics

#ifdef _DEBUG
void CHIOView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHIOView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CHIODoc* CHIOView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHIODoc)));
	return (CHIODoc*)m_pDocument;
}
#endif //_DEBUG


// CHIOView message handlers



void CHIOView::OnTimer(UINT_PTR nIDEvent)
{
	//printf("CHIOView::OnTimer()\r\n");

	if(nIDEvent == IDT_UPDATE_REGISTERS)	
	{	
		CString str;
		
		int iend = (int) m_RegisterRowDialogs_char.size();
		for(int i = 0 ; i < iend ; i++)
		{
			str.Format("%u",HGetRegisterChar(i));
			m_RegisterRowDialogs_char[i]->SetValue(str);
		}
		

		iend = (int) m_RegisterRowDialogs_int.size();
		for(int i = 0 ; i < iend ; i++)
		{
			str.Format("%i",HGetRegisterInt(i));
			m_RegisterRowDialogs_int[i]->SetValue(str);
		}


		iend = (int) m_RegisterRowDialogs_uint.size();
		for(int i = 0 ; i < iend ; i++)
		{
			str.Format("%u",HGetRegisterUInt(i));
			m_RegisterRowDialogs_uint[i]->SetValue(str);
		}
		

		iend = (int) m_RegisterRowDialogs_hex.size();
		for(int i = 0 ; i < iend ; i++)
		{
			str.Format("0x%08X",HGetRegisterHex(i));
			m_RegisterRowDialogs_hex[i]->SetValue(str);
		}
		

		iend = (int) m_RegisterRowDialogs_float.size();
		for(int i = 0 ; i < iend ; i++)
		{
			str.Format("%f",HGetRegisterFloat(i));
			m_RegisterRowDialogs_float[i]->SetValue(str);
		}
	

		if(HGetTextSize() > 0)
		{
			CString str1("");
			while(HGetTextSize() > 0)
			{				
				char buf[256];
				HGetText(buf,256);
				buf[255] = '\0'; // ensure that the string is null terminated
				
				CString str2(buf);
				str2.Replace("\n","\r\n");	// convert from unix to windows new lines		
				str1.Append(str2);
			}	
			m_text_terminal.Append(str1);	
			UpdateData(false);							
		}
	}	
	
	CFormView::OnTimer(nIDEvent);
}

void CHIOView::OnClickedSystemStart()
{
	HEnableRunning(1);
}


void CHIOView::OnClickedSystemStop()
{
	HEnableRunning(0);
}



void CHIOView::OnClickedVideoON()
{
	HEnableVideoTransmission(1);
}


void CHIOView::OnClickedVideoOFF()
{
	HEnableVideoTransmission(0);
}


void CHIOView::OnClickedStateON()
{
	HEnableStateTransmission(1);
}


void CHIOView::OnClickedStateOFF()
{
	HEnableStateTransmission(0);	
}

void CHIOView::OnIndexChangedVideoTXType()
{
	//printf("CHIOView::OnIndexChangedVideoTXType() %d\r\n",m_VideoTXType.GetCurSel());
	HSetVideoTransmissionType(m_VideoTXType.GetCurSel());
}

void CHIOView::OnSelectAll()
{
	m_text_box.SetSel(0xFFFF0000);
}



void CHIOView::OnEditCopy()
{
	m_text_box.Copy();
}

void CHIOView::OnEditCut()
{
	m_text_box.Cut();
}

void CHIOView::OnEditPaste()
{
	m_text_box.Paste();
}

void CHIOView::OnBnClickedEnableDrawCorrolations()
{
	UpdateData(true);	

	if(m_checkbox_draw_corrolations)HEnableGUIProcessingOption(HGUI_PROCESSING_DISPLAY_CORRELATIONS, 1);
	else							HEnableGUIProcessingOption(HGUI_PROCESSING_DISPLAY_CORRELATIONS, 0);
}

void CHIOView::OnBnClickedAVIStart()
{
	HAVIStart();
}

void CHIOView::OnBnClickedAVIStop()
{
	HAVIStop();
}

void CHIOView::OnBnClickedEnableAviWriteAsImage()
{
	UpdateData(true);	
	if(m_check_aviasimages)	HAVIWriteAsImages(1);		
	else					HAVIWriteAsImages(0);	
}

void CHIOView::OnSelchangeCboxAviCompression()
{
	switch(m_CBoxAVICompression.GetCurSel()){
	default: 
	case 0:												break; // Title "Compression"
	case 1: HAVISetFormat(-1);							break; // Menu box
	case 2: HAVISetFormat(CV_FOURCC(0,0,0,0));			break; // RAW
	case 3: HAVISetFormat(CV_FOURCC('I','V','5','0'));	break; // IV50
	case 4: HAVISetFormat(CV_FOURCC('M','S','V','C'));  break; // MSVC
	};
}

void CHIOView::OnClickedShutter()
{
	HShutter();
}

void CHIOView::OnMenuSetAVIFilename()
{
	CFileDialog dlg(false, "avi", "video.avi", 4|2, "Audio-Video-Interlaced (*.avi)|*.avi | All Files (*.*)|*.*||");
	
	int result = (int) dlg.DoModal();
	if( result == IDOK )
	{		
		POSITION psn = dlg.GetStartPosition();
		CString cstr = dlg.GetNextPathName(psn);
		char* str = cstr.GetBuffer(256);		

		HAVISetFilename(str);	
	}
}

void CHIOView::OnViewRefreshddraw()
{
	printf("CHIOView::OnViewRefreshddraw()\r\n");
	
	if(HDDValid())
	{
		printf("DDraw surface is still valid. Destroying...\r\n");
		HDDDestroy();
	}

	HDDInit(m_BitmapViewer.GetSafeHwnd(), 640, 480);	
}
