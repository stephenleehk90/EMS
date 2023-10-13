#if !defined(AFX_COPYLIST_H__FE10F4CF_0970_4551_BE48_1A9739CA2325__INCLUDED_)
#define AFX_COPYLIST_H__FE10F4CF_0970_4551_BE48_1A9739CA2325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CopyList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCopyList dialog

class CCopyList : public CDialog
{
// Construction
public:
	CString m_strFileCode;
	void GetPaperCopyList();
	void InitList();
	CCopyList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCopyList)
	enum { IDD = IDD_COPYLIST };
	CRowListCtrlEx	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCopyList)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYLIST_H__FE10F4CF_0970_4551_BE48_1A9739CA2325__INCLUDED_)
