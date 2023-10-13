// ChildRepFrm.cpp : implementation file
//

#include "stdafx.h"
#include "ChildRepFrm.h"
#include "replacefactory.h"
#include "..\editpro\editpmvw.h"
#include "saveprog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
void DispatchMsg();
/////////////////////////////////////////////////////////////////////////////
// CChildRepFrm
extern HACCEL g_hAccel;
extern BOOL RemoveMenu(CMenu * pMenuDestination, const CMenu * pMenuDel, bool bTopLevel =false);
extern BOOL MergeMenu(CMenu * pMenuDestination, const CMenu * pMenuAdd, bool bTopLevel =FALSE);
IMPLEMENT_DYNCREATE(CChildRepFrm, CEditProRepaceFrame)

CChildRepFrm::CChildRepFrm()
{
	m_bHasDoRep = FALSE;
}

CChildRepFrm::~CChildRepFrm()
{
	m_ToolBarImageList.DeleteImageList();
	m_ToolBarDisableImageList.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CChildRepFrm, CEditProRepaceFrame)
	//{{AFX_MSG_MAP(CChildRepFrm)
	ON_WM_MDIACTIVATE()
	ON_COMMAND(ID_SEARCHANDREPLACE, OnSearchAndReplace)
	ON_COMMAND(ID_SCANNING, OnScanning)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildRepFrm message handlers
void CChildRepFrm::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CEditProRepaceFrame::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add your message handler code here
	
	if (bActivate)
	{
		CEditroomDoc* pDoc = (CEditroomDoc*)GetActiveDocument();

		if(!pDoc->m_hDefaultMenu)
		{
			pDoc->m_hDefaultMenu = 
				::LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_EDITROTYPE));
			if (pDoc->m_hDefaultMenu == NULL)
				return;
		}
		
		CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
		pFrame->RecalcLayout();
		//pFrame->MDISetMenu(CMenu::FromHandle(pDoc->m_hDefaultMenu), NULL);
		CMenu *pMainMenu=pFrame->GetMenu();
		CMenu *pEditMenu=CMenu::FromHandle(pDoc->m_hDefaultMenu);
		MergeMenu(pMainMenu,pEditMenu,TRUE);
		pFrame->DrawMenuBar();
		DoSearchAndReplace();
	}
	else
	{
		CEditroomDoc* pDoc = (CEditroomDoc*)GetActiveDocument();
		
		if(!pDoc->m_hDefaultMenu)
		{
			pDoc->m_hDefaultMenu = 
				::LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_EDITROTYPE));
			if (pDoc->m_hDefaultMenu == NULL)
				return;
		}
		
		CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
		pFrame->RecalcLayout();
		CMenu *pMainMenu=pFrame->GetMenu();
		CMenu *pEditMenu=CMenu::FromHandle(pDoc->m_hDefaultMenu);
		RemoveMenu(pMainMenu,pEditMenu,TRUE);
		pFrame->DrawMenuBar();
	}
}
void DispatchMsg()
{
	MSG  msg;
	//::GetMessage(&msg,NULL,NULL,NULL);
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	::TranslateMessage(&msg);
	::DispatchMessage(&msg); 
}
void CChildRepFrm::OnSearchAndReplace() 
{
	// TODO: Add your command handler code here
	CEditroomDoc* pDoc=(CEditroomDoc*)GetActiveDocument();
	if(!pDoc->IsKindOf(RUNTIME_CLASS(CEditroomDoc)))
	{
		MessageBox(L"Doc error!",L"error");
		return;
	}
	CEditProReplaceView *pReplaceView=NULL;
	CEditProModRepView *pModRepView = NULL;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView =pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CEditProReplaceView)))
			pReplaceView=(CEditProReplaceView*)pView;
		if(pView->IsKindOf(RUNTIME_CLASS(CEditProModRepView)))
			pModRepView=(CEditProModRepView*)pView;
		//pView->UpdateWindow();
	}   
	if(pReplaceView==NULL)
		return;
	DWORD dwBegin = GetTickCount();
	CDoSaveProg *pProg = CDoSaveProg::Instance();
	//---------------------------------------------------------
	//Symbol scanning begin
	CRepFactory *pFactory=CRepFactory::Instance();
	pReplaceView->SetRedraw(FALSE);
	if(pModRepView)
		pModRepView->SetRedraw(FALSE);
	CString str;
	str.LoadString(IDS_SYMBOLSCAN);
	pProg->SetStatus(str);
	pProg->SetProgress(2);
	pProg->ShowDialog(SW_SHOW);
	if(pFactory->m_pmapSymbol)
	{
		POSITION pos = pFactory->m_pmapSymbol->GetStartPosition();
		while (pos != NULL)
		{
			CString str;
			SYMBOL* pMyObject;
			pFactory->m_pmapSymbol->GetNextAssoc(pos, str, pMyObject);
			pReplaceView->FindSymbol(pMyObject,FALSE);
			DispatchMsg();
		}
	}
	
	//Symbol scanning end
	//spellingoverall
	str.LoadString(IDS_SPELLINGOVERALL);
	pProg->SetStatus(str);
	pProg->SetProgress(5);
	if(pFactory->m_pmapSpellingOverAll)
	{
		POSITION pos = pFactory->m_pmapSpellingOverAll->GetStartPosition();
		while (pos != NULL)
		{
			CString strOriginal,strDest;
			pFactory->m_pmapSpellingOverAll->GetNextAssoc(pos, strOriginal, strDest);
			pReplaceView->FindAndReplace(strOriginal,strDest,1,TRUE);
		
		}
	}
	//spellingbycase
	str.LoadString(IDS_SPELLINGBYCASE);
	pProg->SetStatus(str);
	pProg->SetProgress(8);
	if(pFactory->m_pmapSpellingByCase)
	{
		POSITION pos = pFactory->m_pmapSpellingByCase->GetStartPosition();
		while (pos != NULL)
		{
			CString strOriginal,strDest;
			pFactory->m_pmapSpellingByCase->GetNextAssoc(pos, strOriginal, strDest);
			pReplaceView->FindAndReplace(strOriginal,strDest,2,TRUE);
		
		}
	}
	pProg->SetProgress(10);
	pReplaceView->SetRedraw(TRUE);
	if(pModRepView)
	{
		pModRepView->SetRedraw(TRUE);
		pModRepView->Invalidate();
	}
	pReplaceView->SetCaretTop();
	pReplaceView->Invalidate();
}

void CChildRepFrm::OnScanning() 
{
	// TODO: Add your command handler code here
	CEditroomDoc* pDoc=(CEditroomDoc*)GetActiveDocument();
	if(!pDoc->IsKindOf(RUNTIME_CLASS(CEditroomDoc)))
	{
		MessageBox(L"Doc error!",L"error");
		return;
	}
	CEditProReplaceView *pReplaceView=NULL;
	CEditProModRepView *pModRepView = NULL;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView =pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CEditProReplaceView)))
			pReplaceView=(CEditProReplaceView*)pView;
		//pView->UpdateWindow();
	}   
	if(pReplaceView==NULL)
		return;
	pReplaceView->SetRedraw(FALSE);
	//Synonymns
	CRepFactory *pFactory=CRepFactory::Instance();
	if(pFactory->m_pmapSynonymns)
	{
		POSITION pos = pFactory->m_pmapSynonymns->GetStartPosition();
		while (pos != NULL)
		{
			CString strOriginal;
			CStringList *pList;
			pFactory->m_pmapSynonymns->GetNextAssoc(pos, strOriginal, pList);
			pReplaceView->FindSynonyms(strOriginal);
		}
	}	
	pReplaceView->SetRedraw(TRUE);
	pReplaceView->Invalidate();
	pReplaceView->SetCaretTop();
}

void CChildRepFrm::OnDestroy() 
{
	CEditProRepaceFrame::OnDestroy();
	
	// TODO: Add your message handler code here
	CDoSaveProg *pProg = CDoSaveProg::Instance();
	pProg->Destroy();
}

int CChildRepFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditProRepaceFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	/*if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_EDITTOOLBAR1))*/
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI   ) ||
		!m_wndToolBar.LoadToolBar(IDR_EDITTOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	m_ToolBarImageList.Create(24,24,ILC_COLOR24|ILC_MASK,0,1);
	m_ToolBarDisableImageList.Create(24,24,ILC_COLOR24|ILC_MASK,0,1);
	
	SetToolBarBitmap();
	
	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);

#ifdef EMBED_TOOLBAR
	AfxGetMainWnd()->SendMessage(WM_DOCKTOOLBARLEFTOF,(UINT)&m_wndToolBar);
	m_wndToolBar.SetOwner(this);
#else
	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);
#endif

	/*m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);*/
	return 0;
}



void CChildRepFrm::DoSearchAndReplace()
{
	if(!m_bHasDoRep)
	{
		OnSearchAndReplace();
		m_bHasDoRep = TRUE;
	}
}
BOOL CChildRepFrm::PreTranslateMessage(MSG* pMsg)
{
	if(g_hAccel!=NULL)
		
		if(::TranslateAccelerator(m_hWnd,g_hAccel,pMsg))
			
			return TRUE;
		return CEditProRepaceFrame::PreTranslateMessage(pMsg);
}

void CChildRepFrm::SetToolBarBitmap()
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
	
	//字詞替換
	bitmap.LoadBitmap(IDB_BITMAP_WORDCHECK);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//查找同義詞
	bitmap.LoadBitmap(IDB_BITMAP_WORDCHANGE);
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

	// Now set the disabled toolbar

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
	
	//字詞替換
	bitmap.LoadBitmap(IDB_BITMAP_WORDCHECK_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//查找同義詞
	bitmap.LoadBitmap(IDB_BITMAP_WORDCHANGE_DISABLE);
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
