// DBGraphicMain.cpp : implementation file
//

#include "stdafx.h"
#include "DBGraphic.h"
#include "DBGraphicMain.h"
#include "afxdialogex.h"
#include "DBGraphicDlg.h"

#include < shellapi.h>
#include "DBGraphicSelect.h"
#include <io.h>
#include <sys\stat.h>
#include <direct.h>
//#include "..\..\iptc\iptc.h"
#include "Tempapp.h"
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>


// CDBGraphicMain dialog

CDBGraphicMain *p_StatView = NULL;

IMPLEMENT_DYNAMIC(CDBGraphicMain, CDialog)

CDBGraphicMain::CDBGraphicMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDBGraphicMain::IDD, pParent)
{

	m_strFailedCopyLog = _T("");
	m_skipped=0;
	db_config=NULL;
	memset(m_totalFileExt,0,sizeof(m_totalFileExt));
	m_totalPrs=0;
	memset(m_invalidFile,0,sizeof(m_invalidFile));
	m_bExit=0;
	m_checkYsd=0;//for test
	m_Tick=GetTickCount();
	m_lCount=0;
	m_totalrm=0;
	m_errorFile=0;
	m_uTimer = NULL;
	m_TryTime=0;
	m_Eml=0;
	m_newLog=0;
	m_uTimeCheck = 0;
	m_nCol = 0;
	m_nRow = 0;
	m_ulTransFileNum = 0L;
	m_AllTransFileNum=0L;
	m_bCreateWndFont = FALSE;
	m_hglbWndBuffer = NULL;
	m_ulWndBufStart = m_ulWndBufEnd = 0L;
	m_bScrollWindow = FALSE;
	m_bDatFileOpened = FALSE;
	m_bLogFileOpened = FALSE;
	m_bAllFileOpened = FALSE;
	m_szSourcePath=L"";
	m_szTxtSrcPath=L"";
	m_szBmpSrcPath=L"";
	m_szDesPath=L"";
	m_szPhotoDesPath=L"";
	m_errSrvCount=0;
	m_disFile=0;
	m_wLogNum='a';
	//leon
	m_iTraceLogNum=1;
	m_iEmailEnable=0;
	bCreate=FALSE;
	//ImageDlg=new CConvertDlg;
	//ImageDlg=NULL;
	num_copy=0;
	m_totalFile=0;
	m_total=0;
	m_exist=0;
	m_nHeight=0;
	m_nWidth=0;
	m_errCount=0;
	m_bCopyFileSuccess = FALSE;
	m_strFailedCopyLog = _T("");
	m_nMidHeight=300;
	m_nMidWidth = 400;
	m_nDayOffset = 0;
	m_display=0;
	m_dpl=0;
	m_classFileEx=new CStdioFileEx;
	m_EmailParam= new SendEmailParam;

	m_strLog = L"";
	m_ProcessTime=0;
}

CDBGraphicMain::~CDBGraphicMain()
{
	if(m_hDisMessage.m_hFile != CFile::hFileNull)
		m_hDisMessage.Close();
	delete m_EmailParam;
}

void CDBGraphicMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_MSG, m_MSG);
	DDX_Control(pDX, IDC_MSG, m_msg);
	DDX_LBString(pDX, IDC_DBINFO, m_dbInfo);
	DDX_Control(pDX, IDC_PROCESSINGFILENAME, m_stcProcessingFile);
	DDX_Control(pDX, IDC_PROCESSINGAGENCY, m_stcProcessAgency);
	DDX_Control(pDX, IDC_CHECKCOUNT, m_stcTotalChkCount);
}


BEGIN_MESSAGE_MAP(CDBGraphicMain, CDialog)
	ON_BN_CLICKED(IDSTOP, &CDBGraphicMain::OnStop)
	ON_BN_CLICKED(IDSTART, &CDBGraphicMain::OnStart)
	ON_BN_CLICKED(IDCLEAR, &CDBGraphicMain::OnClear)
	ON_BN_CLICKED(IDEXIT, &CDBGraphicMain::OnExit)
	ON_NOTIFY(LVN_INSERTITEM, IDC_MSG, &CDBGraphicMain::OnInsertitemMsg)
	ON_BN_CLICKED(IDC_ADDAGENT, &CDBGraphicMain::OnAddagent)
	ON_WM_TIMER()
	ON_MESSAGE(MESSAGE_UPDATE_UI_STATUS,UpdateUIStatus)
	//ON_MESSAGE(MESSAGE_INSERT_LIST_MESSAGE,UpdateMainFrameMessage)
	ON_BN_CLICKED(ID_TEST_MAIL, &CDBGraphicMain::OnClickedTestMail)
END_MESSAGE_MAP()

int	CDBGraphicMain::m_Lock = 0;
DWORD CDBGraphicMain::m_Tick;
extern HANDLE g_hViewMutex;


// CDBGraphicMain message handlers


void CDBGraphicMain::OnStop()
{
	// TODO: Add your control notification handler code here
	bFlagStop = TRUE;
	CListBox* pBox = (CListBox*)GetDlgItem(IDC_DBINFO);
	if(pBox->GetCount())
		GetDlgItem(IDSTOP)->EnableWindow(FALSE);
	KillTimer(1001);
	GetDlgItem(IDC_NOTYETIMPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CREATETRACELOGCHECK)->EnableWindow(TRUE);
}

void CDBGraphicMain::OnStart()
{
	// TODO: Add your control notification handler code here
	m_bStart =TRUE;
	bFlagStop = FALSE;
	m_iTraceLog = ((CButton*) GetDlgItem(IDC_CREATETRACELOGCHECK))->GetCheck();
	CString temp;
	temp.Format(L"%d",m_iTraceLog);
	WritePrivateProfileStringW(L"System",L"CreateTraceLog",temp,((CWinApp*)AfxGetApp())->m_pszProfileName);
 #ifdef DBG_SING_THREAD
 	LoopAgency();
 #else ifdef DBG_MULTI_THREAD
 	m_tInfo.pctrlProgress=this;
 	AfxBeginThread(ThreadFunc,&m_tInfo);
 #endif
}

void CDBGraphicMain::OnContinue()
{
	// TODO: Add your control notification handler code here
	m_bStart = FALSE;
 #ifdef DBG_SING_THREAD
 	LoopAgency();
 #else ifdef DBG_MULTI_THREAD
 	m_tInfo.pctrlProgress=this;
 	AfxBeginThread(ThreadFunc,&m_tInfo);
 #endif
}


void CDBGraphicMain::OnClear()
{
	// TODO: Add your control notification handler code here
	m_msg.DeleteAllItems();
}


void CDBGraphicMain::OnExit()
{
	// TODO: Add your control notification handler code here
	if (AfxMessageBox(L"確定離開?",MB_YESNO) != IDYES)
		return;
	else
		TerminateProcess(GetCurrentProcess(),0);
	CDialog::OnCancel();
}


void CDBGraphicMain::OnInsertitemMsg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	/*CTime time;
	CString szDate,szText;
	time=CTime::GetCurrentTime();
	CTimeSpan sp=time-m_startTime;
	if((sp.GetHours()+sp.GetDays()*24)>4){
		int i = m_msg.GetItemCount()-1;
		int j = m_msg.GetItemCount()/2-1;
		for(i; i >j ; i--){
			m_msg.DeleteItem(i);
		}
		m_startTime=time;
	}		
	m_msg.Arrange(LVA_ALIGNTOP);
	UpdateData(FALSE);*/
	*pResult = 0;
}


void WriteStringEx(UINT nResID)
{
	CString strMessage;
	strMessage.LoadString(nResID);
	const wchar_t* wc_tmp = strMessage;
	WriteStringEx(wc_tmp);
}

void WriteStringEx(LPCWSTR szString)
{
	if(p_StatView)
	{
		if(IsWindow(p_StatView->m_hWnd)){
			p_StatView->DisplayMessage(szString,0);
		}
	}
	return;
	wchar_t newline[4];
	newline[0] = 0x0D;
	newline[1] = 0x0A;
	newline[2] = 0;
	wchar_t Buffer[512];
	time_t NowTime;
	time(&NowTime);
	lstrcpy(Buffer, newline);
	lstrcat(Buffer, szString);
	lstrcat(Buffer, newline);
	lstrcat(Buffer, L"               ");
	lstrcat(Buffer, _wctime(&NowTime));
	if(Buffer[lstrlen(Buffer) - 1] == 0x0A)
		Buffer[lstrlen(Buffer) - 1] = '\0';
	lstrcat(Buffer, newline);
	if(p_StatView)
	{
		if(IsWindow(p_StatView->m_hWnd)){
			p_StatView->DisplayMessage(Buffer,0);
		}
	}
}

void CDBGraphicMain::DistributePhoto(int num)
{
	DisplayTraceMessage(L"",0,0,TRUE);
	DisplayTraceMessage(L"S loadErrInfo",0,FALSE);
	loadErrInfo();
	DisplayTraceMessage(L"E loadErrInfo",0,FALSE);
	if (m_checkYsd && m_bDBGYesterday)
		getYesterday(m_strDbgStartTime);
	else
		m_strDbgStartTime.Format(L"%4d-%2d-%2d %2d:%2d:%2d",CTime::GetCurrentTime().GetYear(),CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetDay(),CTime::GetCurrentTime().GetHour(),CTime::GetCurrentTime().GetMinute(),CTime::GetCurrentTime().GetSecond());
	bool bBreak = false;
	CString strPhotoEx=m_strPhotoEx;
	m_strDisplayMessage.Format(L"Checking [%s] [Ext: %s] [Photo limit: %d].\r\n",db_config->szTeleName,db_config->szPhotoExtName,m_MaxTm);
	DisplayMessage(m_strDisplayMessage,1);
	//Edit by Leon Li 2013-02-27 for enhancement: remove dat file and check photo record in memeory
	DisplayTraceMessage(L"S LoadDistFileHistory",0,FALSE);
	if (m_checkYsd && !m_bDBGYesterday)
	{
		CString temp = m_strDbgStartTime;
		getYesterday(m_strDbgStartTime);
		LoadDistFileHistory();
		m_strDbgStartTime = temp;
	}
	else
		LoadDistFileHistory();
	DisplayTraceMessage(L"E LoadDistFileHistory",0,FALSE);
	DisplayTraceMessage(L"S InitProcessParam",0,FALSE);
	InitProcessParam();
	DisplayTraceMessage(L"E InitProcessParam",0,FALSE);
	//End Leon Li 2013-02-27
	while (!bBreak) 
	{
		CString strFreeMemory;
		MEMORYSTATUSEX MemStat;
		MemStat.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&MemStat);
		strFreeMemory.Format(L"%s: 系統可用記憶體: %lu KB", db_config->szTeleName, MemStat.ullAvailPhys / 1024L);
		DisplayTraceMessage(strFreeMemory,0,FALSE);

		CString strEx;
		if (strPhotoEx.ReverseFind(';') != -1)
		{
			strEx = strPhotoEx.Right(strPhotoEx.GetLength()-strPhotoEx.ReverseFind(';')-1);
			strPhotoEx = strPhotoEx.Left(strPhotoEx.ReverseFind(';'));
		}
		else
		{
			bBreak = true;
			strEx=strPhotoEx;
		}
		m_strExt=strEx;
		//讀取圖片格式

		if(WAIT_OBJECT_0 == WaitForSingleObject(g_hViewMutex, INFINITE))
		{
			if(m_Lock)
			{
				ReleaseMutex(g_hViewMutex);
				return;
			}
			m_Lock = 1;
			ReleaseMutex(g_hViewMutex);
		}
		p_StatView = this;
		int  nFindFlag;
		CString szSrcPath;
	//	wchar_t szSrcPath[PATH_LENGTH];
		wchar_t szTemp[ TEMP_STRING_LENGTH ];
		struct _wfinddata_t findt;
		CString strApplePath;
		lstrcpy(szTemp,m_szSystemTempPath);
		CString szErrPath;
		int iDay,i,j,k,iContinue=1;
		int iOrder[32]={0,-1,1,2,3,4,5,6,-2,-3,-4,-5,-6,1000};
		for(i=0;i<32;i++)  iOrder[i]=1000;
		iOrder[0]=0;
		iOrder[1]=-1;
		i=db_config->nBefore;
		j=db_config->nAfter;
		iDay=0;
		k=0;
		CString str_Agency ;str_Agency.Format(L"%s",db_config->szTeleName);
		m_stcProcessAgency.SetWindowTextW(L"通訊社:"+str_Agency);
		m_stcProcessingFile.SetWindowTextW(L"");
		for(iDay=1;iDay<=14;iDay++)
		{
			i=iDay/2;
			if(iDay%2==0)
			{
				i=-i;
				if(i>=-db_config->nBefore)
					iOrder[k++]=i;
			}
			else
			{
				if(i<=db_config->nAfter)
					iOrder[k++]=i;
			}
		}
		if(k<2) k=1;
		//if(k<2) k=2;
		if(db_config->bCurrentDate)
		{
			iOrder[0]=0;
			k=1;
		}
		m_totalFile=0;
		int iPreNumCopy=0;
		for(i=0;i<k && iContinue;i++)
		{
			iDay=iOrder[i];

			strApplePath=GetSrcPath(iDay,&iContinue);
			strApplePath.MakeLower();
			m_totalFile=ComputerFileNum(strApplePath);

			m_szSourcePath=strApplePath;
			m_szTxtSrcPath = GetTextPath(iDay);
			m_szTxtSrcPath.MakeLower();
			int nIdxP = -1; // 失敗路徑在log數組中的位置

			//			nIdxP = log.FindLogLine(1, strApplePath);

			/////////////////////////////////////////////
			if(_waccess ( strApplePath , 0 ) == -1 )
			{
				m_strDisplayMessage.Format(L"[%s] Failed to access: %s \r\n",db_config->szCode,strApplePath);
				DisplayMessage(m_strDisplayMessage,1);
				m_Lock=0;
				//				if (nIdxP < 0)			
				//					log.AddLogLine(1, strApplePath);
				return;
			}
			else
			{
				;
				//				if (nIdxP >= 0)
				// 					log.RemovedLogLine(nIdxP);
			}
			CString str=strEx;//m_lpdfs->szPhotoExtName;
			str.TrimLeft();
			str.TrimRight();
			BOOL bIsNoExtend = FALSE;
			if(!str.IsEmpty())
			{
				int nFind=str.ReverseFind('.');

				if(nFind!=-1)
					szSrcPath.Format(L"%s\\%s",m_szSourcePath,str);
				else
					szSrcPath.Format(L"%s\\*.%s",m_szSourcePath,strEx);
			}
			else
			{
				CString strTemp = m_szSourcePath;
				strTemp += "\\*.*";
				szSrcPath = strTemp;
				bIsNoExtend = TRUE;
			}
			//		m_totalFileExt[num]+=ComputerSkpFile(szSrcPath);
			nFindFlag= _wfindfirst( szSrcPath, &findt);
			long lCount = 0 ;
			m_lTotalCheck = 0 ;
			BOOL bFind = FALSE ;
			int nCount=0;

			int nIdxF = -1;// 失敗文件在log數組中的位置
			CString strFullName;
			if( nFindFlag!=-1)
			{
				do
				{		 //cFileName
					//DisplayTraceMessage(L"E Read Path and Time ",0,FALSE);
					DisplayTraceMessage(L"E Init check and read dat **",0,FALSE);
					DisplayTraceMessage(findt.name,0,FALSE);
					if(m_bExit) break;
					if(findt.name[0] == '.')//日期目錄為月$分
						continue;
					if((findt.attrib & _A_SUBDIR) != 0)
						continue;
					if (lCount % 49 == 0)
					{
						MemStat.dwLength = sizeof(MEMORYSTATUSEX);
						GlobalMemoryStatusEx(&MemStat);
						strFreeMemory.Format(L"%s: 系統可用記憶體: %lu KB", db_config->szTeleName, MemStat.ullAvailPhys / 1024L);
						DisplayTraceMessage(strFreeMemory,0,FALSE);
					}
					DisplayTraceMessage(L"S findt.attrib",0,FALSE);
					strFullName.Format(L"%s\\%s", strApplePath, findt.name);
					DisplayTraceMessage(L"Efindt.attrib",0,FALSE);
					m_szFullName=strFullName;
					DisplayTraceMessage(L"S checkErrTable",0,FALSE);
					if(!checkErrTable(strFullName))
						continue;	
					DisplayTraceMessage(L"E checkErrTable",0,FALSE);
					CFile  hFile;
					CString str;str.Format(L"Process on: %s", strFullName);
					DisplayTraceMessage(str,0,FALSE);
					if(!hFile.Open(strFullName, CFile::modeRead|CFile::shareExclusive, NULL))
					{
						m_strDisplayMessage.Format(L"Failed to access file<%s>\r\n", strFullName);
						DisplayMessage(m_strDisplayMessage,display);
						continue;
					}
					int iFileSize = 0;
					try
					{
						//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
						//iFileSize = hFile.GetLength();
						iFileSize = (int)hFile.GetLength();
						hFile.Close();
					}
					catch (...) 
					{
						hFile.Close();
						m_strDisplayMessage.Format(L"Get the size of file<%s> error, skip\r\n", strFullName);
						if(CheckErrFile())
							DisplayMessage(m_strDisplayMessage,display);
						m_szFullName=strFullName;
						continue;
					}
					DisplayTraceMessage(L"E hFile.Close();",0,FALSE);
					if(iFileSize <= 0)
					{
						m_strDisplayMessage.Format(L"File size error: %s\r\n", strFullName);
						DisplayTraceMessage(L"S CheckErrFile",0,FALSE);
						if(CheckErrFile()){
							DisplayMessage(m_strDisplayMessage,display);}
						DisplayTraceMessage(L"E CheckErrFile",0,FALSE);
						m_szFullName=strFullName;
						continue;
					}
					//限制圖片的大小,如果小於設定的?值,則不處理
					DisplayTraceMessage(L"S CDBGraphicApp",0,FALSE);
					CDBGraphicApp *pApp=(CDBGraphicApp*)AfxGetApp();
					if(pApp->m_bUseFileSizeLimit&&iFileSize<pApp->m_nFileMinSize*1024){
						continue;}

					//					nIdxF = log.FindLogLine(2, strFullName);
					DisplayTraceMessage(L"E CDBGraphicApp",0,FALSE);
					if (nIdxF < 0) // 該文件不在拷貝失敗日志中
					{
						if(db_config->bCurrentDate)
						{
							CTime today;
							CString ystDate;
							//for test
							if(m_checkYsd){
								CString ysdDate;
								int year,month,day;
								getYesterday(ysdDate);
								//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
								//swscanf(ysdDate,_T("%04d%02d%02d"),&year,&month,&day);
								swscanf_s(ysdDate,_T("%04d-%02d-%02d"),&year,&month,&day);
								CTime tm(year,month,day,0,0,0);
								today=tm;
							}
							else
								today=CTime::GetCurrentTime();
							CTime FTime;
							//add by wxy below 2003-10-30
							int addday,addmin;
							//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
							//swscanf(db_config->szBeginTime,L"%d:%d",&addday,&addmin);
							swscanf_s(db_config->szBeginTime,L"%d:%d",&addday,&addmin);
							today -= CTimeSpan(0,addday,addmin,0);
							//add by wxy end
							int iYear,iMonth,iDay,fYear,fMonth,fDay;
							FTime=CTime(findt.time_write);
							iYear=today.GetYear();
							iMonth=today.GetMonth();
							iDay=today.GetDay();
							fYear=FTime.GetYear();
							fMonth=FTime.GetMonth();
							fDay=FTime.GetDay();


							if(iYear!=fYear || iMonth!=fMonth || iDay!=fDay){
								continue;}
						}

					}
					DisplayTraceMessage(L"E iYear",0,FALSE);
					CString ext;
					ext=findt.name;
					ext.TrimRight();
					ext.MakeLower();
					DisplayTraceMessage(L"E findt.name;",0,FALSE);			
					if(findt.size>0)
					{
						DisplayTraceMessage(L"S findt.size",0,FALSE);	
						int nSize = findt.size/1024+1;
						DisplayTraceMessage(L"S strfName=findt.name",0,FALSE);	
						CString strfName = findt.name;
						strfName.MakeLower();
						lstrcpy(findt.name, strfName);
						DisplayTraceMessage(L"E lstrcpy(findt.name, strfName);",0,FALSE);
						int nRe = strfName.ReverseFind('.');
						DisplayTraceMessage(L"E int nRe = strfName.ReverseFind('.');",0,FALSE);	
						//m_stcProcessingFile.SetWindowTextW(L"正在處理:"+m_szSourcePath+L"\\"+strfName);
						if(bIsNoExtend)
						{
							if(nRe == -1)
							{
								DISTFILEITEM logItem;
								//DisplayTraceMessage(L"** Start Process Photo ",0,FALSE);
								DisplayTraceMessage(L"Start LogTransFile ",0,FALSE);
								en_ACCESSDBRET ret = LogTransFile(findt.name, lCount, nSize, logItem, iDay);
								DisplayTraceMessage(L"End LogTransFile ",0,FALSE);
								if(ret == SUCCESS_TRUE || ret==FILEEXISTEDWIHTNOLOG)
								{
									DisplayTraceMessage(L"S Add to memory ",0,FALSE);
									m_AgentDistMedia.MediaDistArr.Add(logItem);
									m_AgentDistMedia.nMediaCount++;
									DisplayTraceMessage(L"E Add to memory ",0,FALSE);
								}
								else if(ret == FAILED_FALSE)
								{
									;
								}
								else if(ret == ACCESSDBERROR)
								{
									m_strDisplayMessage.Format(L"Failed to access database\r\n");
									DisplayMessage(m_strDisplayMessage,1);
									if(WAIT_OBJECT_0 == WaitForSingleObject(g_hViewMutex, INFINITE))
									{
										m_Lock = 0;
										ReleaseMutex(g_hViewMutex);
									}
									return;
								}
								else if(ret == NOTACCESSPATH)
								{
									if(WAIT_OBJECT_0 == WaitForSingleObject(g_hViewMutex, INFINITE))
									{
										m_Lock = 0;
										ReleaseMutex(g_hViewMutex);
									}
									return;
								}
								DisplayTraceMessage(L"End _wfindnext ",0,FALSE);
								lCount ++ ;
							}	
						}
						else
						{
							DISTFILEITEM logItem;
							//DisplayTraceMessage(L"** Start Process Photo ",0,FALSE);
							DisplayTraceMessage(L"Start LogTransFile ",0,FALSE);
							en_ACCESSDBRET ret = LogTransFile(findt.name , lCount,nSize, logItem, iDay+m_nDayOffset);
							DisplayTraceMessage(L"End LogTransFile ",0,FALSE);
							if(ret == SUCCESS_TRUE || ret==FILEEXISTEDWIHTNOLOG)
							{
								DisplayTraceMessage(L"S Add to memory ",0,FALSE);
								m_AgentDistMedia.MediaDistArr.Add(logItem);
								m_AgentDistMedia.nMediaCount++;
								DisplayTraceMessage(L"E Add to memory ",0,FALSE);
							}
							else if(ret == FAILED_FALSE)
							{
								//								if ((nIdxF < 0) && !m_bCopyFileSuccess)
								//									log.AddLogLine(2, strFullName);
								;
							}		
							else if(ret == ACCESSDBERROR)
							{
								m_strDisplayMessage.Format(L"Failed to access database\r\n");
								DisplayMessage(m_strDisplayMessage,1);
								if(WAIT_OBJECT_0 == WaitForSingleObject(g_hViewMutex, INFINITE))
								{
									m_Lock = 0;
									ReleaseMutex(g_hViewMutex);
								}
								return;
							}
							else if(ret == NOTACCESSPATH)
							{
								if(WAIT_OBJECT_0 == WaitForSingleObject(g_hViewMutex, INFINITE))
								{
									m_Lock = 0;
									ReleaseMutex(g_hViewMutex);
								}

								return;
							}
							lCount ++ ;
							CString str_Count;str_Count.Format(L"%d", lCount);
							CString str_NumCopy; str_NumCopy.Format(L"%d", num_copy);
							m_stcTotalChkCount.SetWindowTextW(L"本次已成功入庫數目:"+str_NumCopy);
							DisplayTraceMessage(L"End _wfindnext ",0,FALSE);
						}
					}
					DisplayTraceMessage(L"E while(_wfindnext(nFindFlag,&findt);\r\n",0,FALSE);	
					if(bFlagStop == TRUE)
						break;

					if(num_copy>=m_MaxTm)
						break;
				}while(_wfindnext(nFindFlag,&findt) == 0);
				DisplayTraceMessage(L"Start Remove Local Temp File ",0,FALSE);
				CFileFind filfind;
				CString strFileDeletePath;
				strFileDeletePath.Format(L"%s\\*.jpg",m_szPhotoDesPath);
				bFind=filfind.FindFile(strFileDeletePath);
				while (bFind)
				{
					bFind=filfind.FindNextFile();
					strFileDeletePath=filfind.GetFileName();
					if (strFileDeletePath.Find(db_config->szCode)>-1)
					{
						SetFileAttributes(filfind.GetFilePath(),GetFileAttributes(filfind.GetFilePath()) & ~FILE_ATTRIBUTE_READONLY);
						_tremove(filfind.GetFilePath());
					}
				}
				DisplayTraceMessage(L"End Remove Local Temp File ",0,FALSE);

			}
			DisplayTraceMessage(L"E nFindFlag;",0,FALSE);	
			_findclose(nFindFlag);
			DisplayTraceMessage(L"End _findclose ",0,FALSE);
			////若只需當天稿,則break;
			if(db_config->bCurrentDate) 
				break;
		}
		
		DisplayTraceMessage(L"E GetCurTime(curTime);",0,FALSE);	
		int pos;
		CString curTime,strExt;
		GetCurTime(curTime);
		if(bBreak){	
			strPhotoEx=m_strPhotoEx;
			BOOL ifBreak=TRUE;
			while(ifBreak){
				CString strTemp;
				if (strPhotoEx.ReverseFind(';') != -1)
				{
					strEx = strPhotoEx.Right(strPhotoEx.GetLength()-strPhotoEx.ReverseFind(';')-1);
					strPhotoEx = strPhotoEx.Left(strPhotoEx.ReverseFind(';'));
				}
				else
				{
					strEx=strPhotoEx;
					ifBreak=FALSE;
				}

				CString str=strEx;//m_lpdfs->szPhotoExtName;
				str.TrimLeft();
				str.TrimRight();
				BOOL bIsNoExtend = FALSE;
				if(!str.IsEmpty())
				{
					int nFind=str.ReverseFind('.');
					if(nFind!=-1)
						szSrcPath.Format(L"%s\\%s",m_szSourcePath,str);
					else
						szSrcPath.Format(L"%s\\*.%s",m_szSourcePath,strEx);
				}
				DisplayTraceMessage(L"start ComputerSkpFile "+ strEx,0,FALSE);
				m_totalFileExt[num]+=ComputerSkpFile(szSrcPath);
				DisplayTraceMessage(L"End ComputerSkpFile ",0,FALSE);
			}
			m_strDisplayMessage=L"";
			DisplayTraceMessage(L"Start record file name ",0,FALSE);
			for (int p=(num_copy-1);p>=0;p--)
			{
				CString filename1,filename2;
				filename1 = m_fileName[p];
				if(pos=filename1.ReverseFind('\\'))
					filename2=filename1.Right(filename1.GetLength()-pos-1);
				if(p==(num_copy-1))
					m_strDisplayMessageHeader.Format(L"Upload Photo: %s",filename2);
				else if(p==0)
					m_strDisplayMessageHeader.Format(L",%s \r\n",filename2);
				else 
					m_strDisplayMessageHeader.Format(L",%s",filename2);
				if (num_copy==1)
					m_strDisplayMessageHeader+=L" \r\n";
				m_strDisplayMessage+=m_strDisplayMessageHeader;
			}
			DisplayTraceMessage(L"End record file name ",0,FALSE);
			if( num_copy)
				DisplayMessage(m_strDisplayMessage,0);
			m_skipped+=m_disFile+m_exist+m_errCount;
			m_totalrm=m_totalFileExt[num]-(m_skipped+num_copy);
			CString srcPath,folder;
			srcPath=GetSrcPath(0,0);
			folder=srcPath.Right(srcPath.GetLength()-srcPath.ReverseFind('\\')-1);
			if (m_totalrm)
				m_strDisplayMessage.Format(L"%s: [Folder: %s]  [Photo: %d]  [Distributed: %d] [Error: %d] [Remained: %d] [Processed: %d].\r\n\r\n",db_config->szCode,folder,m_totalFileExt[num],m_disFile+m_exist,m_errCount,m_totalrm<=0?0:m_totalrm,num_copy);
			else 
				m_strDisplayMessage.Format(L"%s: [Folder: %s]  [Photo: %d]  [Distributed: %d] [Error: %d] [Processed: %d]. \r\n\r\n",db_config->szCode,folder,m_totalFileExt[num],m_disFile+m_exist,m_errCount,num_copy);
			DisplayMessage(m_strDisplayMessage,1);
			m_totalPrs+=num_copy;
			m_skipped=0;
			m_errCount=0;
			m_disFile=0;
			m_exist=0;
			m_totalFileExt[num]=0;

			MemStat.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&MemStat);
			strFreeMemory.Format(L"%s: 系統可用記憶體: %lu KB", db_config->szTeleName, MemStat.ullAvailPhys / 1024L);
			DisplayTraceMessage(strFreeMemory,0,FALSE);
		}
		if ( m_bLogFileOpened )
		{
			m_fileLog.Close();
			m_bLogFileOpened = FALSE ;
		}
		//m_Lock=0;
		if(WAIT_OBJECT_0 == WaitForSingleObject(g_hViewMutex, INFINITE))
		{
			m_Lock = 0;
			ReleaseMutex(g_hViewMutex);
		}
	}
}

BOOL CDBGraphicMain::LoadConfig(){
	if(m_bDbFail)
	{
		if(!p_dbCmd->LoginDb(TRUE))
			return FALSE;
	}	
	wchar_t szBuffer2[256]={0};
	wchar_t szBuffer3[256]={0};
	wchar_t szBuffer4[256]={0};
	wchar_t szBuffer5[256]={0};
	wchar_t szBuffer6[256]={0};
	wchar_t szBuffer7[256]={0};
	wchar_t szBuffer8[256]={0};
	RETCODE nResult;
	CString code,text;
	SDWORD	cbValue1;
	BOOL bFil = FALSE ;
	int szBuffer1;
	p_dbCmd->ODBCAllocStmt ();
	nResult = p_dbCmd->ODBCExecCmdW( L"select value from sys_newsroom where entry = 'DBG_Interval'");
	p_dbCmd->ODBCBind(1, SQL_C_LONG,&szBuffer1,0,&cbValue1);
	while (nResult != SQL_NO_DATA_FOUND) 
	{

		nResult = p_dbCmd->ODBCFetch();		
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{
			bFil = TRUE;

		}
		else
			nResult = p_dbCmd->ODBCMoreResults();

	}
	p_dbCmd->ODBCFreeStmt(SQL_DROP);
	//m_interval=szBuffer1*60;
	m_interval=szBuffer1;

	p_dbCmd->ODBCAllocStmt ();
	nResult = p_dbCmd->ODBCExecCmdW( L"select value from sys_newsroom where entry = 'DBG_MaxTm'");
	p_dbCmd->ODBCBind(1, SQL_C_LONG,&szBuffer1,0,&cbValue1);
	while (nResult != SQL_NO_DATA_FOUND) 
	{

		nResult = p_dbCmd->ODBCFetch();		
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{
			bFil = TRUE;

		}
		else
			nResult = p_dbCmd->ODBCMoreResults();

	}
	p_dbCmd->ODBCFreeStmt(SQL_DROP);
	m_MaxCopy=m_MaxTm = szBuffer1;

	p_dbCmd->ODBCAllocStmt ();
	nResult = p_dbCmd->ODBCExecCmdW( L"select value from sys_newsroom where entry = 'DBG_TryTime'");
	p_dbCmd->ODBCBind(1, SQL_C_LONG,&szBuffer1,0,&cbValue1);
	while (nResult != SQL_NO_DATA_FOUND) 
	{

		nResult = p_dbCmd->ODBCFetch();		
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{
			bFil = TRUE;

		}
		else
			nResult = p_dbCmd->ODBCMoreResults();

	}
	p_dbCmd->ODBCFreeStmt(SQL_DROP);
	m_TryTime = szBuffer1;

	p_dbCmd->ODBCAllocStmt ();
	nResult = p_dbCmd->ODBCExecCmdW( L"select value from sys_newsroom where entry = 'DBG_Email'");
	p_dbCmd->ODBCBind(1, SQL_C_LONG,&szBuffer1,0,&cbValue1);
	while (nResult != SQL_NO_DATA_FOUND) 
	{

		nResult = p_dbCmd->ODBCFetch();		
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{
			bFil = TRUE;

		}
		else
			nResult = p_dbCmd->ODBCMoreResults();

	}
	p_dbCmd->ODBCFreeStmt(SQL_DROP);
	m_Eml = szBuffer1;

	szBuffer1 = 0;
	p_dbCmd->ODBCAllocStmt ();
	nResult = p_dbCmd->ODBCExecCmdW( L"select value from sys_newsroom where entry = 'DBG_ProcessTime'");
	p_dbCmd->ODBCBind(1, SQL_C_LONG,&szBuffer1,0,&cbValue1);
	while (nResult != SQL_NO_DATA_FOUND) 
	{

		nResult = p_dbCmd->ODBCFetch();		
		if (nResult == SQL_SUCCESS || nResult== SQL_SUCCESS_WITH_INFO) 
		{
			bFil = TRUE;

		}
		else
			nResult = p_dbCmd->ODBCMoreResults();

	}
	p_dbCmd->ODBCFreeStmt(SQL_DROP);
	m_ProcessTime = szBuffer1 == 0? 2: szBuffer1;
	return bFil;
}

void CDBGraphicMain::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1001)
		OnContinue();
	else
		;
	CDialog::OnTimer(nIDEvent);
}


void CDBGraphicMain::CheckTargetDate()
{   
	
	int ntype=0;
	//wchar_t szPath[PATH_LENGTH],szPathAll[PATH_LENGTH],szTemp[TEMP_STRING_LENGTH];
	CString szPath,szPathAll;
	//edit by leon 2012-11-23 for define temporary path in INI instead of hardcode.
	//CString str_temp1,str_temp = AfxGetApp()->GetProfileString(L"System", L"TemporaryPath", L"C:\\FOUNDER\\TEMP"),szCurTime;
	CString str_temp1,szCurTime;
	szPath.Format(L"%s\\%s.DGP", m_szSystemTempPath, db_config->szCode);
	szPathAll.Format(L"%s\\%s.AGP", m_szSystemTempPath, db_config->szCode);
	LOGDISTRIBUTEDHEAD head;
	LOGDISTRIBUTEDALLHEAD  Allhead;
	RealTime=GetConvertTime();
	if(m_strPhotoPath.Find(L"%Y%M%D")>0){
		szCurTime.Format(L"%04d-%02d-%02d", RealTime.GetYear(),RealTime.GetMonth(),RealTime.GetDay());
	}
	else if(m_strPhotoPath.Find(L"%Y%M")>0){
		szCurTime.Format(L"%04d-%02d", RealTime.GetYear(),RealTime.GetMonth());
	}
	else if(m_strPhotoPath.Find(L"%Y")>0){
		szCurTime.Format( L"%04d ", RealTime.GetYear());
	}
	else szCurTime=L"00:00:00";
	if( m_bDatFileOpened )
	{
//		m_fileDat.SeekToBegin();
		m_fileDat.Seek(2, CFile::begin);
		m_fileDat.Read(&head, sizeof(LOGDISTRIBUTEDHEAD));
		if( lstrcmp( head.szDate, szCurTime) != 0 )
		{
			m_fileDat.Close(); 
		//	CFile::Remove(szPath);
			m_bDatFileOpened = m_fileDat.Open(szPath, 
				CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone);
			if( m_bDatFileOpened )
			{
 				if (!Load_Data_File())
 				{
 					DisplayMessage (L"Load Data File Error \r\n",0);
 					return;
 				}
			}
			else{
 				DisplayMessage(L"Create Data File Error \r\n",0);
			}
		}
	}
	else
	{
		m_bDatFileOpened = m_fileDat.Open(szPath, 
			CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone);
		if( m_bDatFileOpened )
		{
			m_fileDat.Seek(2, CFile::begin);
			m_fileDat.Read(&head, sizeof(LOGDISTRIBUTEDHEAD));
			if( lstrcmp( head.szDate, szCurTime) != 0 )
			{
				m_fileDat.Close();
			//	CFile::Remove(szPath);
				m_bDatFileOpened = m_fileDat.Open(szPath, 
					CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone);
				if( m_bDatFileOpened )
				{
					if (!Load_Data_File())
					{
						DisplayErrorMessage (L"Load Data File Error",0);
 						return;
					}
				}
			}
		}
		else
		{
			m_bDatFileOpened = m_fileDat.Open(szPath, 
				CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone);
			if( m_bDatFileOpened )
			{

			// edit by leon 
  			if (!Load_Data_File())
  			{
  				DisplayErrorMessage (L"Load Data File Error",0);
 				return;
			}
			}
		}
	}
	if( m_bAllFileOpened )
	;
	else
	{
		m_bAllFileOpened = m_fileAll.Open(szPathAll, 
			CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone);
		if( m_bAllFileOpened )
		{
			if(m_fileAll.GetLength()==0)
			{
				m_fileAll.SeekToBegin();
				Allhead.lTotal=0;
				m_fileAll.Write( &Allhead, sizeof(Allhead) );
				m_fileAll.Close();
				m_bAllFileOpened = m_fileAll.Open(szPathAll, 
				CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone);
			}
		}
		else
		{
			m_bAllFileOpened = m_fileAll.Open(szPathAll, 
			CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone|CFile::modeCreate);
			m_fileAll.SeekToBegin();
			Allhead.lTotal=0;
			m_fileAll.Write( &Allhead, sizeof(Allhead) );
			m_fileAll.Close();
			m_bAllFileOpened = m_fileAll.Open(szPathAll, 
			CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone);
		}
	}
	// Photo Source Path
	int addday,addmin;
	addday=addmin=0;
//	sscanf(m_lpdfs->szBeginTime,"%d:%d",&addday,&addmin);
	//swscanf(db_config->szBeginTime,L"%d:%d",&addday,&addmin);

	m_szSourcePath = GetPath(db_config->szSourcePath,m_checkYsd);

	m_szTxtSrcPath = GetPath(db_config->szTextPath,m_checkYsd);
	db_config->bAutoBitmap=1;
	m_szBmpSrcPath=L"";		//原簡圖路徑
	m_szPhotoDesPath=L"";	//原--源圖的COPY路徑
}

void CDBGraphicMain::InitCheck()
{   
	LOGDISTRIBUTEDITEM item;
	if(!memitem.ResetContent())
	{
		DisplayMessage(L"Failed resetcontent in  InitCheck.\r\n",1); 
		return;
	}
	m_fileDat.Seek(sizeof(LOGDISTRIBUTEDHEAD)+2 , CFile::begin);
	while( m_fileDat.Read(&item, sizeof(LOGDISTRIBUTEDITEM)) == sizeof(LOGDISTRIBUTEDITEM))
		memitem.InsertData (  (LPBYTE)item.szFile , NAME_LEN ) ;
}

void CDBGraphicMain::UpdateXMLFile()
{
	::CoInitialize(NULL);

	HRESULT hr = m_plDomDocument.CreateInstance(MSXML::CLSID_DOMDocument);
	if (FAILED(hr))
	{
		 _com_error er(hr);
		 //AfxMessageBox(er.ErrorMessage());
	}
	hr = m_plDomDocumentPre.CreateInstance(MSXML::CLSID_DOMDocument);
	if (FAILED(hr))
	{
		 _com_error er(hr);
		 //AfxMessageBox(er.ErrorMessage());
	}

	MSXML::IXMLDOMElementPtr pDocElementAgency;
	variant_t vLoad;

	long lXMLCount = 0 ;

	if (db_config->bXMLOutput == 1)
	{
		
		// XML Parameter use
		wchar_t szXMLSave[ MAX_FILESPECIAL ]; 
//		wchar_t szSrcFile[ PATH_LENGTH1 ];
	//	wchar_t szSrcPath[ PATH_LENGTH ];
		wchar_t szTemp[ TEMP_STRING_LENGTH ];
//		struct _wfinddata_t findt;
		int iDate = 0;

		if (db_config->bXMLByAgency == 1 )
		{
			wchar_t szXMLFile[PATH_LENGTH1];
			swprintf( szXMLFile,MAX_COUNT, L"%s\\%s.xml", db_config->szXMLPath, db_config->szTeleCode );
		//	VARIANT_BOOL fSuccess;
		//	_variant_t v(szXMLFile);
			vLoad = m_plDomDocumentPre->load(szXMLFile);

			if (((bool)vLoad) == TRUE )
			{
				pDocElementAgency = m_plDomDocumentPre->documentElement;
					
				// XML "Agency" Attribute
				// Update latest info of XML
				CString strRootPath = db_config->szSourcePath;
				strRootPath.Format(strRootPath.Left(strRootPath.Find(L"%")-1));
				//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
				//swprintf(szTemp, strRootPath);
				swprintf(szTemp,MAX_COUNT, strRootPath);
				pDocElementAgency->setAttribute((BSTR)"Source_Path", szTemp);
				
			}
			else
			{
				// XML Information Setting
				m_plDomDocument->appendChild(m_plDomDocument->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

				// Get System time for Filling the Create date of XML
				SYSTEMTIME st;
				GetSystemTime(&st);

				// XML Elements Create
				pDocElementAgency = m_plDomDocument->createElement("Agency");

				m_plDomDocument->appendChild(pDocElementAgency);
					
				// XML "Agency" Attribute
				pDocElementAgency->setAttribute("Agency_Name", db_config->szTeleName);
				pDocElementAgency->setAttribute("Agency_Code", db_config->szTeleCode);

				// XML "Agency" Attribute
				// Update latest info of XML
				CString strRootPath = db_config->szSourcePath;
				strRootPath.Format(strRootPath.Left(strRootPath.Find(L"%")-1));
				//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
				//swprintf(szTemp, strRootPath);
				swprintf(szTemp,MAX_COUNT, strRootPath);
				pDocElementAgency->setAttribute("Source_Path", szTemp);
//				pDocElementAgency->setAttribute("Source_Path", m_lpdfs->szSourcePath);


		//		pDocElementAgency->setAttribute("Target_OutPut_Path", m_lpdfs->szDesPath);
			}
		}

		// Scan Folder of Source Path      
		bool bBreak = false;
		CString strPhotoEx=m_strPhotoEx;
		while (!bBreak) 
		{
			CString strEx;
			if (strPhotoEx.ReverseFind(';') != -1)
			{
				strEx = m_strPhotoEx.Right(strPhotoEx.GetLength()-strPhotoEx.ReverseFind(';')-1);
				strPhotoEx = m_strPhotoEx.Left(strPhotoEx.ReverseFind(';'));
			}
			else
			{
				bBreak = true;
				strEx=strPhotoEx;
			}
			int  nFindFlag;
			wchar_t szSrcPath[PATH_LENGTH];
			wchar_t szTemp[ TEMP_STRING_LENGTH ];
			wchar_t szTemp1[250];
			struct _wfinddata_t findt;
			CString strApplePath;
			
			int iDay,i,j,k,iContinue=1;
			int iOrder[32]={0,-1,1,2,3,4,5,6,-2,-3,-4,-5,-6,1000};
			for(i=0;i<32;i++)  iOrder[i]=1000;
			iOrder[0]=0;
			iOrder[1]=-1;
			i=db_config->nBefore;
			j=db_config->nAfter;
			iDay=0;
			k=0;
			for(iDay=1;iDay<=14;iDay++)
			{
				i=iDay/2;
				if(iDay%2==0)
				{
					i=-i;
					if(i>=-db_config->nBefore)
						iOrder[k++]=i;
				}
				else
				{
					if(i<=db_config->nAfter)
						iOrder[k++]=i;
				}
			}
			if(k<2) 
				k=1;

			if(db_config->bCurrentDate)
			{
				iOrder[0]=0;
				k=1;
			}

			for(i=0;i<k && iContinue;i++)
			{
				iDay=iOrder[i];
				strApplePath=GetSrcPath(iDay,&iContinue);
				strApplePath.MakeLower();

				m_szSourcePath=strApplePath;

				m_szTxtSrcPath = GetTextPath(iDay);
				
				LoadString(AfxGetApp()->m_hInstance,IDS_CHECK, 	szTemp1,50);
				//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
// 				swprintf( szTemp, L"%s <%s>...\r\n", szTemp1,m_szSourcePath );
// 				swprintf( szTemp, L"%s chekcing Xml", db_config->szCode );
				swprintf( szTemp,MAX_COUNT,L"%s <%s>...\r\n", szTemp1,m_szSourcePath );
				swprintf( szTemp,MAX_COUNT, L"%s chekcing Xml", db_config->szCode );
				DisplayMessage( szTemp,display);
				if ( _waccess ( strApplePath , 0 ) == -1 )
				{
					CString strBuffer;
					//edit by leon 2012-08-27 for migrating from vc6 to 2010
// 					strBuffer.LoadString(AFX_IDS_NOTSOURCEPATH);
// 					strBuffer+=(CString)"("+strApplePath+")";
					CString str;
					strBuffer.LoadString(AFX_IDS_NOTSOURCEPATH);
					str.Format(L"(%s)",strApplePath);
					strBuffer+=str;
					const wchar_t* wc_tmp = strBuffer;
					
					int nSel;
					nSel = (int)SendDlgItemMessage( IDD_DBGRAPHIC_MAIN, LB_GETCURSEL, 0, 0L );//IDD_MAINFRAME
					if( nSel == LB_ERR )
						return;
				SendDlgItemMessage( IDD_DBGRAPHIC_MAIN, LB_GETTEXT, nSel, (LONG)(LPSTR)wc_tmp );//IDD_MAINFRAME

					//WriteStringEx(wc_tmp);

				}
				

				CString str=strEx;//m_lpdfs->szPhotoExtName;
				str.TrimLeft();
				str.TrimRight();
				BOOL bIsNoExtend = FALSE;
				if(!str.IsEmpty())
				{
					int nFind=str.ReverseFind('.');
					
					//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
// 					if(nFind!=-1)
// 						swprintf( szSrcPath, L"%s\\%s",m_szSourcePath,str);
// 					else
// 						swprintf( szSrcPath, L"%s\\*.%s",m_szSourcePath,strEx);
					if(nFind!=-1)
						swprintf( szSrcPath,MAX_COUNT, L"%s\\%s",m_szSourcePath,str);
					else
						swprintf( szSrcPath,MAX_COUNT, L"%s\\*.%s",m_szSourcePath,strEx);
				}
				else
				{
					CString strTemp = m_szSourcePath;
					strTemp += "\\*.*";
					const wchar_t* wc_tmp = strTemp;
					lstrcpy(szSrcPath, wc_tmp);
					bIsNoExtend = TRUE;
				}
				
				nFindFlag= _wfindfirst( szSrcPath, &findt);
				long lCount = 0 ;
				m_lTotalCheck = 0 ;
				BOOL bFind = FALSE ;
				int nCount=0;


				CString strFullName;
				if( nFindFlag!=-1)
				{
					m_arrDistributedFile.RemoveAll();
					LOGDISTRIBUTEDITEM item;

					m_fileDat.SeekToBegin();

					m_fileDat.Seek(sizeof(LOGDISTRIBUTEDHEAD)+2, CFile::begin);
					while( m_fileDat.Read(&item, sizeof(LOGDISTRIBUTEDITEM)) == sizeof(LOGDISTRIBUTEDITEM))
					{
						m_arrDistributedFile.Add(item);
					}

					//p_dbCmd->BeginTransaction();
					do
					{		 //cFileName
						
						if(m_bExit) break;
						if(findt.name[0] == '.'){//日期目錄為月$分
							continue;
						}
						if((findt.attrib & _A_SUBDIR) != 0){
							continue;
						}
						strFullName.Format(L"%s\\%s", strApplePath, findt.name);
						
						CFile  hFile;
						if(!hFile.Open(strFullName, CFile::modeRead|CFile::shareExclusive, NULL))
						{
							//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
							//swprintf(szTemp, L"Failed to access file<%s>\r\n", strFullName);
							swprintf(szTemp, MAX_COUNT,L"Failed to access file<%s>\r\n", strFullName);
							//m_skipped++;
							DisplayMessage(szTemp,display);
							continue;
						}
						int iFileSize = 0;
						try
						{
							//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
							//iFileSize = hFile.GetLength();
							iFileSize =(int) hFile.GetLength();
							hFile.Close();
						}
						catch (...) 
						{
							hFile.Close();
							//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
							//swprintf(szTemp, L"Get the size of file<%s> error, skip\r\n", strFullName);
							swprintf(szTemp,MAX_COUNT, L"Get the size of file<%s> error, skip\r\n", strFullName);
							DisplayMessage(szTemp,display);
							continue;
						}
						
						if(iFileSize <= 0)
						{
							//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
							//swprintf(szTemp, L"File size error %s\r\n", strFullName);
							swprintf(szTemp,MAX_COUNT, L"File size error %s\r\n", strFullName);
							DisplayMessage(szTemp,display);
							continue;
						}
						//限制圖片的大小,如果小於設定的?值,則不處理
						CDBGraphicApp *pApp=(CDBGraphicApp*)AfxGetApp();
						if(pApp->m_bUseFileSizeLimit&&iFileSize<pApp->m_nFileMinSize*1024)
							continue;

						CString ext;
						ext=findt.name;
						ext.TrimRight();
						ext.MakeLower();

						if(findt.size>0)
						{
							int nSize = findt.size/1024+1;
							CString strfName = findt.name;
							strfName.MakeLower();
							const wchar_t * wc_tmp = strfName;
							lstrcpy(findt.name, wc_tmp);
							int nRe = strfName.ReverseFind('.');
							if(bIsNoExtend)
							{
								if(nRe == -1)
								{
									if (CheckLogInXML( findt.name ) == FALSE)
									{
									
										XMLFileNodeGen( findt.name , pDocElementAgency , nSize, iDay);
										lXMLCount ++ ;
									}
								}	
							}
							else
							{
								if (CheckLogInXML( findt.name ) == FALSE)
								{	
									XMLFileNodeGen( findt.name , pDocElementAgency , nSize, iDay);
									lXMLCount ++ ;
								}							
							}
						}
									
					}while(_wfindnext(nFindFlag,&findt) == 0);
					
				}
				_findclose(nFindFlag);
				iDate = iDay;
				////若只需當天稿,則break;
				if(db_config->bCurrentDate) 
					break;
			}
		}

		if ( db_config->bXMLByAgency == 1 )
		{
			if (lXMLCount > 0)
			{
				if (((bool)vLoad) == TRUE )
				{					
					// Output_path
					CString strServerPath;
					strServerPath=GetServerPath(iDate);
					//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
					//swprintf(szTemp, strServerPath);
					swprintf(szTemp, MAX_COUNT,strServerPath);
					pDocElementAgency->setAttribute("Target_OutPut_Path", szTemp);
					
					// Get System Date to be the create date of the XML
					SYSTEMTIME st;
					GetSystemTime(&st);

					CString strDate;
					strDate.Format(L"%d-%.2d-%.2d %d:%d",st.wYear,st.wMonth,st.wDay, st.wHour+8, st.wMinute);

					pDocElementAgency->setAttribute("Last_Modified_Date", (const class _variant_t)strDate);


					// Count Child in XML
					wchar_t szChildCount[1000];
					int nChildCount =0;
					//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
					//for (IXMLDOMNodePtr m_pFile = pDocElementAgency->firstChild;
					for (MSXML::IXMLDOMNodePtr m_pFile = pDocElementAgency->firstChild;
					NULL != m_pFile;
					m_pFile = m_pFile->nextSibling)
					{
						nChildCount++;
					}
					//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
					//swprintf( szChildCount, L"%d", nChildCount );
					swprintf( szChildCount, MAX_COUNT,L"%d", nChildCount );
					pDocElementAgency->setAttribute("Total_Number_Of_File", szChildCount);


					// XML Saving Path By Agency			
					swprintf ( szXMLSave ,MAX_COUNT, L"%s\\%s.xml" , db_config->szXMLPath , db_config->szTeleCode );
					m_plDomDocumentPre->save( szXMLSave );
				}
				else
				{
					// Output_path
					CString strServerPath;
					strServerPath=GetServerPath(iDate);
					swprintf(szTemp, MAX_COUNT,strServerPath);
					pDocElementAgency->setAttribute("Target_OutPut_Path", szTemp);
					

					// Get System Date to be the create date of the XML
					SYSTEMTIME st;
					GetSystemTime(&st);

					CString strDate;
					strDate.Format(L"%d-%.2d-%.2d %d:%d",st.wYear,st.wMonth,st.wDay, st.wHour+8, st.wMinute);

					pDocElementAgency->setAttribute("Last_Modified_Date", (const class _variant_t)strDate);


					// Count Child in XML
					wchar_t szChildCount[ 1000 ];
					int nChildCount =0;
					//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
					//for (IXMLDOMNodePtr m_pFile = pDocElementAgency->firstChild;
					for (MSXML::IXMLDOMNodePtr m_pFile = pDocElementAgency->firstChild;
					NULL != m_pFile;
					m_pFile = m_pFile->nextSibling)
					{						
						nChildCount++;
					}
					swprintf( szChildCount,MAX_COUNT, L"%d", nChildCount );
					pDocElementAgency->setAttribute("Total_Number_Of_File", szChildCount);

				
					// XML Saving Path By Agency			
					swprintf ( szXMLSave ,MAX_COUNT, L"%s\\%s.xml" , db_config->szXMLPath , db_config->szTeleCode );
					m_plDomDocument->save( szXMLSave );
				}
			}
		}

		swprintf( szTemp,MAX_COUNT, L"nTotal file output to XML : %lu ", lXMLCount );
		if ( m_bLogFileOpened )
		{
    		m_fileLog.Close();
    		m_bLogFileOpened = FALSE ;
		}
		DisplayMessage( szTemp,display); 
	}
	else     
		return;

	::CoUninitialize();
}



#define MAX_ERROR_LEN	4096
void CDBGraphicMain::DisplayMessage(LPCWSTR lpszMsg, int display)
{
	if(IsWindow(m_hWnd)==FALSE) return;
	CFile fil;
	int Opened = 1;
	CString strLog,curTime,curDate;
	CTime time;
	time=CTime::GetCurrentTime();
	curDate.Format(L"%d%02d",time.GetYear(),time.GetMonth());
	GetCurTime(curTime);
	//edit by leon 2012-11-23 for define temporary path in INI instead of hardcode.
	//strLog = L"C:\\Founder\\Dbgraphic"+curDate+L".log";
	CString strLogNum;
	strLogNum+=m_wLogNum;
	strLog.Format(L"%s\\Dbgraphic%s%s.log",m_szSystemLogPath,curDate,strLogNum);
	//strLog.Format(L"%s\\Dbgraphic%s.log",m_szSystemLogPath,curDate);
	//end leon 2012-11-23
	curTime.TrimLeft();
	CString str;
	str=curTime+L"   "+lpszMsg;
	str.TrimLeft();
//	lstrcpy(strTmp,str);
	try
	{
		strLog.TrimRight();
		if(m_hDisMessage.m_hFile == CFile::hFileNull)
		{
			if(!m_hDisMessage.Open(strLog, CFile::modeReadWrite | CFile::shareDenyWrite))
			{
				if(m_hDisMessage.Open(strLog, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite)){
					WORD wSignature = 0xFEFF;// use unicode encoding
					m_hDisMessage.Write(&wSignature, 2);//Unicode的文件符?
					Opened = 0;}
			}
			if(m_hDisMessage.m_hFile != CFile::hFileNull)
				m_hDisMessage.Seek(0, CFile::end);
		}
		if(m_hDisMessage.m_hFile != CFile::hFileNull)
		{
			m_hDisMessage.Seek(0, CFile::end);
			//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
			//m_hDisMessage.WriteHuge(strTmp, lstrlen(strTmp)*2);
			//int iLen = m_hDisMessage.GetLength();
			if(m_hDisMessage.GetLength()>(MAX_LOG_LENGTH*1024*1024)){
				m_hDisMessage.Close();
				if(65<=m_wLogNum<=90||97<=m_wLogNum<=122)
					m_wLogNum=(wchar_t)m_wLogNum+1;
				DisplayMessage(lpszMsg, display);
				return;
			}
			m_hDisMessage.Write(str, lstrlen(str)*2);
 			int iLen =(int) m_hDisMessage.GetLength();
//  			if((iCount++ % 10) == 0)
//  			{
//  				m_hDisMessage.Flush();
//  				iCount = 1;
//  			}
			m_hDisMessage.Close();
 		}
	}
	catch(...){
		DisplayMessage(L"Display Message error \r\n",0);
	}
	if(display)
		UpdateMainFrameMessage(curTime,lpszMsg);
}

void CDBGraphicMain::DisplayTraceMessage(LPCWSTR lpszMsg, int display,BOOL bTime,BOOL bNew)
{
	//return;
	if(IsWindow(m_hWnd)==FALSE) return;
	if (m_iTraceLog == 0) return;
	CFile fil;
	int Opened = 1;
	//	wchar_t strTmp[50000];
	CString strLog,curTime,curDate;
	CTime time;
	time=CTime::GetCurrentTime();
	curDate.Format(L"%d%02d%02d",time.GetYear(),time.GetMonth(),time.GetDay());
	if (m_strStarttime.CompareNoCase(curDate)!=0)
	{
		m_iTraceLogNum = 1;
		m_strStarttime = curDate;
	}
	GetCurTime(curTime);
	//edit by leon 2012-11-23 for define temporary path in INI instead of hardcode.
	//strLog = L"C:\\Founder\\Dbgraphic"+curDate+L".log";
	strLog.Format(L"%s\\DBG_Trace%s_%d.log",m_szSystemLogPath,curDate,m_iTraceLogNum);
	m_strLog = strLog;
	//end leon 2012-11-23
	if (bNew){
		try{
			m_hDisMessage.Remove(strLog);
			return;
		}
		catch(...){
			;
		}
	}	
	curTime.TrimLeft();
	CString str;
	str=curTime+L"  "+lpszMsg;
	str.TrimLeft();
	//	lstrcpy(strTmp,str);
	try
	{
		strLog.TrimRight();
		if(m_hDisMessage.m_hFile == CFile::hFileNull)
		{
			if(!m_hDisMessage.Open(strLog, CFile::modeReadWrite | CFile::shareDenyWrite))
			{
				if(m_hDisMessage.Open(strLog, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite)){
					WORD wSignature = 0xFEFF;// use unicode encoding
					m_hDisMessage.Write(&wSignature, 2);//Unicode的文件符?
					Opened = 0;}
			}
			if(m_hDisMessage.m_hFile != CFile::hFileNull)
				m_hDisMessage.Seek(0, CFile::end);
		}
		if(m_hDisMessage.m_hFile != CFile::hFileNull)
		{
			m_hDisMessage.Seek(0, CFile::end);
			//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
			//m_hDisMessage.WriteHuge(strTmp, lstrlen(strTmp)*2);
			//int iLen = m_hDisMessage.GetLength();
			str+=L"\r\n ";
			m_hDisMessage.Write(str, lstrlen(str)*2);
			int iLen =(int) m_hDisMessage.GetLength();
			//  			if((iCount++ % 10) == 0)
			//  			{
			//  				m_hDisMessage.Flush();
			//  				iCount = 1;
			//  			}
		}
		m_hDisMessage.Close();
	}
	catch(...){
		//DisplayMessage(L"Display Message error \r\n",0);
	}
}


CString CDBGraphicMain::GetSrcPath(int addday,int * iIncludeDay)
{
//	CDbgraphiDoc * pDoc;
//	pDoc=(CDbgraphiDoc * )GetDocument();
//	if(pDoc==NULL) return "";
//	if(pDoc->db_config==NULL) return "";


//	int addhour,addmin;
//	addhour=addmin=0;
	//	sscanf(pDoc->db_config->szBeginTime,"%d:%d",&addhour,&addmin);
//	swscanf(m_szBeginTime,L"%d:%d",&addhour,&addmin);

	return GetPath(m_strPhotoPath,m_checkYsd);
}

CString CDBGraphicMain::GetTextPath(int addday)
{

//	int addhour,addmin;
//	addhour=addmin=0;
	//	sscanf(pDoc->db_config->szBeginTime,"%d:%d",&addhour,&addmin);
//	swscanf(m_szBeginTime,L"%d:%d",&addhour,&addmin);
	return GetPath(m_strFilePath,m_checkYsd);
}

CString &CDBGraphicMain::GetCurTime(CString &strCurTime)
{
	COleDateTime odtCur = COleDateTime::GetCurrentTime();
	strCurTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), odtCur.GetYear(), odtCur.GetMonth()
		, odtCur.GetDay(), odtCur.GetHour(), odtCur.GetMinute(), odtCur.GetSecond());
	return strCurTime;
}

CString &CDBGraphicMain::GetCurDate(CString &strCurDate)
{
	COleDateTime odtCur = COleDateTime::GetCurrentTime();
	strCurDate.Format(_T("%04d-%02d-%02d"), odtCur.GetYear(), odtCur.GetMonth()
		, odtCur.GetDay());
	return strCurDate;
}

CString &CDBGraphicMain::GetTime(CString &strTime)
{
	COleDateTime odtCur = COleDateTime::GetCurrentTime();
	strTime.Format(_T("%02d:%02d:%02d"),odtCur.GetHour(), odtCur.GetMinute(), odtCur.GetSecond());
	return strTime;
}

en_ACCESSDBRET CDBGraphicMain::LogTransFile( wchar_t *szFile ,long lCount,int nSize, DISTFILEITEM &logItem,int iDay)
{

	CString szPhotoName;
	CString szRealSrcPhoto;
	m_bCopyFileSuccess = TRUE;
	wchar_t szDate[26]=L"";
	CString szTempStr;
	CString szSrcFileTemp;
	szSrcFileTemp.Format(L"%s\\%s", m_szSourcePath, szFile );
	CTime time;
	CString strLastModifyTime;
	CFileFind  file; 
	BOOL   bFind=file.FindFile(szSrcFileTemp);
	if(bFind)
		file.FindNextFile();
	else return FAILED_FALSE;
	file.GetLastWriteTime(time);
	strLastModifyTime.Format(L"%04d-%02d-%02d %02d:%02d:%02d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());	
	file.Close();
	CString strRealFilePath,strFileTime;
	DisplayTraceMessage(L"S check dat file record ",0,FALSE);
	DISTFILEITEM item;
	BOOL bIsErrorCorrected = FALSE;
	for(int i = 0; i < m_AgentDistMedia.MediaDistArr.GetSize(); i++)
	{
		item = m_AgentDistMedia.MediaDistArr.GetAt(i);
		strRealFilePath=item.strFilePath;
		strFileTime=item.strFileTime;
		strFileTime.TrimLeft();
		strFileTime.TrimRight();
		strRealFilePath.TrimLeft();
		strRealFilePath.TrimRight();	
		for(int i =0;i<m_errorFile;i++){
			CString curDate,curTime,strtmp;
			CString path1,str;
			CFileFind fil;
			CTime time;
			path1=(CString)fileList[i].szFile;
			path1.MakeLower();
			ERRORFILE error =  fileList[i];
			if(!lstrcmp(path1,m_szFullName)&&fileList[i].err_Count<m_Eml)
			{
				bIsErrorCorrected = TRUE;
				// Edit Stephen  2021-12-23 Comment
				//fileList[i].err_Count = m_Eml+1;
				// Edit Stephen  2021-12-23
				break;
			}
			
			if((!bIsErrorCorrected&&i>=m_errorFile)||(!lstrcmp(szSrcFileTemp, path1)&&fileList[i].err_Count>=m_Eml))
				return FAILED_FALSE;
		}
		if(bIsErrorCorrected)
			break;
		if (db_config->bCheckTime){
			if((lstrcmp(strRealFilePath, szSrcFileTemp) == 0)&&(lstrcmp(strFileTime, strLastModifyTime) == 0)&&!bIsErrorCorrected)
			{
				m_disFile++;
				DisplayTraceMessage(L"Same file ",0,FALSE);
				return FILEEXISTED; // the file has been distributed
			}
		}
		else {
			if((lstrcmp(strRealFilePath, szSrcFileTemp) == 0)&&!bIsErrorCorrected)
			{
			m_disFile++;
			DisplayTraceMessage(L"Same file ",0,FALSE);
			return FILEEXISTED; // the file has been distributed
			}
		}
	}
	DisplayTraceMessage(L"E check dat file record  ",0,FALSE);
	if(num_copy==m_MaxTm)
		return FAILED_FALSE;
	///沒有處理過﹐開始處理
	//char szServerFile[260];
	CString szSrcFile;
	CString szTextFile;
	CString szBmpFile;
	CString szDesFile;
	CString szNote;
	CString szRealSrcFile;
	CString szTime;
	CString strTextSrcFile,strTextDestFile;
	szTime=strLastModifyTime;
	CString strFileName;
	CString strFile;
	strFile=szFile;
	strFileName=strFile.Left(strFile.ReverseFind('.'));
	szSrcFile.Format(L"%s\\%s", m_szSourcePath, szFile );
	strTextSrcFile.Format(L"%s\\%s.txt", m_szSourcePath, strFileName );
	
	szRealSrcFile.Format(L"%s\\%s", m_szSourcePath, szFile );

// Edit Stephen  2014-01-06
// For convert EPS to JPG temp file

	DeleteTempEPSFile();

	CTime cTime=CTime::GetCurrentTime();
	CString strTempCurTime;
	strTempCurTime.Format(_T("%02d%02d%02d"), cTime.GetHour(), cTime.GetMinute(),cTime.GetSecond());;
//	m_strConvertEPSPath = strFile + strTempCurTime;

	CString strAgencyCode = db_config->szCode;
	m_strConvertEPSPath = strAgencyCode + strTempCurTime;

	ImageDlg->SetTempConvertFileName(m_strConvertEPSPath);
// Edit Stephen  2014-01-06

//	SetFileAttributes(szSrcFile,GetFileAttributes(szSrcFile) & ~FILE_ATTRIBUTE_READONLY);
	if(NeedCopy())
	{	
		CString strServerPath;
		strServerPath=GetServerPath(iDay);
		strServerPath.MakeLower();
		CString strMsg;
		szDesFile.Format(L"%s\\%s",strServerPath,szFile);
		strTextDestFile.Format(L"%s\\%s.txt",strServerPath,strFileName);
		if(_taccess(strServerPath, 02) == -1)
		{
			m_strDisplayMessage.Format(L"[%s] Failed to access %s\r\n",db_config->szCode,strServerPath);
			DisplayErrorMessage ( m_strDisplayMessage,NULL,1);
			return NOTACCESSPATH;
		}

		int nPos=-1;
		CString strTmpFile;
		strTmpFile = szDesFile;
		nPos = strTmpFile.ReverseFind(_T('.'));

		DisplayTraceMessage(L"Start check database record ",0,FALSE);
		DisplayTraceMessage(L"End check database record ",0,FALSE);
		en_ACCESSDBRET ret = p_dbCmd->ImageIsLogged( szRealSrcFile,szTime,db_config->bCheckTime);
		if(ret == SUCCESS_TRUE)
		{
			logItem.strFilePath=szRealSrcFile;
			logItem.strFileTime=szTime;
			logItem.strMediaName=L"File Exist";
			m_exist++;
			return FILEEXISTEDWIHTNOLOG;
		}
		else if(ret == ACCESSDBERROR)
		{
			return ACCESSDBERROR;
		}
		m_stcProcessingFile.SetWindowTextW(L"正在處理:\n"+szSrcFileTemp);
		if(!ImageDlg->LoadSourceImage(szSrcFile))
		{
			m_strDisplayMessageHeader.LoadStringW(IDS_LOADSRCPHOTOERROR);
			m_strDisplayMessage.Format(L"%s %s\r\n",m_strDisplayMessageHeader,szSrcFile );
			if(CheckErrFile())
				DisplayErrorMessage( m_strDisplayMessage,NULL,display);		

			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

			return FAILED_FALSE;
					}

		CString filename=(CString)szDesFile;
		if(!m_strTargetPathO.IsEmpty()){
			checkPhotoName(filename,filename.Right(filename.GetLength()-filename.ReverseFind('.')-1),szPhotoName,OrgPhoto);
			szDesFile=filename;
		}
		m_bCopyFileSuccess = CopyFile(szSrcFile, szDesFile, FALSE ) ;
		if ( m_bCopyFileSuccess == FALSE )
		{                     
			DWORD code;
			code=GetLastError();
			m_strDisplayMessage.Format(L"Trans file %s to %s Failed,code=%d \r\n",szSrcFile,szDesFile,code );
			if(CheckErrFile())
				DisplayErrorMessage ( m_strDisplayMessage,NULL,display);
			m_szFullName=szSrcFile;

			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

			return FAILED_FALSE;	
		}
		else
		{
			szRealSrcPhoto=szSrcFile;
		}
		
		if (_waccess(strTextSrcFile,0)==0)
		{
			m_bCopyFileSuccess = CopyFile(strTextSrcFile,strTextDestFile,FALSE);
			if ( m_bCopyFileSuccess == FALSE )
			{                     
				DWORD code;
				code=GetLastError();
				m_strDisplayMessage.Format(L"Trans file %s to %s Failed,code=%d \r\n",strTextSrcFile,strTextDestFile,code );
				DisplayErrorMessage ( m_strDisplayMessage,NULL,display);
			}
		}
	}
	else
	{
		szDesFile=szSrcFile;
		en_ACCESSDBRET ret = p_dbCmd->ImageIsLogged(szRealSrcFile,szTime,db_config->bCheckTime);
		if(ret == SUCCESS_TRUE)
		{
			m_exist++;
		//	DisplayMessage(L"image exists1 \r\n",0);
			//swprintf ( szTemp , L"this image %s is existed in database",szDesFile);
			//DisplayErrorMessage ( szTemp,0,display);//leon
			return FAILED_FALSE;
		}
		else if(ret == ACCESSDBERROR)
		{
			return ACCESSDBERROR;
		}
		else m_szFullName=szSrcFile;
	}
	m_szServerFile=szDesFile;
	///m_szServerFile is File on Server,
	//szSrcFile  is Source File, and they may be same.
	/////////////////////////////////
	CString str=szFile;
	int nFind=str.ReverseFind('.');
	CString  str1,strOrigExt;
	if(nFind!=-1)
	{
		str1=str.Left(nFind);
		strOrigExt = str.Mid(nFind+1);
	}
	else
	{
		strOrigExt.Empty();
		str1=str;
	}
	//edit by leon 2012-11-23 for define temporary path in INI instead of hardcode.
	//CString str_temp = AfxGetApp()->GetProfileString(L"System", L"TemporaryPath", L"C:\\FOUNDER\\TEMP");
	//const wchar_t* wc_tmp = str_temp;
	//lstrcpy(m_szPhotoDesPath,wc_tmp);
	m_szPhotoDesPath=m_szSystemTempPath;
	//end leon 2012-11-23
	//modify by seesea 2003-07-07
	PhotoInfo photoInfo;
	memset(&photoInfo,0,sizeof(PhotoInfo));
	BOOL bIsFrh;
	CString strMidFile=_T(""),strSmallPath=_T("");
	BOOL bMidFile = FALSE;
	bIsFrh=IsSupportedPic(szDesFile);

	CString strFreeMemory;
	MEMORYSTATUSEX MemStat;

	if(bIsFrh)//end modify
	{
		if(db_config->bAutoBitmap)
		{
			DisplayTraceMessage(L"S check photo directory",0,FALSE);
			CString strCurTime;
			//	swprintf ( szBmpFile , MAX_COUNT,L"%s\\%s.jpg" ,m_szPhotoDesPath,strCurTime);//, str1); 
			CTime cTime=CTime::GetCurrentTime();
			strCurTime.Format(_T("%02d%02d%02d"), cTime.GetHour(), cTime.GetMinute(),cTime.GetSecond());
			m_strLocalSourceFile.Format(L"%s\\%s_O%s.jpg",m_szPhotoDesPath,db_config->szCode,strCurTime);	//local source file 
			szBmpFile.Format(L"%s\\%s_S%s.jpg" ,m_szPhotoDesPath,db_config->szCode,strCurTime);				//local small file
		//	swprintf ( szBmpFile , MAX_COUNT,L"%s\\wwwwtemp.jpg" ,m_szPhotoDesPath);//, str1); 

		//	int addhour,addmin;
		//	addhour=addmin=0;
		//	swscanf(db_config->szBeginTime,L"%d:%d",&addhour,&addmin);
			CString strReturn;
			BOOL bMakeDir=FALSE;
			if(bCreate && bMakeDir && (db_config->bTransMidPhoto))
			{
				m_strDisplayMessage.Format(L"[%s] Failed to access %s \r\n", db_config->szCode,strReturn);
				DisplayErrorMessage ( m_strDisplayMessage,NULL,1);
	
			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31
				
				return NOTACCESSPATH;
			}
			strReturn=GetPath(db_config->szSmallPath,m_checkYsd);
			strReturn.MakeLower();
			bMakeDir = MakeDir(strReturn);
		//	if(_waccess(strReturn,0)==0)
		//		bMakeDir=TRUE;
		//	else
		//		bMakeDir=FALSE;
			if(bCreate && (!bMakeDir))
			{
				m_strDisplayMessage.Format(L"[%s] Failed to access %s \r\n",db_config->szCode,strReturn);
				DisplayErrorMessage ( m_strDisplayMessage,NULL,1);
	
			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31
				
				return NOTACCESSPATH;
			}
			/*if(!m_strTargetPathO.IsEmpty()&&!szPhotoName.IsEmpty()){
				strSmallPath.Format(_T("%s\\%s_s.jpg"),strReturn,szPhotoName);
				checkPhotoName(strSmallPath,strOrigExt,szPhotoName,SmlPhoto);}
			else strSmallPath.Format(_T("%s\\%s_s.jpg"),strReturn,str1);*/
			if(!m_strTargetPathO.IsEmpty()&&!szPhotoName.IsEmpty())
			{
				strSmallPath.Format(_T("%s\\%s_%s_s.jpg"),strReturn,szPhotoName,strOrigExt);
				checkPhotoName(strSmallPath,strOrigExt,szPhotoName,SmlPhoto);
			}
			else 
				strSmallPath.Format(_T("%s\\%s_%s_s.jpg"),strReturn,str1,strOrigExt);
			DisplayTraceMessage(L"E check photo directory",0,FALSE);
			szDesFile.Format(L"%s\\%s" ,m_szSourcePath, szFile ); 
			CString filename;
			filename=szDesFile;
			CString ext;//(FindExtension(filename));
			ext=filename.Right(filename.GetLength()-filename.ReverseFind('.')-1);
			ext.MakeLower();
			int nWidth=m_nWidth;//AfxGetApp()->GetProfileInt("ImageSize","Widh",100);
			int nHeight=m_nHeight;//AfxGetApp()->GetProfileInt("ImageSize","Height",100);
			BOOL bRemove=FALSE;
			if(_taccess(szBmpFile, 0) == 0)
			{
				if(_tremove(szBmpFile) != 0)
				{
					m_strDisplayMessage.Format(L"Failed to delete temp thumbnail file<%s> when transfer file<%s>, skip"
						, szBmpFile, szDesFile);
					if(CheckErrFile())
						DisplayErrorMessage(m_strDisplayMessage,NULL,display);

			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

					return FAILED_FALSE;
				}
			}

			//**Handle local source file**//
			if(_taccess(m_strLocalSourceFile, 0) == 0)
			{
				SetFileAttributes(m_strLocalSourceFile,GetFileAttributes(m_strLocalSourceFile) & ~FILE_ATTRIBUTE_READONLY);
				if(_tremove(m_strLocalSourceFile) != 0)
				{
					m_strDisplayMessage.Format( L"Failed to delete temp local file<%s> when transfer file<%s>, skip"
						, m_strLocalSourceFile, szDesFile);
					if(CheckErrFile())
						DisplayErrorMessage(m_strDisplayMessage,NULL,display);

			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

					return FAILED_FALSE;
				}
			}
			//**End**//
			if(bCreate)
			{
				//**Change network convert picture to local convert**//
				if ( CopyFile ( szDesFile , m_strLocalSourceFile,FALSE ) == FALSE )
				{                     
					m_strDisplayMessageHeader.LoadStringW(IDS_TRANSFERFILEFAIL);
					m_strDisplayMessage.Format(L"[%s]: From %s to %s \r\n",m_strDisplayMessageHeader,szDesFile,m_strLocalSourceFile);
					if(CheckErrFile())
						DisplayErrorMessage ( m_strDisplayMessage,NULL,display);

			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

					return FAILED_FALSE ;
				}
				
	//			if(ImageDlg->LoadSourceImage(szDesFile))
				if(ImageDlg->LoadSourceImage(m_strLocalSourceFile))
				//**End**//
				{
					BOOL bIsSuccess = TRUE;
					if(db_config->bTransMidPhoto)
					{
						/*
						if(_taccess(m_lpdfs->szMidPath, 02) == -1)
						{
							sprintf (szTemp, "\r\nFailed to access %s\r\n", m_lpdfs->szMidPath);
							DisplayErrorMessage ( szTemp);
							return NOTACCESSPATH;
						}
						*/
						strReturn=GetPath(db_config->szMidPath,m_checkYsd);
						strReturn.MakeLower();
						 bMakeDir = MakeDir(strReturn);
						/*if(!m_strTargetPathO.IsEmpty()&&!szPhotoName.IsEmpty() && db_config->bTransMidPhoto == 1){
							strMidFile.Format(_T("%s\\%s_m.jpg"),strReturn,szPhotoName);
							checkPhotoName(strMidFile,strOrigExt,szPhotoName,MidPhoto);}
						else strMidFile.Format(_T("%s\\%s_m.jpg"),strReturn,str1);*/
						if(!m_strTargetPathO.IsEmpty()&&!szPhotoName.IsEmpty() && db_config->bTransMidPhoto == 1)
						{
							strMidFile.Format(_T("%s\\%s_%s_m.jpg"),strReturn,szPhotoName, strOrigExt);
							checkPhotoName(strMidFile,strOrigExt,szPhotoName,MidPhoto);
						}
						else 
							strMidFile.Format(_T("%s\\%s_%s_m.jpg"),strReturn,str1, strOrigExt);

						if(_taccess(strMidFile, 0) == 0)
						{
							if(_tremove(strMidFile) != 0)
							{
								m_strDisplayMessage.Format(L"Failed to delete temp middle file<%s> when transfer file<%s>, skip"
									, strMidFile, szDesFile);
								if(CheckErrFile())
									DisplayErrorMessage(m_strDisplayMessage,NULL,display);

			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

								return FAILED_FALSE;
							}
						}
						//DisplayMessage(L"Begin convert mid image.\r\n",0);
						//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
						//bIsSuccess = ImageDlg->ConvertImage((unsigned short *)(LPCTSTR)strMidFile,m_nMidWidth, m_nMidHeight, &photoInfo,TRUE);
						
						MemStat.dwLength = sizeof(MEMORYSTATUSEX);
						GlobalMemoryStatusEx(&MemStat);
						strFreeMemory.Format(L"%s: 系統可用記憶體: %lu KB", db_config->szTeleName, MemStat.ullAvailPhys / 1024L);
						DisplayTraceMessage(strFreeMemory,0,FALSE);

						DisplayTraceMessage(L"Start convert Middle photo ",0,FALSE);
						bIsSuccess = ImageDlg->ConvertImage(strMidFile,
									m_nMidWidth, m_nMidHeight, &photoInfo,TRUE);
						DisplayTraceMessage(L"End convert Middle photo ",0),FALSE;

						
						MemStat.dwLength = sizeof(MEMORYSTATUSEX);
						GlobalMemoryStatusEx(&MemStat);
						strFreeMemory.Format(L"%s: 系統可用記憶體: %lu KB", db_config->szTeleName, MemStat.ullAvailPhys / 1024L);
						DisplayTraceMessage(strFreeMemory,0,FALSE);

						if(!bIsSuccess)
						{
							CString strFormatMsg;
							strFormatMsg.LoadString(IDS_CREATEBITMAPFAIL);
							CString strErrMsg;
							strErrMsg.Format(_T("%s %s\r\n"), strFormatMsg, strMidFile);
							const wchar_t* wc_tmp = strErrMsg;
							if(bRemove)
								_wremove(szDesFile);

			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

							return FAILED_FALSE;
						}


						bMidFile = TRUE;
					}
					else strMidFile=L"";
					/*
					if(_taccess(m_lpdfs->szSmallPath, 02) == -1)
					{
						sprintf (szTemp, "\r\nFailed to access %s\r\n", m_lpdfs->szSmallPath);
						DisplayErrorMessage ( szTemp);
						return NOTACCESSPATH;
					}
					*/

					/*
					 *	Add by Bob 2006-4-11
						在每次轉圖之前，先Load源圖。
						因為Dbgraphic經常在外電分發的時候LeadTool報錯"Lead Error:Invalid bitmap handle."，懷疑是下面讀取圖片信息的時候
						出現的問題，否則不應該報錯。我們在Load, Size, Save的時候都進行了錯誤處理。
					 */
				//	DisplayMessage(L"Begin load source image.\r\n",display);
					/*Edit by Leon Li 2013-02-19 
					Change from network conversion to local conversion
					if(!ImageDlg->LoadSourceImage(szDesFile))
					*/
					
					MemStat.dwLength = sizeof(MEMORYSTATUSEX);
					GlobalMemoryStatusEx(&MemStat);
					strFreeMemory.Format(L"%s: 系統可用記憶體: %lu KB", db_config->szTeleName, MemStat.ullAvailPhys / 1024L);
					DisplayTraceMessage(strFreeMemory,0,FALSE);

					DisplayTraceMessage(L"Start convert Small photo ",0,FALSE);
					if(!ImageDlg->LoadSourceImage(m_strLocalSourceFile))
					{
						m_strDisplayMessageHeader.LoadStringW(IDS_LOADSRCPHOTOERROR);
						m_strDisplayMessage.Format(L"%s %s\r\n",m_strDisplayMessageHeader,szDesFile );
						if(CheckErrFile())
							DisplayErrorMessage( m_strDisplayMessage,NULL,display);
			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

						return FAILED_FALSE;
					}
					
					bIsSuccess = TRUE;
					bIsSuccess = ImageDlg->ConvertImage(szBmpFile, nWidth, nHeight, &photoInfo,TRUE);
					DisplayTraceMessage(L"End convert Small photo ",0,FALSE);

					
					MemStat.dwLength = sizeof(MEMORYSTATUSEX);
					GlobalMemoryStatusEx(&MemStat);
					strFreeMemory.Format(L"%s: 系統可用記憶體: %lu KB", db_config->szTeleName, MemStat.ullAvailPhys / 1024L);
					DisplayTraceMessage(strFreeMemory,0,FALSE);

					if(!bIsSuccess)
					{
						m_strDisplayMessageHeader.LoadStringW(IDS_CREATEBITMAPFAIL);
						m_strDisplayMessage.Format(L"%s %s\r\n",m_strDisplayMessageHeader,szBmpFile );
						if(CheckErrFile())
							DisplayErrorMessage ( m_strDisplayMessage,NULL,display);
						if(bRemove)
							_wremove(szDesFile);		

			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

						return FAILED_FALSE;
					}
				}
				else //Fail to open src photo
				{
					m_strDisplayMessageHeader.LoadStringW(IDS_LOADSRCPHOTOERROR);
					m_strDisplayMessage.Format(L"%s %s\r\n",m_strDisplayMessageHeader,szBmpFile );
					if(CheckErrFile())
						DisplayErrorMessage( m_strDisplayMessageHeader,NULL,display);
			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

					return FAILED_FALSE;
				}
				
			}
			else
			{
				m_strDisplayMessageHeader.LoadStringW(IDS_CREATECONVDLGFAIL);
				m_strDisplayMessage.Format(L"%s %s\r\n",m_strDisplayMessageHeader,szBmpFile );
				if(CheckErrFile())
					DisplayErrorMessage( m_strDisplayMessage,NULL,display);
				if(bRemove)
					_wremove(szDesFile);

			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

				return FAILED_FALSE;
			}
			if(bRemove)
				_wremove( szDesFile);
			/////////////////////////////////////////
		//	LoadString(AfxGetApp()->m_hInstance,IDS_CREATEBITMAPSUCCESS,
//				szTemp5,50);
		//	swprintf ( szTemp , L"%s %s\r\n",szTemp5,szBmpFile );
			//		sprintf( szTemp, "\r\nCreate Bitmapfile :%s Success\r\n", szBmpFile );
		//	DisplayErrorMessage( szTemp,NULL,display);
			szSrcFile=szBmpFile;
//			swprintf(errTmp,L"Finish convert image \r\n");
//			DisplayMessage(errTmp,0);

		}		//轉圖完畢
		else
		{
			//不會執行到的代碼
			szDesFile.Format(L"%s\\%s.bmp" ,m_szPhotoDesPath, str1); 
			szBmpFile.Format(L"%s\\%s.bmp", m_szBmpSrcPath, str1 );
			if ( CopyFile ( szBmpFile , szDesFile,FALSE ) == FALSE )
			{                     
				//lstrcpy(szTemp, GetFileTitleWord( szSrcFile ,m_lpdfs->nTitleLine , &lWord));
				m_fileLog.Write ( szBmpFile , lstrlen ( szBmpFile));
				DisplayErrorMessage (szBmpFile,NULL,display);

				m_strDisplayMessageHeader.LoadStringW(IDS_TRANSFERFILEFAIL);
				m_strDisplayMessage.Format(L"%s\r\n",m_strDisplayMessageHeader);
				if(CheckErrFile())
					DisplayErrorMessage ( m_strDisplayMessage,NULL,display);

			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

				return FAILED_FALSE ;
			}
			szSrcFile= szDesFile;
		}
	}
	else 
	{
			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

		return FAILED_FALSE;
	}
	int iTextPathExists = 0;
	int iTextPathNet = 0;
	CheckPath(db_config->szTextPath, iTextPathExists, iTextPathNet);
	if(iTextPathExists != 1)
	{
		m_strDisplayMessage.Format(L"The path %s is not existed!", m_szTxtSrcPath);
		DisplayErrorMessage ( m_strDisplayMessage,NULL,display);
			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31
		return FAILED_FALSE ;
	}	
	MakeDir(m_szTxtSrcPath);
	str=db_config->szTextExtName;
	nFind=str.ReverseFind('.');
	if(nFind!=-1)
	{
		szTextFile.Format(L"%s\\%s.%s",m_szTxtSrcPath,str1,str.Right(str.GetLength()-nFind-1));
		szDesFile.Format(L"%s\\%s.%s" ,m_szPhotoDesPath,str1,str.Right(str.GetLength()-nFind-1)); 
	}
	else
	{
		szDesFile.Format(L"%s\\%s.%s" ,m_szPhotoDesPath,str1,db_config->szTextExtName); 
		szTextFile.Format(L"%s\\%s.%s",m_szTxtSrcPath,str1,db_config->szTextExtName);
	}
	if(!NeedCopy())
		szRealSrcPhoto.Format(L"%s.%s",str1,m_strExt);
	int bNeedRemove=0;
	/*Read photo caption and write to file then copy to destination*/
	DisplayTraceMessage(L"Start GetFileCaption ",0,FALSE);
	CString strAddtionalInfo,strPhotoCatpion=L"",strCaption;
	if(szRealSrcFile.Find(L'_3_')>-1&& m_strExt == L"eps")
	{
		CString strTempXmlPath;
		strTempXmlPath = szRealSrcFile;
		strTempXmlPath.Replace(L"_3_", L"_0_");
		strTempXmlPath.Replace(L".eps", L".xml");
		if (_waccess(strTempXmlPath,0)==0)
		{
			if(!ReadReutersXML(strPhotoCatpion, strTempXmlPath))
			{
				strPhotoCatpion=GetCaption();
			}
		}
	}
	int iType = 0;
	if(strPhotoCatpion.GetLength()==0)
	{
		if (!GetFileCaption(szTextFile,strPhotoCatpion,iType)){
			DisplayTraceMessage(L"Start LoadPhotoCaption ",0,FALSE);
			if (!ImageDlg->LoadPhotoCaption(m_strLocalSourceFile,strPhotoCatpion))
				strPhotoCatpion=GetCaption();
			DisplayTraceMessage(L"End LoadPhotoCaption ",0,FALSE);
		}
		DisplayTraceMessage(L"End GetFileCaption ",0,FALSE);
	}
	if (m_bLoadAddInfo){
		if (ImageDlg->LoadPhotoAdditionalInfo(m_strLocalSourceFile,strAddtionalInfo))
			strCaption=strPhotoCatpion+strAddtionalInfo;
	}	
	else strCaption=strPhotoCatpion;
	CString strDestCapFile;
	strDestCapFile=m_szServerFile.Left(m_szServerFile.ReverseFind('.'));
	strDestCapFile+=L".txt";
	DisplayTraceMessage(L"Start WriteFileCaption ",0,FALSE);
	CFile fil ;
	CFileException err;
	try
	{
		if(fil.Open(szTextFile, CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone ,&err))
		{
			fil.SeekToBegin();
			if(iType == 3)
			{
				int len = WideCharToMultiByte(CP_ACP,NULL,strCaption,-1,NULL,0,NULL,FALSE);
				char *temp = new char[len+1];
				WideCharToMultiByte (CP_ACP,NULL,strCaption,-1,temp,len+1,NULL,FALSE);
				/*char temp[51200]={0};
				ZeroMemory(temp, sizeof(temp));
				int len = WideCharToMultiByte(CP_ACP, 0, strCaption, -1, temp, sizeof(temp), NULL, NULL);*/
				fil.Write(temp, len);
			}
			else
			{
				fil.Write("\xFF\xFE", sizeof(wchar_t));
				fil.Write(strCaption, strCaption.GetLength()*sizeof(wchar_t));
			}
			fil.Close();
		}
		else
		{
			TCHAR szErr[255];
			err.GetErrorMessage(szErr,254);
			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31
			return FAILED_FALSE;
		}

	}
	catch (...) 
	{
		fil.Close();
		CString strMessage;
		strMessage.Format(_T("Failed to read/write the file of %s."), szTextFile);

			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

		return FAILED_FALSE;
	}
	DisplayTraceMessage(L"End WriteFileCaption ",0,FALSE);
// Edit Stephen 2013-09-18
//	if (strDestCapFile!=szTextFile)
	if (strDestCapFile.MakeUpper() !=szTextFile.MakeUpper())
	{
// End Edit Stephen 2013-09-18
		DisplayTraceMessage(L"Start CopyPhoto ",0,FALSE);
		copyPhoto(szTextFile,strDestCapFile);
		DisplayTraceMessage(L"End CopyPhoto ",0,FALSE);
	}
	//End reading caption and copy file to destination

	CString strTmp1;
	DisplayTraceMessage(L"Start GetTime ",0,FALSE);
	if(db_config->iType==1||db_config->bUseFolderDate==1){
		//edit by Leon 2013-01-25 for change photo time from current day +1 to current day
		//CTime  time1  =   CTime::GetCurrentTime()   +   CTimeSpan(1,0,0,0); 
		/*CTime  time1;
		//Edit by Leon Li 2013-02-25 for checking yesterday folder, the photo time should by yesterday
		if (m_checkYsd)
			  time1  =   CTime::GetCurrentTime() - CTimeSpan(1,0,0,0); 
		else  time1  =   CTime::GetCurrentTime(); 
		//end Leon 2013-01-25
		strTmp1.Format(L"%4d-%2d-%2d %2d:%2d:%2d",time1.GetYear(),time1.GetMonth(),time1.GetDay(),time1.GetHour(),time1.GetMinute(),time1.GetSecond());	*/
		strTmp1=m_strDbgStartTime;
	}
	else if(db_config->iType==0){
		//edit by Leon 2013-01-25 for change photo time from current day +1 to current day
		//time =time+ CTimeSpan(1,0,0,0); 
		//end Leon 2013-01-25
		strTmp1.Format(L"%4d-%2d-%2d %2d:%2d:%2d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
	}
	DisplayTraceMessage(L"End GetTime ",0,FALSE);
	//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 
	//	en_ACCESSDBRET ret = TransFile(szRealSrcFile,&photoInfo,szTextFile,szSrcFile,szFile,(unsigned short *)(LPCTSTR)strMidFile,(unsigned short *)(LPCTSTR)strSmallPath,nSize,iDay,strTmp1,strTmp2);

	en_ACCESSDBRET ret = TransFile(szRealSrcFile,&photoInfo,szTextFile,szSrcFile,szFile
		,(wchar_t *)(LPCTSTR)strMidFile,(wchar_t *)(LPCWSTR)strSmallPath,nSize,iDay,strTmp1,strLastModifyTime);
	if((ret == ACCESSDBERROR) || (ret == FAILED_FALSE))
	{
			// Edit Stephen 2013-12-31
			DeleteTempEPSFile();
			// Edit Stephen 2013-12-31

		return ret;
	}

	if(bNeedRemove)
		_wremove(szTextFile);
	/*CFileFind filfind;
	CString strFileDeletePath;
	strFileDeletePath.Format(L"%s\\*.jpg",m_szPhotoDesPath);
	bFind=filfind.FindFile(strFileDeletePath);
	while (bFind)
	{
		bFind=filfind.FindNextFile();
		strFileName=filfind.GetFileName();
		if (strFileName.Find(db_config->szCode)>-1)
		{
			SetFileAttributes(filfind.GetFilePath(),GetFileAttributes(filfind.GetFilePath()) & ~FILE_ATTRIBUTE_READONLY);
			_tremove(filfind.GetFilePath());
		}
	}*/
	/*
	DisplayTraceMessage(L"S delete src SMALL File",0);
	if(_taccess(szBmpFile, 0) == 0)
	{
		_tremove(szBmpFile);
	}
	DisplayTraceMessage(L"E delete src small File",0);

	DisplayTraceMessage(L"S delete src File",0);
	if(_taccess(m_strLocalSourceFile, 0) == 0)
	{
		_tremove(m_strLocalSourceFile);
	}
	DisplayTraceMessage(L"E delete src File",0);
	*/
	m_fileName.InsertAt(num_copy,szRealSrcPhoto);
	num_copy++;
	m_stcProcessingFile.SetWindowTextW(L"成功處理:\n"+szSrcFileTemp);
	logItem.strFilePath=szRealSrcFile;
	logItem.strFileTime=strLastModifyTime;
	logItem.strMediaName=szFile;

	// Edit Stephen 2014-01-08 
	DeleteTempEPSFile();
	// Edit Stephen 2014-01-08 

	return SUCCESS_TRUE;
}

void CDBGraphicMain::WriteTransLogFile(const LOGDISTRIBUTEDITEM &logItem)
{
	if(m_fileDat.m_hFile != CFile::hFileNull)
	{
		LOGDISTRIBUTEDHEAD logHead;
		//		m_fileDat.SeekToBegin();
		// read unicode txt file
		m_fileDat.Seek(2, CFile::begin);		
		m_fileDat.Read(&logHead, sizeof(LOGDISTRIBUTEDHEAD));
		logHead.lTotal++;
		m_ulTransFileNum = logHead.lTotal;
		
		m_fileDat.SeekToBegin();
		
		WORD wSignature = 0xFEFF;// use unicode encoding
		m_fileDat.Write(&wSignature, 2);//Unicode的文件符?
		
		m_fileDat.Write(&logHead, sizeof(logHead));
		m_fileDat.Seek( 
			sizeof(logHead) + sizeof(logItem) * (m_ulTransFileNum - 1) + 2,
			CFile::begin);
		m_fileDat.Write(&logItem, sizeof(logItem));
	}
}


CTime CDBGraphicMain::GetConvertTime()
{
	CTime tDate,tDateNow;
	tDate = CTime::GetCurrentTime();
	CString strTemp=db_config->szStartTime;
	strTemp.TrimRight();
	int Hour,Min,Scd;
	swscanf_s(db_config->szStartTime,L"%02d:%02d:%02d",&Hour,&Min,&Scd);
	tDateNow = tDate - CTimeSpan (0 , Hour , Min , Scd );
	return tDateNow;	
}



CString CDBGraphicMain::GetPath(CString strFormat,int checkYsd)
{
	int i,year,month,day;
	int *iIncludeDay=0;
	if(iIncludeDay) *iIncludeDay=0;
	CString ysdDate;
	CTime today;
	CString strCurTime;
	strCurTime.Format(L"%04d-%02d-%02d",CTime::GetCurrentTime().GetYear(),CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetDay());
	CString strReturn;
	/*if (m_strDbgStartTime!=strCurTime)
		checkYsd=1;*/
	if(checkYsd){
		getYesterday(ysdDate);
		swscanf_s(ysdDate,_T("%04d-%02d-%02d"),&year,&month,&day);
		CTime tm(year,month,day,0,0,0);
		today=tm;
	}
	else{
			today=CTime::GetCurrentTime();
	}
//	today=CTime::GetCurrentTime();
	//p_dbCmd->GetServerTime(today);
	CString s,t;
	TCHAR ch;
	s="";
	int a=strFormat.GetLength();
	for(i=0;i<strFormat.GetLength();i++)
	{
		ch=strFormat.GetAt(i);
		if(ch=='%')
		{
			i++;
			ch=strFormat.GetAt(i);
			switch(ch)
			{
			case 'y':
				t.Format(L"%02d",today.GetYear()%100);
				if(iIncludeDay) *iIncludeDay=1;
				break;
			case 'Y':
				t.Format(L"%04d",today.GetYear());
				if(iIncludeDay) *iIncludeDay=1;
				break;
			case 'm':
			case 'M':
				t.Format(L"%02d",today.GetMonth());
				if(iIncludeDay) *iIncludeDay=1;
				break;
			case 'd':
			case 'D':
				t.Format(L"%02d",today.GetDay());
				if(iIncludeDay) *iIncludeDay=1;
				break;
			case 'W':
			case 'w':
				t.Format(L"%d",today.GetDayOfWeek());
				if(iIncludeDay) *iIncludeDay=1;
				break;
				/*case 't':
				case 'T':
				t=m_szUser;
				t.TrimRight();
				t.TrimLeft();
				break;*/
			default:
				t=CString(ch);
			}
			s+=t;
		}else
			s+=CString(ch);
	}
	if(s.GetLength()>1)
		while(s.GetAt(s.GetLength()-1)=='\\')
			s=s.Left(s.GetLength()-1);
		return s;
}

BOOL CDBGraphicMain::CheckLogInXML(wchar_t *szFile)   // if the file has been output, return true.
{
	
	HRESULT hr = m_plDomDocumentCheck.CreateInstance(MSXML::CLSID_DOMDocument);
	if (FAILED(hr))
	{
		_com_error er(hr);
		//AfxMessageBox(er.ErrorMessage());
	}
	
	wchar_t szXMLFile[PATH_LENGTH1];
	wchar_t szTemp[PATH_LENGTH1];
	CString strFileName;
	strFileName = szFile;
	
	wchar_t szSrcFile[ PATH_LENGTH ];
	swprintf( szSrcFile,MAX_COUNT, L"%s\\%s", m_szSourcePath, szFile );
	
	wchar_t szXMLFileName[ PATH_LENGTH ];
	CString strSrcFile;
	CString strSrcPath;
	CString strXMLFileName;
	CString strFile;
	strFile = szFile;
	strSrcFile = szSrcFile;
	strSrcPath = db_config->szSourcePath;
	strSrcPath.Format(strSrcPath.Left(strSrcPath.Find(L"%")-1));
	strXMLFileName = strSrcFile.Right( strSrcFile.GetLength() - strSrcPath.GetLength());
	if ( strSrcFile.GetLength() - strSrcPath.GetLength() - strFile.GetLength() > 1)
		swprintf( szXMLFileName, MAX_COUNT,strXMLFileName);
	else
		swprintf( szXMLFileName, MAX_COUNT,strFile);
	
	
	if ( db_config->bXMLByAgency == 0 )
	{
		strFileName = strFileName.Left( strFileName.ReverseFind(L'.') );
		swprintf( szXMLFile,MAX_COUNT, L"%s\\%s.xml", db_config->szXMLPath, strFileName );
		variant_t vLoad = m_plDomDocumentCheck->load(szXMLFile);
		
		if (((bool)vLoad) == TRUE )
		{
			
			swprintf( szTemp,MAX_COUNT, L"The XML File of %s has been output!  \r\n", szFile );
			DisplayMessage( szTemp,0); 
			return TRUE;
		}
	}
	else
	{
		swprintf( szXMLFile, MAX_COUNT,L"%s\\%s.xml", db_config->szXMLPath, db_config->szTeleCode );
		variant_t vLoad = m_plDomDocumentCheck->load(szXMLFile);
		
		if (((bool)vLoad) == TRUE )
		{
			m_pDocRoot = m_plDomDocumentCheck->documentElement;
			
			
			// simple for loop to get all children
			CString strAttributeFileName;
			
			//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
			//for (IXMLDOMNodePtr m_pFile = m_pDocRoot->firstChild;
			for (MSXML::IXMLDOMNodePtr m_pFile = m_pDocRoot->firstChild;
			NULL != m_pFile;
			m_pFile = m_pFile->nextSibling)
			{
				for (int i=0; i < m_pFile->Getattributes()->Getlength(); i++)
				{	 
					MSXML::IXMLDOMNodePtr pAttr = m_pFile->Getattributes()->Getitem(i); 
					
					CString strNodeText(pAttr->text.copy());
					
					if ( strNodeText == szXMLFileName )
					{
						swprintf( szTemp,MAX_COUNT, L"The XML File of %s has been output!  \r\n", szSrcFile );
						DisplayMessage( szTemp,display);
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
	
}

//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
//void MainFrame::XMLFileNodeGen(wchar_t *szFile , IXMLDOMElementPtr pDocElementAgency,int nSize, int iDay)
void CDBGraphicMain::XMLFileNodeGen(wchar_t *szFile , MSXML::IXMLDOMElementPtr pDocElementAgency,int nSize, int iDay)
{

	HRESULT hr = docPtr.CreateInstance(MSXML::CLSID_DOMDocument);
	if (FAILED(hr))
	{
		 _com_error er(hr);
		 //AfxMessageBox(er.ErrorMessage());
	}
	//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
// 	IXMLDOMElementPtr pDocElementFile = m_plDomDocument->createElement("Photo");
// 	IXMLDOMElementPtr pDocElementTitle = m_plDomDocument->createElement("Photo_Title");
// 	IXMLDOMElementPtr pDocElementContent = m_plDomDocument->createElement("Photo_Caption");

	MSXML::IXMLDOMElementPtr pDocElementFile = m_plDomDocument->createElement("Photo");
	MSXML::IXMLDOMElementPtr pDocElementTitle = m_plDomDocument->createElement("Photo_Title");
	MSXML::IXMLDOMElementPtr pDocElementContent = m_plDomDocument->createElement("Photo_Caption");
	wchar_t szSrcFile[ PATH_LENGTH1 ];
	wchar_t szXMLSave[ MAX_FILESPECIAL ];
//	wchar_t szSrcPath[ PATH_LENGTH ];
	wchar_t szTemp[ TEMP_STRING_LENGTH ];
	swprintf( szSrcFile, MAX_COUNT,L"%s\\%s", m_szSourcePath, szFile );

	
	if ( db_config->bXMLByAgency == 1 )
	{	
		wchar_t drive[_MAX_DRIVE];
		wchar_t dir[_MAX_DIR];
		wchar_t fname[_MAX_FNAME];
		wchar_t ext[_MAX_EXT];
		DWORD dwLength;
		LPBYTE hpBuffer ;
		CString strBuffer;
		CFile fil ;
		HGLOBAL hglbFil;
		int nYear,nMonth,nDay;
		nYear=nMonth=nDay=0;

		// XML "File" Attribute
		pDocElementAgency->appendChild(pDocElementFile);


		// File ID in xml
		wchar_t szChildCount[ 1000 ];
		int nChildCount =0;
		//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
		//for (IXMLDOMNodePtr m_pFile = pDocElementAgency->firstChild;
		for (MSXML::IXMLDOMNodePtr m_pFile = pDocElementAgency->firstChild;
		NULL != m_pFile;
		m_pFile = m_pFile->nextSibling)
		{						
			nChildCount++;
		}
		swprintf( szChildCount, MAX_COUNT,L"%d", nChildCount );

		pDocElementFile->setAttribute("File_ID", szChildCount);   // Michael custom


		wchar_t szXMLFileName[ PATH_LENGTH ];
		CString strSrcFile;
		CString strSrcPath;
		CString strXMLFileName;
		CString strFile;
		strFile = szFile;
		strSrcFile = szSrcFile;
		strSrcPath = db_config->szSourcePath;
		strSrcPath.Format(strSrcPath.Left(strSrcPath.Find(L"%")-1));
		strXMLFileName = strSrcFile.Right( strSrcFile.GetLength() - strSrcPath.GetLength());
		swprintf( szXMLFileName,MAX_COUNT, strXMLFileName);
		if ( strSrcFile.GetLength() - strSrcPath.GetLength() - strFile.GetLength() > 1)
			pDocElementFile->setAttribute("Photo_Name", szXMLFileName);
		else
			pDocElementFile->setAttribute("Photo_Name", szFile);


		// File Mid picture path
		CString str=szFile;
		int nFind=str.ReverseFind('.');
		CString  str1,strOrigExt;
		if(nFind!=-1)
		{
			str1=str.Left(nFind);
			strOrigExt = str.Mid(nFind+1);
		}
		else
		{
			strOrigExt.Empty();
			str1=str;
		}
		//edit by leon 2012-11-23 for define temporary path in INI instead of hardcode.
		//CString str_temp = AfxGetApp()->GetProfileString(L"System", L"TemporaryPath", L"C:\\FOUNDER\\TEMP");
		//const wchar_t* wc_tmp = str_temp;
		//lstrcpy(m_szPhotoDesPath,wc_tmp);
		CString str_temp;
		const wchar_t* wc_tmp;
		m_szPhotoDesPath=m_szSystemTempPath;
		//end leon 2012-11-23
		PhotoInfo photoInfo;
		memset(&photoInfo,0,sizeof(PhotoInfo));
//		BOOL bIsFrh;
		CString strMidFile=_T(""),strSmallPath=_T("");
		BOOL bMidFile = FALSE;


	//	int addhour,addmin;
	//	addhour=addmin=0;
	//	swscanf(db_config->szBeginTime,L"%d:%d",&addhour,&addmin);
		CString strReturn=GetPath(db_config->szMidPath,m_checkYsd);
		strReturn.MakeLower();
		if (db_config->bTransMidPhoto == 1)
		{
			BOOL bMakeDir = MakeDir(strReturn);
			if(bCreate && (!bMakeDir) && (db_config->bTransMidPhoto))
			{
				swprintf ( szTemp, MAX_COUNT,L"Failed to access %s\r\n", strReturn);
				DisplayErrorMessage ( szTemp);
				return;
			}
			strMidFile.Format(_T("%s\\%s_%s.jpg"),strReturn,str1,strOrigExt);
		}
		else
			strMidFile = "";
		

		
		
			
			
		strReturn=GetPath(db_config->szSmallPath,m_checkYsd);
		strReturn.MakeLower();
		BOOL bMakeDir = MakeDir(strReturn);
		if(bCreate && (!bMakeDir))
		{
			swprintf ( szTemp,MAX_COUNT, L"Failed to access %s\r\n", strReturn);
			DisplayErrorMessage ( szTemp);
			return;
		}
		
		strSmallPath.Format(_T("%s\\%s_%s.jpg"),strReturn,str1,strOrigExt);

		

		wchar_t szFileSize[33];
		swprintf(szFileSize,MAX_COUNT, L"%d", nSize);    
		pDocElementFile->setAttribute("Photo_Size", szFileSize);
		

		//Get photo width by leadtool
		long lWidth = ImageDlg->GetPhotoWidth(szSrcFile);     // Need tune 
		swprintf(szTemp,MAX_COUNT, L"%d", lWidth);
		pDocElementFile->setAttribute("Photo_Width", szTemp);

		// Get photo heigth by leadtool
		long lHeight= ImageDlg->GetPhotoHeight(szSrcFile);     // Need Tune  Michael 
		swprintf(szTemp, MAX_COUNT,L"%d", lHeight);

		pDocElementFile->setAttribute("Photo_Height", szTemp);


		// Set mid path
		swprintf(szTemp,MAX_COUNT, strMidFile);
		pDocElementFile->setAttribute("Mid_Photo_Path", szTemp);

		// Set Small path
		swprintf(szTemp, MAX_COUNT,strSmallPath);
		pDocElementFile->setAttribute("Small_Photo_Path", szTemp);
		
		//Get File Create Date
		CString strCDate = L""; // The parameter of the create time of file (szSourceFile)
		BOOL bRet = FALSE;
		HANDLE hFile = CreateFile(szSrcFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		_wsplitpath_s ( szSrcFile , drive , dir , fname , ext );

		CString strTemp=db_config->szBeginTime;
		strTemp.TrimRight();

		strTemp = strTemp.Left(strTemp.Find(':'));
		wc_tmp = strTemp;
		int nDelayHour=_wtoi(wc_tmp);

		strTemp=db_config->szBeginTime; // 要重新提取數值, 不然一直拿提取過後的小時值來進行
		strTemp.TrimRight();

		strTemp = strTemp.Right(strTemp.GetLength()-1-strTemp.ReverseFind(':'));
		wc_tmp = strTemp;
		int nMin=_wtoi(wc_tmp);

		nDelayHour = -nDelayHour;
		nMin = -nMin;



		if (hFile != INVALID_HANDLE_VALUE)
		{
			FILETIME ftCreation;
			FILETIME ftLastAccessTime;
			FILETIME ftLastWrite;
 
			SYSTEMTIME stUTC, stLocal;
			bRet = GetFileTime(hFile,&ftCreation, &ftLastAccessTime, &ftLastWrite);
			FileTimeToSystemTime(&ftCreation, &stUTC);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
 
			CString str_CreateTime;
			int LocalHour = stLocal.wHour + nDelayHour;
			int LocalMin = stLocal.wMinute + nMin;
			str_CreateTime.Format(L"%d-%.2d-%.2d %02d:%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay, LocalHour , LocalMin);
			CloseHandle(hFile);
			strCDate = str_CreateTime;
		}
		pDocElementFile->setAttribute("File_Create_Date", (const class _variant_t)strCDate );
		
		//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
		//IXMLDOMNodePtr NodeCData;
		MSXML::IXMLDOMNodePtr NodeCData;


		// prepare for caption (including title)
		wchar_t szTextFile[PATH_LENGTH1 ]={0};
		wchar_t szDesFile[PATH_LENGTH]={0};
		wchar_t szCaption[5120]; // Michael 09012012
		int iTextPathExists = 0;
		int iTextPathNet = 0;
		int iCaptionLoad = 0;
		CheckPath(db_config->szTextPath, iTextPathExists, iTextPathNet);
		if(iTextPathExists != 1)
		{
			
			wchar_t sztempbuf[512];
			swprintf(sztempbuf,MAX_COUNT,L"The path %s is not existed!", m_szTxtSrcPath);
			DisplayErrorMessage ( sztempbuf);
			return ;
		}	
		MakeDir(m_szTxtSrcPath);
		str=db_config->szTextExtName;
		nFind=str.ReverseFind('.');
		if(nFind!=-1)
		{
			swprintf( szTextFile,MAX_COUNT, L"%s\\%s.%s",m_szTxtSrcPath,str1,str.Right(str.GetLength()-nFind-1));
			swprintf ( szDesFile ,MAX_COUNT, L"%s\\%s.%s" ,m_szPhotoDesPath,str1,str.Right(str.GetLength()-nFind-1)); 
		}
		else
		{
			swprintf ( szDesFile ,MAX_COUNT, L"%s\\%s.%s" ,m_szPhotoDesPath,str1,db_config->szTextExtName); 
			swprintf( szTextFile,MAX_COUNT, L"%s\\%s.%s",m_szTxtSrcPath,str1,db_config->szTextExtName);
		}
		int bNeedRemove=0;
		if(_waccess(szTextFile , 0)==-1)
		{
			bNeedRemove=1;
			wchar_t szBuffer[5120];
			memset(szBuffer,0,5120);
			CString strType;
			CString s;
			CString strCap;
			s=m_szServerFile;
			int nFind=s.ReverseFind('.');
			if(nFind!=-1)
				strType=s.Right(s.GetLength()-nFind-1);
			strType.MakeLower( );
			s="";


			try
			{
				//modify iptc
				BOOL bOk = FALSE;				
				/*CIPTC iptc;
				
				if (iptc.Load(szSrcFile))
				{
					if (iptc.GetCaptionAbstract(strCap))
					{
						strCap.TrimLeft();
						bOk = TRUE;
					}
				}
				*/
				if (!bOk)
				{
					CString strMessage;
					strMessage.Format(_T("IPTC info error: %s.\r\n"), szSrcFile);
					const wchar_t* wc_tmp = strMessage;
				//	DisplayMessage(wc_tmp,display);
				}
			}
			catch (...) 
			{
				CString strMessage;
				strMessage.Format(_T("Failed to read/write the file of %s."), szSrcFile);
				const wchar_t* wc_tmp = strMessage;
				WriteStringEx(wc_tmp);
				return;
			}


			s=strCap+s;
			
			int nCount = s.GetLength();
			//strcpy(szBuffer,s);
			memset(szBuffer,0,5120);

			CString str_temp = s;
			const wchar_t* wc_tmp = str_temp;
			wcsncpy_s(szBuffer,wc_tmp, 5100);

			if(lstrlen(szBuffer)<4)
			{
				CString strCaption=GetCaption();
				if(strCaption.GetLength()<4)
					swprintf(szBuffer,MAX_COUNT,L"沒有說明");
				else
				{
					CString str_temp = strCaption;
					const wchar_t* wc_tmp = str_temp;
					lstrcpy(szBuffer,wc_tmp);
				}
				iCaptionLoad = 1;
			}
			CFile fil ;
			CFileException err;
			try
			{
				if(fil.Open(szTextFile, CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone ,&err))
				{

					fil.SeekToBegin();
					WORD wSignature = 0xFEFF;// use unicode encoding
					fil.Write(&wSignature, 2);//Unicode的文件符?

/*					// useless when the file is in UTF case   -- Michael 6-1-2011
					char temp[5120]={0};
					ZeroMemory(temp, sizeof(temp));
					int len = WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, temp, sizeof(temp), NULL, NULL);
*/					
					swprintf( szCaption,MAX_COUNT, szBuffer );
					fil.Write(szBuffer, lstrlen(szBuffer)*2);
					fil.Close();
				}
				else
				{
					TCHAR szErr[255];
					err.GetErrorMessage(szErr,254);
					
					CString str_temp = szErr;
					const wchar_t* wc_tmp = str_temp;
					WriteStringEx(wc_tmp);
					return;
				}
			}
			catch (...) 
			{
				fil.Close();
				CString strMessage;
				strMessage.Format(_T("Failed to read/write the file of %s."), szTextFile);
				const wchar_t* wc_tmp = strMessage;
				WriteStringEx(wc_tmp);
				return ;
			}
//			swprintf(szTemp, szBuffer);
		}


		// Get File Title 
		wchar_t szTitle[TEMP_STRING_LENGTH];
		long  lDelay=0L,lWord=0L;
		bool isUnicode = false; 

		_wsplitpath_s ( szTextFile , drive , dir , fname , ext );
		if ( fil.Open ( szTextFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone ) == 0 )
		{
			strBuffer.LoadString(AFX_IDS_OPENNOTORG);
			CString str_temp = strBuffer;
			const wchar_t* wc_tmp = str_temp;
			WriteStringEx(wc_tmp);
			return;
		}
		fil.SeekToBegin();

		dwLength = (DWORD)fil.GetLength();
		if(dwLength == 0)
		{
			fil.Close();
		}
		else
		{
			WORD wSignature;
			fil.Read(&wSignature, 2);
			if(wSignature == 0xFEFF)
//			if((head[0]==0xFF&&head[1]==0xFE) || (head[0]==0xFE&&head[1]==0xFF))
//			if(head == 0xFEFF)
			{
				isUnicode = true;
//				fil.Seek(2, CFile::begin);  // reading a unicode file and skip the heading of unicode
				fil.SeekToBegin();
			}
			else
				fil.SeekToBegin();

			if ( (hglbFil = GlobalAlloc ( GHND , dwLength + 30) ) == NULL )
			{
//				strBuffer.LoadString(AFX_IDS_NOTMEMORY);

	//			CString str_temp = strBuffer;
	//			const wchar_t* wc_tmp = str_temp;
		//		WriteStringEx(wc_tmp);
				fil.Close() ;
				return;
			}

			hpBuffer = ( LPBYTE) GlobalLock ( hglbFil ) ;
			//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
			//fil.ReadHuge ( hpBuffer  , dwLength ) ;
			fil.Read ( hpBuffer  , dwLength ) ;
			hpBuffer[dwLength] = '\0';
			fil.Close();

			int nTitle = 0;

			if (iCaptionLoad == 1)  //if Caption is not load from file
				nTitle = 1;
			else
				nTitle = db_config->nTitleLine ;


			if (isUnicode)
				lstrcpy ( szTitle , GetTeleTitleWordW ( hpBuffer , dwLength-4 , nTitle ,  &lWord , &lDelay)) ;
			else
			{
				CHAR *pszAnsi = new CHAR[dwLength];
				strcpy(pszAnsi, GetTeleTitleWord ( hpBuffer , dwLength , nTitle ,  &lWord , &lDelay));
					
				WCHAR * szwBuffer = new WCHAR[dwLength*2];

				MultiByteToWideChar(CP_ACP, 0, pszAnsi, -1, szwBuffer, dwLength*2+1);

				lstrcpy ( szTitle , szwBuffer) ;
				delete pszAnsi;
				delete szwBuffer;
			}
			szTitle[80] = '\0' ;
		}
		// CData format of File Title
		
		NodeCData = docPtr->createCDATASection(szTitle);
		pDocElementFile->appendChild(pDocElementTitle);
		pDocElementTitle->appendChild(NodeCData);



		// XML "Content" Attribute
		pDocElementFile->appendChild(pDocElementContent);

		if ( fil.Open ( szTextFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone ) == 0 )
		{
			strBuffer.LoadString(AFX_IDS_OPENNOTORG);
			const wchar_t * wc_tmp =  strBuffer;
			WriteStringEx(wc_tmp);
			return ;
		}                               



		wchar_t szWord[10];

		if (isUnicode)
		{
			fil.Seek(2, CFile::begin); // unicode function 
			dwLength = (DWORD)fil.GetLength();
			if(dwLength == 0)
				return ;

			wchar_t szContent [5120];
			fil.Read(szContent, dwLength);
			str_temp = L"";
			for (int i=0; i < (int)dwLength; i++) 
				str_temp = str_temp + szContent[i];

		}
		else
		{
			fil.SeekToBegin(); // ansi function

			dwLength = (long)fil.GetLength();
			if(dwLength == 0)
				return ;
			
			char* lpszBuffer;
			lpszBuffer = new char[dwLength];    
			//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
			//swprintf(szTemp , (const unsigned short *)lpszBuffer);
			swprintf(szTemp ,MAX_COUNT,(LPCWSTR)lpszBuffer); // for testing
			fil.Read(lpszBuffer,dwLength);		
			wchar_t* szContent = new wchar_t[dwLength];
			MultiByteToWideChar(CP_ACP, 0, lpszBuffer, -1, szContent, dwLength);
			
			str_temp = L"";
			for (int i=0; i < (int)dwLength; i++) 
				str_temp = str_temp + szContent[i];

			delete lpszBuffer;
			delete szContent;
		}

		fil.Close();
		_ltow_s(dwLength/2, szWord, 10);  

		wchar_t * pContent = (wchar_t *)(LPCTSTR)str_temp;
		pDocElementContent->setAttribute("Number_Of_Words", szWord);

		NodeCData = docPtr->createCDATASection(pContent); 
//		NodeCData = docPtr->createCDATASection(szCaption); 
		pDocElementContent->appendChild(NodeCData);
		if(bNeedRemove)
			_wremove(szTextFile);

	}
		
	// XML Output by Every Files
	if ( db_config->bXMLByFile == 1 )
	{
		HRESULT hr = m_plDomDocument.CreateInstance(MSXML::CLSID_DOMDocument);
		if (FAILED(hr))
		{
			 _com_error er(hr);
			 //AfxMessageBox(er.ErrorMessage());
		}

		wchar_t drive[_MAX_DRIVE];
		wchar_t dir[_MAX_DIR];
		wchar_t fname[_MAX_FNAME];
		wchar_t ext[_MAX_EXT];
		DWORD dwLength;
		LPBYTE hpBuffer ;
		CString strBuffer;
		CFile fil ;
		HGLOBAL hglbFil;
		int nYear,nMonth,nDay;
		nYear=nMonth=nDay=0;

		// XML Parameter use
		wchar_t szTemp[ TEMP_STRING_LENGTH ];

		// XML Information Setting
		m_plDomDocument->appendChild(m_plDomDocument->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

		// Get System time for Filling the Create date of XML
		SYSTEMTIME st;
		GetSystemTime(&st);

		// XML Elements Create
		//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
		//IXMLDOMElementPtr pDocElementAgency = m_plDomDocument->createElement("Agency");
		MSXML::IXMLDOMElementPtr pDocElementAgency = m_plDomDocument->createElement("Agency");
		m_plDomDocument->appendChild(pDocElementAgency);
			
		// XML "Agency" Attribute
		pDocElementAgency->setAttribute("Agency_Name", db_config->szTeleName);
		pDocElementAgency->setAttribute("Agency_Code", db_config->szTeleCode);

		// Get System Date to be the create date of the XML                                   Michael Change for dbg
		CString strRootPath = db_config->szSourcePath;
		strRootPath.Format(strRootPath.Left(strRootPath.Find(L"%")-1));
		swprintf(szTemp, MAX_COUNT,strRootPath);
		pDocElementAgency->setAttribute("Source_Path", szTemp);

		// ServerPath of 
		CString strServerPath;
		strServerPath=GetServerPath(iDay);
		swprintf(szTemp,  MAX_COUNT,strServerPath);
		pDocElementAgency->setAttribute("Target_OutPut_Path", szTemp);
				
		CString strDate;
		strDate.Format(L"%d-%.2d-%.2d %d:%d",st.wYear,st.wMonth,st.wDay, st.wHour+8, st.wMinute);
		pDocElementAgency->setAttribute("Last_Modified_Date", (const class _variant_t)strDate);

		pDocElementAgency->setAttribute("Total_Number_Of_File", L"1");		

		// XML "File" Attribute
		pDocElementAgency->appendChild(pDocElementFile);

		// File ID in xml
		pDocElementFile->setAttribute("File_ID", L"1");

		wchar_t szXMLFileName[ PATH_LENGTH ];
		CString strSrcFile;
		CString strSrcPath;
		CString strXMLFileName;
		CString strFile;
		strFile = szFile;
		strSrcFile = szSrcFile;
		strSrcPath = db_config->szSourcePath;
		strSrcPath.Format(strSrcPath.Left(strSrcPath.Find(L"%")-1));
		strXMLFileName = strSrcFile.Right( strSrcFile.GetLength() - strSrcPath.GetLength());
		swprintf( szXMLFileName, MAX_COUNT, strXMLFileName);
		if ( strSrcFile.GetLength() - strSrcPath.GetLength() - strFile.GetLength() > 1)
			pDocElementFile->setAttribute("Photo_Name", szXMLFileName);
		else
			pDocElementFile->setAttribute("Photo_Name", szFile);


		// File Mid picture path
		CString str=szFile;
		int nFind=str.ReverseFind('.');
		CString  str1,strOrigExt;
		if(nFind!=-1)
		{
			str1=str.Left(nFind);
			strOrigExt = str.Mid(nFind+1);
		}
		else
		{
			strOrigExt.Empty();
			str1=str;
		}

		//edit by leon 2012-11-23 for define temporary path in INI instead of hardcode.
		//CString str_temp = AfxGetApp()->GetProfileString(L"System", L"TemporaryPath", L"C:\\FOUNDER\\TEMP");
		//const wchar_t* wc_tmp = str_temp;
		//lstrcpy(m_szPhotoDesPath,wc_tmp);
		CString str_temp;
		const wchar_t* wc_tmp;
		m_szPhotoDesPath=m_szSystemTempPath;
		//end leon 2012-11-23
		PhotoInfo photoInfo;
		memset(&photoInfo,0,sizeof(PhotoInfo));
		CString strMidFile=_T(""),strSmallPath=_T("");
		BOOL bMidFile = FALSE;


	//	int addhour,addmin;
	//	addhour=addmin=0;
//		swscanf(db_config->szBeginTime,L"%d:%d",&addhour,&addmin);
		CString strReturn=GetPath(db_config->szMidPath,m_checkYsd);
		strReturn.MakeLower();
		if (db_config->bTransMidPhoto == 1)
		{
			BOOL bMakeDir = MakeDir(strReturn);
			if(bCreate && (!bMakeDir) && (db_config->bTransMidPhoto))
			{
				swprintf ( szTemp, MAX_COUNT, L"Failed to access %s\r\n", strReturn);
				DisplayErrorMessage ( szTemp);
				return;
			}
			strMidFile.Format(_T("%s\\%s_%s.jpg"),strReturn,str1,strOrigExt);
		}
		else
			strMidFile = "";
		

				
		strReturn=GetPath(db_config->szSmallPath,m_checkYsd);
		strReturn.MakeLower();
		BOOL bMakeDir = MakeDir(strReturn);
		if(bCreate && (!bMakeDir))
		{
			swprintf ( szTemp, MAX_COUNT, L"Failed to access %s\r\n", strReturn);
			DisplayErrorMessage ( szTemp);
			return;
		}
		
		strSmallPath.Format(_T("%s\\%s_%s.jpg"),strReturn,str1,strOrigExt);

		

		wchar_t szFileSize[33];
		swprintf(szFileSize, MAX_COUNT, L"%d", nSize);    // on consturt Michael*
		
		pDocElementFile->setAttribute("Photo_Size", szFileSize);
		

		//Get photo width by leadtool
		long lWidth = ImageDlg->GetPhotoWidth(szSrcFile);     // Need tune 
		swprintf(szTemp, MAX_COUNT, L"%d", lWidth);
		pDocElementFile->setAttribute("Photo_Width", szTemp);

		// Get photo heigth by leadtool
		long lHeight= ImageDlg->GetPhotoHeight(szSrcFile);     // Need Tune  Michael 
		swprintf(szTemp, MAX_COUNT, L"%d", lHeight);

		pDocElementFile->setAttribute("Photo_Height", szTemp);


		// Set mid path
		swprintf(szTemp, MAX_COUNT, strMidFile);
		pDocElementFile->setAttribute("Mid_Photo_Path", szTemp);

		// Set Small path
		swprintf(szTemp, MAX_COUNT, strSmallPath);
		pDocElementFile->setAttribute("Small_Photo_Path", szTemp);
		
		//Get File Create Date
		CString strCDate = L""; // The parameter of the create time of file (szSourceFile)
		BOOL bRet = FALSE;
		HANDLE hFile = CreateFile(szSrcFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		_wsplitpath_s ( szSrcFile , drive , dir , fname , ext );

		CString strTemp=db_config->szBeginTime;
		strTemp.TrimRight();

		strTemp = strTemp.Left(strTemp.Find(':'));
		wc_tmp = strTemp;
		int nDelayHour=_wtoi(wc_tmp);

		strTemp=db_config->szBeginTime; // 要重新提取數值, 不然一直拿提取過後的小時值來進行
		strTemp.TrimRight();

		strTemp = strTemp.Right(strTemp.GetLength()-1-strTemp.ReverseFind(':'));
		wc_tmp = strTemp;
		int nMin=_wtoi(wc_tmp);

		nDelayHour = -nDelayHour;
		nMin = -nMin;



		if (hFile != INVALID_HANDLE_VALUE)
		{
			FILETIME ftCreation;
			FILETIME ftLastAccessTime;
			FILETIME ftLastWrite;
 
			SYSTEMTIME stUTC, stLocal;
			bRet = GetFileTime(hFile,&ftCreation, &ftLastAccessTime, &ftLastWrite);
			FileTimeToSystemTime(&ftCreation, &stUTC);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
 
			CString str_CreateTime;
			int LocalHour = stLocal.wHour + nDelayHour;
			int LocalMin = stLocal.wMinute + nMin;
			str_CreateTime.Format(L"%d-%.2d-%.2d %02d:%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay, LocalHour , LocalMin);
			CloseHandle(hFile);
			strCDate = str_CreateTime;
		}
		pDocElementFile->setAttribute("File_Create_Date", (const class _variant_t)strCDate );

		//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
		//IXMLDOMNodePtr NodeCData;
		MSXML::IXMLDOMNodePtr NodeCData;


		// prepare for caption (including title)
		wchar_t szTextFile[PATH_LENGTH1 ]={0};
		wchar_t szDesFile[PATH_LENGTH]={0};
		wchar_t szCaption[5120]; // Michael 09012012
		int iTextPathExists = 0;
		int iTextPathNet = 0;
		int iCaptionLoad = 0;
		CheckPath(db_config->szTextPath, iTextPathExists, iTextPathNet);
		if(iTextPathExists != 1)
		{
			wchar_t sztempbuf[512];
			swprintf(sztempbuf, MAX_COUNT,L"The path %s is not existed!", m_szTxtSrcPath);
			DisplayErrorMessage ( sztempbuf);
			return ;
		}	
		MakeDir(m_szTxtSrcPath);
		str=db_config->szTextExtName;
		nFind=str.ReverseFind('.');
		if(nFind!=-1)
		{
			swprintf( szTextFile, MAX_COUNT, L"%s\\%s.%s",m_szTxtSrcPath,str1,str.Right(str.GetLength()-nFind-1));
			swprintf ( szDesFile , MAX_COUNT, L"%s\\%s.%s" ,m_szPhotoDesPath,str1,str.Right(str.GetLength()-nFind-1)); 
		}
		else
		{
			swprintf ( szDesFile , MAX_COUNT, L"%s\\%s.%s" ,m_szPhotoDesPath,str1,db_config->szTextExtName); 
			swprintf( szTextFile, MAX_COUNT, L"%s\\%s.%s",m_szTxtSrcPath,str1,db_config->szTextExtName);
		}
		int bNeedRemove=0;
		if(_waccess(szTextFile , 0)==-1)
		{
			bNeedRemove=1;
			wchar_t szBuffer[5120];
			memset(szBuffer,0,5120);
			CString strType;
			CString s;
			CString strCap;
			s=m_szServerFile;
			int nFind=s.ReverseFind('.');
			if(nFind!=-1)
				strType=s.Right(s.GetLength()-nFind-1);
			strType.MakeLower( );
			s="";

			try
			{
				BOOL bOk = FALSE;	
				//modify iptc
				/*
				CIPTC iptc;
				if (iptc.Load(szSrcFile))
				{
					if (iptc.GetCaptionAbstract(strCap))
					{
						strCap.TrimLeft();
						bOk = TRUE;
					}
				}
				*/
				if (!bOk)
				{
					CString strMessage;
					strMessage.Format(_T("IPTC info error: %s.\r\n"), szSrcFile);
					const wchar_t* wc_tmp = strMessage;
				//	DisplayMessage(wc_tmp,display);
				}
			}
			catch (...) 
			{
				CString strMessage;
				strMessage.Format(_T("Failed to read/write the file of %s."), szSrcFile);
				const wchar_t* wc_tmp = strMessage;
				WriteStringEx(wc_tmp);
				return;
			}
			
			s=strCap+s;
			
			int nCount = s.GetLength();
			//strcpy(szBuffer,s);
			memset(szBuffer,0,5120);

			CString str_temp = s;
			const wchar_t* wc_tmp = str_temp;
			wcsncpy_s(szBuffer,wc_tmp, 5100);

			if(lstrlen(szBuffer)<4)
			{
				CString strCaption=GetCaption();
				if(strCaption.GetLength()<4)
					swprintf(szBuffer, MAX_COUNT,L"沒有說明");
				else
				{
					CString str_temp = strCaption;
					const wchar_t* wc_tmp = str_temp;
					lstrcpy(szBuffer,wc_tmp);
				}
				iCaptionLoad = 1;
			}
			CFile fil ;
			CFileException err;
			try
			{
				if(fil.Open(szTextFile, CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone ,&err))
				{

					fil.SeekToBegin();
					WORD wSignature = 0xFEFF;// use unicode encoding
					fil.Write(&wSignature, 2);//Unicode的文件符?

/*					// useless when the file is in UTF case   -- Michael 6-1-2011
					char temp[5120]={0};
					ZeroMemory(temp, sizeof(temp));
					int len = WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, temp, sizeof(temp), NULL, NULL);
*/					
					swprintf( szCaption,  MAX_COUNT,szBuffer );
					fil.Write(szBuffer, lstrlen(szBuffer)*2);
					fil.Close();
				}
				else
				{
					TCHAR szErr[255];
					err.GetErrorMessage(szErr,254);
					
					CString str_temp = szErr;
					const wchar_t* wc_tmp = str_temp;
					WriteStringEx(wc_tmp);
					return;
				}
			}
			catch (...) 
			{
				fil.Close();
				CString strMessage;
				strMessage.Format(_T("Failed to read/write the file of %s."), szTextFile);
				const wchar_t* wc_tmp = strMessage;
				WriteStringEx(wc_tmp);
				return ;
			}
//			swprintf(szTemp, szBuffer);
		}


		// Get File Title 
		wchar_t szTitle[TEMP_STRING_LENGTH];
		long  lDelay=0L,lWord=0L;
		bool isUnicode = false; 

		_wsplitpath_s ( szTextFile , drive , dir , fname , ext );
		if ( fil.Open ( szTextFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone ) == 0 )
		{
			strBuffer.LoadString(AFX_IDS_OPENNOTORG);
			CString str_temp = strBuffer;
			const wchar_t* wc_tmp = str_temp;
			WriteStringEx(wc_tmp);
			return;
		}
		fil.SeekToBegin();

		dwLength = (DWORD)fil.GetLength();
		if(dwLength == 0)
		{
			fil.Close();
		}
		else
		{
			WORD wSignature;
			fil.Read(&wSignature, 2);
			if(wSignature == 0xFEFF)
//			if((head[0]==0xFF&&head[1]==0xFE) || (head[0]==0xFE&&head[1]==0xFF))
//			if(head == 0xFEFF)
			{
				isUnicode = true;
//				fil.Seek(2, CFile::begin);  // reading a unicode file and skip the heading of unicode
				fil.SeekToBegin();
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
				return;
			}

			hpBuffer = ( LPBYTE) GlobalLock ( hglbFil ) ;
			//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
			//fil.ReadHuge ( hpBuffer  , dwLength ) ;
			fil.Read ( hpBuffer  , dwLength ) ;
			hpBuffer[dwLength] = '\0';
			fil.Close();

			int nTitle = 0;

			if (iCaptionLoad == 1)  //if Caption is not load from file
				nTitle = 1;
			else
				nTitle = db_config->nTitleLine ;


			if (isUnicode)
				lstrcpy ( szTitle , GetTeleTitleWordW ( hpBuffer , dwLength-4 , nTitle ,  &lWord , &lDelay)) ;
			else
			{
				CHAR *pszAnsi = new CHAR[dwLength];
				strcpy(pszAnsi, GetTeleTitleWord ( hpBuffer , dwLength , nTitle ,  &lWord , &lDelay));
					
				WCHAR * szwBuffer = new WCHAR[dwLength*2];

				MultiByteToWideChar(CP_ACP, 0, pszAnsi, -1, szwBuffer, dwLength*2+1);

				lstrcpy ( szTitle , szwBuffer) ;
				delete pszAnsi;
				delete szwBuffer;
			}
			szTitle[80] = '\0' ;
		}
		// CData format of File Title
		
		NodeCData = docPtr->createCDATASection(szTitle);
		pDocElementFile->appendChild(pDocElementTitle);
		pDocElementTitle->appendChild(NodeCData);



		// XML "Content" Attribute
		pDocElementFile->appendChild(pDocElementContent);

		if ( fil.Open ( szTextFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone ) == 0 )
		{
			strBuffer.LoadString(AFX_IDS_OPENNOTORG);
			const wchar_t * wc_tmp =  strBuffer;
			WriteStringEx(wc_tmp);
			return ;
		}                               



		wchar_t szWord[10];

		if (isUnicode)
		{
			fil.Seek(2, CFile::begin); // unicode function 
			dwLength = (DWORD)fil.GetLength();
			if(dwLength == 0)
				return ;

			wchar_t szContent [5120];
			fil.Read(szContent, dwLength);
			str_temp = L"";
			for (int i=0; i < (int)dwLength; i++) 
				str_temp = str_temp + szContent[i];

			_ltow_s(dwLength/2, szWord, 10);  

		}
		else
		{
			fil.SeekToBegin(); // ansi function

			dwLength = (long)fil.GetLength();
			if(dwLength == 0)
				return ;
			
			char* lpszBuffer;
			lpszBuffer = new char[dwLength];   
			//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
			//swprintf(szTemp , (const unsigned short *)lpszBuffer
			swprintf(szTemp , MAX_COUNT, (LPCWSTR)lpszBuffer); 
			fil.Read(lpszBuffer,dwLength);
			_ltow_s(dwLength/2, szWord, 10);  
		
			wchar_t* szContent = new wchar_t[dwLength];
			MultiByteToWideChar(CP_ACP, 0, lpszBuffer, -1, szContent, dwLength);
			
			str_temp = L"";
			for (int i=0; i < (int)dwLength; i++) 
				str_temp = str_temp + szContent[i];

			delete lpszBuffer;
			delete szContent;
		}
		fil.Close();
		wchar_t * pContent = (wchar_t *)(LPCTSTR)str_temp;
		pDocElementContent->setAttribute("Number_Of_Words", szWord);

		NodeCData = docPtr->createCDATASection(pContent); 
//		NodeCData = docPtr->createCDATASection(szCaption); 
		pDocElementContent->appendChild(NodeCData);


		if(bNeedRemove)
			_wremove(szTextFile);


		// XML saved by each File
		CString strFileName;
		strFileName = szFile;
		strFileName = strFileName.Left( strFileName.ReverseFind(L'.') );
		swprintf ( szXMLSave , MAX_COUNT,L"%s\\%s.xml" , db_config->szXMLPath , strFileName );
		m_plDomDocument->save( szXMLSave );

	}
}

void CDBGraphicMain::DoWithMessage()
{
	MSG  msg;
	::PeekMessage(&msg,NULL,0,0,PM_REMOVE);
	::TranslateMessage(&msg);
	::DispatchMessage(&msg); 
	//AfxGetMainWnd()->SendMessage(WM_COMMAND,0,0);
}

CString CDBGraphicMain::GetServerPath(int addday)
{
	CString strReturn;
	strReturn=GetPath(m_strTargetPathO,m_checkYsd);
	MakeDir(strReturn);
	return strReturn;
}

void CDBGraphicMain::DisplayErrorMessage(LPCWSTR lpszMsg, LPCWSTR lpszStation,int display)
{
	DisplayMessage(lpszMsg,display);
	return;
// 	wchar_t newline[4];
// 	newline[0] = 0x0D;
// 	newline[1] = 0x0A;
// 	newline[2] = 0;
// 	wchar_t Buffer[512];
// 	time_t NowTime;
// 	time(&NowTime);
// 	lstrcpy(Buffer, lpszMsg);
// 	if(lpszStation)
// 		lstrcat(Buffer, lpszStation);
// 	lstrcat(Buffer, newline);
// 	lstrcat(Buffer, L"               ");
// //	lstrcat(Buffer, _wctime(&NowTime));
// 	if(Buffer[lstrlen(Buffer) - 1] == 0x0A)
// 		Buffer[lstrlen(Buffer) - 1] = '\0';
// 	lstrcat(Buffer, newline);
// 	DisplayMessage(Buffer,display);
}

BOOL CDBGraphicMain::CompareDate(LPCWSTR szFile,wchar_t *szDate)
{                    
	struct _stat FileStat;
	   
	//if(m_lpdfs->bSrcNewDate_Day||m_lpdfs->bSrcNewDate_MD||m_lpdfs->bSrcNewDate_YMD||m_lpdfs->bSrcNewDate_MDY||m_lpdfs->bSrcNewDate_DMY)
	//	return TRUE;
	_wstat(szFile, &FileStat);
	
	// Add by Bob 20091022
	// 在信報發現一個情況, 有一個圖片日期是2078年的,導致CompareDate函數出錯,此時st_mtime==-1。需要過濾這種圖片
	if (FileStat.st_mtime == -1)
		return FALSE;
	lstrcpy(szDate,_wctime(&FileStat.st_mtime));
	LOGDISTRIBUTEDALLITEM Allitem;
	LOGDISTRIBUTEDALLHEAD  Allhead;
	
	time_t NowTime;
	time(&NowTime);
	m_fileAll.SeekToBegin();
	m_fileAll.Read(&Allhead, sizeof(LOGDISTRIBUTEDALLHEAD));
	while( m_fileAll.Read(&Allitem, sizeof(LOGDISTRIBUTEDALLITEM)) == sizeof(LOGDISTRIBUTEDALLITEM))
	{
		if( lstrcmp( Allitem.szFile, szFile) == 0&&lstrcmp(Allitem.szDate, szDate) == 0 )
		{
			if(NowTime > (FileStat.st_mtime + ONEDAY_TIME * (db_config->nKeepDay + 1)))
			{
				_wremove(szFile);
				
			}
			return FALSE; // the file has been distributed
		}
	}
	return TRUE;
}

BOOL CDBGraphicMain::NeedCopy()
{
	//LEON
// 	CDbgraphiDoc * pDoc;
// 	//	pDoc=(CDbgraphiDoc * )GetDocument();
// 	
// 	if(pDoc->db_config->bLocalToServer)
// 	{
// 		
// 		if(pDoc==NULL) return 0;
// 		if(pDoc->db_config==NULL) return 0;
// 		int exist,net;
// 		CheckPath(pDoc->db_config->szDesPath,exist,net);
// 		return !net;
// 	}
// 	return FALSE;
// 	

	int exist,net;
	TCHAR  path[TEMP_STRING_LENGTH] ;
	lstrcpy(path,m_strTargetPathO);
	CheckPath(path,exist,net);
	if(net==1)
		return net;
	else if(exist==1)
		return exist;
	else 
		return 0;
 		
}

BOOL IsSupportedPic(CString strFile)
{
	CString strext;
	strext=strFile.Mid(strFile.ReverseFind('.')+1);
	strext.MakeLower();
	for (int nPos=0;nPos<nSupportPicFormatCount;nPos++)
	{
		if(strext == arraySupportPicFormat[nPos])
			return TRUE;
	}
	return FALSE;
}

BOOL	MakeDir(CString strDir)
{
	strDir.TrimLeft();
	strDir.TrimRight();
	if(_waccess((LPCTSTR )strDir,00)==0) return TRUE;
	if(strDir.GetLength()<2) return FALSE;
	int i;
	i=strDir.ReverseFind('\\');

	if(i<0)
	{
		if(_wmkdir((LPCTSTR )strDir )==0) return TRUE;
		return FALSE;
	}
	CString strLeft;
	strLeft=strDir.Left(i);
	if(MakeDir(strLeft)==FALSE) return FALSE;
	
	CString str_temp = strDir;
	const wchar_t* wc_tmp = str_temp;
	
	if(_wmkdir(wc_tmp )==0) return TRUE;
	return FALSE;
}



BOOL	CheckPath(wchar_t * szPath,int & bExist,int & bNetPath)
{
	bExist=bNetPath=0;
	if(szPath==NULL) {
		return 0;}
	CString s,t;
	s=szPath;
	s.TrimLeft();
	if(s.GetLength()<2) {
		return 0;}
	if(s.Find('%')<0) 
		t=s;
	else
	{
		int i;
		i=s.Find('%');
		for(;i>0 && s.GetAt(i)!='\\';i--) ;
		t=s.Left(i);
	}
	while(t.GetAt(t.GetLength()-1)=='\\')
		t=t.Left(t.GetLength()-1);
	s=t;
	const wchar_t* wc_tmp = s;
	if(_waccess(wc_tmp,00)==0){
		bExist=1;
	}
	else 
		bExist=0;
	s=szPath;
	s.TrimLeft();
	if(s.Left(2)=="\\\\") 
	{
		bNetPath=1; // check path is on local or not. 
		return 1;
	}
	if(s.GetAt(1)!=':'){
		//AfxMessageBox(L":");
		return 0;}
	t=s.Left(2);
	CString lt;
	lt=s.Right(s.GetLength()-2);
	wchar_t temp[512];
	DWORD	len=0;
	if(WNetGetConnection(t,temp,&len)==NO_ERROR)
	{
		s=(CString)temp;
		s.TrimLeft();
		s.TrimRight();
		s+=lt;
		const wchar_t* wc_tmp = s;
		wcsncpy(szPath,wc_tmp,260);
		bNetPath=1;
	}
	return 1;
}

CString CDBGraphicMain::GetCaption()
{
	

	return db_config->szCaption;
}

en_ACCESSDBRET CDBGraphicMain::TransFile(CString szSourthPath,PhotoInfo *pInfo, CString szFile, CString szBmpFile, CString szJpgFile, CString szMidPath, CString szSmallPath,int lSize,int addday,CString szPhotoTime,CString szLastTime)
{
	DisplayTraceMessage(L"Start TransFile ",0,FALSE);
	wchar_t ImageID[20];
	if(p_dbCmd->GetNewImageName(ImageID) == ACCESSDBERROR)
		return ACCESSDBERROR;
	szJpgFile=ImageID;
	m_strDisplayMessageHeader=m_szDatabase;
    CString strTemp=db_config->szBeginTime;
	strTemp.TrimRight();
	strTemp = strTemp.Left(strTemp.Find(':'));
	const wchar_t* wc_tmp = strTemp;
	int nDelayHour=_wtoi(wc_tmp);

	strTemp = strTemp.Right(strTemp.GetLength()-1-strTemp.ReverseFind(':'));
	wc_tmp = strTemp;
	int nMin=_wtoi(wc_tmp);
	en_ACCESSDBRET uI;
	DisplayTraceMessage(L"Start Insert DB ",0,FALSE);
	switch (db_config->iType)
	{
		case 0://tele
			uI=p_dbCmd->InsertValues (pInfo,
				db_config->szTeleCode,
				db_config->nAgencyID,
				db_config->iType, 
				szBmpFile , 
				szFile, 
				szMidPath , 
				szSmallPath,
				nDelayHour,
				nMin,
				szJpgFile,
				db_config->nMarkLine,
				db_config->nTitleLine,
				db_config->bTransfer,
				lSize,
				szLastTime,
				szSourthPath,
				szPhotoTime,
				addday,
				db_config->szBeginTime,
				//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
				//(unsigned short *)(LPCSTR)m_szServerFile/*m_szServerFile*/);
				m_szServerFile/*m_szServerFile*/);
			break;
		case 1://department
			//comment by wxy below editor5
			/*if	(m_lpdfs->szTeleCode[0] !=':')
			{
				sprintf(szDepartment,":%s",m_lpdfs->szTeleCode);
				strcpy(m_lpdfs->szTeleCode,szDepartment);
			}*/

			//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
			//uI=p_dbCmd->InsertValues (pInfo,db_config->szTeleCode,db_config->nAgencyID,db_config->iType,szBmpFile , szFile ,szMidPath,szSmallPath,nDelayHour,nMin,szJpgFile,db_config->nMarkLine,db_config->nTitleLine,db_config->bTransfer,lSize,szLastTime,szSourthPath,szPhotoTime,addday,db_config->szBeginTime,(unsigned short *)(LPCSTR)m_szServerFile);
			uI=p_dbCmd->InsertValues (pInfo,db_config->szTeleCode,db_config->nAgencyID,db_config->iType,szBmpFile , szFile ,szMidPath,szSmallPath,nDelayHour,nMin,szJpgFile,db_config->nMarkLine,db_config->nTitleLine,db_config->bTransfer,lSize,szLastTime,szSourthPath,szPhotoTime,addday,db_config->szBeginTime,m_szServerFile);
			break;
		case 2://private
			//comment by wxy below editor5
			/*if	(m_lpdfs->szTeleCode[0] !=';')
			{
				sprintf(szDepartment,";%s;%s",m_lpdfs->szTeleCode,m_szLogin);
				strcpy(m_lpdfs->szTeleCode,szDepartment);
			}*/
			lstrcpy(db_config->szTeleCode,m_szLogin);//add by wxy editor5
			//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
			//uI=p_dbCmd->InsertValues (pInfo,db_config->szTeleCode,db_config->nAgencyID,db_config->iType,szBmpFile , szFile ,szMidPath,szSmallPath,nDelayHour,nMin,szJpgFile,db_config->nMarkLine,db_config->nTitleLine,db_config->bTransfer,lSize,szLastTime,szSourthPath,szPhotoTime,addday,db_config->szBeginTime,(unsigned short *)(LPCSTR)m_szServerFile);
			uI=p_dbCmd->InsertValues (pInfo,db_config->szTeleCode,db_config->nAgencyID,db_config->iType,szBmpFile , szFile ,szMidPath,szSmallPath,nDelayHour,nMin,szJpgFile,db_config->nMarkLine,db_config->nTitleLine,db_config->bTransfer,lSize,szLastTime,szSourthPath,szPhotoTime,addday,db_config->szBeginTime,m_szServerFile);
			break;
	}
//	changed by vi
//	else uI=p_dbCmd->InsertValues2(m_lpdfs->iType,szBmpFile , szFile ,nDelayHour,nMin,szJpgFile,m_lpdfs->nMarkLine,m_lpdfs->nTitleLine,m_lpdfs->bTransfer,lSize,addday);
	if(uI == SUCCESS_TRUE)
	{
		if (bSupportMedia==TRUE && bMediaColumnExist == TRUE)
			ImageDlg->LoadPhotoMeta(szSourthPath, szJpgFile);
		//swprintf( szTemp, L"%s :\r\n ", szTemp2);
	//	swprintf( szTemp, L"  %s: %s\r\n", szTemp3,szFile );
	//	DisplayMessage( szTemp,0);
	//	swprintf( szTemp, L"  %s  : %s\r\n",szTemp4,L"wires_photo_src");
		//sprintf( szTemp, "  To  : %s\r\n", szDesFile );
	//	DisplayMessage( szTemp,0);

		//sprintf( szTemp, "  To  : %s\r\n", "wires_photo_lib");
	//	swprintf( szTemp, L"%s :\r\n ", szTemp2);
	//	DisplayMessage( szTemp,0);
	//	swprintf( szTemp, L"  %s: %s\r\n", szTemp3,szBmpFile );
	//	DisplayMessage( szTemp,0);
	//	swprintf( szTemp, L"  %s  : %s\r\n",szTemp4,L"wires_photo_src");
	//	DisplayMessage( szTemp,0);
	}
	else if( uI == ACCESSDBERROR)
	{
	//	m_fileLog.Write ( szTemp , lstrlen ( szTemp ));
		m_strDisplayMessageHeader.LoadStringW(IDS_TRANSFERFILEDBFAIL);
		m_strDisplayMessage.Format(L"%s\r\n",m_strDisplayMessageHeader);
		DisplayErrorMessage ( m_strDisplayMessage);
		m_strDisplayMessage.Format(L" DataBase Failed\r\n" );
		return ACCESSDBERROR;
	}
	else// if(uI == ) 
	{
		m_strDisplayMessageHeader.LoadStringW(IDS_TRANSFERFILEDBFAIL);
		m_strDisplayMessage.Format(L"%s.\r\n",m_strDisplayMessageHeader);
		DisplayErrorMessage ( m_strDisplayMessage,0);
		return FAILED_FALSE;
	}
	DisplayTraceMessage(L"End Insert DB ",0,FALSE);
	return SUCCESS_TRUE;
}


void CDBGraphicMain::CreateLog(){
	
	wchar_t    szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
	wchar_t    szFName[_MAX_FNAME], szExt[_MAX_EXT];
	wchar_t    szfileName[PATH_LENGTH];
	_wsplitpath_s( sz_DatFile, szDrive, szDir, szFName, szExt );
	lstrcpy( szExt, L"log" );
	memset(szFName, 0, _MAX_FNAME);
	lstrcpy(szFName,m_code);	
	_wmakepath_s( szfileName, szDrive, szDir, szFName, szExt );
	m_strFailedCopyLog = szfileName;
}

int   CDBGraphicMain::ComputerFileNum(CString   strFolderName) 
{
	int   nNum=0; 
	CFileFind  file; 
	CString strtmp=strFolderName,filename;
	BOOL   bFind=file.FindFile(strtmp+ "\\*.* "); 
	wchar_t szTmp[200]={0};
	while(bFind) 
	{ 
		bFind   =   file.FindNextFile(); 
		filename=file.GetFileName();
		if(file.IsDots()){ 
		//	swprintf(szTmp,MAX_COUNT,L"Skip this file/folder %s\r\n",filename);
			//	DisplayMessage(szTmp,0);
			continue; }
		if(file.IsDirectory()) {
			//swprintf(szTmp,MAX_COUNT,L"Skip this file/folder %s\r\n",filename);
			//	DisplayMessage(szTmp,0);
			continue;}
		nNum++; 
	} 
	file.Close();
	return nNum;
}

int   CDBGraphicMain::ComputerSkpFile(CString strFolderName) 
{
	int   nNum=0; 
	CFileFind  file; 
	CString strtmp=strFolderName;
	BOOL   bFind=file.FindFile(strtmp); 
	while(bFind) 
	{ 
		bFind   =   file.FindNextFile(); 
		if(file.IsDots())  
			continue; 
		if(file.IsDirectory()) 
			continue;
		//	if(file.)
		nNum++; 
	} 
	file.Close();
	return nNum;
}

bool CDBGraphicMain::ChooseByUser(CString code){
	CString strtmp;
	CListBox *pBox =(CListBox *)GetDlgItem(IDC_DBINFO);
	for(int i=0;i<pBox->GetCount();i++){
		pBox->GetText(i,strtmp);
		CString str;
		str=strtmp.Left(strtmp.Find('['));
		if(!lstrcmp(str,code)){
			return TRUE;}
	}
	return FALSE;
}

void CDBGraphicMain::CreateErrFile(wchar_t * filePath){

	ERRORFILE errorFile;
	CString curTime,curDate,strtmp;
	CFileFind fil;
	CTime time;
	if(fil.FindFile(filePath))
		fil.FindNextFile();
	else return;
	fil.GetLastWriteTime(time);
	strtmp.Format(L"%d-%d-%d %d:%d:%d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
	memset(&errorFile,0,sizeof(ERRORFILE));
	errorFile.err_Count=0;
	errorFile.status=0;
	GetTime(curTime);
	GetCurDate(curDate);
	lstrcpy(errorFile.sz_Code,m_code);
	lstrcpy(errorFile.sz_LastTime,strtmp);
	lstrcpy(errorFile.sz_Date,curDate);
	lstrcpy(errorFile.sz_Time,curTime);
	lstrcpy(errorFile.szFile,m_szFullName);
	errorFile.err_Count=1;
	fileList.Add(errorFile);
	uploadDummy(m_errorFile);
	WriteErrFile(m_errorFile);
	m_errorFile++;
	m_errCount++;
	fil.Close();
}

// Edit Stephen 2021-08-25
BOOL CDBGraphicMain::NotifyDummyThumbnail(){

	// Edit Stephen 2021-12-23
	if (m_iEmailEnable==0) return FALSE;
	// Edit Stephen 2021-12-23

	CString str, strTemp;
	str = L"Following files are imported using dummy thumbnail\r\n\r\n";
	for(int i =0;i< dummyThumbnailList.GetCount() ;i++){
		strTemp.Format(L"Agency/Department Code: %s\r\n\r\nFile Path: %s\r\n\r\nImport Date: %s\r\n\r\nImport Time: %s\r\n\r\n===============\r\n\r\n", 
			dummyThumbnailList[i].sz_Code, dummyThumbnailList[i].szFile, dummyThumbnailList[i].sz_Date, dummyThumbnailList[i].sz_Time);
		
		str = str + strTemp;	
	}	
	if (dummyThumbnailList.GetCount()>0)
	{
		m_EmailParam->strContent=str;
		m_EmailParam->strEmailHeader=L"DBGraphic Dummy Thumbnail Notice";
	//	SendEmail();

		m_EmailParam->strToAdd=m_ToAdd;
		m_EmailParam->strSenderName=L"EMS DBGraphic";
	
		CString strMsg =SendEMSEmail(m_EmailParam);
		if (strMsg.Find(L"Succeed")>=0)
		{
			dummyThumbnailList.RemoveAll();
		}
		DisplayMessage(strMsg,1);
	}
	return FALSE;
}
// Edit Stephen 2021-08-25

BOOL CDBGraphicMain::CheckErrFile(){
	CString curDate,curTime,strtmp;
	CString path1,str;
	CFileFind fil;
	CTime time;
	int copy=0;
	m_szFullName.MakeLower();
	if(fil.FindFile(m_szFullName))
		fil.FindNextFile();
	else return FALSE;
	fil.GetLastWriteTime(time);
	strtmp.Format(L"%d-%d-%d %d:%d:%d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
	for(int i =0;i<m_errorFile;i++){
		path1=(CString)fileList[i].szFile;
		path1.MakeLower();ERRORFILE error =  fileList[i];
		if(!lstrcmp(fileList[i].sz_Code,m_code)&&!lstrcmp(path1,m_szFullName)&&!lstrcmp(strtmp,fileList[i].sz_LastTime)&&!fileList[i].status)
		{
			if(fileList[i].err_Count<m_Eml){
				fileList[i].err_Count++;
				m_errCount++;
				GetCurDate(curDate);
				GetTime(curTime);
				memcpy(fileList[i].sz_Date,curDate,10);
				memcpy(fileList[i].sz_Time,curTime,8);
				copy=1;
			}
			if(fileList[i].err_Count==m_Eml){
				if(fileList[i].status==0){
					if(!copy){
						m_errCount++;}
					//*******************************************************************************
					// Edit Stephen 2021-12-22
					//*******************************************************************************
					if (m_iEmailEnable==1)
					{
						CString strJpgFile = L"";
						en_ACCESSDBRET ret = p_dbCmd->IsDummyPhoto(fileList[i].szFile, strJpgFile);
						if (ret!= SUCCESS_TRUE)
						{
						//*******************************************************************************
							GetTime(curTime);
							//fileList[i].status=1;
							//WriteErrFile(i);
							str.Format(L"Sorry, we were unable to process the following photo for %d times \r\nCode: %s\r\nFile path: %s\r\nLocal Time: %s",m_Eml,fileList[i].sz_Code,fileList[i].szFile,curTime);
							m_EmailParam->strContent=str;
							m_EmailParam->strEmailHeader=L"Error Picture Notice";
							SendEmail();
						//*******************************************************************************
						}
					}
					//*******************************************************************************
					//uploadDummy(i);
					fileList[i].err_Count++;
				}
				else if(fileList[i].status){
					if(!copy){
						m_errCount++;}
					return FALSE;}

			}
			return FALSE;
		}
	}
	wchar_t filepath[260]={0};
	lstrcpy(filepath,m_szFullName);
	fil.Close();
	CreateErrFile(filepath);
	return TRUE;
}

void CDBGraphicMain::WriteErrFile(int num){
	CFile fil;	
	m_errPath.TrimRight();
	CFileException fileException;
	//if(fil.Open(m_errPath, CFile::modeWrite | CFile::shareDenyWrite,&fileException)){
	if(fil.Open( m_errPath,CFile::modeWrite|CFile::shareDenyNone|CFile::typeBinary) == TRUE){
		//	WORD wSignature = 0xFEFF;// use unicode encoding
		fil.SeekToEnd();
		fil.Write(&fileList[num], sizeof(ERRORFILE));
		fil.Write(L"\r\n", 4);
	}
}

void CDBGraphicMain::loadErrInfo(){
	CFile fil;
	long pos;
	int i=0;
	CString path;

	wchar_t    szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
	wchar_t    szFName[_MAX_FNAME], szExt[_MAX_EXT];
	wchar_t    szfileName[PATH_LENGTH];
	_wsplitpath_s( sz_DatFile, szDrive, szDir, szFName, szExt );
	lstrcpy( szExt, L"log" );
	memset(szFName, 0, _MAX_FNAME);
	CString str_temp = L"_error";
	const wchar_t* wc_tmp = str_temp;
	lstrcpy(szFName,wc_tmp);	
	_wmakepath_s( szfileName, szDrive, szDir, szFName, szExt );

	m_errPath = szfileName;
	m_errPath.TrimRight();
	int Opened=0;
	if(fil.m_hFile == CFile::hFileNull)
	{
		if(!fil.Open(m_errPath, CFile::modeReadWrite | CFile::shareDenyWrite))
		{
			if(fil.Open(m_errPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
				Opened = 1;
		}
		if(fil.m_hFile != CFile::hFileNull)
		{
			CFileStatus status;
			fil.GetStatus(status);
			COleDateTime t_TodayTime, t_fileTime(status.m_mtime.GetTime());
			t_TodayTime.SetDateTime(COleDateTime::GetCurrentTime().GetYear(), COleDateTime::GetCurrentTime().GetMonth(), COleDateTime::GetCurrentTime().GetDay(), 0, 0, 0);
			if(t_fileTime<t_TodayTime || bIsFirstTime)
			{
				fil.Close();
				CFile::Remove(m_errPath);
				bIsFirstTime = FALSE;
				if(!fil.Open(m_errPath, CFile::modeReadWrite | CFile::shareDenyWrite))
				{
					loadErrInfo();
					return;
				}
			}
			fil.Seek(0, CFile::end);
		}
		if(Opened){
			fil.SeekToBegin();
			WORD wSignature = 0xFEFF;// use unicode encoding
			fil.Write(&wSignature, 2);//Unicode的文件符?
		}

		fil.Close();
	}
	ERRORFILE fileToRead;
	memset( &fileToRead, '\0', sizeof(ERRORFILE) );
	if(WAIT_OBJECT_0 == WaitForSingleObject(g_hViewMutex, INFINITE))
	{
		if(m_Lock) 
		{
			ReleaseMutex(g_hViewMutex);
			return;
		}
		ReleaseMutex(g_hViewMutex);
	}
	if(!Opened){
		if( fil.Open( m_errPath,CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary) == TRUE ) {
			pos = (long)fil.Seek(2, CFile::begin);
			while(TRUE){		
				/*fil.GetLength();10,682 */
				if ((pos=fil.Read(&fileToRead,sizeof(ERRORFILE)))==sizeof(ERRORFILE))
				{
					fileList.Add(fileToRead);
					m_errorFile++;

				}
				else
					break;
			}
			fil.Close();
		}}

	for(int i=0;i<m_errorFile;i++)
	{
		ERRORFILE errorI = fileList[i];
		for(int j=0;j<m_errorFile;j++)
		{
			ERRORFILE errorJ = fileList[j];
			if(i==j)
				break;
			if((!lstrcmp(fileList[i].szFile, fileList[j].szFile)&&
				!lstrcmp(fileList[i].sz_Code, fileList[j].sz_Code)&&
				!lstrcmp(fileList[i].sz_Date, fileList[j].sz_Date)&&
				!lstrcmp(fileList[i].sz_LastTime, fileList[j].sz_LastTime)&&
				!lstrcmp(fileList[i].sz_Time, fileList[j].sz_Time))||(lstrlen(fileList[i].sz_Code)==0||lstrlen(fileList[i].szFile)==0))
			{
				fileList.RemoveAt(i);
				i=j=0;
				m_errorFile--;
				break;
			}
		}
	}
}

BOOL CDBGraphicMain::checkErrTable(CString photoPath){
	CString curDate,path1,path2,strtmp;
	//wchar_t strTmp[1024];
	CFileFind fil;
	CTime time;
	path1=(CString)photoPath;
	path1.MakeLower();
	if(fil.FindFile(path1))
		fil.FindNextFile();
	else return TRUE;
	fil.GetLastWriteTime(time);
	strtmp.Format(L"%d-%d-%d %d:%d:%d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
	for (int i=0;i<m_errorFile;i++)
	{
		path2=(CString)fileList[i].szFile;
		path2.MakeLower();
		if((!lstrcmp(fileList[i].sz_Code,m_code)) && (!lstrcmp(path2,path1))&& (!lstrcmp(strtmp,fileList[i].sz_LastTime))){
			if(fileList[i].err_Count==m_Eml){
				m_errCount++;
				return FALSE;
			}
			else{

				GetCurDate(curDate);
				if(lstrcmp(curDate,fileList[i].sz_Date)){
					continue;}
				else if(timecmp(i))					
					return TRUE;
				else {
					m_errCount++;
					return FALSE;
				}
			}
		}
	}
	fil.Close();
	return TRUE;
}

BOOL CDBGraphicMain::timecmp(int num){
	CString strtmp;
	int curMin,curScd,curHr,fileMin,fileScd,fileHr;
	COleDateTime odtCur = COleDateTime::GetCurrentTime();
	curMin=odtCur.GetMinute();
	curScd=odtCur.GetSecond();
	curHr=odtCur.GetHour();
	swscanf_s(fileList[num].sz_Time,L"%02d:%02d:%02d",&fileHr,&fileMin,&fileScd);
	if(fileHr==curHr)
		if((curMin-fileMin)>m_TryTime)
			return TRUE;
		else if((curMin-fileMin)==m_TryTime)
			if(curScd>fileScd)
				return TRUE;
			else return FALSE;
		else return FALSE;
	else if((curHr-fileHr)>1)
		return TRUE;
	else if((curMin+60-fileMin)>m_TryTime)
		return TRUE;
	else if((curMin+60-fileMin)==m_TryTime)
		if(curScd>fileScd)
			return TRUE;
		else return FALSE;
	else return FALSE;
	return FALSE;
}

void CDBGraphicMain::SendEmail(){

	// Edit Stephen 2021-12-23
	if (m_iEmailEnable==0) return;
	// Edit Stephen 2021-12-23

	bool bError = false;
	m_EmailParam->strToAdd=m_ToAdd;
	m_EmailParam->strSenderName=L"EMS DBGraphic";
	m_strDisplayMessage=SendEMSEmail(m_EmailParam);
	DisplayMessage(m_strDisplayMessage,0);
}

void CDBGraphicMain::AddItem(LPCWSTR time,  LPCWSTR msg)
{
	if (m_msg.GetItemCount () > 200)
		m_msg.DeleteItem(0);
	LV_ITEM	lvitem;
	int nRow ;
	nRow=0;
	nRow = m_msg.GetItemCount ();

	lvitem.mask = LVIF_TEXT;
	lvitem.iItem =nRow ;
	lvitem.iSubItem = 0;
	//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
	//lvitem.pszText = (unsigned short*)time;
	lvitem.pszText =(LPWSTR)(LPCTSTR)time;
	nRow = m_msg.InsertItem(&lvitem);

	lvitem.iSubItem = 1;
	//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
	//lvitem.pszText = (unsigned short*)msg;
	lvitem.pszText = (LPWSTR)(LPCTSTR)msg;
	m_msg.SetItem(&lvitem);

	m_msg.EnsureVisible(nRow, FALSE);
	m_msg.SendMessage(WM_PAINT);	
}

//check if existing such photo in the target path
void CDBGraphicMain::checkPhotoName(CString &fileName,CString Ext,CString &szFile,int type){
	CFileFind fil;
	CString szName;
	CString fileRealName;
	int num=0;
	//int pos = fileName.GetLength();
	//	int pos1 = fileName.ReverseFind('.');
	szName=fileName.Left(fileName.ReverseFind('.'));
	while (fil.FindFile(fileName))
	{	
		fil.FindNextFile();
		if(!num)
			fileRealName=fil.GetFileName();	
		else{
			if(type==OrgPhoto){
				fileName.Format(_T("%s_%d.%s"),szName,num,Ext);
				szFile.Format(_T("%s_%d"),fileRealName.Left(fileRealName.ReverseFind('.')),num);			
			}
			else if(type==SmlPhoto)
				fileName.Format(_T("%s_%s.%s"),fileName.Left(fileName.ReverseFind('.')),L"s",Ext);
			else if(type==MidPhoto)
				fileName.Format(_T("%s_%s.%s"),fileName.Left(fileName.ReverseFind('.')),L"m",Ext);
		}	
		num++;
	}
	fil.Close();
}

void getYesterday(CString &date){
	CTime   today;
	today   =   CTime::GetCurrentTime();
	time_t   tmp   =   today.GetTime()   -   24*3600;
	COleDateTime odtCur(tmp);
	date.Format(_T("%04d-%02d-%02d 23:59:59 "), odtCur.GetYear(), odtCur.GetMonth()
		, odtCur.GetDay());
} 

BOOL CDBGraphicMain::TimeToRead(){
	int Hour,Min,Scd,curHour,curMin,curScd;
	COleDateTime odtCur = COleDateTime::GetCurrentTime();		
	curHour=odtCur.GetHour();
	curMin=odtCur.GetMinute(); 	
	curScd=odtCur.GetSecond();
	swscanf_s(db_config->szStartTime,L"%02d:%02d:%02d",&Hour,&Min,&Scd);
	if(curHour>Hour)
		return FALSE;
	else if(curHour==Hour) 
	{
		if(curMin>Min)
			return FALSE;
		else if(curMin==Min){
			if (curScd>=Scd)
				return FALSE;
			else return TRUE;
		}
		else return TRUE;
	}
	else return TRUE;
}

void CDBGraphicMain::copyPhoto(LPCWSTR lpExistingFileName,LPCTSTR lpNewFileName){
	CFile file1,file2;
	int count=0,num=0;
	char readByte[1];
	unsigned  char szByte; 
	WORD word,wSignature = 0xFEFF;
	if(file1.Open(lpExistingFileName, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone ) == TRUE ){
		if(file2.Open(lpNewFileName, CFile::modeCreate | CFile::modeWrite |CFile::shareDenyNone|CFile::typeBinary)){
			file2.SeekToBegin();
			file1.SeekToBegin();
			file1.Read(&word,2);
			DWORD dwLength = (DWORD)file1.GetLength();
			if(dwLength == 0)
				return ;		
			char* lpszBuffer;
			lpszBuffer = new char[dwLength]; 
			wchar_t *szBuffer1;
			if(dwLength >= 2)
			{
				szBuffer1= new wchar_t[dwLength-2];
				file2.Write(&wSignature, 2);
			}
			else
				szBuffer1= new wchar_t[dwLength];
			if(word==0xFEFF ){
				if(file1.Read(szBuffer1,dwLength-2)>0){			
					file2.Write(szBuffer1,dwLength-2);
				}
			}
			else {
				file1.SeekToBegin();
				while(file1.Read(readByte,1)){
					szByte=readByte[0];
					count++;
					if(szByte>0x7f)
						num++;
				}
				file1.SeekToBegin();
				file1.Read(lpszBuffer,dwLength);
				wchar_t* szContent = new wchar_t[dwLength];
				MultiByteToWideChar(CP_ACP, 0, lpszBuffer, -1, szContent, dwLength);
				file2.Write(szContent,dwLength*2-num);

				delete lpszBuffer;
				delete szBuffer1;
				delete szContent;
			}
		}
	}

}

void CDBGraphicMain::uploadDummy(int num){
	CFileFind file;
	CString TargetPath,MidPath,smallPath,strReturn,PhotoTime,szDummyName,szPhotoName,SrcPath,fileName,szLastTime;	
	wchar_t ImageID[20],szJpgFile[100],szBmpFile[256],szFile[256],szTargetPath[256],szPhotoTime[40],szSrcPath[256],szLastModTime[40];
	CTime time,time1;
	if(_waccess(m_DummyPath,0)==-1){
		m_strDisplayMessage.Format(L"Cannot access dummy photo.\r\n");
		DisplayMessage(m_strDisplayMessage,display);
		return;
	}
	PhotoInfo pInfo;
	memset(&pInfo,0,sizeof(pInfo));
	szDummyName=(CString)m_DummyPath;
	MidPath=(CString)db_config->szMidPath;
	//	BOOL bMakeDir;
	TargetPath=CString(db_config->szDesPath);
	SrcPath=(CString)(fileList[num].szFile);
	szPhotoName=SrcPath.Right(SrcPath.GetLength()-SrcPath.ReverseFind('\\')-1);
	if(!TargetPath.IsEmpty()){
		CopyFile((fileList[num].szFile),TargetPath,FALSE);
		TargetPath=TargetPath+"\\"+szPhotoName;
	}
	else TargetPath=SrcPath;
	TargetPath = GetPath(TargetPath,m_checkYsd);
	BOOL   bFind=file.FindFile(fileList[num].szFile);
	if(bFind)
		file.FindNextFile();
	else return;
	file.GetLastWriteTime(time);
	szLastTime.Format(L"%d-%d-%d %d:%d:%d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());	
	// 	if(!MidPath.IsEmpty()){
	// 	strReturn=GetPath(db_config->szMidPath,m_checkYsd);
	// 	strReturn.MakeLower();
	// 	bMakeDir = MakeDir(strReturn);
	// 	if(bMakeDir)
	// 		CopyFile(m_DummyPath,strReturn,FALSE);
	// 	MidPath=strReturn+"\\"+szPhotoName;
	// 	}
	// 	strReturn=GetPath(db_config->szSmallPath,m_checkYsd);
	// 	strReturn.MakeLower();
	// 	bMakeDir = MakeDir(strReturn);
	// 	CopyFile(m_DummyPath,strReturn,FALSE);
	// 	smallPath=strReturn+"\\"+szPhotoName;
	//time1=CTime::GetCurrentTime()+CTimeSpan(1,0,0,0);
	time1=CTime::GetCurrentTime();
	PhotoTime.Format(L"%d-%d-%d %d:%d:%d",time1.GetYear(),time1.GetMonth(),time1.GetDay(),time1.GetHour(),time1.GetMinute(),time1.GetSecond());	
	if(p_dbCmd->GetNewImageName(ImageID) == ACCESSDBERROR)
		return;		
	szPhotoName=SrcPath.Right(SrcPath.GetLength()-SrcPath.ReverseFind('\\')-1);
	szPhotoName=szPhotoName.Left(szPhotoName.ReverseFind(_T('.')));
	szPhotoName + _T(".txt");
	fileName=m_szTxtSrcPath+szPhotoName;
	lstrcpy(szJpgFile , ImageID);
	lstrcpy(szTargetPath,TargetPath);
	lstrcpy(szPhotoTime,PhotoTime);
	lstrcpy(szFile,fileName);
	lstrcpy(szSrcPath,SrcPath);
	lstrcpy(szLastModTime,szLastTime);
	CString strTemp=db_config->szBeginTime;
	strTemp.TrimRight();
	strTemp = strTemp.Left(strTemp.Find(':'));
	const wchar_t* wc_tmp = strTemp;
	int nDelayHour=_wtoi(wc_tmp);
	strTemp = strTemp.Right(strTemp.GetLength()-1-strTemp.ReverseFind(':'));
	wc_tmp = strTemp;
	int nMin=_wtoi(wc_tmp);
	// Edit Stephen 2021-08-25
	bool bInserted = false;
	p_dbCmd->InsertValuesDummy (bInserted, &pInfo,
	// Edit Stephen 2021-08-25
		db_config->szTeleCode,
		db_config->nAgencyID,
		db_config->iType, 
		szBmpFile , 
		szFile, 
		L"DummyPath",//m_DummyPath, 
		L"DummyPath",//m_DummyPath,
		nDelayHour,
		nMin,
		szJpgFile,
		db_config->nMarkLine,
		db_config->nTitleLine,
		db_config->bTransfer,
		0,
		szLastModTime,
		szSrcPath,
		m_strDbgStartTime.AllocSysString(),
		0,
		db_config->szBeginTime,
		szTargetPath);
	m_strDisplayMessage.Format(L"Upload Dummy [file: %s]\r\n",SrcPath.Right(SrcPath.GetLength()-SrcPath.ReverseFind('\\')-1));
	file.Close();
	DisplayMessage(m_strDisplayMessage,display);

	//**********************************************************************
	// Edit Stephen 2021-08-25
	//**********************************************************************
	if (bInserted)
	{
		DUMMYTHUMBNAILFILE dummyFile;
		CString curTime,curDate,strtmp;
		memset(&dummyFile,0,sizeof(DUMMYTHUMBNAILFILE));
		GetTime(curTime);
		GetCurDate(curDate);
		lstrcpy(dummyFile.sz_Code,m_code);
		lstrcpy(dummyFile.sz_Date,curDate);
		lstrcpy(dummyFile.sz_Time,curTime);
		lstrcpy(dummyFile.szFile,m_szFullName);
		dummyThumbnailList.Add(dummyFile);
	}
	//**********************************************************************
	// Edit Stephen 2021-08-25
	//**********************************************************************

}

int CDBGraphicMain::accessPath(CString szSmlPath,CString szMidPath){
	if(!MakeDir(szSmlPath))
		return 1;
	//	else if	(WNetGetConnection(t,temp,&len)==NO_ERROR)
	//	{
	//	}
	if(szMidPath.IsEmpty())
		return 0;
	else if(!MakeDir(szMidPath))
		return 2;
	else return 0 ;
}


BOOL CDBGraphicMain::Load_Data_File()
{

	memitem_temp.SetUnitSize (sizeof(LOGDISTRIBUTEDITEM)) ;   
	memitem_temp.MemoryAlloc();


	// Modify Stephen 2011-04-15
	// put the write DGP process right after check db process
	wchar_t szTemp[ TEMP_STRING_LENGTH ];
	LOGDISTRIBUTEDHEAD head;
	wchar_t szBmpFile_tmp[PATH_LENGTH1 ]=L"";
	CString szDesFile_tmp;
	wchar_t szNote[NAME_LEN] = L"";
	wchar_t szTempStr[PATH_LENGTH1]=L"";
	wchar_t szPath[PATH_LENGTH1];
	CString szCurTime;
	int ntype=0;
	//edit by leon 2012-11-23 for define temporary path in INI instead of hardcode.
	//lstrcpy(szTemp, AfxGetApp()->GetProfileString(L"System",
	//	L"TemporaryPath", L"C:\\FOUNDER\\TEMP"));
	lstrcpy(szTemp,m_szSystemTempPath);
	//end leon 2012-11-23
	wsprintf(szPath, L"%s\\%s.DGP", (wchar_t *)(LPCWSTR)szTemp, (wchar_t *)(LPCWSTR)db_config->szCode);

	CFile   fileDat;
	if (fileDat.Open(szPath, CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone)== FALSE)
	{
		AfxMessageBox (L" Cannot Open DGP Data File - please check ");
		return FALSE;
	}

	CTime RealTime=GetConvertTime();
	memitem_temp.ResetContent();  
	if(m_strPhotoPath.Find(L"%Y%M%D")>0){
		ntype=1;
		szCurTime.Format(L"%04d-%02d-%02d", RealTime.GetYear(),RealTime.GetMonth(),RealTime.GetDay());
	}
	else if(m_strPhotoPath.Find(L"%Y%M")>0){
		ntype=2;
		szCurTime.Format(L"%04d-%02d", RealTime.GetYear(),RealTime.GetMonth());
	}
	else if(m_strPhotoPath.Find(L"%Y")>0){
		ntype=3;
		szCurTime.Format( L"%04d ", RealTime.GetYear());
	}
	else {
		ntype=4;
		szCurTime=L"00:00:00";
	}
	if (!p_dbCmd->LoadDateProcessImage(&memitem_temp, db_config->szTeleCode, RealTime.GetYear(), RealTime.GetMonth(), RealTime.GetDay(),ntype))
	{
		fileDat.Close(); 	
		AfxMessageBox (L"Init DGP Data File Fail - please check ");
		return FALSE;
	}

	fileDat.SeekToBegin();
	WORD wSignature = 0xFEFF;// use unicode encoding
	fileDat.Write(&wSignature, 2);//Unicode的文件符?

	lstrcpy(head.szCode, db_config->szCode);
	lstrcpy(head.szDate, szCurTime);
	head.lTotal = head.lStart = 0;
	fileDat.Write(&head, sizeof(LOGDISTRIBUTEDHEAD));


	m_nCount =0;

	CString strServerPath;
	strServerPath=GetServerPath(0);

	CString szDesFile;
	if ( memitem_temp.GetUnitCount() != 0 )
	{
		while(m_nCount< memitem_temp.GetUnitCount())
		{
			LOGDISTRIBUTEDITEM item;
			//			LOGDISTRIBUTEDITEM item_new;
			memcpy ( &item , memitem_temp.GetData(m_nCount) , sizeof (LOGDISTRIBUTEDITEM )); 

			//				strcpy( item.szFile, item_new.szFile );

			fileDat.Seek(2, CFile::begin);
			fileDat.Read(&head, sizeof(LOGDISTRIBUTEDHEAD));

			szDesFile.Format(L"%s\\%s",strServerPath,item.szFile);
			head.lTotal ++;
			//m_ulTransFileNum = head.lTotal;
			fileDat.Seek(2, CFile::begin);
			fileDat.Write(&head, sizeof(head) );
			//			strcpy( item.szFile, szFile );

			/////////////////////////////////
			//			CString str=szFile;
			CString str= item.szFile ;
			//modified by wb-zhang 2003-12-1
			//CString str1=str.Left(str.Find('.'));
			int nFind=str.ReverseFind('.');
			CString  str1;
			if(nFind!=-1)
				str1=str.Left(nFind);
			else
				str1=str;
			//modified end
			szDesFile_tmp=L"";
			szDesFile_tmp=m_szSystemTempPath+str1+L".bmp";
			unsigned long Tick_Temp=GetTickCount();

			//			static SYSTEMTIME RealTime;
			//			GetSystemTime( &RealTime );
			//			sprintf ( szBmpFile_tmp , "%s\\%s%02u%02u%02u%02d%02d%02d%03d.jpg" ,szPhotoDesPath, db_config->szCode, 
			//				RealTime.wMonth, RealTime.wDay, RealTime.wYear%100,  RealTime_true.GetHour(), RealTime.wMinute, RealTime.wSecond, 
			//				RealTime.wMilliseconds );//, str1); 

			// 			swprintf ( szBmpFile_tmp , L"%s\\%s%02u%02u%02u%02d%02d%02d%03d.jpg" ,szPhotoDesPath, db_config->szCode, 
			// 				RealTime_true.GetMonth(), RealTime_true.GetDay(), RealTime_true.GetYear()%100,  RealTime_true.GetHour(), RealTime_true.GetMinute(), 
			// 				RealTime_true.GetSecond(), RealTime.wMilliseconds );//, str1); 
			// 
			// 			if(db_config->bAutoBitmap)
			// 				lstrcpy( item.szScrFile, szBmpFile_tmp );
			// 			else 
			// 				lstrcpy( item.szScrFile, szDesFile_tmp); 

	//		swprintf(szTempStr,MAX_COUNT,L"To :%s", szDesFile );
		//	swprintf(szNote, MAX_COUNT,L"%05ld %-8s %s", head.lTotal, item.szFile,szTempStr);

			//	lstrcpy( item.szNote, szNote );
			item.lNumber = head.lTotal;
			fileDat.Seek( 
				2+sizeof(head) + sizeof(item) * (head.lTotal - 1 ),
				CFile::begin );
			fileDat.Write( &item, sizeof(item) );

			m_nCount++;
		}
	}

	fileDat.Close(); 	
	memitem_temp.ResetContent();

	return TRUE;

}

BOOL CDBGraphicMain::OnInitDialog()
{
	CDialog::OnInitDialog();
	ModifyStyleEx(0,WS_EX_APPWINDOW);
	ShowWindow(SW_SHOW);

	// TODO:  Add extra initialization here
	m_msg.SetExtendedStyle(m_msg.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_msg.InsertColumn(0, _T(""), LVCFMT_LEFT, 130);
	m_msg.InsertColumn(1,_T(""),LVCFMT_LEFT,800);
	bCreate = m_ImageDlg.Create();
	if(m_ImageDlg.GetSafeHwnd() == NULL)
	{
		return -1;
	}
	ImageDlg = &m_ImageDlg;	
	m_ImageDlg.m_pView = this;
	wchar_t szWidth[10],szHeight[10];
	p_dbCmd->GetConfigure(L"photo_width",szWidth);
	p_dbCmd->GetConfigure(L"photo_height",szHeight);
	m_nHeight=_wtoi(szHeight);
	m_nWidth=_wtoi(szWidth);
	if(m_nHeight<=0)
		m_nHeight=100;
	if(m_nWidth<=0)
		m_nWidth=100;
	p_dbCmd->GetConfigure(L"photo_widthM",szWidth);
	p_dbCmd->GetConfigure(L"photo_heightM",szHeight);
	m_nMidHeight=_wtoi(szHeight);
	m_nMidWidth=_wtoi(szWidth);
	if(m_nMidHeight<=0)
		m_nMidHeight=300;
	if(m_nMidWidth<=0)
		m_nMidWidth=300;
	p_dbCmd->GetConfigure(L"PhotoOffsetDay",szWidth);
	m_nDayOffset = _wtoi(szWidth);
	// TODO: Add your specialized code here and/or call the base class
	// add by huxf, 2003-12-0
	// From OnCreate()
	//m_uTimer = SetTimer(CHILD_WINDOW_TIMER, 10000, NULL );
	//m_uTimerD = SetTimer(DELETE_TIMER, 30000, NULL);
	/*if( !m_uTimer )
	{
		wchar_t szTemp[50],szTemp2[50];
		LoadString(AfxGetApp()->m_hInstance,AFX_IDS_ERRORTIMER,
			szTemp,50);
		LoadString(AfxGetApp()->m_hInstance,AFX_IDS_MANYTIMER,
			szTemp2,50);
		MessageBox(szTemp, szTemp2, MB_OK | MB_ICONHAND);
		//	return -1;
	}*/
	bIsFirstTime = TRUE;
	loadErrInfo();
	bIsFirstTime = FALSE;
	m_startTime=CTime::GetCurrentTime();
	InitEmailParam();

	m_iTraceLog = GetPrivateProfileInt(L"System",L"CreateTraceLog",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	((CButton*)GetDlgItem(IDC_CREATETRACELOGCHECK))->SetCheck(m_iTraceLog);

	// Edit Stephen 2021-12-23
	m_iEmailEnable = GetPrivateProfileInt(L"SENDEMAIL",L"Enable",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	// Edit Stephen 2021-12-23


	wchar_t str_temp[1024];
	int iSaveAll;
	GetPrivateProfileStringW( L"System",L"SaveAll", L"0", str_temp,100,((CWinApp*)AfxGetApp())->m_pszProfileName);
	iSaveAll = _wtoi(str_temp);
	CListBox* pList = (CListBox*)GetDlgItem(IDC_DBINFO);
	if(iSaveAll)
	{
		CString str_DisplayInfoCode, str_Temp, str_Token ;
		GetPrivateProfileStringW( L"System",L"DisplayInfoCode", L"", str_temp,1024,((CWinApp*)AfxGetApp())->m_pszProfileName);
		str_DisplayInfoCode.Format(L"%s",str_temp);
		int iCurPos = 0, iCount = 0;
		str_Token = str_DisplayInfoCode.Tokenize(L",", iCurPos);
		while(str_Token.GetLength()>0)
		{
			pList->AddString( str_Token);
			iCount++;
			str_Token = str_DisplayInfoCode.Tokenize(L",", iCurPos);
		}
		bIsStartUp = TRUE;
		OnAddagent();
		bIsCancel = pList->GetCount() == 0;
		bIsStartUp = FALSE;
		if(bIsCancel==TRUE)
		{
			pList->ResetContent();
			bIsCancel = FALSE;
		}else
			OnStart();
	}

	CFileFind file;
	CString strLog,strLogNum,curDate;
	CTime time;
	time=CTime::GetCurrentTime();
	curDate.Format(L"%d%02d",time.GetYear(),time.GetMonth());
	/*strLogNum = m_wLogNum;
	
	strLog.Format(L"%s\\Dbgraphic%s_%s_%d.log",m_szSystemLogPath,curDate,strLogNum,m_iLogNum);
	BOOL   bFind=file.FindFile(strLog);
	while(bFind)
	{
		file.FindNextFile();
		if (file.GetLength()>(MAX_LOG_LENGTH*1024))
		{
			m_wLogNum=(wchar_t)m_wLogNum+1;
			m_iLogNum=1;
		}
		else
			m_iLogNum++;
		strLogNum = m_wLogNum;
		strLog.Format(L"%s\\Dbgraphic%s_%s_%d.log",m_szSystemLogPath,curDate,strLogNum,m_iLogNum);
		bFind=file.FindFile(strLog);
	}*/
	curDate.Format(L"%d%02d%02d",time.GetYear(),time.GetMonth(),time.GetDay());
	m_strStarttime = curDate;
	strLog.Format(L"%s\\DBG_Trace%s_%d.log",m_szSystemLogPath,curDate,m_iTraceLogNum);
	BOOL bFind=file.FindFile(strLog);
	while(bFind)
	{
		file.FindNextFile();
		m_iTraceLogNum++;
		strLog.Format(L"%s\\DBG_Trace%s_%d.log",m_szSystemLogPath,curDate,m_iTraceLogNum);
		bFind=file.FindFile(strLog);
	}

	
	p_dbCmd->GetConfigure(L"IsDBGYesterday",szWidth);
	m_bDBGYesterday = szWidth != _T("0") ? TRUE : FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDBGraphicMain::OnAddagent()
{
	// TODO: Add your control notification handler code here
	m_totalPrs=0;
	CDBGraphicSelect dlg(&m_hpdfsDistFile);
	dlg.bIsStartUp = bIsStartUp;
	dlg.DoModal();
	dlg.bIsStartUp = FALSE;
	bIsCancel = dlg.bIsCancel;
}

void CDBGraphicMain::DistributeAV(){

}

INT_PTR CDBGraphicMain::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DoModal();
}

void CDBGraphicMain::LoopAgency(){
	KillTimer(1001);
	CString code,errInfo,curTime,szSmlPath,szMidPath;
	int i=0;
	BOOL bSkip=FALSE;
	BOOL bAllSkip=TRUE;
	BOOL bRun=TRUE;
	SendMessage(MESSAGE_UPDATE_UI_STATUS,0,FALSE);
	if( !p_dbCmd->LoginDb(TRUE)){
		DisplayTraceMessage(L"Cannot access database",0,FALSE);
		if(m_errSrvCount<3){
			DisplayTraceMessage(L"Cannot access database less than three time",0,FALSE);			
			m_strDisplayMessage.Format(L"Failed to access database,please check the network \r\n");
			DisplayMessage(m_strDisplayMessage,1);
			m_strDisplayMessage.Format(L"Stop for 3 minutes \r\n\r\n");
			DisplayMessage(m_strDisplayMessage,1);
			SetTimer(1001,3*60*1000,0);	
			bRun=FALSE;
			m_errSrvCount++;
			SendMessage(MESSAGE_UPDATE_UI_STATUS,0,TRUE);
			return;
		}
		else {
			GetCurTime(curTime);
			DisplayTraceMessage(L"Cannot access database for three time",0,FALSE);
			m_strDisplayMessage.Format(L"Sorry, we cannot access database for three times,please check the network \r\n time: %s",curTime);
			if (m_errSrvCount<5)
			{
				DisplayTraceMessage(L"Send email",0,FALSE);
				m_EmailParam->strContent=m_strDisplayMessage;
				m_EmailParam->strEmailHeader=L"Error Picture Notice";
				SendEmail();
			}
			UpdateMainFrameMessage(curTime,L"Please contact IT department for help");
			UpdateMainFrameMessage(L"",L"");
		}
		bRun=FALSE;
		m_errSrvCount++;
	}
	else{ 
		if (m_hpdfsDistFile.GetCount()==0){
			AfxMessageBox(L"請選擇一個通訊社");
			bRun=FALSE;
			SendMessage(MESSAGE_UPDATE_UI_STATUS,0,TRUE);
			GetDlgItem(IDC_NOTYETIMPORT)->EnableWindow(TRUE);
			GetDlgItem(IDC_CREATETRACELOGCHECK)->EnableWindow(TRUE);
			return;
		}
		m_errSrvCount=0;
	}
	if(IsDlgButtonChecked(IDC_NOTYETIMPORT))
		m_checkYsd=1;
	if (bRun)
	{
		GetDlgItem(IDC_NOTYETIMPORT)->EnableWindow(FALSE);
		CheckDlgButton(IDC_NOTYETIMPORT, BST_UNCHECKED);
		while (m_checkYsd>=0)
		{
			//COleDateTime dtTime = COleDateTime::GetCurrentTime();
			LoadConfig();
			for(int nAgencyNum=0;nAgencyNum<m_hpdfsDistFile.GetCount();nAgencyNum++){
				if(bFlagStop == TRUE)
					break;
				db_config=&(m_hpdfsDistFile.GetAt(nAgencyNum));
				m_strPhotoEx = db_config->szPhotoExtName;
				m_strFilePath = db_config->szTextPath;
				m_strPhotoPath = db_config->szSourcePath;
				m_strTargetPathO = db_config->szDesPath;
				/*m_MaxTm = db_config->nMaxTm;
				if(dtTime < db_config->dtLastImportTime+COleDateTimeSpan(0,0,db_config->nInterval,0) && !m_bStart)
					continue;
				m_hpdfsDistFile.GetAt(nAgencyNum).dtLastImportTime = COleDateTime::GetCurrentTime();*/
				if(m_checkYsd==1&&m_strPhotoPath.Find(L"%Y%M%D")<0)
				{
					continue;
				}
				m_code = db_config->szCode;
				for (i=0;i<m_errAgency.GetSize();i++)
				{
					if (!lstrcmp(m_errAgency[i],db_config->szCode))
					{
						bSkip=TRUE;
						break;
					}
				}
				szSmlPath=GetPath(db_config->szSmallPath,m_checkYsd);
				szMidPath=GetPath(db_config->szMidPath,m_checkYsd);
				int nResult;
				if (db_config->iMediaType==DBG_TYPE_INT_AV)
					nResult = 0;
				else
					nResult = accessPath(szSmlPath,szMidPath);
				if(nResult==1){
					GetCurTime(curTime);		
					m_strDisplayMessage.Format(L"[%s] Path Error: %s \r\n",db_config->szCode,szSmlPath);		
					DisplayMessage(m_strDisplayMessage,1);
					errInfo.Format(L"Sorry, we cannot access this path %s for %s\r\n time: %s",szSmlPath,db_config->szTeleName,curTime);
					if(!bSkip){
						m_EmailParam->strContent=errInfo;
						m_EmailParam->strEmailHeader=L"Access Path Failed Notice";
						SendEmail();
						m_errAgency.Add(db_config->szCode);}
					bAllSkip = FALSE;
					continue;
				}
				else if(nResult==2){
					GetCurTime(curTime);
					m_strDisplayMessage.Format(L"[%s] Path Error: %s \r\n ",db_config->szCode,szMidPath);	
					DisplayMessage(m_strDisplayMessage,1);
					errInfo.Format(L"Sorry, we cannot access this path %s for %s\r\n time: %s",szMidPath,db_config->szTeleName,curTime);
					if(!bSkip){
						m_EmailParam->strContent=errInfo;
						m_EmailParam->strEmailHeader=L"Access Path Failed Notice";
						SendEmail();
						m_errAgency.Add(db_config->szCode);}
					bAllSkip = FALSE;
					continue;
				}
				else if(nResult==0){
					for (i=0;i<m_errAgency.GetSize();i++)
					{
						if (!lstrcmp(m_errAgency[i],db_config->szCode))
						{
							m_errAgency.RemoveAt(i);
							break;
						}
					}
					bAllSkip = FALSE;
				}
				if (m_checkYsd==1)				
					m_MaxTm=100000;

				DisplayTraceMessage(L"***********************************************************************************",0,FALSE);
				CString strFreeMemory;
				MEMORYSTATUSEX MemStat;
				MemStat.dwLength = sizeof(MEMORYSTATUSEX);
				GlobalMemoryStatusEx(&MemStat);
				strFreeMemory.Format(L"%s: 系統可用記憶體: %lu KB", db_config->szTeleName, MemStat.ullAvailPhys / 1024L);
				DisplayTraceMessage(strFreeMemory,0,FALSE);
				DisplayTraceMessage(L"***********************************************************************************",0,FALSE);

				if (db_config->iMediaType==DBG_TYPE_INT_AV)
					DistributeAV(nAgencyNum);
				else
					DistributePhoto(nAgencyNum);
				num_copy=0;
				m_disFile=0;
				m_exist=0;
				if (TimeToRead() && m_strPhotoPath.Find(L"%Y%M%D")>0 && m_checkYsd==0)
				{		
					m_checkYsd=1;
					m_MaxTm=100000;
					if (db_config->iMediaType==DBG_TYPE_INT_AV)
						DistributeAV(nAgencyNum);
					else
						DistributePhoto(nAgencyNum);
					m_checkYsd=0;
				}
				m_MaxTm=m_MaxCopy;
				//m_checkYsd=0;
				num_copy=0;
				m_disFile=0;
				m_exist=0;
				
				DisplayTraceMessage(L"***********************************************************************************",0,FALSE);
				MemStat.dwLength = sizeof(MEMORYSTATUSEX);
				GlobalMemoryStatusEx(&MemStat);
				strFreeMemory.Format(L"%s: 系統可用記憶體: %lu KB", db_config->szTeleCode, MemStat.ullAvailPhys / 1024L);
				DisplayTraceMessage(strFreeMemory,0,FALSE);
				DisplayTraceMessage(L"***********************************************************************************",0,FALSE);
			}
			if (!bAllSkip)
			{
				m_total+=m_totalPrs;
				m_strDisplayMessage.Format(L"Finished. [This round Processed %d] [Total Processed %d]. \r\n",m_totalPrs,m_total);
				m_totalPrs=0;
				DisplayMessage(m_strDisplayMessage,1);
				if (m_checkYsd == 0)
				{
					//m_strDisplayMessage.Format(L"Stop for %d minutes. \r\n\r\n\r\n",m_interval/60);	
					m_strDisplayMessage.Format(L"Stop for %d seconds. \r\n\r\n\r\n",m_interval);
					DisplayMessage(m_strDisplayMessage,1);
				}
				UpdateMainFrameMessage(L"",L"");
			}
			m_checkYsd--;
		}
		m_checkYsd=0;
	}
	
	if(bFlagStop == TRUE)
		KillTimer(1001);
	else
		SetTimer(1001,m_interval*1000,0);
	SendMessage(MESSAGE_UPDATE_UI_STATUS,0,TRUE);
}

UINT   CDBGraphicMain::ThreadFunc(LPVOID lpParam){
	if(WAIT_OBJECT_0 == WaitForSingleObject(g_hViewMutex, INFINITE))
	{
		if(m_Lock) 
		{
			ReleaseMutex(g_hViewMutex);
			return 1;
		}
		ReleaseMutex(g_hViewMutex);
	}
	threadInfo* info=(threadInfo *)lpParam;
	//info->pctrlProgress->PostMessage(MESSAGE_UPDATE_UI_STATUS,0,FALSE);
	// Edit Stephen 2021-12-21
	info->pctrlProgress->NotifyDummyThumbnail();
	// Edit Stephen 2021-12-21
	info->pctrlProgress->LoopAgency();
	//info->pctrlProgress->PostMessage(MESSAGE_UPDATE_UI_STATUS,0,TRUE);
	return 1;
}

//Edit by Leon Li 2013-02-27 for enhancement: remove dat file and check photo record in memeory
BOOL CDBGraphicMain::LoadDistFileHistory(){

	CString szCurTime;
	int ntype=0;
	int Year,Month,day;
	swscanf_s(m_strDbgStartTime,L"%04d-%02d-%02d",&Year,&Month,&day);
	CTime RealTime=CTime(Year,Month,day,0,0,0);
	m_AgentDistMedia.MediaDistArr.RemoveAll();
	m_AgentDistMedia.nMediaCount=0;
	m_AgentDistMedia.strAgencyCode=L"";
	if(m_strPhotoPath.Find(L"%Y%M%D")>0){
		ntype=1;
		szCurTime.Format(L"%04d-%02d-%02d", RealTime.GetYear(),RealTime.GetMonth(),RealTime.GetDay());
	}
	else if(m_strPhotoPath.Find(L"%Y%M")>0){
		ntype=2;
		szCurTime.Format(L"%04d-%02d", RealTime.GetYear(),RealTime.GetMonth());
	}
	else if(m_strPhotoPath.Find(L"%Y")>0){
		ntype=3;
		szCurTime.Format( L"%04d ", RealTime.GetYear());
	}
	else {
		ntype=4;
		szCurTime=L"00:00:00";
	}
	if (!p_dbCmd->LoadDateProcessMedia(&m_AgentDistMedia, db_config->szTeleCode, RealTime.GetYear(), RealTime.GetMonth(), RealTime.GetDay(),ntype))
	{ 	
		m_strDisplayMessage.Format(L"Load wires_photo_src failed \r\n");
		DisplayMessage(m_strDisplayMessage,0);
		return FALSE;
	}

}

void CDBGraphicMain::InitProcessParam(){
	CString strFilePath;
	strFilePath = GetPath(db_config->szSourcePath,m_checkYsd);
	m_szSourcePath=strFilePath;
	m_szTxtSrcPath = GetPath(db_config->szTextPath,m_checkYsd);
	db_config->bAutoBitmap=1;
	m_szBmpSrcPath= L"";		//原簡圖路徑
	m_szPhotoDesPath=L"";	//原--源圖的COPY路徑
}
//End Leon Li 2013-01-03

void CDBGraphicMain::InitEmailParam(){
	m_EmailParam->nSmtpPort=m_nPort;
	m_EmailParam->strFromAdd=m_FromAdd;
	m_EmailParam->strPassWord=m_PassWord;
	m_EmailParam->strSmtpServer=m_SmtpHost;
	m_EmailParam->strToAdd=m_ToAdd;
	m_EmailParam->strUserName=m_FromAdd;
	m_EmailParam->nSecureType=m_nSecure;
	m_EmailParam->SmtpType=SMTP_TYPE::smtp_default;
}

LRESULT CDBGraphicMain::UpdateUIStatus(WPARAM wParam, LPARAM lParam){
	if (lParam==TRUE){
		GetDlgItem(IDSTART)->EnableWindow(TRUE);
		//GetDlgItem(IDSTOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADDAGENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATUS)->SetWindowText(L"就緒");
		//m_stcProcessAgency.SetWindowTextW(L"");
		//m_stcProcessingFile.SetWindowTextW(L"");
		//m_stcTotalChkCount.SetWindowTextW(L"");
		GetDlgItem(IDEXIT)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDSTART)->EnableWindow(FALSE);
		GetDlgItem(IDSTOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_ADDAGENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATUS)->SetWindowText(L"入庫中");
		GetDlgItem(IDEXIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CREATETRACELOGCHECK)->EnableWindow(FALSE);
	}
	return 1;
}

void CDBGraphicMain::UpdateMainFrameMessage(CString strMessageHeader,CString strMessageContent){
	AddItem(strMessageHeader,strMessageContent);
	/*DispalyMessage* DBGMessage=new DispalyMessage;
	DBGMessage->strMessageHeader=strMessageHeader;
	DBGMessage->strMessageContent=strMessageContent;
	SendMessage(MESSAGE_INSERT_LIST_MESSAGE,0,(LPARAM)DBGMessage);
	delete DBGMessage;*/
}

/*LRESULT CDBGraphicMain::UpdateMainFrameMessage(WPARAM wParam, LPARAM lParam){
	DispalyMessage* DBGMessage=(DispalyMessage *)lParam;
	AddItem(DBGMessage->strMessageHeader,DBGMessage->strMessageContent);
	return 1;
}*/

BOOL CDBGraphicMain::GetFileCaption(CString strFilePath,CString &strCaption,int &iType){
	CString caption1;
	/*wchar_t str[51200]=L"";
	char str1[51200]="";
	WORD word;
	CFile fil;*/

	CStdioFileEx classFileEx;
	strCaption=L"";
	try{
		if(classFileEx.Open(strFilePath,CFile::modeRead | CFile::shareDenyNone))
		//if (fil.Open(strFilePath,CFile::modeRead | CFile::shareDenyNone))
		{
			/*fil.SeekToBegin();
			fil.Read(&word,2);
			if(word == 0xBBEF || word ==0xBBBF || word ==0xBBEFBF)
			{
				if (word == 0xBCEF)
					fil.Seek(0,CFile::begin);
				else
					fil.Seek(0,CFile::begin);
				int nFileLen = (int)fil.GetLength()-3;
				char* szDbsBuffer = new char[nFileLen+1];
				ZeroMemory(szDbsBuffer, sizeof(char)*(nFileLen+1));
				int nRead = (int)fil.Read(szDbsBuffer, nFileLen);
				ASSERT(nRead == nFileLen);

				int nMBLen = MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, NULL, NULL);
				WCHAR* szWcsBuffer = new WCHAR[nMBLen+1];
				ZeroMemory(szWcsBuffer, sizeof(WCHAR)*(nMBLen+1));
				MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, szWcsBuffer, nMBLen);
				strCaption = szWcsBuffer;
				DisplayTraceMessage(L"(UTF-8) "+strCaption,0,FALSE);
				delete[]szDbsBuffer;
				delete[]szWcsBuffer;
				iType = 0;
			}
			else if(word==0xFEFF){
				fil.Seek(2,CFile::begin);
				fil.Read(str,(int)fil.GetLength()-2);
				strCaption=(CString)str;
				DisplayTraceMessage(L"(Unicode) "+strCaption,0,FALSE);
				iType = 1;
			}
			else{
				fil.SeekToBegin();
				fil.Read(str1,(int)fil.GetLength());
				strCaption=(CString)str1;
				iType = 3;

				int nMBLen = MultiByteToWideChar(CP_UTF8, 0, str1, fil.GetLength(), NULL, NULL);
				WCHAR* szWcsBuffer = new WCHAR[nMBLen+1];
				ZeroMemory(szWcsBuffer, sizeof(WCHAR)*(nMBLen+1));
				MultiByteToWideChar(CP_UTF8, 0, str1, fil.GetLength(), szWcsBuffer, nMBLen);

				nMBLen = WideCharToMultiByte(CP_UTF8,NULL,szWcsBuffer,-1,NULL,0,NULL,FALSE);
				char *szWcsBuffer2 = new char[nMBLen+1];
				WideCharToMultiByte (CP_UTF8,NULL,szWcsBuffer,-1,szWcsBuffer2,nMBLen+1,NULL,FALSE);

				if (strcmp(szWcsBuffer2,str1) == 0 && strCaption.GetLength()>wcslen(szWcsBuffer))
				{
					strCaption = szWcsBuffer;  //UTF-8 without BOM
					iType = 0;
					DisplayTraceMessage(L"(UTF-8 without BOM) "+strCaption,0,FALSE);
				}
				else
					DisplayTraceMessage(L"(ASCII) "+strCaption,0,FALSE);
					
				delete []szWcsBuffer;
				delete []szWcsBuffer2;
			}
			fil.Close();*/
			classFileEx.ReadString(strCaption);
			iType = classFileEx.GetFileType();
			while(classFileEx.ReadString(caption1))
				strCaption+=caption1;
			iType = classFileEx.GetFileType();
			classFileEx.Close();
		}
		else return FALSE;
	}
	catch(...){
		return FALSE;
	}
	strCaption.TrimRight();
	strCaption.TrimLeft();
	return TRUE;
}

BOOL CDBGraphicMain::GetPhotoCaption(CString strFilePath,CString &strPhotoCaption,CString &strAddCaption){
	CFile fil;
	char szBuffer[5120]={0};
	DWORD dLength=5120;
	memset(szBuffer,0,5120);
	BOOL bNoCap=FALSE;
	strPhotoCaption=L"";
	strAddCaption=L"";
	if(fil.Open(strFilePath,CFile::modeRead))
	{
		long bsign,sign,len,i;
		i=0;
		len=0;
		sign=0;
		bsign=0;
		BOOL bHasCap=FALSE;
		BYTE bMarker=0;
		len=fil.GetLength();
		for(;i<len;i++)
		{
			//photoshop7.0的圖片圖說沒有找到﹐所以修改此處
			//below delete by seesea 2003.09.26
			/*fil.Seek(i,CFile::begin);
			fil.Read(&bsign,2);
			if((bsign&0xffff)==0xdaff)
			{
				sign = 0;
				break;
			}*/
			//end delete
			/*fil.Seek(i, CFile::begin);
			fil.Read(&sign,3);
			if((sign&0xffff)==0x021c && (sign & 0xff0000))
			{
				break;
			}*/
			fil.Seek(i, CFile::begin);
			fil.Read(&sign,3);
			fil.Read(&bMarker,1);
			if((sign&0xffff)==0x021c&&IsCaptionText((sign>>16)&0xff))						
				break;
			else if((sign&0xffff)==0xedff)//0xd9ff 標誌結束位 0xd8ff 標識開始位 
			{
				if(((sign&0xff0000)!=0x0))
					len=i+((sign&0xff0000)>>8)+bMarker;
				bHasCap=TRUE;
			}
		}
		fil.Seek(i+3, CFile::begin);
		while(bHasCap)
		{
			len=0;
			if((sign&0xffff)!=0x021c)
				break;
			int code;
			len=0;
			code=(sign>>16)&0xff;
			fil.Read(&len,2);
			len=(len>>8)+((len&0xff)<<8);
			memset(szBuffer,0,sizeof(szBuffer));
			//fil.Read(szBuffer,len>510?510:len);
			fil.Read(szBuffer,len>2048?2048:len);
			if(IsCaptionText(code)&& len>0)
			{
				//CString st=szBuffer;	
				WCHAR * szwBuffer = new WCHAR[dLength*2];
				MultiByteToWideChar(CP_ACP, 0, szBuffer, -1, szwBuffer, dLength*2+1);
				CString st=szwBuffer;
				st.TrimLeft();
				st.TrimRight();
				if(st!="")
				{
					if(code==0x78)
					{
						strPhotoCaption	+= st;
						strPhotoCaption+="\r\n";
					}else
					{
						strAddCaption+=st;
						strAddCaption+="\r\n ";
					}
				}

				delete szwBuffer;
			}
			sign=0;
			fil.Read(&sign,3);
		}
		fil.Close();
	}
	else return FALSE;
	if (lstrlen(strPhotoCaption)==0)
		return FALSE;
	return TRUE;
}

int CDBGraphicMain::IsCaptionText(int code)
{
	int CaptionCode[]={0x78,0x7a,0x69,0x28,0x50,0x55,0x6e,0x73,0x05,0x37,0x5a,0x5f,
		0x65,0x67,0x0f,0x14,0x19,0x74,-1};
	int i;
	for(i=0;CaptionCode[i]!=-1;i++)
		if(CaptionCode[i]==code) return TRUE;
	return FALSE;
}

void CDBGraphicMain::DeleteTempEPSFile()
{
	// delete previous file
	try
	{
		if (m_strConvertEPSPath != L"")
		{
			CString strTempPath = m_szSystemTempPath;
			if(_taccess(strTempPath + L"\\temp_image\\" + m_strConvertEPSPath + L".jpg", 0) == 0)
			{
				_tremove(strTempPath + L"\\temp_image\\" + m_strConvertEPSPath + L".jpg");
			}

			if(_taccess(strTempPath + L"\\temp_image\\" + m_strConvertEPSPath + L".eps", 0) == 0)
			{
				_tremove(strTempPath + L"\\temp_image\\" + m_strConvertEPSPath + L".eps");
			}

		}
	}
	catch (...) 
	{
	}
	//
}

void CDBGraphicMain::DistributeAV(int nCurNum){
	CFileFind filfind;
	RETCODE bResult;
	BOOL bDistribute=FALSE;
	CString str,strSrcMediaPath,strFileName,strFileExt,szSrcFilePath,strSrcAVPath,strPhotoName,strDestPath,strDestFullPath,strFolderName,strSrcPhotoPath;
	CString strFullPath,strRootPath,strFullSrcPath,strAVFullPath;
	CString strPhotoEx=m_strPhotoEx;
	strPhotoEx.MakeLower();
	strSrcAVPath=GetPath(m_strPhotoPath,m_checkYsd)+L"\\*.*";
	strDestPath=GetPath(m_strTargetPathO,m_checkYsd);
	strSrcMediaPath=GetPath(m_strPhotoPath,m_checkYsd);
	m_strDisplayMessage.Format(L"Checking [%s] [影音] [Ext: %s] [Media limit: %d].\r\n",db_config->szTeleName,db_config->szPhotoExtName,m_MaxTm);
	DisplayMessage(m_strDisplayMessage,1);
	m_totalFileExt[nCurNum]=ComputeTotalFile(strSrcMediaPath,strPhotoEx);
	//Edit by Leon Li 2013-02-28 for enhancement: remove dat file and check photo record in memeory
	// CheckTargetDate();
	//LoadDatHistory();
	if (m_checkYsd)
		getYesterday(m_strDbgStartTime);
	else
		m_strDbgStartTime.Format(L"%04d-%02d-%02d %02d:%02d:%02d",CTime::GetCurrentTime().GetYear(),CTime::GetCurrentTime().GetMonth(),CTime::GetCurrentTime().GetDay()
			,CTime::GetCurrentTime().GetHour(),CTime::GetCurrentTime().GetMinute(),CTime::GetCurrentTime().GetSecond());
	LoadDistFileHistory();
	InitProcessParam();
	//End Leon Li 2013-02-28
	m_stcProcessingFile.SetWindowTextW(L"");
	BOOL bFindFile = filfind.FindFile(strSrcAVPath);
		while(bFindFile){
			bFindFile = filfind.FindNextFile();
		  if(filfind.IsDots())  
			  continue; 
		  if(filfind.IsDirectory()) 
			  continue;  
		  strFileName=filfind.GetFileName();
		  strFileExt=strFileName.Right(strFileName.GetLength()-strFileName.ReverseFind('.')-1);
		  strFileExt.MakeLower();
		  szSrcFilePath=filfind.GetFilePath();
		  if (strPhotoEx.Find(strFileExt)>=0 && num_copy<m_MaxTm)
		  {	  
			  //check already in distributed locally?
			  ///////////////////////////
			  CTime time;
			  CString strLastModifyTime,strFilePath,strFileTime;
			  DISTFILEITEM item;
			  filfind.GetLastWriteTime(time);
			  strLastModifyTime.Format(L"%04d-%02d-%02d %02d:%02d:%02d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());	
			  for(int i = 0; i < m_AgentDistMedia.MediaDistArr.GetSize(); i++)
			  {
				  item = m_AgentDistMedia.MediaDistArr.GetAt(i);
				  strFileTime=item.strFileTime;
				  strFilePath=item.strFilePath;
				  strFileTime.TrimLeft();
				  strFileTime.TrimRight();
				  strFilePath.TrimLeft();
				  strFilePath.TrimRight();
				  if((lstrcmp(szSrcFilePath, strFilePath) == 0) && (lstrcmp(strFileTime, strLastModifyTime) == 0))
				  {

					  m_disFile++;
					  bDistribute=TRUE;
					  break; // the file has been distributed
				  }
			  }
			  if (bDistribute) {
				  bDistribute=FALSE;
				  continue;
			  }
			  ///////////////////////////
			  /*check if already in database*/
			  ///////////////////////////////
			  en_ACCESSDBRET ret = p_dbCmd->ImageIsLogged((LPWSTR)(LPCTSTR)szSrcFilePath,(LPWSTR)(LPCTSTR)strLastModifyTime, db_config->bCheckTime);
			  if(ret == SUCCESS_TRUE)
			  {
				  m_exist++;
				  continue;
			  }
			  else if(ret == ACCESSDBERROR)
			  {
				  continue;
			  }
			  /*copy photo to destination folder*/
			  ////////////////////////////////
			  //sz_DefaultPhoto
			  m_stcProcessingFile.SetWindowTextW(L"正在處理:\n"+szSrcFilePath);
			  m_strMediaCaption.Empty();
			  strDestFullPath.Empty();
			  CString strSmlPhotoPath;
			  if (_waccess(strDestPath,0)==0  && !strDestPath.IsEmpty())
			  {
				  CString str;str.Format(L"Process on: %s", strDestPath);
				  DisplayTraceMessage(str,0,FALSE);
				  CheckAVName(strDestPath,strFileName,strFolderName);
				  strDestFullPath.Format(L"%s\\%s",strDestPath,strFileName);
				  try{
					 DisplayTraceMessage(L"Copy Media",0,FALSE);
				  if (CopyFile(filfind.GetFilePath(),strDestFullPath,FALSE)==FALSE){
					  str.LoadStringW(AFX_IDS_MEDIA_COPY_ERROR);
					  m_strDisplayMessage.Format(L"%s,[file name: %s],[source path: %s],[destination path: %s]",str,filfind.GetFileName(),filfind.GetFilePath(),strDestFullPath);
					  DisplayMessage(m_strDisplayMessage,0);
				  }
				  else  if(!CopyCaptionPhoto(m_szSourcePath,strFolderName,strSmlPhotoPath,strFileName=filfind.GetFileName()))
						{
							str.LoadStringW(IDS_MEDIA_CAPTION_PHOTO_ERROR);
							m_strDisplayMessage.Format(L"%s,[file name: %s]",str,filfind.GetFileName());
							DisplayMessage(m_strDisplayMessage,0);
				  }
				  }
				  catch(...){
					  continue;
				  }
			  }
			  else{
			  /*create sourct folder for photo and copy photo to this folder*/
			  //////////////////////////////
				strFileName=filfind.GetFileName();
				//strFolderName.Format(L"%s\\%s",m_szSourcePath,strFileName.Left(strFileName.ReverseFind('.')));
				strFolderName.Format(L"%s\\%s_%s",m_szSourcePath,strFileName.Left(strFileName.ReverseFind('.')),strFileName.Right(strFileName.GetLength()-strFileName.ReverseFind('.')-1));
				if (MakeDir(strFolderName))
				 {
					 CopyCaptionPhoto(m_szSourcePath,strFolderName,strSmlPhotoPath,strFileName);
				 }
				 }
			  //////////////////////////////

				  //////////////////////////
				  //copy text file to correspondent folder and read file
				  /*CString strSrcTextPath,strTgtTextPath;
				  CFileFind textfind;
				  strSrcPhotoName=strFileName.Left(strFileName.ReverseFind('.'));
				  strSrcTextPath.Format(L"%s\\%s.txt",m_strAVPath,strSrcPhotoName);
				  BOOL bFindText = textfind.FindFile(strSrcTextPath);
				  while(bFindText){
					  bFindText = textfind.FindNextFile();
					  strTgtTextPath.Format(L"%s\\%s",strFolderName,textfind.GetFileName());
					  CopyFile(textfind.GetFilePath(),strTgtTextPath,FALSE);
					  }*/
				  //////////////////////////
			  //generate new image name which is photo code
			 /////////////////////////
			  wchar_t szFileCode[20];	
			  if(p_dbCmd->GetNewImageName(szFileCode) == ACCESSDBERROR){
				  str.LoadStringW(IDS_DB_GETFILEID_ERROR);
				  m_strDisplayMessage.Format(L"%s [file name %s]",str,strFileName);
				  DisplayMessage(m_strDisplayMessage,0);
				  continue;
			  }
				  /////////////////////////
			  //insert new image record to database wires_photo_src table 
			  CString strCurTime;
			  CTime  curTime  =   CTime::GetCurrentTime();
		//	  if (!m_checkYsd)
		//		  curTime+= CTimeSpan(1,0,0,0);
			  strCurTime.Format(L"%04d-%02d-%02d %02d:%02d:%02d",curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());	  
			  if (strDestFullPath.IsEmpty())
			  {
				  strDestFullPath=szSrcFilePath;
			 }
			   
			  ////////////////////////
			  strFullSrcPath = szSrcFilePath;
			  DisplayTraceMessage(L"Start insert Media to DB",0,FALSE);
			  bResult=p_dbCmd->InsertMediaValues(db_config->szTeleCode,db_config->nAgencyID,
				  strCurTime,strDestFullPath/*Photo Path*/,strFullSrcPath/*Photo src path*/,strFolderName,strFileExt,szFileCode,strLastModifyTime,strSmlPhotoPath,strSmlPhotoPath,filfind.GetLength()/1024,m_strMediaCaption,db_config->nTitleLine,0);
			  if (bResult!=SUCCESS_TRUE)
			  {
				  str.LoadStringW(IDS_DB_INSERTPHOTOITEM_ERROR);
				  m_strDisplayMessage.Format(L"%s,[file name]",strFileName);
				  DisplayMessage(m_strDisplayMessage, 1);
				  continue;
			 }
			  else {
				  m_totalPrs++;
				  num_copy++;
				  m_stcProcessingFile.SetWindowTextW(L"成功處理:\n"+szSrcFilePath);
			  }		  
			  //Save record to dat file
			  /////////////////////////
			  item.strFilePath=strFullSrcPath;
			  item.strFileTime=strLastModifyTime;
			  item.strMediaName=szFileCode;
			  m_AgentDistMedia.MediaDistArr.Add(item);
			  m_AgentDistMedia.nMediaCount++;
			  /////////////////////////
			  //Load Metadata and save to database
			  /////////////////////////
			  CString strMetaData;
			  DisplayTraceMessage(L"Load meta data",0,FALSE);
			  if(!LoadAVMetaData(strDestFullPath,szFileCode)){
				  m_strDisplayMessage.LoadString(AFX_IDS_LOADMETADATA_ERROR);
				  DisplayMessage(m_strDisplayMessage,0);
			  }
			  else p_dbCmd->UpdateMediaCaption(szFileCode,m_strMediaCaption,1);
			  //////////////////////////
			  //Load User define AV info
			  CString strSrcTextPath,strTgtTextPath,strContent,strFileLine,strEntryName,strEntryValue,temp;
			  int lFileSize=0;
			  strFileName=filfind.GetFileName().Left(filfind.GetFileName().ReverseFind('.'));
			  strSrcTextPath.Format(L"%s\\%s.txt",m_szSourcePath,strFileName);
			  m_classFileEx->Open(strSrcTextPath,CFile::modeRead);
			  m_classFileEx->ReadString(strContent);
			  while (m_classFileEx->ReadString(temp))
				  strContent += temp;
			  if (!strContent.IsEmpty())
			  {
				  int nStartPos=0,nEndPos=0,nPos;
				  while ((nEndPos=strContent.Find(L"\r\n",nStartPos))>0 || nStartPos<lFileSize/2)
				  {
					  if(nEndPos<0)
						nEndPos=lFileSize/2;
					  strFileLine=strContent.Mid(nStartPos,nEndPos-nStartPos);
					  strFileLine.TrimLeft();
					  strFileLine.TrimRight();
					  nStartPos=nEndPos+1;
					  if(strFileLine.IsEmpty())
						  ;
					  else{
						 nPos=strFileLine.Find(L"=");
						 if(nPos>=0){
						 strEntryName=strFileLine.Mid(0,nPos);
						 strEntryName.TrimRight();
						 strEntryName.TrimLeft();
						 strEntryValue=strFileLine.Right(strFileLine.GetLength()-nPos-1);
						 strEntryValue.TrimLeft();
						 strEntryValue.TrimRight();
						 if (strFileLine.Find(L"Comment"))
							 m_strMediaCaption=strEntryValue;
						 if(!p_dbCmd->InsertMediaInfo(szFileCode,L"Customer",strEntryName,strEntryValue)==SUCCESS_TRUE){
							  str.LoadStringW(IDS_DB_INERTPHOTOINFO_ERROR);
							  m_strDisplayMessage.Format(L"%s,[File Code %s],[Entry Name %s],[Entry Value %s].",str,strFileName,strEntryName,strEntryValue);
							  DisplayMessage(m_strDisplayMessage,0);
						 }
						}
					}
				 }
				 }
			  if(m_classFileEx->m_hFile != CFile::hFileNull)
				  m_classFileEx->Close();
			  DisplayTraceMessage(L"Update Caption",0,FALSE);
			  p_dbCmd->UpdateMediaCaption(szFileCode,m_strMediaCaption,db_config->nTitleLine);
			 // m_classFileEx->Close();
			  //////////////////////////
			CString str_NumCopy; str_NumCopy.Format(L"%d", num_copy);
			m_stcTotalChkCount.SetWindowTextW(L"本次已成功入庫數目:"+str_NumCopy);
		 }
		 if(bFlagStop == TRUE)
						break;
	}
	/*Display Message after one agency*/
	m_skipped+=m_disFile+m_exist;
	m_totalrm=m_totalFileExt[nCurNum]-(m_skipped+num_copy);
	CString strMediafolder=strSrcMediaPath.Right(strSrcMediaPath.GetLength()-strSrcMediaPath.ReverseFind('\\')-1);
	if (m_totalrm)
		m_strDisplayMessage.Format(_T("%s: [Folder: %s]  [Media: %d]  [Distributed: %d]  [Remained: %d] [Processed: %d].\r\n\r\n"),db_config->szCode,strMediafolder,m_totalFileExt[nCurNum],m_disFile+m_exist,m_totalrm<=0?0:m_totalrm,num_copy);
	else 
		m_strDisplayMessage.Format(_T("%s: [Folder: %s]  [Media: %d]  [Distributed: %d]  [Processed: %d]. \r\n\r\n"),db_config->szCode,strMediafolder,m_totalFileExt[nCurNum],m_disFile+m_exist,num_copy);
	DisplayMessage(m_strDisplayMessage,1);
	/*End display message*/
	/*Close some file at the end*/
	////////////////////
	filfind.Close();
	/*End close file operation*/
}

int CDBGraphicMain::ComputeTotalFile(CString strFilePath,CString strFileExt){
	BOOL ifBreak=TRUE;
	CString strEx;
	int nTotal=0;
	CFileFind  file; 
	CString szSrcPath;
	while(ifBreak){
		if (strFileExt.ReverseFind(';') != -1)
		{
			strEx = strFileExt.Right(strFileExt.GetLength()-strFileExt.ReverseFind(';')-1);
			strFileExt = strFileExt.Left(strFileExt.ReverseFind(';'));
		}
		else
		{
			strEx=strFileExt;
			ifBreak=FALSE;
		}
		CString str=strEx;//m_lpdfs->szPhotoExtName;
		str.TrimLeft();
		str.TrimRight();
		if(!str.IsEmpty())
		{
			int nFind=str.ReverseFind('.');
			if(nFind!=-1)
				szSrcPath.Format(L"%s\\%s",strFilePath,str);
			else
				szSrcPath.Format(L"%s\\*.%s",strFilePath,strEx);
		}
		BOOL   bFind=file.FindFile(szSrcPath); 
		while(bFind) 
		{ 
			bFind   =   file.FindNextFile(); 
			if(file.IsDots())  
				continue; 
			if(file.IsDirectory()) 
				continue;
			nTotal++; 
		} 
	}
	return nTotal;
}

BOOL CDBGraphicMain::LoadAVMetaData(CString strAVPath,CString strPhotoName){
	MediaInfo MI;
	String strPath,To_Display;
	strPath=strAVPath.GetBuffer(0);
	if(MI.Open(strPath)==FALSE)
		return FALSE;
	MI.Option(__T("Complete"));
	To_Display += MI.Inform().c_str();
	CFile fil;
	CString str,strEntryType,strEntryName,strEntryValue,strFileLine,strFileContent=To_Display.c_str();
	int nStartPos=0,nEndPos=0,nPos;
	while ((nEndPos=strFileContent.Find(L"\r\n",nStartPos))>0)
	{
		strFileLine=strFileContent.Mid(nStartPos,nEndPos-nStartPos);
		strFileLine.TrimLeft();
		strFileLine.TrimRight();
		nStartPos=nEndPos+1;
		if (!lstrcmp(strFileLine,L"General"))
			strEntryType=L"General";
		else if(!lstrcmp(strFileLine,L"Video"))
			strEntryType=L"Video";
		else if(!lstrcmp(strFileLine,L"Audio"))
			strEntryType=L"Audio";
		else if(strFileLine.IsEmpty())
			;
		else {
			nPos=strFileLine.Find(L":");
			strEntryName=strFileLine.Mid(0,nPos);
			strEntryName.TrimRight();
			strEntryName.TrimLeft();
			strEntryValue=strFileLine.Right(strFileLine.GetLength()-nPos-1);
			strEntryValue.TrimLeft();
			strEntryValue.TrimRight();
			if (!lstrcmp(strEntryName,L"Comment"))
				m_strMediaCaption=strEntryValue;
			if(!p_dbCmd->InsertMediaInfo(strPhotoName,strEntryType,strEntryName,strEntryValue)==SUCCESS_TRUE){
				str.LoadStringW(IDS_DB_INERTPHOTOINFO_ERROR);
				m_strDisplayMessage.Format(L"%s,[File Code %s],[Entry Name %s],[Entry Value %s].",strPhotoName,strEntryName,strEntryValue);
				DisplayMessage(m_strDisplayMessage, 1);
			}
		}
	}

	if(m_strMediaCaption.GetLength()==0)
		m_strMediaCaption = db_config->szCaption;
	return TRUE;
}

BOOL CDBGraphicMain::CopyCaptionPhoto(CString strSrcPath,CString strTgtPath,CString &strSmlPhotoPath,CString strFileName,BOOL bRemove/*Determine Copy or remove file*/){
	//copy corresponding photo to destination folder
	CFileFind photofind;
	CString strPhotoName,str,strSrcPhotoName,strTgtPhotoPath,strSrcPhotoPath;
	strSrcPhotoPath.Format(L"%s\\*.jpg",strSrcPath);
	int nPhotoCnt=0;
	BOOL bFindPhoto = photofind.FindFile(strSrcPhotoPath),bFisrtPhoto=TRUE,bCopy=FALSE;
	strSrcPhotoName=strFileName.Left(strFileName.ReverseFind('.'));
	while(bFindPhoto){
		bFindPhoto = photofind.FindNextFile();
		strPhotoName=photofind.GetFileName();
		if (nPhotoCnt<MEDIA_AV_PHOTO_MAX){
			if (isCaptionPhoto(strSrcPhotoName+L"_",strPhotoName)){
				strTgtPhotoPath.Format(L"%s\\%s",strTgtPath,strPhotoName);
				//if (bRemove){
					if(MoveFileW(photofind.GetFilePath(),strTgtPhotoPath)){		
						nPhotoCnt++;
						bCopy=TRUE;
					}
				//}
				//else {
				//	if(CopyFile(photofind.GetFilePath(),strTgtPhotoPath,FALSE)){		
				//		nPhotoCnt++;
				//		bCopy=TRUE;
				//}
				//}
				if (bFisrtPhoto)
				{
					strSmlPhotoPath=strTgtPhotoPath;
					bFisrtPhoto=FALSE;
				}
				}
				else continue;
				}
			}
	if (!bCopy)
	{
		strTgtPhotoPath.Format(L"%s\\%s",strTgtPath,strSrcPhotoName+L"_1.jpg");
		if (bRemove)
		{
			if (MoveFileW(sz_DefaultPhoto,strTgtPhotoPath)){
				bCopy=TRUE;
				strSmlPhotoPath=strTgtPhotoPath;
			}
		}
		else{
			if (CopyFile(sz_DefaultPhoto,strTgtPhotoPath,FALSE)){
				bCopy=TRUE;
				strSmlPhotoPath=strTgtPhotoPath;
			}
		}
	}
	return bCopy;
}

BOOL isCaptionPhoto(CString strSrcPhotoName,CString strPhotoName){
	int nNameLength=strSrcPhotoName.GetLength();
	CString strPartName=strPhotoName.Mid(0,nNameLength);
	if (!lstrcmp(strPartName,strSrcPhotoName))
		return TRUE;
	else return FALSE;
}

void CDBGraphicMain::CheckAVName(CString strFilePath,CString &strFileName,CString &strFolderName){
	CFileFind fil,folder;
	CString strOrgName;//file original name
	CString strFullName,strExt;
	strOrgName=strFileName.Left(strFileName.ReverseFind('.'));
	strExt=strFileName.Right(strFileName.GetLength()-strFileName.ReverseFind('.')-1);
	strFolderName.Format(L"%s\\%s_%s",strFilePath,strOrgName, strExt);
	strFullName.Format(L"%s\\%s",strFilePath,strFileName);
	int nCount=1;
	while(fil.FindFile(strFullName) || folder.FindFile(strFolderName)){
		fil.FindNextFile();
		folder.FindNextFile();
		strFileName.Format(L"%s_%d.%s",strOrgName,nCount,strExt);
		strFullName.Format(L"%s\\%s",strFilePath,strFileName);
		strFolderName.Format(L"%s\\%s_%s_%d",strFilePath,strOrgName,strExt,nCount);
		nCount++;
	}
	MakeDir(strFolderName);
}

BOOL CDBGraphicMain::ReadReutersXML(CString& strCaption, CString strFileName)
{
	strCaption = L"";
	BOOL nRet = FALSE;
	
		::CoInitialize(NULL);

	HRESULT hr = m_plDomDocument.CreateInstance(MSXML::CLSID_DOMDocument);
	if (FAILED(hr))
	{
		 _com_error er(hr);
		 //AfxMessageBox(er.ErrorMessage());
	}
	hr = m_plDomDocumentPre.CreateInstance(MSXML::CLSID_DOMDocument);
	if (FAILED(hr))
	{
		 _com_error er(hr);
		 //AfxMessageBox(er.ErrorMessage());
	}

	MSXML::IXMLDOMElementPtr pDocElementAgency;
	variant_t vLoad;

	long lXMLCount = 0 ;
	

	wchar_t pwcharMessageBody[PATH_LENGTH1];
	lstrcpy(pwcharMessageBody,strFileName);
	vLoad = m_plDomDocumentPre->load(pwcharMessageBody);

	if (((bool)vLoad) == TRUE )
	{
		pDocElementAgency = m_plDomDocumentPre->documentElement;
		CString strXML, strKey = L"<description role=\"descRole:caption\">";
		strXML = pDocElementAgency->xml.GetBSTR();
		strCaption = strXML.Right(strXML.GetLength()-strXML.Find(L"<description role=\"descRole:caption\">")-strKey.GetLength());
		strCaption = strCaption.Left(strCaption.Find(L"</description>"));
		nRet = TRUE;
	}
	::CoUninitialize();
	//return FALSE;
	return nRet;
}

void CDBGraphicMain::OnClickedTestMail()
{
	m_EmailParam->strSenderName=L"EMS DBGraphic";
	m_EmailParam->strContent=L"DBGraphic 電郵測試";
	m_EmailParam->strEmailHeader=L"DBGraphic Testing Email";
//	SendEmail();
	
	CString strMsg =SendEMSEmail(m_EmailParam);
	if (strMsg.Find(L"Succeed")>=0)
		AfxMessageBox(L"測試成功");
	else
		AfxMessageBox(L"測試失敗");

	// TODO: Add your control notification handler code here
}
