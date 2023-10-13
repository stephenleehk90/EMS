#if !defined(AFX_WORDCHECKDLG_H__9F020763_1241_44B6_BF7D_58C96A9203B8__INCLUDED_)
#define AFX_WORDCHECKDLG_H__9F020763_1241_44B6_BF7D_58C96A9203B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WordCheckDlg.h : header file
//
#include "editdoc.h"
#include "afxcmn.h"
/////////////////////////////////////////////////////////////////////////////
// CWordCheckDlg dialog

// edit by leon 2012-09-04 for word bank
class WORDITEM{
public:
	BOOL bValid;
	CString strName;
	int nPosition;
	//CString strReplaced; 
	//int nMaskPosition;
	WORDITEM();
};
typedef CArray<WORDITEM,WORDITEM> ARRAY_WORDINFO;

class CONTENTITEM{
public:
	int nPosition;
	CString strName;
	
};

typedef CArray<CONTENTITEM,CONTENTITEM> ARRAY_CONTENTITEM;

class CWordCheckDlg : public CDialog
{
// Construction
public:
	CWordCheckDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWordCheckDlg)
	enum { IDD = IDD_WORDCHECK };
	CRichEditCtrl	m_richCrl;
	CListBox	m_wordList;
	CListBox	m_ctrList;
	CString	m_richText, m_strShowText;
	CString m_strMask;// "a" for unchange char, "d" for replace char, "b" for replaced char
	int iTextLenAdded;
	CString	m_DicText;
	CString m_OrgText;
	POINT m_pt;
	CPoint m_ptRtc;
	BOOL m_bCheckDefFirst;
	CToolTipCtrl m_toolTip;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWordCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWordCheckDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy) ;
	afx_msg void OnIgnoreOnce();
	afx_msg void OnIgnoreAll();
	afx_msg void OnChangeWord();
	afx_msg void OnChangeAll();
	afx_msg void OnAddToDictionary();
	afx_msg void OnSetfocusEditText();
	afx_msg void OnChangeDictionary();
	afx_msg void OnDblclkReplaceList();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	//edit by leon 2012-09-04 for word bank
	int m_nWordLength,m_nUseWordLength,m_nCurPos,m_nPos1,m_nPos2;
	ARRAY_WORDINFO m_arrSqlWordRecord,m_arrUseWordRecord;
	ARRAY_CONTENTITEM m_arrContentItemList;
	CString m_strIgnoreText;
	void HightLightWord();
	void FillUseWordList(CString strContent);
	void ResetPosition(int nLength,BOOL bReset=FALSE);
	void ReplaceText(BOOL bReplace=FALSE);
	void SetLibGroup(CString &strLibGroup);	
	void OnCheck(CString strCotent);
	void OnReset();
	void SetReplaceWordPos();
	void SetDefReplaceWordPos();
	void OnChangeAllDef();
public:
	
	CStringArray m_carrDic;	//for record current selecting dictionary
	int m_nDicCount;	//for count the number of selected dictionary
	CString m_strReplaceWord;
	int m_nReplacePos;
	ARRAY_REPLACEWORD m_arrReplaceWord;
	BOOL m_bChangeAllDef;
	afx_msg void OnClose();
	void MarkReplacedMask(int iPosition, CString strOrg, CString strDest);
	CRichEditCtrl m_rtcShowMask;
	void MarkColorOnShowRTC();
	void ClearHightLight();
	afx_msg void OnEnSetfocusRichedit21();
	afx_msg void OnBnClickedFinish();
};
int CharIndexToMultiIndex(CString strSrc,int nIndex);


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORDCHECKDLG_H__9F020763_1241_44B6_BF7D_58C96A9203B8__INCLUDED_)
