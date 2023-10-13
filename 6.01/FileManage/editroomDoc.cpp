// editroomDoc.cpp : implementation of the CEditroomDoc class
//

#include "stdafx.h"

#include "filemanage.h"
#include "..\commkits\dostools.h"
#include "dofilemgr.h"
#include "fileinfodlg.h"
#include "dofilefrm.h"
#include "..\fileoper\fileopermgr.h"
#include "io.h"
#include "TransDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CEditroomDoc
extern "C" void WINAPI DoFile();
extern "C" void WINAPI DoColFile();
extern "C" void WINAPI DoFileWithID(CString *strFileID);
extern "C" void WINAPI DoColFileWithID(CString *strFileID);
IMPLEMENT_DYNCREATE(CEditroomDoc, CEditProDoc)

BEGIN_MESSAGE_MAP(CEditroomDoc, CEditProDoc)
	//{{AFX_MSG_MAP(CEditroomDoc)
	ON_COMMAND(ID_DO_COMMIT, OnDoCommit)
	ON_UPDATE_COMMAND_UI(ID_DO_COMMIT, OnUpdateDoCommit)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_DO_MODIFY, OnDoModify)
	ON_UPDATE_COMMAND_UI(ID_DO_MODIFY, OnUpdateDoModify)
	ON_COMMAND(ID_DO_SEL, OnDoSel)
	ON_UPDATE_COMMAND_UI(ID_DO_SEL, OnUpdateDoSel)
	ON_COMMAND(ID_DO_SOURCE, OnDoSource)
	ON_UPDATE_COMMAND_UI(ID_DO_SOURCE, OnUpdateDoSource)
	ON_COMMAND(ID_DO_SIGN, OnDoSign)
	ON_UPDATE_COMMAND_UI(ID_DO_SIGN, OnUpdateDoSign)
	ON_COMMAND(ID_DO_SIGNDIRECT, OnDoSigndirect)
	ON_UPDATE_COMMAND_UI(ID_DO_SIGNDIRECT, OnUpdateDoSigndirect)
	ON_COMMAND(ID_RDO_RECORD, OnRdoRecord)
	ON_UPDATE_COMMAND_UI(ID_RDO_RECORD, OnUpdateRdoRecord)
	ON_COMMAND(ID_RDO_COMMENT, OnRdoComment)
	ON_UPDATE_COMMAND_UI(ID_RDO_COMMENT, OnUpdateRdoComment)
	ON_COMMAND(ID_RDO_COPY, OnRdoCopy)
	ON_UPDATE_COMMAND_UI(ID_RDO_COPY, OnUpdateRdoCopy)
	ON_COMMAND(ID_DO_SENDCOL, OnDoSendcol)
	ON_UPDATE_COMMAND_UI(ID_DO_SENDCOL, OnUpdateDoSendcol)
	ON_COMMAND(ID_DO_BACKFILE, OnDoBackfile)
	ON_UPDATE_COMMAND_UI(ID_DO_BACKFILE, OnUpdateDoBackfile)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SAVE,OnFileSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditroomDoc construction/destruction

CEditroomDoc::CEditroomDoc()
{
	// TODO: add one-time construction code here
	memset(&m_FileParam,0,sizeof(FILEPARAM));
	CGlobal *pGlobal=CGlobal::GetInstance();
	_tcscpy(m_FileParam.szTempPath,pGlobal->GetTempPath());
	m_FileParam.bCanEdit = TRUE;
//	m_FileParam.bLocal = TRUE;
	m_FileParam.bNewFile = FALSE;
	m_FileParam.bRecord = FALSE;
	m_WiresParam.bCanSaveToLib = FALSE;
	m_WiresParam.strDate = _T("");
	m_WiresParam.strWiresCode = _T("");
	m_WiresParam.strWiresFile = _T("");
	m_DMParam.nDMFI_id = -1;
	m_hDefaultMenu = NULL;
	m_FileParam.bLib = TRUE;
	m_bPending = FALSE;
	m_bNeedSaveDB = FALSE;
	m_bHasCommit =FALSE;
	m_strBakFile=_T("");
	m_bTransfer= TRUE;
}

CEditroomDoc::~CEditroomDoc()
{
	if (m_hDefaultMenu)
		::DestroyMenu(m_hDefaultMenu);	
	if(!m_strBakFile.IsEmpty())
	{
		TRY{
			CFile::Remove(m_strBakFile);
		}
		CATCH(CFileException ,e)
			;
		END_CATCH
	}
}

BOOL CEditroomDoc::OnNewDocument()
{
	if (!CEditProDoc::OnNewDocument())
		return FALSE;
	CGlobal *pGlobal=CGlobal::GetInstance();
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	CString strPath=m_FileParam.szTempPath ;
	
	m_strTmpFileCode=GetTempFileID();
	strPath+=_T("\\")+m_strTmpFileCode;
	SetPathName(strPath);
//	m_FileParam.bLocal = TRUE;
	m_FileParam.bNewFile = TRUE;
	m_FileParam.bRecord = FALSE;
	m_FileParam.nDocStyle=0;
	m_bCreate = TRUE;
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEditroomDoc serialization

void CEditroomDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar<<m_FileParam.bNewFile;
	}
	else
	{
		// TODO: add loading code here
		ar>>m_FileParam.bNewFile;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditroomDoc diagnostics

#ifdef _DEBUG
void CEditroomDoc::AssertValid() const
{
	CEditProDoc::AssertValid();
}

void CEditroomDoc::Dump(CDumpContext& dc) const
{
	CEditProDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditroomDoc commands

void CEditroomDoc::InitChildWnd()
{

}

BOOL CEditroomDoc::InsertText(LPCTSTR lpText)
{
	BYTE *pData;
	LONG lLen=_tcslen(lpText);
	if(lpText>0)
		pData = new BYTE[lLen+1];
	else
		return FALSE;
	_tcscpy((LPWSTR)pData,lpText);
	if (pData)
	{
		// change 0d, 0a to 0d, TS_NORMAL
		LONG l;
		for (l=0; l < lLen; l ++)
		{
			if (pData[l] > ASCII_END)
				l ++;	// chinese
			else if (pData[l] == '\x0d')
			{
				pData[l+1] = TS_NORMAL;
				l ++;
			}
		}
		CEditProDoc::InsertText((BYTE*)pData,lLen);
	}
	delete  pData;
	return TRUE;
}

void CEditroomDoc::OnDoCommit() 
{
	// TODO: Add your command handler code here
	if(IsModified()||m_bPending||m_FileParam.enType==LOCALFILE)
	{
		BOOL bQuit=FALSE;
		BOOL bRet=DoCommit(bQuit);
		if(bRet&&bQuit)
		{
			m_bTransfer = TRUE;
			OnCloseDocument();
		}
	}
	else
	{
		m_bTransfer = TRUE;
		OnCloseDocument();
	}
}

void CEditroomDoc::OnUpdateDoCommit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//明報要求即使沒有修改內容也可以提交
	/*BOOL bEnable = FALSE;
	if (IsModified())
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = m_bPending;
	}*/
	//pCmdUI->Enable(	(m_byteStatus&FS_RDONLY)!=FS_RDONLY);
	pCmdUI->Enable(	m_FileParam.bCanEdit&&m_FileParam.bEdit );
}
CString CEditroomDoc::GetTempFileID()
{
	int id;
	CString str;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	id = GetPrivateProfileIntW( L"AUTOSAVE" , L"EditProID",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//id=AfxGetApp()->GetProfileInt(_T("AUTOSAVE"),_T("EditProID"),0);
	CString str_temp;
	str_temp.Format(L"%d",(id+1));
	WritePrivateProfileStringW(L"AUTOSAVE", L"EditProID",str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//AfxGetApp()->WriteProfileInt(_T("AUTOSAVE"),_T("EditProID"),id+1);
	//end 20120927
	str.Format(_T("%08X"),id);
	return str;
}

void CEditroomDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	CGlobal *pGL=CGlobal::GetInstance();
	if(m_FileParam.bEdit)
		pGL->UnlockFile(m_FileParam.szFileCode);
	
	
	CString str=GetPathName();
	if(m_WiresParam.enOpenType==WIRESFILE)
	{
		try{
			if(_taccess(str,0)!=-1)
				CFile::Remove(str);
		}
		catch (CFileException *e) {
			TCHAR szBuf[256];
			e->GetErrorMessage(szBuf,255);
			e->Delete();
			AfxMessageBox(szBuf);
		}
	}
	int nDocStyle=m_FileParam.nDocStyle;
	BOOL bLib=m_FileParam.bLib;
	//		strPathName.Format("%d",bLib);
	//		AfxMessageBox(strPathName);
	if (m_WiresParam.enOpenType!=WIRESFILE&&m_bTransfer&&CGlobal::GetInstance()->IsWorking())
	{
		
		if(nDocStyle==0  && AfxGetMainWnd()->IsWindowVisible())
		{
			if(bLib)
			{				
				int nExclusive, nLib;
				CString strPlace, strLibName, strTitle;
				
				//if (m_bHasCommit&&pGL->m_FileDB.GetLibFileInfo(m_FileParam.szFileCode, strPlace, strLibName, nLib, nExclusive))
				if (pGL->m_FileDB.GetLibFileInfo(m_FileParam.szFileCode, strPlace, strLibName, strTitle, nLib, nExclusive))
				{						
					if (m_FileParam.bEdit)
					{					
						CTransDlg dlg;
						dlg.m_bNewFile = m_bCreate;
						dlg.m_bExclusive = nExclusive>0;
						dlg.m_nLibID = nLib;
						dlg.m_strFileCode = m_FileParam.szFileCode;
						dlg.m_strPlace = strLibName + strPlace;
						dlg.m_strTitle = m_strDisplayTitle;
						dlg.m_strPlaceName = strPlace;
						dlg.DoModal();
					}
				}
				
				if (m_FileParam.enType != CAPFILE)
					DoFile();
				
				
			}
			else
			{
				if (m_FileParam.enType != CAPFILE)
					DoColFile();
			}
		}
	}
	CEditProDoc::OnCloseDocument();
	
}

BOOL CEditroomDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	/*
	if(!IsModified())
		return TRUE;
	CString strMsg;
	CResManage setRes(g_hInst);
	strMsg.LoadString(IDS_FILEMODIFIED);
	if(AfxMessageBox(strMsg,MB_YESNO,MB_ICONINFORMATION)==IDYES)
		CEditProDoc::OnSaveDocument(GetPathName());
	return TRUE;
	*/

	BOOL bRet = TRUE;
/*
	if (IsModified())
	{
		CString strMsg;
		CResManage setRes(g_hInst);
		strMsg.LoadString(IDS_FILEMODIFIED);
		if(AfxMessageBox(strMsg,MB_YESNO,MB_ICONINFORMATION)==IDYES)
			CEditProDoc::OnSaveDocument(GetPathName());
	}

	if (m_bPending)
	{
		CString strMsg;
		CResManage setRes(g_hInst);
		strMsg.LoadString(IDS_Q_DOCOMMIT);
		if(AfxMessageBox(strMsg,MB_YESNO,MB_ICONINFORMATION)==IDYES)
			bRet = FALSE;
	}
*/
	SaveBkFile();
	if (CGlobal::GetInstance()->IsWorking())
	{
		if (m_bNeedSaveDB||IsModified())
		{
			CString strMsg;
			CResManage setRes(g_hInst);
			strMsg.LoadString(IDS_Q_DOCOMMIT);
			if(AfxMessageBox(strMsg,MB_YESNO,MB_ICONINFORMATION)==IDYES)
			{
				BOOL bQuit=FALSE;
				//return DoCommit(bQuit);
				m_bNeedSaveDB = !DoCommit(bQuit);
				return !m_bNeedSaveDB;
			}
			else
			{
				CString strMsg;
				CResManage setRes(g_hInst);
				strMsg.LoadString(IDS_FILEMODIFIED);
				if(AfxMessageBox(strMsg,MB_YESNO,MB_ICONINFORMATION)==IDYES)
					CEditProDoc::OnSaveDocument(GetPathName());
				
			}
		}
	}

	return bRet;
}

void CEditroomDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	CEditProDoc::OnFileSaveAs();
	try{
	CFileStatus fs;
	CString strTime,strFile;
	strFile=GetPathName();
	if(!CFile::GetStatus(strFile,fs))
		return;
	strTime = m_FileParam.szLastModTime;
	if (!strTime.IsEmpty())
	{	
		COleDateTimeEx dt(strTime.Left(19),_T("%Y-%m-%d %H:%M:%S"));
		fs.m_mtime = CTime(dt.GetYear(),dt.GetMonth(),dt.GetDay(),dt.GetHour(),dt.GetMinute(),dt.GetSecond());
		CFile::SetStatus(strFile,fs);
	}
	SaveBkFile();
	}
	catch (...) {
		AfxMessageBox(IDS_UNKOWNERR);
	}
}

void CEditroomDoc::OnDoModify() 
{
	// TODO: Add your command handler code here
	if(m_FileParam.bCanEdit)
	{
		CGlobal *pGL=CGlobal::GetInstance();
		if(pGL->LockFile(m_FileParam.szFileCode))
		{			
			if(!pGL->CanFileModify(&m_FileParam))
			{
				pGL->UnlockFile(m_FileParam.szFileCode);
				return;
			}
			SetReadOnly(FALSE);
			m_FileParam.bIsLocked = TRUE;
			m_FileParam.bEdit = TRUE;
		}
		else
			AfxMessageBox(IDS_LOCKFILEFAIL);
	}
}

void CEditroomDoc::SetReadOnly(BOOL bReadOnly)
{
	if(m_byteStatus&FS_RDONLY)
	{
		if(!bReadOnly)m_byteStatus ^= FS_RDONLY;
	}
	else
		if(bReadOnly)m_byteStatus ^= FS_RDONLY;
}
void CEditroomDoc::OnUpdateDoModify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(m_FileParam.bCanEdit&&m_byteStatus&FS_RDONLY);
	pCmdUI->Enable(m_FileParam.bCanEdit&&!m_FileParam.bEdit);
}
void CEditroomDoc::GetTitles(TCHAR *szTitle1,TCHAR *szTitle2,TCHAR *szTitle3,TCHAR *szTitle4)
{
    TCHAR FileName[256];
	_tcscpy(FileName,GetPathName());
	_tcscat(FileName,_T("."));
	_tcscat(FileName,EDITFMT);
	szTitle1[0]=0;
	szTitle2[0]=0;
	szTitle3[0]=0;
	szTitle4[0]=0;
    GetStyleTitle ( (LPSTR)FileName , 2 , (LPSTR)szTitle1 );
	GetStyleTitle ( (LPSTR)FileName , 3 , (LPSTR)szTitle2 );
	GetStyleTitle ( (LPSTR)FileName , 4 , (LPSTR)szTitle3 );
	GetStyleTitle ( (LPSTR)FileName , 5 , (LPSTR)szTitle4 );
}


void CEditroomDoc::OnDoSel() 
{
	// TODO: Add your command handler code here
	CString strFileCode,strDate;
	CWiresAttach attach;
	BOOL bOK=CGlobal::GetInstance()->GetFileOperMgr()->SaveWiresToLib(m_WiresParam.strWiresCode,
		//m_WiresParam.strDate,m_WiresParam.strWiresFile,strFileCode, TRUE);
		m_WiresParam.strDate,m_WiresParam.strWiresFile,strFileCode, TRUE, attach);

	if(bOK)
	{
		CGlobal *pGL = CGlobal::GetInstance();
		if(!pGL->LockFile(strFileCode))
			return;
		if(!pGL->m_FileDB.GetFileLastModTime(strDate,strFileCode))
		{
			CString strFmt,strErr;
			strFmt.LoadString(IDS_GETFILEUPDATETIMEERR);
			strErr.Format(strFmt,strFileCode);
			AfxMessageBox(strErr);
			return ;
		}
		m_FileParam.bCanEdit=TRUE;
		m_FileParam.bEdit = TRUE;
		m_FileParam.bIsLocked=TRUE;
		m_FileParam.bNewFile = FALSE;
		m_FileParam.bRecord = TRUE;
		m_FileParam.enType = NETFILE;
		m_FileParam.nDocStyle = 0;
		_tcscpy(m_FileParam.szLastModTime,strDate);
		_tcscpy(m_FileParam.szFileCode,strFileCode);
		_tcscpy(m_FileParam.szFilePath,pGL->GetTempPath()); 
		SetReadOnly(FALSE);
		CString strPath;
		strPath = m_FileParam.szFilePath;
		strPath +=_T("\\")+ strFileCode;
		SetPathName(strPath);
		m_WiresParam.enOpenType = NETFILE;
	}
}

void CEditroomDoc::OnUpdateDoSel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_WiresParam.enOpenType==WIRESFILE&&m_WiresParam.bCanSaveToLib);
}

void CEditroomDoc::OnDoSource() 
{
	// TODO: Add your command handler code here
	CGlobal::GetInstance()->ViewSource(m_FileParam.szFileCode);
}

void CEditroomDoc::OnUpdateDoSource(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FileParam.bHaveSource);
}


void CEditroomDoc::AfterSaveDocumnet()
{
	m_bPending = TRUE;
	
}

BOOL CEditroomDoc::DoCommit(BOOL &bClose)
{
	CGlobal *pGL=CGlobal::GetInstance();
	CString strFileCode=_T(""),strError, strTitle;
	CResManage setRes(g_hInst);
	if(m_FileParam.bNewFile)
	{
		if(!pGL->m_FileDB.GetNewFileCode(strFileCode))
		{
			CResManage setRes(g_hInst);
			strError.LoadString(IDS_CREATENEWFILECODEERR);
			AfxMessageBox(strError);
			return FALSE;
		}
	}
	else if(m_FileParam.bRecord)
		strFileCode=m_FileParam.szFileCode;
	else 
	{
		if(!pGL->m_FileDB.GetNewFileCode(strFileCode))
		{
			AfxMessageBox(IDS_CREATENEWFILECODEERR);
			return FALSE;
		}
	}
	
	BOOL bErr=FALSE,bInsert=TRUE;
	long lWord;//沒有真正使用,僅作為GetFileTitleWord之參數

	CString strPathName=GetPathName();
	
	int nFind=strPathName.ReverseFind('\\');
	if(nFind!=-1)
		strPathName=strPathName.Left(nFind+1);
	_tcscpy(m_FileParam.szFilePath,strPathName);
	strPathName+=strFileCode;
	SetPathName(strPathName);
	BOOL bIsModified=IsModified();


	//if (bIsModified||m_bNeedSaveDB)
		CEditProDoc::OnSaveDocument(strPathName);
	TCHAR str1[255],str2[255],str3[255],str4[255];
	GetTitles(str1,str2,str3,str4);
	
	CFileStatus rStatus;
	CFile::GetStatus(GetPathName()+_T(".")+EDITFMT,rStatus);
	
	if(!m_FileParam.bRecord)//需要插入一條新記錄
	{		
		CFileInfoDlg dlgInfo;
		dlgInfo.m_strTitle=GetFileTitleWord((LPSTR)(LPCTSTR)(strPathName+_T(".txt")),1,&lWord);
		CString strOper;
		if(m_FileParam.enType==CAPFILE)
			strOper.LoadString(IDS_ADDCAPTION);
		else
			strOper.LoadString(IDS_WRITE);
		dlgInfo.m_bNewFile = TRUE;
		dlgInfo.m_strAuthor = pGL->m_pEmployee->m_strUserName;
		dlgInfo.m_dbWords=m_lWords;
		dlgInfo.m_bModify = TRUE;

		if (m_FileParam.enType == DMFILE)
			dlgInfo.m_bExclusive = (m_DMParam.nDMFI_type > 0);
		
		//if(dlgInfo.DoModal()==IDOK)
		if((!m_CaptionParam.strSpecifyPlace.IsEmpty() && !m_CaptionParam.strSpecifyUserCode.IsEmpty())
			|| (dlgInfo.DoModal() == IDOK))
		{			
			pGL->m_FileDB.m_pDB->BeginTransaction();
			BOOL bSuccess_InsertLib = FALSE;
			if(!m_CaptionParam.strSpecifyPlace.IsEmpty() && !m_CaptionParam.strSpecifyUserCode.IsEmpty())
				bSuccess_InsertLib = pGL->m_FileDB.InsertLib(strFileCode,m_strTmpFileCode,m_lWords,dlgInfo.m_strTitle
					, m_CaptionParam.strSpecifyUserCode, m_CaptionParam.nSpecifyLib_id, m_CaptionParam.strSpecifyPlace,strOper,CString(EDITFMT));
			else
				bSuccess_InsertLib = pGL->m_FileDB.InsertLib(strFileCode,m_strTmpFileCode,m_lWords,dlgInfo.m_strTitle,pGL->m_pEmployee->m_nGroup,strOper,CString(EDITFMT),L"",L"",L"",L"");
			if(bSuccess_InsertLib)
			{
				FILEINFO FI;
				FI.nExclusive = dlgInfo.m_bExclusive;
				FI.strFilecode = strFileCode;
				FI.strPosBox =L"";
				FI.nSecret = dlgInfo.m_bSecret;
				FI.strTxtAuthor = dlgInfo.m_strAuthor.IsEmpty()?_T(" "):dlgInfo.m_strAuthor;
				FI.bLock = dlgInfo.m_bLock;
				FI.strTxtStyle = dlgInfo.m_strStyle.IsEmpty()?_T(" "):dlgInfo.m_strStyle;
				FI.strTxtTitle = dlgInfo.m_strTitle.IsEmpty()?_T(" "):dlgInfo.m_strTitle;
				FI.strUrgency = dlgInfo.m_bUrgency?'Y':'N';
				
				switch(pGL->m_FileDB.UpdateFileInfo(&FI))
				{
				case 1://保存成功
//					m_FileParam.bLocal = FALSE;
					if ((m_FileParam.enType == DMFILE ) && (m_DMParam.nDMFI_id > -1))
					{
						CString str;
						str.LoadString(IDS_LINKTODM);
						pGL->m_FileDB.LinkToDM(m_DMParam.nDMFI_id, strFileCode, str);
						// UpdateDeptFileCode @fileinfo_id int,@filecode char(255) , @oper char(19)
					}
					
					bErr = FALSE;
					break;
				case 3:
					pGL->m_FileDB.m_pDB->RollbackTransaction();
					strError.LoadString(IDS_SECRETFAIL);
					AfxMessageBox(strError);
					bErr = TRUE;
					break;
				case 4:
					pGL->m_FileDB.m_pDB->RollbackTransaction();
					strError.LoadString(IDS_LOCKFAIL);
					AfxMessageBox(strError);
					bErr = TRUE;
					break;
				}
					
				if(!bErr&&!pGL->m_FileDB.UpdateText(strPathName,EDITFMT,strFileCode))
				{
					bErr=TRUE;
					pGL->m_FileDB.m_pDB->RollbackTransaction();
					AfxMessageBox(L"UpdateText error!");
				}
				if(!bErr&&!pGL->m_FileDB.UpdateTitles(strFileCode,str1,str2,str3,str4,rStatus.m_mtime.Format(DATETIMEFORMAT)))
				{
					bErr=TRUE;
					pGL->m_FileDB.m_pDB->RollbackTransaction();
					AfxMessageBox(L"UpdateTitles error!");
				}
				if(!bErr&&m_FileParam.enType ==CAPFILE)
				{
					if(!pGL->m_FileDB.InsertPhotoRecord(&m_CaptionParam,L"新增圖說"))
					{
						bErr=TRUE;
						pGL->m_FileDB.m_pDB->RollbackTransaction();
						AfxMessageBox(L"InsertPhotoRecord error!");
					}
					
					BOOL bRelationDepart = TRUE;
					if(!m_CaptionParam.bOnCol)
					{
						int iPhoto_id = -1;
						int iCol_id = -1;
						COleDateTime odtColDate;
						if(pGL->m_FileDB.GetLastOnColPhoto(m_CaptionParam.strPicID, iPhoto_id, iCol_id, odtColDate))
						{
							CCaptionParam capParam;
							capParam.bOnCol = TRUE;
							capParam.enOpenType = m_CaptionParam.enOpenType;
							capParam.nColID = iCol_id;
							capParam.strCaption = m_CaptionParam.strCaption;
							capParam.strColDate = odtColDate.Format(DATEFORMAT);
							capParam.strPicID.Format(_T("%d"), iPhoto_id);
							capParam.strPicName = m_CaptionParam.strPicName;
							capParam.strPictureFile = m_CaptionParam.strPictureFile;
							if(!pGL->m_FileDB.InsertPhotoRecord(&capParam, L"新增圖說"))
							{
								bErr = TRUE;
								pGL->m_FileDB.m_pDB->RollbackTransaction();
								AfxMessageBox(L"InsertPhotoRecord error!");
							}
							
							if(!bErr)
							{
								int nResult = pGL->m_FileDB.UpdateFilePhotoRelation(strFileCode,capParam.strPicID,capParam.bOnCol,0,1,0);
								if((nResult == 1) || (nResult == 2))
								{
									if(nResult == 1)
									{
										AfxMessageBox(IDS_PIC_FILE_HAVERELATION);
									}
									else if(nResult == 2)
									{
										AfxMessageBox(IDS_PICHAVERELATION);
									}
									//bErr = TRUE;
									//pGL->m_FileDB.m_pDB->RollbackTransaction();
								}
							}

							bRelationDepart = FALSE;
						}
					}

					if(bRelationDepart && !bErr)
					{
						int nResult = pGL->m_FileDB.UpdateFilePhotoRelation(strFileCode,m_CaptionParam.strPicID,m_CaptionParam.bOnCol,0,1,0);
						if((nResult == 1) || (nResult == 2))
						{
							if(nResult == 1)
							{
								AfxMessageBox(IDS_PIC_FILE_HAVERELATION);
							}
							else if(nResult == 2)
							{
								AfxMessageBox(IDS_PICHAVERELATION);
							}

							//bErr=TRUE;
							//pGL->m_FileDB.m_pDB->RollbackTransaction();
						}
					}
				}
				if(!bErr)
				{
					pGL->m_FileDB.m_pDB->CommitTransaction();
					m_FileParam.bRecord = TRUE;
				}
				bClose=TRUE;
			}
			else
			{
				bErr = TRUE;
				pGL->m_FileDB.m_pDB->RollbackTransaction();
				AfxMessageBox(L"Insert lib error!");
			}
		}
		else
		{
			
			_tcscpy(m_FileParam.szFileCode,strFileCode);
			bClose=FALSE;
		}
	}
	else
	{
		//if(bIsModified)
		if(m_bPending)
		{
			CString strOper;
			CResManage res(g_hInst);
			strOper.LoadString(IDS_MODIFY);
			strTitle=GetFileTitleWord((LPSTR)(LPCTSTR)(strPathName+_T(".txt")),1,&lWord);
			pGL->m_FileDB.m_pDB->BeginTransaction();

			wchar_t* wc_tmp={0};
			lstrcpy(wc_tmp,strTitle) ; 
			int RetCode;
			if(!bErr&&!pGL->m_FileDB.UpdateLib(strFileCode,wc_tmp,TRUE,m_lWords,pGL->m_pEmployee->m_nGroup,strOper,m_FileParam.szLastModTime, L"", L"", L"", false, L"", L"",L"",RetCode))
			{
				bErr=TRUE;
				pGL->m_FileDB.m_pDB->RollbackTransaction();
			}

			if(!bErr&&!pGL->m_FileDB.UpdateText(strPathName,EDITFMT,strFileCode))
			{
				bErr=TRUE;
				pGL->m_FileDB.m_pDB->RollbackTransaction();
			}
			if(!bErr&&!pGL->m_FileDB.UpdateTitles(strFileCode,str1,str2,str3,str4,rStatus.m_mtime.Format(DATETIMEFORMAT)))
			{
				bErr=TRUE;
				pGL->m_FileDB.m_pDB->RollbackTransaction();
			}
			if(!bErr)
			{
				pGL->m_FileDB.m_pDB->CommitTransaction();
				CString strDate;
				if(pGL->m_FileDB.GetFileLastModTime(strDate,(CString)m_FileParam.szFileCode))
					_tcscpy(m_FileParam.szLastModTime,strDate);
			}
			bClose=TRUE;
		}
	}
	m_strDisplayTitle=strTitle;
	if(!bErr&&bClose)
	{
		if(m_FileParam.bNewFile)
		{
			_tcscpy(m_FileParam.szFileCode,strFileCode);
			m_FileParam.bNewFile = FALSE;
			m_FileParam.bRecord = TRUE;
		}
		m_bHasCommit = TRUE;
	}
	SaveBkFile();
	
	if (bErr)
	{
		AfxMessageBox(IDS_FILECHANGED);
		m_FileParam.bCanEdit = FALSE;
		m_FileParam.bEdit = FALSE;
		m_FileParam.bSendCol = FALSE;
		m_FileParam.bSign = FALSE;
		m_FileParam.bSignDirect = FALSE;

	}

	return !bErr;
}

void CEditroomDoc::OnFileSave() 
{
	if(m_FileParam.bNewFile)
	{
		m_bNeedSaveDB=IsModified();
		SaveBkFile();
		CEditProDoc::OnSaveDocument(GetPathName());
	}
	else
	{
		BOOL bClose;
		DoCommit(bClose);
	}
}

BOOL CEditroomDoc::SetBkEnv(EDITFILEBAK *pEditBak)
{
	memcpy(&m_FileParam,&pEditBak->FileParm,sizeof(FILEPARAM));
	m_WiresParam=pEditBak->WiresParm;
	m_bHasCommit = pEditBak->bHasCommit;
	m_bNeedSaveDB = pEditBak->bNeedSaveDB;
	m_bPending = pEditBak->bPending;
	m_strDisplayTitle = pEditBak->szDisplayTitle;
	m_strTmpFileCode = pEditBak->szTmpFileCode;
	CGlobal *pGL=CGlobal::GetInstance();
	if(!m_FileParam.bNewFile&&(m_FileParam.bEdit)&&
		(!pGL->CanFileModify(&m_FileParam)||!pGL->LockFile(m_FileParam.szFileCode)))
	{
		m_FileParam.bCanEdit = FALSE;
		m_FileParam.bEdit=FALSE;
		
	}
	SetReadOnly(!m_FileParam.bEdit);
	return TRUE;
}

BOOL CEditroomDoc::SaveBkFile()
{
	if(m_FileParam.enType==WIRESFILE)return FALSE;
	EDITFILEBAK bf;
	
	bf.cbSize = sizeof(bf);
	memcpy(&bf.FileParm,&m_FileParam,sizeof(FILEPARAM));
	bf.WiresParm=m_WiresParam;
	bf.bHasCommit=m_bHasCommit;
	bf.bNeedSaveDB=m_bNeedSaveDB;
	bf.bPending = m_bPending;
	_tcscpy(bf.szDisplayTitle,m_strDisplayTitle.Left(253));
	_tcscpy(bf.szTmpFileCode,m_strTmpFileCode.Left(253));
	CString strPathName;
	strPathName = GetPathName();
	if(strPathName.IsEmpty())
		return FALSE;
	if(_taccess(strPathName,00)==0)
		_tcscpy(bf.szPathFile,strPathName);
	else _tcscpy(bf.szPathFile,strPathName+_T(".")+EDITFMT);
	CString strPath,strFileCode;
	strPath=bf.szPathFile;
	if(m_FileParam.enType==NEWFILE)
		strFileCode=m_strTmpFileCode;
	else
		strFileCode=m_FileParam.szFileCode;
	m_strBakFile = CGlobal::GetInstance()->GetEDPAPath()+_T("\\")+strFileCode+_T(".")+EDITBAK;
	CFile bakFile;
	TRY
	{
		CFile::Remove(m_strBakFile);
	}
	CATCH(CFileException ,e)
		;
	END_CATCH
	if(bakFile.Open(m_strBakFile,CFile::modeCreate | CFile::modeWrite )==FALSE) 
		return FALSE;
	bakFile.Write(&bf,sizeof(bf));
	bakFile.Flush(); 
	bakFile.Close();
	return TRUE;
}


void CEditroomDoc::BeforeAutosave()
{
	m_bNeedSaveDB=IsModified();
    SaveBkFile();
    CEditProDoc::OnSaveDocument(GetPathName());

}

void CEditroomDoc::OnDoSign() 
{
	// TODO: Add your command handler code here
	CString strMsg;
	strMsg.LoadString(IDS_SIGNQUESTION);
	if(AfxMessageBox(strMsg,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1)==IDNO)
		return;
	BOOL bQuit=FALSE;
	BOOL bRet=TRUE;
	if(m_FileParam.bEdit&&(IsModified()||m_bPending))
	{
		bRet=DoCommit(bQuit);
		bRet = bRet&&bQuit;
	}
	if(bRet)
	{
		if (CGlobal::GetInstance()->SignFile(m_FileParam))
		{
			m_bTransfer = FALSE;
			OnCloseDocument();
		}
	}
}

void CEditroomDoc::OnUpdateDoSign(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FileParam.bSign);
}

void CEditroomDoc::OnDoSigndirect() 
{
	// TODO: Add your command handler code here
	BOOL bQuit=FALSE;
	BOOL bRet=TRUE;
	if(m_FileParam.bEdit&&(IsModified()||m_bPending))
	{
		bRet=DoCommit(bQuit);
		bRet = bRet&&bQuit;
	}
	if(bRet)
	{
		if (CGlobal::GetInstance()->SignFileDirect(m_FileParam))
		{
			m_bTransfer = FALSE;
			OnCloseDocument();
		}
	}
}

void CEditroomDoc::OnUpdateDoSigndirect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FileParam.bSignDirect);
}

void CEditroomDoc::OnRdoRecord() 
{
	// TODO: Add your command handler code here
	CGlobal::GetInstance()->ShowFileRecord(m_FileParam.szFileCode,m_FileParam.szTitle);
}

void CEditroomDoc::OnUpdateRdoRecord(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_FileParam.bNewFile);
}

void CEditroomDoc::OnRdoComment() 
{
	// TODO: Add your command handler code here
	CGlobal::GetInstance()->ShowFileInfo(&m_FileParam);
}

void CEditroomDoc::OnUpdateRdoComment(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_FileParam.bNewFile);
}

void CEditroomDoc::OnRdoCopy() 
{
	// TODO: Add your command handler code here
	CGlobal::GetInstance()->ShowCopyInfo(m_FileParam.szFileCode);
}

void CEditroomDoc::OnUpdateRdoCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FileParam.bCopy&&!m_FileParam.bNewFile);
}

void CEditroomDoc::OnDoSendcol() 
{
	// TODO: Add your command handler code here
	BOOL bQuit=FALSE;
	BOOL bRet=TRUE;
	if(m_FileParam.bEdit&&(IsModified()||m_bPending))
	{
		bRet=DoCommit(bQuit);
		bRet = bRet&&bQuit;
	}
	if(bRet)
	{
		if (CGlobal::GetInstance()->SendToCol(&m_FileParam))
		{
			m_bTransfer = FALSE;
			OnCloseDocument();
		}
	}
}

void CEditroomDoc::OnUpdateDoSendcol(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FileParam.bSendCol&&!m_FileParam.bNewFile);
}

void CEditroomDoc::OnDoBackfile() 
{
	// TODO: Add your command handler code here
	CString strMsg;
	strMsg.LoadString(IDS_BACKQUESTION);
	if(AfxMessageBox(strMsg,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1)==IDNO)
		return;
	
	if(CGlobal::GetInstance()->RollbackFile(m_FileParam,strMsg))
	{
		AfxMessageBox(strMsg);	
		m_bTransfer = FALSE;
		OnCloseDocument();
	}
}

void CEditroomDoc::OnUpdateDoBackfile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FileParam.bBack&&!m_FileParam.bNewFile);
}


BOOL CEditroomDoc::UnLockFile()
{
	CGlobal *pGL = CGlobal::GetInstance();
	if(m_FileParam.bEdit)
	{
		BOOL bRet=pGL->UnlockFile(m_FileParam.szFileCode);
		CString strLastModTime;
		if(pGL->m_FileDB.GetFileLastModTime(strLastModTime,m_FileParam.szFileCode))
			_tcscpy(m_FileParam.szLastModTime,strLastModTime);
		return bRet;
	}
	return FALSE;
}
