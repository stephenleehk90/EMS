// FileListCtrl.h: interface for the CFileListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILELISTCTRL_H__E92BC77D_930F_4C1D_8C10_26A5077016C8__INCLUDED_)
#define AFX_FILELISTCTRL_H__E92BC77D_930F_4C1D_8C10_26A5077016C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "..\commkits\comctrex.h"
#define  WM_GETLISTCTRLTEXTCLR	WM_USER+300

class CFileListCtrl : public CListCtrlEx
{
public:
	HWND m_hParent;
	int m_nUserCol;
	CFileListCtrl();
	virtual ~CFileListCtrl();
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
};

#endif // !defined(AFX_FILELISTCTRL_H__E92BC77D_930F_4C1D_8C10_26A5077016C8__INCLUDED_)
