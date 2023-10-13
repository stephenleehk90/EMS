#if !defined(AFX_TRANSDLG_H__8044D972_AAD3_4FCD_B3F5_7441A2F71B65__INCLUDED_)
#define AFX_TRANSDLG_H__8044D972_AAD3_4FCD_B3F5_7441A2F71B65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransDlg.h : header file
//
#include "dofilemgr.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CTransDlg dialog

class CTransDlg : public CDialog
{
// Construction
public:
	BOOL m_bNewFile;
	int m_nLibID;
	CString m_strLastModifyTime;
	BOOL GetSelInfo(int &nLib,CString &strPlace);
	CTransDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTransDlg)
	enum { IDD = IDD_TRANSDLG };
	CButton	m_checkUser;
	CComboBox	m_combUser;
	CComboBox	m_combPlace;
	CTreeCtrl	m_FileTree;
	BOOL	m_bCopy;
	BOOL	m_bSecret;
	BOOL	m_bUser;
	CString	m_strFileCode;
	CString	m_strNote;
	CString	m_strTitle;
	CString	m_strPlace;
	BOOL	m_bExclusive;
	BOOL	m_bLayout;
	CComboBox	m_combLayout;
	COleDateTime	m_dtLayout;
	//}}AFX_DATA
	CString m_strPlaceName;
private:
	void InsertSubItem(CADORecordset *pRS,CTreeCtrl *pTreeCtrl,HTREEITEM hParent, WORD wParent, int nLibID,HTREEITEM &hSelItem);
	BOOL SetupLibTree(CTreeCtrl *pTreeCtrl, WORD wParent,int nLibID);
	BOOL LoadPlace(int nType, int nLibID, CComboBox *pBox);
	BOOL GetLibUser(int nLib, CString strPlace, CComboBox *pBox);
	void LinkToDM(LPCTSTR lpszFileCode);

	BOOL TransFile(CString strFileCode, int nDesLibID, CString strDesPlace,
		CString strCopyStyle, int nGroupID, CString strUserCode, short nSecret, CString *strNote, CString strOper, int nAction, short &nRet);
	BOOL GetLayoutData(CComboBox *pBox);
	void UpdataFileTitle();

		
	bool m_bAlert;	// 警告模塊是否存在
	bool m_bDM;		// 部門分組菜單模塊是否存在　
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTransDlg)
	virtual void OnOK();
	afx_msg void OnSelchangePlacecombo();
	afx_msg void OnSelchangedDeslibtree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnUsercheck();
	afx_msg void OnBtnlink();
	afx_msg void OnLayoutcheck();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSDLG_H__8044D972_AAD3_4FCD_B3F5_7441A2F71B65__INCLUDED_)
