// FileManage.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "filemanage.h"
#include "dofiledoc.h"
#include "dofilefrm.h"
#include "dofileview.h"
#include "filedb.h"
#include "docoldoc.h"
#include "docolfrm.h"
#include "docolview.h"
#include "filemanage.h"
#include "ClearFile.h"
#include "recycle.h"
#include "replaceview.h"
#include "childrepfrm.h"

#include "editdoc.h"
#include "editframenew.h"
#include "editview_bottom.h"
#include "editview_top.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
HINSTANCE g_hInst=NULL;
HACCEL g_hAccel = NULL;

static AFX_EXTENSION_MODULE FileManageDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("FILEMANAGE.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(FileManageDLL, hInstance))
			return 0;
		g_hInst = hInstance;
		g_hAccel = LoadAccelerators(g_hInst,MAKEINTRESOURCE(IDR_ACCEL));
		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(FileManageDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("FILEMANAGE.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(FileManageDLL);
		g_hInst = NULL;
	}


	return 1;   // ok
}
extern "C" AFX_EXT_API void WINAPI InitDLL(CADODatabase * const pDB,const CShareData *pData)
{
	CGlobal *pGlobal=CGlobal::GetInstance();
	pGlobal->m_FileDB.SetDB(pDB);
	pGlobal->m_pEmployee = &pData->m_Employee;
	pGlobal->m_pEntry = &pData->m_Entry;
	pGlobal->m_pShareData = pData;
	AfxInitRichEdit2();
	/*
	 * Add by Bob 2005-9-6
		Set the default font and toolbar style
	 */
	/*
	 *	Add by Bob 2006-4-19
		Now, use 'UseDefFontEditFile' to set the Editpro style
		1: use Mingpao's setting, Font is 方正新秀麗, size =15
	 */
	CString strUseDefFontEditFile="";
	pGlobal->m_FileDB.GetConfig(_T("UseDefFontEditFile"),strUseDefFontEditFile);
	if (strUseDefFontEditFile == _T("1"))
		SetSpecifyFontAndToolbar(1);
	else
		SetSpecifyFontAndToolbar(0);
	// End add by Bob

	// 將EditPro初施化部份移後,先完成上面的設置
	InitEditProDLL();
	SetEditProUser((LPSTR)(LPCTSTR)pGlobal->m_pEmployee->m_strUserName);

	CString strCountType;
	if (pGlobal->m_FileDB.GetConfig(_T("banxing"), strCountType))
	{
		if (strCountType == _T("F"))
			SetWordCountType(0);
	}
	//edit by leon 2012-12-19 for crystal report issue
	//create log in sql for crystal report dll
	CString strSqlLogin;
	strSqlLogin.Format(L"Server=%s;Database=%s;UID=%s;PWD=%s",pGlobal->m_pEmployee->m_strServerName,
		pGlobal->m_pEmployee->m_strDatabaseName,pGlobal->m_pEmployee->m_strUserCode,pGlobal->m_pEmployee->m_strPassword);
	pGlobal->m_ReportPara.strSqlLogin=strSqlLogin;
	pGlobal->m_ReportPara.strServer=pGlobal->m_pEmployee->m_strServerName;
	pGlobal->m_ReportPara.strDatabase=pGlobal->m_pEmployee->m_strDatabaseName;
	pGlobal->m_ReportPara.strUser=pGlobal->m_pEmployee->m_strUserCode;
	pGlobal->m_ReportPara.strPassword=pGlobal->m_pEmployee->m_strPassword;
	//end leon 2012-12-19
	pGlobal->UnlockAllFile();
}

extern "C" AFX_EXT_API CDocument * WINAPI OpenDocument(LPTSTR lpFileName,void * pParam)
{
	
	EN_OPENFILETYPE enType;
	enType = *(EN_OPENFILETYPE*)pParam;
	CEditroomDoc *pDoc=NULL;
	editDoc *pDocNew=NULL;	

	
	CGlobal *pGlobal=CGlobal::GetInstance();
	TCHAR tcFileCode[30];
	FILEPARAM FileParam;
	BOOL bNew=FALSE;
	memset(&FileParam,0,sizeof(FILEPARAM));
	memset(tcFileCode,0,sizeof(TCHAR)*30);
	CResManage res(g_hInst);
	pGlobal->m_enOpenFileType = enType;
	
	// Before Open
	if((enType==LOCALFILE||enType==PUBLISHTEXT)&&lpFileName)
	{
		TCHAR *pszDest=_tcsrchr(lpFileName,_T('\\'));
		TCHAR *pszExt =_tcsrchr(lpFileName,_T('.')); 
		if(pszDest&&pszExt&&pszExt>pszDest)
		{
			_tcsncpy(tcFileCode,pszDest+1,pszExt-pszDest-1);
			CFileStatus rStatus;
			CFile::GetStatus(lpFileName,rStatus);
			CString s=rStatus.m_mtime.Format(_T("%Y-%m-%d %H:%M:%S"));
//			FileParam.enType = LOCALFILE;
			FileParam.enType = enType;
			if(pGlobal->CheckFile((CString)tcFileCode,s,FileParam))
			{
				if(!pGlobal->LockFile(tcFileCode))
					return NULL;
			}
			else
				bNew=TRUE;
			FileParam.bEdit = TRUE;
		}
	}
	else if (enType==NEWFILE||enType==CAPFILE)
		bNew = TRUE;
	else if (enType == NETFILE || enType == REPLACEFILE)
	{
		FILEPARAM *pFileParam = (FILEPARAM *)pParam;
		if(pFileParam->bEdit)
		{
			if(!pGlobal->LockFile(pFileParam->szFileCode))
			{
				AfxMessageBox(IDS_LOCKFILEFAIL);
				return NULL;
			}
			if(!pGlobal->CanFileModify(pFileParam))
			{
				pGlobal->UnlockFile(pFileParam->szFileCode);
				return NULL;
			}
		}
	}
	pGlobal->m_enOpenFileType = enType;
	// open Document
	if(enType==REPLACEFILE)
	{
		
		if(pGlobal->m_pDocTempRep==NULL)
		{
			pGlobal->m_pDocTempRep=new CMultiDocTemplate(
				IDR_MAINFRAME,
				RUNTIME_CLASS(CEditroomDoc),
				RUNTIME_CLASS(CChildRepFrm), 
				RUNTIME_CLASS(CReplaceView));
			AfxGetApp()->AddDocTemplate(pGlobal->m_pDocTempRep);
		}
		ASSERT(pGlobal->m_pDocTempRep!=NULL);
		pDoc=(CEditroomDoc*)pGlobal->m_pDocTempRep->OpenDocumentFile(lpFileName);
	}
	else
	{
		pGlobal->m_pDocTemplate = NULL;
      	pGlobal->m_pDocTemplate=new CMultiDocTemplate(
			IDR_MAINFRAME,
			RUNTIME_CLASS(CEditroomDoc),
			RUNTIME_CLASS(CEditFrame), 
			RUNTIME_CLASS(CEditroomView));
		AfxGetApp()->AddDocTemplate(pGlobal->m_pDocTemplate);
		if(enType==CAPFILE)
		{
			CCaptionParam *captionParam = (CCaptionParam*)pParam;
			pGlobal->m_strCaptionPicFile = captionParam->strPictureFile;
		}
		else if(enType == NETFILE)
		{
			FILEPARAM *fileParam = (FILEPARAM*)pParam;
			pGlobal->GetRelPicFile(fileParam->szFileCode);
		}
		ASSERT(pGlobal->m_pDocTemplate!=NULL);
		pDoc=(CEditroomDoc*)pGlobal->m_pDocTemplate->OpenDocumentFile(lpFileName);
	}

	// After Open
	pDoc->m_FileParam.bNewFile = bNew;	
	pDoc->m_FileParam.enType = enType;


	if(enType==NEWFILE)
	{
		pDoc->m_FileParam.bCanEdit = TRUE;
		pDoc->m_FileParam.bEdit = TRUE;
	}
	if(enType==LOCALFILE||enType==PUBLISHTEXT)
	{
		pDoc->m_FileParam.bCanEdit = FileParam.bCanEdit;
		pDoc->m_FileParam.bRecord = FileParam.bRecord;
		pDoc->m_FileParam.bEdit =pDoc->m_FileParam.bIsLocked= FileParam.bEdit;	
		_tcscpy(pDoc->m_FileParam.szFileCode,tcFileCode);
	}
	if(enType==WIRESFILE)
	{
		
		pDoc->m_FileParam.bCanEdit = FALSE;
		pDoc->m_FileParam.bEdit = FALSE;
		pDoc->m_WiresParam = *(CWiresParam*)pParam;
		
	}
	if(enType==DMFILE)
	{
		pDoc->m_FileParam.bCanEdit = TRUE;
		pDoc->m_FileParam.bEdit = TRUE;
		if(_tcslen(pDoc->m_FileParam.szFileCode)==0)
			pDoc->m_FileParam.bNewFile = TRUE;
		pDoc->m_DMParam = *(CDMParam*)pParam;
		//AfxMessageBox(pDoc->m_FileParam.szFileCode);
		/*
		 FILEPARAM *pFileParam = (FILEPARAM *)pParam;
		
		_tcscpy(pDoc->m_FileParam.szLastModTime,pFileParam->szLastModTime);
		_tcscpy(pDoc->m_FileParam.szFileCode,pFileParam->szFileCode);
		pDoc->m_FileParam.bHaveSource = FALSE;
		pDoc->m_FileParam.nDocStyle = pFileParam->nDocStyle;
		pDoc->m_FileParam.bLib = pFileParam->bLib;
		pDoc->m_FileParam.nPosID=pFileParam->nPosID;
		_tcscpy(pDoc->m_FileParam.szParam,pFileParam->szParam);
		*/
		
		pDoc->SaveBkFile();

		
	}
	if(enType==CAPFILE)
	{
		pDoc->m_FileParam.bCanEdit = TRUE;
		pDoc->m_FileParam.bEdit = TRUE;
		CString str;
		CResManage myRes(g_hInst);
		str.Format(IDS_ADDCAPTIONMARK, ((CCaptionParam*)pParam)->strPicName);
		pDoc->m_CaptionParam = *(CCaptionParam*)pParam;
		str+=_T("\x0D\x1")+pDoc->m_CaptionParam.strCaption;

		//20081104 Liubc
		if(!pDoc->m_CaptionParam.strSpecifyUserCode.IsEmpty()
			&& !pDoc->m_CaptionParam.strSpecifyPlace.IsEmpty())
		{
			CString strTemp;
			CString strUserName;
			pGlobal->m_FileDB.GetUserName(pDoc->m_CaptionParam.strSpecifyUserCode, strUserName);
			strTemp.Format(_T("給%s "), strUserName);
			str = strTemp + str;
			pDoc->m_CaptionParam.strCaption = str;
			pDoc->m_CaptionParam.isSimpCap = TRUE;
		}

		pDoc->InsertText(str);
		pDoc->SaveBkFile();
		
		//20081104 Liubc
		if(!pDoc->m_CaptionParam.strSpecifyUserCode.IsEmpty()
			&& !pDoc->m_CaptionParam.strSpecifyPlace.IsEmpty())
		{
			BOOL bQuit = FALSE;
			pDoc->DoCommit(bQuit);

			pDoc->m_bTransfer = FALSE;
			pDoc->OnCloseDocument();
		}
	}
	if (enType == NETFILE || enType == REPLACEFILE)
	{
		FILEPARAM *pFileParam = (FILEPARAM *)pParam;

		pDoc->m_FileParam.bNewFile = pFileParam->bNewFile;
		pDoc->m_FileParam.bRecord = pFileParam->bRecord;
		pDoc->m_FileParam.bCanEdit = pFileParam->bCanEdit;
		pDoc->m_FileParam.bEdit = pDoc->m_FileParam.bIsLocked = pFileParam->bEdit;
		_tcscpy(pDoc->m_FileParam.szLastModTime,pFileParam->szLastModTime);
		_tcscpy(pDoc->m_FileParam.szFileCode,pFileParam->szFileCode);
		pDoc->m_FileParam.bHaveSource = pGlobal->m_FileDB.FileHaveSource(pFileParam->szFileCode);
		pDoc->m_FileParam.nDocStyle = pFileParam->nDocStyle;
		pDoc->m_FileParam.bLib = pFileParam->bLib;
		pDoc->m_FileParam.nColID=pFileParam->nColID;
		pDoc->m_FileParam.nLibID=pFileParam->nLibID;
		pDoc->m_FileParam.nState=pFileParam->nState;
		pDoc->m_FileParam.bBack = pFileParam->bBack;
		pDoc->m_FileParam.bSendCol = pFileParam->bSendCol;
		pDoc->m_FileParam.bSign = pFileParam->bSign;
		pDoc->m_FileParam.bSignDirect = pFileParam->bSignDirect;
		pDoc->m_FileParam.bCopy = pGlobal->m_FileDB.IsCopyFile(pFileParam->szFileCode);
		pDoc->m_FileParam.bIsExclusive = pFileParam->bIsExclusive;
		_tcscpy(pDoc->m_FileParam.szTitle,pFileParam->szTitle);
		_tcscpy(pDoc->m_FileParam.szAssignedUser,pFileParam->szAssignedUser);
		_tcscpy(pDoc->m_FileParam.szParam,pFileParam->szParam);
		pDoc->SaveBkFile();
		
	}
	pDoc->m_bCreate=pDoc->m_FileParam.bNewFile;
	pDoc->SetReadOnly(!pDoc->m_FileParam.bEdit);
	return pDoc;
}

extern "C" AFX_EXT_API void WINAPI TestExport()
{
//	AfxMessageBox(L"Test");
}

extern "C" AFX_EXT_API CDocument * WINAPI OpenDocumentNew(LPTSTR lpFileName,void * pParam)
{
	EN_OPENFILETYPE enType;
	enType = *(EN_OPENFILETYPE*)pParam;
	editDoc *pDoc=NULL;
//	editDoc *pDocNew=NULL;	

	
	CGlobal *pGlobal=CGlobal::GetInstance();
	pGlobal->m_bOldVersion = FALSE;
	TCHAR tcFileCode[30];
	FILEPARAM FileParam;
	BOOL bNew=FALSE;
	memset(&FileParam,0,sizeof(FILEPARAM));
	memset(tcFileCode,0,sizeof(TCHAR)*30);
	CResManage res(g_hInst);
	pGlobal->m_enOpenFileType = enType;
	
	bool b_IsLocked = false;
	CString strLockUser = L"";

	// Before Open
	if((enType==LOCALFILE||enType==PUBLISHTEXT)&&lpFileName)
	{
		TCHAR *pszDest=_tcsrchr(lpFileName,_T('\\'));
		TCHAR *pszExt =_tcsrchr(lpFileName,_T('.')); 
		if(pszDest&&pszExt&&pszExt>pszDest)
		{
			_tcsncpy(tcFileCode,pszDest+1,pszExt-pszDest-1);
			CFileStatus rStatus;
			CFile::GetStatus(lpFileName,rStatus);
			CString s=rStatus.m_mtime.Format(_T("%Y-%m-%d %H:%M:%S"));
//			FileParam.enType = LOCALFILE;
			FileParam.enType = enType;
			if(pGlobal->CheckFile((CString)tcFileCode,s,FileParam))
			{
				AfxMessageBox(L"閣下開啟的舊檔已存在於資料庫中 。本次操作無效。");
				//if(!pGlobal->LockFile(tcFileCode))
					return NULL;
			}
			else
				bNew=TRUE;
			FileParam.bEdit = TRUE;
		}
	}
	else if (enType==NEWFILE||enType==CAPFILE)
		bNew = TRUE;
	else if (enType == NETFILE || enType == REPLACEFILE)
	{
		FILEPARAM *pFileParam = (FILEPARAM *)pParam;
		b_IsLocked = pGlobal->m_FileDB.IsFileLocked(pFileParam->szFileCode, strLockUser);
		if(pFileParam->bEdit)
		{
			if(!pGlobal->LockFile(pFileParam->szFileCode))
			{
				AfxMessageBox(IDS_LOCKFILEFAIL);
				return NULL;
			}
			if(!pGlobal->CanFileModify(pFileParam))
			{
				pGlobal->UnlockFile(pFileParam->szFileCode);
				return NULL;
			}
		}
	}

	pGlobal->m_enOpenFileType = enType;
	// open Document
	if(enType==REPLACEFILE)
	{
		
		if(pGlobal->m_pDocTempRep==NULL)
		{
			pGlobal->m_pDocTempRep=new CMultiDocTemplate(
				IDR_MAINFRAME,
				RUNTIME_CLASS(editDoc),
				RUNTIME_CLASS(editFrameNew), 
				RUNTIME_CLASS(editView_Bottom));
			AfxGetApp()->AddDocTemplate(pGlobal->m_pDocTempRep);
		}
		ASSERT(pGlobal->m_pDocTempRep!=NULL);
		pDoc=(editDoc*)pGlobal->m_pDocTempRep->OpenDocumentFile(lpFileName);
	}
	else
	{
		pGlobal->m_pDocTemplate = NULL;
		pGlobal->m_pDocTemplate=new CMultiDocTemplate(
			IDR_MAINFRAME,
//			RUNTIME_CLASS(CEditroomDoc),
//			RUNTIME_CLASS(CEditFrame), 
//			RUNTIME_CLASS(CEditroomView));
			RUNTIME_CLASS(editDoc),
			RUNTIME_CLASS(editFrameNew), 
			RUNTIME_CLASS(editView_Bottom));
		AfxGetApp()->AddDocTemplate(pGlobal->m_pDocTemplate);
		if(enType==CAPFILE)
		{
			CCaptionParam *captionParam = (CCaptionParam*)pParam;
			pGlobal->m_strCaptionPicFile = captionParam->strPictureFile;
		}
		else if(enType == NETFILE)
		{
			FILEPARAM *fileParam = (FILEPARAM*)pParam;
			pGlobal->GetRelPicFile(fileParam->szFileCode);
		}
		else if(enType == VERSIONFILE)
		{
			pGlobal->m_bOldVersion = TRUE;
			lpFileName = NULL;
		}
		ASSERT(pGlobal->m_pDocTemplate!=NULL);
//		pDoc=(CEditroomDoc*)pGlobal->m_pDocTemplate->OpenDocumentFile(lpFileName);

//		AfxMessageBox(lpFileName);
		

		pDoc=(editDoc*)pGlobal->m_pDocTemplate->OpenDocumentFile(lpFileName);

//			return pDocNew;
	}

	if (!pDoc)
		return NULL;		


	// After Open
	pDoc->m_FileParam.bNewFile = bNew;	
	pDoc->m_FileParam.enType = enType;


	if(enType==NEWFILE)
	{
		pDoc->m_FileParam.bCanEdit = TRUE;
		pDoc->m_FileParam.bEdit = TRUE;

		pDoc->m_FileParam.bNewFile = TRUE;
	}
	if(enType==LOCALFILE||enType==PUBLISHTEXT)
	{
		pDoc->m_FileParam.bCanEdit = FileParam.bCanEdit;
		pDoc->m_FileParam.bRecord = FileParam.bRecord;
		pDoc->m_FileParam.bEdit =pDoc->m_FileParam.bIsLocked= FileParam.bEdit;	
		_tcscpy(pDoc->m_FileParam.szFileCode,tcFileCode);


//		pDoc->m_strVersion = L"Test";	
//		pDoc->m_strVersion_Mark = L"@@@@";	

	}
	if(enType==WIRESFILE)
	{
		
		//pDoc->m_FileParam.bCanEdit = FALSE;
		pDoc->m_FileParam.bCanEdit = TRUE;
		pDoc->m_FileParam.bEdit = FALSE;
		pDoc->m_WiresParam = *(CWiresParam*)pParam;
		
	}
	if(enType==DMFILE)
	{
		pDoc->m_FileParam.bCanEdit = TRUE;
		pDoc->m_FileParam.bEdit = TRUE;
		if(_tcslen(pDoc->m_FileParam.szFileCode)==0)
			pDoc->m_FileParam.bNewFile = TRUE;
		pDoc->m_DMParam = *(CDMParam*)pParam;
		//AfxMessageBox(pDoc->m_FileParam.szFileCode);
		/*
		 FILEPARAM *pFileParam = (FILEPARAM *)pParam;
		
		_tcscpy(pDoc->m_FileParam.szLastModTime,pFileParam->szLastModTime);
		_tcscpy(pDoc->m_FileParam.szFileCode,pFileParam->szFileCode);
		pDoc->m_FileParam.bHaveSource = FALSE;
		pDoc->m_FileParam.nDocStyle = pFileParam->nDocStyle;
		pDoc->m_FileParam.bLib = pFileParam->bLib;
		pDoc->m_FileParam.nPosID=pFileParam->nPosID;
		_tcscpy(pDoc->m_FileParam.szParam,pFileParam->szParam);
		*/
		
//		pDoc->SaveBkFile();

		
	}
	if(enType==CAPFILE)
	{
		pDoc->m_FileParam.bCanEdit = TRUE;
		pDoc->m_FileParam.bEdit = TRUE;
		CString str;
		CResManage myRes(g_hInst);
		str.Format(IDS_ADDCAPTIONMARK, ((CCaptionParam*)pParam)->strPicName);
		pDoc->m_CaptionParam = *(CCaptionParam*)pParam;
		str+=_T("\x0D\x1")+pDoc->m_CaptionParam.strCaption;

		//20081104 Liubc
		if(!pDoc->m_CaptionParam.strSpecifyUserCode.IsEmpty()
			&& !pDoc->m_CaptionParam.strSpecifyPlace.IsEmpty())
		{
			CString strTemp;
			CString strUserName;
			pGlobal->m_FileDB.GetUserName(pDoc->m_CaptionParam.strSpecifyUserCode, strUserName);
			strTemp.Format(_T("給%s "), strUserName);
			str = strTemp + str;
			pDoc->m_CaptionParam.strCaption = str;
			pDoc->m_CaptionParam.isSimpCap = TRUE;
		}

//		pDoc->InsertText(str);
//		pDoc->SaveBkFile();
		
		//20081104 Liubc
		if(!pDoc->m_CaptionParam.strSpecifyUserCode.IsEmpty()
			&& !pDoc->m_CaptionParam.strSpecifyPlace.IsEmpty())
		{
			BOOL bQuit = FALSE;
			pDoc->DoCommit(bQuit);

			pDoc->m_bTransfer = FALSE;
			pDoc->OnCloseDocument();
		}
	}
	if (enType == NETFILE || enType == REPLACEFILE || enType == VERSIONFILE)
	{
		FILEPARAM *pFileParam = (FILEPARAM *)pParam;

		pDoc->m_FileParam.bNewFile = pFileParam->bNewFile;
//		pDoc->m_FileParam.bRecord = pFileParam->bRecord;
		pDoc->m_FileParam.bCanEdit = pFileParam->bCanEdit;
	//	pDoc->m_FileParam.bCanEdit = FALSE;
		pDoc->m_FileParam.bEdit = pDoc->m_FileParam.bIsLocked = pFileParam->bEdit;
		pDoc->m_FileParam.b_AutoSkipLine = pFileParam->b_AutoSkipLine;
		_tcscpy(pDoc->m_FileParam.szLastModTime,pFileParam->szLastModTime);
		if (enType != VERSIONFILE)
		{
			pDoc->m_FileParam.bRecord = pFileParam->bRecord;
			_tcscpy(pDoc->m_FileParam.szFileCode,pFileParam->szFileCode);
		}
		pDoc->m_FileParam.bHaveSource = pGlobal->m_FileDB.FileHaveSource(pFileParam->szFileCode);
		pDoc->m_FileParam.nDocStyle = pFileParam->nDocStyle;
		pDoc->m_FileParam.bLib = pFileParam->bLib;
		pDoc->m_FileParam.nColID=pFileParam->nColID;
		pDoc->m_FileParam.nLibID=pFileParam->nLibID;
		pDoc->m_FileParam.nState=pFileParam->nState;
		pDoc->m_FileParam.bBack = pFileParam->bBack;
		pDoc->m_FileParam.bSendCol = pFileParam->bSendCol;
		pDoc->m_FileParam.bSign = pFileParam->bSign;
		pDoc->m_FileParam.bSignDirect = pFileParam->bSignDirect;
		pDoc->m_FileParam.bCopy = pGlobal->m_FileDB.IsCopyFile(pFileParam->szFileCode);
		pDoc->m_FileParam.bIsExclusive = pFileParam->bIsExclusive;
		_tcscpy(pDoc->m_FileParam.szTitle,pFileParam->szTitle);
		_tcscpy(pDoc->m_FileParam.szAssignedUser,pFileParam->szAssignedUser);
		_tcscpy(pDoc->m_FileParam.szParam,pFileParam->szParam);
//		pDoc->SaveBkFile();
		
	}
	pDoc->SetReadOnly(); 
	pDoc->m_bCreate=pDoc->m_FileParam.bNewFile;
//	pDoc->SetReadOnly(!pDoc->m_FileParam.bEdit);


	CString str_temp = L"";
	if(enType!=NEWFILE && enType != VERSIONFILE)
	{
	
		CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
		editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();	 
		
		long lCount=0, nPos=0;
		WCHAR* lpszBuffer;

		//CFile file;
//		AfxMessageBox(lpFileName);

		if (lpFileName == NULL)
			return pDoc;
			
		//if (!file.Open(lpFileName,CFile::modeRead|CFile::shareDenyNone))
		//	return FALSE;
		//lCount = file.GetLength();
		//if (lCount<=0)
		//{
		//	file.Close();
		//	return FALSE;
		//}

		//lpszBuffer = new WCHAR[lCount-2];

		//// read unicode txt file
		//file.Seek(2, CFile::begin);	
		//file.Read(lpszBuffer, lCount-2);

		//for (int i=0; i < lCount/2-1; i++) 
		//	str_temp = str_temp + lpszBuffer[i];
		CString strFileName;strFileName.Format(L"%s", lpFileName);strFileName.MakeLower();
		CFile fil;
		int pos = 0;
		if ( fil.Open( lpFileName , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) == 0 )
		{		
			return pDoc ;
		}
		WORD buffer;
		int bufferCount = 0;
		str_temp.Empty();
		int  bytelen = 0;
		WORD wSignature = 0;
		fil.Read(&wSignature, 2);
		if(wSignature == 0xBBEF || wSignature ==0xBBBF || wSignature ==0xBBEFBF )
		{
			fil.Seek(3,CFile::begin);
			int nFileLen = (int)fil.GetLength()-2;
			char* szDbsBuffer = new char[nFileLen+1];
			ZeroMemory(szDbsBuffer, sizeof(char)*(nFileLen+1));
			int nRead = (int)fil.Read(szDbsBuffer, nFileLen);
			ASSERT(nRead == nFileLen);

			int nMBLen = MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, NULL, NULL);
			WCHAR* szWcsBuffer = new WCHAR[nMBLen+1];
			ZeroMemory(szWcsBuffer, sizeof(WCHAR)*(nMBLen+1));
			MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, szWcsBuffer, nMBLen);
			str_temp = szWcsBuffer;
			str_temp  = str_temp.Right(nFileLen - 1);
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
						str_temp += CString((WCHAR)buffer);
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
					str_temp += (CString)buffer;
					str_temp += _T("\n");
					unRead = fil.Read(buffer, sizeof(buffer)-1);
				}
			}
		}
		//str_temp.Replace(L"'",L"'+char(39)+N'");
		//str_temp.Replace(L"%",L"%%");
		str_temp.Replace(L"\r\n",L"\n");
		str_temp.Replace(L"\n",L"\r\n");
		str_temp.Replace(L"\r\n",L"\r");
		str_temp.Replace(L"\r",L"\r\n");
		if(pDoc->m_FileParam.nColID == -1 && pDoc->m_FileParam.bHaveSource == TRUE && pDoc->m_FileParam.b_AutoSkipLine == TRUE)
		{
			str_temp.Replace(L"\r\n\r\n",L"\r\t\n\r\t\n");
			str_temp.Replace(L"\r\n ",L"\r\t\n ");
			str_temp.Replace(L"\r\n　",L"\r\t\n　");
			str_temp.Replace(L"\r\n\t",L"\r\t\n\t");
			str_temp.Replace(L"\r\n",L"");
			str_temp.Replace(L"\r\t\n\r\t\n",L"\r\n\r\n");
			str_temp.Replace(L"\r\t\n\t",L"\r\n\t");
			str_temp.Replace(L"\r\t\n　",L"\r\n　");
			str_temp.Replace(L"\r\t\n ",L"\r\n ");
			pDoc->m_strLastUser = pGlobal->m_pEmployee->m_strLoginCode;
		}
		pView->SetWindowText(str_temp);
		pDoc->strTempOrgText = str_temp;
		//file.Close();
		//delete []lpszBuffer;	
	}

	
	if(enType==WIRESFILE || enType == NETFILE || (enType==LOCALFILE||enType==PUBLISHTEXT))
	{
			//pDoc->m_lWords = pFileParam->dWordcount;
			CString strLastUser;
//			m_FileDB.ReadText_Version(pFileParam->szFileCode, pDoc->m_strVersion, pDoc->m_strVersion_Mark, strLastUser);
			
			

		    CString str_buffer = str_temp;
			str_buffer.Replace(L"\n",L""); 

			pDoc->m_strVersion = str_buffer;

			for (int i = 0; i < str_buffer.GetLength(); i ++)
			{
				pDoc->m_strVersion_Mark = pDoc->m_strVersion_Mark + L"a";
				pDoc->m_strVersion_Comment = pDoc->m_strVersion_Comment + L"_";
				pDoc->m_strVersion_Style = pDoc->m_strVersion_Style + L"a";
			}
			if (enType==LOCALFILE||enType==PUBLISHTEXT)
			{
				pDoc->m_str_Current_Version = L"a"; 
				pDoc->m_str_Current_Ver_Del = L"@"; 
			}
			else
			{
				pDoc->m_str_Current_Version = L"b"; 
				pDoc->m_str_Current_Ver_Del = L"!"; 
			}
			
			pDoc->m_str_Comment = L"";

			CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
			CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
			editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();	
			if(enType==WIRESFILE||(enType==LOCALFILE||enType==PUBLISHTEXT))
				pView->SetTextToUpperView(); 

			pView->SetReadOnly(!pDoc->m_FileParam.bEdit);
			if (pDoc->m_FileParam.bEdit)
				pDoc->SetAllowEdit();
			if(pDoc->m_FileParam.nColID == -1 && pDoc->m_FileParam.bHaveSource == TRUE)
			{
				CString strValue;
				CGlobal::GetInstance()->m_FileDB.GetConfig(L"IsWiresModRecAutoUp", strValue);
				if(strValue=="1")
				{
					pDoc->m_str_Current_Version = L"b"; 
					pDoc->m_str_Current_Ver_Del = L"@"; 
					pDoc->m_strLastUser = L"";
				}
				else
				{
					pDoc->m_str_Current_Version = L"a"; 
					pDoc->m_str_Current_Ver_Del = L"!"; 
				}
			}

			if (enType==WIRESFILE) 
			{	
				pDoc->SetReadOnly(); 
				pView->m_b_disable_modify = true;

				bool bPrintDirect = (*(CWiresParam*)pParam).bPrintDirect;
				if (bPrintDirect)
				{
					CMDIFrameWnd* pMainFrame = (CMDIFrameWnd*)AfxGetMainWnd();
					CMDIChildWnd *pChildFrm = pMainFrame->MDIGetActive();
					pChildFrm->ShowWindow(SW_HIDE);				
					pChildFrm->SendMessage(WM_COMMAND, ID_FILE_PRINT_DIRECT);
					pChildFrm->SendMessage(WM_CLOSE);
					
				}
			}

	}			

	if (pDoc)
	{
		if (b_IsLocked)
			pDoc->SetMsg(strLockUser);
	}
	return pDoc;

}


extern "C" AFX_EXT_API void WINAPI DoFile()
{
	CGlobal *pGlobal=CGlobal::GetInstance();
	if(pGlobal->m_pFileTemplate==NULL)
	{		
		pGlobal->m_pFileTemplate=new CMultiDocTemplate(
			IDI_DOFILE,
			RUNTIME_CLASS(CDoFileDoc),
			RUNTIME_CLASS(CDoFileFrm), 
			RUNTIME_CLASS(CDoFileView));
		AfxGetApp()->AddDocTemplate(pGlobal->m_pFileTemplate);
		pGlobal->m_pFileTemplate->OpenDocumentFile(NULL);
		pGlobal->bDoFileOpened = TRUE;
	}
	else 
	{
		pGlobal->ActiveDoFile();
	}
}

extern "C" AFX_EXT_API void WINAPI DoFileWithID(LPTSTR strFileID)
{
	CGlobal *pGlobal=CGlobal::GetInstance();
	BSTR bstr = strFileID;
	pGlobal->str_Share = bstr;
	pGlobal->m_FileDB.GetFileDateAndLibIDWithID(pGlobal->str_Share, pGlobal->strLibId, pGlobal->strPlaceCode);

	if(pGlobal->m_pFileTemplate!=NULL)
		pGlobal->m_pFileTemplate->CloseAllDocuments(TRUE);
	pGlobal->m_pFileTemplate=new CMultiDocTemplate(
		IDI_DOFILE,
		RUNTIME_CLASS(CDoFileDoc),
		RUNTIME_CLASS(CDoFileFrm), 
		RUNTIME_CLASS(CDoFileView));
	AfxGetApp()->AddDocTemplate(pGlobal->m_pFileTemplate);
	pGlobal->m_pFileTemplate->OpenDocumentFile(NULL);
	pGlobal->bDoFileOpened = TRUE;
}
extern "C" AFX_EXT_API void WINAPI DoColFile()
{
	CGlobal *pGlobal=CGlobal::GetInstance();
	if(pGlobal->m_pColTemplate==NULL)
	{		
		pGlobal->m_pColTemplate=new CMultiDocTemplate(
			IDI_DOCOL,
			RUNTIME_CLASS(CDoColDoc),
			RUNTIME_CLASS(CDoColFrm), 
			RUNTIME_CLASS(CDoColView));
		AfxGetApp()->AddDocTemplate(pGlobal->m_pColTemplate);
		pGlobal->m_pColTemplate->OpenDocumentFile(NULL);
		pGlobal->bDoColOpened = TRUE;
	}
	else 
	{
		pGlobal->ActiveDoColFile();
	}
}

extern "C" AFX_EXT_API void WINAPI DoColFileWithID(LPTSTR strFileID)
{
	CGlobal *pGlobal=CGlobal::GetInstance();
	BSTR bstr = strFileID;
	pGlobal->str_Share = bstr;
	pGlobal->m_FileDB.GetColFileDateAndColIDWithID(pGlobal->str_Share, pGlobal->strColCode, pGlobal->dt_Share, pGlobal->strLibId);

	if(pGlobal->m_pColTemplate!=NULL)
		pGlobal->m_pColTemplate->CloseAllDocuments(TRUE);
	pGlobal->m_pColTemplate=new CMultiDocTemplate(
		IDI_DOCOL,
		RUNTIME_CLASS(CDoColDoc),
		RUNTIME_CLASS(CDoColFrm), 
		RUNTIME_CLASS(CDoColView));
	AfxGetApp()->AddDocTemplate(pGlobal->m_pColTemplate);
	pGlobal->m_pColTemplate->OpenDocumentFile(NULL);
	pGlobal->bDoColOpened = TRUE;
}
extern "C" AFX_EXT_API void WINAPI DoSearch()
{
	CGlobal *pGL=CGlobal::GetInstance();
	pGL->DoSearch();
}
extern "C" AFX_EXT_API void WINAPI DoHistCheck()
{
	CGlobal *pGL=CGlobal::GetInstance();
	pGL->DoHistCheck();
}
extern "C" AFX_EXT_API void WINAPI DoClear()
{
	CClearFile dlg;
	dlg.DoModal();
}
extern "C" AFX_EXT_API void WINAPI DoRecycle()
{
	CRecycle dlg;
	dlg.DoModal();
}
extern "C" AFX_EXT_API void WINAPI DoCheckUnsaveFiles()
{
	CGlobal::GetInstance()->CheckUnsaveFiles();
}

extern "C" AFX_EXT_API CDocument * WINAPI OpenDocumentPublish(LPTSTR lpFileName,void * pParam, int i_x,  int i_y, int i_width, int i_height)
{

	EN_OPENFILETYPE enType;
	enType = *(EN_OPENFILETYPE*)pParam;
	editDoc *pDoc=NULL;
//	editDoc *pDocNew=NULL;	

	
	CGlobal *pGlobal=CGlobal::GetInstance();
	pGlobal->m_bOldVersion = FALSE;
	pGlobal->m_enOpenFileType = enType;

	TCHAR tcFileCode[30];
	FILEPARAM FileParam;
	BOOL bNew=FALSE;
	memset(&FileParam,0,sizeof(FILEPARAM));
	memset(tcFileCode,0,sizeof(TCHAR)*30);
	CResManage res(g_hInst);
	
	TCHAR *pszDest=_tcsrchr(lpFileName,_T('\\'));
	TCHAR *pszExt =_tcsrchr(lpFileName,_T('.')); 
	if(pszDest&&pszExt&&pszExt>pszDest)
	{
		_tcsncpy(tcFileCode,pszDest+1,pszExt-pszDest-1);
		CFileStatus rStatus;
		CFile::GetStatus(lpFileName,rStatus);
		CString s=rStatus.m_mtime.Format(_T("%Y-%m-%d %H:%M:%S"));
		FileParam.enType = enType;
		if(pGlobal->CheckFile((CString)tcFileCode,s,FileParam))
		{
			if(!pGlobal->LockFile(tcFileCode))
				return NULL;
		}
		else
			bNew=TRUE;
		FileParam.bEdit = TRUE;
	}

	// open Document
		pGlobal->m_pDocTemplate = NULL;
		pGlobal->m_pDocTemplate=new CMultiDocTemplate(
			IDR_MAINFRAME,
			RUNTIME_CLASS(editDoc),
			RUNTIME_CLASS(editFrameNew), 
			RUNTIME_CLASS(editView_Bottom));
		AfxGetApp()->AddDocTemplate(pGlobal->m_pDocTemplate);
		ASSERT(pGlobal->m_pDocTemplate!=NULL);

		pDoc=(editDoc*)pGlobal->m_pDocTemplate->OpenDocumentFile(lpFileName);

	if (!pDoc)
		return NULL;		

	pDoc->m_FileParam.bNewFile = bNew;	
	pDoc->m_FileParam.enType = enType;


	pDoc->m_FileParam.bCanEdit = FileParam.bCanEdit;
	pDoc->m_FileParam.bRecord = FileParam.bRecord;
	pDoc->m_FileParam.bEdit =pDoc->m_FileParam.bIsLocked= FileParam.bEdit;	
	_tcscpy(pDoc->m_FileParam.szFileCode,tcFileCode);

	pDoc->SetReadOnly(); 
	pDoc->m_bCreate=pDoc->m_FileParam.bNewFile;

	CString str_temp = L"";
	
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();	 
		
	long lCount=0, nPos=0;
	WCHAR* lpszBuffer;

	if (lpFileName == NULL)
		return pDoc;
			
	CString strFileName;strFileName.Format(L"%s", lpFileName);strFileName.MakeLower();
	CFile fil;
	int pos = 0;
	if ( fil.Open( lpFileName , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) == 0 )
	{		
		return pDoc ;
	}
	WORD buffer;
	int bufferCount = 0;
	str_temp.Empty();
	int  bytelen = 0;
	WORD wSignature = 0;
	fil.Read(&wSignature, 2);
	if(wSignature == 0xBBEF || wSignature ==0xBBBF || wSignature ==0xBBEFBF )
	{
		fil.Seek(3,CFile::begin);
		int nFileLen = (int)fil.GetLength()-2;
		char* szDbsBuffer = new char[nFileLen+1];
		ZeroMemory(szDbsBuffer, sizeof(char)*(nFileLen+1));
		int nRead = (int)fil.Read(szDbsBuffer, nFileLen);
		ASSERT(nRead == nFileLen);

		int nMBLen = MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, NULL, NULL);
		WCHAR* szWcsBuffer = new WCHAR[nMBLen+1];
		ZeroMemory(szWcsBuffer, sizeof(WCHAR)*(nMBLen+1));
		MultiByteToWideChar(CP_UTF8, 0, szDbsBuffer, nFileLen, szWcsBuffer, nMBLen);
		str_temp = szWcsBuffer;
		str_temp  = str_temp.Right(nFileLen - 1);
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
					str_temp += CString((WCHAR)buffer);
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
				str_temp += (CString)buffer;
				str_temp += _T("\n");
				unRead = fil.Read(buffer, sizeof(buffer)-1);
			}
		}
	}
	//str_temp.Replace(L"'",L"'+char(39)+N'");
	//str_temp.Replace(L"%",L"%%");
	str_temp.Replace(L"\r\n",L"\n");
	str_temp.Replace(L"\n",L"\r\n");
	str_temp.Replace(L"\r\n",L"\r");
	str_temp.Replace(L"\r",L"\r\n");
	if(pDoc->m_FileParam.nColID == -1 && pDoc->m_FileParam.bHaveSource == TRUE && pDoc->m_FileParam.b_AutoSkipLine == TRUE)
	{
		str_temp.Replace(L"\r\n\r\n",L"\r\t\n\r\t\n");
		str_temp.Replace(L"\r\n ",L"\r\t\n ");
		str_temp.Replace(L"\r\n　",L"\r\t\n　");
		str_temp.Replace(L"\r\n\t",L"\r\t\n\t");
		str_temp.Replace(L"\r\n",L"");
		str_temp.Replace(L"\r\t\n\r\t\n",L"\r\n\r\n");
		str_temp.Replace(L"\r\t\n\t",L"\r\n\t");
		str_temp.Replace(L"\r\t\n　",L"\r\n　");
		str_temp.Replace(L"\r\t\n ",L"\r\n ");
		pDoc->m_strLastUser = pGlobal->m_pEmployee->m_strLoginCode;
	}
	pView->SetWindowText(str_temp);


	CString strLastUser;

	CString str_buffer = str_temp;
	str_buffer.Replace(L"\n",L""); 

	pDoc->m_strVersion = str_buffer;

	for (int i = 0; i < str_buffer.GetLength(); i ++)
	{
		pDoc->m_strVersion_Mark = pDoc->m_strVersion_Mark + L"a";
		pDoc->m_strVersion_Comment = pDoc->m_strVersion_Comment + L"_";
		pDoc->m_strVersion_Style = pDoc->m_strVersion_Style + L"a";
	}
	pDoc->m_str_Current_Version = L"a"; 
	pDoc->m_str_Current_Ver_Del = L"@"; 

	pDoc->m_str_Comment = L"";

//	pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
//	pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();

//	pView = (editView_Bottom *) pChild->GetActiveView();	
	pView->SetTextToUpperView(); 

	pView->SetReadOnly(!pDoc->m_FileParam.bEdit);
	if (pDoc->m_FileParam.bEdit)
		pDoc->SetAllowEdit();
	if(pDoc->m_FileParam.nColID == -1 && pDoc->m_FileParam.bHaveSource == TRUE)
	{
		CString strValue;
		CGlobal::GetInstance()->m_FileDB.GetConfig(L"IsWiresModRecAutoUp", strValue);
		if(strValue=="1")
		{
			pDoc->m_str_Current_Version = L"b"; 
			pDoc->m_str_Current_Ver_Del = L"@"; 
			pDoc->m_strLastUser = L"";
		}
		else
		{
			pDoc->m_str_Current_Version = L"a"; 
			pDoc->m_str_Current_Ver_Del = L"!"; 
		}
	}

//	AfxMessageBox(L"Open 1");
/*		CRect rcMonitor, rcMain;
		pFrame->GetWindowRect(rcMain);

		SystemParametersInfo(SPI_GETWORKAREA,NULL,&rcMonitor,NULL);

		if (rcMain.right + 20 <= rcMonitor.right && rcMain.bottom + 20 <= rcMonitor.bottom)
			rcMain += CPoint(20, 20);
		else
			rcMain += rcMonitor.TopLeft() - rcMain.TopLeft();
		pChild->SetWindowPos(NULL, i_x, i_y, rcMain.Width(), rcMain.Height(),
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
*/
	return pDoc;

}
