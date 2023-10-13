// editView_Bottom.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "editview_bottom.h"
#include "editframenew.h"
#include "resource.h"
#include "inputcommentdlg.h"

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

/////////////////////////////////////////////////////////////////////////////
// editView_Bottom

IMPLEMENT_DYNCREATE(editView_Bottom, CRichEditView)

editView_Bottom::editView_Bottom()
{
	m_pFontSet = NULL;
	m_pStatusBar = NULL;
	m_pDoc = NULL;
	m_pPopupMenu = NULL;
	m_b_on_paste = false;
	m_b_disable_modify = false;
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
	
	ReleaseStatusBar();

}


BEGIN_MESSAGE_MAP(editView_Bottom, CRichEditView)
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
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
//	ON_UPDATE_COMMAND_UI(ID_DO_COMMIT, OnUpdateDoCommit)
	ON_UPDATE_COMMAND_UI(ID_DO_MODIFY, OnUpdateDoModify)
	ON_UPDATE_COMMAND_UI(ID_EDIT_READONLY, OnUpdateEditReadOnly)
	ON_COMMAND(ID_EDIT_READONLY, OnEditReadOnly)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SHOWCHARCODE, OnUpdateEditShowcharcode)
	ON_COMMAND(ID_EDIT_SHOWCHARCODE, OnEditShowcharcode)
	ON_COMMAND(ID_EDIT_ADDCOMMENT, OnEditAddcomment)
	ON_COMMAND(ID_EDIT_SHOWCOMMENT, OnEditShowcomment)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_UP,ID_DEL_MARK,OnInsertWords)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// editView_Bottom drawing

void editView_Bottom::OnDraw(CDC* pDC)
{
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
		GetClassName(pChildWnd->GetSafeHwnd(), (unsigned short *)szClass, sizeof(szClass));
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
	char szSection[TMPSTR_SIZE];
	CString str;
	nStyleNum = pApp->GetProfileInt(L"EditPro", L"TextStyleNum", 1);
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
			str = pApp->GetProfileString((const unsigned short *)szSection, L"StyleName", L"Normal");
			break;
		}
		pSubMenu->AppendMenu(MF_STRING, ID_TS_STYLE1+i, str);
	}


  return 0;
}

BOOL editView_Bottom::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CRichEditView::PreTranslateMessage(pMsg);
}


void editView_Bottom::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	m_pDoc = GetDocument();           
	
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
//	lstrcpy(lf.lfFaceName, L"·s²Ó©úÅé"); 
	lstrcpy(lf.lfFaceName, L"Arial Unicode MS"); 
	pFont->CreateFontIndirect(&lf);

//	SetFont(pFont);

//	CFont* m_Font = new CFont;
  //  LOGFONT lf;
//	memset(&lf, 0, sizeof(lf));
  //  GetEditCtrl().GetFont()->GetLogFont(&lf);
//	lf.lfHeight = -13;
  //  m_Font->CreateFontIndirect(&lf);

	
	//    GetEditCtrl().SetFont(pFont);
    GetRichEditCtrl().SetFont(pFont);


if (m_b_allow_edit)
	GetRichEditCtrl().SetReadOnly(TRUE);
else
	GetRichEditCtrl().SetReadOnly(FALSE);

InitStatusBar();

//GetEditCtrl(). 

//	OnOptionChange();
}

void editView_Bottom::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{ 
	if (!m_b_allow_edit) return;

	int i_Select_Text_Len = 1;

	BOOL bShift=GetKeyState(VK_SHIFT)&0x8000;
	BOOL bCtrl=GetKeyState(VK_CONTROL)&0x8000;	

  int   nStartChar,   nEndChar;   
  GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	
	switch(nChar)
	{
		case 'Y':
			if (bCtrl > 0)
			{
				this->OnEditRedo();
				return;
			}
			break;

		case 'A':
			if (bCtrl > 0)
				return;
			break;
			

		case VK_DELETE:
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

		url.lLen = i_temp_len;

		if (b_Select_Text)
			url.lStartPos = nStartChar;
		else
			url.lStartPos = nEndChar;

		m_pDoc->m_arrUndo.Add(url);
		// clear redo list
		m_pDoc->m_arrRedo.RemoveAll(); 

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
			return;
	}
	
	return;

}

#define		INPUT_BUFFER_SIZE		100
void editView_Bottom::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!m_b_allow_edit) return;

	BOOL bCtrl=GetKeyState(VK_CONTROL)&0x8000;	
	
	switch(nChar)
	{
		case 25:
			return;

		case 1:
			return;
			
		break;
	}
	
	
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
					

	bool b_Select_Text = false;
	if (i_Select_Text_Len > 0) b_Select_Text = true;


	int nCharIndex = nEndChar;

	if (!(GetKeyState(VK_CONTROL) & 0x8000))
	{   

	    UINT uChar;
//	    MSG	 msg;
	    
		uChar = nChar & 0xff;
		switch(uChar)
		{
		    case VK_BACK:
				{
					// add an item to undo list
					UNDOREDOLOG url;
					url.bIsAdd = true;
					url.lSeq = 0;
					int i_temp_len = i_Select_Text_Len;
					
					if (i_temp_len==0) i_temp_len = 1;

					int i_temp_len_back = i_temp_len;


					bool b_new_line = false;

					if (b_Select_Text)
						url.strRedoUndo = GetTextFromPoint_Len(nStartChar , nLen, b_new_line, true, b_Select_Text) ;
					else
						url.strRedoUndo = GetTextFromPoint_Len(nEndChar-i_temp_len , i_temp_len, b_new_line, true, b_Select_Text) ;
					
					if (b_new_line) i_temp_len_back = i_temp_len_back +1;

					url.lLen = i_temp_len;
					
					if (b_Select_Text)
						url.lStartPos = nStartChar;
					else 
						url.lStartPos = nEndChar-i_temp_len_back;

					m_pDoc->m_arrUndo.Add(url);
					m_pDoc->m_arrRedo.RemoveAll(); 
		
					CView::OnChar(nChar, nRepCnt, nFlags);
					if (i_Select_Text_Len == 0) i_Select_Text_Len = 1;
					OnEditDelete(i_Select_Text_Len);
					return;
				}

			default:
				break;
		} 
	}


	int nCharIndex_real = GetActualPos(nCharIndex);
	m_pDoc->DeleteTextVersionPos(nCharIndex_real-i_Select_Text_Len, i_Select_Text_Len);  
	m_pDoc->UpdateTextVersionPos((CString)nChar, nCharIndex_real-i_Select_Text_Len, false);  

	SetTextToUpperView();

	CView::OnChar(nChar, nRepCnt, nFlags);

	// add a record to undo List 
	UNDOREDOLOG url;
	url.bIsAdd = false;
	url.lSeq = 0;

	if (nChar == 13)
		url.strRedoUndo = L"\r\n";
	else	
		url.strRedoUndo = (CString)nChar;

	int i_len = url.strRedoUndo.GetLength(); 
	
	url.lLen = 1;
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

	return;
	// 

}

void editView_Bottom::OnInsertWords(UINT nID )
{
	if (!m_b_allow_edit) return;

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
		strText = wTemp;

    int nCharIndex = nEndChar;
	int nCharIndex_real = GetActualPos(nCharIndex);

	CString strStart, strEnd;
	
	strStart = GetTextFromPoint(nCharIndex-nLen,true);
	strEnd = GetTextFromPoint(nCharIndex,false);
   
	SetWindowText(strStart + strText + strEnd);

	m_pDoc->DeleteTextVersionPos(nCharIndex_real-i_Select_Text_Len, i_Select_Text_Len);  
	
	m_pDoc->UpdateTextVersionPos(strText, nCharIndex_real-i_Select_Text_Len, false);  
	SetTextToUpperView();

	CEdit& edit = GetEditCtrl();
	edit.SetSel(nCharIndex+1, nCharIndex+1);

	// add a record to undo List 
	UNDOREDOLOG url;
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
//	SetPrivateBufferPtr();
//	InsertText( (BYTE *)szInputBuffer, 2 );
//	m_pDoc->LockDocBuffer(FALSE);
}

//
//proedure call when user press delete or 'backspace' 
// 
void editView_Bottom::OnEditDelete(int i_select_text_len)
{
    int   nStartChar,   nEndChar;   
    GetEditCtrl().GetSel(nStartChar,  nEndChar);   	
	
    int nCharIndex = nEndChar;
	nCharIndex = GetActualPos(nCharIndex);

	CString strStart, strEnd;
	m_pDoc->DeleteTextVersionPos(nCharIndex, i_select_text_len);  
//	m_pDoc->DeleteTextVersionPos(3, i_select_text_len);  

	SetTextToUpperView();
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

		if(OpenClipboard()) //OpenClipboard(NULL) gives me error
		{
			EmptyClipboard();
			HLOCAL clipbuffer = LocalAlloc(0, nLen   *   4);
			wcscpy((WCHAR*) clipbuffer, strSelectText);

			SetClipboardData(CF_UNICODETEXT, clipbuffer);
			CloseClipboard();
			//free(szMsg); //not sure what 'szMsg' is
			//LocalFree(clipbuffer);

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
	
	if (m_b_on_paste) 
	{
		m_b_on_paste = false;
		return 1;
	}
	wchar_t* buffer;
	CString strtemp;
	if(OpenClipboard())
	{
		buffer = (wchar_t*)GetClipboardData(CF_UNICODETEXT);
		CloseClipboard();
	}
	
    CString str_buffer = buffer;
	str_buffer.Replace(L"\n",L""); 

    int nCharIndex = nEndChar;
	int nCharIndex_real = GetActualPos(nCharIndex);

	CString strStart, strEnd;
	
	strStart = GetTextFromPoint(nCharIndex-nLen,true);

	strEnd = GetTextFromPoint(nCharIndex,false);
   
	SetWindowText(strStart + buffer + strEnd);

	m_pDoc->DeleteTextVersionPos(nCharIndex_real-i_Select_Text_Len, i_Select_Text_Len);  
	
	m_pDoc->UpdateTextVersionPos(str_buffer, nCharIndex_real-i_Select_Text_Len, false);  

	
	SetTextToUpperView();

	CString str_temp = str_buffer;
	str_temp.Replace(L"\r",L"\r\n"); 


	CEdit& edit = GetEditCtrl();
	int i_pos = nCharIndex+str_temp.GetLength();
	//	int len;
	//len = edit.GetWindowTextLength();
	edit.SetSel(i_pos, i_pos);
	
	UNDOREDOLOG url;
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

    return 1;
}

int editView_Bottom::GetActualPos(int i_Pos)
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

CString editView_Bottom::GetTextFromPoint_Len(int i_Pos, int i_len, bool &b_new_line, bool b_backspace, bool b_select_text)
{
	CString strText = L"";
	GetWindowText(strText);

	strText = strText.Left(i_Pos+ i_len); 


	strText = strText.Right(i_len); 

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
				SetWindowText(strStart + strEnd);
			
			CEdit& edit = GetEditCtrl();
			edit.SetSel(url.lStartPos , url.lStartPos);
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
				
				SetWindowText(strStart + strtemp + strEnd);

				CString str_buffer = url.strRedoUndo_Extra;
				str_buffer.Replace(L"\n",L""); 
				
				CEdit& edit = GetEditCtrl();
				edit.SetSel(url.lStartPos , url.lStartPos);

				int nCharIndex_real = GetActualPos(url.lStartPos);

				m_pDoc->UpdateTextVersionPos(str_buffer, nCharIndex_real, true);  
				SetTextToUpperView();

				edit.SetSel(url.lStartPos + i_temp_len, url.lStartPos + i_temp_len);

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
			
			SetWindowText(strStart + strtemp + strEnd);

		    CString str_buffer = url.strRedoUndo;
			str_buffer.Replace(L"\n",L""); 
			
			CEdit& edit = GetEditCtrl();
			edit.SetSel(url.lStartPos , url.lStartPos);

			int nCharIndex_real = GetActualPos(url.lStartPos);

			m_pDoc->UpdateTextVersionPos(str_buffer, nCharIndex_real, true);  
			SetTextToUpperView();

			edit.SetSel(url.lStartPos + i_temp_len, url.lStartPos + i_temp_len);

			// add a record to redo LIST
			UNDOREDOLOG url_redo;
			url_redo.bIsAdd = false;
			url_redo.lSeq = 0;
			url_redo.strRedoUndo = url.strRedoUndo;
			url_redo.lLen = url.lLen;
			url_redo.lStartPos = url.lStartPos;
			m_pDoc->m_arrRedo.Add(url_redo);	
		}

		// remove the current undo item
		m_pDoc->m_arrUndo.RemoveAt(m_pDoc->m_arrUndo.GetSize()-1);
	
	}


}

void editView_Bottom::OnEditRedo() 
{
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
				
				SetWindowText(strStart + strEnd);
				
				CEdit& edit = GetEditCtrl();
				edit.SetSel(url.lStartPos , url.lStartPos);

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
			
			SetWindowText(strStart + strtemp + strEnd);
			
			CEdit& edit = GetEditCtrl();
			edit.SetSel(url.lStartPos , url.lStartPos);

			int nCharIndex_real = GetActualPos(url.lStartPos);

		    CString str_buffer = url.strRedoUndo;

			
			str_buffer.Replace(L"\n",L""); 

			m_pDoc->UpdateTextVersionPos(str_buffer, nCharIndex_real, false);  
			SetTextToUpperView();

			edit.SetSel(url.lStartPos + i_temp_len  , url.lStartPos + i_temp_len);

			// add a record to undo LIST
			UNDOREDOLOG url_undo;
			url_undo.bIsAdd = false;
			url_undo.lSeq = 0;
			url_undo.strRedoUndo = url.strRedoUndo;
			url_undo.lLen = url.lLen;
			url_undo.lStartPos = url.lStartPos;

			url_undo.bExtra = url.bExtra;
			url_undo.strRedoUndo_Extra = url.strRedoUndo_Extra;
			
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
			
			SetWindowText(strStart + strEnd);
			
			CEdit& edit = GetEditCtrl();
			edit.SetSel(url.lStartPos , url.lStartPos);
			OnEditDelete(url.lLen);

			// add a record to undo LIST
			UNDOREDOLOG url_undo;
			url_undo.bIsAdd = true;
			url_undo.lSeq = 0;
			url_undo.strRedoUndo = url.strRedoUndo;
			url_undo.lLen = url.lLen;
			url_undo.lStartPos = url.lStartPos;
			m_pDoc->m_arrUndo.Add(url_undo);	

		}
		
		// remove the current undo redo item
		m_pDoc->m_arrRedo.RemoveAt(m_pDoc->m_arrRedo.GetSize()-1);
	}	
	
}

void editView_Bottom::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	if (m_pDoc->m_arrUndo.GetSize() >0 && m_b_allow_edit)
		pCmdUI->Enable(TRUE); 
	else
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
*/
void editView_Bottom::OnUpdateDoModify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	if (m_b_disable_modify)
		pCmdUI->Enable(FALSE); 
	else
		pCmdUI->Enable(TRUE); 
}

HBRUSH editView_Bottom::CtlColor(CDC* pDC, UINT nCtlColor) 
{
// 	pDC->SetBkColor(RGB(255, 255, 255));


//	CFont font;
//	VERIFY(font.CreatePointFont(100,L"Arial Unicode MS"));
//	pDC->SelectObject(&font);

	pDC->SetTextColor(RGB(0, 110, 255));


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
	// display the text 
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
//	GetWindowText(strText);

	((editFrameNew*)pChild)->SetTextToUpperView(m_pDoc->m_strVersion, m_pDoc->m_strVersion_Mark, m_pDoc->m_strModHistory);
}


void editView_Bottom::SetReadOnly(BOOL bReadOnly)
{

	GetEditCtrl().SetReadOnly(bReadOnly);
	m_b_allow_edit = !bReadOnly; 
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
		char szTemp1[10]; 
		itoa(*(converted+i), szTemp1, 16);

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


	CString str;
	 int   nStartChar,   nEndChar;   

	 GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	ASSERT((UINT)nEndChar   <=   GetBufferLength());   
	
	str.Format(L"%ld", 100);
	m_pStatusBar->SetPaneText(SB_WORDS, str);

	str.Format(L"%ld", GetEditCtrl().LineFromChar(nEndChar)+1);	
	m_pStatusBar->SetPaneText(SB_LINES, str);

	str = "";
	if (!m_b_allow_edit)
		str = L"°ßÅª";
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
	str = L"°ßÅª";
	size = dc.GetTextExtent( str, str.GetLength() );
	cxWidth = size.cx;
	m_pStatusBar->SetPaneInfo( SB_FILESTATUS, uID, SBPS_NORMAL, cxWidth);

	if (m_pDoc)
		DisplayDocInfo();
}

void editView_Bottom::OnRButtonDown(UINT nFlags, CPoint point) 
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
	CView::OnRButtonDown(nFlags, point);
}

void editView_Bottom::OnEditAddcomment() 
{
//	if ( !m_pDoc->LockDocBuffer( TRUE ) )
//		return;
//	SetPrivateBufferPtr();

	CInputCommentDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
//		m_pDoc->AddComment(dlg.m_szComment);
//		UpdateSelection();
	}

//	m_pDoc->LockDocBuffer(FALSE);
}

void editView_Bottom::OnEditShowcomment() 
{

	  CString strSelectText = L""; 
  	  int   nStartChar,   nEndChar;   
	  GetEditCtrl().GetSel(nStartChar,   nEndChar);   
	  ASSERT((UINT)nEndChar   <=   GetBufferLength());   
	  LPCTSTR   lpszText   =   LockBuffer();   
	  UINT   nLen   =   nEndChar   -   nStartChar;   
	  memcpy(strSelectText.GetBuffer(nLen), lpszText + nStartChar,  nLen   *   sizeof(TCHAR));   

	m_pStatusBar->SetPaneText(SB_STATUS, strSelectText);
	  

	  strSelectText.ReleaseBuffer(nLen);   
	  UnlockBuffer(); 	





}


