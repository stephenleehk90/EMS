
// DBGraphicDlg.h : header file
//

#pragma once


// CDBGraphicDlg dialog
class CDBGraphicDlg : public CDialog
{
// Construction
public:
	CDBGraphicDlg(CWnd* pParent = NULL);	// standard constructor
	CString str_Path, str_Password;
// Dialog Data
	enum { IDD = IDD_DBGRAPHIC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
//	afx_msg void OnBnClickedApply();
	afx_msg void OnApply();
	virtual INT_PTR DoModal();
	CString m_pwd;
	CString m_server;
	CString m_user;
};
