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
    //文件?型值??到字符串  
    CString  FileTypeToString();  
    //?取文件?型  
    TextCode GetFileType();  
    //按行?取文件  
    BOOL     ReadString(CString& strfileContent);
    //??方法  ?取文件?型  
    static   TextCode GetFileType( LPCTSTR lpszFileName);  
protected:  
    TextCode m_FileType;//保存文件?型  
    const static int  PREDEFINEDSIZE;//?定?一行文件所需空?  
protected:  
    //?UTF-8文件按行?取  
    BOOL     ReadStringFromUTF8File(CString& rString);  
    //?ANSI文件按行?取  
    BOOL     ReadStringFromAnsiFile(CString& rString);  
    //重UNCIDOE、UNICODE big endian文件?取  
    BOOL     ReadStringFromUnicodeFile(CString& strFileContent);  
	//UTF-8字符串??到UNICODE字符串  
    CString  UTF8ToUnicode(byte  *szUTF8);  
    //?理文件打??志  
    UINT     ProcessFlags(LPCTSTR lpszFileName, UINT& nOpenFlags,TextCode &tc);  
};  
  
#endif // !defined(AFX_STDIOFILEEX_H__C1F1F96B_9417_4388_8D24_892EDFA2A616__INCLUDED_) 