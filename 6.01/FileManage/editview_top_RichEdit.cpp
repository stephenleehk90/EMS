// editView_Top.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "editview_top_RichEdit.h"
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

COLORREF p_Color_test1[MAX_MODIFY_NUM+1] = 
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

CString p_str_Ver_Del_test1 = L" !@#¡$¢£%¤¥¦^§¨©ª&«¬­®¯°*±²³´µ¶(•¸¹º»¼½)¾¿ÀÁÂÃÄÅ+ÆÇÈÉÊËÌÍÎ{ÏÐÑÒÓÔÕÖ×Ø}ÙÚÛÜÝÞßàáâã<äåæçèéêëìíîï>ðñòóôõö÷øùúûý";
CString p_str_Ver_test1 = L"Aabcdefghijklmno";
CString p_str_Digit1 = L"0123456789abcdefghijklmnopqrstuvwxyz";

CString p_str_Cap_Letter1 = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/////////////////////////////////////////////////////////////////////////////
// editView_Top

IMPLEMENT_DYNCREATE(editView_Top_RichEdit, CRichEditView)
extern "C" AFX_EXT_API CDocument* WINAPI OpenDocumentNew(LPTSTR lpFileName,void * pParam);
editView_Top_RichEdit::editView_Top_RichEdit()
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
	b_IsFirst = TRUE;
	pFrame = NULL;
}

editView_Top_RichEdit::~editView_Top_RichEdit()
{
	if (m_pPopupMenu)
	{
		m_pPopupMenu->DestroyMenu();
		delete m_pPopupMenu;
		m_pPopupMenu = NULL;
	}

}


BEGIN_MESSAGE_MAP(editView_Top_RichEdit, CRichEditView)
	//{{AFX_MSG_MAP(editView_Top)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CHAR()
	ON_WM_SIZE()
//	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
///	ON_WM_LBUTTONUP()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
//  N_UPDATE_COMMAND_UI(ID_DO_COMMIT, OnUpdateDoCommit)
	ON_UPDATE_COMMAND_UI(ID_DO_MODIFY, OnUpdateDoModify)
	ON_COMMAND(ID_EDIT_COPY_TEST, OnCopy)
	ON_COMMAND(ID_EDIT_SHOWMODINFO, OnEditShowModHistory)
	ON_COMMAND(ID_OPTION_MODIFY, OnOptionModifyHistory)
	ON_COMMAND(ID_COMMENT_VIEW, SwitchMode)
	ON_COMMAND(ID_EDIT_ADDCOMMENT, OnEditAddcomment)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EDIT_DELETECOMMENT, OnDeletecomment)
	ON_COMMAND(ID_SWITCH_MODE, SwitchMode)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_MODE, OnUpdateSwitchMode)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//ON_UPDATE_COMMAND_UI(ID_EDIT_ADDCOMMENT, OnUpdateCommand)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_DELETECOMMENT, OnUpdateCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// editView_Top drawing
void editView_Top_RichEdit::OnPaint()
{	 CPaintDC dc(this);

// OnPrepareDC(&dc, NULL);

 OnDraw(&dc);
}

void editView_Top_RichEdit::OnDraw(CDC* pDC)
{	return;//OnDraw method no need to use anymore
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
		//pDC->SetTextColor(p_Color_test[GetVersion(i)]);
		GetRichEditCtrl().SetSel(i, i+1);
		CHARFORMAT cf;
		GetRichEditCtrl().GetSelectionCharFormat(cf);
		cf.crTextColor = p_Color_test1[GetVersion(i)];
		GetRichEditCtrl().SetSelectionCharFormat(cf);

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
		
		if (p_str_Ver_Del_test1.Find(str_temp) >= 0)
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
			pDC->TextOut(posx, posy, str_temp_convert);
			//pDC->TextOut(i_x_shift, i_Line_Number * (i_check_width+5) , str_temp_convert); 
//			COLORREF rgbSave = pDC->GetBkColor();

			
			if (m_str_Text[i] != L'\r\n')
				//pDC->FillSolidRect(i_x_shift, i_Line_Number * (i_check_width+5) + (i_check_width-7), (i_check_width-2), 2, p_Color_test[GetVersion(i)]);
				pDC->FillSolidRect(posx,posy + (i_check_width-7), (i_check_width-2), 2, p_Color_test1[GetVersion(i)]);
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

		CHAR_POS_1 charpos;
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

		/*if ( (i == m_str_Text.GetLength()-1) && !m_b_UpdateScroll)
		{
			m_b_UpdateScroll = true;
			CSize sizeTotal;
			sizeTotal.cx = m_max_width;
			sizeTotal.cy = m_max_height;
			SetScrollSizes(MM_TEXT, sizeTotal);

		}*/

	}

	//CSize sizeTotal;
	//sizeTotal.cx = m_max_width;
	//	
	//sizeTotal.cy = m_max_height;
	//SetScrollSizes(MM_TEXT, sizeTotal);

}


bool editView_Top_RichEdit::IsSelectedChar(int i_temp)  
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

CString editView_Top_RichEdit::GetActualSelectedText()  
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



bool editView_Top_RichEdit::IsDigit_Character(CString strTemp)
{
	if (p_str_Digit1.Find(strTemp) >= 0)
		return true;	
	else
		return false;	
}

bool editView_Top_RichEdit::IsCapLetter_Character(CString strTemp)
{
	if (p_str_Cap_Letter1.Find(strTemp) >= 0)
		return true;	
	else
		return false;	
}


int editView_Top_RichEdit::GetVersion(int i_pos)
{
	if (i_pos >= m_str_Text_Version.GetLength())
		return 0;
	CString str_temp = this->m_str_Text_Version[i_pos];

	int i_version_index = p_str_Ver_test1.Find(str_temp);

	if (i_version_index < 0 )
	{
		double itemp =  p_str_Ver_Del_test1.Find(str_temp);
		if (itemp > 2)
			itemp = ceil( (sqrt(8*(itemp-1)+1)-1) /2)+1;
		i_version_index = itemp;
	}

	if (i_version_index < 0 )
		i_version_index = 0;

	return i_version_index;	
}


int editView_Top_RichEdit::GetVersion_New(int i_pos, bool &b_Del, int &i_old)
{
	b_Del = false;

	if (i_pos >= m_str_Text_Version.GetLength())
		return 0;
	CString str_temp = this->m_str_Text_Version[i_pos];

	int i_version_index = p_str_Ver_test1.Find(str_temp);

	if (i_version_index < 0 )
	{
		double itemp =  p_str_Ver_Del_test1.Find(str_temp);
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
void editView_Top_RichEdit::AssertValid() const
{
	CRichEditView::AssertValid();
}

void editView_Top_RichEdit::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// editView_Top message handlers

void editView_Top_RichEdit::OnInitialUpdate() 
{
	CRichEditView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	EnableScrollBarCtrl(SB_VERT, TRUE);
	EnableScrollBarCtrl(SB_HORZ, FALSE);
//	CSize sizeTotal;
////	// TODO: calculate the total size of this view
//	sizeTotal.cx = sizeTotal.cy = 0;
//	SetScrollSizes(MM_TEXT, sizeTotal);
//	m_max_height = 0;
//	m_max_width = 0;

//	m_b_UpdateScroll = false;
//	SetScrollSizes(MM_TEXT, GetDocument()->GetDocGetDocSize( ) );

	UpdateFontSet();
	
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
//	m_cHeight = lf.lfHeight = tm.tmHeight;
	lstrcpy(lf.lfFaceName, _T("Courier New"));
	m_font.CreateFontIndirect(&lf);



	PARAFORMAT2 pf2; 
	pf2.cbSize = sizeof(PARAFORMAT2); 
	pf2.dwMask = PFM_LINESPACING | PFM_SPACEAFTER | PFM_SPACEBEFORE; 
	pf2.bLineSpacingRule = 5; 
	pf2.dyLineSpacing = 1;
	pf2.dySpaceAfter = 0;
	pf2.dySpaceBefore = 0;
	GetRichEditCtrl().SetParaFormat(pf2);
	::SendMessage(m_hWnd, EM_SETTABSTOPS, 0, 0L);

}

HBRUSH editView_Top_RichEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here

	pDC->SetTextColor(RGB( 100, 0, 0 ));

	// TODO: Return a non-NULL brush if the parent's handler should not be called
return sHBR;
}

void editView_Top_RichEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRichEditView::OnChar(nChar, nRepCnt, nFlags);
}

void editView_Top_RichEdit::SetContentText(CString strText, CString strTextVersion, CString strModHistory)
{
	CRect rc;
	GetWindowRect(rc);
	if(m_str_Text_Version==strTextVersion&&bViewChanged != TRUE || rc.bottom==rc.top)
	//if(m_str_Text_Version==strTextVersion&&bViewChanged != TRUE)
		return;
	m_str_Text = strText;
	m_str_Text_Version = strTextVersion;
	m_str_Mod_History = strModHistory;

	InitModHistArray();

	Invalidate();

	THREADSTRUCT *_param = new THREADSTRUCT;
	_param->_this = this;
	AfxBeginThread(StartThread, _param);
	bViewChanged = FALSE;
	//CRect rc ;
	//if(rc_standard==0)
	//{
	//	GetClientRect(rc);	
	//	rc_standard = rc;
	//}
	//else
	//	rc = rc_standard;
	//if(rc.bottom!=0)
	//	this->ShowWindow(FALSE);
	//this->SetWindowText(m_str_Text); 
	//if(!bIsCommentMode)
	//	setHistoryTextColor();
	//else
	//	OnShowcomment();
	//if(rc.bottom!=0)
	//{
	//	this->ShowWindow(TRUE);
	//	//rc.top+=3;
	//	//rc.left+=3;
	//	MoveWindow(rc_standard);
	//}
	//rc = 0;
}

void editView_Top_RichEdit::OnUpdate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CScrollView::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

int editView_Top_RichEdit::GetLineNumber(int i_pos, int &i_line_pos) 
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
void editView_Top_RichEdit::OnUpdateDoModify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->Enable(FALSE); 
}


int editView_Top_RichEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	bIsCommentMode = 0;
	GetRichEditCtrl().SetReadOnly(1);
	GetClientRect(rc_org);
	m_Tips.Create(CSize(200*3, 20*5));
	bHasComment = 0;
	bStartFlag = 0;
	CRect rc;
	GetRichEditCtrl().GetClientRect(rc);
	/*rc.left -= 2;
	rc.right -= 1.5;*/
	GetRichEditCtrl().SetRect(rc);
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_SETLANGOPTIONS, 0, 0);
	//GetWindowRect(rc_org);
	return 0;
}

void editView_Top_RichEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMenu* pPopup = m_pPopupMenu->GetSubMenu(0);
	CMenu* pSubMenu = pPopup->GetSubMenu(MENUPOS_TEXTSTYLE);

//	pSubMenu->CheckMenuItem(nStyle-1, MF_CHECKED|MF_BYPOSITION);

	ClientToScreen(&point);

		RECT		m_rcWnd;	
	pPopup->TrackPopupMenu(
		TPM_RIGHTBUTTON, point.x, point.y, this, &m_rcWnd);

//	pSubMenu->CheckMenuItem(nStyle-1, MF_UNCHECKED|MF_BYPOSITION);
	CRichEditView::OnRButtonDown(nFlags, point);
	m_Tips.HideTips();
}


void editView_Top_RichEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	((editFrameNew*)pChild)->KillEditToopTips();
	m_Tips.HideTips();
	CRichEditView::OnLButtonDown(nFlags, point);
	
}

void editView_Top_RichEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_mouseMove = FALSE;
	ReleaseCapture();

	m_selectEnd_x = point.x; 	
	m_selectEnd_y = point.y; 	

//	m_selectStart = m_selectEnd = 0;

	SetSelectText();
	m_mouseDown = FALSE;

	CRichEditView::OnLButtonUp(nFlags, point);
}

void editView_Top_RichEdit::SetSelectText()
{
	CString szSection, str_temp;
	szSection.Format(L"EditProTextStyle%ld", m_nStyleNum);
	wchar_t sz_temp[255];
	GetPrivateProfileStringW( szSection , L"Height", L"-15",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp = sz_temp;
	int iFontSize = _wtoi(str_temp);
	if(iFontSize<0)
		iFontSize = iFontSize*-1;

	CPoint point = GetScrollPos(SB_VERT);
	//for (int i=0; i < m_str_Text.GetLength(); i++)
	{
		m_min_select_pos = -1;
		m_max_select_pos = -1;

		for (int j=0; j < m_arrCharPos.GetSize(); j++)
		{
			CHAR_POS_1 temp_pos;
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

	this->Invalidate(); 

	}
}

long editView_Top_RichEdit::SelectionPoint(CPoint point)
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

CPoint editView_Top_RichEdit::GetExtent(CDC * pDC, const CString & string, long xoffset, long line)
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

void editView_Top_RichEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	ptLog = point;
	CRichEditView::OnMouseMove(nFlags, point);
}

void editView_Top_RichEdit::OnCopy()
{
   // TODO: Add your control notification handler code here
	 // CString strSelectText = GetActualSelectedText() ; 
	CString strSelectText = GetRichEditCtrl().GetSelText(); 
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

void editView_Top_RichEdit::InitModHistArray() 
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

void editView_Top_RichEdit::OnEditShowModHistory() 
{

	CString strTemp = L"";
	CString strCurrent = L"";
	CStringArray strAll;
	CString strWrite = L"";

	bool b_Del = false;
	bool b_Reply = false;

	int i_ver = 0;
	int i_old = 0;

	GetRichEditCtrl().GetSel(m_min_select_pos,m_max_select_pos);
	/*for (int i=m_min_select_pos; i < m_str_Text.GetLength(); i++)
	{
		if (m_min_select_pos != -1  && m_max_select_pos != -1 )	
		{
			if (i >= m_min_select_pos && i <= m_max_select_pos)
			{*/
	int i = m_max_select_pos;
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
	/*			}
			}

		
		}*/


	}

//	  CString strSelectText = L"User 在 2012-05-07 15:45:55 輸入"; 
	

	m_pStatusBar->SetPaneText(SB_STATUS, strTemp);
	  

}

void editView_Top_RichEdit::UpdateFontSetTemp(CString strFontName)
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
	GetRichEditCtrl().SetFont(m_ppFont[0]);
		Invalidate();   
		setHistoryTextColor();
}

void editView_Top_RichEdit::UpdateFontSet()
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
	iFontHeight = logfont.lfHeight;
	GetRichEditCtrl().SetFont(m_ppFont[0]);
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

BOOL editView_Top_RichEdit::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
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

	return CRichEditView::OnScroll(nScrollCode, nPos, bDoScroll);
}

void editView_Top_RichEdit::OnOptionModifyHistory()
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
				for (int j = 1; j < p_str_Ver_test1.GetLength(); j++)
				{
					if (j<i)
						str_Ver_Del += p_str_Ver_Del_test1.Mid(j*(j-1)/2+2,i-1) + p_str_Ver_Del_test1.Mid(j*(j-1)/2+2+i,j-i);
					else
						str_Ver_Del +=  p_str_Ver_Del_test1.Mid(j*(j-1)/2+2+i,j-i);
				}
				str_Ver_Del += p_str_Ver_test1.Mid(i+1);
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
			k =strVersion.GetLength();
			strVersion.Replace(L"\n",L"\r");
			strVersion.Replace(L"\r",L"\r\n");
			// Edit Stephen 2014-02-26
			//strVersion.Replace(L"\n",L"\r\n");
			// Edit Stephen 2014-02-26
			k =strVersion.GetLength();
			pView->SetWindowText(strVersion);
			pView->SetTextToUpperView();
		}
	}	
}

void editView_Top_RichEdit::setHistoryTextColor()
{
	GetRichEditCtrl().HideSelection(TRUE, FALSE);
	m_arrCharPos.RemoveAll();
	GetRichEditCtrl().LockWindowUpdate();
	int nFirstVisible = GetRichEditCtrl().GetFirstVisibleLine();
	BOOL bStrokeOut = FALSE;
	CString str;
	GetPrivateProfileStringW(L"EditPro", L"SetStrokeOut", L"", str.GetBufferSetLength(10), 10, ((CWinApp*)AfxGetApp())->m_pszProfileName);
	str.ReleaseBuffer();
	if(str == L"" || str == L"1")
		bStrokeOut = TRUE;
	//if(m_p_str_Text == L"")
	{
		for (int i=0; i < m_str_Text.GetLength(); i++)
		{
			BOOL bBreak = FALSE;
			if(bStrokeOut == TRUE)
			{
				AppendStrokeOut(i, bBreak);
				if(bBreak == TRUE)
					break;
			}else
			{
				AppendUnderLight(i, bBreak);
				if(bBreak == TRUE)
					break;
			}
		}	
		GetRichEditCtrl().UnlockWindowUpdate();
		GetRichEditCtrl().SetSel(0,0);
		GetRichEditCtrl().SetSel(GetRichEditCtrl().LineIndex(nFirstVisible),GetRichEditCtrl().LineIndex(nFirstVisible));
		GetRichEditCtrl().HideSelection(FALSE, FALSE);
	}

	m_p_str_Text = m_str_Text;m_p_str_Text_Version = m_str_Text_Version, m_p_str_Mod_History = m_str_Mod_History;
	if(bStrokeOut==TRUE)
		str = L"1";
	else
		str = L"0";
	WritePrivateProfileStringW(L"EditPro", L"SetStrokeOut", str.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
}

BOOL editView_Top_RichEdit::PreCreateWindow(CREATESTRUCT& cs)
{
	m_hinstRE41 = LoadLibrary(_T("msftedit.dll"));     

	return CRichEditView::PreCreateWindow(cs);
}

void editView_Top_RichEdit::SwitchMode()
{
	pFrame->SwitchingModeOnTop();
}

void editView_Top_RichEdit::OnEditAddcomment() 
{
	if(!bIsCommentMode)
		return;
	int pos, i_Select_Text_Len;  
	long start, end;
	GetRichEditCtrl().GetSel(start, end);
	pos = (int)start;
	i_Select_Text_Len = end - start;
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
		
	}
}

void editView_Top_RichEdit::OnShowcomment()
{
	if(!bIsCommentMode)
		return;
	CString p_str_Ver_Comment = m_pDoc->GetPStrVerComment();
	CString str_Ver_Comment = m_pDoc->m_strVersion_Comment;
	CString m_str_Text_Version_Temp = m_str_Text_Version;
	GetRichEditCtrl().LockWindowUpdate();
	GetRichEditCtrl().HideSelection(TRUE, FALSE);
	for(int i=0;i<str_Ver_Comment.GetLength();i++)
	{
		CString str_temp = this->m_str_Text_Version[i];
		if (p_str_Ver_Del_test1.Find(str_temp) >= 0)
		{
			str_Ver_Comment.Delete(i);
			m_str_Text_Version.Delete(i);
			i = -1;
		 }
	}
	for(int i=0;i<str_Ver_Comment.GetLength();i++)
	{	
		//GetRichEditCtrl().SetSel((i-1)<0?0:(i-1),i);
		
		GetRichEditCtrl().SetSel(i, i+1);
		CHARFORMAT2 cf;
		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_COLOR;
		GetRichEditCtrl().GetSelectionCharFormat(cf);
		if(p_str_Ver_Comment.Find(str_Ver_Comment.GetAt(i))>-1)
		{
			 cf.crBackColor = RGB(255,255,0);
			 if( cf.dwEffects & CFE_AUTOCOLOR )
				{ cf.dwEffects ^= CFE_AUTOCOLOR|CFM_BACKCOLOR;
				}
			GetRichEditCtrl().SetSelectionCharFormat(cf);
		}else
		{
			continue;
			cf.crBackColor = RGB(255,255,255);
			 if( cf.dwEffects & CFE_AUTOCOLOR ) 							 
				{ cf.dwEffects ^= CFE_AUTOCOLOR|CFM_BACKCOLOR;
				}
			GetRichEditCtrl().SetSelectionCharFormat(cf);
		}
	}
	GetRichEditCtrl().UnlockWindowUpdate();
	GetRichEditCtrl().SetSel(0,0);
	GetRichEditCtrl().HideSelection(FALSE, FALSE);
}

void editView_Top_RichEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(CheckRangeOnComment())
		;
	else
		CRichEditView::OnLButtonDblClk(nFlags, point);
}

BOOL editView_Top_RichEdit::CheckRangeOnComment()
{
	if(!bIsCommentMode)
		return FALSE;
	CString str, str_VerMark;
	long start, end;
	if(GetRichEditCtrl().GetSelText().GetLength()>0)
	{
		ShowCommentInToolTip();
		return TRUE;
	}
	GetRichEditCtrl().GetSel(start, end);

	CString p_str_Ver_Comment = m_pDoc->GetPStrVerComment();
	CString str_Ver_Comment = m_pDoc->m_strVersion_Comment;
	int iActualStart = m_pDoc->GetMarkPosition(start, false);
	int iActualEnd= m_pDoc->GetMarkPosition(end, false);
	int iDiff = iActualStart - start;
	str = str_Ver_Comment.GetAt(iActualStart);
	if(p_str_Ver_Comment.Find(str_Ver_Comment.GetAt(iActualStart))>-1)
	{
		str_VerMark = str_Ver_Comment.GetAt(iActualStart);
		start = str_Ver_Comment.Find(str_Ver_Comment.GetAt(iActualStart))-iDiff;
		end = str_Ver_Comment.ReverseFind(str_Ver_Comment.GetAt(iActualEnd))-iDiff;
		//GetRichEditCtrl().SetSel((start-1)<0?0:(start-1), end);
		GetRichEditCtrl().SetSel(start, end+1>str_Ver_Comment.GetLength()?str_Ver_Comment.GetLength():end+1);
		ShowCommentInToolTip();
		return TRUE;
	}else
		return FALSE;
}

void editView_Top_RichEdit::ShowCommentInToolTip()
{
	long start, end;
	GetRichEditCtrl().GetSel(start, end);
	if(GetRichEditCtrl().GetSelText().GetLength()>0)
	{
		 ClientToScreen(&ptLog);
		 CString strTemp;
		 CString strComment = m_pDoc->GetTextVersionComment(start, GetRichEditCtrl().GetSelText().GetLength());
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

UINT editView_Top_RichEdit::StartThread (LPVOID param)
{
	THREADSTRUCT*	ts = (THREADSTRUCT*)param;

	if(ts->_this->bThreadInProcess == TRUE)
		return 0;
	//here is the time-consuming process which interacts with dialog
	ts->_this->bThreadInProcess = TRUE;
	if(ts->_this->GetDebugFlag()==TRUE)
	{
		ts->_this->OnLogFile(L"Upper View Update Flag On");
	}
	CRect rc ;
	if(ts->_this->rc_standard==0)
	{
		ts->_this->GetClientRect(rc);	
		ts->_this->rc_standard = rc;
	}
	else
		rc = ts->_this->rc_standard;
	if(rc.bottom!=0)
		ts->_this->ShowWindow(FALSE);
	ts->_this->SetWindowText(ts->_this->m_str_Text); 
	if(!ts->_this->bIsCommentMode)
		ts->_this->setHistoryTextColor();
	else
		ts->_this->OnShowcomment();
	if(rc.bottom!=0)
	{
		ts->_this->ShowWindow(TRUE);
		ts->_this->MoveWindow(ts->_this->rc_standard);
	}
	ts->_this->SetVSrcollBar();
	rc = 0;
	ts->_this->bThreadInProcess = FALSE;
	if(ts->_this->GetDebugFlag()==TRUE)
	{
		ts->_this->OnLogFile(L"Upper View Update Flag Off");
	}
	return 1;
}

void editView_Top_RichEdit::OnDeletecomment() 
{
	if(!bIsCommentMode)
		return;
	int pos, i_Select_Text_Len;  
	long start, end;
	GetRichEditCtrl().GetSel(start, end);
	pos = (int)start;
	i_Select_Text_Len = end - start;

	m_pDoc->SetTextVersionComment(pos, i_Select_Text_Len, 0, true); 
	OnShowcomment();
}

void editView_Top_RichEdit::OnUpdateSwitchMode(CCmdUI* pCmdUI) 
{
	if(bThreadInProcess==TRUE)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if(bStartFlag==TRUE)
	{
		CString p_str_Ver_Comment = m_pDoc->GetPStrVerComment();
		CString str_Ver_Comment = m_pDoc->m_strVersion_Comment;
		bHasComment = FALSE;
		for(int i = 0; i<p_str_Ver_Comment.GetLength();i++)
		{
			if(str_Ver_Comment.Find(p_str_Ver_Comment.GetAt(i))>-1)
			{
				bHasComment = TRUE;
				break;
			}
		}
	}
	if(bHasComment==FALSE&&bIsCommentMode == TRUE)
		SwitchMode();
	//pCmdUI->Enable(bHasComment); 
	pCmdUI->Enable(FALSE); 
}

BOOL editView_Top_RichEdit::GetDebugFlag() 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(editDoc)));
	editDoc* m_pDocs = (editDoc*)m_pDocument;
	return m_pDocs->m_bEditorLog;
}

void editView_Top_RichEdit::OnLogFile(CString strMessage)
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(editDoc)));
	editDoc* m_pDocs = (editDoc*)m_pDocument;
	m_pDocs->OnLogFile(strMessage);
}

void editView_Top_RichEdit::OnSize(UINT nType, int cx, int cy) 
{
   CView::OnSize(nType, cx, cy);
   CRect rc;
   this->GetWindowRect(rc);
   if(rc.bottom != rc.top && pFrame!=NULL)
   {
	   CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
		editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();
		pView->StartUpdateUpperTimer();
		SetVSrcollBar();
   }
   if(rc.bottom == rc.top && pFrame!=NULL)
   {
	   CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
		editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();
		pView->KillUpdateUpperTimers();
   }
}

void editView_Top_RichEdit::OnUpdateCommand(CCmdUI* pCmdUI) 
{
		pCmdUI->Enable(FALSE); 
}

int editView_Top_RichEdit::GetCurVerEndPos(int iCurIndex) 
{
	int index = iCurIndex;
	CString strCurVerText = m_str_Text_Version[iCurIndex];
	for(int i=iCurIndex;i<m_str_Text_Version.GetLength();i++)
	{
		if(m_str_Text_Version[i]==strCurVerText)
			index++;
		else
			break;
	}
	return index;
}

BOOL editView_Top_RichEdit::PreTranslateMessage(MSG* pMsg) 
{
	/*CTime iCurrent = CTime::GetCurrentTime();
	if((iCurrent-iPastCaretTime).GetSeconds() >=1&& bFocus==TRUE)
	{
		iPastCaretTime = iCurrent;
		if(iFontHeight<0)
			iFontHeight = iFontHeight*-1;
		CreateSolidCaret(3,iFontHeight+5);
		ShowCaret();
	}*/
	return CRichEditView::PreTranslateMessage(pMsg);
}

void editView_Top_RichEdit::KillToopTips()
{
	m_Tips.HideTips();
}

int editView_Top_RichEdit::GetActualPos(int i_Pos)
{
	CString str_Temp;
	GetWindowText(str_Temp);
	int i_new_line_count = 0;

	for (int i=0; i < i_Pos; i++)
	{
		if (str_Temp[i] == L'\r\n')
			i_new_line_count = i_new_line_count +1;
	}

	return i_Pos - i_new_line_count;

}

void editView_Top_RichEdit::SetVSrcollBar()
{
	CRect rc;
	GetRichEditCtrl().GetWindowRect(rc);
	int totalLine = GetRichEditCtrl().GetLineCount();
	int size = totalLine*(iFontHeight/7.7*10.0)*-1;
	if(size < rc.bottom - rc.top)
	{
		//GetRichEditCtrl().SetScrollRange(SB_VERT, 0, rc.bottom - rc.top, TRUE);
		EnableScrollBar(SB_VERT,ESB_DISABLE_BOTH); 
	}else
	{
		//GetRichEditCtrl().SetScrollRange(SB_VERT, 0, rc.bottom - rc.top, TRUE);
		EnableScrollBar(SB_VERT,ESB_ENABLE_BOTH); 
	}
	ShowScrollBar(SB_VERT, TRUE);
	ShowScrollBar(SB_HORZ, FALSE);
}

void editView_Top_RichEdit::AppendStrokeOut(int &iPos, BOOL &bStop)
{
	int iEnd = GetCurVerEndPos(iPos);
	if(iEnd>m_str_Text.GetLength())
		iEnd = m_str_Text.GetLength();
	//pDC->SetTextColor(p_Color_test[GetVersion(i)]);
	//GetRichEditCtrl().SetSel(i, i+1);
	GetRichEditCtrl().SetSel(iPos, iEnd);
		
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	GetRichEditCtrl().GetSelectionCharFormat(cf);
	if(m_str_Text_Version.GetLength()==0)
	{
		GetRichEditCtrl().HideSelection(FALSE, FALSE);
		GetRichEditCtrl().UnlockWindowUpdate();
		return;
	}
	if(iPos>m_str_Text_Version.GetLength()-1)
	{
		bStop = TRUE;
		return;
	}
	CString str_temp = this->m_str_Text_Version[iPos];
	if (p_str_Ver_Del_test1.Find(str_temp) >= 0)
	{
		cf.dwEffects = CFE_STRIKEOUT;
	}
	//bool bDel;int iOldVer;GetVersion_New(iPos, bDel, iOldVer);
	//if(bDel==TRUE)
		//cf.crTextColor = p_Color_test1[iOldVer];
	//else
		cf.crTextColor = p_Color_test1[GetVersion(iPos)];
	
	if( cf.dwEffects & CFE_AUTOCOLOR )
	{ 
		cf.dwEffects ^= CFE_AUTOCOLOR;
	}
	GetRichEditCtrl().SetSelectionCharFormat(cf);
	iPos = iEnd-1;
}

void editView_Top_RichEdit::AppendUnderLight(int &iPos, BOOL &bStop)
{
	int iEnd = GetCurVerEndPos(iPos);
	if(iEnd>m_str_Text.GetLength())
		iEnd = m_str_Text.GetLength();
	GetRichEditCtrl().SetSel(iPos, iEnd);
		
	CHARFORMAT2 cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	GetRichEditCtrl().GetSelectionCharFormat(cf);
	if(m_str_Text_Version.GetLength()==0)
	{
		GetRichEditCtrl().HideSelection(FALSE, FALSE);
		GetRichEditCtrl().UnlockWindowUpdate();
		return;
	}
	if(iPos>m_str_Text_Version.GetLength()-1)
	{
		bStop = TRUE;
		return;
	}
	CString str_temp = this->m_str_Text_Version[iPos];
	if (p_str_Ver_Del_test1.Find(str_temp) >= 0)
	{
		//cf.dwEffects = CFE_STRIKEOUT;
		bool bDel;int iOldVer;GetVersion_New(iPos, bDel, iOldVer);
		cf.crTextColor = p_Color_test1[iOldVer];
		cf.crBackColor = p_Color_test1[GetVersion(iPos)];
		cf.dwMask= CFM_COLOR|CFM_UNDERLINE|CFM_BACKCOLOR;
        cf.dwEffects ^= CFE_AUTOCOLOR |CFE_UNDERLINE |CFE_AUTOBACKCOLOR;
	}
	else
	{
		cf.crTextColor = p_Color_test1[GetVersion(iPos)];
		if( cf.dwEffects & CFE_AUTOCOLOR )
		{ 
			cf.dwEffects ^= CFE_AUTOCOLOR;
		}
	}
	GetRichEditCtrl().SetSelectionCharFormat(cf);
	iPos = iEnd-1;
}