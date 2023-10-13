// editView_Bottom.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "editview_bottom.h"
#include "editframenew.h"
#include "resource.h"
#include "inputcommentdlg.h"
#include "inputLineNumdlg.h"
#include "editview_top_RichEdit.h"

#include "optionst.h"
#include "DlgAddToStory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int g_nWordCountType;
#define 	LEFT_INDENT 			4		// indent in window
#define 	RIGHT_INDENT 			2
#define 	TOP_INDENT  			4     
#define 	BOTTOM_INDENT 			2
#define		MENUPOS_TEXTSTYLE	4
//UINT	u_FindMsg = 0;
UINT u_FindMsg;

/////////////////////////////////////////////////////////////////////////////
// editView_Bottom

IMPLEMENT_DYNCREATE(editView_Bottom, CEditView)
static UINT WM_FINDREPLACE = ::RegisterWindowMessage( FINDMSGSTRING );

editView_Bottom::editView_Bottom()
{
	m_pFontSet = NULL;
	m_pFindDlg = NULL;
	pDlg_Test = NULL;
	m_pStatusBar = NULL;
	m_pDoc = NULL;
	m_pPopupMenu = NULL;
	m_b_on_paste = false;
	m_b_disable_modify = false;
	m_bShowComment = FALSE;
	m_bFontCreated = FALSE;
	m_nStyleNum = 0;

	m_bOpenFindDialog = false;
	m_bUpdateUpper == FALSE;
	m_bHKWord = FALSE;
	m_url.bExtra = FALSE;
	m_autoSaveDuration=0;
}

editView_Bottom::~editView_Bottom()
{

	if (m_bCreateFontSet)
	{
		m_bCreateFontSet = FALSE;
		delete m_pFontSet;
	}

/*	if (m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
*/	
	if (m_pPopupMenu)
	{
		m_pPopupMenu->DestroyMenu();
		delete m_pPopupMenu;
		m_pPopupMenu = NULL;
	}

	if (m_pFindDlg)
	{
		m_pFindDlg->EndDialog(IDCANCEL);
		delete m_pFindDlg;
		m_pFindDlg = NULL;
	}
	
	if (pDlg_Test && m_bOpenFindDialog)
	{
		pDlg_Test->EndDialog(IDCANCEL);
		delete pDlg_Test;
		pDlg_Test = NULL;
	}

	ReleaseStatusBar();

}


BEGIN_MESSAGE_MAP(editView_Bottom, CEditView)
	//{{AFX_MSG_MAP(editView_Bottom)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_COPY, OnCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_OPTION_EDIT, OnUpdateOptionEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
//	ON_UPDATE_COMMAND_UI(ID_DO_COMMIT, OnUpdateDoCommit)
//	ON_UPDATE_COMMAND_UI(ID_DO_MODIFY, OnUpdateDoModify)
	ON_UPDATE_COMMAND_UI(ID_EDIT_READONLY, OnUpdateEditReadOnly)
	ON_COMMAND(ID_EDIT_READONLY, OnEditReadOnly)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SHOWCHARCODE, OnUpdateEditShowcharcode)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_EDIT_SHOWCHARCODE, OnEditShowcharcode)
	ON_COMMAND(ID_EDIT_ADDCOMMENT, OnEditAddcomment)
	ON_COMMAND(ID_EDIT_SHOWCOMMENT, OnEditShowcomment)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_DELETECOMMENT, OnUpdateCommand)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_ADDCOMMENT, OnUpdateCommand)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_SHOWCOMMENT, OnUpdateCommand)
	ON_COMMAND(ID_EDIT_DELETECOMMENT, OnDeletecomment)
	ON_COMMAND(ID_EDIT_GOTOLINE, OnEditGotoline)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_SELALL, OnEditSelAll)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateFileReplace)
	ON_COMMAND(ID_TS_STYLE1, OnTsStyle1)
	ON_COMMAND(ID_TS_STYLE10, OnTsStyle10)
	ON_COMMAND(ID_TS_STYLE11, OnTsStyle11)
	ON_COMMAND(ID_TS_STYLE12, OnTsStyle12)
	ON_COMMAND(ID_TS_STYLE13, OnTsStyle13)
	ON_COMMAND(ID_TS_STYLE14, OnTsStyle14)
	ON_COMMAND(ID_TS_STYLE15, OnTsStyle15)
	ON_COMMAND(ID_TS_STYLE2, OnTsStyle2)
	ON_COMMAND(ID_TS_STYLE3, OnTsStyle3)
	ON_COMMAND(ID_TS_STYLE4, OnTsStyle4)
	ON_COMMAND(ID_TS_STYLE5, OnTsStyle5)
	ON_COMMAND(ID_TS_STYLE6, OnTsStyle6)
	ON_COMMAND(ID_TS_STYLE7, OnTsStyle7)
	ON_COMMAND(ID_TS_STYLE8, OnTsStyle8)
	ON_COMMAND(ID_TS_STYLE9, OnTsStyle9)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdatePrintView)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW,OnFilePrintPreview)
	ON_COMMAND(ID_OPTION_EDIT, OnOptionEdit)
	ON_COMMAND(ID_OPTION_CHANGE, OnOptionChange)
	ON_WM_CTLCOLOR_REFLECT()
	ON_COMMAND(ID_FILE_SAVE,OnFileSave)
	//ON_COMMAND(ID_COMMENT_VIEW,OnChangeUpperView)
	ON_COMMAND(ID_SWITCH_MODE, OnChangeUpperView)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_MODE, OnUpdateSwitchMode)
	ON_WM_TIMER()
//	ON_COMMAND(ID_FILE_SAVE_AS,OnFileSaveAs)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_UP,ID_DEL_MARK,OnInsertWords)
//	ON_REGISTERED_MESSAGE(u_FindMsg, OnFindMsg)
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE ,OnFindReplace)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_ADD_STORY, OnAddStory)
	ON_UPDATE_COMMAND_UI(ID_ADD_STORY, OnUpdateAddStory)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// editView_Bottom drawing

void editView_Bottom::OnDraw(CDC* pDC)
{
/*	int i_check_width;
	pDC->GetCharWidth(L'口',L'口', &i_check_width); 
	if (i_check_width <= 5 ) i_check_width = 6;


		if (IsSelectedChar(i))
		{
			pDC->FillSolidRect(i_x_shift, i_Line_Number * i_check_width+5, i_check_width-2, i_check_width-5, RGB(0, 0, 0));
			pDC->SetTextColor(RGB(255, 255, 255));
		}
		
		pDC->TextOut(i_x_shift, i_Line_Number * (i_check_width+5) , str_temp_convert); 
		//			COLORREF rgbSave = pDC->GetBkColor();
*/

}
/////////////////////////////////////////////////////////////////////////////
// editView_Bottom diagnostics

#ifdef _DEBUG
void editView_Bottom::AssertValid() const
{
	CView::AssertValid();
}

void editView_Bottom::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// editView_Bottom message handlers
#define		SB_INDICATORS_NEED	3
#define		SB_STATUS			0
#define		SB_WORDS			1
#define		SB_LINES			2
#define		SB_FILESTATUS		3

editDoc* editView_Bottom::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(editDoc)));
	return (editDoc*)m_pDocument;
}

int editView_Bottom::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetEditCtrl().SetLimitText(-1);
	
	// TODO: Add your specialized creation code here
	if (!m_pFontSet)	// Create Font Set
	{
		m_bCreateFontSet = TRUE;
		m_pFontSet = new CFontSet(this);
	}


	// Get Status Bar (Class Name has substring "statusbar")
	CWnd *pMainWnd, *pChildWnd;
//	char szClass[TMPSTR_SIZE];
	wchar_t szClass[TMPSTR_SIZE];
	pMainWnd = AfxGetMainWnd();

  
	pChildWnd = pMainWnd->GetWindow(GW_CHILD);
	while (pChildWnd)
	{
		GetClassName(pChildWnd->GetSafeHwnd(), (LPWSTR)szClass, sizeof(szClass));
//		strlwr(szClass);
		_wcslwr(szClass);
//		if (strstr(szClass, "statusbar"))
		if (wcsstr(szClass, _T("statusbar")))
		{
			m_pStatusBar = (CStatusBar*)pChildWnd;
			break;
		}
		pChildWnd = pChildWnd->GetNextWindow();
	}
	if (m_pStatusBar)
	{	// Check if reserved indicators are enough
		int i;
		for (i=0; i < SB_INDICATORS_NEED; i ++)
		{
			if (m_pStatusBar->GetItemID(i+1) != ID_SEPARATOR)
				break;
		}
		if (i < SB_INDICATORS_NEED)
		{
			m_pStatusBar = NULL;
			TRACE(L"Error: No reserved indicators in main program\n");
		}
	}

	// Popup Menu
	m_pPopupMenu = new CMenu;
	m_pPopupMenu->LoadMenu(IDR_POPUPMENU);

	// Append text style menu
	CWinApp* pApp = AfxGetApp();
	CMenu* pSubMenu = (m_pPopupMenu->GetSubMenu(0))->GetSubMenu(MENUPOS_TEXTSTYLE);
	// Remove the tempory item
	pSubMenu->RemoveMenu(0, MF_BYPOSITION); 
	int i, nStyleNum;
	wchar_t szSection[TMPSTR_SIZE];
	CString str;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	nStyleNum = GetPrivateProfileIntW( L"EditPro" , L"TextStyleNum",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//nStyleNum = pApp->GetProfileInt(L"EditPro", L"TextStyleNum", 1);
	//end 20120927
	for (i=0; i < nStyleNum; i ++)
	{
		swprintf(szSection, L"EditProTextStyle%d", i+1);
		switch ( i )
		{
		case 0:
			str.LoadString ( IDS_STYLE0 );
			break;
		case 1:
			str.LoadString ( IDS_STYLE1 ); 
			break;
		case 2:
			str.LoadString ( IDS_STYLE2 ); 
			break;
		case 3:
			str.LoadString ( IDS_STYLE3 ); 
			break;
		case 4:
			str.LoadString ( IDS_STYLE4 ); 
			break;
		case 5:
			str.LoadString ( IDS_STYLE5 ); 
			break;
		default:
			//edit by Ronald 20120927 for migrating from vc6 to 2010
			wchar_t sz_temp[255];
			GetPrivateProfileStringW( (LPWSTR)szSection , L"StyleName",L"Normal",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
			str = sz_temp;
			//str = pApp->GetProfileString((LPWSTR)szSection, L"StyleName", L"Normal");
			//end 20120927
			break;
		}
		pSubMenu->AppendMenu(MF_STRING, ID_TS_STYLE1+i, str);
	}
/*
	AutoSaveFileToLocal();
	wchar_t sz_temp[255];
	GetPrivateProfileStringW(L"EditPro" , L"AutoSave",L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	m_isAutoSave = sz_temp;
*/
	bFirstType = TRUE;
	m_Tips.Create(CSize(200*3, 20*5));
	bUpperStartedFlag = FALSE;

  return 0;
}

BOOL editView_Bottom::PreTranslateMessage(MSG* pMsg) 
{
/*	if (pMsg->message==WM_KEYUP)
		bKeyDown = FALSE;
	if (pMsg->message==WM_KEYDOWN&&pMsg->lParam!=1)
		bKeyDown = TRUE;*/
	// TODO: Add your specialized code here and/or call the base class
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();	
	//editView_Bottom *pView = this;
	CWnd *hWnd = pFrame->GetLastActivePopup();
	if(pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))
	{
		if(hWnd->IsKindOf(RUNTIME_CLASS(editView_Bottom)))  
			pView->SetFocus();
		/*if(!m_bOpenFindDialog)
			pView->SetFocus();*/
		/*if( pMsg->message==WM_KEYDOWN)
		{
			if(pMsg->wParam==VK_F8)
				this->PostMessage(WM_COMMAND,ID_DO_COMMIT);
			if(pMsg->wParam==_T('E') && (GetKeyState(VK_CONTROL) & 0x8000) != 0)
				this->PostMessage(WM_COMMAND,ID_DO_MODIFY);
		}
		/*COleDateTime dt2;
		dt2 = COleDateTime::GetCurrentTime() - COleDateTimeSpan(0,0,0,m_nSpanSec);
		if (dt2>=dt && m_bUpdateUpper)
		{
			pView->SetTextToUpperView();
			dt=COleDateTime::GetCurrentTime();
			m_bUpdateUpper = FALSE;
		}*/

		CTime iCurrent = CTime::GetCurrentTime();
		if(!m_bOpenFindDialog && (iCurrent-iPastCaretTime).GetSeconds() >=1 && hWnd == pFrame && pView==this)
		{
			iPastCaretTime = iCurrent;
			if(iFontHeight<0)
				iFontHeight = iFontHeight*-1;
			CreateSolidCaret(3,iFontHeight);
			ShowCaret();
			pView->SetFocus();
		}
		return CEditView::PreTranslateMessage(pMsg);
	}
}

void editView_Bottom::UpdateFontSet()
{
	int nStyle;

	if (m_bFontCreated)
	{
		m_bFontCreated = FALSE;

		for(nStyle=0; nStyle < m_nStyleNum; nStyle++)
		{
			m_ppFont[nStyle]->DeleteObject();
			delete m_ppFont[nStyle];
		}
	}
	m_bFontCreated = TRUE;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	m_nStyleNum = GetPrivateProfileIntW( L"EditPro" , L"TextStyleNum",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//	m_nStyleNum = AfxGetApp()->GetProfileInt(L"EditPro", L"TextStyleNum", 1);
	//end 20120927
//	char szSection[TMPSTR_SIZE];
	CString szSection;

	
	CWinApp* pApp = AfxGetApp();
	LOGFONT logfont;

	//edit by Ronald 20120927 for migrating from vc6 to 2010
	int nLogicalPixelX = GetPrivateProfileIntW( L"EditPro" , L"LogicalPixelX",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	int nLogicalPixelY = GetPrivateProfileIntW( L"EditPro" , L"LogicalPixelY",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//int nLogicalPixelX = pApp->GetProfileInt(L"EditPro", L"LogicalPixelX", 1 );
	//int nLogicalPixelY = pApp->GetProfileInt(L"EditPro", L"LogicalPixelY", 1 );
	//end 20120927

//	CClientDC dc(pWnd);
//	CDC* pDC = &dc;
	for(nStyle=0; nStyle < m_nStyleNum; nStyle ++)
	{
		//sprintf((char *)szSection, (const char *)"EditProTextStyle%d", nStyle+1);
		szSection.Format(L"EditProTextStyle%ld", nStyle+1);
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		m_pnLineSpace[nStyle] = GetPrivateProfileIntW( L"szSection" , L"LineSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		m_pnCharSpace[nStyle] = GetPrivateProfileIntW( L"szSection" , L"CharSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//m_pnLineSpace[nStyle] = pApp->GetProfileInt( szSection, L"LineSpace", 1 );
		//m_pnCharSpace[nStyle] = pApp->GetProfileInt( szSection, L"CharSpace", 1 );

		//edit by Ronald 20120927 for migrating from vc6 to 2010
		CString str_temp;
		wchar_t sz_temp[255];
		GetPrivateProfileStringW(szSection , L"Height",L"-15",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//CString str_temp = pApp->GetProfileString( szSection, L"Height", L"-15" );;
		logfont.lfHeight = _wtol(str_temp);

		GetPrivateProfileStringW(szSection , L"Width",L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Width", L"0" );
		logfont.lfWidth  = _wtol(str_temp);

		GetPrivateProfileStringW(szSection , L"Escapement",L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Escapement", L"0" );
		logfont.lfEscapement = _wtol(str_temp);

		GetPrivateProfileStringW(szSection , L"Orientation",L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Orientation", L"0" );
		logfont.lfOrientation = _wtol(str_temp);

		GetPrivateProfileStringW(szSection , L"Weight",L"400",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Weight", L"400" );
		logfont.lfWeight = _wtol(str_temp);

		logfont.lfItalic =  GetPrivateProfileIntW(szSection , L"Italic",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Italic", 0 );
		logfont.lfUnderline =  GetPrivateProfileIntW(szSection , L"Underline",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Underline", 0 );
		logfont.lfStrikeOut =  GetPrivateProfileIntW(szSection , L"StrikeOut",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"StrikeOut", 0 );

		logfont.lfCharSet = GetPrivateProfileIntW(szSection , L"CharSet",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"CharSet", 0 );
		logfont.lfOutPrecision = GetPrivateProfileIntW(szSection , L"OutPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"OutPrecision", 0 );
		logfont.lfClipPrecision = GetPrivateProfileIntW( szSection , L"ClipPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"ClipPrecision", 0 );

		logfont.lfQuality = GetPrivateProfileIntW( szSection , L"Quality",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Quality", 0 );
		logfont.lfPitchAndFamily = GetPrivateProfileIntW( szSection , L"PitchAndFamily",49,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"PitchAndFamily", 49 );

		GetPrivateProfileStringW(szSection , L"FaceName",L"Courier New",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString(szSection, L"FaceName", L"Courier New");
		wcscpy(logfont.lfFaceName, str_temp);
		WritePrivateProfileStringW(L"WordCheck", L"FaceName", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
		//end 20120927

		// scale log size
		/*logfont.lfHeight = MulDiv(logfont.lfHeight,
			pDC->GetDeviceCaps(LOGPIXELSY),
			nLogicalPixelY);
		logfont.lfWidth = MulDiv(logfont.lfWidth,
			pDC->GetDeviceCaps(LOGPIXELSX),
			nLogicalPixelX);
		
		m_pnLineSpace[nStyle] = MulDiv(m_pnLineSpace[nStyle],
			pDC->GetDeviceCaps(LOGPIXELSY),
			nLogicalPixelY);
		m_pnCharSpace[nStyle] = MulDiv(m_pnCharSpace[nStyle],
			pDC->GetDeviceCaps(LOGPIXELSX),
			nLogicalPixelX);*/
		
		m_ppFont[nStyle] = new CFont;
 		m_ppFont[nStyle]->CreateFontIndirect(&logfont);
	}
	m_ppFont[0]->GetLogFont(&logfont);
	iFontHeight = logfont.lfHeight;
		
/*	// Get Base Char Size and Max Char Size
	m_nMaxCharHeight = m_nMaxCharWidth = 0;
	for(nStyle=0; nStyle < m_nStyleNum; nStyle ++)
	{
		SetFont(&dc, nStyle+1);
		if (nStyle == 0)
			m_nBaseCharWidth = m_nChineseWidth;
		if (m_nChineseWidth > m_nMaxCharWidth)
			m_nMaxCharWidth = m_nChineseWidth;
		if (m_nCharExtHeight > m_nMaxCharHeight)
			m_nMaxCharHeight = m_nCharExtHeight;
//		RestoreFont();
	}
	m_nSetStyle = m_nCalcStyle = 0;
*/
}


void editView_Bottom::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	m_pDoc = GetDocument();
	m_nSpanSec = GetPrivateProfileIntW( L"EditPro" , L"SpanSec",5,((CWinApp*)AfxGetApp())->m_pszProfileName);
	CString temp;
	temp.Format(L"%d",m_nSpanSec);
	WritePrivateProfileStringW(L"EditPro", L"SpanSec", temp, ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//dt = COleDateTime::GetCurrentTime() - COleDateTimeSpan(0,0,0,m_nSpanSec);
	CFont* pFont = new CFont;
	LOGFONT lf;	
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = -15;
	lf.lfWeight = 0; 
	lf.lfCharSet = ANSI_CHARSET; 
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lf.lfQuality = DEFAULT_QUALITY; 
	lf.lfPitchAndFamily = 49; 
//	lstrcpy(lf.lfFaceName, L"新細明體"); 
	lstrcpy(lf.lfFaceName, L"Arial Unicode MS"); 
	pFont->CreateFontIndirect(&lf);

//	SetFont(pFont);

//	CFont* m_Font = new CFont;
  //  LOGFONT lf;
//	memset(&lf, 0, sizeof(lf));
  //  GetEditCtrl().GetFont()->GetLogFont(&lf);
//	lf.lfHeight = -13;
  //  m_Font->CreateFontIndirect(&lf);

	UpdateFontSet();
//	GetEditCtrl().SetFont(pFont);
	GetEditCtrl().SetFont(m_ppFont[0]);

if (m_b_allow_edit)
	GetEditCtrl().SetReadOnly(TRUE);
else
	GetEditCtrl().SetReadOnly(FALSE);

InitStatusBar();
//GetEditCtrl(). 

//	OnOptionChange();
}

void editView_Bottom::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{ 
	if (!m_b_allow_edit) return;
	if (GetKeyState(MK_LBUTTON)&0x8000) return;

	int i_Select_Text_Len = 1;

	BOOL bShift=GetKeyState(VK_SHIFT)&0x8000;
	BOOL bCtrl=GetKeyState(VK_CONTROL)&0x8000;	

	int   nStartChar,   nEndChar;  

	if (nChar==VK_DELETE && bCtrl && !bShift)
	{
		BYTE keyState[256];
		BOOL bNumLock =FALSE;
		GetKeyboardState((LPBYTE)&keyState);
		if( keyState[VK_NUMLOCK] & 1 )
		{
			keybd_event( VK_NUMLOCK,0, 0, 0 );
			keybd_event( VK_NUMLOCK,0, KEYEVENTF_KEYUP, 0);
			bNumLock = TRUE;
		}
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_DELETE,0,KEYEVENTF_KEYUP,0);
		keybd_event(VK_SHIFT,0,0,0);
		keybd_event(VK_END,0,0,0);
		keybd_event(VK_END,0,KEYEVENTF_KEYUP,0);
		keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);
		keybd_event(VK_DELETE,0,0,0);
		keybd_event(VK_DELETE,0,KEYEVENTF_KEYUP,0);
		keybd_event(VK_CONTROL,0, 0, 0 );
		if( bNumLock )
		{
			keybd_event( VK_NUMLOCK,0, 0, 0 );
			keybd_event( VK_NUMLOCK,0, KEYEVENTF_KEYUP, 0);
		}
		return;
	}
	else if (nChar==VK_DELETE && bCtrl && bShift)
		return;

	GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	
	switch(nChar)
	{
		case 'Y':
			if (bCtrl > 0)
			{
				OnEditRedo();
				return;
			}
			break;

		case 'L':
			if (bCtrl > 0)
			{
				OnEditGotoline();
				return;
			}
			break;

		case 'F':
			if (bCtrl > 0)
			{
				OnEditFind();
				return;
			}
			break;

		case 'H':
			if (bCtrl > 0)
			{
				OnEditReplace();
				return;
			}
			break;

		case 'A':
			if (bCtrl > 0)
			{
				CEdit& edit = GetEditCtrl();
				edit.SetSel(0, -1);
				return;
			}
			break;

		case '\t':
			if (bCtrl > 0)
			{
				return;
			}
			break;

		case 'K':
		case 'J':
		case 'G':
		case 'Q':
		case 'W':
		case 'E':
		case 'R':
		case 'T':
			if (bCtrl > 0)
				return;
			break;
		case VK_BACK:
		{
			if (bCtrl > 0)
			{
				return;
			}
			break;
		}
		
		//// Added by Ronald Saliman on 20120904 (bug fixed item no.1171 on bugzilla)
		//case VK_BACK:
		//{
		//	OnChar(nChar, nRepCnt, nFlags);
		//	break;
		//}
		////End 20120904

		case VK_DELETE:

/*			
			CString strSelectText = L""; 

		  ASSERT((UINT)nEndChar   <=   GetBufferLength());   

		  LPCTSTR   lpszText   =   LockBuffer();   
		  UINT   nLen   =   nEndChar   -   nStartChar;   
		  memcpy(strSelectText.GetBuffer(nLen), lpszText + nStartChar,  nLen   *   sizeof(TCHAR));   

		  wchar_t*  lptemp; 
		  
		  int i_char_count = 0;
		  bool b_newline_detect = false; 	
		  
		  for (int i=0; i < nLen; i++)
		  {
			  lptemp = new wchar_t[1];
			  memset(lptemp , 0, 1);
			  wcsncpy(lptemp, lpszText + nStartChar + i,  1);  
			   CString str_temp(lptemp);

			   if (str_temp[0] == char(13)) 
			   {
					b_newline_detect = true;
			   }

			   if (b_newline_detect && str_temp[0] == char(10))
			   {
				   b_newline_detect = false; 	
				   continue;
			   }

			   i_char_count = i_char_count + 1;
		  }

		i_Select_Text_Len = i_char_count; 
		strSelectText.ReleaseBuffer(nLen);   
		UnlockBuffer(); 
*/
			if(nStartChar != GetBufferLength())
			{
				int pos, nLen;
				CString str_Text = GetActualSelectText(pos, i_Select_Text_Len, nLen);

				bool b_Select_Text = false;
				if (i_Select_Text_Len > 0) b_Select_Text = true;



				UNDOREDOLOG url;
				url.bIsAdd = true;
				url.lSeq = 0;
				int i_temp_len = i_Select_Text_Len;
				if (i_temp_len==0) 
				{
					i_temp_len=1;
				}
	
				bool b_new_line = false;

				if (b_Select_Text)
					url.strRedoUndo = GetTextFromPoint_Len(nStartChar , nLen, b_new_line, false, b_Select_Text) ;
				else
					url.strRedoUndo = GetTextFromPoint_Len(nEndChar , i_temp_len, b_new_line, false, b_Select_Text) ;
				
				i_Select_Text_Len = url.lLen = i_temp_len;

				if (url.strRedoUndo !="\r\n")
					i_temp_len = i_temp_len + url.strRedoUndo.Replace(L"\r",L"\r");

				url.lStartPos = nStartChar;
				if (!b_Select_Text && url.lLen == 1)
					nEndChar = nEndChar + 1;
				else if (!b_Select_Text && url.lLen == 2)
					nEndChar = nEndChar + 2;

				int nTopStart = m_pDoc->GetMarkPosition(GetActualPos(nEndChar-i_temp_len), true);
				int nTopEnd = m_pDoc->GetMarkPosition(GetActualPos(nEndChar), true);
				if(nTopStart == nTopEnd)
					nTopEnd++;
				url.strRedoUndo_Top = m_pDoc->m_strVersion.Mid(nTopStart, nTopEnd - nTopStart);
				url.bExtra = TRUE;
				url.strMark = m_pDoc->m_strVersion_Mark;
				url.strComment = m_pDoc->m_strVersion_Comment;
				url.strStyle = m_pDoc->m_strVersion_Style;
				m_pDoc->m_arrUndo.Add(url);
				// clear redo list
				m_pDoc->m_arrRedo.RemoveAll(); 
				//i_Select_Text_Len=nTopEnd-nTopStart;
			}
			break;
	}
	
	if (i_Select_Text_Len == 0)
		i_Select_Text_Len = 1;

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch(nChar)
	{
		case VK_DELETE:
			{
				OnEditDelete(i_Select_Text_Len);
			}

			break;
		
		default:
			{

				if (m_pDoc)
				DisplayDocInfo();
			return;
			}
	}
	
	if (m_pDoc)
		DisplayDocInfo();

	return;

}

#define		INPUT_BUFFER_SIZE		100
void editView_Bottom::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KillTimer(1002);KillTimer(1003);
	SetTimer(1003,0,0);
	if (!m_b_allow_edit) return;
	/*if (GetKeyState(MK_LBUTTON)&0x8000)
	{
		if(bKeyDown==TRUE)
			return;
		wchar_t data = nChar;
		INPUT input[2];
		memset(input, 0, 2 * sizeof(INPUT));
 
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = 0;
		input[0].ki.wScan = data;
		input[0].ki.dwFlags = KEYEVENTF_UNICODE;
 
		input[1].type = INPUT_KEYBOARD;
		input[1].ki.wVk = 0;
		input[1].ki.wScan = data;
		input[1].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
 
		SendInput(2, input, sizeof(INPUT));
		return;
	}*/

	BOOL bCtrl=GetKeyState(VK_CONTROL)&0x8000;	
	
	switch(nChar)
	{
		case 25:
		case 1:
		case 6:
		case 7:
		case 10:
		case 11:
		case 12:
		case 17:
		case 23:
		case 5:
		case 18:
		case 20:
		//case 8: //// Comment by Ronald Saliman on 20120904
		case 8: if(!bCtrl) break;
		case 27:
		case 0:
		case 2:
		case 3:
		case 4:
		case 19:
		case 22:
		case 24:
		case 14:
		case 15:
		case 16:
		case 21:
		case 26:
		case 28:
		case 29:
		case 30:
		case 31:
		case 127:

			return;

		break;
	}

	  CString strSelectText = L""; 
  	  int   nStartChar,   nEndChar;   
	  GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	  ASSERT((UINT)nEndChar   <=   GetBufferLength());   
	
/*	
	int i_Select_Text_Len = 0;

	  CString strSelectText = L""; 

  	  int   nStartChar,   nEndChar;   

	  GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	  
	  ASSERT((UINT)nEndChar   <=   GetBufferLength());   

	  LPCTSTR   lpszText   =   LockBuffer();   
	  UINT   nLen   =   nEndChar   -   nStartChar;   
	  memcpy(strSelectText.GetBuffer(nLen), lpszText + nStartChar,  nLen   *   sizeof(TCHAR));   

	  wchar_t*  lptemp; 
	  
	  int i_char_count = 0;
	  bool b_newline_detect = false; 	
	  
	  for (int i=0; i < nLen; i++)
	  {
		  lptemp = new wchar_t[1];
		  memset(lptemp , 0, 1);
		  wcsncpy(lptemp, lpszText + nStartChar + i,  1);  
		   CString str_temp(lptemp);

		   if (str_temp[0] == char(13)) 
		   {
				b_newline_detect = true;
		   }

		   if (b_newline_detect && str_temp[0] == char(10))
		   {
			   b_newline_detect = false; 	
			   continue;
		   }

		   i_char_count = i_char_count + 1;
	  }
	  
	  i_Select_Text_Len = i_char_count; 
	  strSelectText.ReleaseBuffer(nLen);   
	  UnlockBuffer(); 					
*/					
		int pos, i_Select_Text_Len, nLen;
		strSelectText = GetActualSelectText(pos, i_Select_Text_Len, nLen);

	bool b_Select_Text = false;
	if (i_Select_Text_Len > 0) b_Select_Text = true;


	int nCharIndex = nEndChar;

	if (!(GetKeyState(VK_CONTROL) & 0x8000) || nChar == 8) // backspace and crtl-H handle
	{   

	    UINT uChar;
//	    MSG	 msg;
	    
		uChar = nChar & 0xffff;
		switch(uChar)
		{
		    case VK_BACK:
			{
					if (pos != 0 || i_Select_Text_Len != 0)
					{
						// add an item to undo list
						UNDOREDOLOG url;
						url.bIsAdd = true;
						url.lSeq = 0;
						int i_temp_len = i_Select_Text_Len;
					
						if (i_temp_len==0) i_temp_len = 1;



						bool b_new_line = false;

						if (b_Select_Text)
							url.strRedoUndo = GetTextFromPoint_Len(nStartChar , nLen, b_new_line, true, b_Select_Text) ;
						else
							url.strRedoUndo = GetTextFromPoint_Len(nEndChar-i_temp_len , i_temp_len, b_new_line, true, b_Select_Text) ;
						
						int i_temp_len_back = i_temp_len;

						if (b_new_line)
						{
							CString temp = url.strRedoUndo;
							temp.Replace(L"\r\n",L"\r");
							i_temp_len_back = temp.GetLength();
							if(i_temp_len_back == 1)
								i_temp_len_back = i_temp_len_back +1;
						}

						url.lLen = i_temp_len;
					
						if (b_Select_Text)
							url.lStartPos = nStartChar;
						else 
							url.lStartPos = nEndChar-i_temp_len_back;
						url.strRedoUndo.Replace(L"\r\r",L"\r");

						int nTopStart = m_pDoc->GetMarkPosition(GetActualPos(nEndChar-i_temp_len_back), true);
						int nTopEnd = m_pDoc->GetMarkPosition(GetActualPos(nEndChar), true);
						url.strRedoUndo_Top = m_pDoc->m_strVersion.Mid(nTopStart, nTopEnd - nTopStart);
						url.bExtra = FALSE;
						url.strMark = m_pDoc->m_strVersion_Mark;
						url.strComment = m_pDoc->m_strVersion_Comment;
						url.strStyle = m_pDoc->m_strVersion_Style;
						/*m_pDoc->m_arrUndo.Add(url);
						m_pDoc->m_arrRedo.RemoveAll(); */
						CString strTemp1,strTemp2;
						GetWindowText(strTemp1);		
						CView::OnChar(nChar, nRepCnt, nFlags);
						GetWindowText(strTemp2);
						if (strTemp1==strTemp2)
							return;
						m_pDoc->m_arrUndo.Add(url);
						m_pDoc->m_arrRedo.RemoveAll(); 
						if (i_temp_len_back == 0) i_temp_len_back = 1;
						OnEditDelete(i_temp_len);
					}
					return;
				}

			default:
				break;
		} 
	}

	if(nChar != 13)
	{
		int nCharIndex_real = GetActualPos(nCharIndex-nLen);
		int nTopStart = m_pDoc->GetMarkPosition(GetActualPos(nCharIndex-nLen), true);
		int nTopEnd = m_pDoc->GetMarkPosition(GetActualPos(nCharIndex), true);
		if(i_Select_Text_Len == 0)
			nTopEnd = nTopStart;

		UNDOREDOLOG url;
		url.strRedoUndo_Top = m_pDoc->m_strVersion.Mid(nTopStart, nTopEnd - nTopStart);
		url.strMark = m_pDoc->m_strVersion_Mark;
		url.strComment = m_pDoc->m_strVersion_Comment;
		url.strStyle = m_pDoc->m_strVersion_Style;

		CString str_Temp;
		str_Temp.Format(L"%c", nChar);
		//m_pDoc->UpdateTextVersionPos((CString)(LPWSTR)nChar, nCharIndex_real-i_Select_Text_Len, false);  

		if(bFirstType==TRUE)
		{
			SetTextToUpperView();
			bFirstType=FALSE;
		}
		
		CString strTemp1,strTemp2;
		GetWindowText(strTemp1);
		CView::OnChar(nChar, nRepCnt, nFlags);
		GetWindowText(strTemp2);
		if (strTemp1==strTemp2)
			return;
		if(nChar == 9)
        {
			CString strTemp;;
            int pos1 = GetEditCtrl().GetScrollPos(SB_VERT);
            /*strStart = GetTextFromPoint(nCharIndex-nLen,true);
            strEnd = GetTextFromPoint(nCharIndex,false);
            SetWindowText(strStart+L"\t"+strEnd);*/
			GetWindowText(strTemp);
			SetWindowText(strTemp);
            GetEditCtrl().LockWindowUpdate();
            GetEditCtrl().LineScroll(pos1);
            GetEditCtrl().SetSel(nCharIndex+1-i_Select_Text_Len, nCharIndex+1-i_Select_Text_Len);
            GetEditCtrl().UnlockWindowUpdate();
        }
		
		m_pDoc->DeleteTextVersionPos(nCharIndex_real, i_Select_Text_Len);  
		m_pDoc->UpdateTextVersionPos(str_Temp, nCharIndex_real, false);  
		// add a record to undo List 
		url.bIsAdd = false;
		url.lSeq = 0;
		
		if (m_bHKWord==FALSE)
		{
			char* pszGbt=NULL; 
			wchar_t* wszUnicode=NULL; 
			CString str, str_Converted;
			str=str_Temp;
			int iLen = 0;
			iLen=WideCharToMultiByte (CP_UTF8, 0, str, -1, NULL,0, NULL, NULL) ; 
			pszGbt=new char[iLen+1];
			wszUnicode=new wchar_t[iLen+1];   
			WideCharToMultiByte (CP_UTF8 , 0, str, -1, pszGbt,iLen, NULL, NULL) ;
			MultiByteToWideChar (CP_UTF8, 0, pszGbt, -1, wszUnicode,iLen); 
			str_Converted.Format(L"%s",wszUnicode );
			delete []pszGbt;
			delete []wszUnicode;
			if (str != str_Converted)
			{
				m_bHKWord = TRUE;
				if (b_Select_Text)
				{
					m_url = url;
					m_url.bExtra = true;
					m_url.strRedoUndo_Extra = strSelectText;
				}
				return;
			}
			url.strRedoUndo = str_Temp;
			url.lLen = 1;
			url.lStartPos = nStartChar;
		}

		int i_len = url.strRedoUndo.GetLength(); 
	

		if (b_Select_Text)
		{
			url.bExtra = true;
			url.strRedoUndo_Extra = strSelectText;	
		}
		else
		{
			url.bExtra = false;
			url.strRedoUndo_Extra = "";	
		}

		if (m_bHKWord==TRUE)
		{
			if(m_url.bExtra == true)
			{
				url=m_url;
				m_url.bExtra = FALSE;
			}
			str_Temp.Format(L"%s", m_pDoc->m_strVersion.Mid(nTopStart-1,2));
			//url.strRedoUndo = (CString)(LPWSTR)nChar;
			url.strRedoUndo = str_Temp;
			url.lLen = 2;
			url.lStartPos = nStartChar-1;
		}

		m_pDoc->m_arrUndo.Add(url);
		m_pDoc->m_arrRedo.RemoveAll(); 
		m_bHKWord = FALSE;
	}
	else
		OnInsertSpaceOnFront();

	if (m_pDoc)
		DisplayDocInfo();

	return;
	// 

}

void editView_Bottom::OnInsertWords(UINT nID )
{
	KillTimer(1002);KillTimer(1003);
	SetTimer(1003,0,0);
	if (!m_b_allow_edit) return;
	  CString strSelectText = L""; 

	  int   nStartChar,   nEndChar;   
	  GetEditCtrl().GetSel(nStartChar,  nEndChar);   
	  ASSERT((UINT)nEndChar   <=   GetBufferLength());   

/*
	  int i_Select_Text_Len = 0;

	  CString strSelectText = L""; 
	  int   nStartChar,   nEndChar;   
	  GetEditCtrl().GetSel(nStartChar,  nEndChar);   
 
	  ASSERT((UINT)nEndChar   <=   GetBufferLength());   

	  LPCTSTR   lpszText   =   LockBuffer();   
	  UINT   nLen   =   nEndChar   -   nStartChar;   
	  memcpy(strSelectText.GetBuffer(nLen), lpszText + nStartChar,  nLen   *   sizeof(TCHAR));   

	  wchar_t*  lptemp; 
  
	  int i_char_count = 0;
	  bool b_newline_detect = false; 	
  
	  for (int i=0; i < nLen; i++)
	  {
		  lptemp = new wchar_t[1];
		  memset(lptemp , 0, 1);
		  wcsncpy(lptemp, lpszText + nStartChar + i,  1);  
		   CString str_temp(lptemp);

		   if (str_temp[0] == char(13)) 
		   {
				b_newline_detect = true;
		   }

		   if (b_newline_detect && str_temp[0] == char(10))
		   {
			   b_newline_detect = false; 	
			   continue;
		   }

		   i_char_count = i_char_count + 1;
	  }
  
	  i_Select_Text_Len = i_char_count; 
	  strSelectText.ReleaseBuffer(nLen);   
	  UnlockBuffer(); 
*/
		int pos, i_Select_Text_Len, nLen;
		strSelectText = GetActualSelectText(pos, i_Select_Text_Len, nLen);


	bool b_Select_Text = false;
	if (i_Select_Text_Len > 0) b_Select_Text = true;

	char szInputBuffer[2];
	wchar_t szInputBuffer_w[3];
	switch(nID)
	{
	case ID_UP:
		szInputBuffer[0]=0xa1;
		szInputBuffer[1]=0xf4;
		break;
    case ID_DOWN:
		szInputBuffer[0]=0xa1;
		szInputBuffer[1]=0xf5;
		break;
	case ID_RIGHT:
		szInputBuffer[0]=0xa1;
		szInputBuffer[1]=0xf7;
		break;
	case ID_LEFT:
		szInputBuffer[0]=0xa1;
		szInputBuffer[1]=0xf6;
		break;
	case ID_DOUP:
//		szInputBuffer[0]=0x87;
//		szInputBuffer[1]=0x49;
		szInputBuffer_w[0]=0xE2;
		szInputBuffer_w[1]=0x9C;
		szInputBuffer_w[2]=0x93;
//0xE2 0x9C 0x93		
		break;
	case ID_ZEROC:
		szInputBuffer[0]=0xa2;
		szInputBuffer[1]=0x4a;
		break;
	case ID_ZEROF:
		szInputBuffer[0]=0xa2;
		szInputBuffer[1]=0x4b;
		break;
	case ID_DIVIDE:
		szInputBuffer[0]=0xa1;
		szInputBuffer[1]=0xd2;
		break;
	case ID_MULTIPLY:
		szInputBuffer[0]=0xa1;
		szInputBuffer[1]=0xd1;
		break;
	case ID_ZONE:
		szInputBuffer[0]=0xa1;
		szInputBuffer[1]=0xb3;
		break;
	case ID_DEL_MARK:
		szInputBuffer[0]=0xa2;
		szInputBuffer[1]=0xae;
		break;
	}


	CString strText = L"";
	WORD wTemp = 0x2713;
	if (nID != ID_DOUP)
	{
		WCHAR * szwBuffer = new WCHAR[1];
		MultiByteToWideChar(CP_ACP, 0, szInputBuffer, -1, szwBuffer, 1);
		
		strText = szwBuffer[0];
	}
	else
	{
		szInputBuffer_w[0] = wTemp;
		strText = szInputBuffer_w[0];
	}

    int nCharIndex = nEndChar;
	int nCharIndex_real = GetActualPos(nCharIndex - nLen);
	int nTopStart = m_pDoc->GetMarkPosition(GetActualPos(nCharIndex-nLen), true);
	int nTopEnd = m_pDoc->GetMarkPosition(GetActualPos(nCharIndex), true);
	if(i_Select_Text_Len == 0)
		nTopEnd = nTopStart;

	CString strStart, strEnd;
	
	strStart = GetTextFromPoint(nCharIndex-nLen,true);
	strEnd = GetTextFromPoint(nCharIndex,false);
   
	CEdit& edit = GetEditCtrl();
	int ipos=GetScrollPos(SB_VERT);
	edit.LockWindowUpdate();
	SetWindowText(strStart + strText + strEnd);
	edit.LineScroll(ipos);
	edit.SetSel(nCharIndex-i_Select_Text_Len+1, nCharIndex-i_Select_Text_Len+1);
	edit.UnlockWindowUpdate();

	UNDOREDOLOG url;
	url.strRedoUndo_Top = m_pDoc->m_strVersion.Mid(nTopStart, nTopEnd - nTopStart);
	url.strMark = m_pDoc->m_strVersion_Mark;
	url.strComment = m_pDoc->m_strVersion_Comment;
	url.strStyle = m_pDoc->m_strVersion_Style;

	m_pDoc->DeleteTextVersionPos(nCharIndex_real, i_Select_Text_Len);  
	
	m_pDoc->UpdateTextVersionPos(strText, nCharIndex_real , false);  
	if(bFirstType==TRUE)
	{
		SetTextToUpperView();
		bFirstType=FALSE;
	}

	//CEdit& edit = GetEditCtrl();
	//edit.SetSel(nCharIndex_real+1, nCharIndex_real+1);

	// add a record to undo List 
	url.bIsAdd = false;
	url.lSeq = 0;
	url.strRedoUndo = strText;
	url.lLen = 1;
//	url.lStartPos = nCharIndex;
	url.lStartPos = nStartChar;

	if (b_Select_Text)
	{
		url.bExtra = true;
		url.strRedoUndo_Extra = strSelectText;	
	}
	else
	{
		url.bExtra = false;
		url.strRedoUndo_Extra = "";	
	}	
	m_pDoc->m_arrUndo.Add(url);
	m_pDoc->m_arrRedo.RemoveAll(); 
	m_pDoc->SetModifiedFlag(TRUE);

	if (m_pDoc)
		DisplayDocInfo();

//	SetPrivateBufferPtr();
//	InsertText( (BYTE *)szInputBuffer, 2 );
//	m_pDoc->LockDocBuffer(FALSE);
}

//
//proedure call when user press delete or 'backspace' 
// 
void editView_Bottom::OnEditDelete(int i_select_text_len)
{
	KillTimer(1002);KillTimer(1003);
	SetTimer(1003,0,0);
    int   nStartChar,   nEndChar;   
    GetEditCtrl().GetSel(nStartChar,  nEndChar);   	
	
    int nCharIndex = nEndChar;
	nCharIndex = GetActualPos(nCharIndex);

	CString strStart, strEnd;
	m_pDoc->DeleteTextVersionPos(nCharIndex, i_select_text_len);  
//	m_pDoc->DeleteTextVersionPos(3, i_select_text_len);  

	if(bFirstType==TRUE)
	{
		SetTextToUpperView();
		bFirstType=FALSE;
	}
}



LRESULT editView_Bottom::OnCut(WPARAM wParam,LPARAM lParam)
{
    // TODO: Add your control notification handler code here
	CString strSelectText = L""; 
	int   nStartChar,   nEndChar;   
	GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	ASSERT((UINT)nEndChar   <=   GetBufferLength());   
	LPCTSTR   lpszText   =   LockBuffer();   
	UINT   nLen   =   nEndChar   -   nStartChar;   
	memcpy(strSelectText.GetBuffer(nLen), lpszText + nStartChar,  nLen   *   sizeof(TCHAR));   
	strSelectText.ReleaseBuffer(nLen);

	if(OpenClipboard()) //OpenClipboard(NULL) gives me error
	{
		EmptyClipboard();
		HLOCAL clipbuffer = LocalAlloc(0, nLen   *   4);
		wcscpy((WCHAR*) clipbuffer, strSelectText);


		SetClipboardData(CF_UNICODETEXT, clipbuffer);
		CloseClipboard();
		//free(szMsg); //not sure what 'szMsg' is
		//LocalFree(clipbuffer);
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_DELETE, 0, 0, 0);
		return 1;
	}

	return 1;
}



LRESULT editView_Bottom::OnCopy(WPARAM wParam,LPARAM lParam)
{
    // TODO: Add your control notification handler code here
	CString strSelectText = L""; 
  	int   nStartChar,   nEndChar;   
	GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	ASSERT((UINT)nEndChar   <=   GetBufferLength());   
	LPCTSTR   lpszText   =   LockBuffer();   
	UINT   nLen   =   nEndChar   -   nStartChar;   
	memcpy(strSelectText.GetBuffer(nLen), lpszText + nStartChar,  nLen   *   sizeof(TCHAR));
	strSelectText.ReleaseBuffer(nLen);

	//put a test char
	//wchar_t *test;
	//test = (wchar_t*) malloc(nLen   *  4);
	//wcscpy(test, strSelectText);
	//codes you've given
	if(OpenClipboard()) //OpenClipboard(NULL) gives me error
	{
		EmptyClipboard();
		HLOCAL clipbuffer = LocalAlloc(0, nLen   *   4);
		wcscpy((WCHAR*) clipbuffer, strSelectText);

		SetClipboardData(CF_UNICODETEXT, clipbuffer);
		CloseClipboard();
		//free(szMsg); //not sure what 'szMsg' is
		//LocalFree(clipbuffer);

		return 1;
	}

	return 1;


}

LRESULT editView_Bottom::OnPaste(WPARAM wParam,LPARAM lParam)
{
 	  int i_Select_Text_Len = 0;

	  CString strSelectText = L""; 
	  int   nStartChar,   nEndChar;   

	  GetEditCtrl().GetSel(nStartChar,  nEndChar);   
  
	  ASSERT((UINT)nEndChar   <=   GetBufferLength());   

	  LPCTSTR   lpszText   =   LockBuffer();   
	  UINT   nLen   =   nEndChar   -   nStartChar;   
	  memcpy(strSelectText.GetBuffer(nLen), lpszText + nStartChar,  nLen   *   sizeof(TCHAR));   

	  wchar_t*  lptemp; 
  
	  int i_char_count = 0;
	  bool b_newline_detect = false; 	
  
	  for (int i=0; i < nLen; i++)
	  {
		  lptemp = new wchar_t[10];
		  memset(lptemp , 0, 10);
		  wcsncpy(lptemp, lpszText + nStartChar + i,  1);  
		   CString str_temp(lptemp);

		   if (str_temp[0] == wchar_t(13)) 
		   {
				b_newline_detect = true;
		   }

		   if (b_newline_detect && str_temp[0] == wchar_t(10))
		   {
			   b_newline_detect = false; 	
			   continue;
		   }

		   i_char_count = i_char_count + 1;
	  }
  
	  i_Select_Text_Len = i_char_count;
	  strSelectText.ReleaseBuffer(nLen);   
	  UnlockBuffer(); 

	bool b_Select_Text = false;
	if (i_Select_Text_Len > 0) b_Select_Text = true;
	
	if (m_b_on_paste) 
	{
		m_b_on_paste = false;
		return 1;
	}
	wchar_t* buffer;
	CString strtemp;
	if(OpenClipboard())
	{
		//buffer = (wchar_t*)GetClipboardData(CF_UNICODETEXT);
		HANDLE hClip=::GetClipboardData(CF_UNICODETEXT);
        buffer =(wchar_t *)GlobalLock(hClip);
		CloseClipboard();
	}
	
    CString str_buffer = buffer;
	//str_buffer.Replace(L"\n",L""); 
	str_buffer.Replace(L"\r\n",L"\r"); 
	str_buffer.Replace(L"\n",L"\r"); 

    int nCharIndex = nEndChar;
	int nCharIndex_real = GetActualPos(nCharIndex - nLen);
	int nTopStart = m_pDoc->GetMarkPosition(GetActualPos(nCharIndex-nLen), true);
	int nTopEnd = m_pDoc->GetMarkPosition(GetActualPos(nCharIndex), true);
	if(i_Select_Text_Len == 0)
		nTopEnd = nTopStart;

	CString strStart, strEnd;
	
	strStart = GetTextFromPoint(nCharIndex-nLen,true);

	strEnd = GetTextFromPoint(nCharIndex,false);

	CEdit& edit = GetEditCtrl();
	int pos=GetScrollPos(SB_VERT);
	
	UNDOREDOLOG url;
	url.strRedoUndo_Top = m_pDoc->m_strVersion.Mid(nTopStart, nTopEnd - nTopStart);
	url.strMark = m_pDoc->m_strVersion_Mark;
	url.strComment = m_pDoc->m_strVersion_Comment;
	url.strStyle = m_pDoc->m_strVersion_Style;

	m_pDoc->DeleteTextVersionPos(nCharIndex_real, i_Select_Text_Len);  
	
	m_pDoc->UpdateTextVersionPos(str_buffer, nCharIndex_real , false);  

	
	if(bFirstType==TRUE)
	{
		SetTextToUpperView();
		bFirstType=FALSE;
	}

	CString str_temp = str_buffer;
	str_temp.Replace(L"\r",L"\r\n"); 

	
	edit.LockWindowUpdate();
	SetWindowText(strStart + str_temp + strEnd);
	edit.LineScroll(pos);
	//CEdit& edit = GetEditCtrl();
	int i_pos = nCharIndex-nLen+str_temp.GetLength();
	//	int len;
	//len = edit.GetWindowTextLength();
	edit.SetSel(i_pos, i_pos);
	edit.UnlockWindowUpdate();
	
	url.bIsAdd = false;
	url.lSeq = 0;

	url.strRedoUndo = str_buffer;

//	int i_len = url.strRedoUndo.GetLength(); 
	
	url.lLen = str_buffer.GetLength();
//	url.lStartPos = nCharIndex;
	url.lStartPos = nStartChar;

	if (b_Select_Text)
	{
		url.bExtra = true;
		url.strRedoUndo_Extra = strSelectText;	
	}
	else
	{
		url.bExtra = false;
		url.strRedoUndo_Extra = "";	
	}	
	m_pDoc->m_arrUndo.Add(url);
	m_pDoc->m_arrRedo.RemoveAll(); 
	m_pDoc->SetModifiedFlag(TRUE);

	if (m_pDoc)
		DisplayDocInfo();

    return 1;
}

int editView_Bottom::GetActualPos(int i_Pos)
{
	CString str_Temp;
	this->GetWindowText(str_Temp);
	int i_new_line_count = 0;

	for (int i=0; i < i_Pos; i++)
	{
		if (str_Temp[i] == L'\r\n')
			i_new_line_count = i_new_line_count +1;
	}

	return i_Pos - i_new_line_count;

}

int editView_Bottom::GetActualDeletePosCount(int i_Pos)
{
	CString str_Temp;
	GetWindowText(str_Temp);
	int i_new_line_count = 0;

	for (int i=0; i < i_Pos; i++)
	{
		if (str_Temp[i] == L'\r\n')
			i_new_line_count = i_new_line_count +1;
	}

	return i_new_line_count;

}

CString editView_Bottom::GetTextFromPoint(int i_Pos, bool b_start)
{
	CString strText = L"";
	this->GetWindowText(strText);

	if (b_start)
		strText = strText.Left(i_Pos); 
	else
		strText = strText.Right(strText.GetLength() - i_Pos); 

	return strText;
}

CString editView_Bottom::GetTextFromPoint_Len(int i_Pos, int &i_len, bool &b_new_line, bool b_backspace, bool b_select_text)
{
	CString strText = L"", temp;
	GetWindowText(strText);

	temp = strText;
	strText = strText.Left(i_Pos+ i_len);
	strText = strText.Right(i_len);
	if (!b_select_text)
	{
		CString strtemp;
		if (!b_backspace)
			strtemp = temp.Left(i_Pos+ i_len+1); 
		else
			strtemp = temp.Left(i_Pos+ i_len); 
		strtemp = strtemp.Right(i_len+1);
		char* pszGbt=NULL; 
		//wchar_t* wszUnicode=NULL; 
		CString str = strtemp, str_Converted;
		int iLen = 0;
		iLen=WideCharToMultiByte (CP_UTF8, 0, str, -1, NULL,0, NULL, NULL) ; 
		pszGbt=new char[iLen+1];
		//wszUnicode=new wchar_t[iLen+1];   
		WideCharToMultiByte (CP_UTF8, 0, str, -1, pszGbt,iLen, NULL, NULL) ;
		/*MultiByteToWideChar (CP_UTF8, 0, pszGbt, -1, wszUnicode,iLen); 
		str_Converted.Format(L"%s",wszUnicode );
		if (str != str_Converted)
		{
			if (!b_backspace)
				strText = temp.Left(i_Pos+ i_len+1);
			else
				strText = temp.Left(i_Pos+ i_len);
			strText = strText.Right(i_len+1);
			i_len++;
		}*/
		CString str_Temp_Hex;
		wchar_t szTemp1[10]; 
		_itow(*(pszGbt), szTemp1, 16);
		str_Temp_Hex = szTemp1;
		str_Temp_Hex.Replace(L"ffffff",L"");
		if (str_Temp_Hex == L"f0" || str_Temp_Hex == L"f1" || str_Temp_Hex == L"f2" || str_Temp_Hex == L"f3" || str_Temp_Hex == L"f4")
		{
			if (!b_backspace)
				strText = temp.Left(i_Pos+ i_len+1);
			else
				strText = temp.Left(i_Pos+ i_len);
			strText = strText.Right(i_len+1);
			i_len++;
		}
		delete []pszGbt;
		//delete []wszUnicode;
	}

	if (!b_select_text)
		strText.Replace(L"\r",L"\r\n"); 

	if (b_backspace)
	{
		if (strText.Find(L"\n") >= 0)
		{
			strText.Replace(L"\n",L"\r\n"); 
			b_new_line = true;
		}
	}
	
	return strText;
}

void editView_Bottom::OnEditUndo() 
{
	KillTimer(1002);KillTimer(1003);
	SetTimer(1003,0,0);
	// TODO: Add your command handler code here
	bool b_Item_Found = false;
	UNDOREDOLOG url;
	if (m_pDoc->m_arrUndo.GetSize() > 0)
	{
		b_Item_Found = true;;
		url=m_pDoc->m_arrUndo.GetAt(m_pDoc->m_arrUndo.GetSize()-1);
	}

	if (b_Item_Found)
	{
		if (url.bIsAdd == false)
		{

			CString strStart, strEnd;
			CString strtemp = url.strRedoUndo;
			
			strtemp.Replace(L"\r\n",L"\r"); 
			strtemp.Replace(L"\r",L"\r\n"); 
	
			int i_temp_len = strtemp.GetLength(); 
			
			strStart = GetTextFromPoint(url.lStartPos,true);
			strEnd = GetTextFromPoint(url.lStartPos+i_temp_len,false);
			
			
			if (strEnd.GetLength() > 0)
			{
				if (strEnd[0] == L'\n') 
					strEnd = "\r" + strEnd; 
			}
			CEdit& edit = GetEditCtrl();
			int pos=GetScrollPos(SB_VERT);
			edit.LockWindowUpdate();
			SetWindowText(strStart + strEnd);
			edit.LineScroll(pos);
			//CEdit& edit = GetEditCtrl();
			edit.SetSel(url.lStartPos , url.lStartPos);
			edit.UnlockWindowUpdate();
			
			OnEditDelete(url.lLen);

			// add a record to redo LIST
			UNDOREDOLOG url_redo;
			url_redo.bIsAdd = true;
			url_redo.lSeq = 0;
			url_redo.strRedoUndo = url.strRedoUndo;
			url_redo.lLen = url.lLen;
			url_redo.lStartPos = url.lStartPos;

			url_redo.bExtra = url.bExtra;
			url_redo.strRedoUndo_Extra = url.strRedoUndo_Extra;
			url_redo.strRedoUndo_Top = url.strRedoUndo_Top;
			url_redo.strMark = url.strMark;
			url_redo.strComment = url.strComment;
			url_redo.strStyle = url.strStyle;

			m_pDoc->m_arrRedo.Add(url_redo);	

			if (url.bExtra)
			{
				CString strStart, strEnd;
				strStart = GetTextFromPoint(url.lStartPos,true);
				strEnd = GetTextFromPoint(url.lStartPos,false);
				
				CString strtemp = url.strRedoUndo_Extra;
				strtemp.Replace(L"\r\n",L"\r"); 
				strtemp.Replace(L"\r",L"\r\n"); 
				int i_temp_len = strtemp.GetLength(); 
				
				CEdit& edit = GetEditCtrl();
				int pos=GetScrollPos(SB_VERT);
				edit.LockWindowUpdate();
				SetWindowText(strStart + strtemp + strEnd);
				edit.LineScroll(pos);
				//CEdit& edit = GetEditCtrl();
				edit.SetSel(url.lStartPos + i_temp_len, url.lStartPos + i_temp_len);
				edit.UnlockWindowUpdate();

				CString str_buffer = url.strRedoUndo_Top;
				str_buffer.Replace(L"\n",L""); 
				

				int nCharIndex_real = GetActualPos(url.lStartPos);

				m_pDoc->UpdateTextVersionPos(str_buffer, nCharIndex_real, true);  
				if(bFirstType==TRUE)
				{
					SetTextToUpperView();
					bFirstType=FALSE;
				}


			}
		}
		else
		{

			CString strStart, strEnd;
			strStart = GetTextFromPoint(url.lStartPos,true);
			strEnd = GetTextFromPoint(url.lStartPos,false);
			
			CString strtemp = url.strRedoUndo;
			strtemp.Replace(L"\r\n",L"\r"); 
			strtemp.Replace(L"\r",L"\r\n"); 
			int i_temp_len = strtemp.GetLength(); 
			
			CEdit& edit = GetEditCtrl();
			SetWindowText(strStart + strtemp + strEnd);
			edit.LockWindowUpdate();
			int pos=GetScrollPos(SB_VERT);
			edit.LineScroll(pos);
			//CEdit& edit = GetEditCtrl();
			if (url.bExtra) //delete
				edit.SetSel(url.lStartPos , url.lStartPos);
			else
				edit.SetSel(url.lStartPos+i_temp_len , url.lStartPos+i_temp_len);
			edit.UnlockWindowUpdate();

		    CString str_buffer = url.strRedoUndo_Top;
			str_buffer.Replace(L"\n",L""); 
			

			int nCharIndex_real = GetActualPos(url.lStartPos);

			m_pDoc->UpdateTextVersionPos(str_buffer, nCharIndex_real, true);  
			if(bFirstType==TRUE)
			{
				SetTextToUpperView();
				bFirstType=FALSE;
			}

			// add a record to redo LIST
			UNDOREDOLOG url_redo;
			url_redo.bIsAdd = false;
			url_redo.lSeq = 0;
			url_redo.strRedoUndo = url.strRedoUndo;
			url_redo.lLen = url.lLen;
			url_redo.lStartPos = url.lStartPos;
			url_redo.strRedoUndo_Top = url.strRedoUndo_Top;
			url_redo.strMark = url.strMark;
			url_redo.strComment = url.strComment;
			url_redo.strStyle = url.strStyle;
			url_redo.bExtra = url.bExtra;
			m_pDoc->m_arrRedo.Add(url_redo);	
		}

		// remove the current undo item
		m_pDoc->m_arrUndo.RemoveAt(m_pDoc->m_arrUndo.GetSize()-1);

	
	}

	if (m_pDoc)
		DisplayDocInfo();

}

void editView_Bottom::OnEditRedo() 
{
	KillTimer(1002);KillTimer(1003);
	SetTimer(1003,0,0);
	// TODO: Add your command update UI handler code here
	bool b_Item_Found = false;
	UNDOREDOLOG url;
	if (m_pDoc->m_arrRedo.GetSize() > 0)
	{
		b_Item_Found = true;;
		url=m_pDoc->m_arrRedo.GetAt(m_pDoc->m_arrRedo.GetSize()-1);
	}

	if (b_Item_Found)
	{
		if (url.bIsAdd)
		{

			if (url.bExtra)
			{

				CString strStart, strEnd;
				CString strtemp = url.strRedoUndo_Extra;
				CString strtemp_1 = url.strRedoUndo_Extra;
				strtemp.Replace(L"\r\n",L"\r"); 
				strtemp.Replace(L"\r",L"\r\n"); 

				strtemp_1.Replace(L"\r",L""); 
				
				int i_temp_len = strtemp.GetLength(); 

				strStart = GetTextFromPoint(url.lStartPos,true);
				strEnd = GetTextFromPoint(url.lStartPos+i_temp_len,false);
				
				CEdit& edit = GetEditCtrl();
				int pos=GetScrollPos(SB_VERT);
				edit.LockWindowUpdate();
				SetWindowText(strStart + strEnd);
				edit.LineScroll(pos);
				//CEdit& edit = GetEditCtrl();
				edit.SetSel(url.lStartPos , url.lStartPos);
				edit.UnlockWindowUpdate();
				

				OnEditDelete(strtemp_1.GetLength());

//				return;

			}
		
			
			CString strStart, strEnd;
			strStart = GetTextFromPoint(url.lStartPos,true);
			strEnd = GetTextFromPoint(url.lStartPos,false);
			
			CString strtemp = url.strRedoUndo;
			strtemp.Replace(L"\r\n",L"\r"); 
			strtemp.Replace(L"\r",L"\r\n"); 
			int i_temp_len = strtemp.GetLength(); 
			
			CEdit& edit = GetEditCtrl();
			int pos=GetScrollPos(SB_VERT);
			edit.LockWindowUpdate();
			SetWindowText(strStart + strtemp + strEnd);
			edit.LineScroll(pos);
			//CEdit& edit = GetEditCtrl();
			edit.SetSel(url.lStartPos + i_temp_len  , url.lStartPos + i_temp_len);
			edit.UnlockWindowUpdate();
			

			int nCharIndex_real = GetActualPos(url.lStartPos);

		    CString str_buffer = url.strRedoUndo;

			
			str_buffer.Replace(L"\n",L""); 

			m_pDoc->UpdateTextVersionPos(str_buffer, nCharIndex_real, false);  
			if(bFirstType==TRUE)
			{
				SetTextToUpperView();
				bFirstType=FALSE;
			}


			// add a record to undo LIST
			UNDOREDOLOG url_undo;
			url_undo.bIsAdd = false;
			url_undo.lSeq = 0;
			url_undo.strRedoUndo = url.strRedoUndo;
			url_undo.lLen = url.lLen;
			url_undo.lStartPos = url.lStartPos;

			url_undo.bExtra = url.bExtra;
			url_undo.strRedoUndo_Extra = url.strRedoUndo_Extra;
			url_undo.strRedoUndo_Top = url.strRedoUndo_Top;
			url_undo.strMark = url.strMark;
			url_undo.strComment = url.strComment;
			url_undo.strStyle = url.strStyle;
			
			m_pDoc->m_arrUndo.Add(url_undo);	

		
		
		}
		else
		{

			CString strStart, strEnd;
			
			CString strtemp = url.strRedoUndo;
			
			int i_temp_len = strtemp.GetLength(); 

			strtemp.Replace(L"\r\n",L"\r"); 
			strtemp.Replace(L"\r",L"\r\n"); 

			strStart = GetTextFromPoint(url.lStartPos,true);
			strEnd = GetTextFromPoint(url.lStartPos+i_temp_len,false);
			
			CEdit& edit = GetEditCtrl();
			int pos=GetScrollPos(SB_VERT);
			edit.LockWindowUpdate();
			SetWindowText(strStart + strEnd);
			edit.LineScroll(pos);
			//CEdit& edit = GetEditCtrl();
			edit.SetSel(url.lStartPos , url.lStartPos);
			edit.UnlockWindowUpdate();
			
			OnEditDelete(url.lLen);

			// add a record to undo LIST
			UNDOREDOLOG url_undo;
			url_undo.bIsAdd = true;
			url_undo.lSeq = 0;
			url_undo.strRedoUndo = url.strRedoUndo;
			url_undo.lLen = url.lLen;
			url_undo.lStartPos = url.lStartPos;
			url_undo.strRedoUndo_Top = url.strRedoUndo_Top;
			url_undo.strMark = url.strMark;
			url_undo.strComment = url.strComment;
			url_undo.strStyle = url.strStyle;
			url_undo.bExtra = url.bExtra;
			m_pDoc->m_arrUndo.Add(url_undo);	

		}
		
		// remove the current undo redo item
		m_pDoc->m_arrRedo.RemoveAt(m_pDoc->m_arrRedo.GetSize()-1);
		m_pDoc->SetModifiedFlag(TRUE);
	}	

	if (m_pDoc)
		DisplayDocInfo();
	
}

void editView_Bottom::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	if (m_pDoc->m_arrUndo.GetSize() >0 && m_b_allow_edit)
		pCmdUI->Enable(TRUE); 
	else
		pCmdUI->Enable(FALSE); 
}

void editView_Bottom::OnUpdateCommand(CCmdUI* pCmdUI) 
{
		pCmdUI->Enable(FALSE); 
}

void editView_Bottom::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	if (m_pDoc->m_arrRedo.GetSize() >0 && m_b_allow_edit)
		pCmdUI->Enable(TRUE); 
	else
		pCmdUI->Enable(FALSE); 
}

void editView_Bottom::OnUpdateOptionEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->Enable(TRUE); 
}


void editView_Bottom::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	if (m_b_allow_edit)
		pCmdUI->Enable(TRUE); 
	else
		pCmdUI->Enable(FALSE); 
}

/*
void editView_Bottom::OnUpdateDoCommit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	if (m_b_allow_edit)
		pCmdUI->Enable(TRUE); 
	else
		pCmdUI->Enable(FALSE); 
}
void editView_Bottom::OnUpdateDoModify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	if (m_b_disable_modify)
		pCmdUI->Enable(FALSE); 
	else
		pCmdUI->Enable(TRUE); 
}
*/

HBRUSH editView_Bottom::CtlColor(CDC* pDC, UINT nCtlColor) 
{
// 	pDC->SetBkColor(RGB(255, 255, 255));


//	CFont font;
//	VERIFY(font.CreatePointFont(100,L"Arial Unicode MS"));
//	pDC->SelectObject(&font);

	pDC->SetTextColor(RGB(0, 110, 255));
//pDC->InvertRgn( 

//pDC->FillSolidRect(0, 0, 100, 200, RGB(111, 111, 111)); 

//static int  npOutTextExt[100];

//pDC->ExtFloodFill(0, 0, RGB(111, 111, 111));
//	return static_cast<HBRUSH>(CBrush(RGB(255, 255, 255)).GetSafeHandle());


// TODO: Change any attributes of the DC here
TRACE(L"%d\n", nCtlColor);
if (nCtlColor == CTLCOLOR_STATIC)
{
//pDC-&gt;SetBkColor(::GetSysColor(COLOR_WINDOW));
CBrush cb(::GetSysColor(COLOR_WINDOW));
HBRUSH hb = cb;
cb.Detach();
return hb;
}
return NULL;
}

void editView_Bottom::ReleaseStatusBar()
{
	if (!m_pStatusBar)
		return;
	int i;
	for (i=0; i < SB_INDICATORS_NEED; i ++)
	{
		m_pStatusBar->SetPaneInfo(i+1, ID_SEPARATOR, SBPS_NOBORDERS, 1);
		m_pStatusBar->SetPaneText(i+1, L"");
	}
}

void editView_Bottom::SetTextToUpperView() 
{
	m_bUpdateUpper = TRUE;
	// display the text 
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	CMDIChildWnd *pChild2 = (CMDIChildWnd *)this->GetParentFrame();
	if (pChild != pChild2)
		return;
//	GetWindowText(strText);
	//COleDateTime dt2;
	//dt2 = COleDateTime::GetCurrentTime() - COleDateTimeSpan(0,0,0,m_nSpanSec);
	//if (dt2>=dt)
	//{
	
	BOOL bIsCommentMode = ((editFrameNew*)pChild)->GetUpperViewMode();
	if(bIsCommentMode==TRUE)
	{
		CString str; GetWindowTextW(str);
		((editFrameNew*)pChild)->SetTextToUpperView(str, m_pDoc->m_strVersion_Mark, m_pDoc->m_strModHistory);
	}
	else
	{
		((editFrameNew*)pChild)->SetTextToUpperView(m_pDoc->m_strVersion, m_pDoc->m_strVersion_Mark, m_pDoc->m_strModHistory);
	}
	//KillTimer(1002);
	//SetTimer(1002,m_nSpanSec*1000,0);
	
		//dt=COleDateTime::GetCurrentTime();
	//	m_bUpdateUpper = FALSE;
	//	dt = dt2;
	//}

	if (m_pDoc)
		DisplayDocInfo();
}

void editView_Bottom::ChangeUpperViewFont() 
{
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();

	((editFrameNew*)pChild)->ChangeUpperViewFont();
}



void editView_Bottom::SetReadOnly(BOOL bReadOnly)
{

	GetEditCtrl().SetReadOnly(bReadOnly);
	m_b_allow_edit = !bReadOnly; 

	if (m_b_allow_edit)
	{
		AutoSaveFileToLocal();
//		wchar_t sz_temp[255];
//		GetPrivateProfileStringW(L"EditPro" , L"AutoSave",L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
//		m_isAutoSave = sz_temp;
	}
	else
	{
		KillTimer(1001);
		m_autoSaveDuration=0;
	}
}

void editView_Bottom::OnUpdatePrintView(CCmdUI* pCmdUI){
	pCmdUI->Enable(TRUE); 
}

void editView_Bottom::OnFilePrintPreview(){
	CView::OnFilePrintPreview();
} 

void editView_Bottom::OnUpdateEditReadOnly(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE); 
	pCmdUI->SetCheck(!m_b_allow_edit);
}

void editView_Bottom::OnEditReadOnly() 
{
	m_b_allow_edit = !m_b_allow_edit; 
	DisplayDocInfo();
}

void editView_Bottom::OnUpdateEditShowcharcode(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE); 
}


void editView_Bottom::OnEditShowcharcode()
{

	if (!m_pStatusBar)
		return;

/*	if ( !m_pDoc->LockDocBuffer( TRUE ) )
		return;
	SetPrivateBufferPtr();
	
	char szTemp[TMPSTR_SIZE];
	LONG lPos = FindCurrentVisible(m_pDoc->m_lModSelEnd);
	if( lPos >= m_lChars )
		return;
	if( (unsigned char)m_pText[lPos] >= (unsigned char)'\x80' )	// chinese
	{
		sprintf(szTemp, "%c%c[%02X%02X]", 
			m_pText[lPos],	m_pText[lPos+1],
			(int)((unsigned char)m_pText[lPos]),
			(int)((unsigned char)m_pText[lPos+1]));
	}
	else
	{
		sprintf(szTemp, "%c[%02X]", 
			m_pText[lPos],
			(int)((unsigned char)m_pText[lPos]) );
	}
*///	DisplayMessage(szTemp);
	
	  CString strSelectText = L""; 
	GetWindowText(strSelectText);

 	  int   nStartChar,   nEndChar;   
	  GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	  ASSERT((UINT)nEndChar   <=   GetBufferLength());   

	CString str_Temp = strSelectText.Mid(nEndChar,1);

	const wchar_t* szTemp = (LPCTSTR)str_Temp; 

// Convert widechar to utf-8
	int aLen = WideCharToMultiByte(CP_UTF8, 0, szTemp, -1, NULL, 0, NULL, NULL);
	char* converted = new char[aLen];
	WideCharToMultiByte(CP_UTF8, 0, szTemp, -1, converted, aLen, NULL, NULL);	
	
//	wchar_t szTemp1[10]; 
//	_itow(*szTemp, szTemp1, 16);

	CString str_Temp_Hex = "";

	for (int i=0; i < aLen -1 ; i++)
	{
		wchar_t szTemp1[10]; 
		_itow(*(converted+i), szTemp1, 16);

		if (str_Temp_Hex.IsEmpty()) 
			str_Temp_Hex = szTemp1;
		else
			str_Temp_Hex = str_Temp_Hex + " " + szTemp1;
	}
	

	str_Temp_Hex.Replace(L"ffffff",L""); 
	m_pStatusBar->SetPaneText(SB_STATUS, str_Temp + L"[" + str_Temp_Hex + L"]");
//	m_pDoc->LockDocBuffer(FALSE);
}

void editView_Bottom::DisplayDocInfo()
{
	if (!m_pStatusBar)
		return;

	CClientDC dc(this);
	UINT uID, uStyle;
	int  cxWidth;
	CSize size;
	// Words
	m_pStatusBar->GetPaneInfo( SB_WORDS, uID, uStyle, cxWidth);
	size = dc.GetTextExtent( L"0000000", 7 );
	cxWidth = size.cx;
	m_pStatusBar->SetPaneInfo( SB_WORDS, uID, SBPS_NORMAL, cxWidth);
    // Lines
	m_pStatusBar->GetPaneInfo( SB_LINES, uID, uStyle, cxWidth);
	size = dc.GetTextExtent( L"00000", 5 );
	cxWidth = size.cx;
	m_pStatusBar->SetPaneInfo( SB_LINES, uID, SBPS_NORMAL, cxWidth);
	// File Status
	m_pStatusBar->GetPaneInfo( SB_FILESTATUS, uID, uStyle, cxWidth);
	cxWidth = size.cx;
	m_pStatusBar->SetPaneInfo( SB_FILESTATUS, uID, SBPS_NORMAL, cxWidth);

	CString str;
	 int   nStartChar,   nEndChar;   

	GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	ASSERT((UINT)nEndChar   <=   GetBufferLength());   
	
	int lWord = m_pDoc->GetActualWordCount();
	actualWordCount = lWord;
	str.Format(L"%ld", lWord);
	m_pStatusBar->SetPaneText(SB_WORDS, str);

	str.Format(L"%ld", GetEditCtrl().LineFromChar(nEndChar)+1);	
	m_pStatusBar->SetPaneText(SB_LINES, str);

	str = "";
	if (!m_b_allow_edit)
		str = L"唯讀";
	m_pStatusBar->SetPaneText(SB_FILESTATUS, str);
}

void editView_Bottom::InitStatusBar()
{
	if (!m_pStatusBar)
		return;
	// Set Pane Info
	CClientDC dc(this);
	UINT uID, uStyle;
	int  cxWidth;
	CString str;
	CSize size;
	// Words
	m_pStatusBar->GetPaneInfo( SB_WORDS, uID, uStyle, cxWidth);
	size = dc.GetTextExtent( L"0000000", 7 );
	cxWidth = size.cx;
	m_pStatusBar->SetPaneInfo( SB_WORDS, uID, SBPS_NORMAL, cxWidth);
    // Lines
	m_pStatusBar->GetPaneInfo( SB_LINES, uID, uStyle, cxWidth);
	size = dc.GetTextExtent( L"00000", 5 );
	cxWidth = size.cx;
	m_pStatusBar->SetPaneInfo( SB_LINES, uID, SBPS_NORMAL, cxWidth);
	// File Status
	m_pStatusBar->GetPaneInfo( SB_FILESTATUS, uID, uStyle, cxWidth);
	str = L"唯讀";
	size = dc.GetTextExtent( str, str.GetLength() );
	cxWidth = size.cx;
	m_pStatusBar->SetPaneInfo( SB_FILESTATUS, uID, SBPS_NORMAL, cxWidth);

	if (m_pDoc)
		DisplayDocInfo();
}

void editView_Bottom::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int nStyle = GetTextStyle();
	CMenu* pPopup = m_pPopupMenu->GetSubMenu(0);
	CMenu* pSubMenu = pPopup->GetSubMenu(MENUPOS_TEXTSTYLE);

	for (int i=0; i < pSubMenu->GetMenuItemCount(); i++)
		pSubMenu->CheckMenuItem(i,  MF_UNCHECKED|MF_BYPOSITION);

	pSubMenu->CheckMenuItem(nStyle , MF_CHECKED|MF_BYPOSITION);

	ClientToScreen(&point);

		RECT		m_rcWnd;  
	pPopup->TrackPopupMenu(
		TPM_RIGHTBUTTON, point.x, point.y, this, &m_rcWnd);
	m_Tips.HideTips();
//	pSubMenu->CheckMenuItem(nStyle, MF_UNCHECKED|MF_BYPOSITION);
	CView::OnRButtonDown(nFlags, point);
	
}

CString editView_Bottom::GetActualSelectText(int &pos,  int &i_Select_Text_Len, int &nLen) 
{
	i_Select_Text_Len = 0;
	  int   nStartChar,   nEndChar;   
	  GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	  ASSERT((UINT)nEndChar   <=   GetBufferLength());   
	
	  CString strSelectText = L""; 

	  ASSERT((UINT)nEndChar   <=   GetBufferLength());   

	  LPCTSTR   lpszText   =   LockBuffer();   
	  nLen   =   nEndChar   -   nStartChar;   
	  memcpy(strSelectText.GetBuffer(nLen), lpszText + nStartChar,  nLen   *   sizeof(TCHAR));   

		  wchar_t*  lptemp; 
		  
		  int i_char_count = 0;
		  bool b_newline_detect = false; 	
		  
		 /* for (int i=0; i < nLen; i++)
		  {
			  lptemp = new wchar_t[1];
			  memset(lptemp , 0, 1);
			  wcsncpy(lptemp, lpszText + nStartChar + i,  1);  
			   CString str_temp(lptemp);

			   if (str_temp[0] == wchar_t(13)) 
			   {
					b_newline_detect = true;
			   }

			   if (b_newline_detect && str_temp[0] == wchar_t(10))
			   {
				   b_newline_detect = false; 	
				   continue;
			   }

			   i_char_count = i_char_count + 1;
		  }*/

		//i_Select_Text_Len = i_char_count; 
		  strSelectText.ReleaseBuffer(nLen);
		  i_Select_Text_Len = strSelectText.GetAllocLength()-strSelectText.Replace(L"\r\n",L"\r\n"); 
		
		UnlockBuffer(); 

//	bool b_Select_Text = false;
//	if (i_Select_Text_Len > 0) b_Select_Text = true;


//	if (i_Select_Text_Len == 0)
//		i_Select_Text_Len = 1;
	
	pos = GetActualPos(nStartChar);
		return strSelectText;
}


void editView_Bottom::GetSelectPosAndLength(int &pos,  int &i_Select_Text_Len) 
{
	  i_Select_Text_Len = 0;
	
	  int nLen;
		GetActualSelectText(pos,  i_Select_Text_Len, nLen);
/*	  int   nStartChar,   nEndChar;   
	  GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	  ASSERT((UINT)nEndChar   <=   GetBufferLength());   
	
	  CString strSelectText = L""; 

	  ASSERT((UINT)nEndChar   <=   GetBufferLength());   

	  LPCTSTR   lpszText   =   LockBuffer();   
	  UINT   nLen   =   nEndChar   -   nStartChar;   
	  memcpy(strSelectText.GetBuffer(nLen), lpszText + nStartChar,  nLen   *   sizeof(TCHAR));   

		  wchar_t*  lptemp; 
		  
		  int i_char_count = 0;
		  bool b_newline_detect = false; 	
		  
		  for (int i=0; i < nLen; i++)
		  {
			  lptemp = new wchar_t[1];
			  memset(lptemp , 0, 1);
			  wcsncpy(lptemp, lpszText + nStartChar + i,  1);  
			   CString str_temp(lptemp);

			   if (str_temp[0] == char(13)) 
			   {
					b_newline_detect = true;
			   }

			   if (b_newline_detect && str_temp[0] == char(10))
			   {
				   b_newline_detect = false; 	
				   continue;
			   }

			   i_char_count = i_char_count + 1;
		  }

		i_Select_Text_Len = i_char_count; 
		strSelectText.ReleaseBuffer(nLen);   
		UnlockBuffer(); 

	bool b_Select_Text = false;
	if (i_Select_Text_Len > 0) b_Select_Text = true;


	if (i_Select_Text_Len == 0)
		i_Select_Text_Len = 1;
	
//    pos = nStartChar;
	pos = GetActualPos(nStartChar);
*/
}

void editView_Bottom::OnEditAddcomment() 
{
	if(m_b_allow_edit!=TRUE)
		return;
	int pos, i_Select_Text_Len;  
	GetSelectPosAndLength(pos, i_Select_Text_Len);

	m_bOpenFindDialog = TRUE;
	CInputCommentDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		int iCommentIndex = -1;
		if (m_pDoc->AddComment(dlg.m_str_Comment, iCommentIndex) == false)
		{
			AfxMessageBox(L"現最多只可增加 30 個批注!");
			return;	
		}

		if (iCommentIndex != -1)
			m_pDoc->SetTextVersionComment(pos, i_Select_Text_Len, iCommentIndex, false);

		/*CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
		((editFrameNew*)pChild)->UpdateUpperCommentModeView();*/
		OnChangeUpperView();OnChangeUpperView();
	}
	m_bOpenFindDialog = FALSE;
}

void editView_Bottom::OnEditShowcomment() 
{
	//m_pDoc->CheckCompareDiff();
	int pos, i_Select_Text_Len;  
	GetSelectPosAndLength(pos, i_Select_Text_Len);

	CString strComment = m_pDoc->GetTextVersionComment(pos, i_Select_Text_Len);
   
	ShowCommentInToolTip();
   //m_pStatusBar->SetPaneText(SB_STATUS, strComment);
}

void editView_Bottom::OnDeletecomment() 
{
	if(m_b_allow_edit!=TRUE)
		return;
	int pos, i_Select_Text_Len;  
	GetSelectPosAndLength(pos, i_Select_Text_Len);

	m_pDoc->SetTextVersionComment(pos, i_Select_Text_Len, 0, true);
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	CString str;GetWindowTextW(str);
	//((editFrameNew*)pChild)->SetTextToUpperView(str, L"", L"");
	//((editFrameNew*)pChild)->UpdateUpperCommentModeView();
	OnChangeUpperView();OnChangeUpperView();
}

void editView_Bottom::OnEditGotoline() 
{
//	int nLine = 1, i;

	int nStartChar,   nEndChar;
	 GetEditCtrl().GetSel(nStartChar,   nEndChar);   

	int i_index = GetEditCtrl().LineFromChar(nStartChar);
	CInputLineNumDlg dlg;
	dlg.m_nLineNum = i_index + 1;
	if (dlg.DoModal() != IDOK)
		return;

	i_index = dlg.m_nLineNum;

	int i_index_line = 0;
	int i_return_char = 0;
	for (int i=0; i < GetEditCtrl().GetWindowTextLength(); i++)
	{
//	    GetEditCtrl().GetSel(nStartChar,   nEndChar);   
		i_index_line = GetEditCtrl().LineFromChar(i);
		if ((i_index_line + 1) == i_index)
		{
			i_return_char = i;
			break;
		}
		
	}
	GetEditCtrl().SetFocus();
	GetEditCtrl().SetSel(i_return_char,i_return_char);

//	GetEditCtrl().LineScroll(i_index-1);
	DisplayDocInfo();
}


void editView_Bottom::OnEditReplace() 
{
	KillTimer(1002);KillTimer(1003);
	SetTimer(1003,0,0);
	int pos, i_Select_Text_Len, nLen;
	CString str_Select_Text = GetActualSelectText(pos, i_Select_Text_Len, nLen);

	if (!m_bOpenFindDialog)
	{
		pDlg_Test = new CFindReplaceDialog(); 
		pDlg_Test->Create( FALSE, str_Select_Text, NULL, FR_DOWN, this ); 
		pDlg_Test->ShowWindow( SW_SHOW );
		m_bOpenFindDialog = true;
	}

}

void editView_Bottom::OnEditFind() 
{
	int pos, i_Select_Text_Len, nLen;
	CString str_Select_Text = GetActualSelectText(pos, i_Select_Text_Len, nLen);

	if (!m_bOpenFindDialog)
	{
		pDlg_Test = new CFindReplaceDialog(); 
		pDlg_Test->Create( TRUE, str_Select_Text, NULL, FR_DOWN, this ); 
		pDlg_Test->ShowWindow( SW_SHOW );
		m_bOpenFindDialog = true;
	}
}


void editView_Bottom::OnFind()
{
	return ;
}
/*
LRESULT editView_Bottom::OnReplace(WPARAM wParam,LPARAM lParam)
{
	return 1;

}
*/
void editView_Bottom::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_b_allow_edit);
}

void editView_Bottom::OnUpdateFileReplace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_b_allow_edit);
}


void editView_Bottom::OnFileSave() 
{
	m_pDoc->SaveDocumentLocal();
//	::OnSaveDocument(GetPathName());
}

void editView_Bottom::OnFileSaveAs() 
{
	m_pDoc->SaveDocumentLocal();
//	::OnSaveDocument(GetPathName());
}

void editView_Bottom::SetTextStyle(int i_Style)
{
	int nStartChar,   nEndChar;
	 GetEditCtrl().GetSel(nStartChar,   nEndChar);   

	int i_index = GetEditCtrl().LineFromChar(nStartChar);
	int i_line_len = 0;

	int i_index_line = 0;
	int i_return_char = 0;

	bool b_Found = false;
	
	for (int i=0; i < GetEditCtrl().GetWindowTextLength(); i++)
	{
		i_index_line = GetEditCtrl().LineFromChar(i);
		if (i_index_line == i_index && !b_Found)
		{
			
			i_return_char = i;
			b_Found = true;
			//break;
		}

		if (b_Found)
			i_line_len = i_line_len +1;

		if (i_index_line == i_index+1)
			break;
	}


	int pos = GetActualPos(i_return_char);

	if (i_line_len >=2) i_line_len =  i_line_len-2;

	m_pDoc->SetTextVersionStyle(pos, i_line_len, i_Style);

	m_pDoc->SetModifiedFlag(TRUE);

}

void editView_Bottom::OnTsStyle1() 
{
	SetTextStyle(0);
}
void editView_Bottom::OnTsStyle10() 
{
	SetTextStyle(9);
}
void editView_Bottom::OnTsStyle11() 
{
	SetTextStyle(10);
}
void editView_Bottom::OnTsStyle12() 
{
	SetTextStyle(11);
}
void editView_Bottom::OnTsStyle13() 
{
	SetTextStyle(12);
}
void editView_Bottom::OnTsStyle14() 
{
	SetTextStyle(13);
}
void editView_Bottom::OnTsStyle15() 
{
	SetTextStyle(14);
}
void editView_Bottom::OnTsStyle2() 
{
	SetTextStyle(1);
}
void editView_Bottom::OnTsStyle3() 
{
	SetTextStyle(2);
}
void editView_Bottom::OnTsStyle4() 
{
	SetTextStyle(3);
}
void editView_Bottom::OnTsStyle5() 
{
	SetTextStyle(4);
}
void editView_Bottom::OnTsStyle6() 
{
	SetTextStyle(5);
}
void editView_Bottom::OnTsStyle7() 
{
	SetTextStyle(6);
}
void editView_Bottom::OnTsStyle8() 
{
	SetTextStyle(7);
}
void editView_Bottom::OnTsStyle9() 
{
	SetTextStyle(8);
}

int editView_Bottom::GetTextStyle()
{
	int nStartChar,   nEndChar;
	 GetEditCtrl().GetSel(nStartChar,   nEndChar);   

	int pos = GetActualPos(nStartChar);
	 
	return m_pDoc->GetTextVersionStyle(pos);

}

// ========= Print Option ========
typedef struct
{
	int nPrintWidth;
	int nPrintColumn;
	int nColumnDistance;
	BOOL bPrintSelection;
	BOOL bNewSelectionLineNum;	// LineNum will begin from 1 in selection
	LONG lStartLine;
	LONG lEndLine;
	LONG lLineNum;
	LONG lWords;
	RECT rcPrint;
	BOOL bPrintLineNum;
	CFontSet* pPrintFontSet;
	int iStartChar; // new add
	int iStartChar_2; // new add
}PRINTOPTION, *PPRINTOPTION;

BOOL editView_Bottom::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (AfxGetApp()->GetProfileInt(L"EditProPrint", L"PrintSelection", 0))
	{
		int pos, i_Select_Text_Len, nLen;
		CString str_Text;
		str_Text = GetActualSelectText(pos, i_Select_Text_Len, nLen);
		if (str_Text.IsEmpty())
		{
			AfxMessageBox(IDS_NOSELECTIONTOPRINT);
			return FALSE;
		}
	}

	return DoPreparePrinting(pInfo);
}

void editView_Bottom::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	static PRINTOPTION PrintOption;
	pInfo->m_lpUserData = &PrintOption;

	PrintOption.pPrintFontSet = new CFontSet;
	PrintOption.pPrintFontSet->UpdatePrintFontSet(pDC);

	CWinApp *pApp = AfxGetApp();

	PrintOption.lLineNum = 0;	// indicate need calculate Page Number and Line Number
	PrintOption.lWords = 0;

	PrintOption.iStartChar = 0;
	PrintOption.iStartChar_2 = 0;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	PrintOption.nPrintWidth = GetPrivateProfileIntW( L"EditProPrint" , L"LineChars",40,((CWinApp*)AfxGetApp())->m_pszProfileName);
    PrintOption.nPrintColumn = GetPrivateProfileIntW( L"EditProPrint" , L"Columns",2,((CWinApp*)AfxGetApp())->m_pszProfileName);
    PrintOption.nColumnDistance = GetPrivateProfileIntW( L"EditProPrint" , L"ColumnDistance",20,((CWinApp*)AfxGetApp())->m_pszProfileName);
	// PrintOption.nPrintWidth = pApp->GetProfileInt(L"EditProPrint", L"LineChars", 40);
    // PrintOption.nPrintColumn = pApp->GetProfileInt(L"EditProPrint", L"Columns", 2);
    // PrintOption.nColumnDistance = pApp->GetProfileInt(L"EditProPrint", L"ColumnDistance", 20);
	//end 20120927
   

	int nLeftMargin;
	int nRightMargin;
	int nTopMargin;
	int nBottomMargin;
	int nLogicalPixelX, nLogicalPixelY;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	// Edit Stephen 2013-10-28
	int nLineSpacing = GetPrivateProfileIntW( L"EditProPrint" , L"LineSpacing",20,((CWinApp*)AfxGetApp())->m_pszProfileName);
	// End Edit Stephen 2013-10-28
	nLogicalPixelX = GetPrivateProfileIntW( L"EditProPrint" , L"LogicalPixelX",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	nLogicalPixelY = GetPrivateProfileIntW( L"EditProPrint" , L"LogicalPixelY",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	nLeftMargin = GetPrivateProfileIntW( L"EditProPrint" , L"LeftMargin",10,((CWinApp*)AfxGetApp())->m_pszProfileName);
	nRightMargin = GetPrivateProfileIntW( L"EditProPrint" , L"RightMargin",10,((CWinApp*)AfxGetApp())->m_pszProfileName);
	nTopMargin = GetPrivateProfileIntW( L"EditProPrint" , L"TopMargin",10,((CWinApp*)AfxGetApp())->m_pszProfileName);
	nBottomMargin = GetPrivateProfileIntW( L"EditProPrint" , L"BottomMargin",10,((CWinApp*)AfxGetApp())->m_pszProfileName);
	PrintOption.nColumnDistance = GetPrivateProfileIntW( L"EditProPrint" , L"ColumnDistance",10,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//nLogicalPixelX = pApp->GetProfileInt( L"EditProPrint", L"LogicalPixelX", 1);
	//nLogicalPixelY = pApp->GetProfileInt( L"EditProPrint", L"LogicalPixelY", 1);
	//nLeftMargin = pApp->GetProfileInt( L"EditProPrint", L"LeftMargin", 10);
	//nRightMargin = pApp->GetProfileInt( L"EditProPrint", L"RightMargin", 10);
	//nTopMargin = pApp->GetProfileInt( L"EditProPrint", L"TopMargin", 10);
	//nBottomMargin = pApp->GetProfileInt( L"EditProPrint", L"BottomMargin", 10);
	//PrintOption.nColumnDistance = pApp->GetProfileInt( L"EditProPrint", L"ColumnDistance", 10);
	//end 20120927
	
	// Scale log size
	nLeftMargin = MulDiv(nLeftMargin,
		pDC->GetDeviceCaps(LOGPIXELSX),
		nLogicalPixelX);
	nTopMargin = MulDiv(nTopMargin,
		pDC->GetDeviceCaps(LOGPIXELSY),
		nLogicalPixelY);
	nRightMargin = MulDiv(nRightMargin,
		pDC->GetDeviceCaps(LOGPIXELSX),
		nLogicalPixelX);
	nBottomMargin = MulDiv(nBottomMargin,
		pDC->GetDeviceCaps(LOGPIXELSY),
		nLogicalPixelY);
	PrintOption.nColumnDistance = MulDiv(PrintOption.nColumnDistance,
		pDC->GetDeviceCaps(LOGPIXELSY),
		nLogicalPixelY);

	PrintOption.rcPrint.left = nLeftMargin;
	PrintOption.rcPrint.top = nTopMargin;
	PrintOption.rcPrint.right = pDC->GetDeviceCaps(HORZRES) - nRightMargin;
	PrintOption.rcPrint.bottom = pDC->GetDeviceCaps(VERTRES) - nBottomMargin;

	//edit by Ronald 20120927 for migrating from vc6 to 2010
	PrintOption.bPrintSelection = GetPrivateProfileIntW( L"EditProPrint" , L"PrintSelection",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	PrintOption.bNewSelectionLineNum = GetPrivateProfileIntW( L"EditProPrint" , L"NewSelectionLineNum",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	PrintOption.bPrintLineNum = GetPrivateProfileIntW( L"EditProPrint" , L"PrintLineNum",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//PrintOption.bPrintSelection = pApp->GetProfileInt(L"EditProPrint", L"PrintSelection", FALSE);
    //PrintOption.bNewSelectionLineNum = pApp->GetProfileInt(L"EditProPrint", L"NewSelectionLineNum", FALSE);
    //PrintOption.bPrintLineNum = pApp->GetProfileInt(L"EditProPrint", L"PrintLineNum", FALSE);
	//end 20120927


	//check the print width
	int nPaperWidth = PrintOption.rcPrint.right - PrintOption.rcPrint.left;

	int nPaperHeight = PrintOption.rcPrint.bottom - PrintOption.rcPrint.top;
	
	
	int nColumnWidth = PrintOption.nPrintWidth * PrintOption.nPrintColumn;
	if (PrintOption.bPrintLineNum) // 00000_
		nColumnWidth += PrintOption.pPrintFontSet->GetBaseCharWidth() * 3;
	if (nColumnWidth + PrintOption.nColumnDistance * PrintOption.nPrintColumn > nPaperWidth)
	{
		AfxMessageBox(L"IDS_ERRORPRINTWIDTH",MB_OK);
		PrintOption.nPrintWidth = nPaperWidth / PrintOption.pPrintFontSet->GetBaseCharWidth() - 1;
		PrintOption.nPrintColumn = 1;
	}

	m_bShowComment = FALSE;

	CFont font1;
	CFont *f_temp = GetPrintFontSize();
	LOGFONT lf;f_temp->GetLogFont(&lf);
	CClientDC dc(this);
	int abc, ContextSize;
	abc = abs(MulDiv(lf.lfHeight,72,dc.GetDeviceCaps(LOGPIXELSY)));
	ContextSize = abc*10;
	font1.CreatePointFont(ContextSize, lf.lfFaceName, pDC);
	pDC->SelectObject(&font1);

	CString str_Text;

	if (PrintOption.bPrintSelection)
	{
		int pos, i_Select_Text_Len, nLen;
		str_Text = GetActualSelectText(pos, i_Select_Text_Len, nLen);
		if (str_Text.IsEmpty())
			GetWindowText(str_Text);
	}
	else
		GetWindowText(str_Text);

//	pDC->TextOut(rcPrint.left, 650, str_Temp);
	PPRINTOPTION pPrintOption;
	pPrintOption = (PPRINTOPTION)pInfo->m_lpUserData;

	RECT rcPrint;
	memcpy(&rcPrint, &(pPrintOption->rcPrint), sizeof(RECT));

	int i_x_shift = rcPrint.left;
	int i_Current_Line = 0;

	bool b_space = false;

	int i_line_num_shift = 0;
	if (pPrintOption->bPrintLineNum)
		i_line_num_shift = nPaperWidth * 0.04;

	//int i_total_line_allow = nPaperHeight / 120;
	// Edit Stephen 2013-10-28
//	int i_total_line_allow = nPaperHeight / (ContextSize+10); 
	int i_total_line_allow = nPaperHeight / (ContextSize+10+nLineSpacing); 

	if (i_total_line_allow > 1 ) i_total_line_allow = i_total_line_allow -1;
	// End Edit Stephen 2013-10-28

	m_pDoc->m_arrPrintPageInfo.RemoveAll();

	int i_page_index = 0;	
	int i_page_temp = 0;	
	int i_last_char_pos = 0;
	
	int i=0;
	int iProjectedIndex = -1;
	for (i=0; i < str_Text.GetLength(); i++)
	{
		
		CString str_temp_convert = str_Text[i];
		BOOL bWord =  FALSE;
		char* pszGbt=NULL; 
		wchar_t* wszUnicode=NULL; 
		CString str = str_temp_convert, str_Converted;
		int iLen = 0;
		iLen=WideCharToMultiByte (CP_UTF8, 0, str, -1, NULL,0, NULL, NULL) ; 
		pszGbt=new char[iLen+1];
		wszUnicode=new wchar_t[iLen+1];   
		WideCharToMultiByte (CP_UTF8, 0, str, -1, pszGbt,iLen, NULL, NULL) ;
		MultiByteToWideChar (CP_UTF8, 0, pszGbt, -1, wszUnicode,iLen); 
		str_Converted.Format(L"%s",wszUnicode );
		if (str_temp_convert != str_Converted)
		{
			str_temp_convert = str_Text.Mid(i,2);
			i++;
			bWord = TRUE;
		}
		delete []pszGbt;
		delete []wszUnicode;
		wchar_t c = str_temp_convert.GetAt(0);
		if(c>0&&c<255)
		{
			if((isalpha(c)||isdigit(c))&&i>iProjectedIndex )
			{
				CString strTemp = L"";
				int i_Temp_Length = 0, iTotalLength = 0;
				for(int j=i;j<str_Text.GetLength();j++)
				{
					c = str_Text.GetAt(j);
					if(str_Text[j]== L'\r\n' || str_Text[j] == L'\n'||str_Text[j] == L' '||c<0||c>255)
					{
						iProjectedIndex = j;
						pDC->GetCharWidthW(str_Text[j],str_Text[j],&i_Temp_Length);
						strTemp += str_Text[j];
						iTotalLength += i_Temp_Length;
						break;
					}
					pDC->GetCharWidthW(str_Text[j],str_Text[j],&i_Temp_Length);
					strTemp += str_Text[j];
					iTotalLength += i_Temp_Length;
				}
				if(iTotalLength<  ((nPaperWidth  * 0.95) - i_line_num_shift) )
				{
					if(i_x_shift + iTotalLength >  ((nPaperWidth  * 0.95) - i_line_num_shift) )
					{
						i_Current_Line = i_Current_Line + 1; 
						i_x_shift = rcPrint.left;
						i_page_index = i_Current_Line % i_total_line_allow;	

						if (i_Current_Line >= i_total_line_allow && i_page_index==0)
						{
							i_page_temp =  i_Current_Line / i_total_line_allow;

							if (m_pDoc->m_arrPrintPageInfo.GetSize() < i_page_temp )
							{
								PRINTPAGEINFO printInfo;
								printInfo.lPageNo = i_page_temp ; 
								printInfo.lStartPos = i_last_char_pos; 
								//printInfo.lEndPos = i; 
								printInfo.lEndPos = i-1;
								m_pDoc->m_arrPrintPageInfo.Add(printInfo);
								//i_last_char_pos = i + 1;
								i_last_char_pos = i;
							}
						}
					}
				}
			}
		}
		int i_temp_width = 0;
		
		if (str_temp_convert == L'\r\n' || str_temp_convert == L'\n')
		{
			str_temp_convert = "";
		}
		else if (!bWord)
			pDC->GetCharWidthW(str_Text[i],str_Text[i], &i_temp_width); 
		else
		{
			pDC->GetCharWidthW(str_Text[i],str_Text[i], &i_temp_width);
			i_temp_width *=2;
		}

		if (str_temp_convert == " ") 
			b_space = true;
		
		i_x_shift = i_x_shift + i_temp_width;


		int nPaperWidth_temp =  nPaperWidth  * 0.95;
		
		int itemp = 0;
		if(i<str_Text.GetLength()-1)
		{
			CString strtemp = str_Text[i+1];
			pDC->GetCharWidthW(str_Text[i+1],str_Text[i+1], &itemp);
		}

		if (i_x_shift >= (nPaperWidth_temp - i_line_num_shift) || str_Text[i] == L'\r\n' || (i_x_shift >= (nPaperWidth_temp - i_line_num_shift) && b_space)||i_x_shift+itemp >= (nPaperWidth - i_line_num_shift))
		{
			i_Current_Line = i_Current_Line + 1; 
			i_x_shift = rcPrint.left;
			if (i+1 < str_Text.GetLength() && str_Text[i] != L'\r\n' && str_Text[i+1] == L'\r\n')
				i++;

		}
		b_space = false;


//		if (i_Current_Line > 50)
		i_page_index = i_Current_Line % i_total_line_allow;	

		if (i_Current_Line >= i_total_line_allow && i_page_index == 0)
		{
			i_page_temp =  i_Current_Line / i_total_line_allow;

			if (m_pDoc->m_arrPrintPageInfo.GetSize() < i_page_temp )
			{
				PRINTPAGEINFO printInfo;
				printInfo.lPageNo = i_page_temp ; 
				printInfo.lStartPos = i_last_char_pos; 
				printInfo.lEndPos = i; 
				m_pDoc->m_arrPrintPageInfo.Add(printInfo);
				i_last_char_pos = i + 1;
			}
//			pPrintOption->iStartChar_2 = i;
//			if (i_Current_Line > 100)
//				pPrintOption->iStartChar_2 = i;	
		}

	
	}

	PRINTPAGEINFO printInfo;
	printInfo.lPageNo = i_page_temp + 1 ; 
	printInfo.lStartPos = i_last_char_pos; 
	printInfo.lEndPos = i; 
	m_pDoc->m_arrPrintPageInfo.Add(printInfo);

	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(i_page_temp + 1 );


	CView::OnBeginPrinting(pDC, pInfo);
}

void editView_Bottom::GetPrintInfo(int iPageIndex, int &iStartPos, int &iEndPos) 
{
	PRINTPAGEINFO printInfo;
	printInfo=m_pDoc->m_arrPrintPageInfo.GetAt(iPageIndex-1);

	iStartPos =  printInfo.lStartPos; 
	iEndPos =  printInfo.lEndPos; 
}

void editView_Bottom::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	PPRINTOPTION pPrintOption;
	pPrintOption = (PPRINTOPTION)pInfo->m_lpUserData;

	if (pPrintOption)
		delete pPrintOption->pPrintFontSet;
	
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	m_bShowComment = GetPrivateProfileIntW( L"EditPro" , L"ShowComment",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//m_bShowComment = AfxGetApp()->GetProfileInt(L"EditPro", L"ShowComment", TRUE);
	//end 20120927

	CView::OnEndPrinting(pDC, pInfo);
}

void editView_Bottom::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	CView::OnPrepareDC(pDC, pInfo);

	if (pDC->IsPrinting())
	{
	    PPRINTOPTION pPrintOption;
	    pPrintOption = (PPRINTOPTION)pInfo->m_lpUserData;
	    if( pInfo->m_nCurPage > pInfo->GetMaxPage() )
			pInfo->m_bContinuePrinting = FALSE;
		else
			pInfo->m_bContinuePrinting = TRUE;
	}
}

void editView_Bottom::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
    PPRINTOPTION pPrintOption;
    pPrintOption=(PPRINTOPTION)pInfo->m_lpUserData;
	CFontSet* pFontSet = pPrintOption->pPrintFontSet;
	RECT rcPrint;
	memcpy(&rcPrint, &(pPrintOption->rcPrint), sizeof(RECT));

	BOOL bPrintLineNum = pPrintOption->bPrintLineNum;
	int nLineNumWidth = 3 * pFontSet->GetBaseCharWidth();
    
    if (pInfo->m_nCurPage > pInfo->GetMaxPage() || pInfo->m_bContinuePrinting == FALSE)
    	return;
    	
    
	// Get the file name, to be displayed on title page
	//edit by leon 2012-10-03 for migrating from ANSI to Unicode
	//char szTemp[TMPSTR_SIZE];
	//wchar_t szTemp[TMPSTR_SIZE];
	//end by leon 2012-10-03
	CString strPageTitle=L"";
	CString strPageTitle2=L"";
	CString strPageTitle3=L"";
	CString strPageTitle4=L"";

	editDoc* edit = GetDocument();

	CString str_Place, str_Param;
	CGlobal *pGL=CGlobal::GetInstance();
	if(edit->m_FileParam.enType !=WIRESFILE)
	{
		if(edit->m_FileParam.bLib==1)
		{
			str_Place = pGL->m_FileDB.GetLibOrColName(edit->m_FileParam.bLib, edit->m_FileParam.nLibID);
			str_Param = edit->m_FileParam.szParam;
		}
		else
		{
			str_Place = pGL->m_FileDB.GetLibOrColName(edit->m_FileParam.bLib, edit->m_FileParam.nColID);
			str_Param = edit->m_FileParam.szParam;
		}
	}else
	{
		str_Place = pGL->m_FileDB.GetAgencyNameByCode(edit->m_WiresParam.strWiresCode);
	}
	strPageTitle.LoadString(IDS_FILENAME);
	strPageTitle +=	L": " +GetDocument()->GetTitle();       //Filename
	if(str_Place.GetLength()>0)
		strPageTitle += L"_"+str_Place;
	if(str_Param.GetLength()>0)
		strPageTitle += L"_"+str_Param;
//	if(FillSpace((char *)strPageTitle.GetBuffer(strPageTitle.GetLength()),szTemp,40))
//		strPageTitle = szTemp;
    //modified by yulu
    if( pInfo->m_nCurPage >= 1 )
    {
		CString str = L"字數: ";

		CString str_temp;
		//str_temp.Format(L"%ld", m_pDoc->GetActualWordCount());
		str_temp.Format(L"%ld",actualWordCount);
		strPageTitle3 = str;
		strPageTitle3 += str_temp;

		/*str.LoadString(IDS_TOTALLINES);
		sprintf( szTemp, "  %s: %ld", str, pPrintOption->lLineNum);
    	strPageTitle += szTemp;
		*/
		str = L"用戶名稱: ";

		str_temp.Format(L"%s", CGlobal::GetInstance()->m_pEmployee->m_strUserName);
		
//		sprintf( szTemp, "%s：%s", str, m_pDoc->m_szUser);

		strPageTitle2 = str;
		strPageTitle2 += str_temp;

//		if(FillSpace((char *)strPageTitle2.GetBuffer(strPageTitle.GetLength()),szTemp,40))
//			strPageTitle2 = szTemp;

		CTime tm = CTime::GetCurrentTime();
		str = L"時間: ";
		str_temp.Format(L"%02d-%02d-%d %02d:%02d", tm.GetDay(), tm.GetMonth(), tm.GetYear(),  
			tm.GetHour(), tm.GetMinute());

//		sprintf(szTemp,
//			"%s： %02d-%02d-%d %02d:%02d", str, tm.GetDay(), tm.GetMonth(), tm.GetYear(),  
//			tm.GetHour(), tm.GetMinute());
    	
		strPageTitle4 = str;
		strPageTitle4 += str_temp;
		

		str_temp.Format(L"     %u / %u", pInfo->GetMaxPage(), pInfo->m_nCurPage);
		
//		sprintf( szTemp, "    %u / %u", pInfo->GetMaxPage(), pInfo->m_nCurPage);
		strPageTitle4 += str_temp;

		strPageTitle4 += " ";
		if ( !m_pDoc->m_szPrintCaption )
			strPageTitle4 += m_pDoc->m_szPrintCaption;
   }

//	m_bRedraw = FALSE ;
//	pFontSet->SetFont(pDC, TS_NORMAL);
	// Display the file name 1 inch below top of the page,
	// Add by Bob, to standard the title's fons
	CFont font;
//	CFont * pFont;
//	font.CreatePointFont(110, L"Arial Unicode MS", pDC);
	font.CreatePointFont(120, L"新細明體-ExtB", pDC);
//	pFont = pDC->SelectObject(&font);
	pDC->SelectObject(&font);
	// end add by Bob
//	pDC->TextOut(rcPrint.left, 0, strPageTitle);
//	pDC->TextOut(rcPrint.left, m_pFontSet->GetCharHeight(), strPageTitle2);

	pDC->TextOut(rcPrint.left, 150, strPageTitle);
	pDC->TextOut(rcPrint.left, 310, strPageTitle2);

	pDC->TextOut(2000, 150, strPageTitle3);
	pDC->TextOut(2000, 310, strPageTitle4);
	

	// Add by Bob following above. Recover the fonts
//	pDC->SelectObject(pFont);
	// End add by Bob
//	pFontSet->RestoreFont();
	CFont font1;
	CFont *f_temp = GetPrintFontSize();
	LOGFONT lf;f_temp->GetLogFont(&lf);
	CClientDC dc(this);
	int abc, ContextSize;
	abc = abs(MulDiv(lf.lfHeight,72,dc.GetDeviceCaps(LOGPIXELSY)));
	ContextSize = abc*10;
	font1.CreatePointFont(ContextSize, lf.lfFaceName, pDC);
	pDC->SelectObject(&font1);

	CString str_Text;

	if (pPrintOption->bPrintSelection)
	{
		int pos, i_Select_Text_Len, nLen;
		str_Text = GetActualSelectText(pos, i_Select_Text_Len, nLen);
		if (str_Text.IsEmpty())
			GetWindowText(str_Text);
	}
	else
		GetWindowText(str_Text);

//	pDC->TextOut(rcPrint.left, 650, str_Temp);

	int i_x_shift = rcPrint.left;
	int i_Current_Line = 0;

	bool b_space = false;


	int i_page_start = 0;
	int i_page_end = 0;
	int	i_char_count = 0;

//	if (pInfo->m_nCurPage > 1)
//	{
//		i_page_start = pPrintOption->iStartChar_2;
		GetPrintInfo(pInfo->m_nCurPage, i_page_start, i_page_end); 


	int nPaperWidth = pPrintOption->rcPrint.right - pPrintOption->rcPrint.left;
	int nPaperWidth_temp =  nPaperWidth  * 0.95;

		int i = 0;
	int nDefaultlinespacing = GetPrivateProfileIntW( L"EditProPrint" , L"LineSpacing",20,((CWinApp*)AfxGetApp())->m_pszProfileName);
	int nLineSpacing;
	int iProjectedIndex = -1;
	for (i=i_page_start; i <= i_page_end; i++)
	{
		// Edit Stephen 2013-10-28
		if (i_Current_Line == 0) 
			nLineSpacing = 0;
		else
			nLineSpacing = nDefaultlinespacing;
	// End Edit Stephen 2013-10-28

//		if (i > pPrintOption->iStartChar_2 && pInfo->m_nCurPage == 1)
//			break;
		
		BOOL bWord=FALSE;
		CString str_temp_convert = str_Text[i];
		char* pszGbt=NULL; 
		wchar_t* wszUnicode=NULL; 
		CString str = str_temp_convert, str_Converted;
		int iLen = 0;
		iLen=WideCharToMultiByte (CP_UTF8, 0, str, -1, NULL,0, NULL, NULL) ; 
		pszGbt=new char[iLen+1];
		wszUnicode=new wchar_t[iLen+1];   
		WideCharToMultiByte (CP_UTF8, 0, str, -1, pszGbt,iLen, NULL, NULL) ;
		MultiByteToWideChar (CP_UTF8, 0, pszGbt, -1, wszUnicode,iLen); 
		str_Converted.Format(L"%s",wszUnicode );
		if (str_temp_convert != str_Converted)
		{
			str_temp_convert = str_Text.Mid(i,2);
			i++;
			bWord=TRUE;
		}
		delete []pszGbt;
		delete []wszUnicode;
		wchar_t c = str_temp_convert.GetAt(0);
		if(c>0&&c<255)
		{
			if((isalpha(c)||isdigit(c))&&i>iProjectedIndex )
			{
				CString strTemp = L"";
				int i_Temp_Length = 0, iTotalLength = 0;
				for(int j=i;j<i_page_end;j++)
				{
					c = str_Text.GetAt(j);
					if(str_Text[j]== L'\r\n' || str_Text[j] == L'\n'||str_Text[j] == L' '||c<0||c>255)
					{
						iProjectedIndex = j;
						pDC->GetCharWidthW(str_Text[j],str_Text[j],&i_Temp_Length);
						strTemp += str_Text[j];
						iTotalLength += i_Temp_Length;
						break;
					}
					pDC->GetCharWidthW(str_Text[j],str_Text[j],&i_Temp_Length);
					strTemp += str_Text[j];
					iTotalLength += i_Temp_Length;
				}
				if(iTotalLength< (nPaperWidth_temp  - (nPaperWidth * 0.04)))
				{
					if(i_x_shift + iTotalLength > nPaperWidth_temp)
					{
						i_Current_Line = i_Current_Line + 1; 
						nLineSpacing = nDefaultlinespacing;
						i_x_shift = rcPrint.left;
					}
				}
			}
		}
		int i_temp_width = 0;
		
		if (str_temp_convert == L'\r\n' || str_temp_convert == L'\n')
		{
			str_temp_convert = "";
		}
		else if (!bWord)
			pDC->GetCharWidthW(str_Text[i],str_Text[i], &i_temp_width);
		else
		{
			pDC->GetCharWidthW(str_Text[i+1],str_Text[i+1], &i_temp_width);
			i_temp_width *=2;
		}

		if (str_temp_convert == " ") 
			b_space = true;

	
//		COLORREF rgbSave = pDC->GetBkColor();
		int nPaperHeight = pPrintOption->rcPrint.bottom - pPrintOption->rcPrint.top;

		int i_start_print_y = nPaperHeight * 0.1;

		// line number 
		if (i_x_shift == rcPrint.left)
		{
			if (bPrintLineNum)
			{
				CString strLineNum;
				strLineNum.Format(L"%ld", i_Current_Line + 1);
				//pDC->TextOut(i_x_shift, (i_Current_Line * 110) + i_start_print_y , strLineNum); 
		// Edit Stephen 2013-10-28
				pDC->TextOut(i_x_shift, (i_Current_Line * (ContextSize + nLineSpacing)) + i_start_print_y , strLineNum); 
		// Edit Stephen 2013-10-28
			}
		}

		int i_line_num_shift = 0;
		if (bPrintLineNum)
		{

			i_line_num_shift = nPaperWidth * 0.04;
			//pDC->TextOut(i_x_shift+i_line_num_shift, (i_Current_Line * 110) + i_start_print_y , str_temp_convert); 
		// Edit Stephen 2013-10-28
			pDC->TextOut(i_x_shift+i_line_num_shift, (i_Current_Line * (ContextSize + nLineSpacing)) + i_start_print_y , str_temp_convert); 
		// Edit Stephen 2013-10-28
		}
		else
			//pDC->TextOut(i_x_shift, (i_Current_Line * 110) + i_start_print_y , str_temp_convert); 
		// Edit Stephen 2013-10-28
			pDC->TextOut(i_x_shift, (i_Current_Line * (ContextSize + nLineSpacing)) + i_start_print_y , str_temp_convert); 
		// Edit Stephen 2013-10-28

		
		//		pDC->SetBkColor(rgbSave);

//		int i_tab_width = 72; 
//		int i_tab_index = i_x_shift/i_tab_width;

		// Handle Tab char
//		if (str_Text[i] ==  L'\t')
//				i_temp_width = ((i_tab_index+1) * i_tab_width ) - i_x_shift;	
		i_x_shift = i_x_shift + i_temp_width;

		int itemp = 0;
		if(i<i_page_end-1)
		{
			CString strtemp = str_Text[i+1];
			pDC->GetCharWidthW(str_Text[i+1],str_Text[i+1], &itemp);
		}
		
		if (i_x_shift >= (nPaperWidth_temp - i_line_num_shift) || str_Text[i] == L'\r\n' || (i_x_shift >= (nPaperWidth_temp - i_line_num_shift) && b_space)||i_x_shift+itemp >= (nPaperWidth - i_line_num_shift))
		{
			i_Current_Line = i_Current_Line + 1; 
			i_x_shift = rcPrint.left;
			if (i+1 < str_Text.GetLength() && str_Text[i] != L'\r\n' && str_Text[i+1] == L'\r\n')
				i++;
		}
		b_space = false;
	}
//	PrintOption.iStartChar = i;

	// a Line
//	pDC->FillSolidRect(50, 440, nPaperWidth+50, 15,  RGB(0, 0, 0));

}

LONG editView_Bottom::OnFindReplace(WPARAM wParam,LPARAM lParam) 
{ 
    CFindReplaceDialog* pDlg = CFindReplaceDialog::GetNotifier(lParam); 
  
	if (pDlg->IsTerminating())
	{
		m_bOpenFindDialog = false; 
		this->SetFocus();
		return 0;
	}	
	
	
	CString m_FindString = pDlg->GetFindString();        
    CString  m_ReplaceString = pDlg->GetReplaceString();  

	if (pDlg->ReplaceCurrent() || pDlg->ReplaceAll())
	{
		if (m_FindString == m_ReplaceString)
			return 0;
	}


	 int   nStartChar,   nEndChar;   
	GetEditCtrl().GetSel(nStartChar,   nEndChar);   
    
	int pos, i_Select_Text_Len, nLen;
	CString str_Select_Text = GetActualSelectText(pos, i_Select_Text_Len, nLen);
	CString temp = str_Select_Text;

	bool b_skip_replace_current = false;

	if (pDlg->ReplaceCurrent())
	{
	
		if (!pDlg->MatchCase())
		{
			str_Select_Text.MakeLower();
			m_FindString.MakeLower();
		}


		if (pDlg->MatchWholeWord())
		{

    		CString strText = L"";
			GetWindowText(strText);

			int i_right_index = nStartChar + m_FindString.GetLength();
			int i_left_index = nStartChar - 1;
				
					if (i_right_index <= strText.GetLength() -1)
					{
						CString strTemp = strText.GetAt(i_right_index); 
						if (strTemp != L" " && strTemp != char(13) && strTemp != char(10))
							b_skip_replace_current = true;
					}

					if (i_left_index >= 0 )
					{
						CString strTemp = strText.GetAt(i_left_index); 
						if (strTemp != L" " && strTemp != char(13) && strTemp != char(10))
							b_skip_replace_current = true;

					}
		
		
		}

	}

	if (pDlg->ReplaceAll())
	{
    	CString strText = L"";
		GetWindowText(strText);		
		temp = strText;

		if (!pDlg->MatchCase())
		{
			strText.MakeLower();
			m_FindString.MakeLower();
			str_Select_Text.MakeLower();
		}

		int i_index = strText.Find(m_FindString,nStartChar);
		temp = temp.Mid(i_index, m_FindString.GetLength());

		int i_find_start = -1;

		while (i_index >= 0 )
		{

			i_find_start = i_index;

			bool b_skip = false;

			if (pDlg->MatchWholeWord())
			{
				

					int i_right_index = i_index + m_FindString.GetLength();
					int i_left_index = i_index - 1;
				
					if (i_right_index <= strText.GetLength() -1)
					{
						CString strTemp = strText.GetAt(i_right_index); 
						if (strTemp != L" " && strTemp != char(13) && strTemp != char(10))
							b_skip = true;
					}

					if (i_left_index >= 0 )
					{
						CString strTemp = strText.GetAt(i_left_index); 
						if (strTemp != L" " && strTemp != char(13) && strTemp != char(10))
							b_skip = true;
					}

					if (b_skip)
						i_find_start = i_index + m_FindString.GetLength();
			}
			
			if (!b_skip)
			{
			
				int nCharIndex = i_index + m_FindString.GetLength() ;
				int nCharIndex_real = GetActualPos(nCharIndex - m_FindString.GetLength());
				int nTopStart = m_pDoc->GetMarkPosition(GetActualPos(nCharIndex-m_FindString.GetLength()), true);
				int nTopEnd = m_pDoc->GetMarkPosition(GetActualPos(nCharIndex), true);

				CString strStart, strEnd;
				strStart = GetTextFromPoint(nCharIndex-m_FindString.GetLength(),true);
				strEnd = GetTextFromPoint(nCharIndex,false);

				CEdit& edit = GetEditCtrl();
				SetWindowText(strStart + m_ReplaceString + strEnd);
				int pos=GetScrollPos(SB_VERT);
				edit.LockWindowUpdate();
				edit.LineScroll(pos);
				edit.UnlockWindowUpdate();

				UNDOREDOLOG url;
				url.strRedoUndo_Top = m_pDoc->m_strVersion.Mid(nTopStart, nTopEnd - nTopStart);
				url.strMark = m_pDoc->m_strVersion_Mark;
			    url.strComment = m_pDoc->m_strVersion_Comment;
			    url.strStyle = m_pDoc->m_strVersion_Style;
				m_pDoc->DeleteTextVersionPos(nCharIndex_real, m_FindString.GetLength());

				CString str_buffer = m_ReplaceString;
				str_buffer.Replace(L"\n",L""); 

				m_pDoc->UpdateTextVersionPos(str_buffer, nCharIndex_real, false);  
				if(bFirstType==TRUE)
				{
					SetTextToUpperView();
					bFirstType=FALSE;
				}

				url.bIsAdd = false;
				url.lSeq = 0;

				url.strRedoUndo = str_buffer;
				url.lLen = str_buffer.GetLength();
				url.lStartPos = nCharIndex-m_FindString.GetLength();

				url.bExtra = true;
				url.strRedoUndo_Extra = temp;	
				
				m_pDoc->m_arrUndo.Add(url);
				m_pDoc->m_arrRedo.RemoveAll(); 
				m_pDoc->SetModifiedFlag(TRUE);

				GetWindowText(strText);

				if (!pDlg->MatchCase())
					strText.MakeLower();

				i_find_start = i_index + m_ReplaceString.GetLength();
				
			}
			
			i_index = strText.Find(m_FindString, i_find_start);

		}
		if (str_Select_Text.Find(m_FindString)>-1)
			GetEditCtrl().SetSel(nStartChar, nEndChar + 
				(m_ReplaceString.GetLength() - m_FindString.GetLength()) * str_Select_Text.Replace(m_FindString,m_FindString));
		else
			GetEditCtrl().SetSel(nStartChar, nEndChar);

	}
	else if (pDlg->ReplaceCurrent() && m_FindString == str_Select_Text && !b_skip_replace_current)
	{
			int nCharIndex = nEndChar;
			int nCharIndex_real = GetActualPos(nCharIndex - m_FindString.GetLength());
			int nTopStart = m_pDoc->GetMarkPosition(GetActualPos(nCharIndex-m_FindString.GetLength()), true);
			int nTopEnd = m_pDoc->GetMarkPosition(GetActualPos(nCharIndex), true);

			
			CString strStart, strEnd;
			strStart = GetTextFromPoint(nCharIndex-nLen,true);
			strEnd = GetTextFromPoint(nCharIndex,false);

			CEdit& edit = GetEditCtrl();
			SetWindowText(strStart + m_ReplaceString + strEnd);
			int pos=GetScrollPos(SB_VERT);
			edit.LockWindowUpdate();
			edit.LineScroll(pos);
			edit.UnlockWindowUpdate();

			UNDOREDOLOG url;
			url.strRedoUndo_Top = m_pDoc->m_strVersion.Mid(nTopStart, nTopEnd - nTopStart);
			url.strMark = m_pDoc->m_strVersion_Mark;
			url.strComment = m_pDoc->m_strVersion_Comment;
			url.strStyle = m_pDoc->m_strVersion_Style;
			m_pDoc->DeleteTextVersionPos(nCharIndex_real, m_FindString.GetLength());  

		    CString str_buffer = m_ReplaceString;
			str_buffer.Replace(L"\n",L""); 

			m_pDoc->UpdateTextVersionPos(str_buffer, nCharIndex_real, false);  
			if(bFirstType==TRUE)
			{
				SetTextToUpperView();
				bFirstType=FALSE;
			}


			url.bIsAdd = false;
			url.lSeq = 0;

			url.strRedoUndo = str_buffer;
			url.lLen = str_buffer.GetLength();
			url.lStartPos = nStartChar;

			url.bExtra = true;
			url.strRedoUndo_Extra = temp;	
			
			m_pDoc->m_arrUndo.Add(url);
			m_pDoc->m_arrRedo.RemoveAll(); 
			m_pDoc->SetModifiedFlag(TRUE);

			 GetEditCtrl().SetSel(nStartChar, nStartChar+ m_ReplaceString.GetLength()  );
	}
	else if( pDlg->FindNext() || pDlg->ReplaceCurrent())          
    { 

    	CString strText = L"";
		GetWindowText(strText);

		if (!pDlg->MatchCase())
		{
			strText.MakeLower();
			m_FindString.MakeLower();
		}

//		if (pDlg->MatchWholeWord())
//		{
//		}


		int i_index = -1;
		if (pDlg->SearchDown())
		{
			i_index = strText.Find(m_FindString, nEndChar);
			if (i_index == -1)
				i_index = strText.Find(m_FindString, 0);

			if (pDlg->MatchWholeWord())
			{
				bool b_skip = false;
				bool b_Find = false;

				while (!b_Find)
				{

					int i_right_index = i_index + m_FindString.GetLength();
					int i_left_index = i_index - 1;
				
					if (i_right_index <= strText.GetLength() -1)
					{
						CString strTemp = strText.GetAt(i_right_index); 
						if (strTemp != L" " && strTemp != char(13) && strTemp != char(10))
							b_skip = true;
					}

					if (i_left_index >= 0 )
					{
						CString strTemp = strText.GetAt(i_left_index); 
						if (strTemp != L" " && strTemp != char(13) && strTemp != char(10))
							b_skip = true;

					}

					if (b_skip && i_index >= 0)
					{
							i_index = strText.Find(m_FindString, i_index + m_FindString.GetLength()); 
							b_skip = false;
					}
					else
						break;
				}
			}

		}
		else
		{
			/*bool b_Find = false;
			int i_searchindex = 0;
			while (!b_Find)
			{
				i_searchindex = strText.Find(m_FindString, i_searchindex); 

				if (i_searchindex >= nStartChar || i_searchindex == -1)
				{

					if (pDlg->MatchWholeWord() && i_searchindex >= nStartChar)
					{

						bool b_skip = false;
						int i_right_index = i_index + m_FindString.GetLength();
						int i_left_index = i_index - 1;
					
						if (i_right_index <= strText.GetLength() -1)
						{
							CString strTemp = strText.GetAt(i_right_index); 
							if (strTemp != L" " && strTemp != char(13) && strTemp != char(10))
								b_skip = true;
						}

						if (i_left_index >= 0 )
						{
							CString strTemp = strText.GetAt(i_left_index); 
							if (strTemp != L" " && strTemp != char(13) && strTemp != char(10))
								b_skip = true;

						}

						if (!b_skip)
							b_Find = true;
						else
						{
							i_index = i_searchindex;
							i_searchindex = i_searchindex + m_FindString.GetLength();
						}

					}
					else
						b_Find = true;

				}
				else
				{
					i_index = i_searchindex;
					i_searchindex = i_searchindex + m_FindString.GetLength();
				}
			}*/
			i_index = nStartChar;
			CString strReverse=strText.MakeReverse();
			CString strTextReverse=m_FindString.MakeReverse();

			i_index=strReverse.Find(strTextReverse,strText.GetLength()-i_index);
			
			if (i_index!=-1)
			{
				nEndChar=strText.GetLength()-i_index-1;
			}
			else
			{
				i_index=strReverse.Find(strTextReverse,0);
				nEndChar=strText.GetLength()-i_index-1;
			}
			i_index = nEndChar + 1 - m_FindString.GetLength();
		}

		if (i_index>=0)
			 GetEditCtrl().SetSel(i_index, i_index+ m_FindString.GetLength()  );

	} 


  return 0; 
}

void editView_Bottom::OnOptionEdit()
{
	// TODO: Add your command handler code here
	COptionSheet sheet(L"編輯器參數設置");
	sheet.DoModal();
} 

void editView_Bottom::OnOptionChange()
{
	// TODO: Add your command handler code here
	UpdateFontSet();
	GetEditCtrl().SetFont(m_ppFont[0]);
	ChangeUpperViewFont();	
	AutoSaveFileToLocal();
//	CClientDC dc(pwndItem);
} 


void editView_Bottom::UpdateFontSetTemp(CString strFontName)
{
	int nStyle;

	if (m_bFontCreated)
	{
		m_bFontCreated = FALSE;

		for(nStyle=0; nStyle < m_nStyleNum; nStyle++)
		{
			m_ppFont[nStyle]->DeleteObject();
			delete m_ppFont[nStyle];
		}
	}
	m_bFontCreated = TRUE;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	m_nStyleNum = GetPrivateProfileIntW( L"EditPro" , L"TextStyleNum",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//m_nStyleNum = AfxGetApp()->GetProfileInt(L"EditPro", L"TextStyleNum", 1);
	//end 20120927
//	char szSection[TMPSTR_SIZE];
	CString szSection;

	
	CWinApp* pApp = AfxGetApp();
	LOGFONT logfont;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	int nLogicalPixelX = GetPrivateProfileIntW( L"EditPro" , L"LogicalPixelX",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	int nLogicalPixelY = GetPrivateProfileIntW( L"EditPro" , L"LogicalPixelY",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//int nLogicalPixelX = pApp->GetProfileInt(L"EditPro", L"LogicalPixelX", 1 );
	//int nLogicalPixelY = pApp->GetProfileInt(L"EditPro", L"LogicalPixelY", 1 );
	//end 20120927

//	CClientDC dc(pWnd);
//	CDC* pDC = &dc;
	for(nStyle=0; nStyle < m_nStyleNum; nStyle ++)
	{
		//sprintf((char *)szSection, (const char *)"EditProTextStyle%d", nStyle+1);
		szSection.Format(L"EditProTextStyle%ld", nStyle+1);
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		m_pnLineSpace[nStyle] =GetPrivateProfileIntW(szSection , L"LineSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		m_pnCharSpace[nStyle] =GetPrivateProfileIntW(szSection , L"CharSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		/*m_pnLineSpace[nStyle] = pApp->GetProfileInt( szSection, L"LineSpace", 1 ); 		
		m_pnCharSpace[nStyle] = pApp->GetProfileInt( szSection, L"CharSpace", 1 );*/


		CString str_temp ;//= pApp->GetProfileString( szSection, L"Height", L"-15" );
		wchar_t sz_temp[255];
		GetPrivateProfileStringW(szSection , L"Height",L"-15",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		logfont.lfHeight = _wtol(str_temp);

		//str_temp = pApp->GetProfileString( szSection, L"Width", L"0" );
		GetPrivateProfileStringW(szSection , L"Width",L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		logfont.lfWidth  = _wtol(str_temp);

		GetPrivateProfileStringW(szSection , L"Escapement",L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Escapement", L"0" );
		logfont.lfEscapement = _wtol(str_temp);

		GetPrivateProfileStringW(szSection , L"Orientation",L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Orientation", L"0" );
		logfont.lfOrientation = _wtol(str_temp);

		GetPrivateProfileStringW(szSection , L"Weight",L"400",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Weight", L"400" );
		logfont.lfWeight = _wtol(str_temp);

		logfont.lfItalic = GetPrivateProfileIntW(szSection , L"Italic",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Italic", 0 );
		logfont.lfUnderline = GetPrivateProfileIntW(szSection , L"Underline",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Underline", 0 );
		logfont.lfStrikeOut = GetPrivateProfileIntW(szSection , L"StrikeOut",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"StrikeOut", 0 );

		logfont.lfCharSet = GetPrivateProfileIntW(szSection , L"CharSet",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"CharSet", 0 );
		logfont.lfOutPrecision = GetPrivateProfileIntW(szSection , L"OutPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"OutPrecision", 0 );
		logfont.lfClipPrecision = GetPrivateProfileIntW(szSection , L"ClipPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"ClipPrecision", 0 );

		logfont.lfQuality = GetPrivateProfileIntW(szSection , L"Quality",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Quality", 0 );
		logfont.lfPitchAndFamily = GetPrivateProfileIntW(szSection , L"PitchAndFamily",49,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"PitchAndFamily", 49 );
		
		if (nStyle == 0)
		{
			wcscpy(logfont.lfFaceName, strFontName);
			WritePrivateProfileStringW(L"WordCheck", L"FaceName", strFontName.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
		}
		else
		{
			GetPrivateProfileStringW(szSection , L"FaceName",L"Courier New",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
			str_temp = sz_temp;
			//str_temp = pApp->GetProfileString(szSection, L"FaceName", L"Courier New");
			wcscpy(logfont.lfFaceName, str_temp);
		}
		//end 20120927
		// scale log size
		/*logfont.lfHeight = MulDiv(logfont.lfHeight,
			pDC->GetDeviceCaps(LOGPIXELSY),
			nLogicalPixelY);
		logfont.lfWidth = MulDiv(logfont.lfWidth,
			pDC->GetDeviceCaps(LOGPIXELSX),
			nLogicalPixelX);
		
		m_pnLineSpace[nStyle] = MulDiv(m_pnLineSpace[nStyle],
			pDC->GetDeviceCaps(LOGPIXELSY),
			nLogicalPixelY);
		m_pnCharSpace[nStyle] = MulDiv(m_pnCharSpace[nStyle],
			pDC->GetDeviceCaps(LOGPIXELSX),
			nLogicalPixelX);*/
		
		m_ppFont[nStyle] = new CFont;
 		m_ppFont[nStyle]->CreateFontIndirect(&logfont);
	}

	GetEditCtrl().SetFont(m_ppFont[0]);
}

void editView_Bottom::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==1001){
		if (m_b_allow_edit)
		{
			m_fileName = m_pDoc->m_strTmpFileCode;	
//			AfxMessageBox(L"Auto Save");
			m_pDoc->AutoSaveDocumentLocal();
			SetTimer(1001,m_autoSaveDuration*1000*60,0);
		}
	}

	if(nIDEvent == 1002)
	{
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
		editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();
		if(pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))
		{
			pView->SetTextToUpperView();
			SetTimer(1002,m_nSpanSec*1000,0);
		}
	}

	if(nIDEvent == 1003)
	{
		SetTextToUpperView();
		KillTimer(1003);
		CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
		CDocument* pDoc = pChild->GetActiveDocument();   
		ASSERT(pDoc != NULL);   
   
		CView* pView;
		POSITION pos = pDoc->GetFirstViewPosition();   
		while (pos != NULL)   
		{   
			pView = pDoc->GetNextView(pos);   
			if (pView->IsKindOf(RUNTIME_CLASS(editView_Top_RichEdit)))   
			{   
				//pView->GetParentFrame()->ActivateFrame();   
				break;   
			}   
		}
		if(pView!=NULL){
			CRect rc;pView->GetWindowRect(rc);
			if(rc.top !=rc.bottom)
				SetTimer(1002,m_nSpanSec*1000,0);
		}
	}
	CWnd::OnTimer(nIDEvent);
}

void editView_Bottom::WordBankReplace(ARRAY_REPLACEWORD *arrList)
{
	int nBuffer=0,nPos=0,nReturnPos,nCurPos=0;
	CString strText,str;
	GetWindowText(strText);
	//m_pDoc->m_bWordBank = TRUE;
	for (int i=0;i<arrList->GetSize();i++)
	{
		if(arrList->GetAt(i).nPosition<nPos)
			nCurPos=nBuffer=0;
		nPos=arrList->GetAt(i).nPosition;
		str=strText.Left(nPos);

		nReturnPos=str.Find(L"\r\n",nCurPos);
		while (nReturnPos>=0 && nReturnPos<nPos)
		{
			nCurPos=nReturnPos+2;
			nBuffer++;
			nReturnPos=str.Find(L"\r\n",nCurPos);
		}
		m_pDoc->DeleteTextVersionPos(nPos-nBuffer, lstrlen(arrList->GetAt(i).strOrgText));  
		m_pDoc->UpdateTextVersionPos(arrList->GetAt(i).strTrgText,nPos-nBuffer,false);
	} 
	//m_pDoc->m_bWordBank = FALSE;
	if(m_pDoc->m_bEditorLog==TRUE)
		m_pDoc->OnLogFile(L"Work bank");
	if(bFirstType==TRUE)
	{
		SetTextToUpperView();
		bFirstType=FALSE;
	}
	m_pDoc->m_arrUndo.RemoveAll();
	m_pDoc->m_arrRedo.RemoveAll();
	m_pDoc->SetModifiedFlag(TRUE);

}

void editView_Bottom::ChangLineSpace(){
/*	CWnd* pwndItem = GetDlgItem(IDC_FONTPREVIEW);
	CClientDC dc(pwndItem);

	
	CString str;
	int *pnWidths, i, nLen;
	
	str = L"方正 Founder";
//	str += "(" + m_parrayStyleName->GetAt(m_nCurStyle) + ")";
	//原來的版本中﹐計算中文字符寬度有問題。
	//如果字體是英文語系的話﹐則用
	
	
	//計算沒有問題﹐但是如果字體是中文語系的話﹐則錯誤。
	//所以調用int nChinese= dc.GetTextExtent("方").cx;來計算。
	//9	int nChinese= dc.GetTextExtent(L"方").cx;
	nLen = str.GetLength();
	pnWidths = new int[nLen];
	for (i=0; i < nLen; i ++)
		dc.GetCharWidth(str.GetAt(i), str.GetAt(i), &pnWidths[i]);
		/*		
		// add by Bob
		// 中文字符計算
		//		if ((BYTE)str.GetAt(i)>(BYTE)ASCII_END)
		//		{
		CButton *pDisplay=(CButton *)(GetDlgItem(IDC_RADIODISP));
		if(pDisplay->GetCheck())
		pnWidths[i]=nChinese+m_pnCharSpace[m_nCurStyle];
		else
		pnWidths[i]=nChinese+m_pnCharSpaceP[m_nCurStyle];
		}
	*/
/*	RECT rc;
	pwndItem->GetClientRect(&rc);
	pwndItem->InvalidateRect(NULL);
	pwndItem->UpdateWindow();
	dc.SetBkMode(TRANSPARENT);
	//CButton *pDisplay=(CButton *)(GetDlgItem(IDC_RADIODISP));
	if(pDisplay->GetCheck())
		dc.ExtTextOut(m_pnCharSpace[m_nCurStyle], m_pnLineSpace[m_nCurStyle], 
		ETO_CLIPPED, &rc, str, pnWidths);
	else
		dc.ExtTextOut(m_pnCharSpaceP[m_nCurStyle], m_pnLineSpaceP[m_nCurStyle], 
		ETO_CLIPPED, &rc, str, pnWidths);
	delete []pnWidths;
	dc.SelectObject(pfontSave);*/
}

void editView_Bottom::AutoSaveFileToLocal(){
	CWinApp* pApp = AfxGetApp();
	GetPrivateProfileStringW(L"EditPro", L"AutoSave", L"0",m_isAutoSave.GetBufferSetLength(10),10,AfxGetApp()->m_pszProfileName);
	int nInterval;
	if(m_isAutoSave !=L"0")
	{
		nInterval=GetPrivateProfileIntW(L"EditPro", L"AutoSaveTime", 5,AfxGetApp()->m_pszProfileName);
		if (nInterval==m_autoSaveDuration)
			return;
		else {
			m_autoSaveDuration=nInterval;
			KillTimer(1001);
			SetTimer(1001,m_autoSaveDuration*1000*60,0);
			}
	}
	else KillTimer(1001);
}

BOOL editView_Bottom::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL bPreCreated = CEditView::PreCreateWindow( cs );
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);
	return bPreCreated;
}

void editView_Bottom::OnInsertSpaceOnFront()
{
	if (!m_b_allow_edit) return;
	CString strSelectText = L""; 

	int   nStartChar,   nEndChar;   
	GetEditCtrl().GetSel(nStartChar,  nEndChar);   
	ASSERT((UINT)nEndChar   <=   GetBufferLength());   
	int pos, i_Select_Text_Len, nLen;
	strSelectText = GetActualSelectText(pos, i_Select_Text_Len, nLen);
	bool b_Select_Text = false;
	if (i_Select_Text_Len > 0) b_Select_Text = true;
	
	CString strText = L"";
	strText = L"\r";
	int m_bAutoSpaceAfterReturn, m_nAutoSpacesAfterReturn = 0;
	m_bAutoSpaceAfterReturn = GetPrivateProfileIntW( L"EditPro" , L"AutoSpaceAfterReturn",0,((CWinApp*)AfxGetApp())->m_pszProfileName);

	if ( m_bAutoSpaceAfterReturn )
	{
		m_nAutoSpacesAfterReturn = GetPrivateProfileIntW( L"EditPro" , L"AutoSpacesAfterReturn",4,((CWinApp*)AfxGetApp())->m_pszProfileName);
		int i ;
		for ( i = 0 ; i < m_nAutoSpacesAfterReturn ; i ++)
		{
			strText+=" ";
		}
	}/*else
		return;*/

    int nCharIndex = nEndChar;
	int nCharIndex_real = GetActualPos(nCharIndex - nLen);
	int nTopStart = m_pDoc->GetMarkPosition(GetActualPos(nCharIndex-nLen), true);
	int nTopEnd = GetActualPos(nCharIndex);//m_pDoc->GetMarkPosition(GetActualPos(nCharIndex), true);
	if(i_Select_Text_Len == 0)
		nTopEnd = nTopStart;

	CString strStart, strEnd;
	
	strStart = GetTextFromPoint(nCharIndex-nLen,true);
	strEnd = GetTextFromPoint(nCharIndex,false);

	UNDOREDOLOG url;
	url.strRedoUndo_Top = m_pDoc->m_strVersion.Mid(nTopStart, nTopEnd - nTopStart);
	url.strMark = m_pDoc->m_strVersion_Mark;
	url.strComment = m_pDoc->m_strVersion_Comment;
	url.strStyle = m_pDoc->m_strVersion_Style;

	CString strTemp1,strTemp2;
	GetWindowText(strTemp1);		
	CView::OnChar(0, 0, 0);
	GetWindowText(strTemp2);
	if (strTemp1==strTemp2)
		return;
	m_pDoc->DeleteTextVersionPos(nCharIndex_real, i_Select_Text_Len);  
	
	m_pDoc->UpdateTextVersionPos(strText, nCharIndex_real, false); 
   
	strText.Replace(L"\r",L"\r\n");

	CEdit& edit = GetEditCtrl();
	int pos1 = edit.GetScrollPos(SB_VERT);
	edit.LockWindowUpdate();
	SetWindowText(strStart + strText + strEnd); 
	if(bFirstType==TRUE)
	{
		SetTextToUpperView();
		bFirstType=FALSE;
	}

	
	//edit.SetSel(nCharIndex+1, nCharIndex+1);
	int iSelTextNewLine = strSelectText.Replace(L"\r\n",L"\r\n");
	edit.LineScroll(pos1);
	edit.SetSel(nCharIndex+m_nAutoSpacesAfterReturn+2-i_Select_Text_Len-iSelTextNewLine, nCharIndex+m_nAutoSpacesAfterReturn+2-i_Select_Text_Len-iSelTextNewLine);
	edit.UnlockWindowUpdate();

	// add a record to undo List 
	url.bIsAdd = false;
	url.lSeq = 0;
	url.strRedoUndo = strText;
	url.lLen = m_nAutoSpacesAfterReturn+1;
//	url.lStartPos = nCharIndex;
	url.lStartPos = nStartChar;

	if (b_Select_Text)
	{
		url.bExtra = true;
		url.strRedoUndo_Extra = strSelectText;	
	}
	else
	{
		url.bExtra = false;
		url.strRedoUndo_Extra = "";	
	}	
	m_pDoc->m_arrUndo.Add(url);
	m_pDoc->m_arrRedo.RemoveAll(); 
	m_pDoc->SetModifiedFlag(TRUE);

	if (m_pDoc)
		DisplayDocInfo();
}

void editView_Bottom::OnChangeUpperView()
{
	BOOL bIsCommentMode;
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();

	bIsCommentMode = ((editFrameNew*)pChild)->GetUpperViewMode();

	if(!bIsCommentMode)
	{
		//OnEditReadOnly();
		KillTimer(1002);
		bUpperStartedFlag = TRUE;
		((editFrameNew*)pChild)->SetUpperViewEditDoc(m_pDoc);
		((editFrameNew*)pChild)->SetUpperViewMode(!bIsCommentMode);
		CString str; GetWindowTextW(str);
		//((editFrameNew*)pChild)->SetTextToUpperView(str, L"", L"");
		((editFrameNew*)pChild)->SetTextToUpperView(str,m_pDoc->m_strVersion_Mark, m_pDoc->m_strModHistory);
		//SetTimer(1002,m_nSpanSec*1000,0);
	}
	else
	{
		//OnEditReadOnly();
		//KillTimer(1002);
		((editFrameNew*)pChild)->SetUpperViewMode(!bIsCommentMode);
		((editFrameNew*)pChild)->SetTextToUpperView(m_pDoc->m_strVersion, m_pDoc->m_strVersion_Mark, m_pDoc->m_strModHistory);
		//SetTimer(1002,m_nSpanSec*1000,0);
	}
}

CFont* editView_Bottom::GetPrintFontSize()
{
	int m_nStyleNum, nStyle;
	m_nStyleNum = GetPrivateProfileIntW(L"EditPro", L"TextStyleNum", 1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//AfxGetApp()->GetProfileInt(L"EditPro", L"TextStyleNum", 1);
	wchar_t szSection[TMPSTR_SIZE];
	CWinApp* pApp = AfxGetApp();
	LOGFONT logfont;
	CFont *m_ppFont1[MAX_TEXTSTYLE_NUM];
	int nLogicalPixelX = GetPrivateProfileIntW(L"EditProPrint", L"LogicalPixelX", 1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->GetProfileInt(L"EditProPrint", L"LogicalPixelX", 1 );
	int nLogicalPixelY = GetPrivateProfileIntW(L"EditProPrint", L"LogicalPixelY", 1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->GetProfileInt(L"EditProPrint", L"LogicalPixelY", 1 );

	for(nStyle=0; nStyle < m_nStyleNum; nStyle ++)
	{
		swprintf(szSection, L"EditProPrintTextStyle%d", nStyle+1);

		m_pnLineSpace[nStyle] =  GetPrivateProfileIntW((LPCTSTR)szSection, L"LineSpace", 1,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( (LPCTSTR)szSection, L"LineSpace", 1 );
		m_pnCharSpace[nStyle] =  GetPrivateProfileIntW((LPCTSTR)szSection, L"CharSpace", 1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->GetProfileInt( (LPCTSTR)szSection, L"CharSpace", 1 );

		wchar_t sz_temp[255];
		GetPrivateProfileStringW((LPCTSTR)szSection, L"Height", L"-15",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		CString str_temp = sz_temp;
			//pApp->GetProfileString( (LPCTSTR)szSection, L"Height", L"-15" );
		const wchar_t * wc_tmp = (LPCTSTR) str_temp;
		//logfont.lfHeight = atol((const char *)wc_tmp);
		logfont.lfHeight = _wtol(wc_tmp);

		GetPrivateProfileStringW((LPCTSTR)szSection, L"Width", L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
			//pApp->GetProfileString( (LPCTSTR)szSection, L"Width", L"0" );
		wc_tmp = (LPCTSTR) str_temp;
		//logfont.lfWidth  = atol((const char *)wc_tmp);
		logfont.lfWidth	= _wtol(wc_tmp);

		GetPrivateProfileStringW((LPCTSTR)szSection, L"Escapement", L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
			//pApp->GetProfileString( (LPCTSTR)szSection, L"Escapement", L"0" );
		wc_tmp = (LPCTSTR) str_temp;
		//logfont.lfEscapement = atol((const char *)wc_tmp);
		logfont.lfEscapement	= _wtol(wc_tmp);

		GetPrivateProfileStringW((LPCTSTR)szSection, L"Orientation", L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
			//pApp->GetProfileString( (LPCTSTR)szSection, L"Orientation", L"0" );
		wc_tmp = (LPCTSTR) str_temp;
		//logfont.lfOrientation = atol((const char *)wc_tmp);
		logfont.lfOrientation	= _wtol(wc_tmp);

		GetPrivateProfileStringW((LPCTSTR)szSection, L"Weight", L"400",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
			//pApp->GetProfileString( (LPCTSTR)szSection, L"Weight", L"400" );
		wc_tmp = (LPCTSTR) str_temp;
		//logfont.lfWeight = atol((const char *)wc_tmp);
		logfont.lfWeight	= _wtol(wc_tmp);
		
		logfont.lfItalic = GetPrivateProfileIntW((LPCTSTR)szSection, L"Italic", 0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( (LPCTSTR)szSection, L"Italic", 0 );
		logfont.lfUnderline = GetPrivateProfileIntW((LPCTSTR)szSection, L"Underline", 0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( (LPCTSTR)szSection, L"Underline", 0 );
		logfont.lfStrikeOut = GetPrivateProfileIntW((LPCTSTR)szSection, L"StrikeOut", 0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( (LPCTSTR)szSection, L"StrikeOut", 0 );

		logfont.lfCharSet = GetPrivateProfileIntW((LPCTSTR)szSection, L"CharSet", 0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( (LPCTSTR)szSection, L"CharSet", 0 );
		logfont.lfOutPrecision = GetPrivateProfileIntW((LPCTSTR)szSection, L"OutPrecision", 0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( (LPCTSTR)szSection, L"OutPrecision", 0 );
		logfont.lfClipPrecision = GetPrivateProfileIntW((LPCTSTR)szSection, L"ClipPrecision", 0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( (LPCTSTR)szSection, L"ClipPrecision", 0 );

		logfont.lfQuality = GetPrivateProfileIntW((LPCTSTR)szSection, L"Quality", 0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( (LPCTSTR)szSection, L"Quality", 0 );
		logfont.lfPitchAndFamily = GetPrivateProfileIntW((LPCTSTR)szSection, L"PitchAndFamily", 49,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( (LPCTSTR)szSection, L"PitchAndFamily", 49 );

		GetPrivateProfileStringW((LPCTSTR)szSection, L"FaceName", L"Courier New",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
			//str_temp = pApp->GetProfileString((LPCTSTR)szSection, L"FaceName", L"Courier New");
		wc_tmp = (LPCTSTR) str_temp;
		lstrcpy(logfont.lfFaceName, wc_tmp);
		//end 20120928
		// scale log size
		m_ppFont1[nStyle] = new CFont;
 		m_ppFont1[nStyle]->CreateFontIndirect(&logfont);
	}

	return m_ppFont1[0];
}

void editView_Bottom::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(CheckRangeOnComment())
		;
	else
		CEditView::OnLButtonDblClk(nFlags, point);
}

BOOL editView_Bottom::CheckRangeOnComment()
{
	if(GetEditCtrl().GetWindowTextLengthW()<=0)
		return FALSE;
	CString str, str_VerMark;
	int start, end, displayPosCalc;
	GetEditCtrl().GetSel(start, end);
	start = GetActualPos(start);
	displayPosCalc = GetActualDeletePosCount(start);
	CString p_str_Ver_Comment = m_pDoc->GetPStrVerComment();
	CString str_Ver_Comment = m_pDoc->m_strVersion_Comment;
	start = start<str_Ver_Comment.GetLength()?start:str_Ver_Comment.GetLength();
	int iActualStart = m_pDoc->GetMarkPosition(start, false);
	int iActualEnd= m_pDoc->GetMarkPosition(GetActualPos(end), false);
	int iDiff = iActualStart - start;
	str = str_Ver_Comment.GetAt(iActualStart);
	if(p_str_Ver_Comment.Find(str_Ver_Comment.GetAt(iActualStart))>-1)
	{
		str_VerMark = str_Ver_Comment.GetAt(iActualStart);
		start = str_Ver_Comment.Find(str_Ver_Comment.GetAt(iActualStart))-iDiff;
		end = str_Ver_Comment.ReverseFind(str_Ver_Comment.GetAt(iActualEnd))-iDiff;
		
		GetEditCtrl().SetSel(start+displayPosCalc, end+displayPosCalc+1);
		ShowCommentInToolTip();
		return TRUE;
	}else
		return FALSE;
}

void editView_Bottom::ShowCommentInToolTip()
{
	int start, end, actualPos;
	GetEditCtrl().GetSel(start, end);
	actualPos = GetActualPos(start);

	if(end - start>0)
	{
		 ClientToScreen(&ptLog);
		 CString strTemp;
		 CString strComment = m_pDoc->GetTextVersionComment(actualPos, end - start);
		 strTemp.Format(L"%s", strComment);
		 if(strTemp.GetLength()==0)return;
		 // show tool tip base on mouse point
		 CRect rc;
		 GetWindowRect(rc);
		 m_Tips.ShowTips((ptLog.x + 5)+(200*3)<rc.right?ptLog.x + 5:(ptLog.x + 5)-(200*3), ptLog.y + 25, strTemp);
		 SetFocus();
	}else
		m_Tips.HideTips();
}

void editView_Bottom::OnMouseMove(UINT nFlags, CPoint point) 
{
	ptLog = point;
	CEditView::OnMouseMove(nFlags, point);
}

void editView_Bottom::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEditView::OnLButtonDown(nFlags, point);
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	BOOL bIsCommentMode = ((editFrameNew*)pChild)->GetUpperViewMode();
	if(bIsCommentMode)
		((editFrameNew*)pChild)->KillUpperToopTips();
	m_Tips.HideTips();
}

void editView_Bottom::OnUpdateSwitchMode(CCmdUI* pCmdUI) 
{
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	/*if(((editFrameNew*)pChild)->bScrollView==TRUE)
	{
		pCmdUI->Enable(FALSE); 
		return;
	}*/
	
	CString p_str_Ver_Comment = m_pDoc->GetPStrVerComment();
	CString str_Ver_Comment = m_pDoc->m_strVersion_Comment;
	BOOL bHasComment = FALSE;
	for(int i = 0; i<p_str_Ver_Comment.GetLength();i++)
	{
		if(str_Ver_Comment.Find(p_str_Ver_Comment.GetAt(i))>-1)
		{
			bHasComment = TRUE;
			break;
		}
	}
	/*CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	((editFrameNew*)pChild)->SetUpperViewEditDoc(m_pDoc);
	((editFrameNew*)pChild)->UpdateSwitchModeFlag(bHasComment);*/
	pCmdUI->Enable(bHasComment); 
	//pCmdUI->Enable(FALSE); 
}

void editView_Bottom::KillUpdateUpperTimers()
{
	KillTimer(1002);
}

void editView_Bottom::StartUpdateUpperTimer()
{
	KillTimer(1002);
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
	editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();	
	if(pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))
	{
		pView->SetTextToUpperView();
		SetTimer(1002,m_nSpanSec*1000,0);
	}
}

void editView_Bottom::KILLEditToopTips()
{
	m_Tips.HideTips();
}

BOOL editView_Bottom::GetEditStatus()
{
	return m_b_allow_edit;
}

void editView_Bottom::OnEditSelAll() 
{
	CEdit& edit = GetEditCtrl();
	edit.SetSel(0, -1);
}

void editView_Bottom::OnUpdateAddStory(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	if(CGlobal::GetInstance()->m_pShareData->m_bSupportPubStory == FALSE)
	{
		pCmdUI->Enable(FALSE); 
		return;
	}
	if (m_b_allow_edit)
		pCmdUI->Enable(TRUE); 
	else
		pCmdUI->Enable(FALSE); 
}

void editView_Bottom::OnAddStory() 
{
//
	CGlobal *pGL=CGlobal::GetInstance();
	CString str, strTemp;
	GetWindowText(str);
	strTemp = str.Left(str.Find(L"\r\n")==-1?str.GetLength():str.Find(L"\r\n"));

	// 

	CDlgAddToStory dlg(pGL->m_FileDB.m_pDB);
	dlg.strStoryName = strTemp.GetLength()>80?strTemp.Left(80):strTemp;
	m_pDoc->SaveDocumentLocal();

	CString strTempFileName = m_pDoc->GetPathName() + L".txt";

	if (m_pDoc->m_strTmpFileCode == L"")
		dlg.strFileName = m_pDoc->GetTempFileID();
	else
		dlg.strFileName =  m_pDoc->m_strTmpFileCode;


	dlg.m_strFilePath = strTempFileName;
//	AfxMessageBox(m_pDoc->GetPathName());

	dlg.m_dt = &COleDateTime::GetCurrentTime();
	dlg.m_strDate = dlg.m_dt->Format(DATEFORMAT);

	dlg.m_b_NewText = true;
	//
	if(dlg.DoModal() != IDOK)
		return;

	m_pDoc->m_bIsStoryFile = TRUE; 
	
	m_pDoc->m_FileParam.lStartWords = dlg.m_i_storyItemID; 
	m_pDoc->OnCloseDocument();

	DeleteFile(strTempFileName);
	///
}
