// StdioFileEx.cpp: implementation of the CStdioFileEx class.  
//  
//////////////////////////////////////////////////////////////////////  
  
#include "stdafx.h"  
#include "StdioFileEx.h"  
#include <string>
#ifdef _DEBUG  
#undef THIS_FILE  
static char THIS_FILE[]=__FILE__;  
#define new DEBUG_NEW  
#endif  
  
//////////////////////////////////////////////////////////////////////  
// Construction/Destruction  
//////////////////////////////////////////////////////////////////////  
/*static*/ const  int  CStdioFileEx::PREDEFINEDSIZE=1024;  
CStdioFileEx::CStdioFileEx():CStdioFile()  
{  
    m_FileType=ANSI;//指定默??型  
}  
CStdioFileEx::CStdioFileEx(FILE* pOpenStream):CStdioFile(pOpenStream)  
{  
  char* filepath=pOpenStream->_tmpfname;//?  尚不清楚File*?构  
  wchar_t* szFilePath = new wchar_t[1000];
  MultiByteToWideChar(CP_ACP, 0, filepath, -1, szFilePath, 1000);
  m_FileType=GetFileType(szFilePath);  
  delete[] szFilePath;
}  
CStdioFileEx::CStdioFileEx(LPCTSTR lpszFileName, UINT nOpenFlags):CStdioFile(lpszFileName,ProcessFlags(lpszFileName, nOpenFlags,m_FileType) )  
{  
}  
CStdioFileEx::~CStdioFileEx()  
{  
  
}  
// --------------------------------------------------------------------------------------------  
//CStdioFileEx::GetFileType  ??方法  ??文本文件?型  
// --------------------------------------------------------------------------------------------  
/*static */ TextCode CStdioFileEx::GetFileType(LPCTSTR lpszFileName)  
{  
    CFile file;  
    byte  buf[3];//unsigned char  
    TextCode tc;  
    try  
    {  
        if(file.Open(lpszFileName,CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary))  
        {     
            file.Read(buf,3);  
            if(buf[0]==0xEF && buf[1]==0xBB && buf[2]==0xBF)  
                tc=UTF8;  
            else  
            if(buf[0]==0xFF && buf[1]==0xFE )  
                tc=UNicode;  
            else  
            if(buf[0]==0xFE && buf[1]==0xFF )  
                tc=UNICODEBIGENDIAN;  
            else  
                tc=ANSI;  
        }  
        else  
            tc=FILEERROR;  
    }  
    catch (CFileException ex)  
    {     
        CString errormsg;  
        errormsg.Format(_T("操作文件%s??生异常!"),ex.m_strFileName);  
        AfxMessageBox(errormsg);  
    }  
    return tc;  
}  
// --------------------------------------------------------------------------------------------  
//CStdioFileEx::Readstring 按行?取文本文件  
//根据不同文件?型 ?用不同的?取函?  
// --------------------------------------------------------------------------------------------  
BOOL CStdioFileEx::ReadString(CString &strFileContent)  
{      
    BOOL flag=FALSE;  
     switch(m_FileType)		
     {  
     case ANSI:  
          flag=ReadStringFromAnsiFile(strFileContent);  
          break;  
     case UNICODE:  
     case UNICODEBIGENDIAN:  
          flag=ReadStringFromUnicodeFile(strFileContent);  
          break;  
     case UTF8:  
         flag=ReadStringFromUTF8File(strFileContent);  
         break;  
     case FILEERROR:  
         flag=FALSE;  
         break;  
     default:  
         break;  
     }  
     return flag;  
}  
// --------------------------------------------------------------------------------------------  
//CStdioFileEx::ReadstringFromAnsiFile  ?ANSI文件?取字符串  
// --
/*------------------------------------------------------------------------------------------  
BOOL CStdioFileEx::ReadStringFromAnsiFile(CString& rString)  
{     
    BOOL flag;  
    try  
    {     
        flag=CStdioFile::ReadString(rString);  
        rString+="\r\n";  
    }  
    catch(CFileException ex)  
    {  
        CString errormsg;  
        errormsg.Format(_T("操作文件%s??生异常!"),ex.m_strFileName);  
        AfxMessageBox(errormsg);  
    }  
    return flag;  
} 
*/
BOOL CStdioFileEx::ReadStringFromAnsiFile(CString& strFileContent) {
	int nFileLength=CFile::GetLength(),nCount=0,nEnterCount=0,nCurPos=0;
	//char readByte[1];
	unsigned  char szByte;
	char * strContent=(char *)malloc(nFileLength+1);
	::ZeroMemory(strContent, nFileLength+1);
	/*while(CFile::Read(readByte,1)){
		szByte=readByte[0];
		if(szByte>0x7f && readByte!='\0'&& *readByte!='\n')
			nCount++;
		if (szByte=='\n')
			nEnterCount++;
		//nCurPos++;
		//CFile::Seek(nCurPos,CFile::begin);
	}*/
	CFile::SeekToBegin();
	CFile::Read(strContent,nFileLength);
	/*char *readByte=strContent;
	int a =strlen(strContent);
	while(*readByte){
		if((*(readByte+1)==NULL)||*readByte<0xa1||*(readByte+1)<0xa1)
		{
			//独1字节或连续两字节有一个<0xa1,肯定不是汉字
			//当普通字符处理
			readByte++;
		}
		else
		{
			//处理汉字
			readByte+=2;
			nCount++;
		}
	}*/

 	wchar_t* szContent = new wchar_t[nFileLength+1];
	ZeroMemory(szContent, sizeof(WCHAR)*(nFileLength+1));
	MultiByteToWideChar(CP_ACP, 0, strContent, -1, szContent, nFileLength);
	strFileContent=szContent;
	/*CString str;
	int nStrLength=lstrlen(szContent);
	str.Format(L"%d",nFileLength);
	strFileContent=strFileContent.Mid(0,nFileLength-nCount);
	free(strContent);*/
	delete[] szContent;
	int nMBLen = MultiByteToWideChar(CP_UTF8, 0, strContent, nFileLength, NULL, NULL);
	WCHAR* szWcsBuffer = new WCHAR[nMBLen+1];
	ZeroMemory(szWcsBuffer, sizeof(WCHAR)*(nMBLen+1));
	MultiByteToWideChar(CP_UTF8, 0, strContent, nFileLength, szWcsBuffer, nMBLen);

	nMBLen = WideCharToMultiByte(CP_UTF8,NULL,szWcsBuffer,-1,NULL,0,NULL,FALSE);
	char *szWcsBuffer2 = new char[nMBLen+1];
	WideCharToMultiByte (CP_UTF8,NULL,szWcsBuffer,-1,szWcsBuffer2,nMBLen+1,NULL,FALSE);

	if (strcmp(szWcsBuffer2,strContent) == 0 && strFileContent.GetLength()>wcslen(szWcsBuffer))
	{
		strFileContent = szWcsBuffer;  //UTF-8 without BOM
		m_FileType = UTF8;
	}
	delete []szWcsBuffer;
	delete []szWcsBuffer2;
	free(strContent);
	//return TRUE;
	return FALSE;
}
// --------------------------------------------------------------------------------------------  
//CStdioFileEx::ReadstringFromUTF8File ?UTF8文件中按行?取   
//由于UTF-8??多字???且各种字符?度不同,判?回??行需要判?????字?    
// --------------------------------------------------------------------------------------------  
BOOL  CStdioFileEx::ReadStringFromUTF8File(CString& rString)  
{  
    long    index;  
    byte    cr=0x0d;//回??行符  
    byte    lf=0x0a;  
    byte    temp[2];  
    byte    tempbyte;  
    byte    *pbuf=new byte[PREDEFINEDSIZE+1];  
    memset(pbuf,0,(PREDEFINEDSIZE+1)*sizeof(byte));  
    UINT    readlen;  
    try  
    {     
        //跳?文件? 移?文件指?  
        if (m_pStream && ( GetPosition() == 0))  
        {  
            CStdioFile::Seek(3*sizeof(byte),CFile::begin);  
        }  
        index=0;  
        do   
        {  
            memset(temp,0,2*sizeof(byte));  
            readlen=CFile::Read(temp,2);//CStdioFile::Read效果不同 ?省去回?符0x0d  
            if(!readlen)  
                break;//return FALSE;  
            //元素存?到字???中  
            pbuf[index++]=temp[0];  
            pbuf[index++]=temp[1];  
            tempbyte=temp[1];  
            //判?回??行  
            if( ( tempbyte==cr && temp[0]==lf) ||(temp[0]==cr && temp[1]==lf))  
                break;  
        } while (readlen==2 && index<PREDEFINEDSIZE );  
        pbuf[index]=0;  
        rString=UTF8ToUnicode(pbuf);//UTF8????到UNICODE  
		  
        if(rString.IsEmpty())  
           return FALSE;  
    }  
    catch (CFileException ex)  
    {  
        CString errormsg;  
        errormsg.Format(_T("操作文件%s??生异常!"),ex.m_strFileName);  
        AfxMessageBox(errormsg);  
    }  
    delete[] pbuf;  
    return TRUE;  
}  
// --------------------------------------------------------------------------------------------  
//?UNICODE、UNICODE big endian文件按行?取  
//??取字?小于?求值(文件?束)或者超??定?空????件退出循?  
//wChLine存放每行字符,wchtemp存放???取字符  
//????UNICODE big endian ?交?高低字? ,?其??成UNICODE字符串  
// --------------------------------------------------------------------------------------------  
BOOL  CStdioFileEx::ReadStringFromUnicodeFile(CString& rString)  
{  
    long    index;  
    UINT    readlen;  
    wchar_t wchcr=MAKEWORD(0x0d,0x00);;//回?符  MakeWord(低、高字??序)   
    wchar_t wchlf=MAKEWORD(0x0a,0x00);  
    wchar_t *wChLine=new wchar_t[PREDEFINEDSIZE+1];  
    memset(wChLine,0,(PREDEFINEDSIZE+1)*sizeof(wchar_t));  
    wchar_t wchtemp[2];  
    BOOL   flag=TRUE;  
    try  
    {     
        //跳?文件? 移?文件指?  
        if (m_pStream && ( GetPosition() ==0))  
        {  
            Seek(2*sizeof(byte),CFile::begin);  
        }  
        index=0;  
        do  
        {     
            memset(wchtemp,0,2*sizeof(wchar_t));  
            readlen=CFile::Read(wchtemp,sizeof(wchar_t)*2);//CStdioFile::Read效果不同  
            if(!readlen)  
                 break;  
            //UNICODE big endian交?高低字?  
            if(UNICODEBIGENDIAN==m_FileType)  
            {     
                unsigned char high, low;  
                high = (wchtemp[0] & 0xFF00) >>8;  
                low  = wchtemp[0] & 0x00FF;  
                wchtemp[0] = ( low <<8) | high;  
                high = (wchtemp[1] & 0xFF00) >>8;  
                low  = wchtemp[1] & 0x00FF;  
                wchtemp[1] = ( low <<8) | high;  
            }  
            wChLine[index++]=wchtemp[0];  
            wChLine[index++]=wchtemp[1];  
            //判?回??行  
            if(wchtemp[0]==wchcr && wchtemp[1]==wchlf)  
                break;  
        }  
        while( (readlen==sizeof(wchar_t)*2) && index<PREDEFINEDSIZE );  
        wChLine[index]=0;  
        CString strtext(wChLine,index);  
        rString=strtext;  
        if(rString.IsEmpty())  
           flag=FALSE;  
    }  
    catch (CFileException ex)  
    {  
        CString errormsg;  
        errormsg.Format(_T("操作文件%s??生异常!"),ex.m_strFileName);  
        AfxMessageBox(errormsg);  
    }  
    delete[] wChLine;  
    return flag;  
}  
/*BOOL  CStdioFileEx::ReadStringFromUnicodeFile(CString &strFileContent){
	int nFileLength=CFile::GetLength();
	wchar_t * strContent=(wchar_t *)malloc(nFileLength+1 );	
	CFile:SeekToBegin();
	CFile::Read(strContent,nFileLength);
	strFileContent=strContent;
	strFileContent=strFileContent.Mid(0,nFileLength/2);
	free(strContent);  
	return TRUE;
}*/
// --------------------------------------------------------------------------------------------  
//CStdioFileEx::UTF8ToUnicode  UTF-8字符串??成UNICODE字符串  
// --------------------------------------------------------------------------------------------  
CString CStdioFileEx::UTF8ToUnicode(byte  *szUTF8)  
{     
    CString strret;  
    strret=_T("");  
    if(!szUTF8)  
        return strret;  
    //?取??后所需串空?的?度   
    int   wcsLen =  MultiByteToWideChar(CP_UTF8,0,(LPSTR)szUTF8,strlen((char*)szUTF8),NULL,0);  
    LPWSTR   lpw=new   WCHAR[wcsLen+1];   
    if(!lpw)  
        return strret;  
    memset(lpw,0,(wcsLen+1)*sizeof(wchar_t));   
    //?施??  
    MultiByteToWideChar(CP_UTF8,0, (LPSTR)szUTF8,   
        strlen((char *)szUTF8),  (LPWSTR)lpw,  wcsLen);   
    CString str(lpw);  
    delete[]   lpw;  
    return str;  
}  
// --------------------------------------------------------------------------------------------  
//CStdioFileEx::GetFileType?取文件?型  
// --------------------------------------------------------------------------------------------  
TextCode CStdioFileEx::GetFileType()  
{     
    return m_FileType;  
}  
// --------------------------------------------------------------------------------------------  
//CStdioFileEx::FileTypeToString 文件?型枚?值???字符串值  
// --------------------------------------------------------------------------------------------  
CString CStdioFileEx::FileTypeToString()  
{     
    CString strtype;  
    switch(m_FileType)  
    {  
    case ANSI:  
        strtype.Format(L"%s",_T("ANSI"));  
        break;  
    case UTF8:  
        strtype.Format(L"%s",_T("UTF8"));  
        break;  
    case UNICODE:  
        strtype.Format(L"%s",_T("UNICODE"));  
        break;  
    case UNICODEBIGENDIAN:  
        strtype.Format(L"%s",_T("UNICODE big endian"));  
        break;  
    case FILEERROR:  
        strtype.Format(L"%s",_T("FILEERROR"));  
        break;  
    default:  
        break;  
    }  
    return strtype;  
}  
// --------------------------------------------------------------------------------------------  
//CStdioFileEx::Open 重?父?的文件打?操作 改?不同?型文件的打?方式  
// --------------------------------------------------------------------------------------------  
BOOL CStdioFileEx::Open( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError)  
{     
    ProcessFlags(lpszFileName,nOpenFlags,m_FileType);//?理文件打?方式  
    return CStdioFile::Open(lpszFileName, nOpenFlags,pError);  
}  
// --------------------------------------------------------------------------------------------  
//CStdioFileEx::ProcessFlags ?理不同文件的打?方式  
//ANSI文件采用文本?取，UNICODE、UNICDOE big endian、UTF-8采用二?制方式?取  
// --------------------------------------------------------------------------------------------  
UINT CStdioFileEx::ProcessFlags(LPCTSTR lpszFileName, UINT& nOpenFlags,TextCode &tc)  
{  
    tc=CStdioFileEx::GetFileType(lpszFileName);  
    if ((nOpenFlags & CFile::modeReadWrite)|| (nOpenFlags & CFile::modeRead) )  
    {  
        switch(tc)  
        {  
        case ANSI:  
            nOpenFlags|= CFile::typeText;  
            nOpenFlags&=~CFile::typeBinary;  
            break;  
        case UTF8:  
            nOpenFlags |= CFile::typeBinary;  
            nOpenFlags&= ~CFile::typeText;  
            break;  
        case UNICODE:  
            nOpenFlags |= CFile::typeBinary;  
            nOpenFlags&= ~CFile::typeText;  
            break;  
        case UNICODEBIGENDIAN:  
            nOpenFlags |= CFile::typeBinary;  
            nOpenFlags&= ~CFile::typeText;  
            break;  
        case FILEERROR:  
            break;  
        default:  
            break;  
        }  
    }  
    nOpenFlags|=CFile::shareDenyNone;  
    return nOpenFlags;  
}  