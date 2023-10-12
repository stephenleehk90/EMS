// SQLApi.cpp : implementation file
//

#include "stdafx.h"
#include "DBGraphic.h"
#include <malloc.h>
#include "DBGraphicMain.h"
#include "tempapp.h"
#include "DBGraphicSelect.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define ODBC_DBERROR					2
#define DATESOURCE_RESTORE				1
#define DATESOURER_SUCSESS				0
/*
#define FILEEXISTED						1
#define FILENOEXISTED					0
*/
#define TELEATTR_LEN					20
/*
#define FileNoContent					3
#define BmpNoContent					4
#define BmpFileNoContent				5
*/
#define ODBCSQLERROR(retcode) (((retcode) == SQL_ERROR) || ((retcode) == SQL_INVALID_HANDLE)) ? TRUE : FALSE
//NPTTYINFO npTTYInfo;
/////////////////////////////////////////////////////////////////////////////
// CSQLApi

IMPLEMENT_DYNCREATE(CSQLApi, CSQLTools)

CSQLApi::CSQLApi()
{
	//m_open=TRUE;
	//else
	//	m_open=FALSE;
	TRACE(L"\nCSQLApi CTor ");
}

CSQLApi::~CSQLApi()
{
	//m_open=FALSE;
}


BEGIN_MESSAGE_MAP(CSQLApi, CSQLTools)
	//{{AFX_MSG_MAP(CSQLApi)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
BOOL CSQLApi::DispalyDb(CListBox *pList)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}
	RETCODE nResult;                              
	wchar_t szDatabase[NAME_LEN];
	wchar_t tempList[MAX_FILESPECIAL][NAME_LEN];
	wchar_t szCmd1[50],szCmd2[50],szCmd3[50];//,szCmd4[50];
	BOOL bFill= FALSE;
	//char szCmd[COMMAND_LEN];
	nResult = ODBCAllocStmt();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT, 20);
	nResult = ODBCExecCmd("select name from master..sysdatabases where dbid>4");
	SDWORD	cbName;
	if(ODBCSQLERROR(nResult))
	{
		int nError = DecideError(FALSE);
		if(nError == DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			nResult = ODBCExecCmd("select name from master..sysdatabases where dbid>4");
			if(nResult == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCBind(1,SQL_C_CHAR, szDatabase, NAME_LEN, &cbName);
	int i=0;
	if(!ODBCSQLERROR(nResult))
	{
		while (nResult != SQL_NO_DATA_FOUND) 
		{
			nResult = ODBCFetch();
			if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
			{	
//				strncpy((char *)tempList[i], (const char *)szDatabase,NAME_LEN);
				wcsncpy(tempList[i], szDatabase,NAME_LEN);
				bFill=TRUE;
				i++;
			 } 
			else
				nResult = ODBCMoreResults();
		}
	}
	else
	{
		return FALSE;
	}
//	strcpy((char *)tempList[i],"\0");
	lstrcpy(tempList[i],L"\0");
	ODBCFreeStmt(SQL_DROP);
	if(bFill)
	{
		bFill=FALSE;
		i=0;
		while(lstrcmp(tempList[i],L"\0"))
		{
			swprintf(szCmd1,L"%s..news_agency_photo",tempList[i]);
			swprintf(szCmd2,L"%s..news_location",tempList[i]);
			swprintf(szCmd3,L"%s..wires_photo_src",tempList[i]);
			if((CheckDbteleTable(szCmd1) == SUCCESS_TRUE)
				&& (CheckDbteleTable(szCmd3) == SUCCESS_TRUE)
				&& (CheckDbteleTable(szCmd2) == SUCCESS_TRUE))
			{
				bFill=TRUE;
				pList->AddString(tempList[i]);
			}
			i++;
		}
	}
	return bFill ;
}
BOOL CSQLApi::LoginDb(BOOL bDisp)
{
	if(!UserLogin(m_szServer, m_szLogin, m_szPassword))
	{
		if(bDisp){
			//AfxMessageBox(L"數據庫連接失敗!");
			//WriteStringEx(L"數據庫連接失敗!");
			;
		}
		else
			//AfxMessageBox(L"數據庫連接失敗!");
			;
		m_bDbFail=TRUE;
		return FALSE;
	}
	else
	{
		m_bDbFail=FALSE;
		return TRUE;
	}
	
}
int CSQLApi::DecideError(BOOL bDisp, BOOL bNeedReLogin/* = FALSE*/)
{
	if(CheckDisConnect(SQL_HANDLE_STMT, m_hStmt))
	{
		if(bNeedReLogin && UserReLogin())
			return DATESOURCE_RESTORE;
		return ODBC_DBERROR;
	}
	return ODBC_DBERROR;
	/*
	char szError[SQL_MAX_MESSAGE_LENGTH];
	char szState[10];
	BOOL bError=FALSE;
	RETCODE nRetcode=ODBCFetchError(szError,szState);
	while(nRetcode!=SQL_NO_DATA_FOUND)
	{
		if(!strcmp(szState,"08001")||!strcmp(szState,"08004")||!strcmp(szState,"08S01"))
		{
			if(bDisp)
				WriteStringEx("數據源斷開");
			//delete by seesea 2003.10.10
			//end delete
		}
		nRetcode=ODBCFetchError(szError,szState);
	}
	if(bError)
	{
		RETCODE retcode=ODBCCancel();
		ODBCFreeStmt(SQL_DROP);
		ODBCDisConnect();
		if(!UserLogin(m_szServer, m_szLogin, m_szPassword))
		{
			if(bDisp)
				WriteStringEx("數據庫連接失敗!");
			else
				AfxMessageBox("數據庫連接失敗!");
			m_bDbFail=TRUE;
			return ODBC_DBERROR;
		}
		else
		{
			m_bDbFail=FALSE;
			return DATESOURCE_RESTORE;
		}
		
	}
	else
		return DATESOURER_SUCSESS;
	*/

}

en_ACCESSDBRET CSQLApi::CheckDbteleTable(wchar_t * szTable)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return ACCESSDBERROR;
	}
	RETCODE retcode;
	long lID=0;
	SDWORD	cbID ;
	ODBCAllocStmt();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
	retcode = ODBCExecCmdW(L"select object_id(N'%s')", szTable);
	if(ODBCSQLERROR(retcode))
	{
		int nError=DecideError(FALSE, TRUE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			retcode = ODBCExecCmd("select name from master..sysdatabases where dbid>4");
			if(ODBCSQLERROR(retcode))
				return ACCESSDBERROR;
		}
		else if(nError==ODBC_DBERROR)
			return ACCESSDBERROR;
	}
	ODBCBind(1, SQL_C_SLONG, &lID, 0, &cbID);
	if(!ODBCSQLERROR(retcode))
	{
		while (retcode != SQL_NO_DATA_FOUND) 
		{
			retcode = ODBCFetch();
			if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
				;
			else
				retcode = ODBCMoreResults();
		}
	}
	ODBCFreeStmt(SQL_DROP);
	if( lID )
		return SUCCESS_TRUE;
	else
		return FAILED_FALSE;
}
BOOL CSQLApi::FetchDB(wchar_t *szDB)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}
	RETCODE retcode;
	SDWORD	cbID ;
	BOOL bTrue=FALSE;
	wchar_t szDBTemp[255],szCmd1[50],szCmd2[50],szCmd3[50];//,szCmd4[50];
	retcode=ODBCAllocStmt();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
	//modified by wxy below editor5
	/*
	retcode = ODBCExecCmd("select value from master..%s where entry='database'",m_szSysDB);
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
			retcode = ODBCExecCmd("select value from master..%s where entry='database'",m_szSysDB);
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCBind(1, SQL_C_CHAR, szDBTemp, sizeof(szDBTemp), &cbID);
	while (retcode != SQL_NO_DATA_FOUND) 
	{
		retcode = ODBCFetch();
		if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
		{
			ChangeSpaceZero(szDBTemp);
			strcpy(szDB,szDBTemp);
			bTrue=TRUE;
		}
		else
			retcode = ODBCMoreResults();
	}*/

	CString str_temp = AfxGetApp()->GetProfileString(L"System", L"DATABASE", L"newsroom");
	const wchar_t* wc_tmp = str_temp;
	lstrcpy(szDB,wc_tmp);//add by wxy

	bTrue = TRUE;
	//modified by  wxy end
	lstrcpy(szDBTemp,L"");
	//modified by wxy below editor5
	//retcode = ODBCExecCmd("select value from master..%s where entry='dbserver_type'",m_szSysDB);
	retcode = ODBCExecCmd("select value from %s where entry='dbserver_type'",m_szSysDB);
	//end
	if(ODBCSQLERROR(retcode))
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			//retcode = ODBCExecCmd("select value from master..%s where entry='dbserver_type'",m_szSysDB);
			retcode = ODBCExecCmd("select value from %s where entry='dbserver_type'",m_szSysDB);
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCBind(1, SQL_C_CHAR, szDBTemp, sizeof(szDBTemp), &cbID);
	if(!ODBCSQLERROR(retcode))
	{
		while (retcode != SQL_NO_DATA_FOUND) 
		{
			retcode = ODBCFetch();
			if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
			{
				ChangeSpaceZeroW(szDBTemp);
				if ( lstrcmp ( szDBTemp , L"Sybase" ) == 0 )
					 m_bSybase = TRUE ;
				 else
					 m_bSybase = FALSE;		
				
			}
			else
				retcode = ODBCMoreResults();
		}
	}
	ODBCFreeStmt(SQL_DROP);
	if(bTrue)
	{
		swprintf(szCmd1,L"%s..news_agency_photo",szDB);
		swprintf(szCmd2,L"%s..news_location",szDB);
		swprintf(szCmd3,L"%s..wires_photo_src",szDB);
		if((CheckDbteleTable(szCmd1) != SUCCESS_TRUE)
			|| (CheckDbteleTable(szCmd3) != SUCCESS_TRUE)
			|| (CheckDbteleTable(szCmd2) != SUCCESS_TRUE))
		{
			szDB[0]='\0';
			return FALSE;
		}
	}
	else
	{
		szDB[0]='\0';	
		return FALSE;
	}
	return TRUE;
}
BOOL CSQLApi::CheckUser(wchar_t *szDataBase)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}
	RETCODE retcode ;
	long nResult=0;
	SDWORD cbResult;
	ODBCAllocStmt ();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,10);
	retcode = ODBCExecCmdW(L"select count(*) from %s..sysusers where name=N'%s' "
		,szDataBase,m_szLogin);
	if(ODBCSQLERROR(retcode))
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			retcode = ODBCExecCmdW(L"select count(*) from %s..sysusers where name=N'%s' "
					,szDataBase,m_szLogin);
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
		{
			ODBCFreeStmt(SQL_DROP);
			return FALSE;
		}
	}
	ODBCBind(1, SQL_C_SHORT, &nResult, 0, &cbResult); 
	if(!ODBCSQLERROR(retcode))
	{
		while (retcode != SQL_NO_DATA_FOUND) 
		{
			retcode = ODBCFetch();
			if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
				;
			else
				retcode = ODBCMoreResults();
		}
	}
	
	ODBCFreeStmt(SQL_DROP);
	if(nResult<=0)
		return FALSE;
	else
		return TRUE;
	
}

BOOL CSQLApi::CheckDatabase(wchar_t *szDB,BOOL bDisp)
{
	// Database: foundertxt
	if(m_bDbFail)
	{
		if(!LoginDb(bDisp))
			return ODBC_DBERROR;
	}
	RETCODE retcode=ODBCAllocStmt();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
	//retcode=ODBCExecCmd("use %s" , szDB);
	long nResult=0;
	retcode=ODBCExecCmdW(L"select count(*) from master..sysdatabases where name=N'%s'",szDB);
	SDWORD	cbName;
	if(ODBCSQLERROR(retcode))
	{
		int nError=DecideError(bDisp);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			retcode = ODBCExecCmdW(L"select count(*) from master..sysdatabases where name=N'%s'",szDB);
			if(retcode == SQL_ERROR)
				return ODBC_DBERROR;
		}
		else if(nError==ODBC_DBERROR)
			return ODBC_DBERROR;
	}
		
	ODBCBind(1,SQL_C_SLONG,&nResult,0,&cbName);
	if(!ODBCSQLERROR(retcode))
	{
		while (retcode != SQL_NO_DATA_FOUND) 
		{
			retcode = ODBCFetch();
			if (retcode == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
				;
			else
				retcode = ODBCMoreResults();
		}
	}
	
	//if(retcode!=SQL_SUCCESS)
	if(nResult<=0)
	{
		MessageBeep(MB_ICONASTERISK);
		ODBCFreeStmt(SQL_DROP);
		//SQLFreeStmt(m_hStmt,SQL_DROP);
		return FALSE;
	}
	ODBCFreeStmt(SQL_DROP);
	//retcode=SQLFreeStmt(m_hStmt,SQL_DROP);
	return TRUE;
}
BOOL CSQLApi::GetTelegraph( void )
{
	if(m_bDbFail)
	{		
		if(!LoginDb(TRUE))
		{
			WriteStringEx(L"數據庫連接失敗!");
			return FALSE;
		}
	}
	CFile	cTeleFile;
	wchar_t	szTelegraph[61];
	RETCODE nResult;
	BOOL bFill=FALSE;

	wchar_t	szFile[PATH_LENGTH];
	//edit by leon 2012-11-23 for define temporary path in INI instead of hardcode.
	//swprintf ( szFile , L"%s\\GRAPHICS.DAT" ,((CWinApp *) AfxGetApp())->GetProfileString ( L"System" , L"TemporaryPath", L"C:\\FOUNDER\\TEMP"));
	swprintf ( szFile , L"%s\\GRAPHICS.DAT" ,m_szSystemTempPath);
	//end leon 2012-11-23
	if( cTeleFile.Open( szFile, CFile::modeCreate|
			CFile::modeReadWrite|CFile::shareDenyNone, NULL) == 0 )
	{
		//WriteStringEx(AFX_IDS_NOTOPENTELE);
		AfxMessageBox( AFX_IDS_NOTOPENTELE );
		return FALSE ;
	}

	//if( CheckDatabase( "founderdata" ) == 0 )
	/*
	if( CheckDatabase( m_szDatabase,FALSE ) == 0 )
	{
	  AfxMessageBox( AFX_IDS_ERRORDB);
	  return FALSE;
	}
	*/
	//if( CheckTable( "founderdata..telegraph" ) == 0 )
	/*
	char szBufTable[80];
	sprintf ( szBufTable , "%s..news_agency_photo" , m_szDatabase );
	if( CheckDbteleTable( szBufTable ) == 0 )
	{
		 AfxMessageBox( AFX_IDS_CREATETELE);
		 return FALSE;
	}
	*/
	ODBCAllocStmt();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
	nResult=ODBCExecCmdW(L"select tele_name, tele_code,tele_id from %s..news_agency_photo",m_szDatabase);
	SDWORD	cbName;	
	if(nResult==SQL_ERROR)
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			nResult = ODBCExecCmdW(L"select tele_name, tele_code,tele_id from %s..news_agency_photo",m_szDatabase);
			if(nResult == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
		{
			cTeleFile.Close();		
			TCHAR	szError[300];
			ShowSQLError((char *)szError);
			ODBCFreeStmt(SQL_DROP);
			return FALSE;
	
		}
	}
	int nAgencyID;
	CString str;
	ODBCBind(1,SQL_C_WCHAR,szTelegraph,61,&cbName);
	ODBCBind(2,SQL_C_WCHAR,&szTelegraph[24],8,&cbName);
	ODBCBind(3,SQL_C_LONG,&nAgencyID,0,&cbName);

WORD wSignature = 0xFEFF;// use unicode encoding
cTeleFile.Write(&wSignature, 2);//Unicode的文件符?

	while (nResult != SQL_NO_DATA_FOUND) 
	{	    
		nResult = ODBCFetch();
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{	
			szTelegraph[60]='\0';
			cTeleFile.Write((LPWSTR)szTelegraph, sizeof(szTelegraph));
			str.Format(L"%d",nAgencyID);
			cTeleFile.Write(str,sizeof(int));
			bFill=TRUE;			
		} 
		else
			nResult = ODBCMoreResults();		
	}


	cTeleFile.Close();
	ODBCFreeStmt(SQL_DROP);
	//SQLFreeStmt(m_hStmt,SQL_DROP);	
    return bFill;
}
BOOL CSQLApi::CheckAgency(wchar_t* szAgency)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}
	RETCODE retcode;
	SDWORD	cbID ;
	BOOL bTrue=FALSE;
	long nCount=0;
	ODBCAllocStmt();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);

	

	retcode = ODBCExecCmdW(L"select count(*) from %s..news_agency_photo where tele_code=N'%s'",m_szDatabase,szAgency);
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			retcode = ODBCExecCmdW(L"select count(*) from %s..news_agency_photo where tele_code=N'%s'",m_szDatabase,szAgency);
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCBind(1, SQL_C_SLONG, &nCount,0, &cbID);
	while (retcode != SQL_NO_DATA_FOUND) 
	{
		retcode = ODBCFetch();
		if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
			;
		else
			retcode = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	//SQLFreeStmt(m_hStmt,SQL_DROP);
	if(nCount>0)
		bTrue=TRUE;
	return bTrue;
}

BOOL CSQLApi::CheckTablePermission( wchar_t *szTable,int nAction)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}
	RETCODE retcode ;
	long nResult=0;
	SDWORD cbResult;
	int nUserid=CheckUser();
	if(nUserid==-1)
		return FALSE;
	ODBCAllocStmt ();
	wchar_t szCmd[200];
	ChangeSpaceZeroW(szTable);
	swprintf(szCmd,L"select object_id('%s..%s')",m_szDatabase,szTable);
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
	//retcode = ODBCExecCmd(szCmd);
	retcode = ODBCExecCmdW(L"select object_id('%s..%s')",m_szDatabase,szTable);
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			//retcode = ODBCExecCmd(szCmd);
			retcode = ODBCExecCmdW(L"select object_id('%s..%s')",m_szDatabase,szTable);
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCBind(1, SQL_C_SLONG, &nResult, 0, &cbResult); 
	while (retcode != SQL_NO_DATA_FOUND) 
	{
		retcode = ODBCFetch();
		 if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
			;
		 else
			retcode = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	if(nResult==0)
	{
		return FALSE;
	}
	swprintf(szCmd,L"select count(*) from %s..sysprotects where uid=%d and id=%ld and action=%d"
		,m_szDatabase,nUserid,nResult,nAction);
	long Result=0;
	ODBCAllocStmt ();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
	//retcode = ODBCExecCmd(szCmd);
	retcode = ODBCExecCmd("select count(*) from %s..sysprotects where uid=%d and id=%ld and action=%d"
			,m_szDatabase,nUserid,nResult,nAction);
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			//retcode = ODBCExecCmd(szCmd);
			retcode = ODBCExecCmd("select count(*) from %s..sysprotects where uid=%d and id=%ld and action=%d"
				,m_szDatabase,nUserid,nResult,nAction);
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCBind(1, SQL_C_SLONG, &Result, 0, &cbResult); 
	while (retcode != SQL_NO_DATA_FOUND) 
	{
		retcode = ODBCFetch();
		 if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
			 ;
		 else
			retcode = ODBCMoreResults();
	}
	
	ODBCFreeStmt(SQL_DROP);
	if(Result>0)
		return TRUE;
	else
	return FALSE ;
	/*
	RETCODE retcode ;
	long nResult=0;
	SDWORD cbResult;
	ODBCAllocStmt ();
	retcode = ODBCExecCmd( "declare @id int "
		"select @id=uid from %s..sysusers where name=N'%s' "
		"select count(*) from %s..sysprotects where uid=@id and id=object_id(N'%s') and action=%d"
		,m_szDatabase,m_szUser,m_szDatabase,szTable,nAction);
	while (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
	{
		ODBCBind(1, SQL_C_SLONG, &nResult, 0, &cbResult); 
		retcode = ODBCFetch();
		 if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
			 ;
		 else
			retcode = ODBCMoreResults();
	}
	
	ODBCFreeStmt(SQL_DROP);
	if(nResult>0)
		return TRUE;
	else
	return FALSE ;
	  */
}
#define FILE_LEN1 250
#define TITLE_LEN1 255
BOOL CSQLApi::CreateTable( wchar_t *szTable )  
{
	//char szTable[81];
	//sprintf ( szTable , "%s..%s" , m_szFounderTele, szCode ) ;
    
    // Create Telegraph Table 
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}
    CString szTeleCode;
	RETCODE retcode;
    CString strTemp=szTable;


	CString str_temp = strTemp.Right(strTemp.GetLength()-strTemp.ReverseFind('.')-1);
	szTeleCode=str_temp;
	ODBCAllocStmt();

	ODBCPrepare("create table %s(tele_name char(%d) not null,\n",szTable,FILE_LEN1-1);			
	ODBCPrepare("tele_time datetime not null,tele_words int null,tele_title wchar_t(%d) null,tele_content text null,tele_location wchar_t(19) null,\n",TITLE_LEN1-1);	
	ODBCPrepare("tele_keyword wchar_t(19) null,tele_urgent wchar_t(1) not null,tele_flag wchar_t(1) null,tele_note wchar_t(254) null,curruser wchar_t(%d) null)\n",NAME_LEN-1);
	retcode=ODBCCatExec();
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			ODBCPrepare("create table %s(tele_name char(%d) not null,\n",szTable,FILE_LEN1-1);			
			ODBCPrepare("tele_time datetime not null,tele_words int null,tele_title wchar_t(%d) null,tele_content text null,tele_location wchar_t(19) null,\n",TITLE_LEN1-1);	
			ODBCPrepare("tele_keyword wchar_t(19) null,tele_urgent wchar_t(1) not null,tele_flag wchar_t(1) null,tele_note wchar_t(254) null,curruser wchar_t(%d) null)\n",NAME_LEN-1);
			retcode = ODBCCatExec();
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCFreeStmt(SQL_DROP);
	ODBCAllocStmt();
	ODBCPrepare("alter table %s..%s Add CONSTRAINT PK_TName_Date%s PRIMARY KEY NONCLUSTERED (tele_name)\n",m_szDatabase,szTeleCode,szTeleCode);
	//ODBCPrepare("
	retcode=ODBCCatExec();
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			ODBCPrepare("alter table %s..%s Add CONSTRAINT PK_TName_Date%s PRIMARY KEY NONCLUSTERED (tele_name)\n",m_szDatabase,szTeleCode,szTeleCode);
			retcode = ODBCCatExec();
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCFreeStmt(SQL_DROP);
	ODBCAllocStmt();
	ODBCPrepare("create index tlocation_ind%s on %s(tele_location)\n",szTeleCode,szTable);
	retcode=ODBCCatExec();
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			ODBCPrepare("create index tlocation_ind%s on %s(tele_location)\n",szTeleCode,szTable);
			retcode = ODBCCatExec();
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCFreeStmt(SQL_DROP);
	ODBCAllocStmt();
	ODBCPrepare("create index tdate_ind%s on %s(tele_time)\n",szTeleCode,szTable);
	retcode=ODBCCatExec();
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			ODBCPrepare("create index tdate_ind%s on %s(tele_time)\n",szTeleCode,szTable);
			retcode = ODBCCatExec();
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCFreeStmt(SQL_DROP);
	ODBCAllocStmt();
	ODBCPrepare("create index tkeyword_ind%s on %s(tele_keyword)\n",szTeleCode,szTable);
	retcode=ODBCCatExec();
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			ODBCPrepare("create index tkeyword_ind%s on %s(tele_keyword)\n",szTeleCode,szTable);
			retcode=ODBCCatExec();
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCFreeStmt(SQL_DROP);
	if(retcode==SQL_ERROR)
		return FALSE;
		return TRUE;
}

int CSQLApi::CheckUser()
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return -1;
	}
	RETCODE retcode ;
	int nResult=0;
	SDWORD cbResult;
	ODBCAllocStmt ();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
	retcode = ODBCExecCmdW(L"select gid from %s..sysusers where name=N'%s' "
		,m_szDatabase,m_szUser);
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(TRUE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			retcode = ODBCExecCmdW(L"select gid from %s..sysusers where name=N'%s' "
					,m_szDatabase,m_szUser);
			if(retcode == SQL_ERROR)
				return -1;
		}
		else if(nError==ODBC_DBERROR)
			return -1;
	}
	ODBCBind(1, SQL_C_SHORT, &nResult, 0, &cbResult); 
	while (retcode != SQL_NO_DATA_FOUND) 
	{
		retcode = ODBCFetch();
		 if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
			 ;
		 else
			retcode = ODBCMoreResults();
	}
	
	ODBCFreeStmt(SQL_DROP);
	return nResult;
	
}


//en_ACCESSDBRET CSQLApi::InsertValues ( PhotoInfo *pInfo, wchar_t *szTeleCode,int nAgencyID,int nAgencyFlag
//		, wchar_t *szBmpFile , wchar_t *szFile , wchar_t *szMidPath, wchar_t *szSmallPath
//		, int nDelayHour,int nMin, wchar_t *szJpgFile,int nMarkLine,int nTitleLine
//		,BOOL bTransfer,int nSize,wchar_t * szLastTime,wchar_t *szSourthpath,wchar_t *szPhotoTime,int addday, wchar_t * szTime, wchar_t *szServerFile)
en_ACCESSDBRET CSQLApi::InsertValues ( PhotoInfo *pInfo, CString szTeleCode,int nAgencyID,int nAgencyFlag
	,CString szBmpFile ,CString szFile , CString szMidPath, CString szSmallPath
	, int nDelayHour,int nMin, CString szJpgFile,int nMarkLine,int nTitleLine
	,BOOL bTransfer,int nSize,CString szLastTime,CString szSourthpath,CString szPhotoTime,int addday, CString szTime, CString szServerFile)

{
//	AfxMessageBox(L"szServerFile");
//	AfxMessageBox(szServerFile);
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return ACCESSDBERROR;
	}
	long lFlag=0L;     //950811
	CString szTitle=L"";
	wchar_t szMarkTitle[TITLE_LEN+5120]=L"";
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];
	wchar_t szTeleAttrL[TELEATTR_LEN]=L"";
	long  lDelay=0L,lWord=0L;
	en_ACCESSDBRET enReturn = FAILED_FALSE;
	if(szTime==L"") szTime=L"";
	CFile fil ;
	DWORD dwLength;
	HGLOBAL hglbFil;
	CString strBuffer,curTime;
	LPBYTE hpBuffer ;
	nDelayHour=-nDelayHour;
	nMin=-nMin;
	CTimeSpan ts;
	CTime t=0,t1=0,t2=0;
	BOOL bTable=TRUE;
	BOOL bHasLength=TRUE;
	RETCODE bResult,retcode;
	wchar_t szCmd[COMMAND_LEN+COMMAND_LEN];
	SDWORD cbData;

	bool isUnicode = false; 
	
	int nFileTable=CheckFileInTable (szTeleCode,szJpgFile ,szLastTime);

	//test
	if(nFileTable == ACCESSDBERROR)
	{
		WriteStringEx(L"error:ODBC ERROR");
		return ACCESSDBERROR;
	}
	else if(nFileTable==FILEEXISTED)
	{
		WriteStringEx(L"error:FILE EXISTED");
		return TEXTFAIL;	
	}
//	_splitpath ( (const char *)szFile , (char *)drive , (char *)dir , (char *)fname , (char *)ext );
	_wsplitpath ( szFile , drive , dir , fname , ext );
	if ( fil.Open ( szFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone ) == 0 )
	{
		strBuffer.LoadString(AFX_IDS_OPENNOTORG);
		CString str_temp = strBuffer;
		const wchar_t* wc_tmp = str_temp;
		WriteStringEx(wc_tmp);
		return TEXTFAIL;
	}

	fil.SeekToBegin();
	dwLength = fil.GetLength();
	if(dwLength == 0)
	{
		fil.Close();
		bHasLength=FALSE;
		bResult = SUCCESS_TRUE;
	}
	else
	{
		bHasLength=TRUE;

		byte head[2];
		fil.Read(head,2);
		if((head[0]==0xff&&head[1]==0xfe) || (head[0]==0xfe&&head[1]==0xff))
		{
		   isUnicode = true;
		}
		else
			fil.SeekToBegin();

		if ( (hglbFil = GlobalAlloc ( GHND , dwLength + 30) ) == NULL )
		{
			//strBuffer.LoadString(AFX_IDS_NOTMEMORY);

			//CString str_temp = strBuffer;
			//const wchar_t* wc_tmp = str_temp;
			//WriteStringEx(wc_tmp);
			fil.Close() ;
			return TEXTFAIL;
		}
		 hpBuffer = ( LPBYTE) GlobalLock ( hglbFil ) ;
		 //edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
		 // fil.ReadHuge ( hpBuffer  , dwLength ) ;
		 fil.Read ( hpBuffer  , dwLength ) ;
		 hpBuffer[dwLength] = '\0';
		 fil.Close();
		 if ( bTransfer )
		 {	
			
	 		if(!tcf.Open(L"Gb_big5.cod"))
			{
				wchar_t szFile[PATH_LENGTH];
				//edit by leon 2012-11-23 for define temporary path in INI instead of hardcode.
				//swprintf ( szFile , L"%s\\Gb_big5.COD" ,((CWinApp *) AfxGetApp())->GetProfileString ( L"System" , L"TemporaryPath", L"C:\\FOUNDER\\TEMP"));
				swprintf ( szFile , L"%s\\Gb_big5.COD" ,m_szSystemTempPath);
				//end leon 2012-11-23
				if(!tcf.Open(szFile))
				{
					MessageBeep( MB_ICONHAND ) ;
					strBuffer.LoadString(AFX_IDS_BIG5 );

					CString str_temp = strBuffer;
					const wchar_t* wc_tmp = str_temp;
					WriteStringEx(wc_tmp);

					GlobalUnlock ( hglbFil ) ;
					_heapmin();
					GlobalFree ( hglbFil ) ;
					return TEXTFAIL;
				}
				else
					tcf.TransformCode(hpBuffer,dwLength);

			 } //
			 else
				tcf.TransformCode(hpBuffer,dwLength);
			lWord=0;
			lDelay=0;
			szTitle=GetTeleTitleWordW ( hpBuffer , dwLength , nTitleLine ,  &lWord , &lDelay) ;
		 }
		 else
		 {
			/*if (isUnicode)
				szTitle= GetTeleTitleWordW ( hpBuffer , dwLength , nTitleLine ,  &lWord , &lDelay) ;
			else
			{
				CHAR *pszAnsi = new CHAR[dwLength];
				strcpy(pszAnsi, GetTeleTitleWord ( hpBuffer , dwLength , nTitleLine ,  &lWord , &lDelay));				
				WCHAR * szwBuffer = new WCHAR[dwLength*2];

				MultiByteToWideChar(CP_ACP, 0, pszAnsi, -1, szwBuffer, dwLength*2+1);
				szTitle=szwBuffer ;

				delete pszAnsi;
				delete szwBuffer;
			}*/
			 szTitle = GetTeleTitle( szFile, nTitleLine);
			 lWord = dwLength;
			
		 }
		 
		 if(nFileTable==FILENOEXISTED)
		 {
			lstrcpy ( szMarkTitle , GetTitleWord ( hpBuffer , dwLength , nMarkLine ,  NULL )) ;
			if(lstrlen(szMarkTitle)>254) szMarkTitle[254]=0;
			ArrangeTitle ( szMarkTitle) ;
			if(lWord <= 0)
			{
				GlobalUnlock ( hglbFil ) ;
				_heapmin();
				GlobalFree ( hglbFil ) ;
				WriteStringEx(L"error:lWord <= 0");
				return TEXTFAIL;
			}
			TransStringToDB ( szMarkTitle ) ;
			CString strTemp;
			if(szMarkTitle[0]!='\0')
			{
				strTemp=szMarkTitle;
				strTemp.MakeUpper();

				const wchar_t* wc_tmp = strTemp;
				lstrcpy(szMarkTitle,wc_tmp);
				if(GetLib1(szMarkTitle,szTeleAttrL)==ODBC_DBERROR)
				{
					GlobalUnlock ( hglbFil ) ;
					GlobalFree ( hglbFil ) ;
					WriteStringEx(L"error:if(GetLib1(szMarkTitle,szTeleAttrL)==ODBC_DBERROR)");
					return TEXTFAIL;
				}
					
			}
			strTemp=szServerFile;
//			char szExt[256]="";
			wchar_t szExt[256]=L"";
			
			int nFind=strTemp.ReverseFind('.');
			if(nFind!=-1)
			{
				CString str_temp = strTemp.Mid(nFind+1);
				const wchar_t* wc_tmp = str_temp;
				lstrcpy(szExt,wc_tmp);
			}
			else
				szExt[0]=0;
			szSourthpath.Replace(L"%",L"%%");
			szMidPath.Replace(L"%",L"%%");
			szSmallPath.Replace(L"%",L"%%");
			szServerFile.Replace(L"%",L"%%");
			szSourthpath.Replace(L"'",L"''");
			szMidPath.Replace(L"'",L"''");
			szSmallPath.Replace(L"'",L"''");
			szServerFile.Replace(L"'",L"''");
			//char szCmd[COMMAND_LEN];
			cbData=(SDWORD) dwLength;
			t=CTime::GetCurrentTime();
			t2=t;
			curTime.Format(L"%4d-%2d-%2d %2d:%2d:%2d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
			ODBCAllocStmt();

			//ODBCExecCmdW(L"delete from %s..wires_photo_src where photo_src_path = N'%s'and year(photo_time) = year('%s')"
			//	L"and month(photo_time) = month('%s') and day(photo_time) = day('%s') and small_path = N'DummyPath'", m_szDatabase, szSourthpath, szPhotoTime, szPhotoTime, szPhotoTime);

			
			en_ACCESSDBRET ret = IsDummyPhoto(szSourthpath, szJpgFile);
			if(ret == SUCCESS_TRUE)
			{
				swprintf(szCmd, L"update %s..wires_photo_src set photo_time = N'%s',import_time = N'%s',photo_title = N'%s',photo_size = %d,photo_format = N'%s ',"
					L"photo_location = N'%s ',photo_flag = 'N',photo_begin_time = N'%s',agency_id = %d,agency_flag = %d,photo_path = N'%s',photo_height = %d,"
					L"photo_width = %d,bits_pixel = %d,mid_path = N'%s',small_path = N'%s',last_modify_time = N'%s',real_time = N'%s' where photo_src_path = N'%s' and small_path = N'DummyPath'"
					,m_szDatabase,szPhotoTime,szPhotoTime,szTitle,nSize,szExt,szTeleAttrL,szTime,nAgencyID,nAgencyFlag,szServerFile,pInfo->nHeight,pInfo->nWidth,pInfo->nBits,szMidPath,szSmallPath,szLastTime,curTime,szSourthpath);
			}
			else if(ret == ACCESSDBERROR)
			{
				WriteStringEx(L"error:ODBC ERROR");
				return ACCESSDBERROR;
			}
			else
				swprintf(szCmd, L"Insert into %s..wires_photo_src(tele_code,photo_name,photo_time,import_time,photo_title,photo_size,photo_format,"
					L"photo_location,photo_flag,photo_begin_time,agency_id,agency_flag,photo_path,photo_height,photo_width,bits_pixel,mid_path,small_path,photo_src_path,last_modify_time,real_time) "
					L" values(N'%s', N'%s',N'%s',N'%s', N'%s ',%d,N'%s ',N'%s ','N',N'%s',%d,%d,N'%s',%d,%d,%d,N'%s',N'%s',N'%s',N'%s',N'%s')"			
					,m_szDatabase,szTeleCode,szJpgFile,szPhotoTime,szPhotoTime,szTitle,nSize,szExt,szTeleAttrL,szTime,nAgencyID,nAgencyFlag,szServerFile,pInfo->nHeight,pInfo->nWidth,pInfo->nBits,szMidPath,szSmallPath,szSourthpath,szLastTime,curTime);
			bResult=SingleCmd(szCmd);
			/*bResult=ODBCExecCmdW( L"Insert into %s..wires_photo_src(tele_code,photo_name,photo_time,import_time,photo_title,photo_size,photo_format,"
				L"photo_location,photo_flag,photo_begin_time,agency_id,agency_flag,photo_path,photo_height,photo_width,bits_pixel,mid_path,small_path,photo_src_path,last_modify_time,real_time) "
				L" values(N'%s', N'%s',N'%s',N'%s', N'%s ',%d,N'%s ',N'%s ','N',N'%s',%d,%d,N'%s',%d,%d,%d,N'%s',N'%s',N'%s',N'%s',N'%s')"			
				,m_szDatabase,szTeleCode,szJpgFile,szPhotoTime,szPhotoTime,szTitle,nSize,szExt,szTeleAttrL,szTime,nAgencyID,nAgencyFlag,szServerFile,pInfo->nHeight,pInfo->nWidth,pInfo->nBits,szMidPath,szSmallPath,szSourthpath,szLastTime,curTime);*/

		/*	bResult=ODBCExecCmdW( L"Insert into %s..wires_photo_src(tele_code,photo_name,photo_time,import_time,photo_title,photo_size,photo_format,"
  				L"photo_location,photo_flag,photo_begin_time,agency_id,agency_flag,photo_path,photo_height,photo_width,bits_pixel,mid_path,small_path,photo_src_path,last_modify_time,real_time) "
 				L" values(N'%s', N'%s',CONVERT(Varchar(64),N'%s', 64),CONVERT(Varchar(64),N'%s', 64), N'%s ',%d,N'%s ',N'%s ','N',N'%s',%d,%d,N'%s',%d,%d,%d,N'%s',N'%s',N'%s',N'%s',N'%s')"			
				,m_szDatabase,szTeleCode,szJpgFile,szPhotoTime,szPhotoTime,szTitle,nSize,szExt,szTeleAttrL,szTime,nAgencyID,nAgencyFlag,szServerFile,pInfo->nHeight,pInfo->nWidth,pInfo->nBits,szMidPath,szSmallPath,szSourthpath,szLastTime,curTime);
			CString str;
			str.Format(L"Insert into %s..wires_photo_src(tele_code,photo_name,photo_time,import_time,photo_title,photo_size,photo_format,"
				L"photo_location,photo_flag,photo_begin_time,agency_id,agency_flag,photo_path,photo_height,photo_width,bits_pixel,mid_path,small_path,photo_src_path,last_modify_time,real_time) "
				L" values(N'%s', N'%s',CONVERT(Varchar(64),N'%s', 64),CONVERT(Varchar(64),N'%s', 64), N'%s ',%d,N'%s ',N'%s ','N',N'%s',%d,%d,N'%s',%d,%d,%d,N'%s',N'%s',N'%s',N'%s',N'%s')"			
				,m_szDatabase,szTeleCode,szJpgFile,szPhotoTime,szPhotoTime,szTitle,nSize,szExt,szTeleAttrL,szTime,nAgencyID,nAgencyFlag,szServerFile,pInfo->nHeight,pInfo->nWidth,pInfo->nBits,szMidPath,szSmallPath,szSourthpath,szLastTime,curTime);
*/
			if(ODBCSQLERROR(bResult))
			{
				int nError=DecideError(FALSE);
				if(nError==DATESOURCE_RESTORE)
				{	
					ODBCAllocStmt();
					swprintf(szCmd,L"Insert into %s..wires_photo_src(tele_code,photo_name,photo_time,import_time,photo_title,photo_size,photo_format,"
						L"photo_location,photo_flag,photo_begin_time,agency_id,agency_flag,photo_path,photo_height,photo_width,bits_pixel,mid_path,small_path,photo_src_path,last_modify_time,real_time) "
						L" values(N'%s', N'%s',CONVERT(Varchar(64),N'%s', 64),CONVERT(Varchar(64),N'%s', 64), N'%s ',%d,N'%s ',N'%s ','N',N'%s',%d,%d,N'%s',%d,%d,%d,N'%s',N'%s,N'%s',N'%s,N'%s'')"
						,m_szDatabase,szTeleCode,szJpgFile,szPhotoTime,szPhotoTime,szTitle,nSize,szExt,szTeleAttrL,szTime,nAgencyID,nAgencyFlag,szServerFile,pInfo->nHeight,pInfo->nWidth,pInfo->nBits,szMidPath,szSmallPath,szSourthpath,szLastTime,curTime);
					bResult=SingleCmd(szCmd);
					/*bResult=ODBCExecCmdW( L"Insert into %s..wires_photo_src(tele_code,photo_name,photo_time,import_time,photo_title,photo_size,photo_format,"
						L"photo_location,photo_flag,photo_begin_time,agency_id,agency_flag,photo_path,photo_height,photo_width,bits_pixel,mid_path,small_path,photo_src_path,last_modify_time,real_time) "
						L" values(N'%s', N'%s',CONVERT(Varchar(64),N'%s', 64),CONVERT(Varchar(64),N'%s', 64), N'%s ',%d,N'%s ',N'%s ','N',N'%s',%d,%d,N'%s',%d,%d,%d,N'%s',N'%s,N'%s',N'%s,N'%s'')"
						,m_szDatabase,szTeleCode,szJpgFile,szPhotoTime,szPhotoTime,szTitle,nSize,szExt,szTeleAttrL,szTime,nAgencyID,nAgencyFlag,szServerFile,pInfo->nHeight,pInfo->nWidth,pInfo->nBits,szMidPath,szSmallPath,szSourthpath,szLastTime,curTime);*/
					if(ODBCSQLERROR(bResult))
					{
						GlobalUnlock ( hglbFil ) ;
						_heapmin();
						GlobalFree ( hglbFil ) ;
						return ACCESSDBERROR;
					}
					
				}
				else if(nError == ODBC_DBERROR)
				{
					GlobalUnlock ( hglbFil ) ;
					_heapmin();
					GlobalFree ( hglbFil ) ;
					WriteStringEx(L"error:else if(nError==ODBC_DBERROR)");
					return ACCESSDBERROR;
				}
			}
			ODBCFreeStmt(SQL_DROP);
			t1=CTime::GetCurrentTime();
		 }
		 else
			bResult=SUCCESS_TRUE;
	}
	
	szSourthpath.Replace(L"%%",L"%");
	szMidPath.Replace(L"%%",L"%");
	szSmallPath.Replace(L"%%",L"%");
	szServerFile.Replace(L"%%",L"%");
	szSourthpath.Replace(L"''",L"'");
	szMidPath.Replace(L"''",L"'");
	szSmallPath.Replace(L"''",L"'");
	szServerFile.Replace(L"''",L"'");
	if ( bResult== SQL_SUCCESS || bResult== SQL_SUCCESS_WITH_INFO )
	{
		ODBCAllocStmt();
		ts=t1-t;
		int nMinDepart=ts.GetTotalSeconds()/60;
		t1=0;t=0;
		t=CTime::GetCurrentTime();
		if(t2==0)
			t2=t;
		if(nFileTable==BmpFileNoContent||nFileTable==FILENOEXISTED)
		{
			//char szCmd[1024];
// 			swprintf(szCmd,L"update %s..wires_photo_src set photo_caption=NULL,photo_bitmap=NULL where photo_name=N'%s' "/*photo_note=NULL,*/
// 				L"and tele_code=N'%s' and datepart ( day , photo_time) = datepart( day , dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				L"and photo_name = N'%s' and datepart (month , photo_time) = datepart(month, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				L"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				,m_szDatabase,szJpgFile,szTeleCode , nMin-nMinDepart,nDelayHour,addday,szJpgFile,nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday);
// 		
			if(bSupportMedia == TRUE&&bMediaColumnExist==TRUE)
				swprintf(szCmd,L"update %s..wires_photo_src set photo_caption=NULL,photo_bitmap=NULL, media_type = 0 where photo_name=N'%s' "/*photo_note=NULL,*/
				L"and tele_code=N'%s' and last_modify_time=N'%s' and photo_time=N'%s'"
				,m_szDatabase,szJpgFile,szTeleCode ,szLastTime,szPhotoTime);
			else
			swprintf(szCmd,L"update %s..wires_photo_src set photo_caption=NULL,photo_bitmap=NULL where photo_name=N'%s' "/*photo_note=NULL,*/
				L"and tele_code=N'%s' and last_modify_time=N'%s' and photo_time=N'%s'"
				,m_szDatabase,szJpgFile,szTeleCode ,szLastTime,szPhotoTime);

			retcode=ODBCExecCmdW(L"update %s..wires_photo_src set photo_caption=NULL,photo_bitmap=NULL where photo_name=N'%s' "/*photo_note=NULL,*/
				L"and tele_code=N'%s' and last_modify_time=N'%s'and photo_time=N'%s'"
				,m_szDatabase,szJpgFile,szTeleCode ,szLastTime,szPhotoTime);				
// 				ODBCExecCmdW(L"update %s..wires_photo_src set photo_caption=NULL,photo_bitmap=NULL where photo_name=N'%s' "/*photo_note=NULL,*/
// 				L"and tele_code=N'%s' and datepart ( day , photo_time) = datepart( day , dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				L"and photo_name = N'%s' and datepart (month , photo_time) = datepart(month, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				L"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				,m_szDatabase,szJpgFile,szTeleCode , nMin-nMinDepart,nDelayHour,addday,szJpgFile,nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday);
// 		
			if(ODBCSQLERROR(retcode))
			{
				int nError=DecideError(FALSE);
				if(nError==DATESOURCE_RESTORE)
				{
					ODBCAllocStmt();
					t1=CTime::GetCurrentTime();
					ts=t1-t;
					nMinDepart=ts.GetTotalSeconds()/60;
					
					retcode=ODBCExecCmdW(L"update %s..wires_photo_src set photo_caption=NULL,photo_bitmap=NULL where photo_name=N'%s' "/*photo_note=NULL,*/
						L"and tele_code=N'%s' and last_modify_time=N'%s' and photo_time=N'%s'"
						,m_szDatabase,szJpgFile,szTeleCode ,szLastTime,szPhotoTime);
// 					retcode=ODBCExecCmdW(L"update %s..wires_photo_src set photo_caption=NULL,photo_bitmap=NULL where photo_name=N'%s' "/*photo_note=NULL,*/
// 					L"and tele_code=N'%s' and datepart ( day , photo_time) = datepart( day , dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					L"and photo_name = N'%s' and datepart (month , photo_time) = datepart(month, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					L"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					,m_szDatabase,szJpgFile,szTeleCode , nMin-nMinDepart,nDelayHour,addday,szJpgFile,nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday);
					if(ODBCSQLERROR(retcode))
					{
						GlobalUnlock ( hglbFil ) ;
						_heapmin();
						GlobalFree ( hglbFil ) ;
						return ACCESSDBERROR;
					}
				}
				else if(nError==ODBC_DBERROR)
				{
					GlobalUnlock ( hglbFil ) ;
					_heapmin();
					GlobalFree ( hglbFil ) ;
					WriteStringEx(L"error:update:else if(nError==ODBC_DBERROR)");
					return ACCESSDBERROR;
				}
			}
			ODBCFreeStmt(SQL_DROP);
		}
		else if(nFileTable==FileNoContent)
		{
			
			retcode=ODBCExecCmdW(L"update %s..wires_photo_src set photo_caption=NULL where photo_name=N'%s' "/*photo_note=NULL,*/
				L"and tele_code=N'%s' and last_modify_time=N'%s' and photo_time=N'%s'"
				,m_szDatabase,szJpgFile,szTeleCode ,szLastTime, szPhotoTime);

// 			retcode=ODBCExecCmdW(L"update %s..wires_photo_src set photo_caption=NULL where photo_name=N'%s' "/*photo_note=NULL*/
// 				L"and tele_code=N'%s' and datepart ( day , photo_time) = datepart( day , dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				L"and photo_name = N'%s' and datepart (month , photo_time) = datepart(month, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				L"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				,m_szDatabase,szJpgFile,szTeleCode , nMin-nMinDepart,nDelayHour,addday,szJpgFile,nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday);
 			if(ODBCSQLERROR(retcode))
			{
				int nError=DecideError(FALSE);
				if(nError==DATESOURCE_RESTORE)
				{
					ODBCAllocStmt();
					t1=CTime::GetCurrentTime();
					ts=t1-t;
					nMinDepart=ts.GetTotalSeconds()/60;
					
					retcode=ODBCExecCmdW(L"update %s..wires_photo_src set photo_caption=NULL where photo_name=N'%s' "/*photo_note=NULL,*/
						L"and tele_code=N'%s' and last_modify_time=N'%s' and photo_time=N'%s'"
					,m_szDatabase,szJpgFile,szTeleCode ,szLastTime, szPhotoTime);

// 					retcode=ODBCExecCmdW(L"update %s..wires_photo_src set photo_caption=NULL where photo_name=N'%s' "//,photo_note=NULL
// 					L"and tele_code=N'%s' and datepart ( day , photo_time) = datepart( day ,dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					L"and photo_name = N'%s' and datepart (month , photo_time) = datepart(month, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					L"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					,m_szDatabase,szJpgFile,szTeleCode , nMin-nMinDepart,nDelayHour,addday,szJpgFile,nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday);
 					if(ODBCSQLERROR(retcode))
					{
						GlobalUnlock ( hglbFil ) ;
						_heapmin();
						GlobalFree ( hglbFil ) ;
						return ACCESSDBERROR;
					}
				}
				else if(nError==ODBC_DBERROR)
				{
					GlobalUnlock ( hglbFil ) ;
					_heapmin();
					GlobalFree ( hglbFil ) ;
					WriteStringEx(L"error:update2:else if(nError==ODBC_DBERROR)");
					return ACCESSDBERROR;
				}
			}
			ODBCFreeStmt(SQL_DROP);
		}
		else if(nFileTable==BmpNoContent)
		{
			
			retcode=ODBCExecCmdW(L"update %s..wires_photo_src set photo_bitmap=NULL where photo_name=N'%s' "/*photo_note=NULL,*/
				L"and tele_code=N'%s' and last_modify_time=N'%s' and photo_time=N'%s'"
				,m_szDatabase,szJpgFile,szTeleCode ,szLastTime,szPhotoTime);
// 			retcode=ODBCExecCmdW(L"update %s..wires_photo_src set photo_bitmap=NULL where photo_name=N'%s' "//photo_note=NULL
// 				L"and tele_code=N'%s' and datepart ( day , photo_time) = datepart( day , dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				L"and photo_name = N'%s' and datepart (month , photo_time) = datepart(month, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				L"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 				,m_szDatabase,szJpgFile,szTeleCode , nMin-nMinDepart,nDelayHour,addday,szJpgFile,nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday);
 			if(ODBCSQLERROR(retcode))
			{
				int nError=DecideError(TRUE);
				if(nError==DATESOURCE_RESTORE)
				{
					ODBCAllocStmt();
					t1=CTime::GetCurrentTime();
					ts=t1-t;
					nMinDepart=ts.GetTotalSeconds()/60;
					retcode=ODBCExecCmdW(L"update %s..wires_photo_src set photo_bitmap=NULL where photo_name=N'%s' "/*photo_note=NULL,*/
					L"and tele_code=N'%s' and last_modify_time=N'%s'and photo_time=N'%s'"
					,m_szDatabase,szJpgFile,szTeleCode ,szLastTime,szPhotoTime);
// 					retcode=ODBCExecCmdW(L"update %s..wires_photo_src set photo_bitmap=NULL where photo_name=N'%s' "//,photo_note=NULL 
// 					L"and tele_code=N'%s' and datepart ( day , photo_time) = datepart( day , dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					L"and photo_name = N'%s' and datepart (month , photo_time) = datepart(month, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					L"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					,m_szDatabase,szJpgFile,szTeleCode , nMin-nMinDepart,nDelayHour,addday,szJpgFile,nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday);
					if(ODBCSQLERROR(retcode))
					{
						GlobalUnlock ( hglbFil ) ;
						_heapmin();
						GlobalFree ( hglbFil ) ;
						return ACCESSDBERROR;
					}
				}
				else if(nError==ODBC_DBERROR)
				{
					GlobalUnlock ( hglbFil ) ;
					_heapmin();
					GlobalFree ( hglbFil ) ;
					WriteStringEx(L"error:update3:else if(nError==ODBC_DBERROR)");
					return ACCESSDBERROR;
				}
			}
			ODBCFreeStmt(SQL_DROP);
		}
	}
	if(retcode== SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO )
	{
		enReturn = SUCCESS_TRUE;
		if(bHasLength)
		{
			if(nFileTable==BmpFileNoContent||nFileTable==FILENOEXISTED||nFileTable==FileNoContent)
			{
				t1=CTime::GetCurrentTime();
				ts=t1-t;
				cbData=(SDWORD) dwLength;
				int nMinDepart=ts.GetTotalSeconds()/60;
				/*sprintf ( szCmd , "update %s..wires_photo_src set photo_caption=? where photo_name=N'%s' "
					"and tele_code=N'%s' and datepart ( day , photo_time) = datepart( day ,dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
					"and datepart (month , photo_time) = datepart(month, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
					"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
					,m_szDatabase,szJpgFile,szTeleCode , nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday);*/
			  // AfxMessageBox((CString)dwLength);
				CString str_temp = PrepareSQL(L"update %s..wires_photo_src set photo_caption=? where photo_name=N'%s' "/*photo_note=NULL,*/
					L"and tele_code=N'%s' and last_modify_time=N'%s' and photo_time=N'%s'"
				,m_szDatabase,szJpgFile,szTeleCode ,szLastTime ,szPhotoTime);
// 
// 				CString str_temp = PrepareSQL(L"update %s..wires_photo_src set photo_caption=? where photo_name=N'%s' "
// 					L"and tele_code=N'%s' and datepart ( day , photo_time) = datepart( day ,dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					L"and datepart (month , photo_time) = datepart(month, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					L"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
// 					,m_szDatabase,szJpgFile,szTeleCode , nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday);
 				const wchar_t* wc_tmp = str_temp;
			
				swprintf ( szCmd, wc_tmp);
				//sprintf ( szCmd , "update %s set tele_content=? where tele_name='%s%s'" ,szTable , fname , ext ); 
				UpdateTextItem1(szCmd,hpBuffer,cbData,szSourthpath,(GetFieldType(L"wires_photo_src",L"photo_caption")==35), isUnicode);
				
				if(1)
				{
					enReturn = SUCCESS_TRUE;
				}
				else
				{
					enReturn = FAILED_FALSE;
					WriteStringEx(L"error1:bReturn=FALSE;");
				}

			}
		}
	}
	else
	{
		enReturn = FAILED_FALSE;
		WriteStringEx(L"error2:bReturn=FALSE;");
	}
	GlobalUnlock ( hglbFil ) ;
	GlobalFree ( hglbFil ) ;
	_heapmin();
	if(enReturn == FAILED_FALSE)
	{
		WriteStringEx(L"error:if(!bReturn)");
		return TEXTFAIL;
	}
	enReturn = SUCCESS_TRUE;
	if(nFileTable==BmpFileNoContent||nFileTable==FILENOEXISTED||nFileTable==BmpNoContent)
	{
		//comment by wxy below
		//小圖不再保存到數據庫中﹐而是放到文件服務器
		/*if ( fil.Open ( szBmpFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone ) == 0 )
		{
			strBuffer.LoadString(AFX_IDS_OPENNOTORG);
			WriteStringEx(strBuffer);
			WriteStringEx("error:if ( fil.Open ( szBmpFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone ) == 0 )");
			return BMPFAIL;
		}
		fil.SeekToBegin();
		dwLength = fil.GetLength();
		if(dwLength == 0)
		{
			fil.Close();
			WriteStringEx("ERROR:if(dwLength == 0)");
			return BMPFAIL;
		}
		else
		{
			if ( (hglbFil = GlobalAlloc ( GHND , dwLength + 30) ) == NULL )
			{
				strBuffer.LoadString(AFX_IDS_NOTMEMORY);
				WriteStringEx(strBuffer);
				fil.Close() ;
				WriteStringEx("ERROR:GLOBALLock FAIL");
				return BMPFAIL;
			}
			 hpBuffer = ( LPBYTE) GlobalLock ( hglbFil ) ;
			 fil.ReadHuge ( hpBuffer  , dwLength ) ;
			 hpBuffer[dwLength] = '\0';
			 fil.Close();
			 cbData=(SDWORD) dwLength;
			 t1=CTime::GetCurrentTime();
			ts=t1-t;
			int nMinDepart=ts.GetTotalSeconds()/60;
			sprintf ( szCmd , "update %s..wires_photo_src set photo_bitmap=? where photo_name=N'%s' "
				"and tele_code=N'%s' and datepart ( day , photo_time) = datepart( day , dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
				"and datepart (month , photo_time) = datepart(month, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
				"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,dateadd(day,%d,getdate())))) "
				,m_szDatabase,szJpgFile,szTeleCode , nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday,nMin-nMinDepart,nDelayHour,addday);
			//sprintf ( szCmd , "update %s set tele_content=? where tele_name='%s%s'" ,szTable , fname , ext ); 
			if(UpdateTextItem1(szCmd  ,hpBuffer , cbData ))
				bReturn=TRUE;
			else
			{
				bReturn=FALSE;
			}
		}*/
		//comment by wxy end
		//add by wxy below
		CString strServerFile;
		strServerFile = szSmallPath;
		int nFind = strServerFile.ReverseFind('\\');
		if(nFind == -1)
		{
			strBuffer = L"file path error:" + strServerFile;
			const wchar_t* wc_tmp = strBuffer;
			WriteStringEx(wc_tmp);
		}
		BOOL bIsFrh;
		//bIsFrh=IsFreeHandPic(strServerFile);
		bIsFrh=IsSupportedPic(strServerFile);
		if(bIsFrh)
		{
			CString FileName = strServerFile.Right(strServerFile.GetLength()-nFind);
			strServerFile=strServerFile.Left(nFind+1);
			CreateDirectory(strServerFile,NULL);
			strServerFile +=FileName;
		
			
			if(CopyFile( szBmpFile,strServerFile,FALSE) == FALSE)
			{
				LPVOID lpMsgBuf;
				DWORD dwBufferSize = FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL
					); 
				for(int i = dwBufferSize - 1; i >= 0; i--)
				{
					TCHAR ch = ((LPTSTR)lpMsgBuf)[i];
					if((ch == _T('\n')) || (ch == _T('\r')) || (ch == _T('\t')))
						((LPTSTR)lpMsgBuf)[i] = _T('\0');
					else
						break;
				}
				CString strErrMsg;
				strErrMsg.Format(_T("copy file error, reason:%s source file:%s, destination file:%s")	
					, (LPTSTR)lpMsgBuf, szBmpFile, strServerFile);
				// Free the buffer.
				LocalFree(lpMsgBuf);
				const wchar_t* wc_tmp = strErrMsg;
				WriteStringEx(wc_tmp);

				enReturn = FAILED_FALSE;
			}
			else
				enReturn = SUCCESS_TRUE;
		}
		//add by wxy end
	}
	return enReturn;

}
int  CSQLApi::CheckFileInTb ( wchar_t *szTable , wchar_t *szFile  )
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return ODBC_DBERROR;
	}
	SDWORD	cbID ; 
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];
	
//	_splitpath ( (const char *)szFile , (char *)drive , (char *)dir , (char *)fname , (char *)ext );
	_wsplitpath ( szFile , drive , dir , fname , ext );
	
	//Check is file exist 
	long lCount=0 ;
	RETCODE retcode;
	ODBCAllocStmt();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
	retcode = ODBCExecCmdW(L"select count(*) from %s where tele_name = '%s%s'", szTable,fname , ext);
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(TRUE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			retcode = ODBCExecCmdW(L"select count(*) from %s where tele_name = '%s%s'", szTable,fname , ext);
			if(retcode == SQL_ERROR)
				return ODBC_DBERROR;
		}
		else if(nError==ODBC_DBERROR)
			return ODBC_DBERROR;
	}
	
	ODBCBind(1, SQL_C_SLONG, &lCount,0, &cbID);
	while (retcode != SQL_NO_DATA_FOUND) 
	{
		retcode = ODBCFetch();
		if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
			;
		else
			retcode = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	//retcode =SQLFreeStmt(m_hStmt,SQL_DROP);
	if ( lCount > 0 )               
	{
		return FILEEXISTED ;
	}
	else
		return FILENOEXISTED;
}

en_ACCESSDBRET CSQLApi::CheckFileInTable (CString szTeleCode,CString szFile ,CString szLastTime)
{
	//ar szDate[16] ;
	//GetCurrentDate ( szDate ) ;
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return ACCESSDBERROR;
	}
	SDWORD	cbID ; 
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];
	_wsplitpath ( szFile , drive , dir , fname , ext );

	//Check is file exist 
	long lCount=0 ;
	long lTextSize=0,lBmpSize=0;
	RETCODE retcode;
	ODBCAllocStmt();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
//	retcode = ODBCExecCmdW(L"select count(*) from %s..wires_photo_src where datepart ( day , photo_time) = datepart( day , dateadd(minute,%d,dateadd(hour,%d,getdate()))) "
///		L"and photo_name = N'%s' and datepart ( month , photo_time) = datepart( month, dateadd(minute,%d,dateadd(hour,%d,getdate()))) "
///		L"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,getdate()))) and tele_code=N'%s'"
//		, m_szDatabase,nMin,nDelayHour,szFile,nMin,nDelayHour,nMin,nDelayHour,szTeleCode);
	retcode = ODBCExecCmdW(L"select count(*) from %s..wires_photo_src where photo_name = N'%s' and photo_time =N'%s' and tele_code=N'%s'"
		, m_szDatabase,szFile,szLastTime,szTeleCode);
	if(ODBCSQLERROR(retcode))
	{
		int nError=DecideError(FALSE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
		//	retcode = ODBCExecCmdW(L"select count(*) from %s..wires_photo_src where datepart ( day , photo_time) = datepart( day , dateadd(minute,%d,dateadd(hour,%d,getdate()))) "
		//	L"and photo_name = N'%s' and datepart ( month , photo_time) = datepart( month, dateadd(minute,%d,dateadd(hour,%d,getdate()))) "
		//	L"and datepart( year , photo_time) = datepart( year, getdate()) and tele_code=N'%s'"
		//	, m_szDatabase,nMin,nDelayHour,szFile,nMin,nDelayHour,szTeleCode);
		retcode = ODBCExecCmdW(L"select count(*) from %s..wires_photo_src where  photo_name = N'%s' and photo_time =N'%s' and tele_code=N'%s'"
		, m_szDatabase,szFile,szLastTime,szTeleCode);
			if(ODBCSQLERROR(retcode))
				return ACCESSDBERROR;
		}
		else if(nError == ODBC_DBERROR)
			return ACCESSDBERROR;
	}
	ODBCBind(1, SQL_C_SLONG, &lCount,0, &cbID);
	//ODBCBind(2, SQL_C_SLONG, &lSize,0, &cbID);
	if(!ODBCSQLERROR(retcode))
	{
		while (retcode != SQL_NO_DATA_FOUND) 
		{
			retcode = ODBCFetch();
			if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
				;
			else
				retcode = ODBCMoreResults();
		}
	}
	else
	{
		return ACCESSDBERROR;
	}
	//retcode =SQLFreeStmt(m_hStmt,SQL_DROP);
	if ( lCount > 0 )               
	{
// 		retcode = ODBCExecCmdW(L"select datalength(photo_caption),datalength(photo_bitmap) from %s..wires_photo_src where datepart ( day , photo_time) = datepart( day , dateadd(minute,%d,dateadd(hour,%d,getdate()))) "
// 		L"and photo_name = N'%s' and datepart ( month , photo_time) = datepart( month, dateadd(minute,%d,dateadd(hour,%d,getdate()))) "
// 		L"and datepart( year , photo_time) = datepart( year, dateadd(minute,%d,dateadd(hour,%d,getdate()))) and tele_code=N'%s'"
// 		, m_szDatabase,nMin,nDelayHour,szFile,nMin,nDelayHour,nMin,nDelayHour,szTeleCode);
		retcode = ODBCExecCmdW(L"select datalength(photo_caption),datalength(photo_bitmap) from %s..wires_photo_src where photo_name = N'%s' and photo_time=N'%s' and tele_code=N'%s'"
		, m_szDatabase,szFile,szLastTime,szTeleCode);
		if(ODBCSQLERROR(retcode))
		{
			int nError=DecideError(TRUE);
			if(nError==DATESOURCE_RESTORE)
			{
				ODBCAllocStmt();
				//retcode = ODBCExecCmdW(L"select datalength(photo_caption),datalength(photo_bitmap) from %s..wires_photo_src where datepart ( day , photo_time) = datepart( day , dateadd(minute,%d,dateadd(hour,%d,getdate()))) "
			//	L"and photo_name = N'%s' and datepart ( month , photo_time) = datepart( month, dateadd(minute,%d,dateadd(hour,%d,getdate()))) "
			//	L"and datepart( year , photo_time) = datepart( year, getdate()) and tele_code=N'%s'"
			//	, m_szDatabase,nMin,nDelayHour,szFile,nMin,nDelayHour,szTeleCode);
				retcode = ODBCExecCmdW(L"select datalength(photo_caption),datalength(photo_bitmap) from %s..wires_photo_src photo_name = N'%s' and photo_time=N'%s' and tele_code=N'%s'"
			, m_szDatabase,szFile,szLastTime,szTeleCode);
				if(ODBCSQLERROR(retcode))
					return ACCESSDBERROR;
			}
			else if(nError==ODBC_DBERROR)
				return ACCESSDBERROR;
		}
		ODBCBind(1, SQL_C_SLONG, &lTextSize,0, &cbID);
		ODBCBind(2, SQL_C_SLONG, &lBmpSize,0, &cbID);
		//ODBCBind(2, SQL_C_SLONG, &lSize,0, &cbID);
		if(retcode != SQL_ERROR)
		{
			while (retcode != SQL_NO_DATA_FOUND) 
			{
				 retcode = ODBCFetch();
				 if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
					 ;
				 else
					retcode = ODBCMoreResults();
			}	
		}
		else
		{
			return ACCESSDBERROR;
		}

		ODBCFreeStmt(SQL_DROP);
		if(lBmpSize>0&&lTextSize>0)
			return FILEEXISTED ;
		else if(lTextSize<=0 &&lBmpSize<=0)
			return BmpFileNoContent;
		else if(lBmpSize<=0)
			return BmpNoContent;
		else if(lTextSize<=0)
			return FileNoContent;
	}
	else
	{
		ODBCFreeStmt(SQL_DROP);
		return FILENOEXISTED;
	}
	return SUCCESS_TRUE;
}
BOOL CSQLApi::UpdateTextItem1(wchar_t *szCmd  , void far *lpText , SDWORD cbData,CString photoPath ,int type, bool bIsunicode)
{
	if(m_bDbFail)		//GetNewImageName
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}
	SDWORD  cbTextParam=0;
	RETCODE	retcode;
	PTR pToken = NULL ;
	BOOL bReturn = TRUE;


//	CString test=L"test1 \r\n";
//	fil1.Write(test,2*lstrlen(test));
	ODBCAllocStmt(); 
	retcode = ODBCCmdW( szCmd ) ;
	if (retcode == SQL_SUCCESS) 
	{

		SDWORD cbFile = SQL_NTS  , cbFlag = 0 , cbWords =0 ;
		if(type>0)
		{

			if (bIsunicode)
			{
				retcode = ODBCBindParameter(1 , SQL_PARAM_INPUT,
							  SQL_C_WCHAR, SQL_WLONGVARCHAR,
							  cbData, 0, (void *)1 , 0, &cbTextParam);
			}
			else
			{
				retcode = ODBCBindParameter(1 , SQL_PARAM_INPUT,
							  SQL_C_CHAR, SQL_LONGVARCHAR,
							  cbData, 0, (void *)1 , 0, &cbTextParam);
			}


		}else
		{
			retcode = ODBCBindParameter(1 , SQL_PARAM_INPUT,
	//					  SQL_C_CHAR, SQL_LONGVARCHAR,
						  SQL_C_BINARY, SQL_LONGVARBINARY,
						  cbData, 0, (void *)1 , 0, &cbTextParam);
		}
		cbTextParam = SQL_LEN_DATA_AT_EXEC(cbData);
	 }
	retcode = ODBCExec();
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(TRUE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			retcode = ODBCCmdW ( szCmd ) ;
			if (retcode == SQL_SUCCESS) 
			{

				SDWORD cbFile = SQL_NTS  , cbFlag = 0 , cbWords =0 ;
				retcode = ODBCBindParameter(1 , SQL_PARAM_INPUT,
								  //SQL_C_CHAR, SQL_LONGVARCHAR,
								  SQL_C_BINARY, SQL_LONGVARBINARY,
								  cbData, 0, (void *)1 , 0, &cbTextParam);
				cbTextParam = SQL_LEN_DATA_AT_EXEC(cbData);
			 }
			retcode = ODBCExec();
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
    if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
	{
		WriteStringEx(L"ODBC Insert Text ERROR");
		bReturn = FALSE ;
	}
	while (retcode == SQL_NEED_DATA) 
	{
		retcode = ODBCParamData(&pToken);
		if (retcode == SQL_NEED_DATA){
			try{
			if ( ODBCPutData(lpText , cbData) == SQL_ERROR ) 
			{
				int nError=DecideError(TRUE);	
				if(nError==DATESOURCE_RESTORE)
					ODBCAllocStmt();	
				else if(nError==ODBC_DBERROR)
					return FALSE;
				bReturn = FALSE ;
				break;
			}
		}
			catch(...){
				CString str;
				str.Format(L"Upload caption error: %s \r\n",photoPath);
				WriteStringEx(str);
				//AfxMessageBox(L"error");
			}
		}		
	 }
//	test=L"test6 \r\n";
//	fil1.Write(test,2*lstrlen(test));
//	fil1.Close();
	ODBCFreeStmt ( SQL_DROP);	
	return bReturn ;

}

BOOL CSQLApi::UpdateTextItem1_Ansi(char *szCmd  , void far *lpText , SDWORD cbData ,int type)
{
	if(m_bDbFail)		//GetNewImageName
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}
	SDWORD  cbTextParam=0;
	RETCODE	retcode;
	PTR pToken = NULL ;
	BOOL bReturn = TRUE;

	ODBCAllocStmt(); 
	retcode = ODBCCmd( szCmd ) ;
	if (retcode == SQL_SUCCESS) 
	{

		SDWORD cbFile = SQL_NTS  , cbFlag = 0 , cbWords =0 ;
		if(type>0)
		{
				retcode = ODBCBindParameter(1 , SQL_PARAM_INPUT,
							  SQL_C_CHAR, SQL_LONGVARCHAR,
							  cbData, 0, (void *)1 , 0, &cbTextParam);

		}else
		{
			retcode = ODBCBindParameter(1 , SQL_PARAM_INPUT,
	//					  SQL_C_CHAR, SQL_LONGVARCHAR,
						  SQL_C_BINARY, SQL_LONGVARBINARY,
						  cbData, 0, (void *)1 , 0, &cbTextParam);
		}
		cbTextParam = SQL_LEN_DATA_AT_EXEC(cbData);
	 }
	retcode = ODBCExec();
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(TRUE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			retcode = ODBCCmd ( szCmd ) ;
			if (retcode == SQL_SUCCESS) 
			{

				SDWORD cbFile = SQL_NTS  , cbFlag = 0 , cbWords =0 ;
				retcode = ODBCBindParameter(1 , SQL_PARAM_INPUT,
								  //SQL_C_CHAR, SQL_LONGVARCHAR,
								  SQL_C_BINARY, SQL_LONGVARBINARY,
								  cbData, 0, (void *)1 , 0, &cbTextParam);
				cbTextParam = SQL_LEN_DATA_AT_EXEC(cbData);
			 }
			retcode = ODBCExec();
			if(retcode == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
    if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
	{
		//AfxMessageBox ( "ODBC Insert Text ERROR");
		WriteStringEx(L"ODBC Insert Text ERROR");
		bReturn = FALSE ;
	}
	while (retcode == SQL_NEED_DATA) 
	{
	
		retcode = ODBCParamData(&pToken);
		if (retcode == SQL_NEED_DATA) 
			if ( ODBCPutData(lpText , cbData) == SQL_ERROR ) 
			{
				int nError=DecideError(TRUE);	
				if(nError==DATESOURCE_RESTORE)
					ODBCAllocStmt();	
				else if(nError==ODBC_DBERROR)
					return FALSE;
				bReturn = FALSE ;
				break;
			}
	 }
	ODBCFreeStmt ( SQL_DROP);	
	return bReturn ;

}


BOOL CSQLApi::GetAgencySt(wchar_t *szTelecode, wchar_t *szPhotoDesPath,int * nMarkLine,int *bTransfer,int * nTitleLine)
{
	BOOL bFill= FALSE;
	WORD nMarkL,nTransfer,nTitleL;
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}
	ODBCAllocStmt();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
	RETCODE nResult=ODBCExecCmdW(L"select title_line,mark_line,translate,photo_despath from %s..news_agency where tele_code=N'%s'",m_szDatabase,szTelecode);
	SDWORD	cbName,cbDesPath;
	if(nResult==SQL_ERROR)
	{
		int nError=DecideError(TRUE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			nResult=ODBCExecCmdW(L"select title_line,mark_line,translate,photo_despath from %s..news_agency where tele_code=N'%s'",m_szDatabase,szTelecode);
			if(nResult == SQL_ERROR)
				return FALSE;
		}
		else if(nError==ODBC_DBERROR)
			return FALSE;
	}
	ODBCBind(1,SQL_C_SHORT,&nTitleL,0,&cbName);
	ODBCBind(2,SQL_C_SHORT,&nMarkL,0,&cbName);
	ODBCBind(3,SQL_C_SHORT,&nTransfer,0,&cbName);
	ODBCBind(4,SQL_C_CHAR,szPhotoDesPath,PATH_LENGTH,&cbDesPath);
	while (nResult != SQL_NO_DATA_FOUND) 
	{
	    nResult = ODBCFetch();
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{	
			*nTitleLine=nTitleL;
			*nMarkLine=nMarkL;
			*bTransfer=nTransfer;
			ChangeSpaceZeroW(szPhotoDesPath,cbDesPath);
			bFill=TRUE;
		 } 
		else
			nResult = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	//SQLFreeStmt(m_hStmt,SQL_DROP);
	return bFill ;
	
}
#define			MAX_TITLE_NUM	255    
#define			MAX_WORD_LENGTH  50

int CSQLApi::GetLib1 ( wchar_t *szTitle , wchar_t * lpszLocalPlace )
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return ODBC_DBERROR;
	}
	wchar_t 	szWord[ 100 +5120]={0};
	lpszLocalPlace[0]='\0';
	//char 	szTitle[MAX_TITLE_NUM];
	CString strTemp;
	int nFind;
	int nResult;
	int		nPos;
	int 	nLen=lstrlen(szTitle) ;//= MAX_TITLE_NUM ;               
	BOOL	bFirst = FALSE ;
	int i = 0 , j = 0 ;
	wchar_t lpLocalPlace [20];
	while ( szTitle[i] != '\0' )	
	{
		if (szTitle[i]>=0 && szTitle[i]<0x80 && isalpha (szTitle[i]) == 0 && szTitle[i] != '.' && szTitle[i] != '\x27')	//sperate char
		{
			szWord[j] = '\0' ;
			nResult=GetTeleAttrL ( szWord , lpLocalPlace , L"N" );
			if (nResult==ODBC_DBERROR )
				return ODBC_DBERROR;
			else if(nResult)
			{
				lstrcpy ( lpszLocalPlace , lpLocalPlace ) ;
				nPos = i ;
				j = 0;
				break;
			}
			j = 0 ;
		}                                        
		else  
		{
			szWord[j] = szTitle[i] ;
			j ++ ;

		}
		i ++ ;
	}
	if ( j != 0 )
	{
		szWord[j] = '\0' ;
		nResult=GetTeleAttrL ( szWord , lpLocalPlace , L"N" );
		if (nResult==ODBC_DBERROR )
			return ODBC_DBERROR;
		else if(nResult)
		{
			lstrcpy ( lpszLocalPlace , lpLocalPlace ) ;
			nPos = i ;
		}
	}
	// Seperate into two words                            
	i = 0 ; j = 0 ;
	while ( szTitle[i] != '\0' )	
	{
		if (isalpha (szTitle[i]) == 0 && szTitle[i] != '.' && szTitle[i] != '\x27')
		{
			if ( bFirst && szTitle[i]>=0)
			{
				bFirst = FALSE ;
				szWord[j] = '\0' ;
				nResult=GetTeleAttrL ( szWord , lpLocalPlace , L"N" );
				if (nResult==ODBC_DBERROR )
					return ODBC_DBERROR;
				else if(nResult)
				{                                              
					if ( i <= nPos )
					{
						lstrcpy ( lpszLocalPlace , lpLocalPlace ) ;
						return TRUE ;     // Use two words compose
					}
				}   
				strTemp=szWord;
				nFind=strTemp.Find(' ');
				//if ( (cp = strchr  ( szWord , ' ' )) == NULL )	// '-' the next word
				//	j = 0 ;
				if(nFind==-1)
					j=0;
				else                    // ' ' connect with other word
				{
					//j = strlen(cp) - 1;
					j=strTemp.GetLength()-nFind;
					//strcpy ( szWord , cp+1 );
					CString str_temp = strTemp.Right(j-1);
					const wchar_t * wc_tmp = str_temp;
					lstrcpy ( szWord ,  wc_tmp );
					szWord[j] = szTitle[i] ;
					j ++ ;
					bFirst = TRUE;
				}
			}
			else              
			{
				bFirst = TRUE;
				szWord[j] = szTitle[i] ;
				j ++ ;
				szWord[j] =0;
			}
			
		}                                        
		else  
		{
			szWord[j] = szTitle[i] ;
			j ++ ;
			szWord[j]=0;
		}
		i ++ ;
	}
	if ( j != 0 )
	{
		szWord[j] = '\0' ;
		nResult=GetTeleAttrL ( szWord , lpLocalPlace , L"N" );
		if (nResult==ODBC_DBERROR )
			return ODBC_DBERROR;
		else if(nResult)
		{
			if ( i <= nPos )
			{
				lstrcpy ( lpszLocalPlace , lpLocalPlace ) ;
				return TRUE ;     // Use two words compose
			}
		}
	}
	
	
	if ( nPos != nLen)
	{
		return TRUE ;	// Use Single Word	
	}

	// Seperate Title to single word  
	i=0 , j = 0;//bFirst = FALSE ;
	while ( szTitle[i] != '\0' )	
	{
//		if (isalpha (szTitle[i]) == 0 && szTitle[i] != '.' && szTitle[i] != '\x27')
		if (iswalpha (szTitle[i]) == 0 && szTitle[i] != '.' && szTitle[i] != '\x27')
		{
			szWord[j] = '\0' ;
			nResult=GetTeleAttrL ( szWord , lpLocalPlace , L"C" );
			if (nResult==ODBC_DBERROR )
				return ODBC_DBERROR;
			else if(nResult)
			{
				lstrcpy ( lpszLocalPlace , lpLocalPlace ) ;
				nPos = i ;
				j = 0;
				break;
			}
			j = 0 ;
		}                                        
		else  
		{
			szWord[j] = szTitle[i] ;
			j ++ ;
			szWord[j] =0;
		}
		i ++ ;
	}
	if ( j != 0 )
	{
		szWord[j] = '\0' ;
		nResult=GetTeleAttrL ( szWord , lpLocalPlace , L"C" );
		if (nResult==ODBC_DBERROR )
			return ODBC_DBERROR;
		else if(nResult)
		{
			lstrcpy ( lpszLocalPlace , lpLocalPlace ) ;
			nPos = i ;
		}
	}
	
	// Seperate into two words                            
	i = 0 ; j = 0 ;
	while ( szTitle[i] != '\0' )	
	{
//		if ( isalpha (szTitle[i]) == 0 && szTitle[i] != '.' && szTitle[i] != '\x27')
		if ( iswalpha (szTitle[i]) == 0 && szTitle[i] != '.' && szTitle[i] != '\x27')
		{
			if ( bFirst )
			{
				bFirst = FALSE ;
				szWord[j] = '\0' ;
				nResult=GetTeleAttrL ( szWord , lpLocalPlace , L"C" );
				if (nResult==ODBC_DBERROR )
					return ODBC_DBERROR;
				else if(nResult)
				{		
					if ( i <= nPos )
					{
						lstrcpy ( lpszLocalPlace , lpLocalPlace ) ;
						return TRUE ;     // Use two words compose
					}
				}   
				
				strTemp=szWord;
				nFind=strTemp.Find(' ');
				//if ( (cp = strchr  ( szWord , ' ' )) == NULL )	// '-' the next word
				//	j = 0 ;
				if(nFind==-1)
					j=0;
				else                    // ' ' connect with other word
				{
					//j = strlen(cp) - 1;
					j=strTemp.GetLength()-nFind;
					//strcpy ( szWord , cp+1 );
					CString str_temp = strTemp.Right(j-1);
					lstrcpy ( szWord , str_temp);
					szWord[j] = szTitle[i] ;
					j ++ ;
					bFirst = TRUE;
				}
			}
			else              
			{
				bFirst = TRUE;
				szWord[j] = szTitle[i] ;
				j ++ ;
				szWord[j] =0;
			}
			
		}                                        
		else  
		{
			szWord[j] = szTitle[i] ;
			j ++ ;
			szWord[j] =0;
		}
		i ++ ;
	}
	if ( j != 0 )
	{
		szWord[j] = '\0' ;
		nResult=GetTeleAttrL ( szWord , lpLocalPlace , L"C" );
		if (nResult==ODBC_DBERROR )
			return ODBC_DBERROR;
		else if(nResult)
		{
			if ( i <= nPos )
			{
				lstrcpy ( lpszLocalPlace , lpLocalPlace ) ;
				return TRUE ;     // Use two words compose
			}
		}
	}
	if ( nPos == nLen)
		return FALSE ;
	else
		return TRUE ;	// Use Single Word	
}
int CSQLApi::GetTeleAttrL(wchar_t *szKeyWord, wchar_t *szTeleAttr, wchar_t *szKeyStyle)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return ODBC_DBERROR;
	}
	CString strTem=szKeyWord;
	strTem.TrimLeft();
	strTem.TrimRight();
	if(strTem.IsEmpty())
	{
		szTeleAttr[0]='\0';
		return FALSE;
	}
	const wchar_t * wc_tmp = strTem;
	lstrcpy ( szKeyWord , wc_tmp);

	wc_tmp = strTem;
	lstrcpy(szKeyWord, wc_tmp);

	RETCODE retcode;
	BOOL	bTrue=FALSE;
	SDWORD	cbID ;
	ODBCAllocStmt();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,10);
	retcode = ODBCExecCmdW(L"select tele_attr from %s..news_location where key_word = '%s ' and key_style='%s '",m_szDatabase,szKeyWord,szKeyStyle);
	if(retcode==SQL_ERROR)
	{
		int nError=DecideError(TRUE);
		if(nError==DATESOURCE_RESTORE)
		{
			ODBCAllocStmt();
			retcode = ODBCExecCmdW(L"select tele_attr from %s..news_location where key_word = '%s ' and key_style='%s '",m_szDatabase,szKeyWord,szKeyStyle);
			if(retcode == SQL_ERROR)
				return ODBC_DBERROR;
		}
		else if(nError==ODBC_DBERROR)
			return ODBC_DBERROR;
	}
	ODBCBind(1, SQL_C_CHAR, szTeleAttr,TELEATTR_LEN , &cbID);
	while (retcode != SQL_NO_DATA_FOUND) 
	{
		//ODBCBind(2, SQL_C_CHAR, szKeyStyle,2, &cbID);
		 retcode = ODBCFetch();
		 if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
		 	 bTrue=TRUE	 ;
		 else
			retcode = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	//SQLFreeStmt(m_hStmt,SQL_DROP);
	if(!bTrue)
		szTeleAttr[0]='\0';
	return bTrue;
}

BOOL CSQLApi::GetConfigure (wchar_t *szEntry , wchar_t *szValue)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}	
	wchar_t szBuffer[256]={0};
	RETCODE nResult;
	SDWORD	cbValue ;
	BOOL bFil = FALSE ;

	ODBCAllocStmt ();
	nResult = ODBCExecCmdW( L"select value from %s where entry=N'%s'",m_szSysDB, szEntry);
	ODBCBind(1, SQL_C_WCHAR, szBuffer, 255, &cbValue);
	while (nResult != SQL_NO_DATA_FOUND) 
	{
		 nResult = ODBCFetch();
		 ChangeSpaceZeroW ( szBuffer , cbValue ) ;
		 if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		 {
			 bFil = TRUE;
		 }
		 else
			nResult = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	lstrcpy ( szValue, szBuffer ); 
	return bFil;
}

BOOL CSQLApi::FormPhotoPath(wchar_t *szPhotodesPath,CTime	rTime, wchar_t *szCode)
{
	wchar_t szFmt[255],szPath[255],szSubDir[255],szDrive[255];
	GetConfigure ( L"photo_src_datefmt" ,szFmt);
	GetConfigure ( L"photo_src_path" ,szPath);
	GetConfigure ( L"photo_src_subdir" ,szSubDir);
	GetConfigure ( L"photo_driver" ,szDrive);
	wchar_t szDatePath[255];
	switch (szFmt[0])
	{
	case '0':	//	none
		swprintf ( szDatePath , L"");
		break;
	case '1': //  mm$dd
		swprintf ( szDatePath , L"%02d$%02d" ,rTime.GetMonth(),rTime.GetDay());// RealrTime.GetYear() % 100);
		break;
	case '2': //  mmddyy
		swprintf ( szDatePath , L"%02d%02d%02d" ,rTime.GetMonth(),rTime.GetDay(),rTime.GetYear() % 100);
		break;
	case '3': //  ddmmyy
		swprintf ( szDatePath , L"%02d%02d%02d" ,rTime.GetDay() ,rTime.GetMonth() ,rTime.GetYear() % 100);
		break;
	case '4': //  yymmdd
		swprintf ( szDatePath , L"%02d%02d%02d" ,rTime.GetYear() % 100, rTime.GetMonth(),rTime.GetDay());
		break;
	case '5': //  dd
		swprintf ( szDatePath , L"%02d" ,rTime.GetDay());
		break;
	case '6': //  mmdd
		swprintf ( szDatePath , L"%02d%02d" ,rTime.GetMonth(),rTime.GetDay());
		break;
	default:
		break;
	}

	switch (szSubDir[0])
	{
	case '0':	//	date\\code
			//use by mingpao with wangquan 's format    mm$dd\code,d,w,no.mdd
			swprintf ( szPhotodesPath , L"%s\\%s\\%s\\%s" , szDrive,szPath, szDatePath ,szCode);
		break;
	case '1': //	code\\date
			swprintf ( szPhotodesPath , L"%s\\%s\\%s\\%s" , szDrive,szPath,szCode , szDatePath);
		break;
	case '2': //date
			//use by singpao	mmdd\code\dd,code,no.txt
			swprintf ( szPhotodesPath , L"%s\\%s\\%s" ,szDrive,szPath , szDatePath );
		break;
	case '3': //code
			swprintf ( szPhotodesPath , L"%s\\%s\\%s" ,szDrive,szPath , szCode );
		break;
	default:
		break;
	}
	return TRUE;			
}	

BOOL CSQLApi::GetPhotoPath(wchar_t *szPhotodesPath)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}	wchar_t szPath[255],szDrive[255],szShare[255],szServer[255];
	GetConfigure ( L"photo_src_path" ,szPath);
	GetConfigure ( L"photo_driver" ,szDrive);
	GetConfigure ( L"photo_server" ,szServer);
	GetConfigure ( L"photo_share" ,szShare);
	swprintf( szPhotodesPath,L"%s\\%s",szDrive,szPath);
	ConnectDisk ( (char *)szServer , (char *)szShare , (char *)szDrive );
	return TRUE;			
}	



BOOL CSQLApi::CheckDB()
{
	CDBGraphicSelect dlgdb;
	//dlgdb.m_szDbNameT[0]='\0';
	dlgdb.DoModal();
	if(!lstrcmp( m_szDatabase, L""))
			return FALSE;
	return TRUE;
}


BOOL CSQLApi::CheckTable() 
{
	CString DatabaseTemp;
	wchar_t szCmd1[50],szCmd2[50],szCmd3[50],szCmd4[50];
	DatabaseTemp=AfxGetApp()->GetProfileString(L"System",L"DATABASE",L"");
	wchar_t Temp[NAME_LEN];

	const wchar_t* wc_tmp = DatabaseTemp;
	lstrcpy(Temp,wc_tmp);
	if(DatabaseTemp.IsEmpty())
	{
		if(!CheckDB())
			return FALSE;
	//AfxGetApp()->WriteProfileString("System","DATABASE",m_szDatabase);

	}
	else
	{
		if(!p_dbCmd->CheckDatabase(Temp,TRUE))
			DatabaseTemp.Empty();
		if(!DatabaseTemp.IsEmpty())
		{
			const wchar_t* wc_tmp =  DatabaseTemp;
			lstrcpy(m_szDatabase,wc_tmp);

			swprintf(szCmd1,L"%s..news_agency",m_szDatabase);
			swprintf(szCmd2,L"%s..news_location",m_szDatabase);
			swprintf(szCmd3,L"%s..news_recv",m_szDatabase);
			swprintf(szCmd4,L"%s..news_keyword",m_szDatabase);
			if((CheckDbteleTable(szCmd1) != SUCCESS_TRUE)
				|| (CheckDbteleTable(szCmd3) != SUCCESS_TRUE)
				|| (CheckDbteleTable(szCmd2) != SUCCESS_TRUE)
				|| (CheckDbteleTable(szCmd4) != SUCCESS_TRUE))
			//if(!p_dbCmd->CheckDbteleTable(szCmd1)||!p_dbCmd->CheckDbteleTable(szCmd3)||!p_dbCmd->CheckDbteleTable(szCmd2)||!p_dbCmd->CheckDbteleTable(szCmd4))		
			{
				if(!CheckDB())
				{
			//		p_dbCmd->SQLFreeStmt(m_hStmt,SQL_DROP);//ODBCFreeStmt(SQL_DROP);
					return FALSE;
				}
			}
			//p_dbCmd->SQLFreeStmt(m_hStmt,SQL_DROP);//ODBCFreeStmt(SQL_DROP);
		}
		else
		{
			if(!CheckDB())
			    return FALSE;
		}
	}
	//edit by leon 2012-08-27 for migrating from vc6 to 2010
	/*AfxGetApp()->WriteProfileString(L"System",L"DATABASE", m_szDatabase);*/
	WritePrivateProfileStringW(L"System",L"DATABASE", m_szDatabase,AfxGetApp()->m_pszProfileName);
	return TRUE;
}


en_ACCESSDBRET CSQLApi::GetNewImageName(wchar_t *szFile)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return ACCESSDBERROR;
	}
	RETCODE retcode;
	SDWORD cbCode;
	wchar_t   szCode[512];
	en_ACCESSDBRET enRet;
	ODBCAllocStmt();
	do
	{
		retcode = ODBCExecCmdW( L"declare @filecode nvarchar(13)\n "
			L"Exec %s..GetImageFileName @filecode output \n"
			L"select @filecode",m_szDatabase );
		ODBCBind(1, SQL_C_WCHAR, szCode, 250 , &cbCode);
		if(!ODBCSQLERROR(retcode))
		{
			while (retcode != SQL_NO_DATA_FOUND) 
			{
				retcode = ODBCFetch();
				if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
					ChangeSpaceZeroW ( szCode , cbCode );
				else
					retcode = ODBCMoreResults();
			}
		}
		else
		{
			return ACCESSDBERROR;
		}
//		sprintf ( szFile , "%s%x%03x" , szCode , nMon , nNo );
		swprintf ( szFile , szCode );
		enRet = IsImageFileExist(szFile);
		if(enRet == ACCESSDBERROR)
			return enRet;
	} while(enRet == SUCCESS_TRUE);
	ODBCFreeStmt(SQL_DROP);
	return SUCCESS_TRUE;
}

en_ACCESSDBRET CSQLApi::IsImageFileExist(wchar_t *szFile)
{
	en_ACCESSDBRET enRet = ACCESSDBERROR;
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return enRet;
	}
	long nResult = 0L;
	RETCODE retcode;
	SDWORD cbResult;
	ODBCAllocStmt ();
	retcode = ODBCExecCmdW(L"select 1 from %s..wires_photo_src where photo_name=N'%s'" ,m_szDatabase , szFile);
	ODBCBind(1, SQL_C_LONG, &nResult, 0, &cbResult);
	if(!ODBCSQLERROR(retcode))
	{
		while (retcode != SQL_NO_DATA_FOUND) 
		{
			retcode = ODBCFetch();
			if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
				;
			else
				retcode = ODBCMoreResults();
		}
	}
	else
	{
		return enRet;
	}
	ODBCFreeStmt(SQL_DROP);
	if(nResult == 0)
		enRet = FAILED_FALSE;
	else
		enRet = SUCCESS_TRUE;
	return enRet;
}


en_ACCESSDBRET CSQLApi::ImageIsLogged(CString szFile,CString szTime,BOOL bCheckTime)
{

	CString fileTime=(CString)szTime;
	fileTime.TrimRight();
	szTime=fileTime;
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return ACCESSDBERROR;
	}
	//AfxMessageBox(szTime);
	wchar_t DBName[128];	
	long exist = 0;
	long cbResult;
	int retcode;
	//modified by wxy below editor5
	//sprintf(DBName,"%s..photo_log",m_szDatabase);
	swprintf(DBName,L"%s..wires_photo_src",m_szDatabase);
	//modified end
	en_ACCESSDBRET ret = CheckDbteleTable(DBName);
	if(ret != SUCCESS_TRUE)
	{
		if(ret == FAILED_FALSE)
		{
			CString str_temp = ((CString)"沒有創建"+DBName+"請在採編圖片處理中設置。");
			const wchar_t* wc_tmp = str_temp;
		
			WriteStringEx(wc_tmp);
		}
		return ACCESSDBERROR;
	}

	//swprintf(szCmd,L"select 1 from %s where photo_path=N'%s'", DBName, szFile);
	ODBCAllocStmt ();
	//retcode = ODBCExecCmd(szCmd);
	if (!bCheckTime)
		retcode = ODBCExecCmdW(L"select 1 from %s where photo_src_path = N'%s' and small_path <> N'DummyPath'", DBName, szFile);	
	else
		retcode = ODBCExecCmdW(L"select 1 from %s where photo_src_path = N'%s' and last_modify_time = N'%s' and small_path <> N'DummyPath'", DBName, szFile,szTime);
	ODBCBind(1, SQL_C_LONG, &exist, 0, &cbResult);
	if(!ODBCSQLERROR(retcode))
	{
		while (retcode != SQL_NO_DATA_FOUND) 
		{
			retcode = ODBCFetch();
			if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO);
			else
				retcode = ODBCMoreResults();
		}
	}
	else
	{
		return ACCESSDBERROR;
	}
	ODBCFreeStmt(SQL_DROP);
	if(exist == 1)
		return SUCCESS_TRUE;
	else
		return FAILED_FALSE;
}


BOOL CSQLApi::GetServerTime(CTime & CurTime)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}
	int y,m,d,hour,min,sec;
	CTime t=CTime::GetCurrentTime();
	y=t.GetYear();
	m=t.GetMonth();
	d=t.GetDay();
	hour=t.GetHour();
	min=t.GetMinute();
	sec=t.GetSecond();
	wchar_t buf[32];
	long cbResult;
	int retcode;
	ODBCAllocStmt ();
	memset(buf,0,32);
	retcode = ODBCExecCmd("select CONVERT(wchar_t(12), GETDATE(), 110)");
	ODBCBind(1, SQL_CHAR, buf, 30, &cbResult);
	while (retcode != SQL_NO_DATA_FOUND) 
	{
		 retcode = ODBCFetch();
		 if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
			;
		 else
			retcode = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	if(buf[0])
	{
		swscanf(buf,L"%02d-%02d-%04d",&m,&d,&y);
	}
	ODBCAllocStmt ();
	memset(buf,0,32);
	retcode = ODBCExecCmd("select CONVERT(char(12), GETDATE(), 114)");
	ODBCBind(1, SQL_CHAR, buf, 30, &cbResult);
	while (retcode != SQL_NO_DATA_FOUND) 
	{
		 retcode = ODBCFetch();
		 if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
			;
		 else
			retcode = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	if(buf[0])
	{
		swscanf(buf,L"%02d:%02d:%02d",&hour,&min,&sec);
	}
	CurTime=CTime(y,m,d,hour,min,sec);
	return TRUE;
}



int CSQLApi::GetFieldType(wchar_t * szTable, wchar_t *  szColumn)
{ 
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}
	CString strTable=szTable;
	if(strTable.ReverseFind('.')>=0) 
		strTable=strTable.Right(strTable.GetLength()-strTable.ReverseFind('.')-1);
	int retcode;
	long type=0;	    
	long cblen;
	ODBCAllocStmt();
	retcode=ODBCExecCmdW(L"select type from %s..syscolumns where  id = "
		L"(select id from %s..sysobjects where type='U' and name =N'%s')"
		L"and name=N'%s'",m_szDatabase,m_szDatabase,strTable,szColumn);
	if(retcode==SQL_SUCCESS)
	{
		ODBCBind(1,SQL_C_LONG,&type,0,&cblen);
		while(TRUE)
		{
			retcode=ODBCFetch();
			if(retcode==SQL_ERROR)
			{
				break;
			}
			else if(retcode==SQL_SUCCESS||retcode==SQL_SUCCESS_WITH_INFO)
			{;
			}
			else
				break;
		}
	}
	ODBCFreeStmt(SQL_DROP);
	return type;
}



CString CSQLApi::GetNewAgencyFlag(wchar_t * m_szTeleCode)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return L"";
	}
	wchar_t szBuf[32];
	wchar_t szCmd[512];
	RETCODE nResult;
	SDWORD	cb=0;
	szBuf[0]=0;
	swprintf(szCmd,L"select flag from %s..news_agency where tele_code=N'%s'",m_szDatabase,m_szTeleCode);
	ODBCAllocStmt ();
	//nResult = ODBCExecCmd ( szCmd);
	nResult = ODBCExecCmdW (L"select flag from %s..news_agency where tele_code=N'%s'",m_szDatabase,m_szTeleCode);
	ODBCBind(1, SQL_C_CHAR , szBuf , 6, &cb );
	while (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
	{
	    nResult = ODBCFetch();
		ChangeSpaceZero((char *)szBuf,cb);
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{
			;
		}
		else
			nResult = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	szBuf[cb]=0;
	return szBuf;
}


void CSQLApi::GetDepartment(CComboBox *m_combo)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return ;
	}
	wchar_t	szdepart[32];
	int nDepart,nItem;
	SDWORD	cb=0;
	ODBCAllocStmt();
	ODBCSetStmtOption(SQL_QUERY_TIMEOUT,20);
	RETCODE nResult=ODBCExecCmdW(L"select lib_name,lib_id from %s..news_lib where parent_id<>0 order by lib_id ",m_szDatabase);
	if(nResult==SQL_ERROR)
	{
			return ;
	}
	ODBCBind(1,SQL_C_WCHAR,&szdepart,32,&cb);
	ODBCBind(2,SQL_C_LONG,&nDepart,0,&cb);
	while (nResult != SQL_NO_DATA_FOUND) 
	{
	    nResult = ODBCFetch();
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{	
	
			ChangeSpaceZeroW(szdepart,cb);
/*
			CString	s,t;
			s.Format("%s",szdepart);
			t.Format("%s",szdepart);
			cb = s.GetLength();
			s = s + "                                          ";
			s=s.Mid(0,24 - t.GetLength());
			s = s + t;
			strcpy(szdepart,s);
			cb = s.GetLength();*/
			nItem=m_combo->AddString(szdepart);
			m_combo->SetItemData(nItem,nDepart);
		 }
		else
			nResult = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	return ;

}

CString CSQLApi::PrepareSQL(LPCTSTR szFormat, ...)
{
	TCHAR szCmd[BUF_SIZE], szArg[BUF_SIZE], szSubFormat[10];
	TCHAR szTwoChar[2] = _T(" ");
	LPCTSTR pszFormat = szFormat;
	LPTSTR pszArg;
	va_list ap;
	BOOL bPercent = FALSE;
	BOOL bFormatOK, bLongStyle;
	BOOL bQuote = FALSE;
	
	szCmd[0] = TCHAR('\0');
	va_start(ap, szFormat);
	
	while(*pszFormat)
	{
		if (bPercent)
		{
			bPercent = FALSE;
			bLongStyle = FALSE;
			switch(*pszFormat)
			{
			case TCHAR('%'):
				_tcscat(szCmd, _T("%"));
				break;
			default:
				_tcscpy(szSubFormat, _T("%"));
				bFormatOK = FALSE;
				while(*pszFormat)
				{
					switch(*pszFormat)
					{
						// int
					case TCHAR('d'):	case TCHAR('i'):	
					case TCHAR('u'):	case TCHAR('o'):
					case TCHAR('x'):	case TCHAR('X'):
					case TCHAR('c'):
						szTwoChar[0] = *pszFormat;
						_tcscat(szSubFormat, szTwoChar);
						if ( bLongStyle )
							_stprintf(szArg, szSubFormat, va_arg(ap, long));
						else
							_stprintf(szArg, szSubFormat, va_arg(ap, int));
						_tcscat(szCmd, szArg);
						bFormatOK = TRUE;
						bQuote = FALSE;
						break;
						// double
					case TCHAR('e'):	case TCHAR('E'):
					case TCHAR('g'):	case TCHAR('G'):
					case TCHAR('f'):
						szTwoChar[0] = *pszFormat;
						_tcscat(szSubFormat, szTwoChar);
						_stprintf(szArg, szSubFormat, va_arg(ap, double));
						_tcscat(szCmd, szArg);
						bFormatOK = TRUE;
						bQuote = FALSE;
						break;
						// string
					case TCHAR('s'):
						szTwoChar[0] = *pszFormat;
						_tcscat(szSubFormat, szTwoChar);
						_stprintf(szArg, szSubFormat, va_arg(ap, char*));
						// convert to DbSafe
						pszArg = szArg;
						while (*pszArg)
						{
							switch(*pszArg)
							{
							case TCHAR('\x27'):	// '
								szTwoChar[0] = *pszArg;
								_tcscat(szCmd, szTwoChar);
								if ( bQuote )
                                		_tcscat(szCmd, szTwoChar);
								break;
							default:
								szTwoChar[0] = *pszArg;
								_tcscat(szCmd, szTwoChar);
							}
							pszArg ++;
						}
						bFormatOK = TRUE;
						bQuote = FALSE;
						break;
						
					default:
						szTwoChar[0] = *pszFormat;
						_tcscat(szSubFormat, szTwoChar);
						if (*pszFormat == TCHAR('l'))
							bLongStyle = TRUE;
						break;
					}
					if (bFormatOK )
						break;
					pszFormat ++;
				}
				break;
			}
		}
		else
		{
			switch(*pszFormat)
			{
			case TCHAR('%'):
				bPercent = TRUE;
				break;
			case TCHAR('\x27'):
				szTwoChar[0] = *pszFormat;	// tmp use
				_tcscat(szCmd, szTwoChar);
				bQuote=TRUE ;
				break;
			default:
				szTwoChar[0] = *pszFormat;	// tmp use
				_tcscat(szCmd, szTwoChar);
				bQuote = FALSE;
			}
		}
		pszFormat ++;
	}
	va_end(ap);
	return CString(szCmd);
}

BOOL CSQLApi::GetConfig (int szNum , wchar_t *szValue)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}	
	wchar_t szBuffer[256]={0};
	RETCODE nResult;
	SDWORD	cbValue ;
	BOOL bFil = FALSE ;
	ODBCAllocStmt ();
	nResult = ODBCExecCmdW( L"select db_Code from %s",L"dbgraphi_info");
	ODBCBind(1, SQL_C_WCHAR, szBuffer, 255, &cbValue);
	while (nResult != SQL_NO_DATA_FOUND) 
	{
		
		nResult = ODBCFetch();
		ChangeSpaceZeroW ( szBuffer , cbValue ) ;
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{
			bFil = TRUE;
		}
		else
			nResult = ODBCMoreResults();

	}
	ODBCFreeStmt(SQL_DROP);
	lstrcpy ( szValue, szBuffer ); 
	return bFil;
}

int CSQLApi::GetNum ( wchar_t *szValue)
{
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}	
	wchar_t szBuffer[256]={0};
	RETCODE nResult;
	SDWORD	cbValue ;
	BOOL bFil = FALSE ;
	ODBCAllocStmt ();
	nResult = ODBCExecCmdW( L"select count(*) db_Code from %s",L"dbgraphi_info");
	ODBCBind(1, SQL_C_WCHAR, szBuffer, 255, &cbValue);
	while (nResult != SQL_NO_DATA_FOUND) 
	{
		
		nResult = ODBCFetch();
		ChangeSpaceZeroW ( szBuffer , cbValue ) ;
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{
			bFil = TRUE;
		}
		else
			nResult = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	lstrcpy ( szValue, szBuffer ); 
	wchar_t length_db[2];
    length_db[0] = szValue[0];
    length_db[1] = 0;
    int numeric_peek = _wtoi(length_db);
	
	return numeric_peek;
}

BOOL CSQLApi::getSeverTime(CString &severTime){
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return FALSE;
	}	
	wchar_t szBuffer[256]={0};
	RETCODE nResult;
	SDWORD	cbValue ;
	BOOL bFil = FALSE ;
	ODBCAllocStmt ();
	nResult = ODBCExecCmdW( L"select getdate()");
	ODBCBind(1, SQL_C_WCHAR, szBuffer, 255, &cbValue);
	while (nResult != SQL_NO_DATA_FOUND) 
	{
		
		nResult = ODBCFetch();
		ChangeSpaceZeroW ( szBuffer , cbValue ) ;
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{
			bFil = TRUE;
		}
		else
			nResult = ODBCMoreResults();
	}
	ODBCFreeStmt(SQL_DROP);
	severTime=(CString)szBuffer;
	return TRUE;
}

void CSQLApi::InsertValuesDummy (bool& bInserted, PhotoInfo *pInfo, wchar_t *szTeleCode,int nAgencyID,int nAgencyFlag
		, wchar_t *szBmpFile , wchar_t *szFile , wchar_t *szMidPath, wchar_t *szSmallPath
		, int nDelayHour,int nMin, wchar_t *szJpgFile,int nMarkLine,int nTitleLine
		,BOOL bTransfer,int nSize,wchar_t * szLastTime,wchar_t *szSourthpath,wchar_t *szPhotoTime,int addday, 
		wchar_t * szTime, wchar_t *szServerFile)
{
//	AfxMessageBox(L"szServerFile");
//	AfxMessageBox(szServerFile);
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return;
	}
	long lFlag=0L;     //950811
	wchar_t szTitle[TITLE_LEN+5120]=L"";
	wchar_t szMarkTitle[TITLE_LEN+5120]=L"";
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];
	wchar_t szTeleAttrL[TELEATTR_LEN]=L"";
	long  lDelay=0L,lWord=0L;
	en_ACCESSDBRET enReturn = FAILED_FALSE;
	if(szTime==NULL) szTime=L"";
	CString PhotoExt;
	nDelayHour=-nDelayHour;
	nMin=-nMin;
	CTimeSpan ts;
	CTime t=0,t1=0,t2=0;
	BOOL bTable=TRUE;
	BOOL bHasLength=TRUE;
	RETCODE bResult;
	wchar_t szCmd[COMMAND_LEN];

	bool isUnicode = false; 
	
	int nFileTable=CheckFileInTable (szTeleCode,szJpgFile ,szLastTime);

	if(nFileTable == ACCESSDBERROR)
	{
		WriteStringEx(L"error:ODBC ERROR");
		return;
	}
	else if(nFileTable==FILEEXISTED)
	{
		WriteStringEx(L"error:FILE EXISTED");
		return;	
	}
	_wsplitpath ( szSourthpath , drive , dir , fname , ext );
	PhotoExt=(CString)ext;
	PhotoExt=PhotoExt.Right(PhotoExt.GetLength()-1);
	try
	{
		CString strJpgFile = szJpgFile;
		en_ACCESSDBRET ret = IsDummyPhoto(szSourthpath, strJpgFile);
		if(ret == SUCCESS_TRUE)
		{
		}
		else if(ret == ACCESSDBERROR)
		{
			WriteStringEx(L"error:ODBC ERROR");
			return;
		}
		else
		{
			ODBCAllocStmt();
			//ODBCExecCmdW(L"delete from %s..wires_photo_src where photo_src_path = N'%s'and year(photo_time) = year(N'%s')"
			//		L"and month(photo_time) = month(N'%s') and day(photo_time) = day(N'%s') and small_path = N'DummyPath'", m_szDatabase, szSourthpath, szPhotoTime, szPhotoTime, szPhotoTime);
		
			swprintf(szCmd, L"Insert into %s..wires_photo_src(tele_code,photo_name,photo_time,import_time,photo_title,photo_size,photo_format,"
				L"photo_location,photo_flag,photo_begin_time,agency_id,agency_flag,photo_path,photo_height,photo_width,bits_pixel,mid_path,small_path,photo_src_path,last_modify_time) "
				L" values(N'%s', N'%s',CONVERT(Varchar(64),N'%s', 64),CONVERT(Varchar(64),N'%s', 64), N'%s ',%d,N'%s ',N'%s ','N',N'%s',%d,%d,N'%s',%d,%d,%d,N'%s',N'%s',N'%s',N'%s')"			
				,m_szDatabase,szTeleCode,szJpgFile,szPhotoTime,szPhotoTime,L"Error Photo",0,PhotoExt,L"",L"",nAgencyID,nAgencyFlag,szServerFile,0,0,0,szMidPath,szSmallPath,szSourthpath,szLastTime);
						bResult=SingleCmd(szCmd);
			/*bResult=ODBCExecCmdW( L"Insert into %s..wires_photo_src(tele_code,photo_name,photo_time,import_time,photo_title,photo_size,photo_format,"
				L"photo_location,photo_flag,photo_begin_time,agency_id,agency_flag,photo_path,photo_height,photo_width,bits_pixel,mid_path,small_path,photo_src_path,last_modify_time) "
				L" values(N'%s', N'%s',CONVERT(Varchar(64),N'%s', 64),CONVERT(Varchar(64),N'%s', 64), N'%s ',%d,N'%s ',N'%s ','N',N'%s',%d,%d,N'%s',%d,%d,%d,N'%s',N'%s',N'%s',N'%s')"			
				,m_szDatabase,szTeleCode,szJpgFile,szPhotoTime,szPhotoTime,L"Error Photo",0,PhotoExt,L"",L"",nAgencyID,nAgencyFlag,szServerFile,0,0,0,szMidPath,szSmallPath,szSourthpath,szLastTime);*/
			ODBCFreeStmt(SQL_DROP);

			bInserted = true;

		}
	}
	catch (...)
	{
		;
	}


	
	 
}


BOOL CSQLApi::LoadDateProcessImage(CMemCmd *memitem, wchar_t *szTeleCode, int iYear, int iMth, int iDay,int type)
{
	RETCODE retcode;
	SDWORD	cbPhotoName ,cbScrFile,cbFileTime,cbPhoto_src_path;
//	int bState = 0;
	CString sql;
	LOGDISTRIBUTEDITEM item;
	ODBCAllocStmt ();
	int i=0;
/*	retcode = ODBCExecCmd( "SELECT REPLACE(REVERSE(SUBSTRING(REVERSE(RTRIM(b.photopath)), 1, CHARINDEX('\', REVERSE(RTRIM(b.photopath))))), '\', ''), a.tele_code "
						   " FROM wires_photo_src AS a INNER JOIN "
						   " photo_log AS b ON a.photo_name = b.photocode "
						   " WHERE (DATEPART(month, a.photo_time) = %ld) "
						   " AND (DATEPART(year, a.photo_time) = %ld) "
						   " AND (DATEPART(day, a.photo_time) = %ld) "
						   " AND (a.tele_code = '%s') "
						   " order by photo_time " , iMth, iYear, iDay,  szTeleCode);*/
	if(type==1){
		retcode = ODBCExecCmdW( L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' "
		L"and DATEPART(month, real_time) = %ld and DATEPART(year, real_time) = %ld and DATEPART(day, real_time) = %ld  " , szTeleCode,iMth, iYear, iDay);
// 		sql.Format(L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' "
// 		L"and DATEPART(month, real_time) = %ld and DATEPART(year, real_time) = %ld and DATEPART(day, real_time) = %ld  " , szTeleCode,iMth, iYear, iDay);
// 		AfxMessageBox(sql);
	}
	else if(type==2){
		retcode = ODBCExecCmdW( L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' "
			L"and DATEPART(month, real_time) = %ld and DATEPART(year, real_time) = %ld " , szTeleCode,iMth, iYear);
// 		sql.Format(L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' "
// 			L"and DATEPART(month, real_time) = %ld and DATEPART(year, real_time) = %ld " , szTeleCode,iMth, iYear);
// 		AfxMessageBox(sql);
	}
	else if(type==3){
		retcode = ODBCExecCmdW( L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' "
			L"and DATEPART(year, real_time) = %ld " , szTeleCode, iYear);
// 		sql.Format( L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' "
// 			L"and DATEPART(year, real_time) = %ld " , szTeleCode, iYear);
// 		AfxMessageBox(sql);
	}
	else if(type==4){
		retcode = ODBCExecCmdW( L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' ", szTeleCode);
// 		sql.Format( L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' ", szTeleCode);
// 		AfxMessageBox(sql);
	}
	/* Bind columns 1, 2, and 3 */
	ODBCBind(1, SQL_C_WCHAR, item.szFile, NAME_LEN, &cbPhotoName);
 	ODBCBind(2, SQL_C_WCHAR, item.szScrFile, PATH_LENGTH1, &cbScrFile);
 	ODBCBind(3, SQL_C_TCHAR, item.szFileTime, 60, &cbFileTime);
 	ODBCBind(4, SQL_C_WCHAR, item.szRealSrcFile, PATH_LENGTH1, &cbPhoto_src_path);
	while (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
	{
		retcode = ODBCFetch();
		if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
		{
			i++;
			ChangeSpaceZeroW ( item.szFile , cbPhotoName ) ;
			ChangeSpaceZeroW ( item.szScrFile , cbScrFile ) ;
			ChangeSpaceZeroW ( item.szFileTime , cbFileTime ) ;
			ChangeSpaceZeroW ( item.szRealSrcFile , cbPhoto_src_path ) ;
		//	AfxMessageBox( item.szScrFile);
			item.lNumber = 0;
			lstrcpy( item.szDstFile, szTeleCode );
			wchar_t szNote[NAME_LEN] = L"";
			lstrcpy( item.szNote, szNote );
			wchar_t szScrFile[PATH_LENGTH1] = L"";
			lstrcpy( item.szScrFile, szScrFile );
			memitem->InsertData ( &item , sizeof ( LOGDISTRIBUTEDITEM));
		}
		else
		{
			retcode = ODBCMoreResults();

			if(retcode==SQL_ERROR)
			{
				int nError=DecideError(FALSE);
				if(nError==DATESOURCE_RESTORE)
				{
					retcode = ODBCMoreResults();
				}
				else if(nError==ODBC_DBERROR)
					return FALSE;
			}

		}
	}
	ODBCFreeStmt(SQL_DROP);
	return TRUE;	
}

//Edit by Leon Li 2013-02-27 for enhancement: remove dat file and check photo record in memeory
BOOL CSQLApi::LoadDateProcessMedia(AGENTDISTFILEARR *DistMediaArr, CString szTeleCode, int iYear, int iMth, int iDay,int type)
{
	RETCODE retcode;
	SDWORD	cbPhotoName ,cbScrFile,cbFileTime,cbPhoto_src_path;
	//	int bState = 0;
	CString sql;
	CString strTemp;
	DISTFILEITEM item;
	ODBCAllocStmt ();
	wchar_t szMediaName[WIRES_TABLE_NAME_LENGTH];
	wchar_t szFilePath[DBG_FILE_PATH_LENGTH];
	wchar_t szFileTime[SYSTEM_TIME_LENGTH];
	int nCount=0;
	if(type==1){
		retcode = ODBCExecCmdW( L"select photo_name,photo_src_path,last_modify_time from wires_photo_src where tele_code=N'%s' "
			L"and DATEPART(month, real_time) = %ld and DATEPART(year, real_time) = %ld and DATEPART(day, real_time) = %ld  " , szTeleCode,iMth, iYear, iDay);
		sql.Format(L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' "
			L"and DATEPART(month, real_time) = %ld and DATEPART(year, real_time) = %ld and DATEPART(day, real_time) = %ld  " , szTeleCode,iMth, iYear, iDay);
		// 		AfxMessageBox(sql);
	}
	else if(type==2){
		retcode = ODBCExecCmdW( L"select photo_name,photo_src_path,last_modify_time from wires_photo_src where tele_code=N'%s' "
			L"and DATEPART(month, real_time) = %ld and DATEPART(year, real_time) = %ld " , szTeleCode,iMth, iYear);
		// 		sql.Format(L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' "
		// 			L"and DATEPART(month, real_time) = %ld and DATEPART(year, real_time) = %ld " , szTeleCode,iMth, iYear);
		// 		AfxMessageBox(sql);
	}
	else if(type==3){
		retcode = ODBCExecCmdW( L"select photo_name,photo_src_path,last_modify_time from wires_photo_src where tele_code=N'%s' "
			L"and DATEPART(year, real_time) = %ld " , szTeleCode, iYear);
		// 		sql.Format( L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' "
		// 			L"and DATEPART(year, real_time) = %ld " , szTeleCode, iYear);
		// 		AfxMessageBox(sql);
	}
	else if(type==4){
		retcode = ODBCExecCmdW( L"select photo_name,photo_src_path,last_modify_time from wires_photo_src where tele_code=N'%s' ", szTeleCode);
		// 		sql.Format( L"select photo_name,photo_path,last_modify_time,photo_src_path from wires_photo_src where tele_code=N'%s' ", szTeleCode);
		// 		AfxMessageBox(sql);
	}
	/* Bind columns 1, 2, and 3 */
	ODBCBind(1, SQL_C_WCHAR, szMediaName, WIRES_TABLE_NAME_LENGTH, &cbPhotoName);
	ODBCBind(2, SQL_C_WCHAR, szFilePath, DBG_FILE_PATH_LENGTH, &cbScrFile);
	ODBCBind(3, SQL_C_TCHAR, szFileTime, SYSTEM_TIME_LENGTH, &cbFileTime);
	while (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
	{
		retcode = ODBCFetch();
		if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
		{
			ChangeSpaceZeroW ( szMediaName, cbPhotoName ) ;
			ChangeSpaceZeroW ( szFilePath , cbScrFile ) ;
			ChangeSpaceZeroW ( szFileTime , cbFileTime ) ;
			item.strMediaName=szMediaName;
			item.strFilePath=szFilePath;
			strTemp=szFileTime;
			item.strFileTime=strTemp.Mid(0,19);
			DistMediaArr->MediaDistArr.Add(item);
			nCount++;
		}
		else
		{
			retcode = ODBCMoreResults();

			if(retcode==SQL_ERROR)
			{
				int nError=DecideError(FALSE);
				if(nError==DATESOURCE_RESTORE)
				{
					retcode = ODBCMoreResults();
				}
				else if(nError==ODBC_DBERROR)
					return FALSE;
			}

		}
	}
	ODBCFreeStmt(SQL_DROP);
	DistMediaArr->nMediaCount=nCount;
	return TRUE;	
}
//End Leon Li 2013-02-27

CString CSQLApi::GetTeleTitle( CString lpszFile, int nTitleLine)
{
	CString aString, str_Temp;
	CFile fil;
	int pos = 0;
	if ( fil.Open( lpszFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) == 0 )
	{		
		return L"" ;
	}
	WORD buffer;
	int bufferCount = 0;
	aString.Empty();
	int  bytelen = 0;
	int i_Counter = 0;
	WORD wSignature;
	fil.Read(&wSignature, 2);
	if(wSignature == 0xBBEF || wSignature ==0xBBBF || wSignature ==0xBBEFBF)
	{
		fil.Seek(3,CFile::begin);
		int nFileLen = (int)fil.GetLength()-3;
		char* szDbsBuffer = new char[nFileLen+1];
		ZeroMemory(szDbsBuffer, sizeof(char)*(nFileLen+1));
		int nRead = (int)fil.Read(szDbsBuffer, nFileLen);
		ASSERT(nRead == nFileLen);

		int nMBLen = MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, NULL, NULL);
		WCHAR* szWcsBuffer = new WCHAR[nMBLen+1];
		ZeroMemory(szWcsBuffer, sizeof(WCHAR)*(nMBLen+1));
		MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, szWcsBuffer, nMBLen);
		aString = szWcsBuffer;
		aString  = aString.Right(nFileLen - 1);

		CString str_Symbol = L"\r\n";
		int iPrevious = 0, iCurrent;
		while(true)
		{
			iCurrent = aString.Find(str_Symbol);
			if (iCurrent == -1)
					iCurrent = aString.GetLength();
			if(iCurrent!=iPrevious||i_Counter!= nTitleLine)
			{
				i_Counter++;
				if(i_Counter== nTitleLine)
				{
					aString = aString.Left(iCurrent);
					break;
				}
				else
				{
					iPrevious = iCurrent;
					aString = aString.Right(aString.GetLength()-iCurrent-1);
				}
			}else

				break;
		}
		delete[]szDbsBuffer;
		delete[]szWcsBuffer;
	}
	else
	{
		fil.SeekToBegin();
		wSignature = 0;
		fil.Read(&wSignature, 2);
		if(wSignature == 0xFEFF || wSignature == 0xFFFE )
		{
			while ((bytelen=fil.Read(&buffer,sizeof(WORD)))!= 0)
			{
				if (buffer == 0x0D)
				{
					bufferCount++;
					i_Counter ++;
					int len = fil.Read(&buffer,sizeof(WORD));
					if (len&&buffer == 0x0A)
					{
						pos = fil.GetPosition();
					}
					if(i_Counter != nTitleLine)
					{
						aString.Replace(L" ", L"");
						aString.Replace(L"\r\n",L""); 	
						aString.Replace(L"\t",L""); 
						aString = L"";
					}
					else
					{
						str_Temp = aString;
						str_Temp.Replace(L" ", L"");
						str_Temp.Replace(L"\r\n",L""); 	
						str_Temp.Replace(L"\t",L""); 
						break;
					}
				}
				else
				{
					if(buffer == 65279)
						continue;
					aString += CString((WCHAR)buffer);
				} 
			}
			pos = fil.GetPosition();
		}
		else
		{
			fil.SeekToBegin();
			wSignature = 0;
			char buffer[8192];
			UINT unRead;
    
			unRead = fil.Read(buffer, sizeof(buffer)-1);
			while(unRead)
			{
			/*	if(i_Counter != nTitleLine)
				{
					aString.Replace(L" ", L"");
					aString.Replace(L"\r\n",L""); 	
					aString.Replace(L"\t",L""); 
					nNonContentWordCount += aString.GetLength();
					aString = L"";
				}
				else
				{
					str_Temp = aString;
					str_Temp.Replace(L" ", L"");
					str_Temp.Replace(L"\r\n",L""); 	
					str_Temp.Replace(L"\t",L""); 
					nNonContentWordCount += str_Temp.GetLength();
					break;
				}
				i_Counter++;*/
				buffer[unRead] = '\0';
				TRACE(_T("%s"),buffer);
				aString += buffer;
				aString += _T("\n");
				unRead = fil.Read(buffer, sizeof(buffer)-1);
			}
			CString str_Symbol = L"\r\n";
			int iPrevious = 0, iCurrent;
			while(true)
			{
				iCurrent = aString.Find(str_Symbol);
				if (iCurrent == -1)
					iCurrent = aString.GetLength();
				if(iCurrent!=iPrevious||i_Counter!= nTitleLine)
				{
					i_Counter++;
					if(i_Counter== nTitleLine)
					{
						aString = aString.Left(iCurrent);
						break;
					}
					else
					{
						iPrevious = iCurrent;
						aString = aString.Right(aString.GetLength()-iCurrent-1);
					}
				}else

					break;
			}
		}
	}
	if(aString.GetLength()>=TITLE_LEN1)
		aString = aString.Left(TITLE_LEN1-1);

	aString.Replace(L"'",L"''");
	aString.Replace(L"%",L"%%");
	return aString;
}

CString CSQLApi::HandleUnicodeContent(CString lpszFile)
{
	CFile fil;
	CString aString = L"";
	int pos = 0;
	if ( fil.Open( lpszFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) == 0 )
	{		
		return L"" ;
	}
	WORD buffer;
	int bufferCount = 0;
	aString.Empty();
	int  bytelen = 0;
	WORD wSignature;
	fil.Read(&wSignature, 3);
	if(wSignature == 0xBBEF || wSignature ==0xBBBF || wSignature ==0xBBEFBF )
	{
		fil.Seek(3,CFile::begin);
		int nFileLen = (int)fil.GetLength()-3;
		char* szDbsBuffer = new char[nFileLen+1];
		ZeroMemory(szDbsBuffer, sizeof(char)*(nFileLen+1));
		int nRead = (int)fil.Read(szDbsBuffer, nFileLen);
		ASSERT(nRead == nFileLen);

		int nMBLen = MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, NULL, NULL);
		WCHAR* szWcsBuffer = new WCHAR[nMBLen+1];
		ZeroMemory(szWcsBuffer, sizeof(WCHAR)*(nMBLen+1));
		MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, szWcsBuffer, nMBLen);
		aString = szWcsBuffer;
		aString  = aString.Right(nFileLen - 1);

		delete []szDbsBuffer;
		delete []szWcsBuffer;
	}
	else
	{
		fil.SeekToBegin();
		fil.Read(&wSignature, 2);
		if(wSignature == 0xFEFF || wSignature == 0xFFFE )
		{
			fil.SeekToBegin();
			while ((bytelen=fil.Read(&buffer,sizeof(WORD)))!= 0)
			{
					if(buffer == 65279)
						continue;
					aString += CString((WCHAR)buffer);
			} 
		}
		else
		{
 			fil.SeekToBegin();
			char buffer[8192];
			UINT unRead;
    
			unRead = fil.Read(buffer, sizeof(buffer)-1);
			while(unRead)
			{
				buffer[unRead] = '\0'; 
				TRACE(_T("%s"),buffer);
				aString += (CString)buffer;
				aString += _T("\n");
				unRead = fil.Read(buffer, sizeof(buffer)-1);
			}
		}
	}
	aString.Replace(L"%",L"%%");
	return aString;
}

en_ACCESSDBRET CSQLApi::InsertMediaValues (CString strLibCode,int nLibID,CString strMediaTime,CString strMediaPath,
	CString strSrcPath,CString strMediaFolder,CString strMediaExt,CString strFileCode,CString strLastModTime,CString strSmlPhotoPath,CString strMidPhotoPath,int nFileLength,CString strCaption,int nTitleLine,int nMediaRoot,int nAgencyType)
{
	CString strCurTime;
//	CADORecordset rs(m_pDB);
	CString sql;
	
	int nFileTable=CheckFileInTable (strLibCode,strFileCode,strLastModTime);
	CTime tCurTime=CTime::GetCurrentTime();
	strCurTime.Format(L"%d-%d-%d %d:%d:%d",tCurTime.GetYear(),tCurTime.GetMonth(),tCurTime.GetDay(),tCurTime.GetHour(),tCurTime.GetMinute(),tCurTime.GetSecond());
	
	strCaption.Replace(L"'",L"''");
	strMediaPath.Replace(L"'",L"''");
	strSmlPhotoPath.Replace(L"'",L"''");
	strMidPhotoPath.Replace(L"'",L"''");
	strSrcPath.Replace(L"'",L"''");
	strMediaFolder.Replace(L"'",L"''");
	if(nFileTable == ACCESSDBERROR)
	{
		WriteStringEx(L"error:ODBC ERROR");
		return ACCESSDBERROR;
	}
	else if(nFileTable==FILEEXISTED)
	{
		WriteStringEx(L"error:FILE EXISTED");
		return TEXTFAIL;	
	}
	try
	{
		//Execute UpdateWiresPhotoSrc N'%s', N'%s',N'%s',N'%s', N'%s ',%d,N'%s ',N'%s ','N',N'%s',%d,%d,N'%s',%d,%d,%d,N'%s',N'%s',N'%s',N'%s',N'%s',%d,%d,%d,%d,%d"			
		//	,szTeleCode,szJpgFile,szPhotoTime,szPhotoTime,szTitle,nSize,szExt,szTeleAttrL,szTime,nAgencyID,nAgencyFlag,szServerFile,pInfo->nHeight,pInfo->nWidth,pInfo->nBits,szMidPath,szSmallPath,szSourthpath,szLastTime,curTime,DBG_TYPE_INT_PIC,nSrcRoot,nSmlRoot,nMidRoot);
	//		(tele_code,photo_name,photo_time,import_time,photo_title,photo_size,photo_format,"
	//		L"photo_location,photo_flag,photo_begin_time,agency_id,agency_flag,photo_path,photo_height,photo_width,bits_pixel,mid_path,small_path,photo_src_path,last_modify_time,photo_folder,media_type,photo_caption) "
		//	L" values(N'%s', N'%s',CONVERT(Varchar(64),N'%s', 64),CONVERT(Varchar(64),N'%s', 64), N'%s',%d,N'%s',N'%s','N',N'%s',%d,%d,N'%s',%d,%d,%d,N'%s',N'%s',N'%s',N'%s',N'%s',%d,N'%s')"			
		sql.Format(L"Execute UpdateWiresPhotoSrc N'%s', N'%s',N'%s',N'%s', N'%s ',%d,N'%s ',N'%s ','N',N'%s',%d,%d,N'%s',%d,%d,%d,N'%s',N'%s',N'%s',N'%s',N'%s',%d,%d,%d,%d,N'%s'"
			,strLibCode,strFileCode,strMediaTime,strMediaTime,L"",nFileLength,strMediaExt,L"",L"",nLibID,nAgencyType,strMediaPath,0,0,0,strMidPhotoPath,strSmlPhotoPath,strSrcPath,strLastModTime,strCurTime,DBG_TYPE_INT_AV,nMediaRoot,nMediaRoot,nMediaRoot,strMediaFolder);
		//m_pView->DisplayTraceMessage(sql,0,FALSE);
		wchar_t *pwcharMessageBody = T2W(sql.GetBuffer()); 
		SingleCmd(pwcharMessageBody);
		sql.ReleaseBuffer();
	}
	catch (...)
	{
		return FAILED_FALSE;
	}
	UpdateMediaCaption(strFileCode,strCaption,nTitleLine);
	return SUCCESS_TRUE;
}

en_ACCESSDBRET CSQLApi::InsertMediaInfo(CString strFileCode,CString strEntryType,CString strEntryName,CString strEntryValue){
	CString strCurTime;
//	CADORecordset rs(m_pDB);
	
	try
	{
		CString sql;
		strEntryValue = strEntryValue.Left(500);
		strEntryValue.Replace(L"'",L"''");
		sql.Format(L"INSERT INTO dbmedia_av_info(photo_name,type,entry,value) VALUES(N'%s',N'%s',N'%s',N'%s')",
			strFileCode,strEntryType,strEntryName,strEntryValue);
		//m_pView->DisplayTraceMessage(sql,0,FALSE);
		wchar_t *pwcharMessageBody = T2W(sql.GetBuffer()); 
		SingleCmd(pwcharMessageBody);
		sql.ReleaseBuffer();
	}
	catch (...)
	{
		return FAILED_FALSE;
	}
	return SUCCESS_TRUE;
}


en_ACCESSDBRET CSQLApi::UpdateMediaCaption (CString strFileCode,CString strCaption,int nTitleLine)
{
	CString sql;
	CString szTitle=L"";
	/*long lWord=0,lDelay=0;
	LPBYTE hpBuffer;
	DWORD dwLength;
	HGLOBAL hglbFil;*/

	if (strCaption.IsEmpty())
		return FAILED_FALSE;

	/*dwLength=strCaption.GetLength();
	const wchar_t *szMediaCaption=strCaption;
	if ( (hglbFil = GlobalAlloc ( GHND , dwLength + 30) ) == NULL )
	{
		return TEXTFAIL;
	}
	hpBuffer=(LPBYTE)szMediaCaption;
	szTitle= GetTeleTitleWordW ( hpBuffer , dwLength , 1 ,  &lWord , &lDelay) ;*/
	int iPrevious = 0, i_Counter = 0,iCurrent;
	CString aString = strCaption;
	while(true)
	{
		iCurrent = aString.Find(L"\r\n");
		if (iCurrent == -1)
			iCurrent = aString.GetLength();
		if(iCurrent!=iPrevious||i_Counter!= nTitleLine)
		{
			i_Counter++;
			if(i_Counter== nTitleLine)
			{
				aString = aString.Left(iCurrent);
				break;
			}
			else
			{
				iPrevious = iCurrent;
				aString = aString.Right(aString.GetLength()-iCurrent-1);
			}
		}else

			break;
	}
	try
	{
		sql.Format(L"Execute UpdatePhotoCaption N'%s', N'%s',N'%s'",strCaption,szTitle,strFileCode);
		wchar_t *pwcharMessageBody = T2W(sql.GetBuffer()); 
		SingleCmd(pwcharMessageBody);
		sql.ReleaseBuffer();
		sql.Format(L"update wires_photo_src set media_type = 1 where photo_name = N'%s'",strFileCode);
		pwcharMessageBody = T2W(sql.GetBuffer()); 
		SingleCmd(pwcharMessageBody);
		sql.ReleaseBuffer();
	}
	catch (...)
	{
		return FAILED_FALSE;
	}
	return SUCCESS_TRUE;
}

en_ACCESSDBRET CSQLApi::GetDummyPhotoPath(CString &strFilePath,int nID){
//	CADORecordset rs(m_pDB);
	CString sql;
	wchar_t szBuffer[512]={0};
	BOOL bFil = FALSE ;
	sql.Format(L"select rootpath_id,rootpath from rootpath where bDummy=1");
	m_pView->DisplayTraceMessage(sql,0,FALSE);
//	if(rs.Open(sql, CADORecordset::openQuery))
	{
//		if(!rs.IsEOF())
		{
//			rs.GetFieldValue(0,nID);
//			rs.GetFieldValue(1,strFilePath);
			bFil = TRUE;
		}
		//else
		{
			return FAILED_FALSE;
		}	
//		rs.Close();
	}		
//	else
	{
		return FAILED_FALSE;
	}
	strFilePath=szBuffer; 
	if (bFil)
		return SUCCESS_TRUE;
	else return FAILED_FALSE;
}

en_ACCESSDBRET CSQLApi::GetDefualtSmptInfo(SendEmailParam *EmailParam){
	wchar_t szBuffer[512]={0};
	BOOL bFil = FALSE ;
	CString sql, str;
//	CADORecordset rs(m_pDB);
	try{
		sql = (L"select smtp_server_add,port,protocol_type \
			FROM smtp_server_list where is_default=1");
		m_pView->DisplayTraceMessage(sql,0,FALSE);
//		if (rs.Open(sql, CADORecordset::openQuery))
		{
///			if(!rs.IsEOF())
			{
				bFil = TRUE;
//				rs.GetFieldValue(0, str);
//				rs.GetFieldValue(1, EmailParam->nSmtpPort);
//				rs.GetFieldValue(2, EmailParam->nSecureType);
			}
//			rs.Close();
		}
	}
	catch(...){
		return FAILED_FALSE;
	}
	EmailParam->strSmtpServer=str;
	GetConfigure(_T("SmtpEmailName"), szBuffer);
	EmailParam->strUserName=szBuffer;
	EmailParam->strFromAdd=szBuffer;
	GetConfigure(_T("SmtpEmailPwd"),szBuffer);
	EmailParam->strPassWord=szBuffer;
	if (bFil)
		return SUCCESS_TRUE;
	else return FAILED_FALSE;
}

en_ACCESSDBRET CSQLApi::IsDummyPhoto(CString szFile, CString &szJpgFile)
{
	en_ACCESSDBRET enRet = ACCESSDBERROR;
	if(m_bDbFail)
	{
		if(!LoginDb(TRUE))
			return enRet;
	}
	wchar_t   szCode[512];
	RETCODE retcode;
	SDWORD cbCode;
	BOOL bFil = FALSE ;
	ODBCAllocStmt ();
	retcode = ODBCExecCmdW(L"select photo_name from %s..wires_photo_src where photo_src_path = N'%s' and small_path = N'DummyPath'" ,m_szDatabase , szFile);
	ODBCBind(1, SQL_C_WCHAR, szCode, 512, &cbCode);
	if(!ODBCSQLERROR(retcode))
	{
		while (retcode != SQL_NO_DATA_FOUND) 
		{
			retcode = ODBCFetch();
			if (retcode == SQL_SUCCESS || retcode== SQL_SUCCESS_WITH_INFO) 
			{
				ChangeSpaceZeroW ( szCode , cbCode );
				szJpgFile = szCode;
				bFil = TRUE;
			}
			else
				retcode = ODBCMoreResults();
		}
	}
	else
	{
		return enRet;
	}
	ODBCFreeStmt(SQL_DROP);
	if(!bFil)
		enRet = FAILED_FALSE;
	else
		enRet = SUCCESS_TRUE;
	return enRet;
}
//End Leon Li 2013-02-27