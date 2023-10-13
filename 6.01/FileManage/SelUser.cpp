// SelUser.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "SelUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelUser dialog


CSelUser::CSelUser(CWnd* pParent /*=NULL*/)
	: CDialog(CSelUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelUser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelUser)
	DDX_Control(pDX, IDC_LISTUSER, m_clbList);
	DDX_Control(pDX, IDC_SYSTREE, m_ctcLib);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelUser, CDialog)
	//{{AFX_MSG_MAP(CSelUser)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SYSTREE, OnSelchangedSystree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelUser message handlers

BOOL CSelUser::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int nLib;

	// TODO: Add extra initialization here
	CDbTree dbtree;
	dbtree.AssignTree(&m_ctcLib);
	CGlobal *pGL = CGlobal::GetInstance();
	dbtree.AssignDb(pGL->m_FileDB.m_pDB,_T("news_lib"),_T("lib_id"),_T("parent_id"),_T("lib_name"));
	dbtree.BuildTree();
	CString str;
	pGL->m_FileDB.GetDefaultLib(nLib,str,pGL->m_pEmployee->m_nGroup);
	dbtree.SelectItem(nLib);
	LoadLibUser();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelUser::OnSelchangedSystree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	LoadLibUser();
	*pResult = 0;
}

void CSelUser::OnOK() 
{
	// TODO: Add extra validation here

	int nSel=m_clbList.GetCurSel();
	if(nSel!=LB_ERR)
		m_clbList.GetText(nSel,m_strUserName);
	else
	{
		AfxMessageBox(IDS_PLEASECHOOSEUSER);
		return;
		
	}
	CDialog::OnOK();
}

void CSelUser::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CSelUser::LoadLibUser()
{
	DWORD dwLibID;
	HTREEITEM hTreeItem;
	hTreeItem=m_ctcLib.GetSelectedItem();
	dwLibID=m_ctcLib.GetItemData(hTreeItem);
	
	m_clbList.ResetContent();
	CGlobal::GetInstance()->LoadLibUser(&m_clbList,dwLibID);
}
