// editFrameNew.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "editframenew.h"
#include "editdoc.h"
#include "editview_top.h"
#include "editview_top_RichEdit.h"
#include "editview_bottom.h"
#include "DlgDisplyPicture.h"

// used to define the upper view on editor is using Scroll View or RichEditView
//#define EDIT_RICHEDIT
#define EDIT_SCROLL    // EDIT_RICHEDIT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern HACCEL g_hAccel;
/////////////////////////////////////////////////////////////////////////////
// editFrameNew

IMPLEMENT_DYNCREATE(editFrameNew, CMDIChildWnd)

editFrameNew::editFrameNew()
{
	m_pDlgDisplyPicture = NULL;	
}

editFrameNew::~editFrameNew()
{
	if(m_pDlgDisplyPicture != NULL)
	{
		if(m_pDlgDisplyPicture->GetSafeHwnd() != NULL)
			m_pDlgDisplyPicture->DestroyWindow();
		delete m_pDlgDisplyPicture;
		m_pDlgDisplyPicture = NULL;
	}
}


BEGIN_MESSAGE_MAP(editFrameNew, CMDIChildWnd)
	//{{AFX_MSG_MAP(editFrameNew)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(ID_COMBO, OnSelchangeCombo)
	ON_CBN_SETFOCUS(ID_COMBO, OnFocusCombo)
	ON_CBN_KILLFOCUS(ID_COMBO, OnKillFocusCombo)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PICTURE, OnCaptionPicture)
	ON_UPDATE_COMMAND_UI(ID_FILE_PICTURE, OnUpdateCaptionPicture)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// editFrameNew message handlers
BOOL MergeMenu(CMenu * pMenuDestination, const CMenu * pMenuAdd, bool bTopLevel =FALSE);


BOOL editFrameNew::OnCreateClient( LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	if (!m_wndSplitter.CreateStatic(this, 2, 1))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	bScrollView = FALSE;
	#ifdef EDIT_RICHEDIT
 	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(editView_Top_RichEdit), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->pFrame = this;
	 #else ifdef EDIT_SCROLL
 	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(editView_Top), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	((editView_Top*)m_wndSplitter.GetPane(0,0))->pFrame = this;
	bScrollView = TRUE;
	 #endif
	/*if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(editView_Top), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}*/

	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(1, 0,
		pContext->m_pNewViewClass, CSize(200, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(1,0));

	return TRUE;
}

void editFrameNew::SetTextToUpperView(CString strText, CString strTextVersion, CString strModHistory)
{
	//SetActiveView((CView*)m_wndSplitter.GetPane(1,0));
	#ifdef EDIT_RICHEDIT
		CRect rc;
		m_wndSplitter.GetWindowRect(rc);
		if(rc.bottom<=0)
			return;
		editView_Top_RichEdit* pView = (editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0);
		if(pView==NULL)
			return;
 		((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->SetContentText(strText, strTextVersion, strModHistory);
	 #else ifdef EDIT_SCROLL
		CRect rc;
		m_wndSplitter.GetWindowRect(rc);
		if(rc.bottom<=0)
			return;
		editView_Top* pView = (editView_Top*)m_wndSplitter.GetPane(0,0);
		if(pView==NULL)
			return;
 		((editView_Top*)m_wndSplitter.GetPane(0,0))->SetContentText(strText, strTextVersion, strModHistory);
	 #endif
		
}

void editFrameNew::ChangeUpperViewFont()
{
	#ifdef EDIT_RICHEDIT
 		((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->UpdateFontSet();
	 #else ifdef EDIT_SCROLL
		((editView_Top*)m_wndSplitter.GetPane(0,0))->UpdateFontSet();
	 #endif
}


int editFrameNew::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
#ifndef _DISEABLECAPTIONPICTRUE_
	if ((!CGlobal::GetInstance()->m_bPrintDirect) &&((CGlobal::GetInstance()->m_enOpenFileType == CAPFILE) 
		|| (CGlobal::GetInstance()->m_enOpenFileType == NETFILE 
					&& CGlobal::GetInstance()->m_arrRelPicFile.GetSize() > 0)))
	{
		if(m_pDlgDisplyPicture == NULL)
		{
			m_pDlgDisplyPicture = new CDlgDisplyPicture;
		}
		if(m_pDlgDisplyPicture == NULL)
			return -1;
	}
#endif

	m_bCaptionPicShowed = FALSE;
	CreateToolBarOnMainFrm(IDR_EDITTOOLBAR);
	// TODO: Add your specialized creation code here
	CreateModifyToolbar();
	
	return 0;
}

BOOL RemoveMenu(CMenu * pMenuDestination, const CMenu * pMenuDel, bool bTopLevel =false);

void editFrameNew::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	CGlobal *pGlobal = CGlobal::GetInstance();

	// TODO: Add your message handler code here
#ifdef EMBED_TOOLBAR
	if(!m_bCreateToolbar)return;
	if(bActivate)
		((CFrameWnd*)AfxGetMainWnd())->ShowControlBar(&m_wndToolBar,TRUE,FALSE);
	else
		((CFrameWnd*)AfxGetMainWnd())->ShowControlBar(&m_wndToolBar,FALSE,FALSE);
#endif


	// TODO: Add your message handler code here
	if (bActivate)
	{
#ifndef _DISEABLECAPTIONPICTRUE_
		CGlobal *pGlobal = CGlobal::GetInstance();
		if(m_pDlgDisplyPicture != NULL && (pGlobal->m_enOpenFileType == CAPFILE 
			|| (pGlobal->m_enOpenFileType == NETFILE 
					&& pGlobal->m_arrRelPicFile.GetSize() > 0
					)) )
		{
			if(m_pDlgDisplyPicture->GetSafeHwnd() == NULL)
			{
				CResManage myRes(g_hInst);
				m_pDlgDisplyPicture->Create(IDD_DLGPICTURE, this);
				RECT rcFrame;
				this->GetActiveView()->GetWindowRect(&rcFrame);
				m_pDlgDisplyPicture->SetFrameRect(rcFrame);
				if(pGlobal->m_enOpenFileType == CAPFILE)
					m_pDlgDisplyPicture->SetPictureFile(pGlobal->m_strCaptionPicFile);
				else if(pGlobal->m_enOpenFileType == NETFILE)
					m_pDlgDisplyPicture->SetPictureFile(pGlobal->m_arrRelPicFile);
				m_pDlgDisplyPicture->ShowCaptionPicture();
			}
			m_pDlgDisplyPicture->ShowWindow(SW_NORMAL);
			this->SetFocus();
			m_bCaptionPicShowed = TRUE;
		}
#endif
		
		editDoc* pDoc = (editDoc*)GetActiveDocument();

		if(!pDoc->m_hDefaultMenu)
		{
			pDoc->m_hDefaultMenu = 
				::LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_EDITROTYPE));
			if (pDoc->m_hDefaultMenu == NULL)
				return;
		}
		

//		if(pGlobal->m_enOpenFileType != PUBLISHTEXTMULTI)
//		{
			CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
			pFrame->RecalcLayout();
			//pFrame->MDISetMenu(CMenu::FromHandle(pDoc->m_hDefaultMenu), NULL);
			CMenu *pMainMenu=pFrame->GetMenu();
			CMenu *pEditMenu=CMenu::FromHandle(pDoc->m_hDefaultMenu);
			if (pMainMenu && pEditMenu)
				MergeMenu(pMainMenu,pEditMenu,TRUE);
			pFrame->DrawMenuBar();
//		}
	}
	else
	{
#ifndef _DISEABLECAPTIONPICTRUE_

		//if(CGlobal::GetInstance()->m_enOpenFileType == CAPFILE)
		if(m_pDlgDisplyPicture->GetSafeHwnd() != NULL)
		{
			m_pDlgDisplyPicture->ShowWindow(SW_HIDE);
			m_bCaptionPicShowed = FALSE;
		}
#endif
		//AfxGetMainWnd()->SendMessage(WM_RELOADMENU,0,0);
		editDoc* pDoc = (editDoc*)GetActiveDocument();
		
		if(!pDoc->m_hDefaultMenu)
		{
			pDoc->m_hDefaultMenu = 
				::LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_EDITROTYPE));
			if (pDoc->m_hDefaultMenu == NULL)
				return;
		}
		
//		if(pGlobal->m_enOpenFileType != PUBLISHTEXTMULTI)
//		{
			CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
			pFrame->RecalcLayout();
			//pFrame->MDISetMenu(CMenu::FromHandle(pDoc->m_hDefaultMenu), NULL);
			CMenu *pMainMenu=pFrame->GetMenu();
			CMenu *pEditMenu=CMenu::FromHandle(pDoc->m_hDefaultMenu);
			if (pMainMenu && pEditMenu)
				RemoveMenu(pMainMenu,pEditMenu,TRUE);
			pFrame->DrawMenuBar();

//		}
	}

}
int CALLBACK EnumFontFamProc(
  ENUMLOGFONT *lpelf,    // logical-font data
  NEWTEXTMETRIC *lpntm,  // physical-font data
  DWORD nFontType,        // type of font
  LPARAM lParam          // application-defined data
)
{
	editFrameNew* pWnd=(editFrameNew*)lParam;
	pWnd->m_comboBox.AddString(lpelf->elfLogFont.lfFaceName);


	return 1;
}

int editFrameNew::CreateToolBarOnMainFrm(UINT nID,CWnd *pParent)
{
	CWnd *pWnd=NULL;
#ifdef EMBED_TOOLBAR
	pWnd = pParent;
	if(pWnd==NULL)
		pWnd=AfxGetMainWnd();
#else
	pWnd = this;
#endif
	if (!m_wndToolBar.CreateEx(pWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI|CBRS_SIZE_DYNAMIC   ) ||
		!m_wndToolBar.LoadToolBar(nID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
#ifdef _DISEABLECAPTIONPICTRUE_
		CResManage myRes(g_hInst);
		for(int iIndex  = 0; ; iIndex++)
		{
			if(m_wndToolBar.GetItemID(iIndex) == ID_FILE_PICTURE)
			{
				m_wndToolBar.GetToolBarCtrl().DeleteButton(iIndex);
				m_wndToolBar.GetToolBarCtrl().DeleteButton(iIndex -1);
				break;
			}
		}
#else
		if( (CGlobal::GetInstance()->m_enOpenFileType != CAPFILE) 
			&& (!(CGlobal::GetInstance()->m_enOpenFileType == NETFILE 
						&& CGlobal::GetInstance()->m_arrRelPicFile.GetSize() > 0)) )
		{
			CResManage myRes(g_hInst);
			for(int iIndex  = 0; ; iIndex++)
			{
				if(m_wndToolBar.GetItemID(iIndex) == ID_FILE_PICTURE)
				{
					m_wndToolBar.GetToolBarCtrl().DeleteButton(iIndex);
					m_wndToolBar.GetToolBarCtrl().DeleteButton(iIndex -1);
					break;
				}
			}
		}
#endif		
	m_ToolBarImageList.Create(24,24,ILC_COLOR24|ILC_MASK,0,1);
	m_ToolBarDisableImageList.Create(24,24,ILC_COLOR24|ILC_MASK,0,1);
	
	SetToolBarBitmap();

	if(CGlobal::GetInstance()->m_pShareData->m_bSupportPubStory == FALSE)
		m_wndToolBar.GetToolBarCtrl().HideButton(ID_ADD_STORY,1);
	
	CRect rect;
	int nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_COMBO);
	m_wndToolBar.SetButtonInfo(nIndex, ID_COMBO, TBBS_SEPARATOR, 205);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.top = 1;
	rect.bottom = rect.top + 250 /*drop height*/;
	if(!m_comboBox.Create(CBS_DROPDOWNLIST | CBS_SORT | WS_VISIBLE |
		WS_TABSTOP | WS_VSCROLL, rect, &m_wndToolBar, ID_COMBO))
	{
		TRACE(_T("Failed to create combo-box\n"));
		return FALSE;
	}

	nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_MSGDLG);
    //next convert that button to a seperator and get its position
    m_wndToolBar.SetButtonInfo(nIndex, ID_MSGDLG, 
		TBBS_SEPARATOR, 550);
    m_wndToolBar.GetItemRect(nIndex, &rect);
	m_dlgMsg.Create(IDD_MSG,&m_wndToolBar);
	m_dlgMsg.MoveWindow(rect);
	m_dlgMsg.ShowWindow(SW_SHOW);

	//edit by leon 2012-09-04
// 	CRect rect1;
// 	int nIndex1 = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_STYLE);
// 	m_wndToolBar.SetButtonInfo(nIndex1, ID_STYLE, TBBS_SEPARATOR, 205);
// 	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex1, &rect1);
// 	rect1.top = 1;
// 	rect1.bottom = rect1.top + 250 /*drop height*/;
// 	if(!m_comboBox.Create(CBS_DROPDOWNLIST | CBS_SORT | WS_VISIBLE |
// 		WS_TABSTOP | WS_VSCROLL, rect1, &m_wndToolBar, ID_STYLE))
// 	{
// 		TRACE(_T("Failed to create combo-box\n"));
// 		return FALSE;
// 	}

	m_comboBox.ResetContent();
	CClientDC dc(this);
	::EnumFontFamilies((HDC)dc,NULL,(FONTENUMPROC)EnumFontFamProc,(LPARAM)this);

	CWinApp *pApp = AfxGetApp();
	CString szSection;
	szSection.Format(L"EditProTextStyle%ld", 1);
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	CString str_temp ;//= pApp->GetProfileString(szSection, L"FaceName", L"");
	wchar_t sz_temp[255];
	GetPrivateProfileStringW(szSection , L"FaceName",L"",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp = sz_temp;
	//end 20120927

	CString str_Cur = L"";
	for (int i=0; i < m_comboBox.GetCount(); i++)
	{
		m_comboBox.GetLBText(i, str_Cur);
		if (str_Cur == str_temp)
		{
			m_comboBox.SetCurSel(i); 
			break;
		}

	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

#ifdef EMBED_TOOLBAR
	AfxGetMainWnd()->SendMessage(WM_DOCKTOOLBARLEFTOF,(UINT)&m_wndToolBar);
#endif
	m_wndToolBar.SetOwner(pWnd);
	m_bCreateToolbar = TRUE;
	return 0;
}


void editFrameNew::SetToolBarBitmap()
{
	CResManage myRes(g_hInst);
	
	CBitmap bitmap;
	//提交稿件
	bitmap.LoadBitmap(IDB_BITMAP_SUBMIT);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//復原鍵入
	bitmap.LoadBitmap(IDB_BITMAP_UNDO);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//重復鍵入
	bitmap.LoadBitmap(IDB_BITMAP_REDO);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//修改
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//剪下
	bitmap.LoadBitmap(IDB_BITMAP_CUT);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//複製
	bitmap.LoadBitmap(IDB_BITMAP_COPY);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//貼上
	bitmap.LoadBitmap(IDB_BITMAP_PASTE);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//預覽列印
	bitmap.LoadBitmap(IDB_BITMAP_PRINTPREVIEW);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//列印
	bitmap.LoadBitmap(IDB_BITMAP_PRINT);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	if(m_pDlgDisplyPicture != NULL)
	{
		//顯示圖片
		bitmap.LoadBitmap(IDB_BITMAP_CAPTIONPIC);
		m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
		bitmap.DeleteObject();
	}else
	{
		//字詞檢查
		bitmap.LoadBitmap(IDB_BITMAP_CHECK);
		m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
		bitmap.DeleteObject();
	}
	
	//字詞檢查
	bitmap.LoadBitmap(IDB_BITMAP_CHECK);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//批註 <-> 編寫 模式
	bitmap.LoadBitmap(IDB_BITMAP_SWITCH_MODE);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//Add ToStory
	if(CGlobal::GetInstance()->m_pShareData->m_bSupportPubStory == TRUE)
	{
		bitmap.LoadBitmap(IDB_BITMAP_ADD_STORY);
		m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
		bitmap.DeleteObject();
	}

	//Now, set the disabled toolbar

	//提交稿件
	bitmap.LoadBitmap(IDB_BITMAP_SUBMIT_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//復原鍵入
	bitmap.LoadBitmap(IDB_BITMAP_UNDO_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//重復鍵入
	bitmap.LoadBitmap(IDB_BITMAP_REDO_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//修改
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//剪下
	bitmap.LoadBitmap(IDB_BITMAP_CUT_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//複製
	bitmap.LoadBitmap(IDB_BITMAP_COPY_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//貼上
	bitmap.LoadBitmap(IDB_BITMAP_PASTE_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//預覽列印
	bitmap.LoadBitmap(IDB_BITMAP_PRINTPREVIEW_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//列印
	bitmap.LoadBitmap(IDB_BITMAP_PRINT_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();

	if(m_pDlgDisplyPicture != NULL)
	{
		//顯示圖片
 		bitmap.LoadBitmap(IDB_BITMAP_CAPTIONPIC);
 		m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
 		bitmap.DeleteObject();
	}else
	{
		//字詞檢查
		bitmap.LoadBitmap(IDB_BITMAP_CHECK_DISABLE);
		m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
		bitmap.DeleteObject();
	}

	//字詞檢查
	bitmap.LoadBitmap(IDB_BITMAP_CHECK_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();

	//批註\編寫模式
	bitmap.LoadBitmap(IDB_BITMAP_SWITCH_MODE_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();

	//Add ToStory
	if(CGlobal::GetInstance()->m_pShareData->m_bSupportPubStory == TRUE)
	{
		bitmap.LoadBitmap(IDB_BITMAP_ADD_STORY_DISABLE);
		m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
		bitmap.DeleteObject();
	}


	CImageList *pImagelist=m_wndToolBar.GetToolBarCtrl().GetImageList();
	if(pImagelist)
		pImagelist->DeleteImageList();
	pImagelist=m_wndToolBar.GetToolBarCtrl().GetDisabledImageList();
	if(pImagelist)
		pImagelist->DeleteImageList();

	m_wndToolBar.GetToolBarCtrl().SetButtonSize(TOOLBARBUTTONSIZE);
	m_wndToolBar.GetToolBarCtrl().SetBitmapSize(TOOLBARBITMAPSIZE);
	
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ToolBarImageList);
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ToolBarDisableImageList);
}

void editFrameNew::OnDestroy() 
{

	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
//	AfxGetMainWnd()->SendMessage(WM_RELOADMENU,0,0);
	
}

void editFrameNew::CreateModifyToolbar()
{
//	if (g_nSpecifyFontAndToolbar == 0)
//		return;
	CString strUseDefFontEditFile="";

	
	CGlobal *pGlobal = CGlobal::GetInstance();
	pGlobal->m_FileDB.GetConfig(_T("UseDefFontEditFile"),strUseDefFontEditFile);
	if (strUseDefFontEditFile == _T("0"))
		return;

	CWinApp *pApp=AfxGetApp();
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	DWORD dwStyle = GetPrivateProfileIntW( L"EditPro" , L"ModtoolbarStyle",
		CBRS_ALIGN_LEFT	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI,
		((CWinApp*)AfxGetApp())->m_pszProfileName);
	//DWORD dwStyle=pApp->GetProfileInt(L"EditPro", L"ModtoolbarStyle", CBRS_ALIGN_LEFT
	//	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI);
	//end 20120927
	if (!m_wndModifyToolBar.CreateEx(this,TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | dwStyle )
		||!m_wndModifyToolBar.LoadToolBar(IDR_MODIFYTOOLBAR))
    {
		TRACE0("Create modify toolbar failed!\n");
		return;
	}
	
	/*
	因為FantArt有Bug,不能正確識別這幾個字符,因此在FantArt修復
	問題之前,先去掉ID_WUP,ID_WDOWN,ID_WLEFT,ID_WRIGHT,ID_DOUP,ID_WRONG,
	並且將ID_UP,ID_DOWN,ID_RIGHT,ID_LEFT,ID_ZEROC,ID_ZEROF,ID_DEL的值改成Big5碼
	在FantArt修復問題之後,還原此次修改
	*/
//	m_wndModifyToolBar.GetToolBarCtrl().HideButton(ID_WUP,TRUE);
//	m_wndModifyToolBar.GetToolBarCtrl().HideButton(ID_WDOWN,TRUE);
//	m_wndModifyToolBar.GetToolBarCtrl().HideButton(ID_WLEFT,TRUE);
//	m_wndModifyToolBar.GetToolBarCtrl().HideButton(ID_WRIGHT,TRUE);
	//m_wndModifyToolBar.GetToolBarCtrl().HideButton(ID_DOUP,TRUE);
//	m_wndModifyToolBar.GetToolBarCtrl().HideButton(ID_WRONG,TRUE);

	m_wndModifyToolBar.SetBarStyle(m_wndModifyToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	
	m_wndModifyToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndModifyToolBar);
}

void editFrameNew::OnCaptionPicture()
{
	m_bCaptionPicShowed = !m_bCaptionPicShowed;
	if(m_bCaptionPicShowed)
	{
		if(m_pDlgDisplyPicture->GetSafeHwnd() == NULL)
		{
			CResManage myRes(g_hInst);
			m_pDlgDisplyPicture->Create(IDD_DLGPICTURE, this);
			RECT rcFrame;
			this->GetActiveView()->GetWindowRect(&rcFrame);
			m_pDlgDisplyPicture->SetFrameRect(rcFrame);
			m_pDlgDisplyPicture->ShowCaptionPicture();
		}
		this->SetFocus();
		m_pDlgDisplyPicture->ShowWindow(SW_NORMAL);
	}
	else
	{
		if(m_pDlgDisplyPicture->GetSafeHwnd() != NULL)
			m_pDlgDisplyPicture->ShowWindow(SW_HIDE);
	}
}

void editFrameNew::OnUpdateCaptionPicture(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bCaptionPicShowed);
}

void editFrameNew::OnSelchangeCombo() 
{
	CString strSelectText = L"";
	m_comboBox.GetLBText(m_comboBox.GetCurSel(), strSelectText);
	#ifdef EDIT_RICHEDIT
 		((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->UpdateFontSetTemp(strSelectText);
	 #else ifdef EDIT_SCROLL
		((editView_Top*)m_wndSplitter.GetPane(0,0))->UpdateFontSetTemp(strSelectText);
	 #endif
	
	((editView_Bottom*)m_wndSplitter.GetPane(1,0))->UpdateFontSetTemp(strSelectText);
}

void editFrameNew::OnFocusCombo() 
{	
	((editView_Bottom*)m_wndSplitter.GetPane(1,0))->m_bOpenFindDialog = TRUE;
}

void editFrameNew::OnKillFocusCombo() 
{	
	((editView_Bottom*)m_wndSplitter.GetPane(1,0))->m_bOpenFindDialog = FALSE;
}

BOOL editFrameNew::GetUpperViewMode()
{
	BOOL bMode = FALSE;
	#ifdef EDIT_RICHEDIT
		CRect rc;
		m_wndSplitter.GetWindowRect(rc);
		if(rc.bottom <=0)
			return bMode;
		editView_Top_RichEdit* pView = (editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0);
		if(pView!=NULL)
			bMode = ((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->bIsCommentMode;
	 #else ifdef EDIT_SCROLL
		//bMode = FALSE;
		CRect rc;
		m_wndSplitter.GetWindowRect(rc);
		if(rc.bottom <=0)
			return bMode;
		editView_Top* pView = (editView_Top*)m_wndSplitter.GetPane(0,0);
		if(pView!=NULL)
			bMode = ((editView_Top*)m_wndSplitter.GetPane(0,0))->bIsCommentMode;
	 #endif

	return bMode;
}

BOOL editFrameNew::SetUpperViewMode(BOOL bMode)
{
	#ifdef EDIT_RICHEDIT
		 ((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->bIsCommentMode = bMode;
		 ((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->bStartFlag = TRUE;
		 ((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->bViewChanged = TRUE;
	 #else ifdef EDIT_SCROLL
		((editView_Top*)m_wndSplitter.GetPane(0,0))->bIsCommentMode = bMode;
		((editView_Top*)m_wndSplitter.GetPane(0,0))->bViewChanged = TRUE;
	 #endif

	return bMode;
}

void editFrameNew::SwitchingModeOnTop()
{
	#ifdef EDIT_RICHEDIT
	((editView_Bottom*)m_wndSplitter.GetPane(1,0))->OnChangeUpperView();
	 #else ifdef EDIT_SCROLL
		//return;
		((editView_Bottom*)m_wndSplitter.GetPane(1,0))->OnChangeUpperView();
	 #endif
}

void editFrameNew::SetUpperViewEditDoc(editDoc* m_pDoc)
{
	#ifdef EDIT_RICHEDIT
	((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->m_pDoc = m_pDoc;
	#else ifdef EDIT_SCROLL
	((editView_Top*)m_wndSplitter.GetPane(0,0))->m_pDoc = m_pDoc;
	 #endif
}

void editFrameNew::UpdateUpperCommentModeView()
{
	#ifdef EDIT_RICHEDIT
	((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->OnShowcomment();
	#else ifdef EDIT_SCROLL
	((editView_Top*)m_wndSplitter.GetPane(0,0))->InvalidateRect(NULL);
	 #endif
}

void editFrameNew::UpdateSwitchModeFlag(BOOL bFlag)
{
	#ifdef EDIT_RICHEDIT
	((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->bHasComment = bFlag;
	//((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->bHasComment = FALSE;
	#else ifdef EDIT_SCROLL
		return;
	 #endif
}

BOOL editFrameNew::GetUpperProcessingFlag()
{
	BOOL bFlag = FALSE;
	#ifdef EDIT_RICHEDIT
	CWnd *test = m_wndSplitter.GetParentFrame();
	if(test != 0)
		bFlag = ((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->bThreadInProcess;
	 #else ifdef EDIT_SCROLL
		bFlag = FALSE;
	 #endif

	return bFlag;
}

void editFrameNew::KillUpperToopTips()
{
	#ifdef EDIT_RICHEDIT
	CWnd *test = m_wndSplitter.GetParentFrame();
	if(test != 0)
		((editView_Top_RichEdit*)m_wndSplitter.GetPane(0,0))->KillToopTips();
	 #else ifdef EDIT_SCROLL
	CWnd *test = m_wndSplitter.GetParentFrame();
	if(test != 0)
		((editView_Top*)m_wndSplitter.GetPane(0,0))->KillToopTips();
	 #endif
}

void editFrameNew::KillEditToopTips()
{
	#ifdef EDIT_RICHEDIT
	((editView_Bottom*)m_wndSplitter.GetPane(1,0))->KILLEditToopTips();
	 #else ifdef EDIT_SCROLL
		((editView_Bottom*)m_wndSplitter.GetPane(1,0))->KILLEditToopTips();
	 #endif
}

BOOL editFrameNew::PreTranslateMessage(MSG* pMsg) 
{
	if(g_hAccel!=NULL)
		
		if(::TranslateAccelerator(m_hWnd,g_hAccel,pMsg))
			
			return TRUE;
		
	return CMDIChildWnd::PreTranslateMessage(pMsg);		
}

BOOL editFrameNew::GetEditStatus()
{
	return ((editView_Bottom*)m_wndSplitter.GetPane(1,0))->GetEditStatus();
}