// AttachFiles.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "AttachFiles.h"
#include "../commkits/dirdialog.h"

#include <io.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAttachFiles dialog


CAttachFiles::CAttachFiles(CWnd* pParent /*=NULL*/)
	: CDialog(CAttachFiles::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAttachFiles)
	m_strFileCode = _T("");
	//}}AFX_DATA_INIT
	m_pMgr = NULL;
	m_bModify = FALSE;
}


void CAttachFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAttachFiles)
	DDX_Control(pDX, IDC_FILELIST, m_FileList);
	DDX_Text(pDX, IDC_STATICFILENAME, m_strFileCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAttachFiles, CDialog)
	//{{AFX_MSG_MAP(CAttachFiles)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_DOWNLOAD, OnDownload)
	ON_BN_CLICKED(IDC_UPLOAD, OnUpload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAttachFiles message handlers

void CAttachFiles::OnDelete() 
{
	// TODO: Add your control notification handler code here
	CString strSourceName;
	CString strDestName;
	
	int nRow=-1;
	int nCount;
	nCount=m_FileList.GetSelectedCount();
	for(int i=0;i<nCount;i++)
	{
		nRow=m_FileList.GetNextItem(nRow,LVNI_SELECTED);
		
		CString strAttachName,strUploadDate;
		strAttachName=m_FileList.GetItemText(nRow,0);
		strUploadDate=m_FileList.GetItemText(nRow,3);

		CString strday, strYr, strMon;
		strMon = strUploadDate.Left(strUploadDate.Find('-'));
		strUploadDate.Replace(strMon+L"-",L"" );
		strday = strUploadDate.Left(strUploadDate.Find('-'));
		strUploadDate.Replace(strday+L"-",L"" );
		strYr = strUploadDate;
		strUploadDate = strYr+strMon+strday;
		
		strSourceName=strUploadDate+_T("\\");
		strSourceName+=m_strFileCode;
		strSourceName+=strAttachName;
		if(DeleteFile(strSourceName,strAttachName))
			
			m_pMgr->DeleteAttach(m_strFileCode,strAttachName);
	}
	m_pMgr->LoadAttach(&m_FileList,m_strFileCode);
}

void CAttachFiles::OnDownload() 
{
	// TODO: Add your control notification handler code here
	CString strSourceName;
	CString strDestName;
	CString strAttachName,strUploadDate;
	int nRow=-1;
	int nCount;
	nCount=m_FileList.GetSelectedCount();

	if(nCount>0)
	{
		CString str;//=AfxGetApp()->GetProfileString(_T("Attach"),_T("DownloadPath"),_T("C:\\My Documents"));
		//added by Ronald 20120927 for migrating from vc6 to 2010
		wchar_t sz_temp[255];
		GetPrivateProfileStringW( L"Attach" , L"DownloadPath",L"C:\\My Documents",sz_temp ,255,((CWinApp*)AfxGetApp())->m_pszProfileName);	
		str = sz_temp;
		//end 20120927
		nRow=m_FileList.GetNextItem(nRow,LVNI_SELECTED);
		strAttachName=m_FileList.GetItemText(nRow,0);
		CDirDialog dlg;
		CResManage res(g_hInst);
		dlg.m_strTitle.LoadString(IDS_SELECTSAVEPATH);
		dlg.m_strSelDir = str;
		if (dlg.DoBrowse(this) == IDOK)
		{
			strDestName=dlg.m_strPath;
			//edit by Ronald 20120927 for migrating from vc6 to 2010
			WritePrivateProfileStringW(L"Attach", L"DownloadPath", strDestName.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
			//AfxGetApp()->WriteProfileString(_T("Attach"),_T("DownloadPath"),strDestName);
			//end 20120927
		}	
		else
			return;
	}
	nRow=-1;
	for(int i=0;i<nCount;i++)
	{
		nRow=m_FileList.GetNextItem(nRow,LVNI_SELECTED);	
		
		strAttachName=m_FileList.GetItemText(nRow,0);
		strUploadDate=m_FileList.GetItemText(nRow,3);
		
		COleDateTimeEx dt(strUploadDate,DATEFORMAT);
		strSourceName=dt.Format(_T("%Y%m%d"))+_T("\\");
		strSourceName+=m_strFileCode;
		strSourceName+=strAttachName;

		CString s1;
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		//s1=AfxGetApp()->GetProfileString(_T("Attach"),_T("downloadpath"),_T(""));
		wchar_t sz_temp[255];
		GetPrivateProfileStringW( L"Attach" , L"downloadpath",L"",sz_temp ,255,((CWinApp*)AfxGetApp())->m_pszProfileName);	
		s1 = sz_temp;
		//end 20120927

		CFileDialog dlg(false,_T(""),strAttachName);
		dlg.m_ofn.lpstrInitialDir=(LPCTSTR)s1;

		//Pat 20150709 for show ws alert----------------------------------------

		CGlobal::GetInstance()->clear_ws_alert_msg();

		//----------------------------------------------------------------------

		if(DownLoad(strSourceName,strDestName+_T("\\")+strAttachName))
			AfxMessageBox(IDS_DOWNLOADSUC);
		else
			AfxMessageBox(IDS_DOWNLOADFAIL);
		//Pat 20150709 for show ws alert----------------------------------------

		CGlobal::GetInstance()->show_ws_alert_msg();

		//----------------------------------------------------------------------

	}
}

void CAttachFiles::OnUpload() 
{
	// TODO: Add your control notification handler code here
	CString strSourceName;
	CString strDestName;
	CAttach cAttach;
	cAttach.strFileCode = m_strFileCode;
	cAttach.strUser = CGlobal::GetInstance()->m_pEmployee->m_strUserCode;

	COleDateTime date;
	date = COleDateTime::GetCurrentTime();
	cAttach.strUploadDate=date.Format(_T("%Y%m%d"));
	strDestName=cAttach.strUploadDate+_T("\\");

	CString str;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	//str=AfxGetApp()->GetProfileString(_T("Attach"),_T("uploadpath"),_T(""));
	wchar_t sz_temp[255];
	GetPrivateProfileStringW( L"Attach" , L"uploadpath",L"",sz_temp ,255,((CWinApp*)AfxGetApp())->m_pszProfileName);	
	str = sz_temp;
	//end 20120927
	CFileDialog dlg(true,_T(""),_T(""));
	dlg.m_ofn.lpstrInitialDir=(LPCTSTR)str;
	if(dlg.DoModal()==IDOK)
	{		
		strSourceName=dlg.GetPathName();
		strDestName+=m_strFileCode;
		cAttach.strName=dlg.GetFileName();
		strDestName+=dlg.GetFileName();
		
		str=strSourceName.Left(strSourceName.ReverseFind(_T('\\')));
		//edit by Ronald 20120927 for migrating from vc6 to 2010
		WritePrivateProfileStringW(L"Attach", L"uploadpath", str.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
		//AfxGetApp()->WriteProfileString(_T("Attach"),_T("uploadpath"),str);
		//end 20120927
		if( _taccess(strSourceName, 0 ) == -1 )
		{
			str.Format(IDS_FILENOTEXIST,strSourceName);
			AfxMessageBox(IDS_FILENOTEXIST);
			return;
		}
		CFile fSource;
		CFileStatus FileStatus;			
		fSource.GetStatus((LPCTSTR)strSourceName,FileStatus);
	
		cAttach.nSize=FileStatus.m_size;
		// Add by Bob 2009/9/23
		// 限定Attachment的大小,從config中讀取AttachSize
		//    因為明報版本限定了2M, 所以不能直接改成超過2M
		//    信報需要將該限制放寬到10M,
		int nAttachSize;
		CString strAttachSize;
		m_pMgr->m_pFileDB->GetConfig(_T("AttachSize"),strAttachSize);
		if (strAttachSize.IsEmpty() || strAttachSize == _T(""))
			strAttachSize = _T("2");
		nAttachSize = _ttoi(strAttachSize);

		//if(cAttach.nSize>(1024*1024*2))
		if(cAttach.nSize>(1024*1024*nAttachSize))
		{
			str.Format(IDS_ATTACHSIZETOOLARGE,nAttachSize);
			AfxMessageBox(str);
			return;
		}

		int nSize;
		m_pMgr->m_pFileDB->GetConfig(_T("AttachTotalSize"),strAttachSize);
		if (strAttachSize.IsEmpty() || strAttachSize == _T(""))
			strAttachSize = _T("10");
		nAttachSize = _ttoi(strAttachSize);

        if((m_pMgr->GetAttachTotalSize(m_strFileCode,nSize))&&(nSize+cAttach.nSize)>(1024*1024*nAttachSize))
		{			
			str.Format(IDS_ATTACHTOTALSIZEERR,nAttachSize);
			AfxMessageBox(str);
			return;			
		}
		cAttach.strType=strDestName.Right(strDestName.GetLength()-strDestName.ReverseFind('.')-1);	
		
		CString strOldUploadDate;
		CString strOldSourceName;
		if(m_pMgr->ExistAttach(m_strFileCode,cAttach.strName,strOldUploadDate))
		{
			
			strOldSourceName.Format(_T("%s\\%s%s"),strOldUploadDate,m_strFileCode,cAttach.strName);
			str.Format(IDS_ATTACHEXIST,cAttach.strName);
			if(AfxMessageBox(str,MB_OKCANCEL)==IDOK)
			{
				DeleteFile(strOldSourceName,"");
				//Pat 20150709 for show ws alert----------------------------------------

				CGlobal::GetInstance()->clear_ws_alert_msg();

				//----------------------------------------------------------------------
				if(UpLoad(strSourceName,strDestName)&&m_pMgr->InsertAttach(&cAttach))
					;		
				else
					AfxMessageBox(IDS_UPLOADFAIL);
				//Pat 20150709 for show ws alert----------------------------------------

				CGlobal::GetInstance()->show_ws_alert_msg();

				//----------------------------------------------------------------------
			}
		}
		else
		{
			//Pat 20150709 for show ws alert----------------------------------------

			CGlobal::GetInstance()->clear_ws_alert_msg();

			//----------------------------------------------------------------------
			if(UpLoad(strSourceName,strDestName)&&m_pMgr->InsertAttach(&cAttach))
				;		
			else
				AfxMessageBox(IDS_UPLOADFAIL);				
			//Pat 20150709 for show ws alert----------------------------------------

			CGlobal::GetInstance()->show_ws_alert_msg();

			//----------------------------------------------------------------------
		}
		m_pMgr->LoadAttach(&m_FileList,m_strFileCode);
	}
}

BOOL CAttachFiles::OnInitDialog() 
{
	m_FileList.SubclassCtrl( IDC_FILELIST, this);
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitList();
	m_pMgr->LoadAttach(&m_FileList,m_strFileCode);
	GetDlgItem(IDC_UPLOAD)->EnableWindow(m_bModify);
	GetDlgItem(IDC_DELETE)->EnableWindow(m_bModify);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAttachFiles::InitList()
{
	CRect			rect;
	int				i;
	LV_COLUMN		lvcolumn;
	CString 		rgtsz[5];
	int				rgtcx[5] = { 12,3,5,6,6};

	CResManage res(g_hInst);
	rgtsz[0].LoadString(IDS_ATTACHNAME);
	rgtsz[1].LoadString(IDS_FILEFORMAT);
	rgtsz[2].LoadString(IDS_ATTACHSIZE);
	rgtsz[3].LoadString(IDS_UPLOADDATE);
	rgtsz[4].LoadString(IDS_UPLOADUSER);
    m_FileList.GetWindowRect(&rect);
	
	// Insert ListCtrl Columns
	for (i = 0; i < 5; i++)  // add the columns to the list control
	{
		lvcolumn.mask =LVIF_STATE |LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		
		if(i==2)
			lvcolumn.fmt = LVCFMT_RIGHT;
		else
			lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.pszText = (LPTSTR)(LPCTSTR)rgtsz[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = rect.Width() * rgtcx[i] /32;  // SubItem is twice as large
		m_FileList.InsertColumn(i, &lvcolumn);  // assumes return value is OK.
	}
}

BOOL CAttachFiles::DeleteFile(CString strFileName, CString strAttachName)
{
	strFileName.Format(_T("%s\\%s"),CGlobal::GetInstance()->m_pEntry->strAttachPath,strFileName);
//	AfxMessageBox(strFileName);

	//if( _taccess(strFileName, 0 ) == -1)
	if( _waccess(strFileName, 0 ) == -1)
	{
		return TRUE;	
	}

	TRY
	{
		CFile fSource;			
		CFileStatus FileStatus;			
		fSource.GetStatus((LPCTSTR)strFileName,FileStatus);
		FileStatus.m_attribute=0x00;
		fSource.SetStatus((LPCTSTR)strFileName,FileStatus);
		//_t
		CGlobal::GetInstance()->delete_file(strFileName,CGlobal::GetInstance()->m_pShareData->m_Employee.m_strUserCode,CGlobal::GetInstance()->m_pShareData->str_company,L"_t");
	}
	CATCH( CFileException, e )
	{
		return false;
	}
	END_CATCH
	return true;
}

BOOL CAttachFiles::UpLoad(CString &strSourceName, CString strDestName)
{
	strDestName.Format(_T("%s\\%s"),CGlobal::GetInstance()->m_pEntry->strAttachPath,strDestName);

	if(CGlobal::GetInstance()->upload_file(strSourceName,strDestName,CGlobal::GetInstance()->m_pShareData->m_Employee.m_strUserCode,CGlobal::GetInstance()->m_pShareData->str_company,false))
	{
		return true;
	}
	else
	{	
		CString strDir;
		strDir=strDestName.Left(strDestName.ReverseFind(_T('\\')));		
		if(CreateDir(strDir))
			if(CGlobal::GetInstance()->upload_file(strSourceName,strDestName,CGlobal::GetInstance()->m_pShareData->m_Employee.m_strUserCode,CGlobal::GetInstance()->m_pShareData->str_company,false))
				return true;
		return false;
	}
	return true;
}

BOOL CAttachFiles::CreateDir(const CString &strDir)
{
	CString strParentDir;	
	if( (_taccess(strDir, 0 )) == -1 )
		;
	else
		return true;
	
	strParentDir=strDir.Left(strDir.ReverseFind(_T('\\')));
	if(!CreateDirectory(strDir,NULL))
	{		
		CreateDir(strParentDir);
		CreateDirectory(strDir,NULL);
		return true;
	}
	else
	{			
		return true;
	}		
}

BOOL CAttachFiles::DownLoad(CString strSourceName, CString &strDestName)
{
	strSourceName.Format(_T("%s\\%s"),CGlobal::GetInstance()->m_pEntry->strAttachPath,strSourceName);
	CString str;
	if(_taccess(strDestName,0)!=-1)
	{
		str.Format(IDS_ATTACHEXIST,strDestName);
		if(AfxMessageBox(str,MB_OKCANCEL)==IDOK)
		{
			CFile fSource;			
			CFileStatus FileStatus;			
			fSource.GetStatus((LPCTSTR)strDestName,FileStatus);
			FileStatus.m_attribute=0x00;
			fSource.SetStatus((LPCTSTR)strDestName,FileStatus);
		}
		else
			return true;
	}

	if(_taccess(strSourceName,0)==-1)
	{
		str.Format(IDS_FILENOTEXIST,strDestName.Right(strDestName.GetLength()-strDestName.ReverseFind('\\')-1));
		AfxMessageBox(str,MB_OK);
		return false;
	}

	if(CGlobal::GetInstance()->upload_file(strSourceName,strDestName,CGlobal::GetInstance()->m_pShareData->m_Employee.m_strUserCode,CGlobal::GetInstance()->m_pShareData->str_company,false))
	{
		return true;
	}
	else
	{	
		DWORD err=GetLastError();
		CString strDir;
		strDir=strDestName.Left(strDestName.ReverseFind(_T('\\')));		
		if(CreateDir(strDir))
			if(CGlobal::GetInstance()->upload_file(strSourceName,strDestName,CGlobal::GetInstance()->m_pShareData->m_Employee.m_strUserCode,CGlobal::GetInstance()->m_pShareData->str_company,false))
				return true;
		return false;
	}
	return true;
}

BOOL CAttachFiles::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			OnCancel();
	}
    return CDialog::PreTranslateMessage(pMsg);
}