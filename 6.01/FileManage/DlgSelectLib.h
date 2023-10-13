#pragma once


// CDlgSelectLib dialog

class CDlgSelectLib : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectLib)

public:
	CDlgSelectLib(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSelectLib();
	CWordArray m_waValidLibIds;

// Dialog Data
	enum { IDD = IDD_SELECTLIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL SetupLibTree(CTreeCtrl *pTreeCtrl, WORD wParent, int nLibID);
	void InsertSubItem(CADORecordset *pRS,CTreeCtrl *pTreeCtrl,HTREEITEM hParent, WORD wParent, int nLibID,HTREEITEM &hSelItem);
	BOOL LoadPlace(int nType, int nLibID, CComboBox *pBox);
	BOOL GetSelInfo(int &nLib, CString &strPlace);
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_FileTree;
	CComboBox m_combPlace;
	CString str_FilePlace;
	int iLibId;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedDeslibtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
