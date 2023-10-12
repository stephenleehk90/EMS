
// DBGraphic.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DBGraphic.h"
#include "DBGraphicDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDBGraphicApp

int display=0;
int writeLog=0;
wchar_t sz_DatFile[ PATH_LENGTH ]={0};
//wchar_t sz_LogFile[ PATH_LENGTH ]={0};
CSQLApi  Database ;
CSQLApi  *p_dbCmd = &Database ;
//char m_szDatabase[NAME_LEN]={0};
//char m_szUser[NAME_LEN]={0};
wchar_t		m_szDatabase[NAME_LEN]={0};
wchar_t		m_szUser[NAME_LEN]={0};
BOOL		m_bStop = FALSE;
BOOL		m_bSybase=FALSE;
wchar_t		m_szLogin[NAME_SIZE]={0};
wchar_t		m_szServer[NAME_SIZE]={0};
wchar_t		m_szPassword[PSWD_SIZE]={0};
BOOL		m_bDbFail=FALSE;
wchar_t		m_szSysDB[20]={0};

wchar_t     m_SmtpHost[PATH_LENGTH]={0};
wchar_t     m_ToAdd[PATH_LENGTH]={0};
wchar_t     m_FromAdd[PATH_LENGTH]={0};
wchar_t     m_PassWord[PATH_LENGTH]={0};
int			m_nSecure;
int			m_nPort;


wchar_t     m_DummyPath[PATH_LENGTH]={0};
wchar_t		m_szSystemTempPath[PATH_LENGTH]={0};
wchar_t		m_szSystemLogPath[PATH_LENGTH]={0};
BOOL	m_bLoadAddInfo = FALSE;
HANDLE  g_hViewMutex = NULL;
BOOL bSupportMedia = FALSE, bMediaColumnExist = FALSE;
wchar_t sz_DefaultPhoto[PATH_LENGTH]={0};
CCrypto m_crypto;

BEGIN_MESSAGE_MAP(CDBGraphicApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDBGraphicApp construction

CDBGraphicApp::CDBGraphicApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDBGraphicApp object

CDBGraphicApp theApp;


// CDBGraphicApp initialization

BOOL CDBGraphicApp::InitInstance()
{  
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	HANDLE m_hMutex=CreateMutex(NULL,TRUE,m_pszAppName);

	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AfxMessageBox(L"方正圖片分發程式 已經在運行");
		ExitProcess(0);
		//	return FALSE;
	}
	wchar_t	   szPath[ PATH_LENGTH ];
	wchar_t    szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
	wchar_t    szFName[_MAX_FNAME], szExt[_MAX_EXT];
	CTime time;
	CString curDate;
	time=CTime::GetCurrentTime();
	// edit by leon 2012-08-24 for remove sys_newsroom from master to owning database
	lstrcpy(m_szSysDB,L"sys_newsroom");
	curDate.Format(L"%d%02d%02d",time.GetYear(),time.GetMonth(),time.GetDay());
	GetModuleFileName( m_hInstance, szPath, sizeof( szPath ) );
	_wsplitpath_s( szPath, szDrive, szDir, szFName, szExt );
	lstrcpy( szExt, L"INI");
	_wmakepath_s( szPath, szDrive, szDir, szFName, szExt );
	wchar_t  * szProfileName=(wchar_t*)malloc(PATH_LENGTH);
	lstrcpy( szProfileName, szPath);
	m_pszProfileName = szProfileName;
	_wmakepath_s( szPath, szDrive, szDir, szFName, szExt );
	lstrcpy( szExt, L"DAT" );

	_wmakepath_s( szPath, szDrive, szDir, szFName, szExt );
	lstrcpy( sz_DatFile , szPath );

//	_wmakepath_s( szPath, szDrive, szDir, szFName, szExt );
//	lstrcpy( szExt, L"log" );
//	lstrcat(szFName,curDate);
//	_wmakepath_s( szPath, szDrive, szDir, szFName, szExt );
//	lstrcpy( sz_LogFile, szPath );

	lstrcpy( szExt, L"jpg" );
	lstrcat(szDir,L"resource\\pics\\");
	lstrcpy(szFName,L"DefaultPic");
	_wmakepath_s( szPath, szDrive, szDir, szFName, szExt );
	//default photo and dummy photo//
//	lstrcpy( sz_DefaultPhoto, szPath );
	lstrcpy(m_DummyPath,szPath);
	///////////////////////////////
	
	m_crypto.DeriveKey(L"Founder");
	bool bSavePass=GetPrivateProfileIntW(L"Login",L"SavePass", 0,m_pszProfileName);

	//bSavePass=AfxGetApp()->GetProfileInt(L"Login",L"SavePass",0);
	if(bSavePass==TRUE)
	{
		wchar_t szPath[255];
//		lstrcpy((unsigned short *)szPath, L"dbtele.pwl");
		//lstrcpy(szPath, L"dbtele.pwl");
		CString str_Path; str_Path.Format(L"%s", szProfileName);
		str_Path.Replace(L".INI", L".pwl");
		lstrcpy(szPath, str_Path);
		if( _waccess( szPath, 0 ) == -1 )
			bSavePass=FALSE;
	}
	

	CDBGraphicDlg dlg;
	
	CString str_Path, str_Password; str_Path.Format(L"%s", szProfileName);
	str_Path.Replace(L".INI", L"");
	lstrcpy(szPath, str_Path);
	
	dlg.str_Path = str_Path;
	if(bSavePass==TRUE)
	{
		//GetUserPass(m_szPassword,CT2A(str_Path));
		//dlg.str_Password.Format(L"%s", m_szPassword);
		CFile fil;
		CByteArray arData;
		fil.Open ( str_Path + L".pwl" , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone );
		fil.SeekToBegin();

		arData.SetSize(static_cast<INT_PTR>(fil.GetLength()));
		fil.Read(arData.GetData(), static_cast<UINT>(fil.GetLength()));
		fil.Close();

		m_crypto.Decrypt(arData, dlg.str_Password);
	}
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

