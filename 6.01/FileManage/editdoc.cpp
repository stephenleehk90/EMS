// editDoc.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "editDoc.h"
#include "TransDlg.h"
#include "dofilemgr.h"
#include "fileinfodlg.h"
#include "..\commkits\dostools.h"
#include "..\fileoper\fileopermgr.h"
#include "editview_bottom.h"
//#include "WordCheckDlg.h"
#include "ChoseDicDlg.h"
#include "editframenew.h"
#include "editview_top_RichEdit.h"
#include "DlgSelectLib.h"
#include "RebuildHistory.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// editDoc
BOOL g_bReplace = FALSE;
#define ID_CHECK_DISCONNECT 88649000


wchar_t	sz_User[NAME_SIZE] = L"User";

COLORREF p_Color[MAX_MODIFY_NUM+1] = 
{
	RGB(0, 0, 0),
	RGB(0, 0, 255),
	RGB(255, 0, 0),
	RGB(128, 0, 0),
	RGB(0, 128, 128),
	RGB(128, 128, 0),
	RGB(128, 0, 128),
	RGB(0, 0, 128),
	RGB(0, 255, 0),
	RGB(0, 128, 0),
	RGB(255, 0, 255),
	RGB(0, 255, 255),
	RGB(192, 192, 192),
	RGB(192, 0, 128),
	RGB(255, 128, 0),
	RGB(128, 128, 128)
};
COLORREF p_CommentColor[MAX_COMMENT_NUM+1] = 
{
	RGB(0, 0, 0),
	RGB(189, 189, 255),
	RGB(255, 189, 189),
	RGB(214, 189, 189),
	RGB(189, 214, 214),
	RGB(214, 214, 189),
	RGB(214, 189, 214),
	RGB(189, 189, 214),
	RGB(189, 255, 189),
	RGB(189, 214, 189),
	RGB(255, 189, 255),
	RGB(189, 255, 255),
	RGB(230, 230, 230),
	RGB(230, 189, 214),
	RGB(255, 214, 189),
	RGB(214, 214, 214)
};

extern int g_bReplace ;
int g_nWordCountType=1;
extern "C" void WINAPI DoFile();
extern "C" void WINAPI DoColFile();

IMPLEMENT_DYNCREATE(editDoc, CDocument)

editDoc::editDoc()
{
	// TODO: add one-time construction code here
	memset(&m_FileParam,0,sizeof(FILEPARAM));
	CGlobal *pGlobal=CGlobal::GetInstance();
	_tcscpy(m_FileParam.szTempPath,pGlobal->GetTempPath());
	m_FileParam.bCanEdit = FALSE;
	m_FileParam.bNewFile = FALSE;
	m_FileParam.bRecord = FALSE;
	m_FileParam.bLib = TRUE;
	m_WiresParam.bCanSaveToLib = FALSE;
	m_WiresParam.strDate = _T("");
	m_WiresParam.strWiresCode = _T("");
	m_WiresParam.strWiresFile = _T("");
	m_DMParam.nDMFI_id = -1;
	m_hDefaultMenu = NULL;

	lstrcpy(m_szAbstract, L"");
	lstrcpy(m_szAuthor,L"");
	lstrcpy(m_szUser, sz_User);
	lstrcpy(m_szPrintCaption, L"");
	
	m_bPending = FALSE;

	m_strVersion = L"";
	m_strVersion_Mark = L"";
	m_strModHistory = L"";
	m_strLastUser = L"";
	m_strVersion_Comment = L"";
	m_strVersion_Style = L"";
	m_str_Comment = L"";

	m_str_Current_Version = "a";
	m_str_Current_Ver_Del = "!";

	m_bNeedSaveDB = FALSE;

	CString s1;wchar_t sz_temp[255];
	GetPrivateProfileStringW( L"System" , L"EditorDebug",L"0",sz_temp ,255,((CWinApp*)AfxGetApp())->m_pszProfileName);	
	s1 = sz_temp;
	if(s1==L"1")
		m_bEditorLog = TRUE;
	else
		m_bEditorLog = FALSE;

	m_bClose = TRUE;

	p_str_Ver_Del = L" !@#¡$¢£%¤¥¦^§¨©ª&«¬­®¯°*±²³´µ¶(•¸¹º»¼½)¾¿ÀÁÂÃÄÅ+ÆÇÈÉÊËÌÍÎ{ÏÐÑÒÓÔÕÖ×Ø}ÙÚÛÜÝÞßàáâã<äåæçèéêëìíîï>ðñòóôõö÷øùúûý";
	p_str_Ver = L"Aabcdefghijklmno";
	p_str_Ver_Style = L" abcdefghijklmno";
	p_str_Ver_Comment = L" !@#$%^&*()+{}<>abcdefghijklmno";
	m_bIsStoryFile = FALSE;
	m_bNewCheckingApplied = FALSE;
}

BOOL editDoc::OnNewDocument()
{
	if(m_bEditorLog==TRUE)
		OnLogFile(L"Open New File");
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetModifiedFlag(FALSE);

	lstrcpy(m_szAuthor,m_szUser);

	CString strPath=m_FileParam.szTempPath ;
	m_strTmpFileCode=GetTempFileID();
	strPath+=_T("\\")+m_strTmpFileCode;
	SetPathName(strPath);

	m_FileParam.bNewFile = TRUE;
	m_FileParam.bRecord = FALSE;
	m_FileParam.nDocStyle=0;
	m_bCreate = TRUE;
	
	return TRUE;
}

editDoc::~editDoc()
{
	if (m_hDefaultMenu)
		::DestroyMenu(m_hDefaultMenu);	
}


BEGIN_MESSAGE_MAP(editDoc, CDocument)
	//{{AFX_MSG_MAP(editDoc)
	ON_COMMAND(ID_DO_COMMIT, OnDoCommit)
	ON_UPDATE_COMMAND_UI(ID_DO_COMMIT, OnUpdateDoCommit)
	ON_COMMAND(ID_WORD_CHECK,OnWordCheck)
	ON_UPDATE_COMMAND_UI(ID_WORD_CHECK,OnUpdateWordChk)
	ON_UPDATE_COMMAND_UI(ID_UP, OnUpdateUp)
	ON_UPDATE_COMMAND_UI(ID_DOWN, OnUpdateDown)
	ON_UPDATE_COMMAND_UI(ID_RIGHT, OnUpdateRight)
	ON_UPDATE_COMMAND_UI(ID_LEFT, OnUpdateLeft)
	ON_UPDATE_COMMAND_UI(ID_DOUP, OnUpdateDoup)
	ON_UPDATE_COMMAND_UI(ID_ZEROC, OnUpdateZeroC)
	ON_UPDATE_COMMAND_UI(ID_ZEROF, OnUpdateZeroF)
	ON_UPDATE_COMMAND_UI(ID_DIVIDE, OnUpdateLeft)
	ON_UPDATE_COMMAND_UI(ID_MULTIPLY, OnUpdateMultiply)
	ON_UPDATE_COMMAND_UI(ID_ZONE, OnUpdateZone)
	ON_UPDATE_COMMAND_UI(ID_DEL_MARK, OnUpdateDel)
	ON_COMMAND(ID_DO_MODIFY, OnDoModify)
	ON_UPDATE_COMMAND_UI(ID_DO_MODIFY, OnUpdateDoModify)
	ON_COMMAND(ID_DO_SEL, OnDoSel)
	ON_UPDATE_COMMAND_UI(ID_DO_SEL, OnUpdateDoSel)
	ON_COMMAND(ID_DO_SOURCE, OnDoSource)
	ON_UPDATE_COMMAND_UI(ID_DO_SOURCE, OnUpdateDoSource)
	ON_COMMAND(ID_DO_SENDCOL, OnDoSendcol)
	ON_UPDATE_COMMAND_UI(ID_DO_SENDCOL, OnUpdateDoSendcol)
	ON_COMMAND(ID_DO_SIGN, OnDoSign)
	ON_UPDATE_COMMAND_UI(ID_DO_SIGN, OnUpdateDoSign)
	ON_COMMAND(ID_DO_SIGNDIRECT, OnDoSigndirect)
	ON_UPDATE_COMMAND_UI(ID_DO_SIGNDIRECT, OnUpdateDoSigndirect)
	ON_COMMAND(ID_DO_BACKFILE, OnDoBackfile)
	ON_UPDATE_COMMAND_UI(ID_DO_BACKFILE, OnUpdateDoBackfile)
	ON_COMMAND(ID_RDO_COMMENT, OnRdoComment)
	ON_UPDATE_COMMAND_UI(ID_RDO_COMMENT, OnUpdateRdoComment)
	ON_COMMAND(ID_RDO_RECORD, OnRdoRecord)
	ON_UPDATE_COMMAND_UI(ID_RDO_RECORD, OnUpdateRdoRecord)
	ON_COMMAND(ID_RDO_COPY, OnRdoCopy)
	ON_UPDATE_COMMAND_UI(ID_RDO_COPY, OnUpdateRdoCopy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	//for test
//	ON_MESSAGE(WM_COTENTCHANGED,OnContentChanged)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// editDoc diagnostics

#ifdef _DEBUG
void editDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void editDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// editDoc serialization

void editDoc::Serialize(CArchive& ar)
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
// editDoc commands

void editDoc::SaveDocumentAs() 
{
	SetModifiedFlag(FALSE);
}

void editDoc::SaveDocumentLocal() 
{

	//CGlobal *pGL=CGlobal::GetInstance();
	CString strFileCode=_T(""),strError, strTitle;
/*
	CResManage setRes(g_hInst);
	if(m_FileParam.bNewFile)
	{
		if(!pGL->m_FileDB.GetNewFileCode(strFileCode))
		{
			CResManage setRes(g_hInst);
			strError.LoadString(IDS_CREATENEWFILECODEERR);
			AfxMessageBox(strError);
			return ;
		}
	}
	else if(m_FileParam.bRecord)
		strFileCode=m_FileParam.szFileCode;
	else 
	{
		if(!pGL->m_FileDB.GetNewFileCode(strFileCode))
		{
			AfxMessageBox(IDS_CREATENEWFILECODEERR);
			return;
		}
	}
*/
//	strFileCode=m_FileParam.szFileCode;

	CString str_Temp = m_FileParam.szFileCode;
	
	if (!str_Temp.IsEmpty())
		strFileCode=str_Temp;
	else
		strFileCode=m_strTmpFileCode;

	CString strPathName=GetPathName();
	
	int nFind=strPathName.ReverseFind('\\');
	if(nFind!=-1)
		strPathName=strPathName.Left(nFind+1);
	_tcscpy(m_FileParam.szFilePath,strPathName);
	strPathName+=strFileCode;
	SetPathName(strPathName);
	BOOL bIsModified=IsModified();
	m_bNeedSaveDB=TRUE;

	OnSaveDocument(strPathName);

	if(!m_FileParam.bNewFile)
	{	
		BOOL bQuit=FALSE;
		DoCommit(bQuit);
	}
}





void editDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
	
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
	if (m_WiresParam.enOpenType!=WIRESFILE&&CGlobal::GetInstance()->IsWorking())
//	if (m_WiresParam.enOpenType!=WIRESFILE&&m_bTransfer)
	{
		
//		if(nDocStyle==0  && AfxGetMainWnd()->IsWindowVisible())
		if(nDocStyle==0  && (AfxGetMainWnd() != NULL))
		{
			if(bLib)
			{				
				int nExclusive, nLib;
				CString strPlace, strLibName, strTitle;
				
				//if (m_bHasCommit&&pGL->m_FileDB.GetLibFileInfo(m_FileParam.szFileCode, strPlace, strLibName, nLib, nExclusive))
				if (m_bTransfer&&pGL->m_FileDB.GetLibFileInfo(m_FileParam.szFileCode, strPlace, strLibName, strTitle,nLib, nExclusive))
				{						
					if (m_FileParam.bEdit)
					{					
						CTransDlg dlg;
						dlg.m_bNewFile = m_bCreate;
						dlg.m_bExclusive = nExclusive>0;
						dlg.m_nLibID = nLib;
						dlg.m_strFileCode = m_FileParam.szFileCode;
						dlg.m_strPlace = strLibName + strPlace;
						dlg.m_strTitle = strTitle;
						dlg.m_strPlaceName = strPlace;
						dlg.DoModal();
					}
				}
				
				if (m_FileParam.enType != CAPFILE && m_FileParam.enType != VERSIONFILE && m_FileParam.enType != PUBLISHTEXT && m_FileParam.enType != PUBLISHTEXTMULTI)
				{
					if (!m_bIsStoryFile)
						DoFile();
					else
					{
						AfxGetMainWnd()->SendMessage(WM_DO_PUBLISH, m_FileParam.lStartWords ,0);
					}

				}
				
			}
			else
			{
				if (m_FileParam.enType != CAPFILE && m_FileParam.enType != PUBLISHTEXT && m_FileParam.enType != PUBLISHTEXTMULTI)
				{
					if (!m_bIsStoryFile)
						DoColFile();
				}
			}
		}
	}
	ShowWindow(((editFrameNew*)pChild)->m_hWnd, SW_MINIMIZE);
	CMDIChildWnd *pChild2 = (CMDIChildWnd *) pFrame->GetActiveFrame();
	if(m_bEditorLog==TRUE)
		OnLogFile("File Minimize");
	if (pChild2 != pChild)
	{
		pChild2->MDIMaximize();
		do
		{
			MSG msg;
			GetMessage(&msg,NULL,0,0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		while (((editFrameNew*)pChild)->GetUpperProcessingFlag() == TRUE);
	}
	if(m_bEditorLog==TRUE)
		OnLogFile(L"Close File");
	CDocument::OnCloseDocument();
	
}

BOOL editDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_b_allow_edit)
		return TRUE;
	
	BOOL bRet = TRUE;
//	SaveBkFile();
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
			else if(IsModified())
			{
				CString strMsg;
				CResManage setRes(g_hInst);
				strMsg.LoadString(IDS_FILEMODIFIED);
				if(AfxMessageBox(strMsg,MB_YESNO,MB_ICONINFORMATION)==IDYES)
					SaveDocumentLocal();
				

				m_bTransfer = FALSE;
			
			}
		}
	}

	return bRet;
}

int editDoc::GetActualWordCount()
{
	CString strText;
	/*CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	//editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();*/
	CDocument* pDoc = this;
	if (pDoc==NULL)
		return 0;
	ASSERT(pDoc != NULL);
   
    CView* pView;
    POSITION pos = pDoc->GetFirstViewPosition();
    while (pos != NULL)
    {
        pView = pDoc->GetNextView(pos);
        if (pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))
        {
            //pView->GetParentFrame()->ActivateFrame();
            break;
        }
	}
	if(pView!=NULL)
		pView->GetWindowText(strText);
	
	// skip space character 
	strText.Replace(L" ",L""); 		

	// skip return
	strText.Replace(L"\r\n",L""); 		

	// skip TAB character
	strText.Replace(L"\t",L""); 		


	return strText.GetLength(); 
}

CString editDoc::GetAllCommentString()
{
	CString strTemp = L""; 
	for (int i=0; i < m_arrComment.GetSize(); i++)  
	{
		DOC_COMMENT temp_comment = m_arrComment.GetAt(i); 

		if (strTemp.IsEmpty())
			strTemp = temp_comment.strComment;  
		else
			strTemp = strTemp + L"#$^" + temp_comment.strComment;  
	}

	return strTemp;
}

void editDoc::InitCommentStringArray()
{

	int i_ver = 0;
	int i_pos = 0;
	int i_start = 0;
	int i_current_index = 0;

	m_arrComment.RemoveAll();
	
	CString strTemp = L"";

	while(i_start < this->m_str_Comment.GetLength())
	{
		 i_pos = m_str_Comment.Find(L"#$^", i_start);  
		
		 if  (i_pos < 0) 
		 {
			DOC_COMMENT temp_comment;
			temp_comment.iIndex = i_current_index;
			strTemp = m_str_Comment.Mid(i_start);
			temp_comment.strComment = strTemp;
			m_arrComment.Add(temp_comment);  	
			i_current_index = i_current_index + 1;
			break;
		 }
		 else
		 {

			DOC_COMMENT temp_comment;
			temp_comment.iIndex = i_current_index;
			 
			 strTemp = m_str_Comment.Mid(i_start, i_pos - i_start);
			temp_comment.strComment = strTemp;
			m_arrComment.Add(temp_comment);  	
			i_current_index = i_current_index + 1;
		 }	
		 i_start =  i_pos + 3;
	}


}

BOOL editDoc::CheckCompareDiff()
{
	CString strVersion, strVersion_Mark, strContent, strLastUser;
	strLastUser = m_strLastUser;
	strContent = GetEditViewText();
	strVersion = m_strVersion;
	strVersion_Mark = m_strVersion_Mark;
	int i = 1 + 1 + m_strModHistory.Replace(L"#",L"#");
	CString str_Ver_Del = p_str_Ver_Del[1];
	for (int j = 1; j < p_str_Ver_Del.GetLength(); j++)
	{
		if (j<i)
			str_Ver_Del += p_str_Ver_Del.Mid(j*(j-1)/2+2,i-1) + p_str_Ver_Del.Mid(j*(j-1)/2+2+i,j-i);
		else
			str_Ver_Del +=  p_str_Ver_Del.Mid(j*(j-1)/2+2+i,j-i);
	}
	str_Ver_Del += p_str_Ver.Mid(i+1);
	int index;
	while((index = strVersion_Mark.FindOneOf(str_Ver_Del))>= 0)
	{
		strVersion.Delete(index);
		strVersion_Mark.Delete(index);
	}

	if (strVersion.GetLength()==0)
	{
		strLastUser.Format(L"%c%c",65279,32);
		strContent.Replace(strLastUser,L"");
	}
	TCHAR char_temp = strContent.GetAt(0);
	if (char_temp == 65279)
		strContent = strContent.Right(strContent.GetLength()-1);
	strContent.Replace(L"\r\n",L"\r");
	strContent.Replace(L"\n",L"\r");
	strVersion.Replace(L"\r\n",L"\r");
	strVersion.Replace(L"\n",L"\r");
	//strVersion.Delete(0,1);

	int iCheck = GetPrivateProfileIntW( L"System" , L"EditorHistDebug",0,((CWinApp*)AfxGetApp())->m_pszProfileName);	

	if(iCheck == 1)
	{
		//if(AfxMessageBox(L"測試新檢查歷史?",MB_YESNO) != IDYES)
		//	return true;
		CRebuildHistory rebuild;
		rebuild.m_pDoc = this;
		rebuild.m_strText = GetEditViewText();
		rebuild.CompareDiff();
		m_strVersion_Mark = rebuild.m_strOldMask;
		m_strVersion = rebuild.m_strOldVersion;
		m_bNewCheckingApplied = TRUE;
		return true;
	}

	if (strContent != strVersion)
	{
		//if(AfxMessageBox(L"歷史記錄出錯，確定修正嗎?",MB_YESNO) != IDYES)
		//	return false;
		CRebuildHistory rebuild;
		rebuild.m_pDoc = this;
		rebuild.m_strText = GetEditViewText();
		rebuild.CompareDiff();
		m_strVersion_Mark = rebuild.m_strOldMask;
		m_strVersion = rebuild.m_strOldVersion;
		m_bNewCheckingApplied = TRUE;
	}

	return true;
}

BOOL editDoc::DoCommit(BOOL &bClose)
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
	CString strValue;
	
	int nFind=strPathName.ReverseFind('\\');
	if(nFind!=-1)
		strPathName=strPathName.Left(nFind+1);
	_tcscpy(m_FileParam.szFilePath,strPathName);
	strPathName+=strFileCode;
	SetPathName(strPathName);
	BOOL bIsModified=IsModified();
	//if (bIsModified||m_bNeedSaveDB)

	OnSaveDocument(strPathName);
	
	TCHAR str1[255],str2[255],str3[255],str4[255];
	lstrcpy(str1, L"Test1");
	lstrcpy(str2, L"Test2");
	lstrcpy(str3, L"Test3");
	lstrcpy(str4, L"Test4");

	CFileStatus rStatus; 
	CFile::GetStatus(GetPathName()+_T(".")+EDITFMT,rStatus);
	//CFile::GetStatus(GetPathName()+_T(".txt"),rStatus);
	if(CheckCompareDiff() == FALSE)
		return FALSE;
	
	if(!m_FileParam.bRecord)//需要插入一條新記錄
	{		
		CFileInfoDlg dlgInfo;
//		dlgInfo.m_strTitle=GetFileTitleWord((LPSTR)(LPCTSTR)(strPathName+_T(".txt")),1,&lWord);
		CString str, strTemp;
		str = GetEditViewText();
		strTemp = str.Left(str.Find(L"\r\n")==-1?str.GetLength():str.Find(L"\r\n"));
		//dlgInfo.m_strTitle=GetFileTitleWord_UNI((LPTSTR)(LPCTSTR)(strPathName+_T(".txt")),1,&lWord);
		dlgInfo.m_strTitle = strTemp.GetLength()>80?strTemp.Left(80):strTemp;
//		dlgInfo.m_strTitle=GetFileTitleWord_UNI((LPTSTR)(LPCTSTR)(strPathName+_T(".FMT")),1,&lWord);
		lWord = GetActualWordCount();
		CString strTempCap;
		if(m_FileParam.enType==CAPFILE&&lWord ==0&&m_CaptionParam.isSimpCap==TRUE)
		{
			strTempCap = m_CaptionParam.strCaption;
			strTempCap.Replace(L" ",L""); 		
			strTempCap.Replace(L"\r\n",L""); 		
			strTempCap.Replace(L"\t",L""); 	
			lWord = strTempCap.GetLength();
		}

		CString strOper;
		if(m_FileParam.enType==CAPFILE)
			strOper.LoadString(IDS_ADDCAPTION);
		else
			strOper.LoadString(IDS_WRITE);
		dlgInfo.m_bNewFile = TRUE;
		dlgInfo.m_strAuthor = pGL->m_pEmployee->m_strUserName;
//		dlgInfo.m_dbWords=m_lWords;
		dlgInfo.m_dbWords=lWord;
		dlgInfo.m_bModify = TRUE;

		if (m_FileParam.enType == DMFILE)
			dlgInfo.m_bExclusive = (m_DMParam.nDMFI_type > 0);
		CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
		editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();	
		pView->m_bOpenFindDialog=TRUE;

		
		//if(dlgInfo.DoModal()==IDOK)
		if((!m_CaptionParam.strSpecifyPlace.IsEmpty() && !m_CaptionParam.strSpecifyUserCode.IsEmpty())
			|| (dlgInfo.DoModal() == IDOK))
		{			
			pGL->m_FileDB.m_pDB->BeginTransaction();
			BOOL bSuccess_InsertLib = FALSE;


			
//			pView->GetWindowText(m_strVersion); 
			
			m_str_Comment = GetAllCommentString(); 
			
			if(!m_CaptionParam.strSpecifyPlace.IsEmpty() && !m_CaptionParam.strSpecifyUserCode.IsEmpty())
			{
				bSuccess_InsertLib = pGL->m_FileDB.InsertLib(strFileCode,m_strTmpFileCode,lWord,dlgInfo.m_strTitle
					, m_CaptionParam.strSpecifyUserCode, m_CaptionParam.nSpecifyLib_id, m_CaptionParam.strSpecifyPlace,strOper,CString(EDITFMT));
				pGL->m_FileDB.UpdateSimpCapHist(strFileCode, m_CaptionParam.strCaption);
			}
			else
				bSuccess_InsertLib = pGL->m_FileDB.InsertLib(strFileCode,m_strTmpFileCode,lWord,dlgInfo.m_strTitle,pGL->m_pEmployee->m_nGroup,strOper,CString(EDITFMT), m_strVersion, m_strVersion_Comment, m_str_Comment, m_strVersion_Style);
			if(bSuccess_InsertLib)
			{
				FILEINFO FI;
				FI.nExclusive = dlgInfo.m_bExclusive;
				FI.strFilecode = strFileCode;
				FI.strPosBox = L"";
				FI.nSecret = dlgInfo.m_bSecret;
				FI.strTxtAuthor = dlgInfo.m_strAuthor.IsEmpty()?_T(" "):dlgInfo.m_strAuthor;
				FI.bLock = dlgInfo.m_bLock;
				FI.strTxtStyle = dlgInfo.m_strStyle.IsEmpty()?_T(" "):dlgInfo.m_strStyle;
				FI.strTxtTitle = dlgInfo.m_strTitle.IsEmpty()?_T(" "):dlgInfo.m_strTitle;
				FI.strUrgency = dlgInfo.m_bUrgency?'Y':'N';
				strTitle = FI.strTxtTitle;
				
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
				}else
				{
					CString sql, str_Title;
					str_Title = dlgInfo.m_strTitle;
					str_Title.Replace(L"'",L"''");
					sql.Format(L"update text_lib set txt_title = N'%s' where filecode = N'%s'", str_Title, strFileCode);
					pGL->m_FileDB.m_pDB->Execute(sql);
				}
				
				if(!bErr&&m_FileParam.enType ==CAPFILE)
				{
					if(!pGL->m_FileDB.InsertPhotoRecord(&m_CaptionParam,L"新增圖說"))
					{
						bErr=TRUE;
						pGL->m_FileDB.m_pDB->RollbackTransaction();
						AfxMessageBox(L"InsertPhotoRecord error!");
					}
					COleDateTime dt = COleDateTime::GetCurrentTime();
					CString strDate; 
					strDate.Format(L"%s",dt.Format(DATETIMEFORMAT));
					CString strHist = CGlobal::GetInstance()->m_pEmployee->m_strUserName + L"於" + strDate;
					CString strSql; strSql.Format(L"update text_lib set text_mod_hist = N'%s' where filecode = N'%s'",strHist,strFileCode);
					pGL->m_FileDB.m_pDB->Execute(strSql);
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
								//AfxMessageBox("UpdateFilePhotoRelation error!");
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
						//AfxMessageBox("UpdateFilePhotoRelation error!");
					}
				}
				if(!bErr && !pGL->m_FileDB.UpdateHist(strFileCode))
				{
					bErr=TRUE;
					pGL->m_FileDB.m_pDB->RollbackTransaction();
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
		pView->m_bOpenFindDialog=FALSE;
	}
	else
	{
		//if(bIsModified)

	
		if(m_bPending)
		{
			CString strOper;
			CResManage res(g_hInst);
			strOper.LoadString(IDS_MODIFY);
			strTitle=GetFileTitleWord_UNI((LPTSTR)(LPCTSTR)(strPathName+_T(".txt")),1,&lWord);
			lWord = GetActualWordCount();

			pGL->m_FileDB.m_pDB->BeginTransaction();

			CString temp;
			double dtemp;
			BOOL btemp;
			pGL->m_FileDB.LoadFileInfo(strFileCode, strTitle, &dtemp, &btemp, &btemp, &btemp, &btemp, temp, temp, temp, temp);
			wchar_t wc_tmp[5000]={0};
			lstrcpy(wc_tmp,strTitle) ; 

			bool b_update_mod_hist = false;
			if (m_strLastUser != pGL->m_pEmployee->m_strLoginCode)
				b_update_mod_hist = true;

//			if(!bErr&&!pGL->m_FileDB.UpdateLib(strFileCode,(const char*)wc_tmp,TRUE,m_lWords,pGL->m_pEmployee->m_nGroup,strOper,m_FileParam.szLastModTime))

			m_str_Comment = GetAllCommentString(); 
			
			//if txt on Column that ignore the "curruser" checking
			BOOL bIgnored = FALSE;
			CString strCurrUser = L"";
			bIgnored = CheckTxtIsOnCol(strFileCode, strCurrUser);
			int RetCode;
			if(!bErr&&!pGL->m_FileDB.UpdateLib(strFileCode,wc_tmp,TRUE,lWord,pGL->m_pEmployee->m_nGroup,strOper,m_FileParam.szLastModTime,m_strVersion,m_strVersion_Mark,m_strModHistory, b_update_mod_hist,m_strVersion_Comment,m_str_Comment,m_strVersion_Style, RetCode))
			{
				bErr=TRUE;
				pGL->m_FileDB.m_pDB->RollbackTransaction();
				if(RetCode == 2)
					AfxMessageBox(L"不能提交稿件!\n另外有用家把稿件上鎖");
				else if (RetCode == 4)
					AfxMessageBox(L"不能提交稿件!\n最後修改時間遭改變");
				else if(RetCode == 3)
					AfxMessageBox(L"不能提交稿件!\n稿件沒有正常開啟");
				else if(RetCode == 1)
					AfxMessageBox(L"不能提交稿件!\n稿件並不存在於系統中");
			}
			if(!bErr&& m_bNewCheckingApplied == TRUE)
			{
				CString strSql = L"";
				strSql.Format(L"update file_record set note = N'更新稿件修改記錄（系統）' where record_id = "
					L"(select top 1 record_id from file_record where filecode = N'%s' and operation = N'修改' "
					L"order by finish_time desc)", strFileCode);
				pGL->m_FileDB.m_pDB->Execute(strSql);
			}

			if(!bErr&& bIgnored == TRUE)
			{
				ResetCurruser(strFileCode, strCurrUser);
			}
			if(!bErr&&!pGL->m_FileDB.UpdateText(strPathName,EDITFMT,strFileCode))
			{
				bErr=TRUE;
				pGL->m_FileDB.m_pDB->RollbackTransaction();
				AfxMessageBox(L"不能提交稿件!\n稿件更新內容時出錯");
			}

			if(!bErr&&!pGL->m_FileDB.UpdateTitles(strFileCode,str1,str2,str3,str4,rStatus.m_mtime.Format(DATETIMEFORMAT)))
			{
				bErr=TRUE;
				pGL->m_FileDB.m_pDB->RollbackTransaction();
				AfxMessageBox(L"不能提交稿件!\n稿件更新標題時出錯");
			}
			//added on 2013-10-16 by Ronald Saliman with Auto Update Txt title option
			bool m_bAutoUpdateTxtTile = FALSE;
			if(pGL->m_FileDB.GetConfig(_T("IsAutoUpdateTxtTile"), strValue))
			{
				m_bAutoUpdateTxtTile = strValue != _T("0") ? TRUE : FALSE;
			}
			if(!bErr&& m_bAutoUpdateTxtTile == TRUE)
			{
				CString str, strTemp;
				str = GetEditViewText();
				strTemp = str.Left(str.Find(L"\r\n")==-1?str.GetLength():str.Find(L"\r\n"));
				CString strTitle = strTemp.GetLength()>80?strTemp.Left(80):strTemp;
				CString sql;
				strTitle.Replace(L"'", L"''");
				sql.Format(L"update text_lib set txt_title = N'%s' where filecode = N'%s'", strTitle, strFileCode);
				pGL->m_FileDB.m_pDB->Execute(sql);
			}
			//
			if(!bErr && !pGL->m_FileDB.UpdateHist(strFileCode))
			{
				bErr=TRUE;
				pGL->m_FileDB.m_pDB->RollbackTransaction();
				AfxMessageBox(L"不能提交稿件!\n稿件更新歷史時出錯");
			}
			if(!bErr)
			{
				pGL->m_FileDB.m_pDB->CommitTransaction();
				CString strDate;
				if(pGL->m_FileDB.GetFileLastModTime(strDate,(CString)m_FileParam.szFileCode))
					_tcscpy(m_FileParam.szLastModTime,strDate);
				else
					bErr=TRUE;
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
//	SaveBkFile();
	
	/*if (bErr)
	{
		//AfxMessageBox(IDS_FILECHANGED);
		m_FileParam.bCanEdit = FALSE;
		m_FileParam.bEdit = FALSE;
		m_FileParam.bSendCol = FALSE;
		m_FileParam.bSign = FALSE;
		m_FileParam.bSignDirect = FALSE;

	}*/

	return !bErr;
}

BOOL editDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	if(m_bEditorLog==TRUE)
		OnLogFile(L"Saving File");
	// TODO: Add your specialized code here and/or call the base class
	wchar_t szFmtFile[PATH_SIZE], szTxtFile[PATH_SIZE];
	wchar_t szBakFile[PATH_SIZE];

	wchar_t szTxtFile_Format[PATH_SIZE];

	wchar_t *pExt;

	CString str_temp = lpszPathName ;
	const wchar_t * wc_tmp = str_temp;
	lstrcpy(szFmtFile, wc_tmp);

	pExt = wcsstr(szFmtFile, L".");
	if(pExt)
		*pExt = '\0';
	//leon
	//lstrcat(szFmtFile, L".FMT");
	lstrcat(szFmtFile, L".txt");
	str_temp = lpszPathName ;
	wc_tmp = str_temp;
	lstrcpy(szTxtFile, wc_tmp);
	pExt = wcsstr(szTxtFile, L".");
	if(pExt)
		*pExt = '\0';
	lstrcat(szTxtFile, L".TXT");

	str_temp = lpszPathName ;
	wc_tmp = str_temp;
	lstrcpy(szBakFile, wc_tmp);

	pExt = wcsstr(szBakFile, L".");
	if(pExt)
		*pExt = '\0';
	lstrcat(szBakFile, L".BAK");

	/////

	str_temp = lpszPathName ;
	wc_tmp = str_temp;
	lstrcpy(szTxtFile_Format, wc_tmp);
	pExt = wcsstr(szTxtFile_Format, L".");
	if(pExt)
		*pExt = '\0';
	//edit by leon li
	//lstrcat(szTxtFile_Format, L".FMT");
	//lstrcat(szFmtFile, L".txt");
	// Backup Old .TXT file
	//if( _waccess( szTxtFile, 0 ) == 0 )		// file exists
	if( _waccess( szFmtFile, 0 ) == 0 )		// file exists
	{
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		if(GetPrivateProfileIntW( L"EditPro" , L"Backup Original File",1,((CWinApp*)AfxGetApp())->m_pszProfileName))
		//if (AfxGetApp()->GetProfileInt(L"EditPro", L"Backup Original File", TRUE))
		//end 20120927
		{
			_wremove(szBakFile);
		}
	}

	
	CFile file;
	CString strText; 
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	// Get the active MDI child window.
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	//editView_Bottom *pView;
	CString str_FileName;
	str_temp.Format(L"%s",szFmtFile);
	str_FileName = str_temp.Right(str_temp.GetLength()-1-str_temp.ReverseFind('\\'));
	str_FileName.Replace(L".txt", L"");
	/*while(pChild)
	{
		if(pChild->IsKindOf(RUNTIME_CLASS(editFrameNew)))
		{
			pView = (editView_Bottom *) pChild->GetActiveView();	
			if(pView->m_fileName.CompareNoCase(m_strTmpFileCode)==0)
				break;
		}
		pChild = (CMDIChildWnd*)pChild->GetNextWindow();
		
	}*/
	CDocument* pDoc = this;   
	ASSERT(pDoc != NULL);   
   
    CView* pView;   
    POSITION pos = pDoc->GetFirstViewPosition();   
    while (pos != NULL)   
    {   
        pView = pDoc->GetNextView(pos);   
        if (pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))   
        {   
            //pView->GetParentFrame()->ActivateFrame();   
            break;   
        }   
    }   
	//pView = (editView_Bottom *) pChild->GetActiveView();
	if(pView!=NULL)
		pView->GetWindowText(strText);
	
	if(m_FileParam.enType ==CAPFILE&&strText.IsEmpty()&&m_CaptionParam.isSimpCap==TRUE)
		strText = m_CaptionParam.strCaption;
	if(strText.IsEmpty())
		strText = L" ";
	//CString test;
	//str_temp.Format(L"%s",szFmtFile);
	//test = str_temp.Right(str_temp.GetLength()-1-str_temp.ReverseFind('\\'));
	//test.Replace(L".txt", L"");
	//pChild = (CMDIChildWnd *) pFrame->FindWindowW(L"editFrameNew",test);
	//pView = (editView_Bottom *) pChild->GetActiveView();	 
	//pView->GetWindowText(strText);
	// Text File
	//if (! file.Open(szTxtFile, CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone))
	if (! file.Open(szFmtFile, CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone))
	{
		CString strMsg = L"Cannot Load File";
		//MessageBox(NULL,szTxtFile,strMsg, MB_OK|MB_ICONEXCLAMATION);
		MessageBox(NULL,szFmtFile,strMsg, MB_OK|MB_ICONEXCLAMATION);
		//		LockDocBuffer(FALSE);
		return FALSE;
	}

	file.SeekToBegin();
	WORD wSignature = 0xFEFF;// use unicode encoding
//	WORD wSignature = 0xEF;
//	WORD wSignature1 = 0xBB;
//	WORD wSignature2 = 0xBF;
	
	file.Write(&wSignature, 2);//Unicode的文件符?
//	file.Write(&wSignature, 1);//Unicode的文件符?
//	file.Write(&wSignature1, 1);//Unicode的文件符?
//	file.Write(&wSignature2, 1);//Unicode的文件符?

	file.Write(strText, strText.GetLength()*2);
	file.Close();

/*
	// Text Format  File
	if (! file.Open(szTxtFile_Format, CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone))
	{
		CString strMsg = L"Cannot Load File";
		MessageBox(NULL,szTxtFile,strMsg, MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	file.SeekToBegin();
//	WORD wSignature = 0xFEFF;// use unicode encoding
	file.Write(&wSignature, 2);//Unicode的文件符?
	file.Write(m_strVersion, m_strVersion.GetLength()*2);
	file.Close();
*/	
	SetModifiedFlag(FALSE);
	
	m_bPending = TRUE;
	
	return TRUE;
}

CString editDoc::GetTempFileID()
{
	int id;
	CString str;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	id = GetPrivateProfileIntW( L"AUTOSAVE" , L"EditProID",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	//id=AfxGetApp()->GetProfileInt(_T("AUTOSAVE"),_T("EditProID"),0);
	CString str_temp;
	str_temp.Format(L"%d",(id+1));
	WritePrivateProfileStringW(L"AUTOSAVE", L"EditProID", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//AfxGetApp()->WriteProfileInt(_T("AUTOSAVE"),_T("EditProID"),id+1);
	//end 20120927
	str.Format(_T("%08X"),id);
	return str;
}


void editDoc::SetReadOnly()
{
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();	 
	if (!m_FileParam.bNewFile )
	{
		pView->SetReadOnly(TRUE); 
		m_b_allow_edit = false;
	}
	else
	{
		pView->SetReadOnly(FALSE); 
		m_b_allow_edit = true;
	}

}


void editDoc::SetAllowEdit()
{
	m_b_allow_edit = true;
}

BOOL editDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if(m_bEditorLog==TRUE)
	{
		CString strFileName;
		strFileName.Format(L"%s", lpszPathName)	;
		OnLogFile(L"Open Existing File " + strFileName);
	}
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;

}

BOOL editDoc::OpenTextFile(const wchar_t *pszPathName)
{
	CFile file;
//	if (access(pszPathName, 02) != 0)	// File is read-only
	_waccess(pszPathName, 02);	// File is read-only
//		m_byteStatus = FS_RDONLY | FS_BYSYSTEM;
	
	if (! file.Open(pszPathName, CFile::modeRead | CFile::shareDenyNone))
	{
		CString strMsg;
//		strMsg.LoadString(IDS_CANNOTOPENFILE);
		MessageBox(NULL,pszPathName, L"Cannot Open File", MB_OK|MB_ICONHAND);
		return FALSE;
	}

	SetModifiedFlag(FALSE);
	return TRUE;
}

void editDoc::OnUpdateDoCommit(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(	m_FileParam.bCanEdit&&m_FileParam.bEdit );
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateUp(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateWordChk(CCmdUI* pCmdUI){
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateDown(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateRight(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateLeft(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateDoup(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateZeroC(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateZeroF(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateDivide(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateMultiply(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateZone(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnUpdateDel(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(true);
	pCmdUI->Enable(m_b_allow_edit);
}

void editDoc::OnDoCommit() 
{

//
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	if (!Check_Connect())
	{
		pFrame->PostMessage(WM_COMMAND, ID_CHECK_DISCONNECT, 0L);
		return;
	}
	if (m_FileParam.enType==PUBLISHTEXT)
	{
		if(AfxMessageBox(L"請確認修改",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1)==IDNO)
			return;

		CFile file;
		CString strText; 
		CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		// Get the active MDI child window.
		CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();

		CDocument* pDoc = this;   
		ASSERT(pDoc != NULL);   
   
		CView* pView;   
		POSITION pos = pDoc->GetFirstViewPosition();   
		while (pos != NULL)   
		{   
			pView = pDoc->GetNextView(pos);   
			if (pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))   
			{   
				break;   
			}   
		}   
		if(pView!=NULL)
			pView->GetWindowText(strText);
	
		if (! file.Open(this->m_strPathName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone))
		{
		}

		file.SeekToBegin();
		WORD wSignature = 0xFEFF;// use unicode encoding
	
		file.Write(&wSignature, 2);//Unicode的文件符?

		file.Write(strText, strText.GetLength()*2);
		file.Close();



		CDocument::OnCloseDocument();

	}
	else
	{
		if(m_bEditorLog==TRUE)
			OnLogFile(L"Commit File");
		// TODO: Add your command handler code here
		m_bPending = TRUE;
		/*CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		// Get the active MDI child window.
		CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();*/
		CDocument* pDoc = this;   
		ASSERT(pDoc != NULL);   
   
		CView* pView;   
		POSITION pos = pDoc->GetFirstViewPosition();   
		while (pos != NULL)   
		{   
			pView = pDoc->GetNextView(pos);   
			if (pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))   
			{   
				//pView->GetParentFrame()->ActivateFrame();   
				break;   
			}   
		}   

		if(IsModified()||m_bPending||m_FileParam.enType==LOCALFILE)
		{
			BOOL bQuit=FALSE;
			BOOL bRet=DoCommit(bQuit);
			if(bRet&&bQuit)
			{
				m_bTransfer = TRUE;
				if(pView!=NULL)
					((editView_Bottom*)pView)->KillUpdateUpperTimers();
				OnCloseDocument();
			}
		}
		else
		{
			m_bTransfer = TRUE;
			if(pView!=NULL)
					((editView_Bottom*)pView)->KillUpdateUpperTimers();
			OnCloseDocument();
		}
	}
}

void editDoc::OnUpdateEditUndoNew(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
}

void editDoc::OnUpdateEditRedoNew(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
}

/*
void editDoc::UpdateTextVersion(CString strAppend)
{
	m_strVersion = m_strVersion + strAppend; 
	m_strVersion_Mark = m_strVersion_Mark + m_str_Current_Version; 
}
*/

void editDoc::UpdateTextVersionPos(CString strAppend, int i_Pos, bool b_redoundo){
	
	CString strText_Start, strText_End;
	CString strText_Start_Mark, strText_End_Mark;

	CString strText_Start_Com, strText_End_Com;

	CString strText_Start_Style, strText_End_Style;

	int i_actual_pos;
	if (strAppend.GetAt(0) == L'\r')
		i_actual_pos = GetEnterMarkPosition(i_Pos, b_redoundo);
	else
		i_actual_pos = GetMarkPosition(i_Pos, b_redoundo); 

	strText_Start = m_strVersion.Left(i_actual_pos); 
	strText_End = m_strVersion.Right(m_strVersion.GetLength() - i_actual_pos); 

	strText_Start_Mark = m_strVersion_Mark.Left(i_actual_pos); 
	strText_End_Mark = m_strVersion_Mark.Right(m_strVersion_Mark.GetLength() - i_actual_pos); 
	
	strText_Start_Com = m_strVersion_Comment.Left(i_actual_pos); 
	strText_End_Com = m_strVersion_Comment.Right(m_strVersion_Comment.GetLength() - i_actual_pos); 

	strText_Start_Style = m_strVersion_Style.Left(i_actual_pos); 
	strText_End_Style = m_strVersion_Style.Right(m_strVersion_Style.GetLength() - i_actual_pos); 

	if (b_redoundo)
	{
		m_strVersion_Mark = m_arrUndo.GetAt(m_arrUndo.GetSize()-1).strMark; 
		m_strVersion_Comment = m_arrUndo.GetAt(m_arrUndo.GetSize()-1).strComment; 
		m_strVersion_Style = m_arrUndo.GetAt(m_arrUndo.GetSize()-1).strStyle;
		if (strAppend.GetLength() < m_strVersion_Mark.GetLength())
		{
			i_actual_pos = GetMarkPosition(i_Pos, b_redoundo);
			CString strTemp;
			strTemp = m_strVersion;
			int i = strTemp.GetLength() - m_strVersion_Mark.GetLength();
			if (i != 0)
				strTemp = m_strVersion.Left(i_actual_pos) + strAppend;

			if(i == 0)
				m_strVersion = strTemp;
			else if (i > 0)
				m_strVersion = strTemp + m_strVersion.Mid(i_actual_pos);
			else
				m_strVersion = strTemp + m_strVersion.Mid(strTemp.GetLength()+i);
		}
		else if (strAppend.GetLength() == m_strVersion_Mark.GetLength())
		{
			m_strVersion = strAppend;
		}
	}
	else
	{
		m_strVersion = strText_Start + strAppend + strText_End; 

		CString strVersion;
		CString strVersion_Com;
		CString strVersion_Style;

		for (int i = 0; i < strAppend.GetLength(); i++) 
		{
			strVersion = strVersion + m_str_Current_Version;
			strVersion_Com = strVersion_Com + L"_";
			strVersion_Style = strVersion_Style + L"a";
		}
		m_strVersion_Mark = strText_Start_Mark + strVersion + strText_End_Mark; 
		m_strVersion_Comment = strText_Start_Com + strVersion_Com + strText_End_Com; 
		m_strVersion_Style = strText_Start_Style + strVersion_Style + strText_End_Style; 
	}

}


CString editDoc::GetCurrentVersion(CString str_lastuser)
{
	int i_max_version = 0;

	int i_version = 0;
	int i_version_del = 0;

	/*for (int i= 0; i < m_strVersion_Mark.GetLength(); i++)
	{

		CString str_temp = m_strVersion_Mark[i];
		i_version = p_str_Ver.Find(str_temp); 
		double itemp =  p_str_Ver_Del.Find(str_temp);
		if (itemp > 2)
			itemp = ceil( (sqrt(8*(itemp-1)+1)-1) /2) +1;
		i_version_del = itemp; 

		if ( i_version > i_max_version || i_version_del > i_max_version)
			i_max_version = i_version > i_version_del? i_version: i_version_del;
	}*/
	CString strtemp = m_strModHistory;
	i_max_version = 1;
	int i =strtemp.Find(L"#");
	while ((i =strtemp.Find(L"#")) > -1)
	{
		strtemp = strtemp.Mid(i+1);
		i_max_version++;
	}

	if(i_max_version>14)
		i_max_version = 15;

	CGlobal *pGL=CGlobal::GetInstance();
	if (i_max_version <= 14 && pGL->m_pEmployee->m_strLoginCode != str_lastuser  )
		i_max_version = i_max_version +1;

	return p_str_Ver[i_max_version];

//	return str_ret;
}

CString editDoc::GetCurrentVersion_Del(CString str_lastuser)
{
	int i_max_version = 0;
	int i_version = 0;
	int i_version_del = 0;

	/*for (int i= 0; i < m_strVersion_Mark.GetLength(); i++)
	{

		CString str_temp = m_strVersion_Mark[i];
		i_version = p_str_Ver.Find(str_temp); 
		double itemp =  p_str_Ver_Del.Find(str_temp);
		if (itemp > 2)
			itemp = ceil( (sqrt(8*(itemp-1)+1)-1) /2) +1;
		i_version_del = itemp; 

		if ( i_version > i_max_version || i_version_del > i_max_version)
			i_max_version = i_version > i_version_del? i_version: i_version_del;
	}*/
	CString strtemp = m_strModHistory;
	i_max_version = 1;
	int i =strtemp.Find(L"#");
	while ((i =strtemp.Find(L"#")) > -1)
	{
		strtemp = strtemp.Mid(i+1);
		i_max_version++;
	}
	
	if(i_max_version>14)
		i_max_version = 15;

	CGlobal *pGL=CGlobal::GetInstance();
	if (i_max_version <= 14 && pGL->m_pEmployee->m_strLoginCode != str_lastuser)
		i_max_version = i_max_version +1;
	
	if (i_max_version > 2)
		i_max_version = (i_max_version-2)*(i_max_version-1)/2+2;
	return p_str_Ver_Del[i_max_version];

}

int editDoc::GetMarkPosition(int i_Pos, bool is_undoredo)
{
	int i_Current_Pos = 0;
	int i_Final_Pos = m_strVersion_Mark.GetLength();
	CString str_tmp;
	
	for (int i=0; i < m_strVersion_Mark.GetLength(); i++)  
	{
		str_tmp = m_strVersion_Mark[i];

//		if (str_tmp ==  L"*") continue;
		if (p_str_Ver_Del.Find(str_tmp) >= 0) continue;

		if (i_Current_Pos == i_Pos) 
		{
			i_Final_Pos = i;
			break;
		}
		i_Current_Pos = i_Current_Pos +1;	

		if (is_undoredo)
		{
			if (i_Current_Pos == i_Pos) 
			{
				i_Final_Pos = i + 1;
				break;
			}
		}

	}

	return i_Final_Pos;
}

int editDoc::GetMarkLength(int i_Pos, int i_Length)
{

	int i_Current_Pos = 0;
	int i_Final_Pos = 0;
	int i=0;
	CString str_tmp;
	
	for (i=i_Pos; i < m_strVersion_Mark.GetLength(); i++)  
	{
		str_tmp = m_strVersion_Mark[i];
//		if (str_tmp ==  L"*") continue;
		/*if (m_bWordBank)
		{
			if (p_str_Ver_Del.Find(str_tmp) >= 0) continue;
		}
		else
			if (p_str_Ver_Del.Find(str_tmp) >= 0 && str_tmp!=m_str_Current_Ver_Del) continue;*/
		if (p_str_Ver_Del.Find(str_tmp) >= 0) continue;

		if (i_Current_Pos == i_Length) 
		{
			i_Final_Pos = i;
			break;
		}
		i_Current_Pos = i_Current_Pos +1;	
	}


	if (i_Current_Pos == i_Length) 
		i_Final_Pos = i;

	if (i_Final_Pos - i_Pos < 0)
		return i_Length;
	else
		return (i_Final_Pos - i_Pos);
}


void editDoc::DeleteTextVersionPos(int i_Pos, int i_Length)
{

	CString strText_Start_Mark, strText_End_Mark;
	CString strText_Start_Com, strText_End_Com;
	CString strText_Start_Style, strText_End_Style;

	int i_actual_pos = GetMarkPosition(i_Pos, false); 

	strText_Start_Mark = m_strVersion_Mark.Left(i_actual_pos); 
	strText_Start_Com = m_strVersion_Comment.Left(i_actual_pos); 
	strText_Start_Style = m_strVersion_Style.Left(i_actual_pos); 

	int i_actual_length = GetMarkLength(i_actual_pos, i_Length ); 

	strText_End_Mark = m_strVersion_Mark.Right(m_strVersion_Mark.GetLength() - i_actual_pos - i_actual_length); 
	strText_End_Com = m_strVersion_Comment.Right(m_strVersion_Comment.GetLength() - i_actual_pos - i_actual_length); 
	strText_End_Style = m_strVersion_Style.Right(m_strVersion_Style.GetLength() - i_actual_pos - i_actual_length); 

	CString strVersion;
	CString strVersion_Com;
	CString strVersion_Style;
	for (int i = i_actual_length-1; i >= 0; i--) 
	{

		if (i_actual_pos+i > m_strVersion.GetLength() -1 ) continue; 

		// delete the full version string if it is a return character or current version 
//		if (m_strVersion[i_actual_pos+i] == L'\r\n' || m_strVersion_Mark[i_actual_pos+i] == m_str_Current_Version ) 
		if (m_strVersion_Mark[i_actual_pos+i] == m_str_Current_Version ) 
		{
			m_strVersion.Delete(i_actual_pos+i); 
			continue;
		}

		if (p_str_Ver_Del.Find(m_strVersion_Mark[i_actual_pos+i]) >= 0)
		{
			strVersion = m_strVersion_Mark[i_actual_pos+i] + strVersion;
			strVersion_Com = m_strVersion_Comment[i_actual_pos+i] + strVersion_Com;
			strVersion_Style = m_strVersion_Style[i_actual_pos+i] + strVersion_Style;
		}
		else
		{
			CString str_Ver_Del;
			if (p_str_Ver.Find(m_strVersion_Mark[i_actual_pos+i]) != 0)
				str_Ver_Del = p_str_Ver_Del[p_str_Ver_Del.Find(m_str_Current_Ver_Del) + p_str_Ver.Find(m_strVersion_Mark[i_actual_pos+i])-1];
			else
				str_Ver_Del = p_str_Ver_Del[p_str_Ver_Del.Find(m_str_Current_Ver_Del) + p_str_Ver.Find(m_strVersion_Mark[i_actual_pos+i])];
			strVersion = str_Ver_Del + strVersion;
			strVersion_Com = L"_" + strVersion_Com;
			strVersion_Style = L"a" + strVersion_Style;
		}
	}
	m_strVersion_Mark = strText_Start_Mark + strVersion + strText_End_Mark; 
	m_strVersion_Comment = strText_Start_Com + strVersion_Com + strText_End_Com; 
	m_strVersion_Style = strText_Start_Style + strVersion_Style + strText_End_Style; 

}

CString editDoc::GetTextVersionComment(int i_Pos, int i_Length)
{
	int i_actual_pos = GetMarkPosition(i_Pos, false); 
	int i_actual_length = GetMarkLength(i_actual_pos, i_Length ); 

	CString strTemp = L"";
	CString strTemp_Comment;

	CString strCurrent = L"";
	CString strAll = L"";
	
	int i_comment_index;

	for (int i = i_actual_length-1; i >= 0; i--) 
	{
		strTemp_Comment  =  m_strVersion_Comment[i_actual_pos + i];

		i_comment_index = p_str_Ver_Comment.Find(strTemp_Comment);

		strCurrent.Format(L"#%ld#",i_comment_index);  

		if (strAll.Find(strCurrent,0) < 0)
		{	

			strAll = strAll + strCurrent;

			for (int k=0; k < m_arrComment.GetSize(); k++)  
			{	
				DOC_COMMENT temp_comment = m_arrComment[k];
				 if (temp_comment.iIndex == i_comment_index)
					strTemp = strTemp + L" \r\n" + temp_comment.strComment; 
			}

		}
	}

	return strTemp;
}

int editDoc::GetTextVersionStyle(int i_Pos)
{
	if (m_strVersion_Style.GetLength() <= 0) 	return 0;


	int i_actual_pos = GetMarkPosition(i_Pos, false); 

	if (i_actual_pos >= m_strVersion_Style.GetLength()) return 0; 

	CString strTemp_Style  =  m_strVersion_Style[i_actual_pos];

	int i_index = p_str_Ver_Style.Find(strTemp_Style);

	if (i_index < 0)
		i_index = 0;
	return i_index;
}


void editDoc::SetTextVersionComment(int i_Pos, int i_Length, int i_comment_index, bool b_Delete)
{

	CString strText_Start_Comment, strText_End_Comment;
	int i_actual_pos = GetMarkPosition(i_Pos, false); 

	strText_Start_Comment = m_strVersion_Comment.Left(i_actual_pos); 

	int i_actual_length = GetMarkLength(i_actual_pos, i_Length ); 

	strText_End_Comment = m_strVersion_Comment.Right(m_strVersion_Comment.GetLength() - i_actual_pos - i_actual_length); 

	CString strTemp = L"";
	for (int i = i_actual_length-1; i >= 0; i--) 
	{

		if (i_actual_pos+i > m_strVersion.GetLength() -1 ) continue; 

		if (b_Delete)		
			strTemp  = strTemp +  L"_";
		else
			strTemp  = strTemp +  p_str_Ver_Comment[i_comment_index];
	}
	m_strVersion_Comment = strText_Start_Comment + strTemp + strText_End_Comment; 

	if (b_Delete)		
		DeleteUnUsedComment();
}

void editDoc::SetTextVersionStyle(int i_Pos, int i_Length, int i_style_index)
{

	CString strText_Start_Style, strText_End_Style;
	int i_actual_pos = GetMarkPosition(i_Pos, false); 

	strText_Start_Style = m_strVersion_Style.Left(i_actual_pos); 

	int i_actual_length = GetMarkLength(i_actual_pos, i_Length ); 

	strText_End_Style = m_strVersion_Style.Right(m_strVersion_Style.GetLength() - i_actual_pos - i_actual_length); 

	CString strTemp = L"";
	for (int i = i_actual_length-1; i >= 0; i--) 
	{

		if (i_actual_pos+i > m_strVersion.GetLength() -1 ) continue; 
		strTemp  = strTemp +  p_str_Ver_Style[i_style_index];
	}
	m_strVersion_Style = strText_Start_Style + strTemp + strText_End_Style; 

}


void editDoc::DeleteUnUsedComment()
{
	for (int i = m_arrComment.GetSize()-1; i >=0 ; i--) 
	{
		DOC_COMMENT temp_comment = m_arrComment[i];
		int i_cur_index = temp_comment.iIndex;

		bool bFound = false;
		for (int k = 0; k < m_strVersion_Comment.GetLength(); k++)
		{
			if (m_strVersion_Comment[k] ==  p_str_Ver_Comment[i_cur_index])
				bFound = true;

		}

		if (!bFound)
			m_arrComment.RemoveAt(i);
	}
}

void editDoc::OnDoModify() 
{
	// TODO: Add your command handler code here
	if(m_WiresParam.enOpenType==WIRESFILE)
	{
		m_bClose = FALSE;
		OnDoSel();
		return;
	}

	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();

	editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();
	CGlobal *pGlobal=CGlobal::GetInstance();
	if (m_FileParam.enType == NETFILE || m_FileParam.enType == REPLACEFILE)
	{
		FILEPARAM *pFileParam = (FILEPARAM *)&m_FileParam;
		if(!pGlobal->LockFile(pFileParam->szFileCode))
		{
			AfxMessageBox(IDS_LOCKFILEFAIL);
			return;
		}
		if(!pGlobal->CanFileModify(pFileParam))
		{
			pGlobal->UnlockFile(pFileParam->szFileCode);
			return;
		}
	}

	if(m_bEditorLog==TRUE)
		OnLogFile(L"Edit File");

	pView->SetReadOnly(FALSE); 
	m_FileParam.bEdit = true;
	m_b_allow_edit = true;


}

void editDoc::OnUpdateDoModify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//edit by leon for bug 1187
//	pCmdUI->Enable(!m_b_allow_edit);
	pCmdUI->Enable(m_FileParam.bCanEdit&&!m_FileParam.bEdit);
}

bool editDoc::AddComment(CString strComment, int &iCommentIndex)
{
	if (m_arrComment.GetSize()>= 30)
		return false;

	DOC_COMMENT temp_comment;  
	
	COleDateTime dt = COleDateTime::GetCurrentTime();
	CString strDate; 
	strDate.Format(L"%s",dt.Format(DATETIMEFORMAT));

	CGlobal *pGL=CGlobal::GetInstance();
	strComment = strComment + L" [" + pGL->m_pEmployee->m_strUserName + L"於 " + strDate + L"]";

	temp_comment.strComment = strComment;	

	temp_comment.iIndex = m_arrComment.GetSize(); 

	iCommentIndex = temp_comment.iIndex;

	m_arrComment.Add(temp_comment);  

	SetModifiedFlag(TRUE);

	return true;

}

void editDoc::GetComment(CString& strComment, int iStart, int iEnd)
{
	strComment = L"";
	for (int i=0; i < m_arrComment.GetSize(); i++)  
	{	
		DOC_COMMENT temp_comment = m_arrComment.GetAt(i);  
	
//		if (iStart >= temp_comment.iStart && iStart <= temp_comment.iEnd)
//			strComment = temp_comment.strComment;	
//		else if (iEnd >= temp_comment.iStart && iEnd <= temp_comment.iEnd)
//			strComment = temp_comment.strComment;	
//		else if (iStart <= temp_comment.iStart && iEnd >= temp_comment.iEnd)
//			strComment = temp_comment.strComment;	
	
	}
}

void editDoc::OnContentChanged(CString strContent){
	CString strText; 
	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	// Get the active MDI child window.
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();	 
	pView->SetWindowText(strContent);
}

//edit by leon 2012-09-04 for word bank
void editDoc::OnWordCheck(){
	
//	CString strText; 
//	CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	// Get the active MDI child window.
//	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
//	editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();	 
//	pView->GetWindowText(strText);
	if(m_bEditorLog==TRUE)
		OnLogFile(L"Word Check");

	CChoseDicDlg dlg;
	dlg.DoModal();
	//SetModifiedFlag();
}


void editDoc::AutoSaveDocumentLocal() 
{

	//CGlobal *pGL=CGlobal::GetInstance();
	CString strFileCode=_T(""),strError, strTitle;
/*
	CResManage setRes(g_hInst);
	if(m_FileParam.bNewFile)
	{
		if(!pGL->m_FileDB.GetNewFileCode(strFileCode))
		{
			CResManage setRes(g_hInst);
			strError.LoadString(IDS_CREATENEWFILECODEERR);
			AfxMessageBox(strError);
			return ;
		}
	}
	else if(m_FileParam.bRecord)
		strFileCode=m_FileParam.szFileCode;
	else 
	{
		if(!pGL->m_FileDB.GetNewFileCode(strFileCode))
		{
			AfxMessageBox(IDS_CREATENEWFILECODEERR);
			return;
		}
	}
*/
//	strFileCode=m_FileParam.szFileCode;

	CString str_Temp = m_FileParam.szFileCode;

	if (!str_Temp.IsEmpty())
		strFileCode=str_Temp;
	else
		strFileCode=m_strTmpFileCode;

	CString strPathName=GetPathName();
	
	int nFind=strPathName.ReverseFind('\\');
	if(nFind!=-1)
		strPathName=strPathName.Left(nFind+1);
	_tcscpy(m_FileParam.szFilePath,strPathName);
	strPathName+=strFileCode;
	SetPathName(strPathName);
	BOOL bIsModified=IsModified();
	m_bNeedSaveDB=TRUE;

	if(m_bEditorLog==TRUE)
		OnLogFile(L"Auto Save File");

	OnSaveDocument(strPathName);

}

int editDoc::GetEnterMarkPosition(int i_Pos, bool is_undoredo)
{
	int i_Current_Pos = 0;
	int i_Final_Pos = m_strVersion_Mark.GetLength();
	CString str_tmp;
	
	for (int i=0; i < m_strVersion_Mark.GetLength(); i++)  
	{
		str_tmp = m_strVersion_Mark[i];
//		if (str_tmp ==  L"*") continue;
		int testLength = m_strVersion.GetLength();
		int testLength2 = m_strVersion_Mark.GetLength();
		if (p_str_Ver_Del.Find(str_tmp) >= 0 && i_Current_Pos != i_Pos && (i!=0 && m_strVersion[i]!='\r')) 
			continue;
		if(p_str_Ver_Del.Find(str_tmp) >= 0)
			continue;

		if (i_Current_Pos == i_Pos) 
		{
			i_Final_Pos = i;
			break;
		}
		i_Current_Pos = i_Current_Pos +1;	

		if (is_undoredo)
		{
			if (i_Current_Pos == i_Pos) 
			{
				i_Final_Pos = i + 1;
				break;
			}
		}

	}

	return i_Final_Pos;
}

CString editDoc::GetPStrVerComment()
{
	return p_str_Ver_Comment;
}

void editDoc::OnLogFile(CString str_Message)
{
	wchar_t str_temp[100];
	GetPrivateProfileStringW( L"System" , L"EditorLogPath", L"C:\\FOUNDER\\TEMP\\EDITOR", str_temp,100,((CWinApp*)AfxGetApp())->m_pszProfileName);
	wsprintf(str_temp,L"%s\\%s", str_temp, L"LOG");
	MakeDirectoryW(str_temp);
	CString strCDate;
	CTime today=CTime::GetCurrentTime();
	strCDate.Format(L"[%04d-%02d-%02d %02d:%02d:%02d]",today.GetYear(),today.GetMonth(),today.GetDay()
		,today.GetHour(),today.GetMinute(), today.GetSecond());
	CString strFile = m_strTmpFileCode==L""?m_FileParam.szFileCode:m_strTmpFileCode;
	str_Message = strCDate + strFile +":" +str_Message;
	wchar_t szErrPath[255];
	swprintf ( szErrPath , L"%s\\EDITOR_%04d%02d%02d.LOG" , str_temp,today.GetYear(),today.GetMonth(),today.GetDay());
	CFile m_DbTeleLog;
	if ( _waccess ( szErrPath , 0 ) != 0 )
		m_DbTeleLog.Open ( szErrPath , 
					CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone);
	else
		m_DbTeleLog.Open ( szErrPath , 
				CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone);
	m_DbTeleLog.SeekToBegin();
	WORD wSignature = 0xFEFF;
	m_DbTeleLog.Write(&wSignature, 2);
	m_DbTeleLog.SeekToEnd();
	str_Message+=L"\r\n";
	m_DbTeleLog.Write ( str_Message.AllocSysString(), str_Message.GetLength()*2);
	m_DbTeleLog.Close();
}

void editDoc::OnUpdateDoSel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_WiresParam.enOpenType==WIRESFILE&&m_WiresParam.bCanSaveToLib);
}

void editDoc::OnDoSel() 
{
	// TODO: Add your command handler code here
	CString strFileCode,strDate;

	int iLibid = -1;
	CString str_LibName;
	CDlgSelectLib dlg;
	if(dlg.DoModal()!=IDOK)
		return ;
	iLibid = dlg.iLibId;
	str_LibName = dlg.str_FilePlace;

	CWiresAttach attach = PrepareAttachment(m_WiresParam.strWiresFile, m_WiresParam.strWiresCode);
	BOOL bOK=CGlobal::GetInstance()->GetFileOperMgr()->SaveWiresToLib(m_WiresParam.strWiresCode,
		//m_WiresParam.strDate,m_WiresParam.strWiresFile,strFileCode, TRUE);
		m_WiresParam.strDate,m_WiresParam.strWiresFile,strFileCode, TRUE, attach);

	if(bOK)
	{
		CGlobal *pGL = CGlobal::GetInstance();
		if(iLibid!=-1 && str_LibName.GetLength()>0)
		{
			CString sql;
			sql.Format(_T("update file_place set lib_id=%d, place_code = N'%s' where filecode=N'%s'"),iLibid,str_LibName,strFileCode);
			pGL->m_FileDB.m_pDB->Execute(sql);
			sql.Format(_T("update file_record set src_libid =%d, lib_id = %d, src_place = N'%s',  place_code = N'%s' where filecode=N'%s'"),iLibid,iLibid,str_LibName,str_LibName,strFileCode);
			pGL->m_FileDB.m_pDB->Execute(sql);
		}
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
		m_FileParam.bHaveSource = TRUE;
		_tcscpy(m_FileParam.szLastModTime,strDate);
		_tcscpy(m_FileParam.szFileCode,strFileCode);
		_tcscpy(m_FileParam.szFilePath,pGL->GetTempPath()); 
		SetReadOnly();
		SetAllowEdit();
		CString strPath;
		strPath = m_FileParam.szFilePath;
		strPath +=_T("\\")+ strFileCode;
		SetPathName(strPath);
		m_WiresParam.enOpenType = NETFILE;

		if(m_bClose == TRUE)
		{
			m_bTransfer = FALSE;
			OnCloseDocument();
		}
		CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
		editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();	 
		pView->SetReadOnly(FALSE);
	}
}

CWiresAttach editDoc::PrepareAttachment(CString strFileName, CString strWiresCode)
{
	CWiresAttach attach;
	attach.strSourcePath = CGlobal::GetInstance()->m_FileDB.GetPDFAttachedFile(strWiresCode, strFileName);
	if(attach.strSourcePath == L"Not Found" ||attach.strSourcePath.GetLength()==0)
	{	
		return attach;
	}
	COleDateTime date;
	date = COleDateTime::GetCurrentTime();
	attach.strUploadDate=date.Format(_T("%Y%m%d"));
	CFile fSource;
	CFileStatus FileStatus;			
	fSource.GetStatus(attach.strSourcePath,FileStatus);
	attach.nSize=FileStatus.m_size;
	attach.strType=attach.strSourcePath.Right(attach.strSourcePath.GetLength()-attach.strSourcePath.ReverseFind('.')-1);	
	attach.strName = attach.strSourcePath.Right(attach.strSourcePath.GetLength()-attach.strSourcePath.ReverseFind('\\')-1);	
	if(attach.strName.GetLength()==0)
		attach.strName= attach.strSourcePath.Right(attach.strSourcePath.GetLength()-attach.strSourcePath.ReverseFind('/')-1);	
	return attach;
}

BOOL editDoc::CheckTxtIsOnCol(CString strFileName, CString &strCurrUser)
{
	BOOL bRet = FALSE;
	CGlobal *pGL=CGlobal::GetInstance();
	CADORecordset rs(pGL->m_FileDB.m_pDB);
	CString strSQL;
	strSQL.Format(L"select curruser from paper_filelist a inner join text_lib b on a.filecode = b.filecode where b.filecode = N'%s'",strFileName );
	if(rs.Open(pGL->m_FileDB.m_pDB->GetActiveConnection(),strSQL,CADORecordset::openQuery))
	{
		while(!rs.IsEof())
		{
			bRet = TRUE;
			rs.GetFieldValue(0,strCurrUser);
			rs.MoveNext();
		}
		if(bRet == TRUE)
		{
			strSQL.Format(L"update text_lib set curruser = NULL where filecode = N'%s'",strFileName );
			pGL->m_FileDB.m_pDB->Execute(strSQL);
		}
	}
	return bRet;
}

BOOL editDoc::ResetCurruser(CString strFileName, CString strCurrUser)
{
	CGlobal *pGL=CGlobal::GetInstance();
	CString strSQL;
	strSQL.Format(L"update text_lib set curruser = N'%s' where filecode = N'%s'",strCurrUser,strFileName );
	return pGL->m_FileDB.m_pDB->Execute(strSQL);;
}

void editDoc::OnUpdateDoSource(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FileParam.bHaveSource);
}

void editDoc::OnDoSource() 
{
	// TODO: Add your command handler code here
	CGlobal::GetInstance()->ViewSource(m_FileParam.szFileCode);
}

void editDoc::OnDoSendcol() 
{
	// TODO: Add your command handler code here
	BOOL bQuit=FALSE;
	BOOL bRet=TRUE;
	if(m_WiresParam.enOpenType==WIRESFILE)
	{
		int nColumnID1;
		CString strDate1,m_strDate;
		CString strUserCode1;
		BOOL bSecret1;
		CString strNote1;
		BOOL bOK1;
		CWiresAttach attach = PrepareAttachment(m_WiresParam.strWiresFile, m_WiresParam.strWiresCode);
		BOOL bRet = CGlobal::GetInstance()->GetFileOperMgr()->SendWiresFileToColumn(m_WiresParam.strWiresCode,m_WiresParam.strDate,m_WiresParam.strWiresFile,
			//nColumnID1,strDate1,strUserCode1,bSecret1,strNote1,bOK1, b_AutoSkipLine);
			nColumnID1,strDate1,strUserCode1,bSecret1,strNote1,bOK1,m_FileParam.b_AutoSkipLine, attach);
		if(bRet == TRUE)CGlobal::GetInstance()->m_FileDB.UpdateWiresFileFlag(m_WiresParam.strWiresCode,m_WiresParam.strWiresFile,_T('D'));
		else
			return;
		m_bTransfer = FALSE;
		OnCloseDocument();
	}else
	{
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
}

void editDoc::OnUpdateDoSendcol(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable((m_FileParam.bSendCol&&!m_FileParam.bNewFile)||(m_WiresParam.enOpenType==WIRESFILE&&m_WiresParam.bCanSaveToLib));
}

void editDoc::OnDoSign() 
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

void editDoc::OnUpdateDoSign(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FileParam.bSign);
}

void editDoc::OnDoSigndirect() 
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

void editDoc::OnUpdateDoSigndirect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FileParam.bSignDirect);
}

void editDoc::OnDoBackfile() 
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

void editDoc::OnUpdateDoBackfile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FileParam.bBack&&!m_FileParam.bNewFile);
}

void editDoc::OnRdoComment() 
{
	// TODO: Add your command handler code here
	CGlobal::GetInstance()->ShowFileInfo(&m_FileParam);

}

void editDoc::OnUpdateRdoComment(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_FileParam.bNewFile);
	if(m_WiresParam.enOpenType == WIRESFILE)
		pCmdUI->Enable(FALSE);
}

void editDoc::OnRdoRecord() 
{
	// TODO: Add your command handler code here
	CGlobal::GetInstance()->ShowFileRecord(m_FileParam.szFileCode,m_FileParam.szTitle);
}

void editDoc::OnUpdateRdoRecord(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_FileParam.bNewFile);
	if(m_WiresParam.enOpenType == WIRESFILE)
		pCmdUI->Enable(FALSE);
}

void editDoc::OnRdoCopy() 
{
	// TODO: Add your command handler code here
	CGlobal::GetInstance()->ShowCopyInfo(m_FileParam.szFileCode);
}

void editDoc::OnUpdateRdoCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FileParam.bCopy&&!m_FileParam.bNewFile);
}

CString editDoc::GetEditViewText()
{
	CString strText;
	/*CMDIFrameWnd *pFrame =     (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =         (CMDIChildWnd *) pFrame->GetActiveFrame();
	//editView_Bottom *pView = (editView_Bottom *) pChild->GetActiveView();*/
	CDocument* pDoc = this;
	if (pDoc==NULL)
		return L"";
	ASSERT(pDoc != NULL);
   
    CView* pView;
    POSITION pos = pDoc->GetFirstViewPosition();
    while (pos != NULL)
    {
        pView = pDoc->GetNextView(pos);
        if (pView->IsKindOf(RUNTIME_CLASS(editView_Bottom)))
        {
            //pView->GetParentFrame()->ActivateFrame();
            break;
        }
	}
	if(pView!=NULL)
		pView->GetWindowText(strText);
	return strText;
}

bool editDoc::Check_Connect() 
{
	CGlobal *pGL=CGlobal::GetInstance();
	if(pGL->m_FileDB.m_pDB->Execute(L"select top 1 * from sys_newsroom")==FALSE)
		return false;
	return true;
}

void editDoc::SetMsg(CString strLockUser) 
{
	CGlobal *pGL = CGlobal::GetInstance();
	FILEPARAM *pFileParam = (FILEPARAM *)&m_FileParam;
//	AfxMessageBox(m_FileParam.szFileCode);
//	CString strLockUser = L"";
//	if (pGL->m_FileDB.IsFileLocked(pFileParam->szFileCode, strLockUser))
//	{
//		AfxMessageBox(L"");
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		if (pFrame!=NULL)
		{
			CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
			if (pChild!= NULL)
			{
				CString strMsg = L"";
				strMsg.Format(L"用戶 %s 正在使用當前稿件，稿件已鎖定，不能提交！", strLockUser);
				((editFrameNew*)pChild)->m_dlgMsg.SetDlgItemText(IDC_STATIC_MSG1,(LPCTSTR)strMsg);
			}
		}
//	}
}