// SaveProg.cpp : implementation file
//

#include "stdafx.h"
#include "SaveProg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveProg dialog


CSaveProg::CSaveProg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveProg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveProg)
	m_status = _T("");
	//}}AFX_DATA_INIT
}


void CSaveProg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveProg)
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Text(pDX, IDC_STATUS, m_status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveProg, CDialog)
	//{{AFX_MSG_MAP(CSaveProg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
CDoSaveProg * CDoSaveProg::pInstance = NULL;
CDoSaveProg::CDoSaveProg()
{
	pDlg = NULL;
	pDlg = new CSaveProg;
	pDlg->Create(IDD_PROGRESS,NULL);
	pInstance = NULL;
}

CDoSaveProg::~CDoSaveProg()
{
	if(pDlg)
	{
		TRACE(L"Delete SaveProgress dialog");
		delete pDlg;
		pDlg = NULL;
	}
	pInstance = NULL;
}
CDoSaveProg * CDoSaveProg::Instance()
{
	if(pInstance==NULL)
	{
		pInstance = new CDoSaveProg;
	}
	return pInstance;
}
void CDoSaveProg::ShowDialog( int nCmdShow )
{
	if(pDlg)
		pDlg->ShowWindow( nCmdShow );
}
void CDoSaveProg::SetStatus(const CString &str)
{
	if(pDlg)
		pDlg->SetStatus(str);
}
void CDoSaveProg::SetProgress(int nProgress)
{
	if(pDlg)
		pDlg->SetProgress(nProgress);
}

void CDoSaveProg::Destroy()
{
	if(pInstance)
		delete pInstance;
}
/////////////////////////////////////////////////////////////////////////////
// CSaveProg message handlers

BOOL CSaveProg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Progress.SetRange(0,10);
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSaveProg::SetStatus(const CString &str)
{
	m_status = str;
	UpdateData(FALSE);
}

void CSaveProg::SetProgress(int nProgress)
{
	m_Progress.SetPos(nProgress);
	if(nProgress==10)
		ShowWindow(SW_HIDE);
}


void CSaveProg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	ShowWindow(SW_HIDE);
}
