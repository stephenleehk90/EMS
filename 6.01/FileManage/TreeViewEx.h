#ifndef _TREEVIEWEX_H
#define _TREEVIEWEX_H

class AFX_EXT_CLASS CTreeViewEx : public CTreeView
{
private:
	CImageList m_ImageList;
protected:
	CTreeViewEx();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTreeViewEx)
		
		// Attributes
public:
	
	// Operations
public:
	void SetNewStyle(long lStyleMask, BOOL bSetBits);
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeViewEx)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	virtual ~CTreeViewEx();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeViewEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
