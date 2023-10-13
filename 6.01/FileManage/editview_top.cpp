// editView_Top.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "editview_top.h"
#include "resource.h"
#include "editview_bottom.h"
#include "OptionModifyDlg.h"
#include "inputcommentdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		MENUPOS_TEXTSTYLE	2
#define		SB_INDICATORS_NEED	3
#define		SB_STATUS			0

COLORREF p_CommentColor1[MAX_COMMENT_NUM] = 
{
	RGB(189, 189, 255),
	RGB(255, 189, 189),
	RGB(214, 189, 189),
	RGB(189, 214, 214),
	RGB(214, 214, 189),
	RGB(214, 189, 214),
	RGB(189, 189, 214),
	RGB(189, 255, 189),
	RGB(189, 214, 189),
	RGB(255, 189, 255),
	RGB(189, 255, 255),
	RGB(230, 230, 230),
	RGB(230, 189, 214),
	RGB(255, 214, 189),
	RGB(214, 214, 214)
};

COLORREF p_Color_test[MAX_MODIFY_NUM+1] = 
{
	RGB(0, 0, 0),
	RGB(0, 0, 255),
	RGB(255, 0, 0),
	RGB(128, 0, 0),
	RGB(0, 128, 128),
	RGB(128, 128, 0),
	RGB(128, 0, 128),
	RGB(0, 0, 128),
	RGB(0, 255, 0),
	RGB(0, 128, 0),
	RGB(255, 0, 255),
	RGB(0, 255, 255),
	RGB(192, 192, 192),
	RGB(192, 0, 128),
	RGB(255, 128, 0),
	RGB(128, 128, 128)
};

CString p_str_Ver_Del_test = L" !@#¡$¢£%¤¥¦^§¨©ª&«¬­®¯°*±²³´µ¶(•¸¹º»¼½)¾¿ÀÁÂÃÄÅ+ÆÇÈÉÊËÌÍÎ{ÏÐÑÒÓÔÕÖ×Ø}ÙÚÛÜÝÞßàáâã<äåæçèéêëìíîï>ðñòóôõö÷øùúûý";
CString p_str_Ver_test = L"Aabcdefghijklmno";
CString p_str_Digit = L"0123456789abcdefghijklmnopqrstuvwxyz";

CString p_str_Cap_Letter = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/////////////////////////////////////////////////////////////////////////////
// editView_Top

IMPLEMENT_DYNCREATE(editView_Top, CScrollView)
extern "C" AFX_EXT_API CDocument* WINAPI OpenDocumentNew(LPTSTR lpFileName,void * pParam);
editView_Top::editView_Top()
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

	iCurLine = 0;
	iLineSpaceConstant = 0;
	iNextStartLineStrPos =0;
	iTextSpaceConstant = 0;
	iPrevScrollPos = 0;
	str_SelText = L"";
	bIsCommentMode = FALSE;
	bViewChanged = FALSE;
}

editView_Top::~editView_Top()
{
	if (m_pPopupMenu)
	{
		m_pPopupMenu->DestroyMenu();
		delete m_pPopupMenu;
		m_pPopupMenu = NULL;
	}

}


BEGIN_MESSAGE_MAP(editView_Top, CScrollView)
	//{{AFX_MSG_MAP(editView_Top)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
//  N_UPDATE_COMMAND_UI(ID_DO_COMMIT, OnUpdateDoCommit)
	ON_UPDATE_COMMAND_UI(ID_DO_MODIFY, OnUpdateDoModify)
	ON_COMMAND(ID_EDIT_COPY_TEST, OnCopy)
	ON_COMMAND(ID_EDIT_SHOWMODINFO, OnEditShowModHistory)
	ON_COMMAND(ID_OPTION_MODIFY, OnOptionModifyHistory)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_COMMAND(ID_SWITCH_MODE, SwitchMode)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_MODE, OnUpdateSwitchMode)
	ON_COMMAND(ID_EDIT_ADDCOMMENT, OnEditAddcomment)
	ON_COMMAND(ID_EDIT_DELETECOMMENT, OnDeletecomment)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// editView_Top drawing
void editView_Top::OnPaint()
{	
	CPaintDC dc(this);

	OnPrepareDC(&dc, NULL);
	
	OnDraw(&dc);
}

void editView_Top::OnDraw(CDC* pDC)
{
	TestOnDraw();
	return;
	/*CRect rc;
	GetClientRect(rc);
	if(rc.top == rc.bottom)
		return;
	return;*/
	CDocument* pDoc = GetDocument();
	int i_Line_Constant = 0;
	int i_x_shift = 0;
//	int	i_tab_count = 0; 
	m_max_width = 0;
	m_max_height = 0;
	int posx=0;
	int posy=0;
	
	m_arrCharPos.RemoveAll(); 
	for (int i=0; i < m_str_Text.GetLength(); i++)
	{

		int i_line_pos = 0;
		int i_Line_Number = GetLineNumber(i, i_line_pos);

		if (i_line_pos == 0)
		{
			i_x_shift = 0;
//			i_tab_count = 0; 
		}	
		pDC->SetTextColor(p_Color_test[GetVersion(i)]);

//		CFont font;
//		VERIFY(font.CreatePointFont(115,m_ppFont[0]));
//m_ppFont[0]->Get
		pDC->SelectObject(m_ppFont[0]);

		int i_check_width;
	//	pDC->GetCharWidth(L'口',L'口', &i_check_width); 
		pDC->GetCharWidth(L'都',L'都', &i_check_width); 
		if (i_check_width <= 5 ) i_check_width = 6;
		CString str_temp = this->m_str_Text_Version[i];
		
		CString str_temp_convert = m_str_Text[i];
		if (str_temp_convert == L'\r\n' || str_temp_convert == L'\n')
			str_temp_convert = "";
		
		if (p_str_Ver_Del_test.Find(str_temp) >= 0)
		{
			COLORREF rgbSave = pDC->GetBkColor();
			if (IsSelectedChar(i))
			{
				//pDC->FillSolidRect(i_x_shift, i_Line_Number * i_check_width+5, i_check_width-2, i_check_width-5, RGB(0, 0, 0));
				pDC->FillSolidRect(posx, posy, i_check_width-2, i_check_width-5, RGB(0, 0, 0));
				pDC->SetTextColor(RGB(255, 255, 255));
			}
			
			
			CSize size;
			CRect rt;
			GetClientRect(rt);
			size=pDC->GetTextExtent(str_temp_convert);
			if(str_temp_convert == "")
			{
				posy+=i_check_width+5;
				posx=0;
			}
			else if(posx+size.cx>rt.right-5)
			{
				posy+=i_check_width+5;
				posx=0;
				i_Line_Constant++;
				i_x_shift = 0;
			}
			int iPos;bool bDel;
			GetVersion_New(i, bDel, iPos);
			pDC->SetTextColor(p_Color_test[GetVersion(iPos)]);
			pDC->TextOut(posx, posy, str_temp_convert);
			
			//pDC->TextOut(i_x_shift, i_Line_Number * (i_check_width+5) , str_temp_convert); 
//			COLORREF rgbSave = pDC->GetBkColor();

			
			if (m_str_Text[i] != L'\r\n')
				//pDC->FillSolidRect(i_x_shift, i_Line_Number * (i_check_width+5) + (i_check_width-7), (i_check_width-2), 2, p_Color_test[GetVersion(i)]);
				pDC->FillSolidRect(posx,posy + (i_check_width-7), (i_check_width-2), 2, p_Color_test[GetVersion(i)]);
			posx+=size.cx;
			pDC->SetBkColor(rgbSave);
		}
		else
		{
//				pDC->TextOut(i_line_pos*15 + i_x_shift, i_Line_Number * 20 , str_temp_convert); 
				
				COLORREF rgbSave = pDC->GetBkColor();
				if (IsSelectedChar(i))
				{
					//pDC->FillSolidRect(i_x_shift, i_Line_Number * (i_check_width+5), (i_check_width-2), (i_check_width-5), RGB(0, 0, 0));
					pDC->FillSolidRect(posx, posy, i_check_width-2, i_check_width-5, RGB(0, 0, 0));
					pDC->SetTextColor(RGB(255, 255, 255));
				}
				CSize size;
				CRect rt;
				GetClientRect(rt);
				size=pDC->GetTextExtent(str_temp_convert);
				if(str_temp_convert == "")
				{
					posy+=i_check_width+5;
					posx=0;
				}
				else if(posx+size.cx>rt.right-5)
				{
					posy+=i_check_width+5;
					posx=0;
					i_Line_Constant++;
					i_x_shift = 0;
				}
			    pDC->TextOut(posx, posy, str_temp_convert);
				posx+=size.cx;
				//pDC->TextOut(i_x_shift, i_Line_Number * (i_check_width+5), str_temp_convert); 
				
				 // pDC->GetCurrentPosition() 
				pDC->SetBkColor(rgbSave);
		}

		int i_temp_width;
		pDC->GetCharWidth(m_str_Text[i],m_str_Text[i], &i_temp_width); 
		
		
		int i_tab_width = 1;
		if (i_check_width == 16)
			i_tab_width = 72;
		else if (i_check_width == 28)
			i_tab_width = 128;
		else if (i_check_width < 10)
			i_tab_width = i_check_width * 3.8; 
		else if (i_check_width < 16)
			i_tab_width = i_check_width * 4; 
		else if (i_check_width < 22)
			i_tab_width = i_check_width * 4.2; 
		else if (i_check_width < 28)
			i_tab_width = i_check_width * 4.4; 
		else 
			i_tab_width = i_check_width * 4.6; 

//		pDC->GetCharWidth(L'l', L'l', &i_tab_width); 


		int i_tab_index = i_x_shift/i_tab_width;

		// Handle Tab char
		if (m_str_Text[i] ==  L'\t')
		{
//			if (i_x_shift >  (i_tab_index * i_tab_width ) )
//			{	
//				i_temp_width = (i_tab_index * i_tab_width ) - i_x_shift % (i_tab_index * i_tab_width );	
//
//			}
//			else
				i_temp_width = ((i_tab_index+1) * i_tab_width ) - i_x_shift;	

//			i_tab_count = i_tab_count +1;
				posx+=i_temp_width;
		}

//		int i_line_width;
//		pDC->GetCharWidth(m_str_Text[i],m_str_Text[i], &i_line_width); 
		
		i_x_shift = i_x_shift + i_temp_width;

		CHAR_POS charpos;
		charpos.pt.x  = i_x_shift;
		//charpos.pt.y  = i_Line_Number * (i_check_width+5);
		charpos.pt.y  = (i_Line_Number+i_Line_Constant) * (i_check_width+5);
		charpos.i_pos = i;
		m_arrCharPos.Add(charpos);


		/*if (i_x_shift > m_max_width)
			m_max_width = i_x_shift;*/
		if (posx-i_tab_index > m_max_width)
			m_max_width = posx-i_tab_index-1;

		//int i_temp_height = (i_Line_Number+1) * (i_check_width+5);
		int i_temp_height = max((i_Line_Number+1) * (i_check_width+5),posy+(i_check_width+5));

		if (i_temp_height > m_max_height)
			m_max_height = i_temp_height;


//		if (IsDigit_Character(m_str_Text[i]))
//			i_x_shift = i_x_shift + i_temp_width;
//		else if (IsCapLetter_Character(m_str_Text[i])) 
//			i_x_shift = i_x_shift + 11;
//		else  
//			i_x_shift = i_x_shift + 20;

		if ( (i == m_str_Text.GetLength()-1) && !m_b_UpdateScroll)
		{
			m_b_UpdateScroll = true;
			CSize sizeTotal;
			sizeTotal.cx = m_max_width;
			sizeTotal.cy = m_max_height;
			SetScrollSizes(MM_TEXT, sizeTotal);

		}

	}

	CSize sizeTotal;
	sizeTotal.cx = m_max_width;
		
	sizeTotal.cy = m_max_height;
	SetScrollSizes(MM_TEXT, sizeTotal);

}


bool editView_Top::IsSelectedChar(int i_temp)  
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

CString editView_Top::GetActualSelectedText()  
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



bool editView_Top::IsDigit_Character(CString strTemp)
{
	if (p_str_Digit.Find(strTemp) >= 0)
		return true;	
	else
		return false;	
}

bool editView_Top::IsCapLetter_Character(CString strTemp)
{
	if (p_str_Cap_Letter.Find(strTemp) >= 0)
		return true;	
	else
		return false;	
}


int editView_Top::GetVersion(int i_pos)
{
	if (i_pos >= m_str_Text_Version.GetLength())
		return 0;
	CString str_temp = this->m_str_Text_Version[i_pos];

	int i_version_index = p_str_Ver_test.Find(str_temp);

	if (i_version_index < 0 )
	{
		double itemp =  p_str_Ver_Del_test.Find(str_temp);
		if (itemp > 2)
			itemp = ceil( (sqrt(8*(itemp-1)+1)-1) /2)+1;
		i_version_index = itemp;
	}

	if (i_version_index < 0 )
		i_version_index = 0;

	return i_version_index;	
}


int editView_Top::GetVersion_New(int i_pos, bool &b_Del, int &i_old)
{
	b_Del = false;

	if (i_pos >= m_str_Text_Version.GetLength())
		return 0;
	CString str_temp = this->m_str_Text_Version[i_pos];

	int i_version_index = p_str_Ver_test.Find(str_temp);

	if (i_version_index < 0 )
	{
		double itemp =  p_str_Ver_Del_test.Find(str_temp);
		if (itemp > 2)
		{
			int ipos = itemp;
			itemp = ceil( (sqrt(8*(itemp-1)+1)-1) /2)+1;
			i_old = ((2*itemp-3)*(2*itemp-3)-1)/8+1;
			i_old = ipos - i_old;
		}
		else
			i_old = 1;
			
		i_version_index = itemp;
		if (i_version_index >= 0 )
			b_Del = true;	
	}

	if (i_version_index < 0 )
		i_version_index = 0;

	return i_version_index;	
}


/////////////////////////////////////////////////////////////////////////////
// editView_Top diagnostics

#ifdef _DEBUG
void editView_Top::AssertValid() const
{
	CScrollView::AssertValid();
}

void editView_Top::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// editView_Top message handlers

void editView_Top::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	EnableScrollBarCtrl(SB_BOTH, FALSE);
	CSize sizeTotal;
//	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);
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

	CRect rc;
	GetClientRect(rc);
	
}

HBRUSH editView_Top::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here

	pDC->SetTextColor(RGB( 100, 0, 0 ));

	// TODO: Return a non-NULL brush if the parent's handler should not be called
return sHBR;
}

void editView_Top::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}

void editView_Top::SetContentText(CString strText, CString strTextVersion, CString strModHistory)
{
	//m_Edit.ShowWindow(bIsCommentMode);
	if(bIsCommentMode!=TRUE)
	{
		if(strTextVersion.CompareNoCase(m_str_Text_Version)==0)
			return;
	}else
	{
		strModHistory = L"";strTextVersion = strTextVersion+L" ";
		p_str_Ver_Comment = m_pDoc->GetPStrVerComment();
		str_Ver_Comment = m_pDoc->m_strVersion_Comment;
		for(int i=0;i<str_Ver_Comment.GetLength();i++)
		{
			CString str_temp = this->m_str_Text_Version[i];
			if (p_str_Ver_Del_test.Find(str_temp) >= 0)
			{
				str_Ver_Comment.Delete(i);
				m_str_Text_Version.Delete(i);
				i--;
			 }
		}
	}
	//bReDraw = TRUE;
	m_str_Text = strText;
	m_str_Text_Version = strTextVersion;
	m_str_Mod_History = strModHistory;

	if(strModHistory.GetLength()>0)
		InitModHistArray();
	InvalidateRect(NULL, FALSE);
	//Invalidate();
	CPoint pt,pt2; 
	pt=GetScrollPosition( ); 
	pt2.x=pt2.y=0;
	ScrollToPosition(pt2);
	//OnPaint();
	ScrollToPosition(pt);

	CString m_Temp_str_Text = m_str_Text;
	m_Temp_str_Text.Replace(L"\r\n", L"\r");
	m_Temp_str_Text.Replace(L"\n", L"\r");
	m_Temp_str_Text.Replace(L"\r", L"\r\n");
	m_Edit.SetWindowTextW(m_Temp_str_Text);
	m_str_Text.Replace(L"\r\n",L"\r");
	SetScrollBarOnText();
	//this->SetWindowText(m_str_Text); 

//	CSize sizeTotal;
//	sizeTotal.cx = sizeTotal.cy = m_str_Text.GetLength() ;
//	SetScrollSizes(MM_TEXT,  sizeTotal);

}

void editView_Top::OnUpdate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CScrollView::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

int editView_Top::GetLineNumber(int i_pos, int &i_line_pos) 
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
void editView_Top::OnUpdateDoCommit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->Enable(FALSE); 
}
*/
void editView_Top::OnUpdateDoModify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->Enable(FALSE); 
}


int editView_Top::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
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

	int iDeviceCap =  GetDC()->GetDeviceCaps(VERTRES);
	SetScrollRange(SB_VERT, 0,iDeviceCap);


	m_Edit.Create(WS_CHILD | WS_VISIBLE| ES_MULTILINE ,
		CRect(0,0,0,0), this, AFX_IDW_PANE_FIRST);
	m_Edit.ModifyStyleEx(ES_AUTOHSCROLL|WS_HSCROLL,0,0);
	//m_Edit.SetReadOnly(TRUE);
	//m_Edit.GetDC()->SetBkMode(TRANSPARENT);
	m_Edit.ShowWindow(FALSE);
	iPrevScrollPos = 0;
	m_Tips.Create(CSize(200*3, 20*5));
	return 0;
}

void editView_Top::OnRButtonDown(UINT nFlags, CPoint point) 
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

	CScrollView::OnRButtonDown(nFlags, point);
	m_Tips.HideTips();
}


void editView_Top::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_Tips.HideTips();
	bSelText = FALSE;
	iAutoScrollLine = 0;
	bAutoLineScrolledDown = FALSE;
	bAutoLineScrolledUp = FALSE;
	long	p;
	bSelText = FALSE;
	//SetCapture();
	m_mouseDown = TRUE;
	m_mouseMove = FALSE;


	m_selectStart_x = point.x; 	
	m_selectStart_y = point.y; 	
	m_selectEnd_x = point.x; 
	m_selectEnd_y = point.y; 
	m_Actual_StartX = m_selectStart_x;
	m_Actual_StartY = m_selectStart_y;
	iStartSelPos = -1;
	iEndSelPos = -1;
	SetTextCaret();
	cktPrevScroll = clock();
	pFrame->KillEditToopTips();
//	if ((p = SelectionPoint(point)) != m_caret) {
//		m_caret = m_selectStart = m_selectEnd = p;
//		Invalidate();
//	}
}

void editView_Top::OnLButtonUp(UINT nFlags, CPoint point) 
{ 
	if(bIsCommentMode != TRUE)
		MarkDownHightLightTextPosition();
	m_mouseMove = FALSE;
	iAutoScrollLine = 0;
	bAutoLineScrolledDown = FALSE;
	bAutoLineScrolledUp = FALSE;
	//ReleaseCapture();

	m_selectEnd_x = point.x; 	
	m_selectEnd_y = point.y; 	

//	m_selectStart = m_selectEnd = 0;

	SetSelectText();
	m_mouseDown = FALSE;
	CScrollView::OnLButtonUp(nFlags, point);
}

void editView_Top::SetSelectText()
{
	CString szSection, str_temp;
	szSection.Format(L"EditProTextStyle%ld", m_nStyleNum);
	wchar_t sz_temp[255];
	GetPrivateProfileStringW( szSection , L"Height", L"-15",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp = sz_temp;
	int iFontSize = _wtoi(str_temp);
	if(iFontSize<0)
		iFontSize = iFontSize*-1;

	CPoint point = GetScrollPosition();
	//for (int i=0; i < m_str_Text.GetLength(); i++)
	{
		m_min_select_pos = -1;
		m_max_select_pos = -1;

		for (int j=0; j < m_arrCharPos.GetSize(); j++)
		{
			CHAR_POS temp_pos;
			temp_pos = m_arrCharPos.GetAt(j); 
			
			
			CPoint temp_pt = temp_pos.pt; 
			if ( temp_pt.x  >= m_selectStart_x &&  temp_pt.x  <= m_selectEnd_x 
				//&& temp_pt.y + 10  >= m_selectStart_y &&  temp_pt.y  <= m_selectEnd_y)
				&& temp_pt.y + iFontSize  >= m_selectStart_y+point.y &&  temp_pt.y  <= m_selectEnd_y+point.y)
			{
				if (temp_pos.i_pos < m_min_select_pos || m_min_select_pos == -1)
					m_min_select_pos = temp_pos.i_pos;

				if (temp_pos.i_pos > m_max_select_pos || m_max_select_pos == -1)
					m_max_select_pos = temp_pos.i_pos;
			}
			//else if (temp_pt.x  <= m_selectEnd_x  && temp_pt.y + 10  >= m_selectStart_y + 10 &&  temp_pt.y  <= m_selectEnd_y - 10)
			else if (temp_pt.x  <= m_selectEnd_x  && temp_pt.y + iFontSize  >= m_selectStart_y+point.y + iFontSize &&  temp_pt.y  <= m_selectEnd_y+point.y - iFontSize)
			{
				if (temp_pos.i_pos < m_min_select_pos || m_min_select_pos == -1)
					m_min_select_pos = temp_pos.i_pos;

				if (temp_pos.i_pos > m_max_select_pos || m_max_select_pos == -1)
					m_max_select_pos = temp_pos.i_pos;
			}
			else if ( temp_pt.x  >= m_selectStart_x  
				//&& temp_pt.y + 10  >= m_selectStart_y &&  temp_pt.y  <= m_selectEnd_y - 15)
				&& temp_pt.y + iFontSize  >= m_selectStart_y+point.y &&  temp_pt.y  <= m_selectEnd_y+point.y - iFontSize)
			{
				if (temp_pos.i_pos < m_min_select_pos || m_min_select_pos == -1)
					m_min_select_pos = temp_pos.i_pos;

				if (temp_pos.i_pos > m_max_select_pos || m_max_select_pos == -1)
					m_max_select_pos = temp_pos.i_pos;
			}

		}

		//this->Invalidate(); 
	
		InvalidateRect(NULL, FALSE);
	}
}

long editView_Top::SelectionPoint(CPoint point)
{
	LOGFONT			lf;
	CPaintDC		dc(this);

//	CColorEditDoc* pDoc = GetDocument();

//	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	CPoint	pos = point;

	m_font.GetLogFont(&lf);
	CFont* oldFont = dc.SelectObject(&m_font);
	CSize		size = GetExtent(&dc, _T(" "));

	long line = pos.y/size.cy;

//	CString string = pDoc->GetLine(line);
	CString string = m_str_Text;
//	long offset = pDoc->GetLineOffset(line);
	long offset = 4;

	for (int i = 0; i < string.GetLength()-1; i++) {
		CSize size = GetExtent(&dc, string.Left(i+1));
		if (pos.x-m_indent < size.cx) {
			return offset + i;
		}
	}

	dc.SelectObject(oldFont);
	return offset + string.GetLength();
}

CPoint editView_Top::GetExtent(CDC * pDC, const CString & string, long xoffset, long line)
{
	CSize	size = pDC->GetTextExtent(_T(" "));
	int		x = 0, y = size.cy;
	int		tabWidth = size.cx*m_tabSize;

	for (int i = 0; i < string.GetLength(); i++) {
		TCHAR c = string[i];
		if (iswprint(c) || iswspace(c)) {
			if (c == '\t') {
				x = ((x/tabWidth)+1)*tabWidth;
			} else {
				x += pDC->GetTextExtent(c).cx;
			}
		}
	}
	return CPoint(x+xoffset, y*line);
}

void editView_Top::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(bKeyDown==TRUE)
		return;
	if (m_mouseDown==TRUE)
	{
		m_selectEnd_x = point.x; 	
		m_selectEnd_y = point.y; 
		m_CurMouse_X = point.x; 
		m_CurMouse_Y = point.y;
		//SetSelectText();
		bSelText = TRUE;

		MarkDownHightLightTextPosition();

		CRect rc;
		GetClientRect(rc);
		if(point.y>= rc.bottom-iFontHeight/2)
		{
			bAutoLineScrolledDown = TRUE;
			bAutoLineScrolledUp = FALSE;
			ScrollDownOneLine();
		}

		if(point.y<= rc.top + iFontHeight/2)
		{
			bAutoLineScrolledUp = TRUE;
			bAutoLineScrolledDown = FALSE;
			ScrollUpOneLine();
		}
		SetTextCaret();
	}
	CScrollView::OnMouseMove(nFlags, point);
}

void editView_Top::OnCopy()
{
   // TODO: Add your control notification handler code here
	  CString strSelectText = GetActualSelectedText() ; 
	CString str;str = m_str_Text;
	str= str.Left(iEndSelPos+1);
	str= str.Right(iEndSelPos- iStartSelPos+1);
	strSelectText = str;
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

void editView_Top::InitModHistArray() 
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

void editView_Top::OnEditShowModHistory() 
{

	CString strTemp = L"";
	CString strCurrent = L"";
	CStringArray strAll;
	CString strWrite = L"";

	bool b_Del = false;
	bool b_Reply = false;

	int i_ver = 0;
	int i_old = 0;

	//for (int i=m_min_select_pos; i < m_str_Text.GetLength(); i++)
	//{
		//if (m_min_select_pos != -1  && m_max_select_pos != -1 )	
		//{
			//if (i >= m_min_select_pos && i <= m_max_select_pos)
			//{
	if(iSelTextPos == -1||iSelTextPos>m_str_Text.GetLength())
	{
		m_pStatusBar->SetPaneText(SB_STATUS, strTemp);
		return;
	}
	int i = iSelTextPos-1;//m_max_select_pos;
	if (i == m_str_Text.GetLength())
		i = i - 1;
	i_ver = GetVersion_New(i, b_Del, i_old);

	if (b_Del)
		strCurrent.Format(L"#%ldD#",i_old,i_ver);  
	else
		strCurrent.Format(L"#%ld#",i_ver);  
				
	if (strAll.IsEmpty() || (strAll.GetAt(strAll.GetCount()-1) != strCurrent))
	{
		strWrite.Format(L"#%ld#",i_old);  
		if (!strAll.IsEmpty() && strAll.GetAt(strAll.GetCount()-1) == strWrite)
			b_Reply = true;
		strAll.Add(strCurrent);

		if (m_arrModHist.GetSize() > 0)
		{
			if (b_Del && !b_Reply)
				strTemp = strTemp + L" " + m_arrModHist.GetAt(i_old-1) + L"輸入";

			if (i_ver > m_arrModHist.GetSize() - 1) 
				strTemp	=  strTemp	+ L" " + m_arrModHist.GetAt(m_arrModHist.GetSize() - 1);  
			else
				strTemp	=  strTemp	+ L" " + m_arrModHist.GetAt(i_ver-1);  
					
			if (b_Del)
				strTemp = strTemp + L"刪除";
			else	
				strTemp = strTemp + L"輸入";
		}


	}
//	  CString strSelectText = L"User 在 2012-05-07 15:45:55 輸入"; 
	m_pStatusBar->SetPaneText(SB_STATUS, strTemp);
}

void editView_Top::UpdateFontSetTemp(CString strFontName)
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
	m_ppFont[0]->GetLogFont(&logfont);
	iFontHeight = logfont.lfHeight<0?logfont.lfHeight*-1: logfont.lfHeight;
	m_Edit.SetFont(m_ppFont[0]);
	GetASCIIActualWidth();
	InvalidateRect(NULL);   

}

void editView_Top::UpdateFontSet()
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
	m_ppFont[0]->GetLogFont(&logfont);
	iFontHeight = logfont.lfHeight<0?logfont.lfHeight*-1: logfont.lfHeight;
	m_Edit.SetFont(m_ppFont[0]);
	InvalidateRect(NULL);  
	GetASCIIActualWidth();
	/*m_arrCharWidth.RemoveAll();
	for(int i=0;i<255;i++)
	{
		CHAR_WIDTH m_CharWidth;
		m_CharWidth.c = i;
		GetDC()->SelectObject(m_ppFont[0]);
		CSize size;
		size = GetDC()->GetTextExtent((char)i);
		int j = 8 *(iFontHeight/size.cy);
		int k = 8*(iFontHeight*1.0/size.cy);
		m_CharWidth.iWidth = size.cx * (iFontHeight*1.0/size.cy);
		m_arrCharWidth.Add(m_CharWidth);
	}*/


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

BOOL editView_Top::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_TRACKPOS;

	if (LOBYTE(nScrollCode) == SB_THUMBTRACK)
	{
		GetScrollInfo(SB_HORZ, &info);
		nPos = info.nTrackPos;
	}
	if (HIBYTE(nScrollCode) == SB_THUMBTRACK)
	{
		GetScrollInfo(SB_VERT, &info);
		nPos = info.nTrackPos;
	}

	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}

void editView_Top::OnOptionModifyHistory()
{
	FILEPARAM fileParam;
	CString strVersion, strVersion_Mark, strLastUser, strModHistory, strVersion_Comment, str_Comment, strVersion_Style, strValue;
	COptionModifyDlg dlg;
	BOOL bTextHist = TRUE;
	CGlobal *pGlobal=CGlobal::GetInstance();
	if(pGlobal->m_FileDB.GetConfig(_T("IsStoreTextHist"), strValue))
		bTextHist = strValue != _T("0") ? TRUE : FALSE;

	editDoc *pDoc=(editDoc*)GetDocument();
	CString temp = pDoc->GetPathName();
	fileParam = pDoc->m_FileParam;
	fileParam.enType=VERSIONFILE;
	_tcscpy(fileParam.szLastModTime,L"");
	pGlobal->m_FileDB.ReadText_Version(fileParam.szFileCode, strVersion, strVersion_Mark, strLastUser, strModHistory, strVersion_Comment, str_Comment, strVersion_Style);
	dlg.m_strModHistory = strModHistory;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bOldVersion)
		{
			int i = dlg.m_nSelVersion + 1;
			if(bTextHist)
				bTextHist = pGlobal->m_FileDB.LoadTextHist(fileParam.szFileCode, strVersion, i);
			if(!bTextHist)
			{
				CString str_Ver_Del;
				for (int j = 1; j < p_str_Ver_test.GetLength(); j++)
				{
					if (j<i)
						str_Ver_Del += p_str_Ver_Del_test.Mid(j*(j-1)/2+2,i-1) + p_str_Ver_Del_test.Mid(j*(j-1)/2+2+i,j-i);
					else
						str_Ver_Del +=  p_str_Ver_Del_test.Mid(j*(j-1)/2+2+i,j-i);
				}
				str_Ver_Del += p_str_Ver_test.Mid(i+1);
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
			}
			else
			{
				if (strVersion.GetAt(0) == 65279)
					strVersion = strVersion.Mid(1);
				strVersion_Mark=L"";
				for (int i=0; i < strVersion.GetLength(); i++) 
					strVersion_Mark = strVersion_Mark + L"A";
			}

			pDoc=(editDoc*)OpenDocumentNew((LPTSTR)(LPCTSTR)temp, (void*)&fileParam);
			CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
			CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
			editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();
			pDoc->m_strVersion = strVersion;
			pDoc->m_strVersion_Mark = strVersion_Mark;
			pDoc->m_strVersion_Comment = strVersion_Comment;
			pDoc->m_str_Comment = L"";
			pDoc->m_strVersion_Style = strVersion_Style;
			int k =strVersion.GetLength();
			strVersion.Replace(L"\r\n",L"\r");
			strVersion.Replace(L"\n",L"\r");
			k =strVersion.GetLength();
			strVersion.Replace(L"\r",L"\r\n");
			//// Edit Stephen 2014-02-26
			//strVersion.Replace(L"\n",L"\r\n");
			//// Edit Stephen 2014-02-26
			k =strVersion.GetLength();
			pView->SetWindowText(strVersion);
			pView->SetTextToUpperView();
		}
	}	
}

void editView_Top::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CPoint pt;
	CRect rc;
	GetClientRect(rc);
	int iMaxLineNo = (rc.bottom - rc.top)/(iFontHeight+iLineSpaceConstant) ;
	int iLines = m_Edit.GetLineCount();
	SetScrollRange(SB_VERT,0,iLines*(iFontHeight+iLineSpaceConstant) - (rc.bottom - rc.top));
	if(iLines<=iMaxLineNo)
		EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
	else
		EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);
	if(iPrevScrollPos<0)
		iPrevScrollPos = 0;
	if(iPrevScrollPos>iLines*(iFontHeight+iLineSpaceConstant))
		iPrevScrollPos = iLines*(iFontHeight+iLineSpaceConstant);
	switch(nSBCode)
    {
		case SB_LINEDOWN:
			if(iCurLine + iMaxLineNo>=iLines)
			{
				//InvalidateRect(NULL);
				UnlockWindowUpdate();
				return;
			}
			LockWindowUpdate();
			pt=GetScrollPosition( ); 
			pt.y += iFontHeight<0?(iFontHeight*-1)+iLineSpaceConstant:iFontHeight+iLineSpaceConstant;
			this->ScrollToPosition(pt);
			iCurLine++;
			iPrevScrollPos +=iFontHeight+iLineSpaceConstant;
		    break;
		case SB_LINEUP:
			LockWindowUpdate();
			if(iCurLine==0)
			{
				//InvalidateRect(NULL);
				UnlockWindowUpdate();
				iCurLine = 0;
				return;
			}
			pt=GetScrollPosition( ); 
			pt.y -= iFontHeight<0?(iFontHeight*-1)+iLineSpaceConstant:iFontHeight+iLineSpaceConstant;
			this->ScrollToPosition(pt);
			iCurLine--;
			if(iCurLine<0)
			{
				//InvalidateRect(NULL);
				UnlockWindowUpdate();
				iCurLine = 0;
				return;
			}
			iPrevScrollPos -=iFontHeight+iLineSpaceConstant;
			break;

		case SB_THUMBPOSITION:
			{
				nPos = nPos;
				iCurLine = nPos/(iFontHeight+iLineSpaceConstant);
				iPrevScrollPos = nPos;
				if(iCurLine<=0)
					iCurLine = 0;
				if(iCurLine>= iLines-iMaxLineNo)
					iCurLine = iLines-iMaxLineNo;
				this->SetScrollPos(SB_VERT, iPrevScrollPos);
				break;
			}

		case SB_PAGEUP:
			{
				LockWindowUpdate();
				pt=GetScrollPosition( ); 
				pt.y -= iFontHeight<0?(iFontHeight*-1)+iLineSpaceConstant:iFontHeight+iLineSpaceConstant;
				this->ScrollToPosition(pt);
				iCurLine-= iMaxLineNo;
				iPrevScrollPos -= iMaxLineNo*(iFontHeight+iLineSpaceConstant);
				if(iCurLine<0)
				{
					//InvalidateRect(NULL);
					UnlockWindowUpdate();
					iCurLine = 0;
					iPrevScrollPos = 0;
				}
				break;
			}

		case SB_PAGEDOWN:
			{
				if(iMaxLineNo>=iLines)
				{
					//InvalidateRect(NULL);
					UnlockWindowUpdate();
					return;
				}
				LockWindowUpdate();
				pt=GetScrollPosition( ); 
				pt.y += iFontHeight<0?(iFontHeight*-1)+iLineSpaceConstant:iFontHeight+iLineSpaceConstant;
				this->ScrollToPosition(pt);
				iCurLine+=iMaxLineNo;
				if(iCurLine>= iLines)
					iCurLine = iLines;
				if(iCurLine + iMaxLineNo >=iLines)
				{
					iPrevScrollPos += (iLines - iCurLine) * (iFontHeight+iLineSpaceConstant);
					iCurLine = iLines - iMaxLineNo/*iCurLine*/;
				}
				else
					iPrevScrollPos += iMaxLineNo*(iFontHeight+iLineSpaceConstant);
				break;
			}
		default:
			return;
	}

	this->SetScrollPos(SB_VERT, iPrevScrollPos);
	InvalidateRect(NULL, FALSE);
	UnlockWindowUpdate();
	//bReDraw = TRUE;
}

BOOL editView_Top::HasScrollToBottom()
{
	int y;
	long lThis;
	BOOL bRtCode = FALSE;
	return bRtCode;
}

int editView_Top::FindNextLineStrPos(int iCurLine)
{
	int iNextPos = 0;

	return iNextPos;
}

void editView_Top::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	//SetScrollBarOnText();
}

void editView_Top::GetASCIIActualWidth()
{
	int posx=0;
	int posy=0;
	CDC *pDC;
	pDC = GetDC();
	m_arrCharWidth.RemoveAll();
	for(int i=0;i<=255;i++)
	{
		pDC->SelectObject(m_ppFont[0]);

		int i_check_width;
		pDC->GetCharWidth(L'都',L'都', &i_check_width); 
		if (i_check_width <= 5 ) i_check_width = 6;

		CSize size;
		CRect rt;
		GetClientRect(rt);
		size=pDC->GetTextExtent((char)i);
		posx =size.cx;

		CHAR_WIDTH m_CharWidth;
		m_CharWidth.c = i;
		m_CharWidth.iWidth = posx;
		m_arrCharWidth.Add(m_CharWidth);
	}
	iFontHeight = pDC->GetTextExtent(L'都').cy;
	m_iFontWidth = pDC->GetTextExtent(L'都').cx;
}

void editView_Top::TestOnDraw()
{
	CPoint pt;
	pt = GetScrollPosition();
	CDC* pDC = GetDC();
	CRect rc;
	GetClientRect(rc);
	SetScrollBarOnText();
	int iMaxLineNo = (rc.bottom - rc.top)/(iFontHeight+iLineSpaceConstant);
	int iLineWidth = rc.right - rc.left;
	int iMaxHeight = iMaxLineNo * (iFontHeight+iLineSpaceConstant);
	int iCurLineTemp = 0;
	pDC->SelectObject(m_ppFont[0]);

	BOOL bFillRectToEnd= FALSE;
	if(iPrevTotalLine!= m_Edit.GetLineCount())
	{
		bFillRectToEnd = TRUE;
		iPrevTotalLine = m_Edit.GetLineCount();
	}

	int iCurX = 0, iCurY = 0, iNextX = 0, iNextY = 0,iPos = 0;
	bool bDel;
	int iNextHistoryPos = 0;
	int iCheckedLength = 0;
	int iEnterCount = 0;
	int iSkipLength = GetSkippedTextLength(iCurLine, iEnterCount);
	iCheckedLength = iSkipLength;
	str_SelText = L"";
	
	for(int i=0;i<iMaxLineNo;i++)
	{
		CString strLine = GetTextByLine(iCurLine+i);
		if(iCurLine+i>=m_Edit.GetLineCount())
			break;
		//iNextHistoryPos = m_str_Text.Find(strLine, iNextHistoryPos)<0?0:m_str_Text.Find(strLine);
		for(int j=0;j<strLine.GetLength();j++)
		{
			CString str_temp_convert =  strLine[j];
			wchar_t c = str_temp_convert.GetAt(0);
			if(bIsCommentMode!=TRUE)
			{
				int iCurPos = GetVersion_New(iCheckedLength, bDel, iPos);
				if(bDel==TRUE)
					pDC->SetTextColor(p_Color_test[iPos]);
				else
					pDC->SetTextColor(p_Color_test[iCurPos]);
				pDC->TextOut(iCurX, iCurY,str_temp_convert);
				if(bDel==TRUE)
				{
					if (strLine[j] != L'\r\n')
						pDC->FillSolidRect(iCurX,iCurY + ((iFontHeight+iLineSpaceConstant)-((iFontHeight+iLineSpaceConstant)/3)), c>0&&c<255?GetWidthByChar(c, iCheckedLength, iEnterCount):iFontHeight, 2, p_Color_test[iCurPos]);
					else
						bDel = bDel;
				}
			}else
			{
				int iTempPos = 0;
				iTempPos = p_str_Ver_Comment.Find(str_Ver_Comment[iCheckedLength]);
				iTempPos = iTempPos>15?iTempPos-15:iTempPos;
				if(iTempPos>-1)
				{
					if (str_temp_convert != L'\r\n')
						pDC->FillSolidRect(iCurX,iCurY, c>0&&c<255?GetWidthByChar(c, iCheckedLength, iEnterCount):iFontHeight,iFontHeight+iLineSpaceConstant, p_CommentColor1[iTempPos]);
				}
				pDC->SetTextColor(RGB(0,0,0));
				pDC->TextOut(iCurX, iCurY,str_temp_convert);
			}

			if(iCheckedLength>=iStartSelPos && iCheckedLength<=iEndSelPos)
			{
				CRect rc;
				rc.top = iCurY; rc.bottom = iCurY+(iFontHeight+iLineSpaceConstant);
				rc.left = iCurX;
				int iX = c>0&&c<255?GetWidthByChar(c, iCheckedLength, iEnterCount):iFontHeight;
				rc.right = iCurX+iX;
				pDC->InvertRect(rc);
				str_SelText += str_temp_convert;
				iSelTextPos = iCheckedLength+1;
			}
			if(c>0&&c<255)
			{
				CHAR_WIDTH m_CharWidth;
				m_CharWidth = m_arrCharWidth.GetAt(c);
				int iWidth = m_CharWidth.iWidth;
				if(isdigit(c)||ispunct(c))
				{	
					if(c==95)
					{
						iCurX += iWidth + iTextSpaceConstant;
					}else
					if(c>=48 && c<=57)
						iCurX += iWidth + iTextSpaceConstant;
					else
						iCurX += iWidth + iTextSpaceConstant;
				}else
				if(c == 32)
				{
					iCurX += iWidth +iTextSpaceConstant;
				}else
				if(c == 9)
				{
					int i_tab_width = 0;
					CPoint pp;
					pp = m_Edit.PosFromChar(iCheckedLength+1+iEnterCount);
					if(pp.x>iCurX)
					{
						if(pp.x !=0)
						{
							if (m_mouseDown != TRUE)
								pDC->FillSolidRect(iCurX,iCurY, pp.x - iCurX, (iFontHeight+iLineSpaceConstant), RGB(255,255,255));
						}
						if (pp.x!=0)
							iCurX = pp.x ;
					}
				}
				else 
				/*if(c<32)
				{
					if(c==22)
						iCurX += iFontHeight/2 +1+iTextSpaceConstant;
					else
						continue;
				}
				else*/ if(c>=97&& c<=122)
				{
					iCurX += iWidth+iTextSpaceConstant;
				}
				else if(c>=65&& c<=90)
				{
					iCurX += iWidth+iTextSpaceConstant;
				}else
				{
					iCurX += iWidth+iTextSpaceConstant;
				}
			}
			else
				iCurX += (m_iFontWidth/*iFontHeight*/+iTextSpaceConstant);
			pDC->SetBkColor(RGB(255,255,255));
			iCheckedLength++;
		}
		if((m_mouseDown!=TRUE||m_str_Text[iCheckedLength-iEnterCount]!=L'\t'))
		{
			pDC->FillSolidRect(iCurX,iCurY, rc.right - iCurX, iFontHeight, RGB(255,255,255));			
			pDC->FillSolidRect(0,iCurY+iFontHeight, rc.right, rc.bottom-iCurY, RGB(255,255,255));
		}

		if(iCheckedLength>m_str_Text.GetLength())
			break;
		char cc = m_str_Text[iCheckedLength];
		if(m_str_Text[iCheckedLength]== L'\r\n'||m_str_Text[iCheckedLength]==L'\r'||m_str_Text[iCheckedLength]==L'\n')
		{
			iCurX = 0;
			iCurY += iFontHeight+iLineSpaceConstant;
			int iCurPos = GetVersion_New(iCheckedLength, bDel, iPos);
			//if(bDel==TRUE)
			//	pDC->FillSolidRect(iCurX,iCurY + (iFontHeight-iFontHeight/3), iFontHeight/4, 2, p_Color_test[iCurPos]);
			iCheckedLength++;
			//if(m_str_Text[iCheckedLength]== L'\r\n')
			iEnterCount++;
			if(m_mouseDown!=TRUE && bFillRectToEnd == TRUE)
				pDC->FillSolidRect(iCurX,iCurY, rc.right - iCurX, iFontHeight+iLineSpaceConstant, RGB(255,255,255));
		}
		else
		{
			iCurX = 0;
			iCurY += iFontHeight+iLineSpaceConstant;
		}
		pDC->SetBkColor(RGB(255,255,255));
	}
	//if(bReDraw)
	//	Invalidate();
	ScrollToPosition(pt);
	bReDraw = FALSE;
}

CString editView_Top::GetTextByLine(int iRelativeLine)
{
	int len = m_Edit.LineLength(m_Edit.LineIndex(iRelativeLine));
	CString path;
	LPTSTR p = path.GetBuffer(len);
	m_Edit.GetLine(iRelativeLine, p, len);
	path.ReleaseBuffer();
	return path;
}

int editView_Top::GetSkippedTextLength(int iRelativeLine, int &iEnterCount)
{
	int iLength = 0;iEnterCount = 0;
	int iTextLength = m_str_Text.GetLength();
	for(int i=0;i<iRelativeLine;i++)
	{
		int len = m_Edit.LineLength(m_Edit.LineIndex(i));
		CString path;
		LPTSTR p = path.GetBuffer(len);
		m_Edit.GetLine(i, p, len);
		path.ReleaseBuffer();
		if(path.GetLength()!=0)
		{
			iLength += path.GetLength();
			if(iTextLength>=iLength)
			{
				if(m_str_Text[iLength]== L'\r\n'||m_str_Text[iLength]== L'\n'||m_str_Text[iLength]== L'\r')
				{
					iLength++;
					iEnterCount++;
				}
					
			}
		}
		else
		{
			iEnterCount++;
			iLength++;
		}
	}
	return iLength;
}

int editView_Top::GetWidthByChar(wchar_t c, int iCheckedLength, int iEnterCount)
{
	int iX = 0;
	CHAR_WIDTH m_CharWidth;
	if(c>255)
		return m_iFontWidth/*iFontHeight*/;
	if(c<0)
		return iFontHeight;
	m_CharWidth = m_arrCharWidth.GetAt(c);
	int iWidth = m_CharWidth.iWidth;
	if(isdigit(c)||ispunct(c))
	{	
		if(c==95)
		{
			iX += iWidth + iTextSpaceConstant;
		}else
		if(c>=48 && c<=57)
			iX += iWidth + iTextSpaceConstant;
		else
			iX += iWidth + iTextSpaceConstant;
	}else
	if(c == 32)
	{
		iX += iWidth +iTextSpaceConstant;
	}else
	if(c == 9)
	{
		int i_tab_width = 0;
		CPoint pp = m_Edit.PosFromChar(iCheckedLength+1+iEnterCount);
		m_Edit.SetFocus();
		CPoint pt2 = m_Edit.GetCaretPos();
		iX = pp.x ;
	}
	else 
	/*if(c<32)
	{
		if(c==22)
			iCurX += iFontHeight/2 +1+iTextSpaceConstant;
		else
			continue;
	}
	else*/ if(c>=97&& c<=122)
	{
		iX += iWidth+iTextSpaceConstant;
	}
	else if(c>=65&& c<=90)
	{
		iX += iWidth+iTextSpaceConstant;
	}else
	{
		iX += iWidth+iTextSpaceConstant;
	}

	return iWidth;
}

void editView_Top::SetTextCaret()
{
	CPoint pt, pt2;
	pt.x = m_selectEnd_x;
	pt.y = (m_selectEnd_y/(iFontHeight+iLineSpaceConstant))*(iFontHeight+iLineSpaceConstant);
	pt2 = pt;
	int iLineTotal = m_Edit.GetLineCount();
	if(iLineTotal == 0)
		iLineTotal = 1;
	if(pt.y > (iLineTotal - iCurLine - 1)*iFontHeight)
	{
		pt.y =  (iLineTotal - iCurLine - 1)*iFontHeight;
	}
	
	int iCurFocusLine = iCurLine + (m_selectEnd_y/iFontHeight), iEnterCount;
	CString strLine = GetTextByLine(iCurFocusLine);
	int iSkippedLen = GetSkippedTextLength(iCurFocusLine, iEnterCount);
	int iCheckedLength = iSkippedLen;
	int iStartIndex = 0, iEndIndex = 0;
	wchar_t szFound;

	int iCurX = 0;
	for(int i=0;i<strLine.GetLength();i++)
	{
		CString str_Temp = strLine[i];
		wchar_t c = str_Temp.GetAt(0);
		int iWidth = 0;
		if(c!= 9)
		{
			iWidth = GetWidthByChar(c, iCheckedLength, iEnterCount);
			pt2.x -= iWidth;
		}
		else
		{
			int i_tab_width = 0;
			CPoint pp;
			pp = m_Edit.PosFromChar(iCheckedLength+1+iEnterCount);
			i_tab_width = pp.x-iCurX;
			pt2.x -= i_tab_width;
			iCurX += i_tab_width;
		}
		if(pt2.x<0)
		{
			iEndIndex = iCheckedLength;break;
		}

		if(pt2.x>0 && i == strLine.GetLength() -1)
		{
			iEndIndex = iCheckedLength;
			iCurX += iWidth;break;
		}
		
		iCurX += iWidth;
		iCheckedLength++;
	}
	if(pt.x!=0)
		pt.x = iCurX;

	iSelTextPos = iEndIndex+1;

	CreateSolidCaret(3, iFontHeight);
	SetCaretPos(pt);
	ShowCaret();

	Invalidate(FALSE);
}

void editView_Top::MarkDownHightLightTextPosition()
{
	CPoint pt;
	pt.x = m_selectStart_x;
	pt.y = m_selectStart_y;

	CPoint pt2;
	pt2.x = m_selectEnd_x;
	pt2.y = m_selectEnd_y;

	if(pt == pt2)
	{
		iEndSelPos = iStartSelPos;
		//iSelTextPos = -1;
		return;
	}
	if(iAutoScrollLine == 0 || (pt2.y/iFontHeight)==(pt.y/iFontHeight))
	{
		bAutoLineScrolledDown = FALSE;
		bAutoLineScrolledUp = FALSE;
	}

	if(iAutoScrollLine!=0)
		pt.y += iAutoScrollLine* iFontHeight;

	if((pt2.x<pt.x && (pt2.y/iFontHeight)==(pt.y/iFontHeight))
		||
		((pt2.y/iFontHeight)<(pt.y/iFontHeight)))
	{
		if(bAutoLineScrolledUp == FALSE)
		{
			CPoint pt_Temp;
			pt_Temp = pt2;
			pt2 = pt;
			pt = pt_Temp;
		}else
		{
			pt.x = m_Actual_StartX;
			pt.y = m_Actual_StartY;
			CPoint pt_Temp;
			pt_Temp = pt2;
			pt2 = pt;
			pt = pt_Temp;
		}
	}

	int iEnterCount = 0;
	
	int iStartLine = (pt.y/iFontHeight) + iCurLine;
	int iEndLine = (pt2.y/iFontHeight) + iCurLine;
	int iSkippedLen = GetSkippedTextLength(iStartLine, iEnterCount);
	int iCheckedLength = iSkippedLen;
	int iStartIndex = 0, iEndIndex = 0;
	CString strLine;
	if(bAutoLineScrolledDown == FALSE)
	{
		strLine = GetTextByLine(iStartLine);

		for(int i=0;i<strLine.GetLength();i++)
		{
			CString str_Temp = strLine[i];
			wchar_t c = str_Temp.GetAt(0);
			int iWidth;
			if(c!= 9)
			{
				iWidth = GetWidthByChar(c, iCheckedLength, iEnterCount);
				pt.x -= iWidth;
			}
			else
			{
				int i_tab_width = 0;
				CPoint pp;
				pp = m_Edit.PosFromChar(iCheckedLength+1+iEnterCount);
				pt.x -= pp.x;
			}
			iCheckedLength++;
			if(pt.x<0)
			{
				iStartIndex = iCheckedLength-1;break;
			}
			else if(pt.x == 0)
			{
				iStartIndex = iCheckedLength;break;
			}
			
			if(pt.x>0 && i == strLine.GetLength() -1)
			{
				iStartIndex = iCheckedLength;break;
			}
		}
		if(strLine.GetLength()==0)
			iStartIndex = iCheckedLength;
		iStartSelPos = iStartIndex;
	}
	
	if(bAutoLineScrolledUp == FALSE)
	{
		strLine = GetTextByLine(iEndLine);
		iSkippedLen = GetSkippedTextLength(iEndLine, iEnterCount);
		iCheckedLength = iSkippedLen;
		BOOL bTabProcessed = FALSE;
		int iCurX = 0;
		for(int i=0;i<strLine.GetLength();i++)
		{
			CString str_Temp = strLine[i];
			wchar_t c = str_Temp.GetAt(0);
			int iWidth = 0;
			if(c!= 9)
			{
				iWidth = GetWidthByChar(c, iCheckedLength, iEnterCount);
				pt2.x -= iWidth;
			}
			else
			{
				int i_tab_width = 0;
				CPoint pp;
				pp = m_Edit.PosFromChar(iCheckedLength+1+iEnterCount);
				i_tab_width = pp.x-iCurX;
				pt2.x -= i_tab_width;
				iCurX += i_tab_width;
			}
			if(pt2.x<0)
			{
				iEndIndex = iCheckedLength-1;break;
			}
			else if(pt2.x == 0)
			{
				iEndIndex = iCheckedLength;break;
			}
			
			if(pt2.x>0 && i == strLine.GetLength() -1)
			{
				iEndIndex = iCheckedLength;break;
			}
			iCurX += iWidth;
			iCheckedLength++;
		}
		if(strLine.GetLength()==0)
			iEndIndex = iCheckedLength;
		/*if(iCurX<=0)
			iEndSelPos = iStartSelPos;
		else*/
			iEndSelPos = iEndIndex;
	}
}

void editView_Top::ScrollDownOneLine(int iLine)
{
	clock_t start = clock();

	int ms = (start - cktPrevScroll);
	if(ms<100)
		return;
	cktPrevScroll =  clock();
	CPoint pt;
	CRect rc;
	GetClientRect(rc);
	int iMaxLineNo = (rc.bottom - rc.top)/(iFontHeight+iLineSpaceConstant) ;
	int iLines = m_Edit.GetLineCount();
	SetScrollRange(SB_VERT,0,iLines*(iFontHeight+iLineSpaceConstant) - (rc.bottom - rc.top));
	if(iPrevScrollPos<0)
		iPrevScrollPos = 0;
	if(iPrevScrollPos>iLines*(iFontHeight+iLineSpaceConstant))
		iPrevScrollPos = iLines*(iFontHeight+iLineSpaceConstant);
	if(iCurLine + iMaxLineNo>=iLines)
	{
		UnlockWindowUpdate();
		return;
	}
	LockWindowUpdate();
	pt=GetScrollPosition( ); 
	pt.y += iLine*iFontHeight<0?iLine*((iFontHeight*-1)+iLineSpaceConstant):iLine*(iFontHeight+iLineSpaceConstant);
	this->ScrollToPosition(pt);
	iCurLine+=iLine;
	if (iLines<iMaxLineNo+iCurLine)
		iCurLine -= (iMaxLineNo+iCurLine)-iLines;
	iPrevScrollPos +=iLine*iFontHeight;
	this->SetScrollPos(SB_VERT, iPrevScrollPos);
	//InvalidateRect(NULL, FALSE);
	UnlockWindowUpdate();
	iAutoScrollLine-=iLine;
}

void editView_Top::ScrollUpOneLine(int iLine)
{
	clock_t start = clock();

	int ms = (start - cktPrevScroll);
	if(ms<100)
		return;
	cktPrevScroll =  clock();
	LockWindowUpdate();
	CPoint pt;
	CRect rc;
	GetClientRect(rc);
	int iMaxLineNo = (rc.bottom - rc.top)/(iFontHeight+iLineSpaceConstant) ;
	int iLines = m_Edit.GetLineCount();
	SetScrollRange(SB_VERT,0,iLines*(iFontHeight+iLineSpaceConstant) - (rc.bottom - rc.top));
	if(iPrevScrollPos<0)
		iPrevScrollPos = 0;
	if(iPrevScrollPos>iLines*(iFontHeight+iLineSpaceConstant))
		iPrevScrollPos = iLines*(iFontHeight+iLineSpaceConstant);
	if(iCurLine==0)
	{
		UnlockWindowUpdate();
		iCurLine = 0;
		return;
	}
	pt=GetScrollPosition( ); 
	pt.y -= iLine*iFontHeight<0?iLine*((iFontHeight*-1)+iLineSpaceConstant):iLine*iFontHeight+iLineSpaceConstant;
	this->ScrollToPosition(pt);
	iCurLine -= iLine;
	if(iCurLine<0)
	{
		UnlockWindowUpdate();
		iCurLine = 0;
		return;
	}
	iPrevScrollPos -=iLine*iFontHeight;
	UnlockWindowUpdate();
	iAutoScrollLine+=iLine;
}

BOOL editView_Top::PreTranslateMessage(MSG* pMsg) 
{
	CRect rc;GetClientRect(rc);
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam=='A' && GetKeyState(VK_CONTROL)< 0)
	{
		m_mouseDown = TRUE;
		iStartSelPos = 0;
		iEndSelPos = m_str_Text.GetLength()-1;
		m_mouseDown = FALSE;
		InvalidateRect(NULL);
	}

	if(pMsg->message==WM_KEYDOWN && GetKeyState(VK_SHIFT)< 0)
	{
		m_mouseDown = TRUE;
		bKeyDown = TRUE;
	}
	if(pMsg->message==WM_KEYUP && GetKeyState(VK_SHIFT) >= 0)
	{
		bSelText = FALSE;
		iAutoScrollLine = 0;
		bAutoLineScrolledDown = FALSE;
		bAutoLineScrolledUp = FALSE;
		m_mouseDown = FALSE;
		bKeyDown = FALSE;

	}

	if(pMsg->message==WM_KEYDOWN) 
	{
		int iLineTotal = m_Edit.GetLineCount();
		int iMaxLine = (rc.bottom - rc.top)/iFontHeight;
		BOOL bTempKeyDown = bKeyDown;
		int iLineNo = iCurLine + m_selectEnd_y/iFontHeight;
		switch(pMsg->wParam)
		{
			case 35://End Key
				m_selectEnd_x = GetLineMaxX(iLineNo);
				if(bKeyDown == TRUE)
					MarkDownHightLightTextPosition();
				SetTextCaret();
				break;
			case 36://Home Key
				m_selectEnd_x = 0;
				if(bKeyDown == TRUE)
					MarkDownHightLightTextPosition();
				SetTextCaret();
				break;
			case 37:// Left Key
				MoveCaretHorizontialLeft();
				if(bKeyDown == TRUE)
					MarkDownHightLightTextPosition();
				SetTextCaret();
				break;
			case 38:// Up Key
				if(m_selectEnd_y/iFontHeight == 0 && iCurLine == 0)
					return CScrollView::PreTranslateMessage(pMsg);
				m_selectEnd_y -= iFontHeight;
				if(m_selectEnd_y<=rc.top + iFontHeight/2)
				{
					m_selectEnd_y = rc.top;
					ScrollUpOneLine();
				}
				if(bKeyDown == TRUE)
					MarkDownHightLightTextPosition();
				SetTextCaret();
				if(m_selectEnd_x == m_selectStart_x && m_selectStart_y == m_selectEnd_y)
					 iEndSelPos=iStartSelPos ;
				break;
			case 39:// Right Key
				MoveCaretHorizontialRight();
				if(bKeyDown == TRUE)
					MarkDownHightLightTextPosition();
				SetTextCaret();
				break;
			case 40:// Down Key
				if((iCurLine + (m_selectEnd_y/iFontHeight))>=iLineTotal-1)
					return CScrollView::PreTranslateMessage(pMsg);
				m_selectEnd_y += iFontHeight;
				if(m_selectEnd_y>=rc.bottom - iFontHeight/2)
				{
					m_selectEnd_y = rc.bottom - iFontHeight;
					ScrollDownOneLine();
				}
				if(bKeyDown == TRUE)
					MarkDownHightLightTextPosition();
				SetTextCaret();
				if(m_selectEnd_x == m_selectStart_x && m_selectStart_y == m_selectEnd_y)
					iStartSelPos = iEndSelPos;
				break;
		}
		if(bTempKeyDown!= TRUE)
			{
				iStartSelPos = iEndSelPos = -1;
				m_selectStart_x = m_selectEnd_x;
				m_selectStart_y = m_selectEnd_y;
				bSelText = FALSE;
			}
		this->SetFocus();
	}

	if(pMsg->message==WM_MOUSEWHEEL) 
	{
		int delta=GET_WHEEL_DELTA_WPARAM(pMsg->wParam);
		if(delta == 120)
        {
              ScrollUpOneLine(3);
        }
        else if(delta == -120)
        {
              ScrollDownOneLine(3);
        }
	}
	
	if(bAutoLineScrolledDown == TRUE && m_CurMouse_Y>= rc.bottom-iFontHeight/2)
		ScrollDownOneLine();
	if(bAutoLineScrolledUp == TRUE && m_CurMouse_Y<= rc.top + iFontHeight/2)
		ScrollUpOneLine();
	return CScrollView::PreTranslateMessage(pMsg);
}

void editView_Top::MoveCaretHorizontialLeft()
{
	int iLineByY = iCurLine + (m_selectEnd_y/iFontHeight);
	int iTempX = m_selectEnd_x;
	int iCheckedLength, iEnterCount;
	iCheckedLength = GetSkippedTextLength(iLineByY, iEnterCount);
	CString strLine = GetTextByLine(iLineByY);
	int iCurX = 0, iPrevX = 0, iEndIndex = 0;
	for(int i=0;i<strLine.GetLength();i++)
	{
		CString str_Temp = strLine[i];
		wchar_t c = str_Temp.GetAt(0);
		int iWidth = 0;
		if(c!= 9)
		{
			iWidth = GetWidthByChar(c, iCheckedLength, iEnterCount);
			iTempX -= iWidth;
		}
		else
		{
			int i_tab_width = 0;
			CPoint pp;
			pp = m_Edit.PosFromChar(iCheckedLength+1+iEnterCount);
			i_tab_width = pp.x-iCurX;
			iTempX -= i_tab_width;
			iCurX += i_tab_width;
		}
		
		if(iTempX<0)
		{
			iCurX = iCurX +iTempX;
			iEndIndex = iCheckedLength-1;break;
		}
		else if(iTempX == 0)
		{
			iEndIndex = iCheckedLength;break;
		}
			
		if(iTempX>0 && i == strLine.GetLength() -1)
		{
			iEndIndex = iCheckedLength-1;break;
		}
		iCurX += iWidth;
		iCheckedLength++;
	}
	if(iCurX <= 0)
	{
		CString str = m_str_Text[iCheckedLength];
		wchar_t c = str.GetAt(0);
		GetSkippedTextLength(iLineByY-1, iEnterCount);
		if(iCheckedLength ==0)
		{
			m_selectEnd_x = 0;
			m_selectEnd_y = 0;
		}
		else
		{
			m_selectEnd_x = GetLineMaxX(iLineByY-1);
			m_selectEnd_y = m_selectEnd_y - iFontHeight;
			CRect rc ; GetClientRect(rc);
			if(m_selectEnd_y<=rc.top + iFontHeight/2)
			{
				ScrollUpOneLine();
				m_selectEnd_y = 0;
			}
		}
	}
	else
		m_selectEnd_x = iCurX;
}

void editView_Top::MoveCaretHorizontialRight()
{
	int iLineByY = iCurLine + (m_selectEnd_y/iFontHeight);
	int iTempX = m_selectEnd_x;
	int iCheckedLength, iEnterCount;
	iCheckedLength = GetSkippedTextLength(iLineByY, iEnterCount);
	CString strLine = GetTextByLine(iLineByY);
	int iCurX = 0, iEndX = -1, iEndIndex = 0;
	BOOL bNext = FALSE;
	for(int i=0;i<strLine.GetLength();i++)
	{
		CString str_Temp = strLine[i];
		wchar_t c = str_Temp.GetAt(0);
		int iWidth = 0;
		if(c!= 9)
		{
			iWidth = GetWidthByChar(c, iCheckedLength, iEnterCount);
			iTempX -= iWidth;
		}
		else
		{
			int i_tab_width = 0;
			CPoint pp;
			pp = m_Edit.PosFromChar(iCheckedLength+1+iEnterCount);
			i_tab_width = pp.x-iCurX;
			iTempX -= i_tab_width;
			iCurX += i_tab_width;
		}
		if(iTempX<0 ||iTempX == 0 ||i == strLine.GetLength() -1)
			bNext= TRUE;
		iCurX += iWidth;
		iCheckedLength++;
		if(bNext == TRUE && i<strLine.GetLength()&&iEndX<0)
		{
			if(m_selectEnd_x !=0)
			{
				CString str_Temp = strLine[i+1];
				wchar_t c = str_Temp.GetAt(0);
				int iWidth = 0;
				if(c!= 9)
				{
					iWidth = GetWidthByChar(c, iCheckedLength, iEnterCount);
					iTempX -= iWidth;
				}
				else
				{
					int i_tab_width = 0;
					CPoint pp;
					pp = m_Edit.PosFromChar(iCheckedLength+1+iEnterCount);
					i_tab_width = pp.x-iCurX;
					iTempX -= i_tab_width;
					iCurX += i_tab_width;
				}
				if (iWidth == 0)
					iWidth = 1;
				iEndX = iCurX+iWidth;
				iEndIndex = iCheckedLength + 1;
			}else
				iEndX = iCurX;
		}
	}
	if(strLine.GetLength() == 0)
		iEndX = 1;
	int iLineMaxX = GetLineMaxX(iLineByY);
	if(iCurX < iEndX)
	{
			
		CString str = m_str_Text[iCheckedLength];
		int iTotal = m_Edit.GetLineCount()*iFontHeight;
		if((m_selectEnd_y + iFontHeight)+(iCurLine*iFontHeight) >= m_Edit.GetLineCount()*iFontHeight)
			m_selectEnd_x = iEndX;
		else
		{
			m_selectEnd_x = 0;
			m_selectEnd_y = m_selectEnd_y + iFontHeight;
		}
		CRect rc ; GetClientRect(rc);
		if(m_selectEnd_y>=rc.bottom - iFontHeight/2)
		{
			ScrollDownOneLine();
			m_selectEnd_y = rc.bottom - iFontHeight;
		}
	}
	else
		m_selectEnd_x = iEndX;
}

int editView_Top::GetLineMaxX(int iLineNo)
{
	CString strLine = GetTextByLine(iLineNo);
	int iEnterCount;
	int iSkippedLen = GetSkippedTextLength(iLineNo, iEnterCount);
	int iCheckedLength = iSkippedLen;
	int iStartIndex = 0, iEndIndex = 0;
	int iCurX = 0;
	for(int i=0;i<strLine.GetLength();i++)
	{
		CString str_Temp = strLine[i];
		wchar_t c = str_Temp.GetAt(0);
		int iWidth = 0;
		if(c!= 9)
			iWidth = GetWidthByChar(c, iCheckedLength, iEnterCount);
		else
		{
			int i_tab_width = 0;
			CPoint pp;
			pp = m_Edit.PosFromChar(iCheckedLength+1+iEnterCount);
			i_tab_width = pp.x-iCurX;
			iCurX += i_tab_width;
		}
		iCurX += iWidth;
		iCheckedLength++;
	}
	return iCurX;
}

void editView_Top::SetScrollBarOnText()
{
	CRect rc;
	GetClientRect(rc);
	if(rc.top < rc.bottom)
	{
		int iMaxLineNo = (rc.bottom - rc.top)/iFontHeight ;
		int iTotalLine = m_Edit.GetLineCount();
		int iRange = iTotalLine*(iFontHeight+iLineSpaceConstant) - (rc.bottom - rc.top);
		SetScrollRange(SB_VERT,0,iRange);
		if(iTotalLine<=iMaxLineNo)
		{
			SetScrollRange(SB_VERT,0,rc.bottom);
			EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
		}
		else
			EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);
		m_Edit.MoveWindow(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}
}

void editView_Top::SwitchMode()
{
	pFrame->SwitchingModeOnTop();
}

void editView_Top::OnUpdateSwitchMode(CCmdUI* pCmdUI) 
{
	editDoc* m_pDoc = (editDoc*)m_pDocument;
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
	if(bHasComment==FALSE&&bIsCommentMode == TRUE)
		SwitchMode();
	pCmdUI->Enable(bHasComment); 
	//pCmdUI->Enable(FALSE); 
}

void editView_Top::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	if(bIsCommentMode!=TRUE)
		return;
	int iEnterCount = 0;
	
	int iStartLine = (point.y/iFontHeight) + iCurLine;
	int iSkippedLen = GetSkippedTextLength(iStartLine, iEnterCount);
	int iCheckedLength = iSkippedLen;
	int iStartIndex = 0, iEndIndex = 0, iPos = 0;
	CString strLine;
	strLine = GetTextByLine(iStartLine);

	for(int i=0;i<strLine.GetLength();i++)
	{
		CString str_Temp = strLine[i];
		wchar_t c = str_Temp.GetAt(0);
		int iWidth;
		if(c!= 9)
		{
			iWidth = GetWidthByChar(c, iCheckedLength, iEnterCount);
			point.x -= iWidth;
		}
		else
		{
			int i_tab_width = 0;
			CPoint pp;
			pp = m_Edit.PosFromChar(iCheckedLength+1+iEnterCount);
			point.x -= pp.x;
		}
		iCheckedLength++;
		if(point.x<0)
		{
			iPos = iCheckedLength-1;break;
		}
		else if(point.x == 0)
		{
			iPos = iCheckedLength;break;
		}
			
		if(point.x>0 && i == strLine.GetLength() -1)
		{
			iPos = iCheckedLength;break;
		}
	}
	if(strLine.GetLength()==0)
		iPos = iCheckedLength;

	CString str = str_Ver_Comment[iPos], str_Temp_Comment = str_Ver_Comment ;
	if(p_str_Ver_Comment.Find(str)>-1)
	{
		int position = str_Temp_Comment.Find(str);
		int length = str_Temp_Comment.Replace(str, str);
		iStartSelPos = position;
		iEndSelPos = position+length-1;
	}
	ShowCommentInToolTip();
	this->SetFocus();
}

void editView_Top::OnEditAddcomment() 
{
	if(!pFrame->GetEditStatus() || iStartSelPos == -1)
		return;
	int pos, i_Select_Text_Len;  
	pos = iStartSelPos;
	i_Select_Text_Len = iEndSelPos - iStartSelPos + 1;
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
		SwitchMode();SwitchMode();
	}
}

void editView_Top::ShowCommentInToolTip()
{
	if(iEndSelPos - iStartSelPos >0)
	{
		 CString strTemp;
		 CString strComment = m_pDoc->GetTextVersionComment(iStartSelPos, iEndSelPos-iStartSelPos+1);
		 strTemp.Format(L"%s", strComment);
		 if(strTemp.GetLength()==0)return;
		 // show tool tip base on mouse point
		 CRect rc;
		 GetWindowRect(rc);
		 m_Tips.ShowTips((m_selectEnd_x + 5)+(200*3)<rc.right?m_selectEnd_x + 5:(m_selectEnd_x + 5)-(200*3), 
			 m_selectEnd_y + 25, strTemp);
		 SetFocus();
	}else
		m_Tips.HideTips();
}

void editView_Top::KillToopTips()
{
	m_Tips.HideTips();
}

void editView_Top::OnDeletecomment() 
{
	if(pFrame->GetEditStatus()!=TRUE || iStartSelPos == -1)
		return;
	m_pDoc->SetTextVersionComment(iStartSelPos, iEndSelPos-iStartSelPos+1, 0, true);
	SwitchMode();SwitchMode();
}