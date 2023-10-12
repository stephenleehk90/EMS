#pragma once
#include "Convert.h"
#include "afxwin.h"
#include "StdioFileEx.h"

#define ONEDAY_TIME 86400
#define         CHILD_WINDOW_TIMER                              2
// CDBGraphicMain dialog
class CStdioFileEx;

class CDBGraphicMain : public CDialog
{
	DECLARE_DYNAMIC(CDBGraphicMain)

public:
	CDBGraphicMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDBGraphicMain();

	static int m_Lock;
	static DWORD m_Tick;
	DISTFILESTRUCT  *db_config;
	CString m_code;
	CString m_szMediaCode;
	CString m_strMediaPath;
	CString m_strMediaExt;
	CString m_strFileEx;
	CString m_strPhotoEx;
	CString m_strPhotoPath;
	CString m_strFilePath,m_strExt;
	CString m_strTargetPathO;
	CString m_strTargetPathM;
	CString m_strTargetPathS;
	CString m_errPath,m_szFullName;
	CString m_strLocalSourceFile;
	CStringArray m_fileName,m_errAgency;
	BOOL m_bStart;
	wchar_t	m_szBeginTime[32];
	int m_checkYsd,m_nCount;
	CTime m_startTime;
	int m_interval,m_display,m_dpl;
	ADISTFILESTRUCT m_hpdfsDistFile;
	HGLOBAL			m_hglbDistFile;
	BOOL m_bCopyFileSuccess;
	int m_nDayOffset,m_MaxTm,m_MaxCopy,m_bExit,num_copy,m_errCount,m_TryTime,m_Eml,m_ProcessTime;
	unsigned long   m_ulTransFileNum,m_AllTransFileNum;
	BOOL			m_bDatFileOpened,m_bAllFileOpened,m_bLogFileOpened;
	CFile			m_fileDat , m_fileLog,m_fileAll;
	CString 			m_szSourcePath;
	CString			m_szTxtSrcPath;
	CString			m_szBmpSrcPath;
	CString			m_szDesPath;
	CString			m_szPhotoDesPath;
	CString			m_szServerFile;	//圖片存于server的路徑
	MSXML::IXMLDOMDocumentPtr m_plDomDocument;  // Michael 22-11-23
	MSXML::IXMLDOMDocumentPtr m_plDomDocumentPre; // the previous version xml file.
	MSXML::IXMLDOMDocumentPtr m_plDomDocumentCheck; // For Checking use
	MSXML::IXMLDOMDocumentPtr docPtr;  // For CData
	MSXML::IXMLDOMElementPtr m_pDocRoot;     // Michael 22-11-23
	long			m_lTotalCheck;
	long			m_lCount;
	CArray<LOGDISTRIBUTEDITEM, LOGDISTRIBUTEDITEM&> m_arrDistributedFile;
	CArray<ERRORFILE, ERRORFILE&> fileList;
// Edit Stephen 2021-08-25 
	CArray<DUMMYTHUMBNAILFILE, DUMMYTHUMBNAILFILE&> dummyThumbnailList;
// Edit Stephen 2021-08-25 
	CConvertDlg m_ImageDlg;
	CConvertDlg *ImageDlg;
	BOOL bCreate;
	int m_nHeight,m_nWidth,m_exist,m_existLast[50],m_totalFile,m_total;
	CTime			RealTime;
	CMemCmd			memitem;
	UINT			m_uTimer, m_uTimeCheck;
	int				m_nCol, m_nRow,m_disFile,m_totalFileExt[50],m_totalPrs,m_totalrm;//,m_sleep;
	int				m_nCharHeight, m_nCharWidth,m_errorFile;
	int             m_nMaxRow, m_nMaxCol,m_newLog,m_skipped;
	int				m_errSrvCount;
	CFont			m_fontWnd;
	BOOL			m_bCreateWndFont;
	BOOL			m_bScrollWindow;
	unsigned long   m_ulWndBufStart, m_ulWndBufEnd;
	HGLOBAL			m_hglbWndBuffer;
	CSQLApi m_sqlDb;
	int m_nMidHeight,m_invalidFile[50];
	int m_nMidWidth;
	BOOL m_bDBGYesterday;
	friend class CConvertDlg;
	CMemCmd			memitem_temp;
	CString m_strDbgStartTime;
	/**Send Email **/
	SendEmailParam *m_EmailParam;
	//**Define for display message**//
	CString m_strDisplayMessage;
	CString m_strDisplayMessageHeader;
	wchar_t m_szDisplayMessage[MAX_MESSAGE_LENGTH];
	//**End message define**//
	//Edit by Leon Li 2013-02-27 for enhancement: remove dat file and check photo record in memeory
	AGENTDISTFILEARR m_AgentDistMedia;
	//End Leon Li 2013-02-27
	BOOL bIsFirstTime;
	BOOL bIsStartUp;
	BOOL bIsCancel;
// Dialog Data
	BOOL bFlagStop;
	CString m_strMediaCaption;

// Edit Stephen  2014-01-06
// For convert EPD to JPG temp file
	CString m_strConvertEPSPath;
// Edit Stephen  2014-01-06

	CString m_strLog;

	enum { IDD = IDD_DBGRAPHIC_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL ReadReutersXML(CString &strCaption, CString strFileName);

private:
	CString m_strFailedCopyLog;
	CFile m_hDisMessage;
	CStdioFileEx *m_classFileEx;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStop();
	afx_msg void OnStart();
	afx_msg void OnContinue();
	afx_msg void OnClear();
	afx_msg void OnExit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnInsertitemMsg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT UpdateUIStatus(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT UpdateMainFrameMessage(WPARAM wParam, LPARAM lParam);
	CListCtrl m_msg;
	CString m_dbInfo;
	void DistributePhoto(int num);
	BOOL LoadConfig();
	void DisplayMsg(CString db_code, CString db_picSource);
	void CheckTargetDate();
	void InitCheck();
	void UpdateXMLFile();
	void DisplayMessage(LPCWSTR lpszMsg, int display);
	CString GetSrcPath(int addday,int * iIncludeDay);
	CString GetTextPath(int addday);
	CString &GetCurTime(CString &strCurTime);
	en_ACCESSDBRET LogTransFile( wchar_t *szFile ,long lCount,int nSize, DISTFILEITEM &logItem,int iDay);
	void WriteTransLogFile(const LOGDISTRIBUTEDITEM &logItem);
	CTime GetConvertTime();
	BOOL CheckLogInXML(wchar_t *szFile) /* if the file has been output, return true. */;
	CString GetServerPath(int addday);
	void XMLFileNodeGen(wchar_t *szFile , MSXML::IXMLDOMElementPtr pDocElementAgency,int nSize, int iDay);
	void DoWithMessage();
	BOOL CompareDate(LPCWSTR szFile,wchar_t *szDate);
	void DisplayErrorMessage(LPCWSTR lpszMsg, LPCWSTR lpszStation = NULL,int display=0);
	BOOL NeedCopy();
	CString GetCaption();
	en_ACCESSDBRET TransFile(CString szSourthPath,PhotoInfo *pInfo, CString szFile,CString szBmpFile, CString szJpgFile, CString szMidPath, CString szSmallPath,int lSize,int addday,CString szPhotoTime,CString szLastTime);
	void CreateLog();
	int ComputerFileNum(CString strFolderName);
	int ComputerSkpFile(CString strFolderName);
	bool ChooseByUser(CString code);
	CString &GetCurDate(CString &strCurDate);
	CString &GetTime(CString &strTime);
	void CreateErrFile(wchar_t * filePath);
	BOOL CheckErrFile();
	// Edit Stephen 2021-08-25
	BOOL NotifyDummyThumbnail();
	// Edit Stephen 2021-08-25
	void WriteErrFile(int num);
	void loadErrInfo();
	BOOL checkErrTable(CString photoPath);
	BOOL timecmp(int num);
	void SendEmail();
	void AddItem(LPCWSTR time, LPCWSTR msg);
	void checkPhotoName(CString &fileName,CString Ext,CString &szFile,int type);
	BOOL TimeToRead();
	void copyPhoto(LPCWSTR lpExistingFileName,LPCTSTR lpNewFileName);
	void uploadDummy(int num);
	int accessPath(CString szSmlPath,CString szMidPath);
	BOOL Load_Data_File();
	int DeleteMyExe();
	virtual BOOL OnInitDialog();
	afx_msg void OnAddagent();
	virtual INT_PTR DoModal();
	void DistributeAV();
	//Edit by Leon Li 2013-02-27 for enhancement: remove dat file and check photo record in memeory
	BOOL LoadDistFileHistory();
	void InitProcessParam();
	//End Leon Li 2013-02-28
	//Edit by Leon Li 2013-02-28 for enhancement: multi thread for handling dbgraphic,handling log file size
	CWinThread* m_pThread;
	static UINT ThreadFunc(LPVOID lpParam); 
	void LoopAgency();
	threadInfo m_tInfo;
	wchar_t m_wLogNum;
	//End Leon Li 2013-02-28.
	int m_iLogNum;
	int m_iTraceLogNum;	
	int m_iTraceLog;
	// Edit Stephen 2021-12-23
	int m_iEmailEnable;
	// Edit Stephen 2021-12-23
	CString m_strStarttime;
	//Test//
	void DisplayTraceMessage(LPCWSTR lpszMsg, int display,BOOL bTime=FALSE,BOOL bNew=FALSE);
	void InitEmailParam();
	void UpdateMainFrameMessage(CString strMessageHeader,CString strMessageContent);
	CString GetPath(CString strFormat,int checkYsd=0);
	int IsCaptionText(int code);
	BOOL GetPhotoCaption(CString strFilePath,CString &strPhotoCaption,CString &strAddCaption);
	BOOL GetFileCaption(CString strFilePath,CString &strCaption,int &iType);
	void DistributeAV(int nCurNum);
	int ComputeTotalFile(CString strFilePath,CString strFileExt);
	BOOL LoadAVMetaData(CString strAVPath,CString strPhotoName);
	void CheckAVName(CString strFilePath,CString &strFileName,CString &strFolderName);
	CStatic m_stcProcessingFile;
	CStatic m_stcProcessAgency;
	CStatic m_stcTotalChkCount;
	BOOL CopyCaptionPhoto(CString strSrcPath,CString strTgtPath,CString &strSmlPhotoPath,CString strFileName=L"",BOOL bRemove=FALSE/*Determine Copy or remove file*/);//copy caption folder to destination

// Edit Stephen  2014-01-06
	void DeleteTempEPSFile();
// Edit Stephen  2014-01-06

	afx_msg void OnClickedTestMail();
};


void WriteStringEx(UINT nResID);
void WriteStringEx(LPCWSTR szString);
BOOL IsSupportedPic(CString strFile);
BOOL MakeDir(CString strDir);
BOOL CheckPath(wchar_t * szPath,int & bExist,int & bNetPath);
void getYesterday(CString &date);
int GetTotal(CString Feedback,char * test);

BOOL isCaptionPhoto(CString strSrcPhotoName,CString strPhotoName);