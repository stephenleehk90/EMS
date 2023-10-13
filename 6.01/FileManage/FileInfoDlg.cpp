// FileInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "FileInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileInfoDlg dialog


CFileInfoDlg::CFileInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileInfoDlg)
	m_strTitle = _T("");
	m_strAuthor = _T("");
	m_bLock = FALSE;
	m_bHasPhoto = FALSE;;
	m_bSecret = FALSE;
	m_bExclusive = FALSE;
	m_strStyle = _T("");
	m_bUrgency = FALSE;
	m_dbWords = 0;
	//}}AFX_DATA_INIT
	m_bNewFile = FALSE;
	m_strFileCode=_T("");
	m_bModify = FALSE;
}


void CFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileInfoDlg)
	DDX_Text(pDX, IDC_COMMENT, m_strTitle);
	DDV_MaxChars(pDX, m_strTitle, 254);
	DDX_Text(pDX, IDC_AUTHOR, m_strAuthor);
	DDX_Check(pDX, IDC_LOCKCHECK, m_bLock);
	DDX_Check(pDX, IDC_PHOTOCHECK, m_bHasPhoto);
	DDX_Check(pDX, IDC_SECRETCHECK, m_bSecret);
	DDX_Check(pDX, IDC_SINGLECHECK, m_bExclusive);
	DDX_CBString(pDX, IDC_STYLE, m_strStyle);
	DDX_Check(pDX, IDC_URGENCYCHECK, m_bUrgency);
	DDX_Text(pDX, IDC_WORD, m_dbWords);
	DDX_Control(pDX, IDC_AUTHOR, edtTest);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CFileInfoDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CFileInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileInfoDlg message handlers

BOOL CFileInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CGlobal*pGL=CGlobal::GetInstance();
	
	// Below modify by Bob 2006-4-19
	// ㄓ絪亩家Αэノ╰参把计秸竊北
/*#ifdef _MINGPAO_DEMO
	(CWnd*)GetDlgItem(IDC_SINGLECHECK)->EnableWindow(FALSE);
#endif*/
	// 縒產絑ンパIsSingleFile北
	CString strIsSingleFile;
	pGL->m_FileDB.GetConfig(_T("IsSingleFile"),strIsSingleFile);
	if (strIsSingleFile == _T("1"))
		(CWnd*)GetDlgItem(IDC_SINGLECHECK)->EnableWindow(TRUE);
	else
		(CWnd*)GetDlgItem(IDC_SINGLECHECK)->EnableWindow(FALSE);
	// End modify by Bob
	CString str;
	CString strUser;
	if(!m_bNewFile)
	{
		pGL->m_FileDB.LoadFileInfo(m_strFileCode,m_strTitle,&m_dbWords,&m_bExclusive,&m_bSecret,
			&m_bUrgency,&m_bLock,m_strAuthor,m_strStyle,str, strUser);
	}
	if(!m_bModify)
		DisableAllWnd();
	else
	{
		//if(!pGL->m_pEmployee->m_bUrgency)
		//	GetDlgItem ( IDC_URGENCYCHECK )->EnableWindow ( FALSE );
		//else
		//	GetDlgItem ( IDC_URGENCYCHECK )->EnableWindow ( TRUE );

		//if ( !pGL->m_pEmployee->m_bSecret)
		//	GetDlgItem ( IDC_SECRETCHECK )->EnableWindow ( FALSE );
		//else
		//	GetDlgItem ( IDC_SECRETCHECK )->EnableWindow ( TRUE );

		BOOL bUrgency;
		BOOL bSecret;

		bUrgency = FALSE;
		bSecret = FALSE;

		pGL->m_FileDB.GetLibUrgency(m_nLibID,m_strFileCode,bUrgency);
		pGL->m_FileDB.GetLibSecret(m_nLibID,m_strFileCode,bSecret);

		/*
		if(!bUrgency)
			GetDlgItem ( IDC_URGENCYCHECK )->EnableWindow ( FALSE );
		else
			GetDlgItem ( IDC_URGENCYCHECK )->EnableWindow ( TRUE );
		
		if ( !bSecret)
			GetDlgItem ( IDC_SECRETCHECK )->EnableWindow ( FALSE );
		else
			 TRUE );
		*/

		BOOL bEnable = (strUser.CompareNoCase(pGL->m_pEmployee->m_strUserCode) == 0) ||
			(m_strAuthor.CompareNoCase(pGL->m_pEmployee->m_strUserName)==0);

		CheckDlgButton(IDC_URGENCYCHECK, bUrgency);
		CheckDlgButton(IDC_SECRETCHECK, bSecret);
	
		if(!m_bNewFile)
		{
			GetDlgItem ( IDC_SECRETCHECK )->EnableWindow (bEnable);
			GetDlgItem ( IDC_URGENCYCHECK )->EnableWindow (bEnable);
			// Modify by Bob 2006-4-19
/*#ifndef _MINGPAO_DEMO
			GetDlgItem ( IDC_SINGLECHECK )->EnableWindow(bEnable);
#endif*/
			if (strIsSingleFile == _T("1"))
				GetDlgItem ( IDC_SINGLECHECK )->EnableWindow(bEnable);
			else
				GetDlgItem ( IDC_SINGLECHECK )->EnableWindow(FALSE);
			// End modify by Bob
		}		
		
	}
	UpdateData(FALSE);

	//Create the ToolTip control
	if( !m_ToolTip.Create(this))
	{
	   TRACE0("Unable to create the ToolTip!");
	}
	else
	{
	  // Add tool tips to the controls, either by hard coded string 
	  // or using the string table resource
	  m_ToolTip.AddTool( &edtTest, _T(""));

	  m_ToolTip.Activate(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	
	CDialog::OnOK();
}
void CFileInfoDlg::DisableAllWnd()
{
	(CWnd*)GetDlgItem(IDC_COMMENT)->EnableWindow(FALSE);

	(CWnd*)GetDlgItem(IDC_STYLE)->EnableWindow(FALSE);
	(CWnd*)GetDlgItem(IDC_SINGLECHECK)->EnableWindow(FALSE);
	(CWnd*)GetDlgItem(IDC_WORD)->EnableWindow(FALSE);
	(CWnd*)GetDlgItem(IDC_PHOTOCHECK)->EnableWindow(FALSE);

	(CWnd*)GetDlgItem(IDC_AUTHOR)->EnableWindow(FALSE);
	
	(CWnd*)GetDlgItem(IDC_URGENCYCHECK)->EnableWindow(FALSE);
	(CWnd*)GetDlgItem(IDC_SECRETCHECK)->EnableWindow(FALSE);
	(CWnd*)GetDlgItem(IDC_LOCKCHECK)->EnableWindow(FALSE);
	(CWnd*)GetDlgItem(IDOK)->ShowWindow(FALSE);
}

BOOL CFileInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	     m_ToolTip.RelayEvent(pMsg);

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			OnCancel();
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CFileInfoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
