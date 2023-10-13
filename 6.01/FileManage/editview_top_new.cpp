// editView_Top_New.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "editView_Top_New.h"
#include "resource.h"
#include "editview_bottom.h"
#include "OptionModifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		MENUPOS_TEXTSTYLE	2
#define		SB_INDICATORS_NEED	3
#define		SB_STATUS			0


CString p_str_Ver_Del_test_new = L" !@#¡$¢£%¤¥¦^§¨©ª&«¬­®¯°*±²³´µ¶(•¸¹º»¼½)¾¿ÀÁÂÃÄÅ+ÆÇÈÉÊËÌÍÎ{ÏÐÑÒÓÔÕÖ×Ø}ÙÚÛÜÝÞßàáâã<äåæçèéêëìíîï>ðñòóôõö÷øùúûý";
CString p_str_Ver_test_new = L"Aabcdefghijklmno";
CString p_str_Digit_new = L"0123456789abcdefghijklmnopqrstuvwxyz";

CString p_str_Cap_Letter_new = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/////////////////////////////////////////////////////////////////////////////
// editView_Top_New

IMPLEMENT_DYNCREATE(editView_Top_New, CRichEditView)
extern "C" AFX_EXT_API CDocument* WINAPI OpenDocumentNew(LPTSTR lpFileName,void * pParam);
editView_Top_New::editView_Top_New()
{
	sBRUSH.CreateSolidBrush(RGB(255, 255, 255));
	m_pPopupMenu = NULL;
	m_mouseMove = m_mouseDown = FALSE;
	m_selectStart_x = m_selectEnd_x = -1;
	m_selectStart_y = m_selectEnd_y = -1;
	m_max_select_pos = m_min_select_pos = -1;
	sHBR = sBRUSH;
	m_tabSize = 4;
	m_indent = 10;
	m_pStatusBar = NULL;

	m_bFontCreated = FALSE;
	m_nStyleNum = 0;

}

editView_Top_New::~editView_Top_New()
{
	if (m_pPopupMenu)
	{
		m_pPopupMenu->DestroyMenu();
		delete m_pPopupMenu;
		m_pPopupMenu = NULL;
	}

}


BEGIN_MESSAGE_MAP(editView_Top_New, CRichEditView)
	//{{AFX_MSG_MAP(editView_Top_New)
	ON_WM_CREATE()
	ON_WM_CHAR()
//	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
//  N_UPDATE_COMMAND_UI(ID_DO_COMMIT, OnUpdateDoCommit)
	ON_UPDATE_COMMAND_UI(ID_DO_MODIFY, OnUpdateDoModify)
	ON_COMMAND(ID_EDIT_COPY_TEST, OnCopy)
	ON_COMMAND(ID_EDIT_SHOWMODINFO, OnEditShowModHistory)
	ON_COMMAND(ID_OPTION_MODIFY, OnOptionModifyHistory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

bool editView_Top_New::IsSelectedChar(int i_temp)  
{

	if (m_min_select_pos != -1  && m_max_select_pos != -1 )	
	{
		if (i_temp >= m_min_select_pos && i_temp <= m_max_select_pos)
			return true;
		else
			return false;
	}
	else
		return false;
}

CString editView_Top_New::GetActualSelectedText()  
{
	CString strTemp = L"";

	for (int i=0; i < m_str_Text.GetLength(); i++)
	{
		if (m_min_select_pos != -1  && m_max_select_pos != -1 )	
		{
			if (i >= m_min_select_pos && i <= m_max_select_pos)
				strTemp = strTemp + m_str_Text[i];
		}


	}
	return strTemp;
}



bool editView_Top_New::IsDigit_Character(CString strTemp)
{
	if (p_str_Digit_new.Find(strTemp) >= 0)
		return true;	
	else
		return false;	
}

bool editView_Top_New::IsCapLetter_Character(CString strTemp)
{
	if (p_str_Cap_Letter_new.Find(strTemp) >= 0)
		return true;	
	else
		return false;	
}


int editView_Top_New::GetVersion(int i_pos)
{

	CString str_temp = this->m_str_Text_Version[i_pos];

	int i_version_index = p_str_Ver_test_new.Find(str_temp);

	if (i_version_index < 0 )
	{
		double itemp =  p_str_Ver_Del_test_new.Find(str_temp);
		if (itemp > 2)
			itemp = ceil( (sqrt(8*(itemp-1)+1)-1) /2)+1;
		i_version_index = itemp;
	}

	if (i_version_index < 0 )
		i_version_index = 0;

	return i_version_index;	
}


int editView_Top_New::GetVersion_New(int i_pos, bool &b_Del)
{
	b_Del = false;	

	CString str_temp = this->m_str_Text_Version[i_pos];

	int i_version_index = p_str_Ver_test_new.Find(str_temp);

	if (i_version_index < 0 )
	{
		double itemp =  p_str_Ver_Del_test_new.Find(str_temp);
		if (itemp > 2)
			itemp = ceil( (sqrt(8*(itemp-1)+1)-1) /2)+1;
		i_version_index = itemp;
		if (i_version_index >= 0 )
			b_Del = true;	
	}

	if (i_version_index < 0 )
		i_version_index = 0;

	return i_version_index;	
}


/////////////////////////////////////////////////////////////////////////////
// editView_Top_New diagnostics

#ifdef _DEBUG
void editView_Top_New::AssertValid() const
{
	CRichEditView::AssertValid();
}

void editView_Top_New::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// editView_Top_New message handlers

void editView_Top_New::OnInitialUpdate() 
{
	CRichEditView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	EnableScrollBarCtrl(SB_BOTH, FALSE);
	CSize sizeTotal;
//	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 0;
	m_max_height = 0;
	m_max_width = 0;

	m_b_UpdateScroll = false;
//	SetScrollSizes(MM_TEXT, GetDocument()->GetDocGetDocSize( ) );

	UpdateFontSet();
	
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
//	m_cHeight = lf.lfHeight = tm.tmHeight;
	lstrcpy(lf.lfFaceName, _T("Courier New"));
	m_font.CreateFontIndirect(&lf);

}


void editView_Top_New::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRichEditView::OnChar(nChar, nRepCnt, nFlags);
}

void editView_Top_New::SetContentText(CString strText, CString strTextVersion, CString strModHistory)
{
	m_str_Text = strText;
	m_str_Text_Version = strTextVersion;
	m_str_Mod_History = strModHistory;

	InitModHistArray();

	Invalidate();

	this->SetWindowText(m_str_Text); 

//	CSize sizeTotal;
//	sizeTotal.cx = sizeTotal.cy = m_str_Text.GetLength() ;
//	SetScrollSizes(MM_TEXT,  sizeTotal);

}


int editView_Top_New::GetLineNumber(int i_pos, int &i_line_pos) 
{
	int i_Line_Number = 0;
	i_line_pos = 0;
	for (int i=0; i < i_pos; i++)
	{
		i_line_pos  = i_line_pos + 1;
//		if (m_str_Text[i] == L'\r\n' || m_str_Text[i] == L'\0' || i_line_pos > 90 )
		if (m_str_Text[i] == L'\r\n')
		{
			i_line_pos = 0;
			i_Line_Number = i_Line_Number + 1;
		}
	}
	return i_Line_Number;
}

/*
void editView_Top_New::OnUpdateDoCommit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->Enable(FALSE); 
}
*/
void editView_Top_New::OnUpdateDoModify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->Enable(FALSE); 
}


int editView_Top_New::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
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
	m_pPopupMenu->LoadMenu(IDR_MODPOPUPMENU);
	// Append text style menu
	CWinApp* pApp = AfxGetApp();
	CMenu* pSubMenu = (m_pPopupMenu->GetSubMenu(0))->GetSubMenu(MENUPOS_TEXTSTYLE);
	// Remove the tempory item
	pSubMenu->RemoveMenu(0, MF_BYPOSITION); 
	int i, nStyleNum;
	char szSection[TMPSTR_SIZE];
	CString str;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	nStyleNum = GetPrivateProfileIntW( L"EditPro" , L"TextStyleNum",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//nStyleNum = pApp->GetProfileInt(L"EditPro", L"TextStyleNum", 1);
	//end 20120927
	for (i=0; i < nStyleNum; i ++)
	{
		sprintf(szSection, "EditProTextStyle%d", i+1);
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
			GetPrivateProfileStringW( (LPWSTR)szSection,L"StyleName" , L"Normal",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
			str = sz_temp;
			//str = pApp->GetProfileString( (LPWSTR)szSection, L"StyleName", L"Normal");
			//end 20120927
			break;
		}
		pSubMenu->AppendMenu(MF_STRING, ID_TS_STYLE1+i, str);
	}

	return 0;
}

void editView_Top_New::OnRButtonDown(UINT nFlags, CPoint point) 
{
//	int nStyle = GetTextStyle(m_lCurrentLine);
	CMenu* pPopup = m_pPopupMenu->GetSubMenu(0);
	CMenu* pSubMenu = pPopup->GetSubMenu(MENUPOS_TEXTSTYLE);

//	pSubMenu->CheckMenuItem(nStyle-1, MF_CHECKED|MF_BYPOSITION);

	ClientToScreen(&point);

		RECT		m_rcWnd;	
	pPopup->TrackPopupMenu(
		TPM_RIGHTBUTTON, point.x, point.y, this, &m_rcWnd);

//	pSubMenu->CheckMenuItem(nStyle-1, MF_UNCHECKED|MF_BYPOSITION);
	CRichEditView::OnRButtonDown(nFlags, point);
}


void editView_Top_New::OnCopy()
{
   // TODO: Add your control notification handler code here
	  CString strSelectText = GetActualSelectedText() ; 

	if (strSelectText.IsEmpty()) return ;

	strSelectText.Replace(L"\r", L"\r\n"); 

	if(OpenClipboard()) //OpenClipboard(NULL) gives me error
	{
		EmptyClipboard();

		HLOCAL clipbuffer = LocalAlloc(0,  strSelectText.GetLength() *   4);
		wcscpy((WCHAR*) clipbuffer, strSelectText);

		SetClipboardData(CF_UNICODETEXT, clipbuffer);
		CloseClipboard();

	}

	return;

}

void editView_Top_New::InitModHistArray() 
{
	int i_ver = 0;
	int i_pos = 0;
	int i_start = 0;

	m_arrModHist.RemoveAll();
	
	CString strTemp = L"";

	while(i_start < m_str_Mod_History.GetLength())
	{
		 i_pos = m_str_Mod_History.Find(L"#", i_start);  
		
		 if  (i_pos < 0) 
		 {
			strTemp = m_str_Mod_History.Mid(i_start);

			m_arrModHist.Add(strTemp);  	
			break;
		 }
		 else
			strTemp = m_str_Mod_History.Mid(i_start, i_pos - i_start);
			m_arrModHist.Add(strTemp);  	

		 i_start =  i_pos + 1;
	}

	CString strModHistory;
	COleDateTime dt = COleDateTime::GetCurrentTime();
	CString strDate; 
	strDate.Format(L"%s",dt.Format(DATETIMEFORMAT));
	strModHistory = CGlobal::GetInstance()->m_pEmployee->m_strUserName + L"於" + strDate;
	m_arrModHist.Add(strModHistory);  	

}

void editView_Top_New::OnEditShowModHistory() 
{

	CString strTemp = L"";
	CString strCurrent = L"";
	CString strAll = L"";

	bool b_Del = false;

	int i_ver = 0;

	for (int i=m_min_select_pos; i < m_str_Text.GetLength(); i++)
	{
		if (m_min_select_pos != -1  && m_max_select_pos != -1 )	
		{
			if (i >= m_min_select_pos && i <= m_max_select_pos)
			{
				i_ver = GetVersion_New(i, b_Del);

				if (b_Del)
					strCurrent.Format(L"#%ldD#",i_ver);  
				else
					strCurrent.Format(L"#%ld#",i_ver);  
				
				if (strAll.Find(strCurrent,0) < 0)
				{				
					strAll = strAll + strCurrent;

					if (m_arrModHist.GetSize() > 0)
					{
						if (i_ver > m_arrModHist.GetSize() - 1) 
							strTemp	=  strTemp	+ L" " + m_arrModHist.GetAt(m_arrModHist.GetSize() - 1);  
						else
							strTemp	=  strTemp	+ L" " + m_arrModHist.GetAt(i_ver);  
					
						if (b_Del)
							strTemp = strTemp + L"刪除";
						else	
							strTemp = strTemp + L"輸入";
					}
				}
			}

		
		}


	}

//	  CString strSelectText = L"User 在 2012-05-07 15:45:55 輸入"; 
	

	m_pStatusBar->SetPaneText(SB_STATUS, strTemp);
	  

}

void editView_Top_New::UpdateFontSetTemp(CString strFontName)
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
	m_nStyleNum= GetPrivateProfileIntW( L"EditPro" , L"TextStyleNum",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
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
		m_pnLineSpace[nStyle] = GetPrivateProfileIntW( szSection , L"LineSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		m_pnCharSpace[nStyle] = GetPrivateProfileIntW( szSection , L"CharSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//m_pnLineSpace[nStyle] = pApp->GetProfileInt( szSection, L"LineSpace", 1 );
		//m_pnCharSpace[nStyle] = pApp->GetProfileInt( szSection, L"CharSpace", 1 );


		CString str_temp;// = pApp->GetProfileString( szSection, L"Height", L"-15" );
		wchar_t sz_temp[255];
		GetPrivateProfileStringW( szSection , L"Height",L"-15" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		logfont.lfHeight = _wtol(str_temp);

		GetPrivateProfileStringW( szSection , L"Width",L"0" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Width", L"0" );
		logfont.lfWidth  = _wtol(str_temp);

		GetPrivateProfileStringW( szSection , L"Escapement",L"0" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Escapement", L"0" );
		logfont.lfEscapement = _wtol(str_temp);

		GetPrivateProfileStringW( szSection , L"Orientation",L"0" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Orientation", L"0" );
		logfont.lfOrientation = _wtol(str_temp);

		GetPrivateProfileStringW( szSection , L"Weight",L"400" ,sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Weight", L"400" );
		logfont.lfWeight = _wtol(str_temp);

		logfont.lfItalic = GetPrivateProfileIntW( szSection , L"Italic",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Italic", 0 );
		logfont.lfUnderline = GetPrivateProfileIntW( szSection , L"Underline",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Underline", 0 );
		logfont.lfStrikeOut = GetPrivateProfileIntW( szSection , L"StrikeOut",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"StrikeOut", 0 );

		logfont.lfCharSet = GetPrivateProfileIntW( szSection , L"CharSet",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"CharSet", 0 );
		logfont.lfOutPrecision = GetPrivateProfileIntW( szSection , L"OutPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"OutPrecision", 0 );
		logfont.lfClipPrecision = GetPrivateProfileIntW( szSection , L"ClipPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"ClipPrecision", 0 );

		logfont.lfQuality =  GetPrivateProfileIntW( szSection , L"Quality",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Quality", 0 );
		logfont.lfPitchAndFamily =  GetPrivateProfileIntW( szSection , L"PitchAndFamily",49,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"PitchAndFamily", 49 );

		if (nStyle == 0)
		{
			wcscpy(logfont.lfFaceName, strFontName);
		}
		else
		{
			 GetPrivateProfileStringW( szSection , L"FaceName", L"Courier New",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
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
		m_pnCharSp``ace[nStyle] = MulDiv(m_pnCharSpace[nStyle],
			pDC->GetDeviceCaps(LOGPIXELSX),
			nLo`gicalPixelX);*/
		
		m_ppFont[nStyle] = new CFont;
 		m_ppFont[nStyle]->CreateFontIndirect(&logfont);
	}

		Invalidate();   

}

void editView_Top_New::UpdateFontSet()
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
		m_pnLineSpace[nStyle] = GetPrivateProfileIntW( szSection , L"LineSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		m_pnCharSpace[nStyle] = GetPrivateProfileIntW( szSection , L"CharSpace",1,((CWinApp*)AfxGetApp())->m_pszProfileName);
		//m_pnLineSpace[nStyle] = pApp->GetProfileInt( szSection, L"LineSpace", 1 );
		//m_pnCharSpace[nStyle] = pApp->GetProfileInt( szSection, L"CharSpace", 1 );
		
//		m_pnLineSpace[nStyle] = 100;
//		m_pnCharSpace[nStyle] =100;

		CString str_temp;// = pApp->GetProfileString( szSection, L"Height", L"-15" );
		wchar_t sz_temp[255];
		GetPrivateProfileStringW( szSection , L"Height", L"-15",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		logfont.lfHeight = _wtol(str_temp);

		GetPrivateProfileStringW( szSection , L"Width",  L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Width", L"0" );
		logfont.lfWidth  = _wtol(str_temp);

		GetPrivateProfileStringW( szSection , L"Escapement", L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Escapement", L"0" );
		logfont.lfEscapement = _wtol(str_temp);

		GetPrivateProfileStringW( szSection , L"Orientation", L"0",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Orientation", L"0" );
		logfont.lfOrientation = _wtol(str_temp);

		GetPrivateProfileStringW( szSection , L"Weight", L"400",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString( szSection, L"Weight", L"400" );
		logfont.lfWeight = _wtol(str_temp);

		logfont.lfItalic = GetPrivateProfileIntW( szSection , L"Italic",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Italic", 0 );
		logfont.lfUnderline = GetPrivateProfileIntW( szSection , L"Underline",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Underline", 0 );
		logfont.lfStrikeOut = GetPrivateProfileIntW( szSection , L"StrikeOut",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"StrikeOut", 0 );

		logfont.lfCharSet = GetPrivateProfileIntW( szSection , L"CharSet",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"CharSet", 0 );
		logfont.lfOutPrecision = GetPrivateProfileIntW( szSection , L"OutPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"OutPrecision", 0 );
		logfont.lfClipPrecision = GetPrivateProfileIntW( szSection , L"ClipPrecision",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"ClipPrecision", 0 );

		logfont.lfQuality = GetPrivateProfileIntW( szSection , L"Quality",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"Quality", 0 );
		logfont.lfPitchAndFamily = GetPrivateProfileIntW( szSection , L"PitchAndFamily",49,((CWinApp*)AfxGetApp())->m_pszProfileName);
			//pApp->GetProfileInt( szSection, L"PitchAndFamily", 49 );

		GetPrivateProfileStringW( szSection , L"FaceName",L"Courier New",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_temp = sz_temp;
		//str_temp = pApp->GetProfileString(szSection, L"FaceName", L"Courier New");
		wcscpy(logfont.lfFaceName, str_temp);
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
		Invalidate();   

}

void editView_Top_New::OnOptionModifyHistory()
{
	FILEPARAM fileParam;
	CString strVersion, strVersion_Mark, strLastUser, strModHistory, strVersion_Comment, str_Comment, strVersion_Style;
	COptionModifyDlg dlg;

	editDoc *pDoc=(editDoc*)GetDocument();
	CString temp = pDoc->GetPathName();
	fileParam = pDoc->m_FileParam;
	fileParam.enType=VERSIONFILE;
	_tcscpy(fileParam.szLastModTime,L"");
	CGlobal::GetInstance()->m_FileDB.ReadText_Version(fileParam.szFileCode, strVersion, strVersion_Mark, strLastUser, strModHistory, strVersion_Comment, str_Comment, strVersion_Style);
	dlg.m_strModHistory = strModHistory;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bOldVersion)
		{
			int i = dlg.m_nSelVersion + 1;
			pDoc=(editDoc*)OpenDocumentNew((LPTSTR)(LPCTSTR)temp, (void*)&fileParam);
			CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
			CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
			editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();
			CString str_Ver_Del;
			for (int j = 1; j < p_str_Ver_test_new.GetLength(); j++)
				if (j<i)
					str_Ver_Del += p_str_Ver_Del_test_new.Mid(j*(j-1)/2+2,i-1) + p_str_Ver_Del_test_new.Mid(j*(j-1)/2+2+i,j-i);
				else
					str_Ver_Del +=  p_str_Ver_Del_test_new.Mid(j*(j-1)/2+2+i,j-i);
			str_Ver_Del += p_str_Ver_test_new.Mid(i+1);
			int index;
			while((index = strVersion_Mark.FindOneOf(str_Ver_Del))>= 0)
			{
				strVersion.Delete(index);
				strVersion_Mark.Delete(index);
				strVersion_Comment.Delete(index);
				strVersion_Style.Delete(index);
			}
			for(int j = 0; j < strVersion.GetLength(); j++)
			{
				strVersion_Mark.SetAt(j,'A');
				strVersion_Comment.SetAt(j,'_');
				strVersion_Style.SetAt(j,'a');
			}
			pDoc->m_strVersion = strVersion;
			pDoc->m_strVersion_Mark = strVersion_Mark;
			pDoc->m_strVersion_Comment = strVersion_Comment;
			pDoc->m_str_Comment = L"";
			pDoc->m_strVersion_Style = strVersion_Style;
			strVersion.Replace(L"\r\n",L"\r");
			strVersion.Replace(L"\r",L"\r\n");
			pView->SetWindowText(strVersion);
			pView->SetTextToUpperView();
		}
	}	
}