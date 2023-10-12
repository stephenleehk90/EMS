// StdioFileEx.h: interface for the CStdioFileEx class.  
//  
//////////////////////////////////////////////////////////////////////  
  
#if !defined(AFX_STDIOFILEEX_H__C1F1F96B_9417_4388_8D24_892EDFA2A616__INCLUDED_)  
#define AFX_STDIOFILEEX_H__C1F1F96B_9417_4388_8D24_892EDFA2A616__INCLUDED_  

#if _MSC_VER > 1000  
#pragma once  
#endif // _MSC_VER > 1000  
                                             
#include "stdafx.h"  
typedef enum TextCodeType  
{  
    UTF8=0,  
    UNicode =1,  
    UNICODEBIGENDIAN=2,  
    ANSI=3,  
    FILEERROR=4  
}TextCode;  
class CStdioFileEx  :public CStdioFile  
{  
public:  
    CStdioFileEx();  
    CStdioFileEx(FILE* pOpenStream);  
    CStdioFileEx(LPCTSTR lpszFileName, UINT nOpenFlags);  
    virtual ~CStdioFileEx();  
    virtual BOOL Open( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL);  
public:  
    //���?����??��r�Ŧ�  
    CString  FileTypeToString();  
    //?�����?��  
    TextCode GetFileType();  
    //����?�����  
    BOOL     ReadString(CString& strfileContent);
    //??��k  ?�����?��  
    static   TextCode GetFileType( LPCTSTR lpszFileName);  
protected:  
    TextCode m_FileType;//�O�s���?��  
    const static int  PREDEFINEDSIZE;//?�w?�@����һݪ�?  
protected:  
    //?UTF-8������?��  
    BOOL     ReadStringFromUTF8File(CString& rString);  
    //?ANSI������?��  
    BOOL     ReadStringFromAnsiFile(CString& rString);  
    //��UNCIDOE�BUNICODE big endian���?��  
    BOOL     ReadStringFromUnicodeFile(CString& strFileContent);  
	//UTF-8�r�Ŧ�??��UNICODE�r�Ŧ�  
    CString  UTF8ToUnicode(byte  *szUTF8);  
    //?�z���??��  
    UINT     ProcessFlags(LPCTSTR lpszFileName, UINT& nOpenFlags,TextCode &tc);  
};  
  
#endif // !defined(AFX_STDIOFILEEX_H__C1F1F96B_9417_4388_8D24_892EDFA2A616__INCLUDED_) 