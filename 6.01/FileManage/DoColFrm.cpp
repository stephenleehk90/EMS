// DoColFrm.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DoColFrm.h"
#include "docoltreeview.h"
#include "docolview.h"
#include "docoldoc.h"
#include "fileinfodisplayview.h"
#include "ColReport.h"
#include "CopyList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern HACCEL g_hAccel;
/////////////////////////////////////////////////////////////////////////////
// CDoColFrm

IMPLEMENT_DYNCREATE(CDoColFrm, CChildMDIFrm)

CDoColFrm::CDoColFrm()
{
	m_pDoc=NULL;
	m_fontFace.CreatePointFont(SYSDEFAULTFONTPOINT,SYSDEFAULTFONTNAME,NULL);
//	m_strOldFileCode=_T("");
}

CDoColFrm::~CDoColFrm()
{
	m_fontFace.DeleteObject();
}


BEGIN_MESSAGE_MAP(CDoColFrm, CChildMDIFrm)
	//{{AFX_MSG_MAP(CDoColFrm)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDT_MODIFYCOL, OnModifycol)
	ON_UPDATE_COMMAND_UI(IDT_MODIFYCOL, OnUpdateModifycol)
	ON_COMMAND(IDT_TRANSPAPER, OnTranspaper)
	ON_UPDATE_COMMAND_UI(IDT_TRANSPAPER, OnUpdateTranspaper)
	ON_COMMAND(IDT_ROLLBACK, OnRollback)
	ON_UPDATE_COMMAND_UI(IDT_ROLLBACK, OnUpdateRollback)
	ON_COMMAND(IDT_ATTACH, OnAttach)
	ON_UPDATE_COMMAND_UI(IDT_ATTACH, OnUpdateAttach)
	ON_COMMAND(IDT_SIGNPAPER, OnSignpaper)
	ON_UPDATE_COMMAND_UI(IDT_SIGNPAPER, OnUpdateSignpaper)
	ON_COMMAND(IDT_BACKFILE, OnBackfile)
	ON_UPDATE_COMMAND_UI(IDT_BACKFILE, OnUpdateBackfile)
	ON_COMMAND(ID_REFRESHFILE, OnRefresh)
	ON_COMMAND(ID_PRINT, OnPrint)
	ON_UPDATE_COMMAND_UI(ID_PRINT, OnUpdatePrint)
	ON_COMMAND(ID_RDO_COPY, OnRdoCopy)
	ON_UPDATE_COMMAND_UI(ID_RDO_COPY, OnUpdateRdoCopy)
	ON_WM_MDIACTIVATE()
	ON_CBN_SELCHANGE(IDT_SELECTDATE, OnSelchangeDatecombo)
	ON_CBN_SELCHANGE(IDT_SELECTPAPER, OnSelchangeColumncombo)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FILESELCHANGED,OnFileSelChanged)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDT_SELECTDATE, OnDatetimechangeDtpicker)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoColFrm message handlers
BOOL CDoColFrm::PreTranslateMessage(MSG* pMsg) 
{
	if(g_hAccel!=NULL)
		
		if(::TranslateAccelerator(m_hWnd,g_hAccel,pMsg))
			
			return TRUE;
		
		return CChildMDIFrm::PreTranslateMessage(pMsg);		
}
void CDoColFrm::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	ShowWindow(SW_MAXIMIZE);
	CChildMDIFrm::ActivateFrame(nCmdShow);
}
int CDoColFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CChildMDIFrm::OnCreate(lpCreateStruct) == -1)
		return -1;
	CreateToolBarOnMainFrm(IDR_DOCOLTOOLBAR);		
		
	#define TIME_WIDTH		100 //the width of the combo box
	#define FILEDB_WIDTH	180+80
	#define DISPLAY_WIDTH	140
	#define TOTALDLG_WIDTH	230

	 int index = 0;
	CRect rect;
    while(m_wndToolBar.GetItemID(index)!=IDT_SELECTDATE) index++;
    m_wndToolBar.SetButtonInfo(index, IDT_SELECTDATE, 
		TBBS_SEPARATOR, TIME_WIDTH);
    m_wndToolBar.GetItemRect(index, &rect);
	
	
	m_staticText1.Create(L"",SS_LEFT,rect,&m_wndToolBar);
	m_staticText1.MoveWindow(rect);
	m_staticText1.ShowWindow(SW_SHOW);

	
	CString str;
	CGlobal::GetInstance()->m_FileDB.GetConfig(_T("UseDateCombo"),str);
	m_bDateCombo = _ttoi(str);
	if (m_bDateCombo == TRUE)
	{
		rect.top += 3;
		rect.bottom += 200;
		// then .Create the combo box and show it
		if (!m_wndTime.Create(
			WS_CHILD|WS_VISIBLE | CBS_AUTOHSCROLL |WS_VSCROLL| CBS_DROPDOWNLIST | CBS_HASSTRINGS,
			rect, &m_wndToolBar, IDT_SELECTDATE))
		{
			TRACE0("Failed to create combo-box\n");
			return FALSE;
		}
	}
	else
	{
		rect.top += 3;
		rect.bottom -= 3;
		if (!m_wndDTPicker.Create(WS_VISIBLE | WS_CHILD | DTS_SHORTDATEFORMAT, rect, &m_wndToolBar, IDT_SELECTDATE))
		{
			TRACE0("Failed to create DateTimeCtrl\n");
			return FALSE;
		}
		m_wndDTPicker.SetFormat(DATETIMEPICKERDATEFORMAT);
	
		m_wndDTPicker.SetFont(&m_fontFace);
		COleDateTime dt;
		dt = COleDateTime::GetCurrentTime() - CGlobal::GetInstance()->m_pEntry->tsSpanOfDay + COleDateTimeSpan(1,0,0,0);
		m_wndDTPicker.SetTime(dt);
	}
	
	CWinApp *pApp ;
	pApp = (CWinApp*)AfxGetApp();
	index=0;
	while(m_wndToolBar.GetItemID(index)!=IDT_SELECTPAPER) index++;
    //next convert that button to a seperator and get its position
    m_wndToolBar.SetButtonInfo(index, IDT_SELECTPAPER, 
		TBBS_SEPARATOR, FILEDB_WIDTH);
    m_wndToolBar.GetItemRect(index, &rect);
	
	m_staticText2.Create(L"",SS_LEFT,rect,&m_wndToolBar);
	m_staticText2.MoveWindow(rect);
	m_staticText2.ShowWindow(SW_SHOW);
	
	rect.top+=3;
    rect.bottom += 200;
    // then .Create the combo box and show it
    if (!m_wndColumn.Create(
        WS_CHILD|WS_VISIBLE | CBS_AUTOHSCROLL | TBSTYLE_FLAT|/*CBS_SORT|   */
        CBS_DROPDOWNLIST | CBS_HASSTRINGS|WS_VSCROLL,
        rect, &m_wndToolBar, IDT_SELECTPAPER))
    {
        TRACE0("Failed to create combo-box\n");
        return FALSE;
    }
    m_wndColumn.ShowWindow(SW_SHOW);
	m_wndToolBar.ShowWindow(SW_SHOW);

	index=0;
	while(m_wndToolBar.GetItemID(index)!=ID_TOTALFILE) index++;
    //next convert that button to a seperator and get its position
    m_wndToolBar.SetButtonInfo(index, ID_TOTALFILE, 
		TBBS_SEPARATOR, TOTALDLG_WIDTH);
    m_wndToolBar.GetItemRect(index, &rect);
	m_dlgTotal.Create(IDD_TOTAL,&m_wndToolBar);
	m_dlgTotal.MoveWindow(rect);
	m_dlgTotal.ShowWindow(SW_SHOW);
	
	CResManage res(g_hInst);
	HICON hIcon=pApp->LoadIcon(IDI_DOCOL);
	SetIcon(hIcon,TRUE);
	CString strTitle;
	strTitle.LoadString(IDS_DO_PAPER);
	SetWindowText(strTitle);
//	GetWindowText(strTitle);
	return 0;
}

void CDoColFrm::OnDestroy() 
{


	CChildMDIFrm::OnDestroy();
	
	// TODO: Add your message handler code here
	CGlobal *pGlobal=CGlobal::GetInstance();
	pGlobal->bDoColOpened = FALSE;
}
BOOL CDoColFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic(this, 1,2))
	{
		TRACE0("Failed to create split bar ");
		return FALSE;
	}
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
	int nPartCol_x,nPartCol_y,nInfoCol_y;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	nPartCol_x = GetPrivateProfileIntW( L"DoColFrm" , L"nPartCol_x",150,((CWinApp*)AfxGetApp())->m_pszProfileName);	
	nPartCol_y = GetPrivateProfileIntW( L"DoColFrm" , L"nPartCol_y",280,((CWinApp*)AfxGetApp())->m_pszProfileName);
	nInfoCol_y = GetPrivateProfileIntW( L"DoColFrm" , L"nInfoCol0_y",130,((CWinApp*)AfxGetApp())->m_pszProfileName);
	/*nPartCol_x = AfxGetApp()->GetProfileInt(_T("DoColFrm"),_T("nPartCol_x"),150);
	nPartCol_y= AfxGetApp()->GetProfileInt(_T("DoColFrm"),_T("nPartCol_y"),280);
	nInfoCol_y= AfxGetApp()->GetProfileInt(_T("DoColFrm"),_T("nInfoCol0_y"),130);*/
	//end 20120927
	
	if (!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CDoColTreeView),
		CSize(nPartCol_x,nPartCol_y),pContext))
		return FALSE;
	if (!m_wndSplitter2.CreateView(0,0,RUNTIME_CLASS(CDoColView),
		CSize(nPartCol_x,nPartCol_y-nInfoCol_y),pContext))
		return FALSE;
	if (!m_wndSplitter2.CreateView(1,0,RUNTIME_CLASS(CFileInfoDisplayView),
		CSize(nPartCol_x,nInfoCol_y),pContext))
		return FALSE;
	//m_wndSplitter.ReposChildSplitter();
	//m_wndSplitter2.ReposChildSplitter();
	m_wndSplitter2.SetRowInfo( 0, nInfoCol_y,10);	
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	nInfoCol_y = GetPrivateProfileIntW( L"DoColFrm" , L"nInfoCol0_y",130,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//nInfoCol_y= AfxGetApp()->GetProfileInt(_T("DoColFrm"),_T("nInfoCol0_y"),130);
	//end20120927
	//m_wndSplitter2.SetRowInfo( 1, nInfoCol_y,10);
	m_wndSplitter.RecalcLayout( );
	m_wndSplitter2.RecalcLayout( );
	return TRUE;		
}

void CDoColFrm::ConditionSelChange()
{
	CDoColDoc* pDoc=GetDoc();
	pDoc->m_Mgr.m_nItem = -1;
	pDoc->m_Mgr.m_strCurFileCode = _T("");
	
	m_wndSplitter2.GetPane(1,0)->SendMessage(WM_FILESELCHANGED, (WPARAM)&(pDoc->m_Mgr.m_strCurFileCode), 0);
	COleDateTime dtDate;
	m_wndDTPicker.GetTime(dtDate);
	if(CGlobal::GetInstance()->str_Share.GetLength()>0)
	{
		dtDate = CGlobal::GetInstance()->dt_Share;
		m_wndDTPicker.SetTime(dtDate);
	}

	pDoc->m_Mgr.m_strDate = dtDate.Format(DATEFORMAT);
	pDoc->m_Mgr.m_strCurFileCode = L"";
	pDoc->m_Mgr.m_iCurFilePos = -1;
	pDoc->m_Mgr.LoadPaperFile(FALSE);
}


void CDoColFrm::OnDatetimechangeDtpicker(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMDATETIMECHANGE* pNMDtChg = (NMDATETIMECHANGE*)pNMHDR;

	ConditionSelChange();
	
	*pResult = 0;
}


void CDoColFrm::OnSelchangeDatecombo()
{
	CDoColDoc* pDoc=GetDoc();	
	
	pDoc->m_Mgr.m_nItem = -1;
	pDoc->m_Mgr.m_strCurFileCode = _T("");

	m_wndSplitter2.GetPane(1,0)->SendMessage(WM_FILESELCHANGED,(WPARAM)&(pDoc->m_Mgr.m_strCurFileCode),0);
	if(CGlobal::GetInstance()->str_Share.GetLength()>0)
	{
		CString strDate;
		strDate.Format(L"%02d-%02d-%04d", CGlobal::GetInstance()->dt_Share.GetMonth(), CGlobal::GetInstance()->dt_Share.GetDay(), CGlobal::GetInstance()->dt_Share.GetYear());
		int index = m_wndTime.FindString(0, strDate);
		m_wndTime.SetCurSel(index);
	}
	int nSel=m_wndTime.GetCurSel();
	if(nSel!=-1)
	{
		m_wndTime.GetLBText(nSel,pDoc->m_Mgr.m_strDate);
	}
		pDoc->m_Mgr.LoadPaperFile(FALSE);
		

}

void CDoColFrm::OnSelchangeColumncombo()
{
	CDoColView *pView = (CDoColView *)m_wndSplitter2.GetPane(0,0);
	if (pView) 
		pView->m_FileListCtr.DeleteAllItems();

	CDoColDoc* pDoc=GetDoc();	
	pDoc->m_Mgr.m_nItem = -1;
	pDoc->m_Mgr.m_strCurFileCode = _T("");
	
	m_wndSplitter2.GetPane(1,0)->SendMessage(WM_FILESELCHANGED,(WPARAM)&(pDoc->m_Mgr.m_strCurFileCode),0);
	
	
	int nSel=m_wndColumn.GetCurSel();
	if(nSel!=-1)
	{
		pDoc->m_Mgr.m_nColumnID=m_wndColumn.GetItemData(nSel);
		CString strSource;
		m_wndColumn.GetLBText(nSel,strSource);
		pDoc->m_Mgr.GetPaperCol(strSource,pDoc->m_Mgr.m_strPaper,pDoc->m_Mgr.m_strCol);
		if (m_bDateCombo == FALSE)
			ConditionSelChange();
		else
		{
			pDoc->m_Mgr.ListColumnDate(&m_wndTime);
			nSel=m_wndTime.SetCurSel(0);
			//if(nSel!=CB_ERR)
			{
				OnSelchangeDatecombo();		
			}
		}
	}
	
}

void CDoColFrm::OnModifycol() 
{
	// TODO: Add your command handler code here
	
	GetDoc()->m_Mgr.OpenFile(FALSE);
}

void CDoColFrm::OnUpdateModifycol(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDoc()->m_Mgr.CanModify());
	//	pCmdUI->Enable(TRUE);
}

void CDoColFrm::OnTranspaper() 
{
	// TODO: Add your command handler code here
	if (GetDoc()->m_Mgr.TransPaper())
	{
		GetDoc()->m_Mgr.m_nItem = -1;
		GetDoc()->m_Mgr.m_strCurFileCode = _T("");
	};
	RefreshFile();
}

void CDoColFrm::OnUpdateTranspaper(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDoc()->m_Mgr.CanTransPaper());
}

void CDoColFrm::OnRollback() 
{
	// TODO: Add your command handler code here
	CString strPrompt;
	if (GetDoc()->m_Mgr.RollbackFile(strPrompt))
	{
		GetDoc()->m_Mgr.m_nItem = -1;
		GetDoc()->m_Mgr.m_strCurFileCode = _T("");
		RefreshFile();
		strPrompt.TrimLeft();
		if(strPrompt!=_T(""))
			AfxMessageBox(strPrompt);
	};
	//GetDoc()->m_Mgr.LoadPaperFile();
}

void CDoColFrm::OnUpdateRollback(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDoc()->m_Mgr.CanRollBack());
}

void CDoColFrm::OnAttach() 
{
	// TODO: Add your command handler code here
	BOOL bManage;
	bManage = GetDoc()->m_Mgr.CanManageAttach();
	GetDoc()->m_Mgr.ManageAttach(bManage);
	GetDoc()->m_Mgr.LoadPaperFile();
}

void CDoColFrm::OnUpdateAttach(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDoc()->m_Mgr.CanManageAttach());
}

void CDoColFrm::OnSignpaper() 
{
	// TODO: Add your command handler code here
	GetDoc()->m_Mgr.SignFile();
	RefreshFile();
}

void CDoColFrm::OnUpdateSignpaper(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDoc()->m_Mgr.CanSign());
}

void CDoColFrm::OnBackfile() 
{
	// TODO: Add your command handler code here
	if(AfxMessageBox(IDS_BACKFILEQUESTION,MB_YESNO)!=IDYES)
		return;
	GetDoc()->m_Mgr.BackFile();
	RefreshFile();
}

void CDoColFrm::OnUpdateBackfile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDoc()->m_Mgr.CanBack());
}

void CDoColFrm::OnRefresh() 
{
	// TODO: Add your command handler code here
//	CString strFilecode = _T("");
//	m_wndSplitter2.GetPane(1,0)->SendMessage(WM_FILESELCHANGED,(WPARAM)&strFilecode,0);
	RefreshFile();
}

CDoColDoc * CDoColFrm::GetDoc()
{
	if(!m_pDoc)
	{
		m_pDoc=(CDoColDoc*)GetActiveDocument();	
		ASSERT(m_pDoc!=NULL);
	}
	return m_pDoc;
}

void CDoColFrm::RefreshFile()
{
	GetDoc()->m_Mgr.Refresh();
//	m_wndSplitter2.GetPane(1,0)->SendMessage(WM_FILESELCHANGED,(WPARAM)&GetDoc()->m_Mgr.m_strCurFileCode,0);

}
LRESULT CDoColFrm::OnFileSelChanged(WPARAM wParam,LPARAM lParam)
{
//	if(m_strOldFileCode.Compare(*(CString*)wParam)!=0)
//	{	
		m_wndSplitter2.GetPane(1,0)->SendMessage(WM_FILESELCHANGED,wParam,lParam);
//		m_strOldFileCode=*(CString*)wParam;
//	}
		return 0;
}
void CDoColFrm::OnPrint() 
{
	// TODO: Add your command handler code here
	//edit by leon 2012-11-19 for crystal report issue
	/*
	if (GetDoc()->m_Mgr.rs->IsOpen())
	{
		if (GetDoc()->m_Mgr.rs->GetRecordCount() >0)
		{	
			CColReport dlg;
			dlg.rs = GetDoc()->m_Mgr.rs;

			COleDateTime odt;
			m_wndDTPicker.GetTime(odt);
			
			dlg.m_strPreTitle = GetDoc()->m_Mgr.m_strCol + odt.Format(_T(" %Y-%m-%d "));
			while (dlg.DoModal() == IDOK);


		}
	}*/
	CGlobal *pGL=CGlobal::GetInstance();
	if (pGL->m_ReportPara.bValid)
	{
		TCHAR path[100];
		CString strFileName,strError;
		GetModuleFileName(AfxGetInstanceHandle(),path,100);
		CString strPath = path;
		int nLength = strPath.ReverseFind(_T('\\'));
		strFileName = strPath.Left(nLength);
		strFileName += COFILERPT;	
		if( (_taccess(strFileName, 0 )) == -1 )
		{
			strError.Format(IDS_FILENOTEXIST, strFileName);		
			AfxMessageBox(strError);			
			return;
		}
		COleDateTime odt;
		if (m_bDateCombo == TRUE)
		{
			odt.ParseDateTime(GetDoc()->m_Mgr.m_strDate);
		}
		else
			m_wndDTPicker.GetTime(odt);
		pGL->m_ReportPara.enType=COFILEREPORT;
		pGL->m_ReportPara.strFilePath=strFileName;
		pGL->m_ReportPara.strAppTittle=COFILENAME;
		pGL->m_ReportPara.strTableTittle= GetDoc()->m_Mgr.m_strCol + odt.Format(_T(" %Y-%m-%d "));

		// Edit Stephen 2013-12-17
		CString strColID = "0";
		strColID.Format(L"%ld",GetDoc()->m_Mgr.m_nColumnID); 
		pGL->m_ReportPara.strDomainID = strColID;
		pGL->m_ReportPara.strStartDate = GetDoc()->m_Mgr.m_strDate;
		pGL->m_ReportPara.strCurUser = pGL->m_pEmployee->m_strUserName;

		// Edit Stephen 2013-12-17


		if(pGL->LoadDLL(MD_CRYSTALREPORT)){
			pGL->DoCommand(MD_CRYSTALREPORT, "DisplayReport");
		}	
	}
	
}

void CDoColFrm::OnUpdatePrint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
	
}

void CDoColFrm::OnRdoCopy() 
{
	// TODO: Add your command handler code here
	//IsCopyFile(m_strCurFileCode)
	if((m_pDoc != NULL) && (m_pDoc->m_Mgr.IsCopyFile(m_pDoc->m_Mgr.m_strCurFileCode)))
	{
		CCopyList dlg;
		dlg.DoModal();
	}
}

void CDoColFrm::OnUpdateRdoCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
	
}

void CDoColFrm::SetToolBarBitmap()
{
	CResManage myRes(g_hInst);
	
	CBitmap bitmap;
	
	//Date
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//Col
	bitmap.LoadBitmap(IDB_BITMAP_GETFILE);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//修改
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//調版
	bitmap.LoadBitmap(IDB_BITMAP_CROSS);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//退稿
	bitmap.LoadBitmap(IDB_BITMAP_SENDBACK);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//附件
	bitmap.LoadBitmap(IDB_BITMAP_ATTACHMENT);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//簽發
	bitmap.LoadBitmap(IDB_BITMAP_SIGN);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//回調
	bitmap.LoadBitmap(IDB_BITMAP_UNSIGN);
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
	// Now , set the disabled toolbar

	//Date
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//Col
	bitmap.LoadBitmap(IDB_BITMAP_GETFILE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//修改
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//調版
	bitmap.LoadBitmap(IDB_BITMAP_CROSS_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//退稿
	bitmap.LoadBitmap(IDB_BITMAP_SENDBACK_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//附件
	bitmap.LoadBitmap(IDB_BITMAP_ATTACHMENT_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//簽發
	bitmap.LoadBitmap(IDB_BITMAP_SIGN_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//回調
	bitmap.LoadBitmap(IDB_BITMAP_UNSIGN_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//列印
	bitmap.LoadBitmap(IDB_BITMAP_PRINT_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//更新
	//bitmap.LoadBitmap(IDB_BITMAP_REFRESH_DISABLE);
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

void CDoColFrm::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CChildMDIFrm::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add your message handler code here
	static int nCount = 0;
	
	// TODO: Add your message handler code here
	
	if (bActivate)
	{
		if (nCount > 0)
			RefreshFile();
		
		++nCount;
	}	
}

void CDoColFrm::OnClose() 
{
	//// TODO: Add your message handler code here and/or call default
	//int nPartCol_x,nPartCol_y,nPartCol_cx,nPartCol_cy;
	//m_wndSplitter.GetColumnInfo(0,nPartCol_x,nPartCol_cx);
	//m_wndSplitter.GetRowInfo(0,nPartCol_y,nPartCol_cy);
	//
	//CWinApp *pApp=AfxGetApp();
	////edit by Ronald 20120927 for migrating from vc6 to 2010
	//CString str_temp;
	//str_temp.Format(L"%d",nPartCol_x);
	//WritePrivateProfileStringW(L"DoColFrm", L"nPartCol_x", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//str_temp.Format(L"%d",nPartCol_y);
	//WritePrivateProfileStringW(L"DoColFrm", L"nPartCol_y", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	////	pApp->WriteProfileInt (_T("DoColFrm"),_T("nPartCol_x"),nPartCol_x);
	//// pApp->WriteProfileInt (_T("DoColFrm"),_T("nPartCol_y"),nPartCol_y);
	////end 20120927
	//
	//m_wndSplitter2.GetRowInfo(0,nPartCol_y,nPartCol_cy);
	////edit by Ronald 20120927 for migrating from vc6 to 2010
	////pApp->WriteProfileInt (_T("DoColFrm"),_T("nInfoCol0_y"),nPartCol_y);
	//str_temp.Format(L"%d",nPartCol_y);
	//WritePrivateProfileStringW(L"DoColFrm", L"nInfoCol0_y", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//m_wndSplitter2.GetRowInfo(1,nPartCol_y,nPartCol_cy);
	////pApp->WriteProfileInt (_T("DoColFrm"),_T("nInfoCol_y"),nPartCol_y);
	//str_temp.Format(L"",nPartCol_y);
	//WritePrivateProfileStringW(L"DoColFrm", L"nInfoCol_y", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	////end 20120927
	//
	//int nSel = m_wndColumn.GetCurSel();
	//if(nSel >= 0)
	//{
	//	//edit by Ronald 20120927 for migrating from vc6 to 2010
	//	CString strTemp; 
	//	strTemp.Format(L"%d",m_wndColumn.GetItemData(nSel));
	//	WritePrivateProfileStringW(L"DoColFrm", L"column_id",strTemp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//	//pApp->WriteProfileInt(_T("DoColFrm"),_T("column_id"), m_wndColumn.GetItemData(nSel));
	//	//end 20120927
	//}
	CChildMDIFrm::OnClose();
}

BOOL CDoColFrm::DestroyWindow()
{
	// TODO: Add your message handler code here and/or call default
	int nPartCol_x,nPartCol_y,nPartCol_cx,nPartCol_cy;
	m_wndSplitter.GetColumnInfo(0,nPartCol_x,nPartCol_cx);
	m_wndSplitter.GetRowInfo(0,nPartCol_y,nPartCol_cy);
	
	CWinApp *pApp=AfxGetApp();
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	CString str_temp;
	str_temp.Format(L"%d",nPartCol_x);
	WritePrivateProfileStringW(L"DoColFrm", L"nPartCol_x", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",nPartCol_y);
	WritePrivateProfileStringW(L"DoColFrm", L"nPartCol_y", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//	pApp->WriteProfileInt (_T("DoColFrm"),_T("nPartCol_x"),nPartCol_x);
	// pApp->WriteProfileInt (_T("DoColFrm"),_T("nPartCol_y"),nPartCol_y);
	//end 20120927
	
	m_wndSplitter2.GetRowInfo(0,nPartCol_y,nPartCol_cy);
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	//pApp->WriteProfileInt (_T("DoColFrm"),_T("nInfoCol0_y"),nPartCol_y);
	str_temp.Format(L"%d",nPartCol_y);
	WritePrivateProfileStringW(L"DoColFrm", L"nInfoCol0_y", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	m_wndSplitter2.GetRowInfo(1,nPartCol_y,nPartCol_cy);
	//pApp->WriteProfileInt (_T("DoColFrm"),_T("nInfoCol_y"),nPartCol_y);
	str_temp.Format(L"",nPartCol_y);
	WritePrivateProfileStringW(L"DoColFrm", L"nInfoCol_y", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//end 20120927
	
	int nSel = m_wndColumn.GetCurSel();
	if(nSel >= 0)
	{
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		CString strTemp; 
		strTemp.Format(L"%d",m_wndColumn.GetItemData(nSel));
		WritePrivateProfileStringW(L"DoColFrm", L"column_id",strTemp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp->WriteProfileInt(_T("DoColFrm"),_T("column_id"), m_wndColumn.GetItemData(nSel));
		//end 20120927
	}
	return CChildMDIFrm::DestroyWindow();
}