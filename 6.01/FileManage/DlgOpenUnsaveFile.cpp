// DlgOpenUnsaveFile.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DlgOpenUnsaveFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOpenUnsaveFile dialog


CDlgOpenUnsaveFile::CDlgOpenUnsaveFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOpenUnsaveFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOpenUnsaveFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgOpenUnsaveFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOpenUnsaveFile)
	DDX_Control(pDX, IDC_FILELIST, m_lstFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOpenUnsaveFile, CDialog)
	//{{AFX_MSG_MAP(CDlgOpenUnsaveFile)
	ON_BN_CLICKED(ID_DEL, OnDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOpenUnsaveFile message handlers

BOOL CDlgOpenUnsaveFile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	str.LoadString(IDS_FILENAME);
	m_lstFile.InsertColumn(0,str,LVCFMT_LEFT,150);
	ListView_SetExtendedListViewStyle(m_lstFile.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);	
	for(int i=0;i<m_arrFile.GetSize();i++)
		m_lstFile.InsertItem(i,m_arrFile.GetAt(i).strFileName);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOpenUnsaveFile::OnOK() 
{
	// TODO: Add extra validation here
	//m_arrFile.RemoveAll();
	int nCount=m_lstFile.GetItemCount();
	BOOL bSel=FALSE;
	for(int i=0;i<nCount;i++)
	{
		if(m_lstFile.GetCheck(i))
		{
			UNSAVEFILE usf=m_arrFile.GetAt(i);
			usf.bNeedOpen = TRUE;
			m_arrFile.SetAt(i,usf);
			bSel = TRUE;
		}
	}
	if(!bSel)
		return;

	CDialog::OnOK();
}

void CDlgOpenUnsaveFile::OnDel() 
{
	// TODO: Add your control notification handler code here
	int nCount=m_lstFile.GetItemCount();
	BOOL bSel=FALSE;
	int i;
	for( i=nCount-1;i>=0;i--)
	{
		if(m_lstFile.GetCheck(i))
		{
			bSel=TRUE;
			break;
		}
	}
	if(!bSel)
	{
		AfxMessageBox(IDS_NOTSELITEM);
		return;
	}
	if(AfxMessageBox(IDS_DELUNSAVEFILE,MB_YESNO)==IDNO)
		return;
	
	
	
	for( i=nCount-1;i>=0;i--)
	{
		if(m_lstFile.GetCheck(i))
		{
			UNSAVEFILE usf=	m_arrFile.GetAt(i);
			TRY{
				CFile::Remove(m_strPath+_T("\\")+usf.strFileName+_T(".")+EDITBAK);
			}
			CATCH (CFileException, e) {
				TCHAR szMsg[256];
				e->GetErrorMessage(szMsg,255);
				AfxMessageBox(szMsg);
				return;
			}
			END_CATCH
			m_lstFile.DeleteItem(i);
			m_arrFile.RemoveAt(i);
		}
	}
}
