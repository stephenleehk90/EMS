// RecordReport.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "RecordReport.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordReport dialog


CRecordReport::CRecordReport(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordReport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordReport)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRecordReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordReport)
	DDX_Control(pDX, IDC_RECORDREPORT, m_Viewer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecordReport, CDialog)
	//{{AFX_MSG_MAP(CRecordReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordReport message handlers

BEGIN_EVENTSINK_MAP(CRecordReport, CDialog)
    //{{AFX_EVENTSINK_MAP(CRecordReport)
	ON_EVENT(CRecordReport, IDC_RECORDREPORT, 408 /* RefreshButtonClicked */, OnRefreshButtonClickedRecordreport, VTS_PBOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CRecordReport::OnRefreshButtonClickedRecordreport(BOOL FAR* UseDefault) 
{
	// TODO: Add your control notification handler code here

	UseDefault = FALSE;

	CDialog::OnOK();
	
}

BOOL CRecordReport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		CString strError, strFileName ;
		
	TCHAR path[100];
	GetModuleFileName(AfxGetInstanceHandle(),path,100);
	CString strPath = path;
	int nLength = strPath.ReverseFind(_T('\\'));
	strFileName = strPath.Left(nLength);
	
	strFileName += _T("\\Reports\\record5.rpt");	
		
	if( (_taccess(strFileName, 0 )) == -1 )
	{
		strError.Format(IDS_FILENOTEXIST, strFileName);		
		AfxMessageBox(strError);		
		EndDialog(0);		
		return FALSE;
	}
	
	ShowWindow(SW_SHOWMAXIMIZED);
	
	if (m_Viewer.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
		m_Viewer.MoveWindow(rect);
	}

	IApplicationPtr m_Application;
	IReportPtr		m_Report;
	VARIANT			dummy;
	
	VariantInit (&dummy);
	dummy.vt = VT_EMPTY;
	HRESULT hr = S_OK;

	CGlobal *pGlobal = CGlobal::GetInstance();	

	m_Application = NULL;
	m_Report = NULL;

	CLSID CLSID_Application;
	CLSIDFromProgID(L"CrystalRuntime.Application", &CLSID_Application);
	
	_bstr_t ReportPath(strFileName);
	
	hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_INPROC_SERVER , __uuidof(IApplication), (void **) &m_Application);
	
	m_Report = m_Application->OpenReport(ReportPath, dummy);
	
	INameValuePairsPtr  sp_ConnInfo = m_Report->Database->Tables->GetItem(1)->ConnectionProperties;
	
	m_Report->Database->Tables->GetItem(1)->DllName = _bstr_t("crdb_ado.dll");
	
	sp_ConnInfo->DeleteAll();

	
	sp_ConnInfo->Add(_bstr_t("Provider"), _bstr_t("SQLOLEDB"));
	sp_ConnInfo->Add(_bstr_t("Data Source"), _bstr_t(pGlobal->m_pEmployee->m_strServerName));
	
	sp_ConnInfo->Add(_bstr_t("Initial Catalog"), _bstr_t(pGlobal->m_pEmployee->m_strDatabaseName));
	
	sp_ConnInfo->Add(_bstr_t("User ID"), _bstr_t(pGlobal->m_pEmployee->m_strLoginCode));
	sp_ConnInfo->Add(_bstr_t("Password"), _bstr_t(pGlobal->m_pEmployee->m_strPassword));

	m_Report->DiscardSavedData();

	IParameterFieldDefinitionPtr spPrara1 = m_Report->ParameterFields->Item[1];
	
	_bstr_t _str("'" + m_strFileCode + "'");
	spPrara1->AddCurrentValue(_str);
	

	m_Report->EnableParameterPrompting = VARIANT_FALSE;
	
	m_Viewer.SetReportSource(m_Report);

	m_Viewer.ViewReport();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
