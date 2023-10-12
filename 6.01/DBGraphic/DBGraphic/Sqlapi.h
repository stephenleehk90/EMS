// SQLApi.h : header file
//
#include "..\..\trancode\trancode.h"
#define		DBMSWIN
#include "globaldef.h"
#define			SYSADMIN_USER					"sa"
/*
#define			TEXTFAIL						1
#define			BMPFAIL						2
*/
/////////////////////////////////////////////////////////////////////////////
// CSQLApi command target
struct PhotoInfo
{
	int nHeight;
	int nWidth;
	int nBits;
};

enum en_ACCESSDBRET {SUCCESS_TRUE, FAILED_FALSE, ACCESSDBERROR, FILEEXISTED
, BmpFileNoContent, BmpNoContent, FileNoContent, FILENOEXISTED, TEXTFAIL, NOTACCESSPATH,FILEEXISTEDWIHTNOLOG};

class CSQLApi : public CSQLTools
{
	DECLARE_DYNCREATE(CSQLApi)
	
	CSQLApi();           // protected constructor used by dynamic creation

// Attributes
public:
	BOOL GetServerTime(CTime & CurTime);
	BOOL m_open;
	CTransCodeFile tcf;
	CDBGraphicMain *m_pView;
// Operations
public:
	static CString PrepareSQL(LPCTSTR szFormat, ...);
	CString GetNewAgencyFlag( wchar_t * m_szTeleCode);
	int		GetFieldType(wchar_t * szTable, wchar_t *  szColumn);
	en_ACCESSDBRET ImageIsLogged(CString szFile,CString szTime,BOOL bCheckTime);
	en_ACCESSDBRET	IsImageFileExist ( wchar_t  *szFile );
	en_ACCESSDBRET	GetNewImageName(wchar_t  *szFile);
	BOOL 	DispalyDb(CListBox *pList);
	BOOL	LoginDb(BOOL);
	int		DecideError(BOOL bDisp, BOOL bNeedReLogin = FALSE);
	en_ACCESSDBRET	CheckDbteleTable(wchar_t * szTable);
	int     CheckDatabase(wchar_t *szDB,BOOL bDisp);
	BOOL	FetchDB(wchar_t *szDB);
	BOOL    CheckUser(wchar_t *szDataBase);
	BOOL	GetTelegraph( void );
	BOOL    CheckAgency(wchar_t * szAgency);
	BOOL	CheckTablePermission( wchar_t *szTable,int);
	BOOL    CreateTable( wchar_t *szTable ) ;
	int		CheckUser();
	//UINT	InsertValues2 (long type,char *szBmpFile , char *szFile , int nDelayHour,int nMin,char *szJpgFile,int nMarkLine,int nTitleLine,BOOL bTransfer,int nSize,int addday=0);
	en_ACCESSDBRET	InsertValues (PhotoInfo *pInfo, CString szCode,int nAgencyID,int nAgencyFlag
		, CString szBmpFile , CString szFile , CString szMidPath, CString szSmallPath
		,int nDelayHour,int nMin,CString szJpgFile,int nMarkLine,int title_line
		,BOOL bTransferbig5,int nSize,CString szLastTime,CString szSourthpath,CString szPhotoTime,int addday=0, CString szTime=L"", CString szServerFile=L"");
	void	InsertValuesDummy (bool& bInserted, PhotoInfo *pInfo, wchar_t *szCode,int nAgencyID,int nAgencyFlag
		, wchar_t *szBmpFile , wchar_t *szFile , wchar_t *szMidPath, wchar_t *szSmallPath
		,int nDelayHour,int nMin, wchar_t *szJpgFile,int nMarkLine,int title_line
		,BOOL bTransferbig5,int nSize,wchar_t * szLastTime,wchar_t *szSourthpath,wchar_t *szPhotoTime,
		int addday=0, wchar_t * szTime=0, wchar_t *szServerFile=NULL);
	int		CheckFileInTb ( wchar_t *szCode , wchar_t *szFile  );
	en_ACCESSDBRET CheckFileInTable (CString szTeleCode, CString szFile ,CString time);
//	BOOL	UpdateTextItem1( wchar_t *szCmd  , void far *lpText , SDWORD cbData ,int type=0);
	BOOL	UpdateTextItem1( wchar_t *szCmd  , void far *lpText , SDWORD cbData,CString photoPath ,int type=0, bool b_isunicode = true);
	BOOL    UpdateTextItem1_Ansi(char *szCmd  , void far *lpText , SDWORD cbData ,int type);
	BOOL	GetAgencySt(wchar_t *szTeleName, wchar_t *szPhotoDesPath,int * nMarkLine,int *bTransfer,int * nTitleLine);
	int		GetLib1 ( wchar_t *szTitle , wchar_t * lpszLocalPlace );
	int		GetTeleAttrL(wchar_t *szKeyWord, wchar_t *szTeleAttr, wchar_t *szKeyStyle);
	BOOL    FormPhotoPath(wchar_t *,CTime, wchar_t *);
	BOOL    GetConfigure (wchar_t *szEntry , wchar_t *szValue);
	BOOL	GetPhotoPath(wchar_t *szPhotodesPath);
	BOOL    CheckTable();
	BOOL	CheckDB();
	BOOL getSeverTime(CString &severTime);
	BOOL GetConfig (int szNum , wchar_t *szValue);
	int CSQLApi::GetNum ( wchar_t *szValue);
	BOOL LoadDateProcessImage(CMemCmd *memitem, wchar_t *szTeleCode, int iYear, int iMth, int iDay,int type);
	en_ACCESSDBRET IsAVFileExist(wchar_t *szFile);
	en_ACCESSDBRET InsertMediaValues (CString strLibCode,int nLibID,CString strMediaTime,CString strMediaPath, CString strSrcPath,CString strMediaFolder,CString strMediaExt,CString strFileCode,CString strLastModTime,CString strSmlPhotoPath,CString strMidPhotoPath,int nFileLength,CString strCaption,int nTitleLine,int nMediaRoot,int nAgencyType=1);
	en_ACCESSDBRET InsertMediaInfo(CString strFileCode,CString strEntryType,CString strEntryName,CString strEntryValue);
	en_ACCESSDBRET UpdateMediaCaption (CString strFileCode,CString strCaption,int nTitleLine);
	en_ACCESSDBRET GetDummyPhotoPath(CString &strFilePath,int nID);
	en_ACCESSDBRET GetDefualtSmptInfo(SendEmailParam *EmailParam);
	BOOL CSQLApi::LoadDateProcessMedia(AGENTDISTFILEARR *DistMediaArr, CString szTeleCode, int iYear, int iMth, int iDay,int type);
	CString GetTeleTitle( CString lpszFile, int nTitleLine);
	CString HandleUnicodeContent(CString lpszFile);
	en_ACCESSDBRET	IsDummyPhoto (CString szFile, CString &szJpgFile);
	//End Leon Li 2013-02-27
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSQLApi)
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetAgencyID(CString strAgencyCode,int nAgencyFlag);
	void GetDepartment(CComboBox * m_combo);
	virtual ~CSQLApi();
protected:
	// Generated message map functions
	//{{AFX_MSG(CSQLApi)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
		
		
		
	
};

/////////////////////////////////////////////////////////////////////////////
#ifndef	_DBAdmin_CPP_
extern CSQLApi* p_dbCmd;
#endif
