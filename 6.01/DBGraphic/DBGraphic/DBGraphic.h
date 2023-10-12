
// DBGraphic.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CDBGraphicApp:
// See DBGraphic.cpp for the implementation of this class
//
#include "..\..\dbtools\sqltools.h"
#include "..\..\commkits\commkits.h"
#include "..\..\commkits\dostools.h"
#include "..\..\Editroom\globaldef.h"
#include "..\..\EmailTools\EmailTools\SendEmailTools.h"
//#include "..\..\IPTCEMS\\IPTCEMS\iptc.h"
#include "..\..\IPTC\\iptc.h"
#include "sqlapi.h"
#include "Crypto.h"

class CDBGraphicApp : public CWinApp
{
public:
	CDBGraphicApp();
	BOOL m_bUseFileSizeLimit;//是否使用文件大小限制
	int m_nFileMinSize;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CDBGraphicApp theApp;

extern int writeLog;
extern wchar_t     m_DummyPath[PATH_LENGTH1];
extern int      display;
extern wchar_t		sz_DatFile[ PATH_LENGTH1];
extern wchar_t		sz_DefaultPhoto[PATH_LENGTH];
//extern wchar_t		sz_LogFile[ PATH_LENGTH1];
extern CSQLApi  Database ;
extern CSQLApi  *p_dbCmd ;
extern wchar_t		m_szDatabase[NAME_LEN];
extern wchar_t		m_szUser[NAME_LEN];
extern BOOL	 	m_bStop;
extern BOOL	 	m_bLoadAddInfo;
extern BOOL     m_bSybase;
extern wchar_t		m_szServer[NAME_SIZE];
extern wchar_t		m_szPassword[PSWD_SIZE];
extern wchar_t		m_szLogin[NAME_SIZE];
extern BOOL		m_bDbFail;
extern wchar_t     m_szSysDB[20];
extern wchar_t     m_SmtpHost[PATH_LENGTH];
extern wchar_t     m_ToAdd[PATH_LENGTH];
extern wchar_t     m_FromAdd[PATH_LENGTH];
extern wchar_t		m_szSystemTempPath[PATH_LENGTH];
extern wchar_t		m_szSystemLogPath[PATH_LENGTH];
extern BOOL		bSupportMedia, bMediaColumnExist;
extern wchar_t     m_PassWord[PATH_LENGTH];
extern int			m_nSecure;
extern int			m_nPort;
extern CCrypto m_crypto;
