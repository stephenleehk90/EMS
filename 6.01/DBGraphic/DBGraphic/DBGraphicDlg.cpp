
// DBGraphicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DBGraphic.h"
#include "DBGraphicDlg.h"
#include "afxdialogex.h"
#include "DBGraphicMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDBGraphicDlg dialog



CDBGraphicDlg::CDBGraphicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBGraphicDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pwd = _T("");
	m_server = _T("");
	m_user = _T("");
}

void CDBGraphicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PWD, m_pwd);
	DDX_Text(pDX, IDC_SERVER, m_server);
	DDX_Text(pDX, IDC_USER, m_user);
}

BEGIN_MESSAGE_MAP(CDBGraphicDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDAPPLY, &CDBGraphicDlg::OnBnClickedApply)
ON_BN_CLICKED(IDAPPLY, &CDBGraphicDlg::OnApply)
END_MESSAGE_MAP()


// CDBGraphicDlg message handlers
BOOL CDBGraphicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	CString strServer,strLogin,strDummy;
	CWinApp *pApp;
	wchar_t wServer[100]=L"";
	pApp=(CWinApp*)AfxGetApp();
	GetPrivateProfileStringW( L"System" , L"ServerName",NULL, wServer,100,pApp->m_pszProfileName);	
//	GetPrivateProfileStringW( L"System" , L"DummyPath",NULL, m_DummyPath,100,pApp->m_pszProfileName);
	strServer=wServer;
	//edit by leon 2012-08-27 for migrating from vc6 to 2010
// 	strServer=pApp->GetProfileString(L"System",L"ServerName",NULL);
// 	strDummy=pApp->GetProfileString(L"System",L"DummyPath",NULL);
	display=GetPrivateProfileIntW(L"System",L"Debug",0,pApp->m_pszProfileName);
	writeLog=GetPrivateProfileIntW(L"System",L"writeLog",0,pApp->m_pszProfileName);
	wchar_t szBuffer[40] = {0};	
	CString str_temp;
	GetPrivateProfileStringW( L"LOGIN" , L"NAME" , L"sa",szBuffer,100,pApp->m_pszProfileName);
//	lstrcpy(m_DummyPath,strDummy);
	((CEdit*)GetDlgItem(IDC_USER))->SetWindowText (szBuffer ) ;
	SetDlgItemText(IDC_PWD, L"");
	SetDlgItemText(IDC_SERVER , strServer ) ;
	GotoDlgCtrl(GetDlgItem(IDC_PWD));
	if(str_Password.GetLength()>0&&!str_Password.IsEmpty())
	{
		CheckDlgButton(IDC_SAVEPASSWORDCHECK, TRUE);	
		SetDlgItemText(IDC_PWD, str_Password);
	}
	else
		SetDlgItemText(IDC_PWD, L"");
//  edit by leon 2012-08-27 for migrating from vc6 to 2010
// 	str_temp=pApp->GetProfileString(L"SENDEMAIL",L"SMTPSERVER",L"smtp.example.com");
// 	lstrcpy(m_SmtpHost,str_temp);
// 	str_temp=pApp->GetProfileString(L"SENDEMAIL",L"TOADD",L"FromAdd@example.com.hk");
// 	lstrcpy(m_ToAdd,str_temp);
// 	str_temp=pApp->GetProfileString(L"SENDEMAIL",L"FROMADD",L"ToAdd@example.com.hk");
// 	lstrcpy(m_FromAdd,str_temp);

	GetPrivateProfileStringW(L"SENDEMAIL",L"SMTPSERVER",L"smtp.example.com",m_SmtpHost,300,pApp->m_pszProfileName);
	GetPrivateProfileStringW(L"SENDEMAIL",L"TOADD",L"FromAdd@example.com.hk",m_ToAdd,300,pApp->m_pszProfileName);
	GetPrivateProfileStringW(L"SENDEMAIL",L"FROMADD",L"ToAdd@example.com.hk",m_FromAdd,300,pApp->m_pszProfileName);
	GetPrivateProfileStringW(L"SENDEMAIL",L"FROMADD",L"ToAdd@example.com.hk",m_FromAdd,300,pApp->m_pszProfileName);
	GetPrivateProfileStringW(L"SENDEMAIL",L"PASSWORD",L"12345",m_PassWord,300,pApp->m_pszProfileName);
	m_nSecure=GetPrivateProfileIntW(L"SENDEMAIL",L"SECURETYPE",0,pApp->m_pszProfileName);
	m_nPort=GetPrivateProfileIntW(L"SENDEMAIL",L"PORT",25,pApp->m_pszProfileName);

	WritePrivateProfileStringW(L"SENDEMAIL",L"SMTPSERVER",m_SmtpHost,pApp->m_pszProfileName);
	WritePrivateProfileStringW(L"SENDEMAIL",L"TOADD",m_ToAdd,pApp->m_pszProfileName);
	WritePrivateProfileStringW(L"SENDEMAIL",L"FROMADD",m_FromAdd,pApp->m_pszProfileName);
	WritePrivateProfileStringW(L"SENDEMAIL",L"PASSWORD",m_PassWord,pApp->m_pszProfileName);
	CString strTemp;
	strTemp.Format(L"%d",m_nSecure);
	WritePrivateProfileStringW(L"SENDEMAIL",L"SECURETYPE",strTemp,pApp->m_pszProfileName);
	strTemp.Format(L"%d",m_nPort);
	WritePrivateProfileStringW(L"SENDEMAIL",L"PORT",strTemp,pApp->m_pszProfileName);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDBGraphicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDBGraphicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDBGraphicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CDBGraphicDlg::OnBnClickedApply()ds
//{
//	// TODO: Add your control notification handler code here
//}


void CDBGraphicDlg::OnApply()
{
	// TODO: Add your control notification handler code here
	CWinApp *pApp ;
	pApp = (CWinApp*)AfxGetApp();
	GetDlgItemText(IDC_SERVER,m_szServer,sizeof(m_szServer));
	GetDlgItemText ( IDC_PWD , m_szPassword , sizeof ( m_szPassword ) );
	GetDlgItemText ( IDC_USER , m_szLogin , sizeof ( m_szLogin ) );

	if ( p_dbCmd->UserLogin ( m_szServer , m_szLogin , m_szPassword ) )
	{
		
		WritePrivateProfileStringW( L"LOGIN" , L"NAME" , m_szLogin,pApp->m_pszProfileName);
		WritePrivateProfileStringW(L"System", L"ServerName", m_szServer,pApp->m_pszProfileName);
	}
	else
	{
		AfxMessageBox (AFX_IDS_CONNECTFAIL);
		return ;
	}
 	BOOL bSavePass=IsDlgButtonChecked(IDC_SAVEPASSWORDCHECK);
	CString str_SavePass;str_SavePass.Format(L"%d",bSavePass); 
 	WritePrivateProfileStringW( L"Login" , L"SavePass" ,str_SavePass,AfxGetApp()->m_pszProfileName);
 	if(bSavePass)
 	{
 		//SaveUserPass(m_szPassword,CT2A(str_Path));
		CByteArray arData;
		m_crypto.Encrypt(m_szPassword, arData);

		CFile fil;
		fil.Open (str_Path + L".pwl", CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone );
		fil.SeekToBegin();
		fil.Write(arData.GetData(), static_cast<UINT>(arData.GetCount()));
		fil.Close();
 	}

	CString strValue;
	wchar_t szValue[10];
	p_dbCmd->GetConfigure(L"SupportMedia", szValue);
	strValue = szValue;
	if(strValue == L"1")
		bSupportMedia = TRUE;

	int iExist = 0;
	RETCODE nResult;
	SDWORD	cbValue ;
	p_dbCmd->ODBCAllocStmt ();
	nResult = p_dbCmd->ODBCExecCmdW(L"if exists(select * from sys.columns "
			L" where Name = N'%s' and Object_ID = Object_ID(N'%s')) "
			L"begin "
			L"	select 1 "
			L"end "
			L"else "
			L"	select 0", L"db_media_type", L"dbgraphi_info");
	p_dbCmd->ODBCBind(1, SQL_C_LONG, &iExist, 0, &cbValue);
	while (nResult != SQL_NO_DATA_FOUND) 
	{
		 nResult = p_dbCmd->ODBCFetch();
		 if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		 {
			strValue.Format(L"%d", iExist);
		 }
		 else
			nResult = p_dbCmd->ODBCMoreResults();
	}
	p_dbCmd->ODBCFreeStmt(SQL_DROP);
	bMediaColumnExist = FALSE;
	if(strValue == L"1")
		bMediaColumnExist = TRUE;

	CDBGraphicMain dlg;	
	CDialog::OnOK();
	dlg.DoModal();
	
}

INT_PTR CDBGraphicDlg::DoModal(){
	CWinApp* pApp=AfxGetApp();
	CString strTemp;
	GetPrivateProfileStringW(L"System",L"TemporaryPath",DISKHISTORYADD,m_szSystemTempPath,PATH_LENGTH,pApp->m_pszProfileName);
	//GetPrivateProfileStringW(L"System",L"LogPath",DISKLOGADD,m_szSystemLogPath,PATH_LENGTH,pApp->m_pszProfileName);
	strTemp=m_szSystemTempPath;
	strTemp+=L"\\log";
	lstrcpy(m_szSystemLogPath,strTemp);
	m_bLoadAddInfo=GetPrivateProfileIntW(L"System",L"LoadAddInfo",0,pApp->m_pszProfileName);
	if (!MakeDir(m_szSystemTempPath) || !MakeDir(m_szSystemLogPath))
	{
		AfxMessageBox(L"請在INI設置系統路徑");
		return FALSE;
	}

	// Edit Stephen 2014-01-13
	CString strTemp_Image;
	strTemp_Image=m_szSystemTempPath;
	strTemp_Image+=L"\\temp_image";
	MakeDir(strTemp_Image);
	// Edit Stephen 2014-01-13

	strTemp.Format(L"%d",m_bLoadAddInfo);
	WritePrivateProfileStringW(L"System",L"LoadAddInfo",strTemp,pApp->m_pszProfileName);
	WritePrivateProfileStringW(L"System",L"TemporaryPath",m_szSystemTempPath,pApp->m_pszProfileName);
//	WritePrivateProfileStringW(L"System",L"LogPath",m_szSystemLogPath,pApp->m_pszProfileName);
	return CDialog::DoModal();
}