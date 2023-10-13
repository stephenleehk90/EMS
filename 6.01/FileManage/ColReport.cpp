// ColReport.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "ColReport.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColReport dialog


CColReport::CColReport(CWnd* pParent /*=NULL*/)
	: CDialog(CColReport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColReport)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strPreTitle = _T("");
}


void CColReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColReport)
	DDX_Control(pDX, IDC_COLREPORT, m_Viewer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColReport, CDialog)
	//{{AFX_MSG_MAP(CColReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColReport message handlers

BEGIN_EVENTSINK_MAP(CColReport, CDialog)
    //{{AFX_EVENTSINK_MAP(CColReport)
	ON_EVENT(CColReport, IDC_COLREPORT, 408 /* RefreshButtonClicked */, OnRefreshButtonClickedColreport, VTS_PBOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CColReport::OnRefreshButtonClickedColreport(BOOL FAR* UseDefault) 
{
	// TODO: Add your control notification handler code here
	UseDefault = FALSE;
	CDialog::OnOK();
	
}

BOOL CColReport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CString strError, strFileName ;
	
	TCHAR path[100];
	GetModuleFileName(AfxGetInstanceHandle(),path,100);
	CString strPath = path;
	int nLength = strPath.ReverseFind(_T('\\'));
	strFileName = strPath.Left(nLength);
	
	strFileName += _T("\\Reports\\colfile5.rpt");		
	
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
	
	
	m_Report->DiscardSavedData();
	
	IParameterFieldDefinitionPtr spPrara1 = m_Report->ParameterFields->Item[1];
	
	spPrara1->AddCurrentValue(_bstr_t(m_strPreTitle));
	
	m_Report->Database->Tables->GetItem(1)->SetDataSource
		(_variant_t((IDispatch *)(rs->GetRecordset()),true));
	
	m_Report->EnableParameterPrompting = VARIANT_FALSE;
	
	
	
	m_Report->Database->Tables->GetItem(1)->SetDataSource
		(_variant_t((IDispatch *)(rs->GetRecordset()),true));
	
	m_Report->EnableParameterPrompting = VARIANT_FALSE;
	m_Viewer.SetEnableRefreshButton(false);
	
	m_Viewer.SetReportSource(m_Report);
	
	m_Viewer.ViewReport();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
