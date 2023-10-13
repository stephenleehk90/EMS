// DoFileFrm.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DoFileFrm.h"
#include "dofiletreeview.h"
#include "fileinfodisplayview.h"
#include "dofileview.h"
#include "dofiledoc.h"
#include "transdlg.h"
#include "..\fileoper\fileopermgr.h"

#include "ChoseDicDlg.h"
#include "editview_bottom.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern HACCEL g_hAccel;
/////////////////////////////////////////////////////////////////////////////
// CDoFileFrm

IMPLEMENT_DYNCREATE(CDoFileFrm, CChildMDIFrm)

CDoFileFrm::CDoFileFrm()
{
//	m_strOldFileCode.Empty();
}

CDoFileFrm::~CDoFileFrm()
{
}


BEGIN_MESSAGE_MAP(CDoFileFrm, CChildMDIFrm)
	//{{AFX_MSG_MAP(CDoFileFrm)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_FILE_COMBO, OnSelchangeFilecombo)
	ON_CBN_SELCHANGE(IDC_TIME_COMBO, OnSelchangeTimecombo)
	ON_WM_DESTROY()
	ON_COMMAND(ID_GETFILE, OnGetfile)
	ON_UPDATE_COMMAND_UI(ID_GETFILE, OnUpdateGetfile)
	ON_COMMAND(IDT_TRANSFERFILE, OnTransferfile)
	ON_UPDATE_COMMAND_UI(IDT_TRANSFERFILE, OnUpdateTransferfile)
	ON_COMMAND(IDT_SIGNCOLOUMN, OnSigncoloumn)
	ON_UPDATE_COMMAND_UI(IDT_SIGNCOLOUMN, OnUpdateSigncoloumn)
	ON_UPDATE_COMMAND_UI(IDT_SENDTRANSLATE, OnUpdateSendtranslate)
	ON_COMMAND(IDT_SENDTRANSLATE, OnSendtranslate)
	ON_UPDATE_COMMAND_UI(IDT_DELETE, OnUpdateDelete)
	ON_COMMAND(ID_REFRESHFILE, OnRefreshfile)
	ON_UPDATE_COMMAND_UI(ID_REFRESHFILE, OnUpdateRefreshfile)
	ON_COMMAND(IDT_ATTACH, OnAttach)
	ON_UPDATE_COMMAND_UI(IDT_ATTACH, OnUpdateAttach)
	ON_COMMAND(ID_SIGNDIRECT, OnSigndirect)
	ON_UPDATE_COMMAND_UI(ID_SIGNDIRECT, OnUpdateSigndirect)
	ON_COMMAND(IDT_DELETE, OnDelete)
	ON_COMMAND(ID_LINKTODM, OnLinktodm)
	ON_UPDATE_COMMAND_UI(ID_LINKTODM, OnUpdateLinktodm)
	ON_COMMAND(ID_UNLINKTODM, OnUnlinktodm)
	ON_UPDATE_COMMAND_UI(ID_UNLINKTODM, OnUpdateUnlinktodm)
	ON_COMMAND(ID_BTNPRINT, OnBtnprint)
	ON_UPDATE_COMMAND_UI(ID_BTNPRINT, OnUpdateBtnprint)
	ON_WM_MDIACTIVATE()
	ON_COMMAND(ID_SPELLCHECK, OnSpellcheck)
	ON_UPDATE_COMMAND_UI(ID_SPELLCHECK, OnUpdateSpellcheck)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FILESELCHANGED,OnFileSelChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoFileFrm message handlers
BOOL CDoFileFrm::PreTranslateMessage(MSG* pMsg) 
{
	if(g_hAccel!=NULL)
		
		if(::TranslateAccelerator(m_hWnd,g_hAccel,pMsg))
			
			return TRUE;
		
		return CChildMDIFrm::PreTranslateMessage(pMsg);		
}
int CDoFileFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CResManage res(g_hInst);	
	// TODO: Add your specialized creation code here
	CreateToolBarOnMainFrm(IDR_DOFILETOOLBAR);
	#define TIME_WIDTH		110 //the width of the combo box
	#define FILEDB_WIDTH	120
	#define DISPLAY_WIDTH	140
	#define TOTALDLG_WIDTH	230
	
	// 屏蔽送譯功能
	m_wndToolBar.GetToolBarCtrl().HideButton(IDT_SENDTRANSLATE);
#ifdef _STANDARD_VERSION_
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_SPELLCHECK, TRUE);
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_LINKTODM, TRUE);
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_UNLINKTODM, TRUE);
#endif
    //set up the ComboBox control as a snap mode select box
    //First get the index of the placeholder's position in the toolbar
	//create m_wndTime
    int index = 0;
	CRect rect;
    while(m_wndToolBar.GetItemID(index)!=IDC_TIME_COMBO) index++;
    //next convert that button to a seperator and get its position
    m_wndToolBar.SetButtonInfo(index, IDC_TIME_COMBO, 
    TBBS_SEPARATOR, TIME_WIDTH);
    m_wndToolBar.GetItemRect(index, &rect);
	// then .Create the combo box and show it
	m_staticText1.Create(L"",SS_LEFT,rect,&m_wndToolBar);
	m_staticText1.MoveWindow(rect);
	m_staticText1.ShowWindow(SW_SHOW);
    
	rect.top+=3;
    rect.bottom += 200;
    
	if (!m_wndTime.Create(
        WS_CHILD|WS_VISIBLE | CBS_AUTOHSCROLL |TBSTYLE_FLAT|
        CBS_DROPDOWNLIST | CBS_HASSTRINGS,
        rect, &m_wndToolBar, IDC_TIME_COMBO))
    {
        TRACE0("Failed to create combo-box\n");
        return FALSE;
    }
	CString str;
	str.LoadString(IDS_CURRENTFILE);
	m_wndTime.AddString(str);
	str.LoadString(IDS_THREEDAYS);
	m_wndTime.AddString(str);
	str.LoadString(IDS_WEEKFILE);
	m_wndTime.AddString(str);
	str.LoadString(IDS_WHOLEFILE);
    m_wndTime.AddString(str);
    m_wndTime.ShowWindow(SW_SHOW);

	CWinApp *pApp ;
	int nDate;
	pApp = (CWinApp*)AfxGetApp();
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	nDate = GetPrivateProfileIntW( L"DoFile" , L"Date",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//nDate = pApp->GetProfileInt ( _T("DoFile") , _T("Date"),  0);
	//end 20120927
	
	m_wndTime.SetCurSel (nDate);

    //create m_wndFile
	index=0;
	while(m_wndToolBar.GetItemID(index)!=IDC_FILE_COMBO) index++;
    //next convert that button to a seperator and get its position
    m_wndToolBar.SetButtonInfo(index, IDC_FILE_COMBO, 
    TBBS_SEPARATOR, FILEDB_WIDTH);
    m_wndToolBar.GetItemRect(index, &rect);

	m_staticText2.Create(L"",SS_LEFT,rect,&m_wndToolBar);
	m_staticText2.MoveWindow(rect);
	m_staticText2.ShowWindow(SW_SHOW);

    rect.top+=3;
    rect.bottom += 200;
    // then .Create the combo box and show it
    if (!m_wndFile.Create(
        WS_CHILD|WS_VISIBLE | CBS_AUTOHSCROLL | TBSTYLE_FLAT|
        CBS_DROPDOWNLIST | CBS_HASSTRINGS|WS_VSCROLL,
        rect, &m_wndToolBar, IDC_FILE_COMBO))
    {
        TRACE0("Failed to create combo-box\n");
        return FALSE;
    }
    m_wndFile.ShowWindow(SW_SHOW);

	// add by Bob
	// Now show the totoal count and total words
	index=0;
	while(m_wndToolBar.GetItemID(index)!=ID_TOTALDLG) index++;
    //next convert that button to a seperator and get its position
    m_wndToolBar.SetButtonInfo(index, ID_TOTALDLG, 
		TBBS_SEPARATOR, TOTALDLG_WIDTH);
    m_wndToolBar.GetItemRect(index, &rect);
	//rect.top+=1;
    //rect.bottom -= 1;
	m_dlgTotal.Create(IDD_TOTAL,&m_wndToolBar);
	m_dlgTotal.MoveWindow(rect);
	m_dlgTotal.ShowWindow(SW_SHOW);

	HICON hIcon=pApp->LoadIcon(IDI_DOFILE);
	SetIcon(hIcon,TRUE);
	CString strTitle;
	strTitle.LoadString(IDS_DO_NOTPAPER);
	SetWindowText(strTitle);
	return 0;
}

BOOL CDoFileFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic(this, 1,2))
	{
		TRACE0("Failed to create split bar ");
		return FALSE;
	}

	/*if (!m_wndSplitter1.CreateStatic(
		&m_wndSplitter,
		1,1,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		m_wndSplitter.IdFromRowCol(0, 0)
		))
	{
		TRACE0("Failed to create nested splitter\n");
		return FALSE;
	}*/
	if (!m_wndSplitter2.CreateStatic(
		&m_wndSplitter,
		2,1,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		m_wndSplitter.IdFromRowCol(0,1)
		))
	{
		TRACE0("Failed to create nested splitter\n");
		return FALSE;
	}
	m_wndSplitter.ReposChildSplitter();
	int nPartTree_x,nPartTree_y,nInfoCol_y;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	nPartTree_x = GetPrivateProfileIntW( L"DoFileFrm" , L"nPartTree_x",150,((CWinApp*)AfxGetApp())->m_pszProfileName);
	nPartTree_y = GetPrivateProfileIntW( L"DoFileFrm" , L"nPartTree_y",280,((CWinApp*)AfxGetApp())->m_pszProfileName);
	nInfoCol_y = GetPrivateProfileIntW( L"DoFileFrm" , L"nInfoCol_y",130,((CWinApp*)AfxGetApp())->m_pszProfileName);
	/*nPartTree_x = AfxGetApp()->GetProfileInt(_T("DoFileFrm"),_T("nPartTree_x"),150);
	nPartTree_y= AfxGetApp()->GetProfileInt(_T("DoFileFrm"),_T("nPartTree_y"),280);
	nInfoCol_y= AfxGetApp()->GetProfileInt(_T("DoFileFrm"),_T("nInfoCol_y"),130);*/
	//end 20120927

	if (!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CDoFileTreeView),
							      CSize(nPartTree_x,nPartTree_y),pContext))
		return FALSE;
	if (!m_wndSplitter2.CreateView(0,0,RUNTIME_CLASS(CDoFileView),
							      CSize(nPartTree_x,nPartTree_y-nInfoCol_y),pContext))
		return FALSE;
	if (!m_wndSplitter2.CreateView(1,0,RUNTIME_CLASS(CFileInfoDisplayView),
		CSize(nPartTree_x,nInfoCol_y),pContext))
		return FALSE;

	//m_wndSplitter.ReposChildSplitter();
	//m_wndSplitter2.ReposChildSplitter();
	m_wndSplitter2.SetRowInfo( 1, nInfoCol_y,10);
	m_wndSplitter.RecalcLayout( );
	m_wndSplitter2.RecalcLayout( );
	
/*	CResManage rs(g_hInst);
	CString strTitle;
	strTitle.LoadString(IDS_DO_NOTPAPER);
	SetWindowText(strTitle);*/
	return TRUE;
}

void CDoFileFrm::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	ShowWindow(SW_MAXIMIZE);
	CChildMDIFrm::ActivateFrame(nCmdShow);
}
void CDoFileFrm::OnSelchangeFilecombo()
{
	CDoFileDoc* pDoc=(CDoFileDoc *)(this->GetActiveView()->GetDocument());	
	int nSel=m_wndFile.GetCurSel();
	if(nSel!=-1)
		m_wndFile.GetLBText(nSel,pDoc->m_Mgr.m_strPlaceName);
	else
		pDoc->m_Mgr.m_strPlaceName="";
	
	// pDoc->m_Mgr.LoadLibFile();
	pDoc->m_Mgr.m_strCurFileCode = L"";
	pDoc->m_Mgr.m_iCurFilePos = -1;
	RefreshFile();
}
void CDoFileFrm::OnSelchangeTimecombo()
{
	CDoFileDoc* pDoc=(CDoFileDoc *)(this->GetActiveView()->GetDocument());	
	
	int nSel=m_wndTime.GetCurSel();
	if(nSel!=-1)
	{
		pDoc->m_Mgr.m_enDateType = EN_DATETYPE(nSel);
		//pDoc->m_Mgr.LoadLibFile();
		RefreshFile();
	}

}

void CDoFileFrm::OnDestroy() 
{
	CChildMDIFrm::OnDestroy();
	
	// TODO: Add your message handler code here

}

void CDoFileFrm::OnGetfile() 
{
	// TODO: Add your command handler code here
	CDoFileDoc* pDoc=(CDoFileDoc *)(this->GetActiveView()->GetDocument());	
	pDoc->m_Mgr.GetFile();
	RefreshFile();
}

void CDoFileFrm::OnUpdateGetfile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL tes = GetDoc()->m_Mgr.m_bGetFile;
	pCmdUI->Enable(GetDoc()->m_Mgr.m_bGetFile&&GetDoc()->m_Mgr.m_nItem!=-1);
}

void CDoFileFrm::OnTransferfile() 
{
	// TODO: Add your command handler code here
	CDoFileDoc* pDoc=(CDoFileDoc *)(this->GetActiveView()->GetDocument());	
	CTransDlg dlg;
	dlg.m_bExclusive = pDoc->m_Mgr.m_bExclusive;
	dlg.m_nLibID = pDoc->m_Mgr.m_nDepartID;
	dlg.m_strFileCode = pDoc->m_Mgr.m_strCurFileCode;
	dlg.m_strPlace = pDoc->m_Mgr.m_strDepartName+pDoc->m_Mgr.m_strPlaceName;
	dlg.m_strTitle = pDoc->m_Mgr.m_strCurFileTitle;
	dlg.m_strPlaceName = pDoc->m_Mgr.m_strPlaceName;
	if(dlg.DoModal()==IDOK)
	{
		//pDoc->m_Mgr.LoadLibFile();
		RefreshFile();
	}
}

void CDoFileFrm::OnUpdateTransferfile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDoc()->m_Mgr.m_nItem!=-1&&GetDoc()->m_Mgr.m_bSendFile);
}

void CDoFileFrm::OnSigncoloumn() 
{
	// TODO: Add your command handler code here
	CDoFileDoc* pDoc=(CDoFileDoc *)(this->GetActiveView()->GetDocument());	
	if (pDoc->m_Mgr.SendToCol())
	{
		RefreshFile();
	};
	
}

void CDoFileFrm::OnUpdateSigncoloumn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDoc()->m_Mgr.m_bSendCol&&GetDoc()->m_Mgr.m_nItem!=-1);
}

CDoFileDoc* CDoFileFrm::GetDoc()
{
	/*
	CDoFileDoc* pDoc=(CDoFileDoc *)(this->GetActiveView()->GetDocument());	
	if(pDoc->IsKindOf(RUNTIME_CLASS(CDoFileDoc)))
		return pDoc;
	else
		return NULL;
	*/
	CDoFileDoc* pDoc = (CDoFileDoc *)GetActiveDocument();
	return pDoc;

}

void CDoFileFrm::OnUpdateSendtranslate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(GetDoc()->m_Mgr.m_bReadTrans&&GetDoc()->m_Mgr.m_nItem!=-1);

	pCmdUI->Enable((GetDoc()->m_Mgr.m_bReadTrans || GetDoc()->m_Mgr.m_bFillTrans)	&&
		GetDoc()->m_Mgr.m_nItem!=-1);
}

void CDoFileFrm::OnSendtranslate() 
{
	// TODO: Add your command handler code here
	GetDoc()->m_Mgr.Translate();
}

void CDoFileFrm::OnUpdateDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	// pCmdUI->Enable(GetDoc()->m_Mgr.m_bDelete&&GetDoc()->m_Mgr.m_nItem!=-1&&GetDoc()->m_Mgr.m_bOwnFile);	
	pCmdUI->Enable(GetDoc()->m_Mgr.m_bDelete&&GetDoc()->m_Mgr.m_nItem!=-1);	
}

void CDoFileFrm::OnRefreshfile() 
{
	// TODO: Add your command handler code here
	int nCurSel = GetDoc()->m_Mgr.m_nItem;
	CString strFileCode = GetDoc()->m_Mgr.m_strCurFileCode;
	RefreshFile();
	if (nCurSel>-1)
	{
		GetDoc()->m_Mgr.m_nItem = nCurSel;
		GetDoc()->m_Mgr.m_strCurFileCode = strFileCode;
		m_wndSplitter2.GetPane(1,0)->SendMessage(WM_FILESELCHANGED,(WPARAM)&GetDoc()->m_Mgr.m_strCurFileCode,0);	
	}
}

void CDoFileFrm::OnUpdateRefreshfile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CDoFileFrm::RefreshFile()
{
	GetDoc()->m_Mgr.m_nItem = -1;
	//GetDoc()->m_Mgr.m_strCurFileCode = _T("");
	
	GetDoc()->m_Mgr.Refresh();
	
	//m_wndSplitter2.GetPane(1,0)->SendMessage(WM_FILESELCHANGED,(WPARAM)&GetDoc()->m_Mgr.m_strCurFileCode,0);
}


void CDoFileFrm::OnAttach() 
{
	// TODO: Add your command handler code here
	BOOL bManage;
	bManage = GetDoc()->m_Mgr.CanManageAttach();
	GetDoc()->m_Mgr.ManageAttach(bManage);
	GetDoc()->m_Mgr.LoadLibFile(TRUE);
}

void CDoFileFrm::OnUpdateAttach(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDoc()->m_Mgr.m_bEdit&&GetDoc()->m_Mgr.m_nItem!=-1);
}

void CDoFileFrm::OnSigndirect() 
{
	// TODO: Add your command handler code here
	if (GetDoc()->m_Mgr.SignDirect())
	{
		RefreshFile();
	};
}

void CDoFileFrm::OnUpdateSigndirect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDoc()->m_Mgr.m_bSignDirect && GetDoc()->m_Mgr.m_nItem!=-1);
	
}
LRESULT CDoFileFrm::OnFileSelChanged(WPARAM wParam,LPARAM lParam)
{
//	if(m_strOldFileCode.Compare(*(CString*)wParam)!=0)
//	{
		m_wndSplitter2.GetPane(1,0)->SendMessage(WM_FILESELCHANGED,wParam,lParam);
//		m_strOldFileCode=*(CString*)wParam;
//	}
		return 0;
}
void CDoFileFrm::OnDelete() 
{
	// TODO: Add your command handler code here
	if (AfxMessageBox(IDS_DELETE,MB_YESNO) != IDYES)
		return;
	if (GetDoc()->m_Mgr.DeleteFile())
		RefreshFile();

}

void CDoFileFrm::OnLinktodm() 
{
	// TODO: Add your command handler code here
	GetDoc()->m_Mgr.LinkToDM(GetDoc()->m_Mgr.m_strCurFileCode);
	//RefreshFile();
	OnRefreshfile();
}

void CDoFileFrm::OnUpdateLinktodm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(
		(GetDoc()->m_Mgr.m_nItem!=-1) &&
		(!GetDoc()->m_Mgr.m_bLinked) &&
		(GetDoc()->m_Mgr.m_bCanlink)
		);
	
}

void CDoFileFrm::OnUnlinktodm() 
{
	// TODO: Add your command handler code here
	GetDoc()->m_Mgr.UnlinkDM(GetDoc()->m_Mgr.m_strCurFileCode);
	//RefreshFile();
	OnRefreshfile();
}

void CDoFileFrm::OnUpdateUnlinktodm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(
		(GetDoc()->m_Mgr.m_nItem!=-1) &&
		(GetDoc()->m_Mgr.m_bLinked) &&
		(GetDoc()->m_Mgr.m_bCanUnlink)
		);
}

void CDoFileFrm::OnBtnprint() 
{
	// TODO: Add your command handler code here
	CString strGroup;
	int nSel = m_wndTime.GetCurSel();
	m_wndTime.GetLBText(nSel, strGroup);
	GetDoc()->m_Mgr.PrintView(strGroup);
}

void CDoFileFrm::OnUpdateBtnprint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = FALSE;
	//CDoFileDoc *pDoc = (CDoFileDoc *)GetActiveDocument();
	CDoFileView *pView = (CDoFileView *)m_wndSplitter2.GetPane(0,0);
	if (pView) 
		bEnable = pView->m_FileListCtr.GetItemCount() > 0;

	pCmdUI->Enable(bEnable);
	
}

void CDoFileFrm::SetToolBarBitmap()
{
	CResManage myRes(g_hInst);
	
	CBitmap bitmap;
	//Time
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//Lib
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//修改
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//取稿
	bitmap.LoadBitmap(IDB_BITMAP_GETFILE);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//傳遞
	bitmap.LoadBitmap(IDB_BITMAP_TRANSFILE);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//附件
	bitmap.LoadBitmap(IDB_BITMAP_ATTACHMENT);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//字詞檢查
	bitmap.LoadBitmap(IDB_BITMAP_WORDBANK);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//送譯
	bitmap.LoadBitmap(IDB_BITMAP_TRANSLATE);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//上欄
	bitmap.LoadBitmap(IDB_BITMAP_SENDTOCOL);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//直接簽發
	bitmap.LoadBitmap(IDB_BITMAP_SIGN);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//部門分組菜單的關聯
	bitmap.LoadBitmap(IDB_BITMAP_FILELINKMENU);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//解除與部門分組菜單的關聯
	bitmap.LoadBitmap(IDB_BITMAP_UNLINKFILEMENU);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//刪除
	bitmap.LoadBitmap(IDB_BITMAP_DELFILE);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//列印
	bitmap.LoadBitmap(IDB_BITMAP_PRINT);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//更新
	bitmap.LoadBitmap(IDB_BITMAP_REFRESH);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//更新
	bitmap.LoadBitmap(IDB_BITMAP_REFRESH);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	// Now, set the disabled toolbar

	//Time
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//Lib
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//修改
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//取稿
	bitmap.LoadBitmap(IDB_BITMAP_GETFILE_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//傳遞
	bitmap.LoadBitmap(IDB_BITMAP_TRANSFILE_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//附件
	bitmap.LoadBitmap(IDB_BITMAP_ATTACHMENT_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//字詞檢查
	bitmap.LoadBitmap(IDB_BITMAP_WORDBANK_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//送譯
	bitmap.LoadBitmap(IDB_BITMAP_TRANSLATE_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//上欄
	bitmap.LoadBitmap(IDB_BITMAP_SENDTOCOL_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//直接簽發
	bitmap.LoadBitmap(IDB_BITMAP_SIGN_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//部門分組菜單的關聯
	bitmap.LoadBitmap(IDB_BITMAP_FILELINKMENU_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//解除與部門分組菜單的關聯
	bitmap.LoadBitmap(IDB_BITMAP_UNLINKFILEMENU_DISABLEIDB_BITMAP_UNLINKFILEMENU);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//刪除
	bitmap.LoadBitmap(IDB_BITMAP_DELFILE_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//列印
	bitmap.LoadBitmap(IDB_BITMAP_PRINT_DISABLE);	
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();

	
	//更新
	//bitmap.LoadBitmap(IDB_BITMAP_REFRESH_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTMASKCOLOR);
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

void CDoFileFrm::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	static int nCount = 0;
	CChildMDIFrm::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add your message handler code here

	if (bActivate)
	{
		if (nCount > 0)
			//RefreshFile();
			OnRefreshfile();

		++nCount;
	}
}



void CDoFileFrm::OnSpellcheck() 
{
	// TODO: Add your command handler code here
	GetDoc()->m_Mgr.OpenFile(FALSE,TRUE);
	CString strText; 
	//edit by leon for word bank function
	CChoseDicDlg dlg;
	dlg.DoModal();
	//SetModifiedFlag();
}

void CDoFileFrm::OnUpdateSpellcheck(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDoc()->m_Mgr.m_bEdit&&GetDoc()->m_Mgr.m_nItem!=-1);
}

void CDoFileFrm::OnClose() 
{
	//// TODO: Add your message handler code here and/or call default
	//CGlobal *pGlobal=CGlobal::GetInstance();
	//pGlobal->bDoFileOpened = FALSE;
	//
	//CWinApp *pApp=AfxGetApp();
	//int nPartTree_x,nPartTree_y,nPartTree_cx,nPartTree_cy;
	//m_wndSplitter.GetColumnInfo(0,nPartTree_x,nPartTree_cx);
	//m_wndSplitter.GetRowInfo(0,nPartTree_y,nPartTree_cy);
	////edit by Ronald 20120927 for migrating from vc6 to 2010
	//CString str_temp;
	//str_temp.Format(L"%d",nPartTree_x);
	//WritePrivateProfileStringW(L"DoFileFrm", L"nPartTree_x", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//str_temp.Format(L"%d",nPartTree_y);
	//WritePrivateProfileStringW(L"DoFileFrm", L"nPartTree_y", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	////pApp->WriteProfileInt (_T("DoFileFrm"),_T("nPartTree_x"),nPartTree_x);
	////pApp->WriteProfileInt (_T("DoFileFrm"),_T("nPartTree_y"),nPartTree_y);
	////end 20120927
	//
	//m_wndSplitter2.GetRowInfo(1,nPartTree_y,nPartTree_cy);
	////edit by Ronald 20120927 for migrating from vc6 to 2010
	//str_temp.Format(L"%d",nPartTree_y);
	//WritePrivateProfileStringW(L"DoFileFrm", L"nInfoCol_y", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	////pApp->WriteProfileInt (_T("DoFileFrm"),_T("nInfoCol_y"),nPartTree_y);	
	////end 20120927
	CChildMDIFrm::OnClose();
}

BOOL CDoFileFrm::DestroyWindow()
{
		// TODO: Add your message handler code here and/or call default
	CGlobal *pGlobal=CGlobal::GetInstance();
	pGlobal->bDoFileOpened = FALSE;
	
	CWinApp *pApp=AfxGetApp();
	int nPartTree_x,nPartTree_y,nPartTree_cx,nPartTree_cy;
	m_wndSplitter.GetColumnInfo(0,nPartTree_x,nPartTree_cx);
	m_wndSplitter.GetRowInfo(0,nPartTree_y,nPartTree_cy);
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	CString str_temp;
	str_temp.Format(L"%d",nPartTree_x);
	WritePrivateProfileStringW(L"DoFileFrm", L"nPartTree_x", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",nPartTree_y);
	WritePrivateProfileStringW(L"DoFileFrm", L"nPartTree_y", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt (_T("DoFileFrm"),_T("nPartTree_x"),nPartTree_x);
	//pApp->WriteProfileInt (_T("DoFileFrm"),_T("nPartTree_y"),nPartTree_y);
	//end 20120927
	
	m_wndSplitter2.GetRowInfo(1,nPartTree_y,nPartTree_cy);
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	str_temp.Format(L"%d",nPartTree_y);
	WritePrivateProfileStringW(L"DoFileFrm", L"nInfoCol_y", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt (_T("DoFileFrm"),_T("nInfoCol_y"),nPartTree_y);	
	//end 20120927
	return CChildMDIFrm::DestroyWindow();
}

void CDoFileFrm::QueryFileWithID(CString strFileID)
{
	CDoFileDoc* pDoc=(CDoFileDoc *)(this->GetActiveView()->GetDocument());	
	pDoc->m_Mgr.m_enDateType = EN_DATETYPE(m_wndTime.GetCount());
		//pDoc->m_Mgr.LoadLibFile();
	RefreshFile();
}