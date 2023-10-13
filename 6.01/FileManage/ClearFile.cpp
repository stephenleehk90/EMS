// ClearFile.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "ClearFile.h"
#include "..\commkits\dostools.h"
#include <fstream>
#include <iostream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClearFile dialog


CClearFile::CClearFile(CWnd* pParent /*=NULL*/)
	: CDialog(CClearFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClearFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CClearFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClearFile)
	DDX_Control(pDX, IDC_FILE, m_lcFile);
	DDX_Control(pDX, IDC_DATE, m_lbDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClearFile, CDialog)
	//{{AFX_MSG_MAP(CClearFile)
	ON_BN_CLICKED(IDC_DELDIR, OnDeldir)
	ON_LBN_SELCHANGE(IDC_DATE, OnSelchangeDate)
	ON_BN_CLICKED(IDC_DELFILE, OnDelfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClearFile message handlers

BOOL CClearFile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	
	DWORD dwstyle=m_lcFile.GetExtendedStyle();
	dwstyle &=~(LVS_EX_ONECLICKACTIVATE );
	dwstyle |= LVS_EX_FULLROWSELECT;
	m_lcFile.SetExtendedStyle(dwstyle);
	m_lcFile.DeleteAllItems();

	ListDate();

	InitList();

	OnSelchangeDate();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CClearFile::ListDate()
{
	m_lbDate.ResetContent();
	CGlobal *pGL = CGlobal::GetInstance();
	
	CString strLoginPath;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	//strLoginPath = AfxGetApp()->GetProfileString (_T("System"),_T("TempPath"),
	//	_T("c:\\founder" )) + "\\" + pGL->m_pEmployee->m_strLoginCode;
	wchar_t sz_temp[255];
	GetPrivateProfileStringW( L"System" , L"TempPath",L"c:\\founder",sz_temp ,255,((CWinApp*)AfxGetApp())->m_pszProfileName);	
	strLoginPath.Format(L"%s%s%s", sz_temp,"\\" ,pGL->m_pEmployee->m_strLoginCode);
	//end 20120927
	
	CFileFind find;
	BOOL bFind;
	bFind = (BOOL)find.FindFile(strLoginPath + "\\*.*");	
	while (bFind)
	{
		bFind = (BOOL)find.FindNextFile();
		if (!find.IsDots())
		{
			if (find.IsDirectory())
			{
				m_lbDate.AddString(find.GetFileName());
			}
		}		
	}
	find.Close();

	m_lbDate.SetCurSel(0);


}

void CClearFile::OnDeldir() 
{
	// TODO: Add your control notification handler code here

	int nSel;
	nSel = m_lbDate.GetCurSel();
	if (nSel < 0) 
		return;
	
	if (AfxMessageBox(IDS_DELETE,MB_YESNO) != IDYES)
		return;
	CString strDate;
	m_lbDate.GetText(nSel,strDate);
	
	CGlobal *pGL = CGlobal::GetInstance();
	
	CString strLoginPath;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	//strLoginPath = AfxGetApp()->GetProfileString (_T("System"),
	//	_T("TempPath") , _T("c:\\founder" )) + "\\" + 
	//	pGL->m_pEmployee->m_strLoginCode;
	wchar_t sz_temp[255];
	GetPrivateProfileStringW( L"System" , L"TempPath",L"c:\\founder",sz_temp ,255,((CWinApp*)AfxGetApp())->m_pszProfileName);	
	strLoginPath.Format(L"%s%s%s",sz_temp , "\\" ,pGL->m_pEmployee->m_strLoginCode);
	//end 20120927


	CString strPath;
	strPath = strLoginPath + "\\" + strDate;

	TCHAR szDir[100];
	lstrcpy(szDir,strPath);

	CFileFind fnd;
	BOOL bfnd=fnd.FindFile(strPath+CString("\\*.*"));  //write your file extension
	while(bfnd)
	{
		bfnd=fnd.FindNextFile();
		Win32DeleteFile(fnd.GetFilePath());
	}
	//DeleteDirectory((char*)szDir,0);
	//remove(szDir);
	 RemoveDirectory(strPath);
	_wremove(szDir);

	ListDate();
	
	OnSelchangeDate();
	m_lbDate.DeleteString(nSel);
}

void CClearFile::InitList()
{
	CRect			rect;
	m_lcFile.GetWindowRect(&rect);
	
	LV_COLUMN		lvcolumn;
		
	CString strItem;
	TCHAR szItem[100];
	

	lvcolumn.iSubItem = 0;
	strItem.LoadString(IDS_FILEITEM0);
//	strcpy(szItem,strItem);
	lstrcpy(szItem,strItem);
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = szItem;
	lvcolumn.cx = rect.Width() * 20 /100;
	m_lcFile.InsertColumn(0,&lvcolumn);

	lvcolumn.iSubItem = 1;
	strItem.LoadString(IDS_FILEITEM1);
//	strcpy(szItem,strItem);
	lstrcpy(szItem,strItem);
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = szItem;
	lvcolumn.cx = rect.Width() * 56 /100;
	m_lcFile.InsertColumn(1,&lvcolumn);

	lvcolumn.iSubItem = 2;
	strItem.LoadString(IDS_FILEITEM2);
//	strcpy(szItem,strItem);
	lstrcpy(szItem,strItem);
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_RIGHT;
	lvcolumn.pszText = szItem;
	lvcolumn.cx = rect.Width() * 20 /100;
	m_lcFile.InsertColumn(2,&lvcolumn);


	
}

void CClearFile::OnSelchangeDate() 
{
	// TODO: Add your control notification handler code here
	m_lcFile.DeleteAllItems();	
	
	CFileFind find;
	BOOL bFind;
	int nSel = m_lbDate.GetCurSel();
	int nRow;
	nRow = 0;

	if (nSel < 0)
		return;

	CGlobal *pGL = CGlobal::GetInstance();
	
	CString strLoginPath;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	//strLoginPath = AfxGetApp()->GetProfileString (_T("System"),_T("TempPath"),_T("c:\\founder" ))
	//	+ "\\" + pGL->m_pEmployee->m_strLoginCode;	
	wchar_t sz_temp[255];
	GetPrivateProfileStringW( L"System" , L"TempPath",L"c:\\founder",sz_temp ,255,((CWinApp*)AfxGetApp())->m_pszProfileName);	
	strLoginPath = sz_temp;
	//end 20120927
	CString strDate;
	m_lbDate.GetText(nSel,strDate);
	CString strPath;
	//strPath = strLoginPath + "\\" + strDate + "\\"; 
	strPath = strLoginPath +"\\"+ pGL->m_pEmployee->m_strLoginCode+ "\\" + strDate + "\\"; 

	bFind = (BOOL)find.FindFile(strPath + "*.*");
	
	while (bFind)
	{
		bFind = (BOOL)find.FindNextFile();
		if (!find.IsDots())
		{
			if (!find.IsDirectory())
			{
				CString strFileName;
				strFileName = find.GetFileName();				
				
				//if (strFileName.Right(4) == _T(".FMT"))
				if (strFileName.Right(4).CompareNoCase(L".FMT")||strFileName.Right(4).CompareNoCase(L".TXT"))
				{
					CString strFile;
					strFile = strFileName.Left(strFileName.GetLength()-4);
					
					LV_ITEM lvi;

					TCHAR szItem[100];
//					strcpy(szItem,strFile);
					lstrcpy(szItem,strFile);
					lvi.mask = LVIF_TEXT;
					
					lvi.iItem = nRow;
					lvi.iSubItem = 0;
					lvi.pszText = szItem;
					nRow = m_lcFile.InsertItem(&lvi);

					long lWord = 0;
					CString strTitle;
					CString strTextFile;
					TCHAR szTextFile[100];
					strTextFile = strPath + strFile + _T(".TXT");
//					strcpy(szTextFile,strTextFile);
					lstrcpy(szTextFile,strTextFile);
					//strTitle = GetFileTitleWord((char*)szTextFile,1,&lWord);
					strTitle = GetFileTitle(szTextFile);
					lWord = GetFileWordCount(szTextFile);
//					strcpy(szItem,strTitle);
					lstrcpy(szItem,strTitle);
					
					lvi.iItem = nRow;
					lvi.iSubItem = 1;
					lvi.pszText = szItem;
					m_lcFile.SetItem(&lvi);

					//ltoa(lWord,szItem,10);
					_ltow(lWord,szItem,10);
					lvi.iItem = nRow;
					lvi.iSubItem = 2;
					lvi.pszText = szItem;
					m_lcFile.SetItem(&lvi);					
					
				}
								
			}
		}
	}

	find.Close();
}

void CClearFile::OnDelfile() 
{

	// TODO: Add your control notification handler code here
	CGlobal *pGL = CGlobal::GetInstance();
	
	CString strLoginPath;
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	/*strLoginPath = AfxGetApp()->GetProfileString (_T("System"),
		_T("TempPath") , _T("c:\\founder" )) + "\\" + 
		pGL->m_pEmployee->m_strLoginCode;*/
	wchar_t sz_temp[255];
	GetPrivateProfileStringW( L"System" , L"TempPath",L"c:\\founder",sz_temp ,255,((CWinApp*)AfxGetApp())->m_pszProfileName);	
	strLoginPath.Format(L"%s%s%s",sz_temp,"\\",pGL->m_pEmployee->m_strLoginCode);
	//end 20120927
	
	
	int nSel;
	nSel = m_lbDate.GetCurSel();
	if (nSel < 0) 
		return;
	CString strDate;
	m_lbDate.GetText(nSel,strDate);

	int nCount=m_lcFile.GetSelectedCount();
	if(nCount==0)
		return;

	if (AfxMessageBox(IDS_DELETE,MB_YESNO) != IDYES)
		return;
	CString strFileName;
	int nItem = 0;
	for(int i=0;i<nCount;i++)
	{		
		nItem = m_lcFile.GetNextItem(nItem-1,LVNI_SELECTED);
		TCHAR szFileName[100];
		m_lcFile.GetItemText(nItem,0 ,szFileName,50);
		strFileName = szFileName;
		CString strDelName = strLoginPath + "\\" + strDate + 
			//"\\EDPASave\\" + strFileName + ".FMT";  
			"\\" + strFileName + ".FMT";  

		DWORD fileAtt = GetFileAttributesW(strDelName);
		if(fileAtt != INVALID_FILE_ATTRIBUTES)
			CFile::Remove(strDelName);
		

		strDelName = strLoginPath + "\\" + strDate + 
			//"\\EDPASave\\" + strFileName + ".TXT";  
			"\\" + strFileName + ".TXT";  
		fileAtt = GetFileAttributesW(strDelName);
		if(fileAtt != INVALID_FILE_ATTRIBUTES)
			CFile::Remove(strDelName);

		m_lcFile.DeleteItem(nItem);
	}
}

long CClearFile::GetFileWordCount(CString szSrcFile) 
{
	CString str_Title;
	CFile  fil ;
	long lWord = 0;
	BOOL isUnicode = FALSE;
	if ( fil.Open( szSrcFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) == 0 )
    {		
		return 0 ;
    }
	
	WORD wSignature;
	fil.Read(&wSignature, 2);
	if(wSignature == 0xFEFF || wSignature == 0xFFFE )
	{
		lWord = fil.GetLength()/2;
		lWord -= 1;//for unicode signature 
		isUnicode=TRUE;
	}else
		lWord = fil.GetLength();
	
	fil.Close();
	
	return lWord;
}

CString CClearFile::GetFileTitle(CString szSrcFile) 
{
	CString str_Title;
	CFile  fil ;
	BOOL isUnicode = FALSE;
	if ( fil.Open( szSrcFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) == 0 )
    {		
		return str_Title ;
    }
	
	WORD wSignature;
	fil.Read(&wSignature, 2);
	if(wSignature == 0xFEFF || wSignature == 0xFFFE )
	{
		isUnicode=TRUE;
	}
	
	fil.Close();
	int i_Counter = 0;
	if(!isUnicode)
	{
		wstring s_textTitle;
		/*char* szStr ;
		wcstombs(szStr, str, str.GetLength()*2);*/
		const wchar_t * file = szSrcFile.AllocSysString();//
		wfstream ifs(file, wfstream::in|wfstream::binary);
		if (ifs.good())   { 
			while (!ifs.eof()) {
				getline(ifs, s_textTitle);
				i_Counter++;
				if(i_Counter == 1)
				{
					break;
				}
			}
			ifs.close();
		}	
		//lstrcpy((LPWSTR)szTitle, s_textTitle.c_str());
		str_Title.Format(L"%s",s_textTitle);
		//end 20120917
	}
	else
	{
		CString aString;
		int pos = 0;
		if ( fil.Open( szSrcFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) == 0 )
		{		
			return str_Title ;
		}
		WORD buffer;
		int bufferCount = 0;
		aString.Empty();
		int  bytelen = 0;
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
				//szUniTitle = (char*)(LPCTSTR)aString;
				str_Title = aString;
			}
			else
			{
				if(buffer == 65279)
					continue;
				aString += CString((WCHAR)buffer);
			}
		}
		pos = fil.GetPosition();
		if(bufferCount==0)
			str_Title = aString;
		/*CString str; str.Format();
		AfxMessageBox(szUniTitle);*/
		if (aString.IsEmpty()&&bytelen==0)
		{
		}
	}
	if(str_Title.GetLength()>50)
		str_Title = str_Title.Left(47)+L"...";
	return str_Title;
}

BOOL CClearFile::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			OnCancel();
	}
    return CDialog::PreTranslateMessage(pMsg);
}