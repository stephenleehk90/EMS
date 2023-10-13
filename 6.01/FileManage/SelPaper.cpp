// SelPaper.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "SelPaper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelPaper dialog


CSelPaper::CSelPaper(CWnd* pParent /*=NULL*/)
	: CDialog(CSelPaper::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelPaper)
	m_strNote = _T("");
	m_bSecret = FALSE;
	m_bUser = FALSE;
	//}}AFX_DATA_INIT
}


void CSelPaper::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelPaper)
	DDX_Control(pDX, IDC_SECRET, m_chkSecret);
	DDX_Control(pDX, IDC_USERCOMBO, m_combUser);
	DDX_Control(pDX, IDC_LIBTREE, m_LibTree);
	DDX_Control(pDX, IDC_COLUMNLIST, m_ColumnList);
	DDX_Control(pDX, IDC_DT, m_btnDate);
	DDX_Text(pDX, IDC_NOTE, m_strNote);
	DDX_Check(pDX, IDC_SECRET, m_bSecret);
	DDX_Check(pDX, IDC_USERCHECK, m_bUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelPaper, CDialog)
	//{{AFX_MSG_MAP(CSelPaper)
	ON_BN_CLICKED(IDC_USERCHECK, OnUsercheck)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COLUMNLIST, OnItemchangedColumnlist)
	ON_NOTIFY(TVN_SELCHANGED, IDC_LIBTREE, OnSelchangedLibtree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelPaper message handlers

void CSelPaper::InitList()
{
	CRect			rect;
	int				i ;
	LV_COLUMN		lvcolumn;
	CString			rgtsz[2] ;
	int				rgtcx[2] = {46,46};
	CResManage rs(g_hInst);
	rgtsz[0].LoadString(IDS_PAPER);
	rgtsz[1].LoadString(IDS_COLUMN);
	m_ColumnList.GetWindowRect(&rect);

	// Insert ListCtrl Columns
	for (i = 0; i < 2; i++)  // add the columns to the list control
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = (LPSTR)(LPCTSTR)rgtsz[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = rect.Width() * rgtcx[i] / 100;  // SubItem is twice as large
		m_ColumnList.InsertColumn(i, &lvcolumn);  // assumes return value is OK.
	}
}

BOOL CSelPaper::OnInitDialog() 
{
	m_ColumnList.SubclassCtrl( IDC_COLUMNLIST, this);
	m_LibTree.SubclassDlgItem( IDC_LIBTREE, this);
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitList();
	VARIANT vdt;
	vdt.vt = VT_DATE;
	vdt.date = DATE(COleDateTime::GetCurrentTime());
	m_btnDate.SetValue(vdt);

	//build lib tree
	CDbTree dbtree;
	dbtree.AssignTree(&m_LibTree);
	CGlobal *pGL = CGlobal::GetInstance();
	dbtree.AssignDb(pGL->m_FileDB.m_pDB,_T("news_lib"),_T("lib_id"),_T("parent_id"),_T("lib_name"));
	dbtree.BuildTree();
	CString sql;
	sql.Format(_T("select t1.lib_id from news_column t1, column_permission t2 where group_id = %d" 
		" and t1.paper_code = t2.paper_code and t1.column_code = t2.column_code "), pGL->m_pEmployee->m_nGroup );
	dbtree.Filter(sql);

	CWinApp *pApp= (CWinApp*)AfxGetApp();
	int nLib;
	nLib = pApp->GetProfileInt ( "SelPaper" , "Lib" , m_nLibID );
	if(dbtree.SelectItem(nLib))
		m_nLibID = nLib;
	m_bUser = pApp->GetProfileInt ( "SelPaper" , "UserCheck" , 0 );
	m_bSecret = pApp->GetProfileInt ( "SelPaper" , "SecretCheck" , 0 );
	
	OnUsercheck();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelPaper::OnUsercheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_bUser)
	{
		CWinApp *pApp=(CWinApp*)AfxGetApp();
		CString strUser=pApp->GetProfileString ( "SelPaper" , "User" , NULL ) ;
		m_combUser.EnableWindow(TRUE);
		CGlobal *pGL=CGlobal::GetInstance();
		if(pGL->HasSecretPerm())
		{
			m_chkSecret.EnableWindow(TRUE);
		}
		else
			m_chkSecret.EnableWindow(FALSE);
		m_combUser.SelectString(-1,strUser);

	}
	else
	{
		m_combUser.EnableWindow(FALSE);
		m_chkSecret.EnableWindow(FALSE);
		m_bSecret=0;
		UpdateData(FALSE);
	}
}

void CSelPaper::OnItemchangedColumnlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMListView->iItem==NULL)
		return;
	m_strPaper=m_ColumnList.GetItemText(pNMListView->iItem,0);
	m_strCol = m_ColumnList.GetItemText(pNMListView->iItem,1 );
	if(m_bUser)
		m_pMgr->LoadColUser(&m_combUser , &m_strPaper , &m_strCol , m_nLibID);
	*pResult = 0;
}

void CSelPaper::OnSelchangedLibtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMTreeView->itemNew.hItem==NULL)
		return;
	m_nLibID = m_LibTree.GetItemData (pNMTreeView->itemNew.hItem) ;
	m_pMgr->LoadColumn(&m_ColumnList , m_nLibID , "S" );

	//user
	m_combUser.ResetContent();
	if(m_bUser)
		m_pMgr->LoadColUser(&m_combUser , &m_strPaper , &m_strCol , m_nLibID);
	*pResult = 0;
}
