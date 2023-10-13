// LinkToDMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "LinkToDMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinkToDMDlg dialog


CLinkToDMDlg::CLinkToDMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinkToDMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinkToDMDlg)
	m_bShowAll = FALSE;
	m_odtSelDate = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT

	m_pGlobal = NULL;
	m_bInitializing = true;

	m_nDeptFileInfoId = -1;
}


void CLinkToDMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinkToDMDlg)
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtcDate);
	DDX_Control(pDX, IDC_LIBTREE, m_tvLib);
	DDX_Control(pDX, IDC_FILIST, m_clcDeptFileInfo);
	DDX_Check(pDX, IDC_CHECK_SHOWALL, m_bShowAll);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_odtSelDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinkToDMDlg, CDialog)
	//{{AFX_MSG_MAP(CLinkToDMDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_LIBTREE, OnSelchangedLibtree)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_CHECK_SHOWALL, OnCheckShowall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinkToDMDlg message handlers

BOOL CLinkToDMDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_clcDeptFileInfo.SetExtendedStyle(m_clcDeptFileInfo.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	InitImageList();
	InitHeader();

	m_bInitializing = true;
	m_nDeptFileInfoId = -1;

	m_pGlobal = CGlobal::GetInstance();
	m_odtSelDate = COleDateTime::GetCurrentTime();
	
	COleDateTimeSpan dts(1,0,0,0);
	dts -= m_pGlobal->m_pEntry->tsSpanOfDay;
	
	m_odtSelDate += dts;

	InitDMLibTree();

	m_dtcDate.SetFormat(DATETIMEPICKERDATEFORMAT);
	
	UpdateData(FALSE);

	m_bInitializing = false;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLinkToDMDlg::OnSelchangedLibtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if (!m_bInitializing)
	{
		UpdateData();
		LoadDMList();
	}

	*pResult = 0;
}

void CLinkToDMDlg::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if (!m_bInitializing)
	{
		UpdateData();
		LoadDMList();
	}
	*pResult = 0;
}

void CLinkToDMDlg::OnOK() 
{
	// TODO: Add extra validation here
	int nListItem = m_clcDeptFileInfo.GetNextItem ( -1 , LVNI_SELECTED ) ;
	if ( nListItem == -1 )
	{
		AfxMessageBox(IDS_P_SELECT_DMFI);
		return ;
	}
	
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_PARAM ;
	lvi.iItem = nListItem; 
	lvi.iSubItem = 0; 
	
	m_clcDeptFileInfo.GetItem(&lvi);
	
	if (lvi.iImage == 1)
	{
		if (AfxMessageBox(IDS_Q_REPLACE_LINK,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
			return;
		
		CString strFilecode;
		BOOL bLinked = m_pGlobal->m_FileDB.GetLinkedFilecode(int(lvi.lParam), strFilecode);
		if (bLinked && (strFilecode.GetLength() > 0))
		{
			if (!m_pGlobal->m_FileDB.CanUnlink((LPCTSTR)strFilecode, m_pGlobal->m_pEmployee->m_nGroup))
			{
				AfxMessageBox(IDS_E_CANNOT_REPLACE_LINK);
				return;
			}
		}
		
	}
	
	m_nDeptFileInfoId = m_clcDeptFileInfo.GetItemData(nListItem);

	HTREEITEM hTreeItem = m_tvLib.GetSelectedItem();
	if(hTreeItem != NULL)
	{	
		DWORD dwLibID = m_tvLib.GetItemData(hTreeItem);
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		CString str_temp;
		str_temp.Format(L"%d",dwLibID);
		WritePrivateProfileStringW(L"LinkToDMDlg", L"LibID" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//((CWinApp*)AfxGetApp())->WriteProfileInt(L"LinkToDMDlg", L"LibID", dwLibID);
		//end 20120927
	}
	CDialog::OnOK();
}

void CLinkToDMDlg::InitImageList()
{
	CBitmap				bitmap;

	m_ImageListOfTree.Create(16, 16, ILC_COLOR24|ILC_MASK, 2, 1);
	
	bitmap.LoadBitmap(IDB_BMTREENODE);
	m_ImageListOfTree.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BMTREENODESEL);
	m_ImageListOfTree.Add(&bitmap, ARTMASKCOLOR);
	bitmap.DeleteObject();

	m_tvLib.SetImageList(&m_ImageListOfTree, TVSIL_NORMAL);

	m_ImageListOfList.Create(16, 16, ILC_MASK/*bMask*/,	2/*nInitial*/, 1/*nGrow*/);
	
	bitmap.LoadBitmap(IDB_BMTREELEAF);
	m_ImageListOfList.Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	
	bitmap.LoadBitmap(IDB_BMFINISH);
	m_ImageListOfList.Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	
	m_clcDeptFileInfo.SetImageList(&m_ImageListOfList, LVSIL_SMALL);

}

void CLinkToDMDlg::InitHeader()
{
	CRect			rect;
	int				i;
	LV_COLUMN		lvcolumn;
	
	int				rgtcx[3] = { 8,8,4};
	
    m_clcDeptFileInfo.GetWindowRect(&rect);
	
	// Insert ListCtrl Columns
	for (i = 0; i < 3; i++)  // add the columns to the list control
	{
		CString strHeader;

		strHeader.LoadString(IDS_DMLIST_HEADER0 + i);
		lvcolumn.mask =LVIF_STATE |LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = (LPTSTR)(LPCTSTR)strHeader;
		lvcolumn.iSubItem = i;
		lvcolumn.cx = rect.Width() * rgtcx[i] /21;  // SubItem is twice as large
		m_clcDeptFileInfo.InsertColumn(i, &lvcolumn);  // assumes return value is OK.
	}
	

}

void CLinkToDMDlg::InitDMLibTree()
{
	CDbTree dbTree;
	
	dbTree.AssignTree(&m_tvLib);
	dbTree.AssignDb(m_pGlobal->m_FileDB.m_pDB, _T("news_lib"), _T("lib_id"), _T("parent_id"), _T("lib_name"));
	dbTree.BuildTree();
	
	m_pGlobal->m_FileDB.GetDMValidLibIds(m_pGlobal->m_pEmployee->m_nGroup, m_waFilter);
	
	dbTree.Filter(m_waFilter);

	WORD dwLibID;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	dwLibID = GetPrivateProfileIntW( L"LinkToDMDlg" , L"LibID",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//dwLibID=((CWinApp*)AfxGetApp())->GetProfileInt(L"LinkToDMDlg",L"LibID",0);
	//end 20120927

	
	HTREEITEM hi = m_tvLib.GetRootItem();
	if (hi)
		ExpandBranch(&m_tvLib, hi);

	if (IsInArrayLibID(m_waFilter, dwLibID))
	{
		dbTree.SelectItem(dwLibID);
		LoadDMList();
	}
}

BOOL CLinkToDMDlg::IsInArrayLibID(CWordArray &arryLibID, DWORD wLib)
{
	BOOL bRet = FALSE;

	for(int i=0; i<arryLibID.GetSize(); i++)
	{
		if(arryLibID[i] == wLib)
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet;
	
}

void CLinkToDMDlg::LoadDMList()
{
	HTREEITEM hTreeItem = m_tvLib.GetSelectedItem();
	if(hTreeItem == NULL)
		return;

	DWORD dwLibID = m_tvLib.GetItemData(hTreeItem);

	CString strDate = m_odtSelDate.Format(L"%Y-%m-%d");

	BOOL bHaveDMPerm = FALSE;

	if (IsInArrayLibID(m_waFilter, dwLibID))
	{
		int nPerm = m_pGlobal->m_FileDB.GetDMPrivilege(m_pGlobal->m_pEmployee->m_nGroup, dwLibID);
		bHaveDMPerm = ((nPerm & 1) + (nPerm & 2)) > 0;
	}

	m_clcDeptFileInfo.DeleteAllItems();

	m_pGlobal->LoadDeptFileInfo(&m_clcDeptFileInfo, strDate, dwLibID, bHaveDMPerm, !m_bShowAll);
	
}

void CLinkToDMDlg::OnCheckShowall() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	LoadDMList();
}

BOOL CLinkToDMDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			OnCancel();
	}
    return CDialog::PreTranslateMessage(pMsg);
}