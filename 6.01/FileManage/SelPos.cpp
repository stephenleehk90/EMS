// SelPos.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "SelPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelPos dialog


CSelPos::CSelPos(CWnd* pParent /*=NULL*/)
	: CDialog(CSelPos::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelPos)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelPos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelPos)
	DDX_Control(pDX, IDC_PAPCOMBO, m_ccbPaper);
	DDX_Control(pDX, IDC_LIBTREE, m_ctcLib);
	DDX_Control(pDX, IDC_COLUMNLIST, m_clbColumn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelPos, CDialog)
	//{{AFX_MSG_MAP(CSelPos)
	ON_BN_CLICKED(IDC_LIBRADIO, OnLibradio)
	ON_BN_CLICKED(IDC_PAPRADIO, OnPapradio)
	ON_CBN_SELCHANGE(IDC_PAPCOMBO, OnSelchangePapcombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelPos message handlers

void CSelPos::OnOK() 
{
	// TODO: Add extra validation here
	CWinApp *pApp ;
	pApp = (CWinApp*)AfxGetApp();
	if ( IsDlgButtonChecked(IDC_PAPRADIO) )
	{
		m_bType = FALSE;
		m_ccbPaper.GetWindowText(m_strPaper);
		if( m_strPaper.IsEmpty() )
			return;
		int nSel = m_clbColumn.GetCurSel();
		if(nSel==LB_ERR)return;
		m_clbColumn.GetText ( nSel , m_strColumn );
		if(m_strColumn.IsEmpty())
			return ;
		m_nColID=m_clbColumn.GetItemData(nSel);
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		WritePrivateProfileStringW(L"SelPos", L"Radio" , L"0",((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp ->WriteProfileInt ( _T("SelPos") , _T("Radio") , FALSE );
		nSel = m_ccbPaper.GetCurSel();
		CString str_temp;
		str_temp.Format(L"%d",nSel);
		WritePrivateProfileStringW(L"SelPos", L"Paper" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp ->WriteProfileInt ( _T("SelPos") , _T("Paper") , nSel );
		//
	}
	else
	{
		m_bType = TRUE;
		HTREEITEM hItem ;
		if ( (hItem = m_ctcLib.GetSelectedItem () )!=NULL)
		{
			m_nLib = m_ctcLib.GetItemData(hItem);
			m_strLibName = m_ctcLib.GetItemText ( hItem ) ;
		}
		else
			return ;
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		CString str_temp;
		str_temp.Format(L"%d",m_nLib);
		WritePrivateProfileStringW(L"SelPos", L"Lib" , str_temp.AllocSysString(),((CWinApp*)AfxGetApp())->m_pszProfileName);
		WritePrivateProfileStringW(L"SelPos", L"Radio" , L"1",((CWinApp*)AfxGetApp())->m_pszProfileName);
		//pApp ->WriteProfileInt ( _T("SelPos") , _T("Lib") , m_nLib );
		//pApp ->WriteProfileInt ( _T("SelPos") , _T("Radio") , TRUE );
		//end 20120927
	}
	CDialog::OnOK();
}

BOOL CSelPos::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWinApp *pApp ;
	pApp = (CWinApp *)AfxGetApp();
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	WORD wLib = GetPrivateProfileIntW( L"SelPos" , L"Lib",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	BOOL bSel =GetPrivateProfileIntW( L"SelPos" , L"Radio",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	int nSel =GetPrivateProfileIntW( L"SelPos" , L"Paper",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//WORD wLib = pApp->GetProfileInt ( _T("SelPos") , _T("Lib")  , 0 );
	//BOOL bSel = pApp->GetProfileInt ( _T("SelPos") , _T("Radio") , FALSE );
	//int nSel = pApp->GetProfileInt ( _T("SelPos") , _T("Paper") , 0 );
	//end 20120927
	if ( bSel == FALSE )
	{
		CheckDlgButton(IDC_LIBRADIO , FALSE);
		CheckDlgButton(IDC_PAPRADIO , TRUE);
		GetDlgItem(IDC_LIBTREE)->ShowWindow ( SW_HIDE );
		GetDlgItem(IDC_PAPCOMBO)->ShowWindow ( SW_SHOW);
		GetDlgItem(IDC_COLUMNLIST)->ShowWindow ( SW_SHOW);
	}
	else
	{
		CheckDlgButton(IDC_LIBRADIO , TRUE);
		CheckDlgButton(IDC_PAPRADIO , FALSE);
		GetDlgItem(IDC_LIBTREE)->ShowWindow ( SW_SHOW );
		GetDlgItem(IDC_PAPCOMBO)->ShowWindow ( SW_HIDE);
		GetDlgItem(IDC_COLUMNLIST)->ShowWindow ( SW_HIDE);
	}
	CDbTree dbtree;
	dbtree.AssignTree(&m_ctcLib);
	CGlobal *pGL = CGlobal::GetInstance();
	dbtree.AssignDb(pGL->m_FileDB.m_pDB,_T("news_lib"),_T("lib_id"),_T("parent_id"),_T("lib_name"));
	dbtree.BuildTree();
	CString sql;
	sql.Format(_T("select lib_id from list_permission where group_id = %d and perm_style = 6 and (perm_range = 'M' or perm_range = 'S') "), pGL->m_pEmployee->m_nGroup );
	dbtree.Filter(sql);
	dbtree.SelectItem(wLib);
	
	pGL->m_FileDB.LoadNewspaper(&m_ccbPaper,pGL->m_pEmployee->m_nGroup,_T('S'));
	m_ccbPaper.SetCurSel(nSel);
	CString strPaper;
	m_ccbPaper.GetWindowText(strPaper) ;
	if(!strPaper.IsEmpty())
	{
		pGL->LoadColumn(&m_clbColumn,strPaper,pGL->m_pEmployee->m_nGroup,_T('R'));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelPos::FillSetting()
{
	BOOL bSel ;
	bSel = IsDlgButtonChecked(IDC_LIBRADIO);
	if ( bSel == FALSE )
	{
		CheckDlgButton(IDC_LIBRADIO , FALSE);
		CheckDlgButton(IDC_PAPRADIO , TRUE);
		GetDlgItem(IDC_LIBTREE)->ShowWindow ( SW_HIDE );
		GetDlgItem(IDC_PAPCOMBO)->ShowWindow ( SW_SHOW);
		GetDlgItem(IDC_COLUMNLIST)->ShowWindow ( SW_SHOW);
	}
	else
	{
		CheckDlgButton(IDC_LIBRADIO , TRUE);
		CheckDlgButton(IDC_PAPRADIO , FALSE);
		GetDlgItem(IDC_LIBTREE)->ShowWindow ( SW_SHOW );
		GetDlgItem(IDC_PAPCOMBO)->ShowWindow ( SW_HIDE);
		GetDlgItem(IDC_COLUMNLIST)->ShowWindow ( SW_HIDE);
	}
}

void CSelPos::OnLibradio() 
{
	// TODO: Add your control notification handler code here
	FillSetting();
}

void CSelPos::OnPapradio() 
{
	// TODO: Add your control notification handler code here
	FillSetting();
}

void CSelPos::OnSelchangePapcombo() 
{
	// TODO: Add your control notification handler code here
	CString strPaper;
	m_ccbPaper.GetWindowText(strPaper) ;
	if(!strPaper.IsEmpty())
	{
		CGlobal *pGL=CGlobal::GetInstance();
		pGL->LoadColumn(&m_clbColumn,strPaper,pGL->m_pEmployee->m_nGroup,_T('R'));
	}
}

BOOL CSelPos::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			OnCancel();
	}
    return CDialog::PreTranslateMessage(pMsg);
}