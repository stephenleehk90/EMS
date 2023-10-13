// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D0ECFD44_61E4_4B08_B0CB_D88B7CEBEA6C__INCLUDED_)
#define AFX_STDAFX_H__D0ECFD44_61E4_4B08_B0CB_D88B7CEBEA6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "resource.h"
#include "..\editroom\globaldef.h"
#include "..\fzado\ado2.h"
#include "global.h"
#include "..\commkits\commkits.h"
#include "filedb.h"
extern HINSTANCE g_hInst;

#import <msxml.dll> named_guids
using namespace MSXML;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D0ECFD44_61E4_4B08_B0CB_D88B7CEBEA6C__INCLUDED_)
