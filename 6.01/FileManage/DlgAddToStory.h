#pragma once
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
#include "atlcomtime.h"
#include "afxdtctl.h"


// CDlgAddToStory dialog

class CDlgAddToStory : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddToStory)

public:
	CDlgAddToStory(CADODatabase *pDB, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddToStory();
	CString strStoryName, strFileName, m_strDate, m_strFilePath;

	CStringArray m_arrFileName;

	COleDateTime *m_dt;
	CListItem * m_pCurItem;
// Dialog Data
	enum { IDD = IDD_DLG_ADDTOSTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	vector<int> vt_CateID, vt_LibID;

	DECLARE_MESSAGE_MAP()
private:
	CADODatabase* m_db;
	void SetStoryList();
	void SetPubTypeList();
	void InitList();
	void LoadListOfStory();
	void SetComboItems();
	void CreateAllDirectory(CString strPath);
	bool AddToStory(int i_StoryID, COleDateTime dt_target);
	CString LoadFileText(CString strFilePath);

public:
	int m_storyID;
	int m_PublishTypeID;
	CListCtrl m_lstStory;
	CComboBox m_cmbStoryCategory;
	CComboBox m_cmbNewLib;
	CDateTimeCtrl m_dtStoryDate;
	bool m_b_NewText;
	int m_i_storyItemID;

	afx_msg void OnDtnDatetimechangeStorydate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedCancel();
	CListBox m_lstMenu;
	CListBox m_lbPubType;
	virtual BOOL OnInitDialog();

	COleDateTime m_selectDate;

	afx_msg void OnBnClickedOk();
	afx_msg void OnDtnDatetimechangeDtpicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadexist();
	afx_msg void OnBnClickedRadnew();
	CComboBox m_cbx_Lib;
	afx_msg void OnCbnSelchangeCbLib();
	CListCtrl m_lst_Story_Cat;
	COleDateTime m_dt_Date_AddStory;
	CDateTimeCtrl m_dtStoryDate_Add;
	COleDateTime m_dt_Date;
};
