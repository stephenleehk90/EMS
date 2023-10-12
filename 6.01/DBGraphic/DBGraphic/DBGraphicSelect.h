#pragma once


// CDBGraphicSelect dialog

class CDBGraphicSelect : public CDialog
{
	DECLARE_DYNAMIC(CDBGraphicSelect)

public:
	CDBGraphicSelect(ADISTFILESTRUCT * pDistFileArr=NULL,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDBGraphicSelect();
	ADISTFILESTRUCT *m_DistFileArr;
	ADISTFILESTRUCT   m_hpdfsDistFile;
	HGLOBAL			m_hglbDistFile;
	friend class CDBGraphicMain;
	BOOL bIsStartUp;
	BOOL bIsCancel;

// Dialog Data
	enum { IDD = IDD_DBGRAPHIC_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	BOOL OnLoadDfalt();
	void AddItem(wchar_t* time, wchar_t* msg);
	void AddItem(LPWSTR lpszCode,LPWSTR lpszMediaType, LPWSTR lpszDpmt,LPWSTR lpszDcode , LPWSTR lpszPath);
	BOOL LoadConfig();
public:
//	CListCtrl m_dblist;
	afx_msg void OnApply();
	afx_msg void OnSelect();
	afx_msg void OnReselect();
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	CListCtrl m_dbList;

};
int toInt(wchar_t *szValue);
BOOL CheckPath(wchar_t * szPath,int & bExist,int & bNetPath);
