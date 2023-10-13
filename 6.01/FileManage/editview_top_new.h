#if !defined(AFX_editView_Top_New_H__98EBC5C4_8A0D_450B_B0DF_7F0E12E97470__INCLUDED_)
#define AFX_editView_Top_New_H__98EBC5C4_8A0D_450B_B0DF_7F0E12E97470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// editView_Top_New.h : header file
//
#include <afxrich.h>
/////////////////////////////////////////////////////////////////////////////
// editView_Top_New view

class editView_Top_New : public CRichEditView
{
protected:
	editView_Top_New();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(editView_Top_New)
	CBrush sBRUSH;
	HBRUSH sHBR;
	int GetVersion(int i_pos);
	int GetLineNumber(int i_pos, int &i_line_pos); 
	bool IsDigit_Character(CString strTemp);

	bool IsSelectedChar(int i_temp); 	

	bool IsCapLetter_Character(CString strTemp);
	CString GetActualSelectedText();  
	int GetVersion_New(int i_pos, bool &b_Del);
	void InitModHistArray(); 

	CFont* m_pFont;
	CStatusBar* m_pStatusBar;
	CString m_str_Text, m_str_Text_Temp;
	CString m_str_Text_Version;
	CString m_str_Mod_History;

	CFont	*m_ppFont[MAX_TEXTSTYLE_NUM];
	int		m_pnLineSpace[MAX_TEXTSTYLE_NUM];
	int		m_pnCharSpace[MAX_TEXTSTYLE_NUM];
	int		m_nStyleNum;	// 1 ... MAX_STYLE_NUM
	bool m_bFontCreated;

	int m_max_height;
	int m_max_width;
	CMenu*		m_pPopupMenu;
	bool m_b_UpdateScroll;
	BOOL						m_mouseDown;
	BOOL						m_mouseMove;
	long						m_caret;
	long						m_selectStart_x;
	long						m_selectEnd_x;
	long						m_selectStart_y;
	long						m_selectEnd_y;
	long						m_previous_x;
	long						m_previous_y;

	long						m_indent;
	long						m_tabSize;
	CFont						m_font;
	CPoint m_mousePosition;
	
	CStringArray m_arrModHist;
	int m_max_select_pos;
	int m_min_select_pos;

// Attributes
public:
//	void SetContentText(CString strText, CString strTextVersion);
	void SetContentText(CString strText, CString strTextVersion, CString strModHistory);
	void UpdateFontSet();
	void UpdateFontSetTemp(CString strFontName);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(editView_Top_New)
	public:
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~editView_Top_New();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(editView_Top_New)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCopy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateDoModify(CCmdUI* pCmdUI);
	afx_msg void OnEditShowModHistory();
	afx_msg void OnOptionModifyHistory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_editView_Top_New_H__98EBC5C4_8A0D_450B_B0DF_7F0E12E97470__INCLUDED_)
