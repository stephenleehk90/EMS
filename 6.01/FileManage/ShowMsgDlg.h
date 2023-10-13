#pragma once


// CShowMsgDlg dialog

class CShowMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowMsgDlg)

public:
	CShowMsgDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShowMsgDlg();

// Dialog Data
	enum { IDD = IDD_MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
