#include "stdafx.h"
#include "tempapp.h"
#include <ctype.h>
#include <direct.h>
#include "DBGraphic.h"
int n_TwoAsciiIsOneWord = 0;

wchar_t *GetTitleWord( LPBYTE hpszBuffer , long lBytesRead , int nTitleLine  , long *plWordNumber )
{
//    static char szTitle[ TITLE_LEN + 1 ] = "" ;
    static wchar_t szTitle[ TITLE_LEN + 1 ] = L"" ;
    wchar_t  	*szTitlePtr= NULL ;
    int 	nChineseFlag = 0 , nGetTitle = 0 ;
    int		nAsciiArticleFlag = 1 ;
    long    lWordNumber = 0 , lLineStart ;
    register long l ;
    int  	nLineChar , nAsciiCharFlag = 0 ;
    wchar_t	c ;
    // Buffer
    long	lBufferSize = 0L ;

    lstrcpy( szTitle , L"" ) ;

    
    // Check if it is not a Chinese Article
    for( l = 0L ; l < lBytesRead ; l ++ )
    {
    	if( (unsigned int)hpszBuffer[ l ] > (unsigned int)'\xa0' )
    	{
    		nAsciiArticleFlag = 0 ;
    		break ;
    	}
    }
    
    // Get Title 
    szTitlePtr = szTitle ;

    nLineChar = 0 ;
    lLineStart = 0 ;
    for( l = 0L ; l < lBytesRead ; l ++ )
    {
		if( nLineChar >0 )				 //2
		{
	    	if( nTitleLine <= 1 )
	    	{
				l = lLineStart ;
				break ;
	    	}
		}
		c = hpszBuffer[ l ] ;
		switch( c )
		{
	    	case '\x0a' :
				if( nLineChar > 2 )
		    		nTitleLine -- ;
				lLineStart = l + 1 ;
				nLineChar = 0 ;
				break ;
				
	    	case ' ' :
				break;
	    	case '\x0d' :
				 if( nLineChar > 2 )
		    		nTitleLine -- ;
				lLineStart = l + 1 ;
				nLineChar = 0 ;
				break;
	    	case '\t' :
				break ;
	    	default :
				if( (unsigned char)c > (unsigned char)'\xa0' )
				{
		    		nLineChar ++ ;
				}
				else if( nAsciiArticleFlag && iswprint( c ) )
				{
					nLineChar ++ ;
				}
		}
    }

    for( ; l < lBytesRead ; l ++	)
    {
		c = hpszBuffer[ l ] ;

		if( nGetTitle == 0 )
		{
	    	switch( c )
	    	{
				case ' ' :
				case '\x0d' :
				case '\x0a' :
		    		break ;
				default :
		    		if( nChineseFlag )
		    		{
						*szTitlePtr ++ = c ;
						nGetTitle ++ ;
						nChineseFlag ^= 1 ;
		    		}
		    		else if( (unsigned char)c > (unsigned char)'\xa0')
		    		{
						*szTitlePtr ++ = c ;
						nGetTitle ++ ;
						nChineseFlag ^= 1 ;
		    		}
		    		else if( iswprint( c ) )
		    		{
						*szTitlePtr ++ = c ;
						nGetTitle ++ ;
						nChineseFlag = 0 ;
		    		}
		    		else
						nChineseFlag = 0 ;
	    	}
		}
		else if( (c == '\x0d') || (c == '\x0a') )
	    	break ;
		else if( nChineseFlag || ((unsigned char)c > (unsigned char)'\xa0') )
		{
	    	*szTitlePtr ++ = c ;
	    	nGetTitle ++ ;
	    	nChineseFlag ^= 1 ;
		}
		else if( iswprint( c ) )
		{
	    	*szTitlePtr ++ = c ;
	    	nGetTitle ++ ;
	    	nChineseFlag = 0 ;
		}
		else
	    	nChineseFlag = 0 ;

		if( (nGetTitle >= TITLE_LEN - 2) && nChineseFlag == 0 )
	  		break ;
    }

    *szTitlePtr = '\0' ;
    
    // Get Word Number
    if ( plWordNumber == NULL )
    	return szTitle ;
    	
    	
    for( l = 0L ; l < lBytesRead ; l ++	)
	{
		c = hpszBuffer[ l ] ;

		if( nChineseFlag || ((unsigned char)c > (unsigned char)'\xa0') )
		{                      
			if( nChineseFlag )
				lWordNumber ++ ;
    		nChineseFlag ^= 1 ;
		}
		else if( iswprint( c ) )
		{   
			if( n_TwoAsciiIsOneWord )
			{
				nAsciiCharFlag ++ ;
				if( nAsciiCharFlag >= 2 )
				{
					lWordNumber ++ ;
					nAsciiCharFlag = 0 ;
				}
			}
			else
			 	lWordNumber ++ ;
				 	
    		nChineseFlag = 0 ;
		}
		else
    		nChineseFlag = 0 ;
	}


	*plWordNumber = lWordNumber ;
    return szTitle ;
}

void ArrangeTitle ( wchar_t *szTitle )
{
	long lSize ;
	lSize = lstrlen ( szTitle );
	int i = 0 ;
	while ( i < lSize)
	{
		if( (unsigned char)szTitle[i] > (unsigned char)'\xa0' )
			break;
		else if(isalnum(szTitle[i]))
			break;
		i ++ ;
	}         
	szTitle = szTitle + i ;
	lSize -= i ;       
	while ( lSize > 0 )
	{
		if( (unsigned char)szTitle[lSize-1] > (unsigned char)'\xa0' )
			break;
		else if(isalnum(szTitle[lSize-1]))
			break;
		if(szTitle[lSize-1]=='.'||szTitle[lSize-1]=='`'||szTitle[lSize-1]=='\'')
			break;
		else
		lSize -- ;
	}           
	szTitle[lSize] = '\0' ;
}

char *GetTeleTitleWord( LPBYTE hpszBuffer , long lBytesRead , int nTitleLine  , long *plWordNumber , long *plDelay )
{
    static char szTitle[ TITLE_LEN + 1 ] = "" ;
    char  	*szTitlePtr= NULL ;
    char	 *cp ;
    int 	nChineseFlag = 0 , nGetTitle = 0 ;
    int		nAsciiArticleFlag = 1 ;
    long    lWordNumber = 0 , lLineStart ;
    register long l ;
    int  	nLineChar , nAsciiCharFlag = 0 ;
    char	c ;
    // Buffer
    long	lBufferSize = 0L ;

    strcpy( szTitle , "" ) ;

    
    // Check if it is not a Chinese Article
    for( l = 0L ; l < lBytesRead ; l ++ )
    {
    	if( (unsigned int)hpszBuffer[ l ] > (unsigned int)'\xa0' )
    	{
    		nAsciiArticleFlag = 0 ;
    		break ;
    	}
    }
    
	
    // Get Title 
    szTitlePtr = szTitle ;

    nLineChar = 0 ;
    lLineStart = 0 ;
		for( l = 0L ; l < lBytesRead ; l ++ )
		{
			if( nLineChar >0 )				//2
			{
	    		if( nTitleLine <= 1 )
	    		{
					l = lLineStart ;
					break ;
	    		}
			}
			c = hpszBuffer[ l ] ;
			switch( c )
			{
	    		case '\x0a' :
					if( nLineChar > 2 )
		    			nTitleLine -- ;
					lLineStart = l + 1 ;
					nLineChar = 0 ;
					break ;
				
	    		case ' ' :
					break;
	    		case '\x0d' :
					 if( nLineChar > 2 )
		    			nTitleLine -- ;
					lLineStart = l + 1 ;
					nLineChar = 0 ;
					break;
	    		case '\t' :
					break ;
	    		default :
					if( (unsigned char)c > (unsigned char)'\xa0' )
					{
		    			nLineChar ++ ;
					}
					else if( nAsciiArticleFlag && isprint( c ) )
					{
						nLineChar ++ ;
					}

	//				nLineChar ++ ;
			}
		}

	// cut files before titleline
	cp  = (char *)hpszBuffer + l ,
	lBytesRead -= l ;	
	*plDelay = l ;
	l = 0 ;
    for( ; l < lBytesRead ; l ++	)
    {
		c = cp[ l ] ;

		if( nGetTitle == 0 )
		{
	    	switch( c )
	    	{
				case ' ' :
				case '\x0d' :
				case '\x0a' :
		    		break ;
				default :
		    		if( nChineseFlag )
		    		{
						*szTitlePtr ++ = c ;
						nGetTitle ++ ;
						nChineseFlag ^= 1 ;
		    		}
		    		else if( (unsigned char)c > (unsigned char)'\xa0')
		    		{
						*szTitlePtr ++ = c ;
						nGetTitle ++ ;
						nChineseFlag ^= 1 ;
		    		}
		    		else if( isprint( c ) )
		    		{
						*szTitlePtr ++ = c ;
						nGetTitle ++ ;
						nChineseFlag = 0 ;
		    		}
		    		else
						nChineseFlag = 0 ;
	    	}
		}
		else if( (c == '\x0d') || (c == '\x0a') ){
				break ;
		}
		else if( nChineseFlag || ((unsigned char)c > (unsigned char)'\xa0') )
		{
	    	*szTitlePtr ++ = c ;
	    	nGetTitle ++ ;
	    	nChineseFlag ^= 1 ;
		}
		else if( isprint( c ) )
		{
	    	*szTitlePtr ++ = c ;
	    	nGetTitle ++ ;
	    	nChineseFlag = 0 ;
		}
		else
	    	nChineseFlag = 0 ;

		if( (nGetTitle >= TITLE_LEN - 2) && nChineseFlag == 0 )
	  		break ;
	  	else if ( (nGetTitle >= TITLE_LEN - 2) && nChineseFlag != 0 )
	  	{
	  		*szTitlePtr ++ = cp[ l +1 ] ;
	  		break;
	  	}
    }

    *szTitlePtr = '\0' ;
    
	
    // Get Word Number
    for( l = 0L ; l < lBytesRead ; l ++	)
	{
		c = cp[ l ] ;

		if( nChineseFlag || ((unsigned char)c > (unsigned char)'\xa0') )
		{                      
			if( nChineseFlag )
				lWordNumber ++ ;
    		nChineseFlag ^= 1 ;
		}
		else if( isprint( c ) )
		{   
			if( n_TwoAsciiIsOneWord )
			{
				nAsciiCharFlag ++ ;
				if( nAsciiCharFlag >= 2 )
				{
					lWordNumber ++ ;
					nAsciiCharFlag = 0 ;
				}
			}
			else
			 	lWordNumber ++ ;
				 	
    		nChineseFlag = 0 ;
		}
		else
    		nChineseFlag = 0 ;
	}
	if (lWordNumber==0){
		lWordNumber=1;
		strcpy(szTitle," ");
	}
	*plWordNumber = lWordNumber ;
    return szTitle ;
}

wchar_t *GetTeleTitleWordW( LPBYTE hpszBuffer , long lBytesRead , int nTitleLine  , long *plWordNumber , long *plDelay )
{
//    static char szTitle[ TITLE_LEN + 1 ] = "" ;
    static wchar_t szTitle[ TITLE_LEN + 1 ] = L"" ;
    wchar_t  	*szTitlePtr= NULL ;
    wchar_t	 *cp ;
    int 	nChineseFlag = 0 , nGetTitle = 0 ;
    int		nAsciiArticleFlag = 1 ;
    long    lWordNumber = 0 , lLineStart ;
    register long l ;
    int  	nLineChar , nAsciiCharFlag = 0 ;
    wchar_t	c ;
    // Buffer
    long	lBufferSize = 0L ;

    lstrcpy( szTitle , L"" ) ;

    
    // Check if it is not a Chinese Article
    for( l = 0L ; l < lBytesRead ; l ++ )
    {
    	if( (unsigned int)hpszBuffer[ l ] > (unsigned int)'\xa0' )
    	{
    		nAsciiArticleFlag = 0 ;
    		break ;
    	}
    }
    
	
    // Get Title 
    szTitlePtr = szTitle ;

    nLineChar = 0 ;
    lLineStart = 0 ;
    for( l = 0L ; l < lBytesRead ; l ++ )
    {
		if( nLineChar >0 )				//2
		{
	    	if( nTitleLine <= 1 )
	    	{
				l = lLineStart ;
				break ;
	    	}
		}
		c = hpszBuffer[ l ] ;
		switch( c )
		{
	    	case '\x0a' :
				if( nLineChar > 2 )
		    		nTitleLine -- ;
				lLineStart = l + 1 ;
				nLineChar = 0 ;
				break ;
				
	    	case ' ' :
				break;
	    	case '\x0d' :
				 if( nLineChar > 2 )
		    		nTitleLine -- ;
				lLineStart = l + 1 ;
				nLineChar = 0 ;
				break;
	    	case '\t' :
				break ;
	    	default :
				if( (unsigned char)c > (unsigned char)'\xa0' )
				{
		    		nLineChar ++ ;
				}
				else if( nAsciiArticleFlag && iswprint( c ) )
				{
					nLineChar ++ ;
				}

//				nLineChar ++ ;
		}
    }

	// cut files before titleline
	cp  = (wchar_t *)(LPCWSTR)hpszBuffer + l ,
	lBytesRead -= l ;	
	*plDelay = l ;
	l = 0 ;
    for( ; l < lBytesRead ; l ++	)
    {
		c = cp[ l ] ;

		if( nGetTitle == 0 )
		{
	    	switch( c )
	    	{
				case ' ' :
				case '\x0d' :
				case '\x0a' :
		    		break ;
				default :
		    		if( nChineseFlag )
		    		{
						*szTitlePtr ++ = c ;
						nGetTitle ++ ;
						nChineseFlag ^= 1 ;
		    		}
		    		else if( (unsigned char)c > (unsigned char)'\xa0')
		    		{
						*szTitlePtr ++ = c ;
						nGetTitle ++ ;
						nChineseFlag ^= 1 ;
		    		}
		    		else if( iswprint( c ) )
		    		{
						*szTitlePtr ++ = c ;
						nGetTitle ++ ;
						nChineseFlag = 0 ;
		    		}
		    		else
						nChineseFlag = 0 ;
	    	}
		}
		else if( (c == '\x0d') || (c == '\x0a') )
	    	break ;
		else if( nChineseFlag || ((unsigned char)c > (unsigned char)'\xa0') )
		{
	    	*szTitlePtr ++ = c ;
	    	nGetTitle ++ ;
	    	nChineseFlag ^= 1 ;
		}
		else if( iswprint( c ) )
		{
	    	*szTitlePtr ++ = c ;
	    	nGetTitle ++ ;
	    	nChineseFlag = 0 ;
		}
		else
	    	nChineseFlag = 0 ;

		if( (nGetTitle >= TITLE_LEN - 2) && nChineseFlag == 0 )
	  		break ;
	  	else if ( (nGetTitle >= TITLE_LEN - 2) && nChineseFlag != 0 )
	  	{
	  		*szTitlePtr ++ = cp[ l +1 ] ;
	  		break;
	  	}
    }

    *szTitlePtr = '\0' ;
    
	
    // Get Word Number
    for( l = 0L ; l < lBytesRead ; l ++	)
	{
		c = cp[ l ] ;

		if( nChineseFlag || ((unsigned char)c > (unsigned char)'\xa0') )
		{                      
			if( nChineseFlag )
				lWordNumber ++ ;
    		nChineseFlag ^= 1 ;
		}
		else if( iswprint( c ) )
		{   
			if( n_TwoAsciiIsOneWord )
			{
				nAsciiCharFlag ++ ;
				if( nAsciiCharFlag >= 2 )
				{
					lWordNumber ++ ;
					nAsciiCharFlag = 0 ;
				}
			}
			else
			 	lWordNumber ++ ;
				 	
    		nChineseFlag = 0 ;
		}
		else
    		nChineseFlag = 0 ;
	}
	if (lWordNumber==0){
		lWordNumber=1;
		lstrcpy(szTitle,L" ");
	}
	*plWordNumber = lWordNumber ;
    return szTitle ;
}

BOOL DeleteDirectory1(LPWSTR lpszDir , LPWSTR lpszFileFormat )
{
  struct _wfinddata_t findt ;
  long nFindFlag ;
  CString	 szPath,szDirectory;

  if( lpszDir[ lstrlen( lpszDir) - 1 ] == '\\' )
    return FALSE ;

 // swprintf( szPath , L"%s\\%s" , lpszDir , lpszFileFormat ) ;
  szPath.Format(L"%s\\%s" , lpszDir , lpszFileFormat ) ;
  nFindFlag = _wfindfirst( szPath ,  &findt ) ;
  if( nFindFlag ==-1 )
	  return FALSE;
  else{
      if ( findt.attrib != _A_SUBDIR) 
		{
			szDirectory.Format(L"%s\\%s" , lpszDir , findt.name ) ;
			if( _wremove( szDirectory ) )
			{
				SetFileAttributes( szDirectory , FILE_ATTRIBUTE_NORMAL ) ;
				_wremove( szDirectory ) ;
			}
		}
    while(_wfindnext( nFindFlag , &findt )==0)
	{
		 if ( findt.attrib != _A_SUBDIR) 
		{
			szDirectory.Format( L"%s\\%s" , lpszDir , findt.name ) ;
			if( _wremove( szDirectory ) )
			{
				SetFileAttributes( szDirectory , FILE_ATTRIBUTE_NORMAL ) ;
				_wremove( szDirectory ) ;
			}
		}
	}
  }
  szPath.Format(L"%s\\*.*" , lpszDir ) ;
  nFindFlag = _wfindfirst( szPath ,  &findt ) ;
  if( nFindFlag ==-1 )
	  return FALSE;
  else{
      if( (findt.attrib & _A_SUBDIR) && ( wcsncmp( findt.name , L"." , 1 ) != 0 ) )
		{
			szDirectory.Format( L"%s\\%s" , lpszDir , findt.name ) ;
			DeleteDirectoryW( szDirectory , lpszFileFormat ) ;
		}
    while(_wfindnext( nFindFlag , &findt )==0)
	{
		 if( (findt.attrib & _A_SUBDIR) && ( wcsncmp( findt.name , L"." , 1 ) != 0 ) )
		{
			szDirectory.Format( L"%s\\%s" , lpszDir , findt.name ) ;
			DeleteDirectoryW(szDirectory , lpszFileFormat ) ;
		}
	}
  }
  szPath=lpszDir;
  _findclose(nFindFlag);

  if( _wrmdir( szPath ) == 0)
	return TRUE;
  else
    return FALSE;
}

BOOL FindKeyWordL ( wchar_t *szTitle , wchar_t *szKey )
{
	wchar_t szWord[100];
	wchar_t sztKey[100];
	wchar_t szTemp[100];
	int nFind;
	//char szTemp1[50];
	//CString strTemp=szKey;
	 CString strTemp;
	CString strKey1;
	CString strKey=szKey;
	strKey.TrimLeft();
	strKey.TrimRight();
	if((nFind=strKey.Find(','))!=-1)
	{
		if(szKey[nFind+1]==' ')
		{
			strTemp=strKey.Left(nFind+1)+strKey.Right(strKey.GetLength()-nFind-2);
			strKey=strTemp;
		}
		if(szKey[nFind-1]==' ')
		{
			strTemp=strKey.Left(nFind-1)+strKey.Right(strKey.GetLength()-nFind);
			strKey=strTemp;
		}
	}
	CString str_temp = strKey;
	const wchar_t* wc_tmp = str_temp;
	lstrcpy(szKey,wc_tmp);

	//strcpy(szTemp1,strKey);
	sztKey[strKey.GetLength()]='\0';
	strKey=sztKey;
	strKey1=sztKey;
	int i=0 , j=0 ,k=0;

	while ( szTitle[i] != '\0' )	
	{
		if ( szTitle[i] == ' ' || szTitle[i] == '-'||szTitle[i]==',' )
		{                                            
			if(szTitle[i-1]!=' '&&szTitle[i-1]!=',')
			{
				if(sztKey[j]=='\0')
				{
					szWord[j] = '\0' ;
					if ( lstrcmp ( wcsupr(szWord) , wcsupr (sztKey )) == 0 ) 
					{
						return TRUE ;
					}
					j = 0 ;
					CString str_temp = strKey1;
					const wchar_t* wc_tmp = str_temp;
					lstrcpy(szKey,wc_tmp);
					strKey=strKey1;
				}
				else
				{
					if ( sztKey[j] == ' ' || sztKey[j] == '-'||sztKey[j]==',' )
					{
						szWord[j] = '\0' ;
						wcsncpy(szTemp, sztKey,j+1);
						szTemp[j]='\0';
						if ( lstrcmp ( wcsupr(szWord) , wcsupr (szTemp )) == 0 ) 
						{
							strTemp=strKey.Right(strKey.GetLength()-j-1);
							//if(sztKey[
							//strTemp.TrimLeft();
							CString str_temp = strTemp;
							const wchar_t* wc_tmp = str_temp;
							lstrcpy(szKey,wc_tmp);
							strKey=strTemp;
						}
						else
						{
							CString str_temp = strKey1;
							const wchar_t* wc_tmp = str_temp;
							lstrcpy(szKey,wc_tmp);
							strKey=strKey1;
						}
						j=0;
					}
					else
					{
						j=0;
						CString str_temp = strKey1;
						const wchar_t* wc_tmp = str_temp;
						lstrcpy(szKey,wc_tmp);
						strKey=strKey1;
					}
				}
			}
			else
			{
				j=0;
			}

		}
		else  
		{
			szWord[j] = szTitle[i] ;
			j ++ ;
			if(j>99)
				j=0;
		}
		i ++ ;
	}
	if ( j != 0 )
	{
		szWord[j] = '\0' ;
		if ( lstrcmp ( _wcsupr ( szWord) , _wcsupr ( sztKey ) )== 0)
		{
			return TRUE ;
		}
	}   
	//strcpy(szTitle,"");
	return FALSE ;
}

void TransStringToDB(wchar_t *szString )
{               
	int nLen = lstrlen(szString );
//	if ( strstr ( (const char *)szString , "'" ) == NULL && strstr ( (const char *)szString , "\'" ) == NULL )
	if ( wcsstr ( szString , L"'" ) == NULL && wcsstr ( szString , L"\'" ) == NULL )
		return ;	
	int i = 0 ;		
	for ( i = 0 ; i < nLen ; i ++ )
	{
		if ( szString[i] == '\x27' ) // char = ' to `
			szString [i] = '`' ;
		else if ( szString[i] == '\x22' ) // char = "  to ~
			szString[i] = '~' ;
	}
	return ; 
}
/*
BOOL DeleteDirectoryC(LPSTR lpszDir , LPSTR lpszFileFormat )
{
  struct _finddata_t findt ;
  long nFindFlag ;
  char	 szPath[ 255 ] , szDirectory[ 255 ] ;

  if( lpszDir[ lstrlen( lpszDir) - 1 ] == '\\' )
    return FALSE ;

  wsprintf( szPath , "%s\\%s" , lpszDir , lpszFileFormat ) ;

  nFindFlag = _findfirst( szPath ,  &findt ) ;
  if( nFindFlag ==-1 )
	  return FALSE;
  else{
      if ( findt.attrib != _A_SUBDIR) 
	  {
			wsprintf( szDirectory , "%s\\%s" , lpszDir , (LPSTR)findt.name ) ;
			if( remove( szDirectory ) )
			{
				SetFileAttributes( szDirectory , FILE_ATTRIBUTE_NORMAL ) ;
				remove( szDirectory ) ;
			}
	  }
    while(_findnext( nFindFlag , &findt )==0)
	{
		 if ( findt.attrib != _A_SUBDIR) 
		{
			wsprintf( szDirectory , "%s\\%s" , lpszDir , (LPSTR)findt.name ) ;
			if( remove( szDirectory ) )
			{
				SetFileAttributes( szDirectory , FILE_ATTRIBUTE_NORMAL ) ;
				remove( szDirectory ) ;
			}
		}
	}
  }

  wsprintf( szPath , "%s\\*.*" , lpszDir ) ;
   _findclose(nFindFlag);
  nFindFlag = _findfirst( szPath ,  &findt ) ;
  if( nFindFlag ==-1 )
	  return FALSE;
  else{
      if( (findt.attrib & _A_SUBDIR) && ( strncmp( findt.name , "." , 1 ) != 0 ) )
		{
			wsprintf( szDirectory , "%s\\%s" , lpszDir , (LPSTR)findt.name ) ;
			DeleteDirectory( szDirectory , lpszFileFormat ) ;
		}
    while(_findnext( nFindFlag , &findt )==0)
	{
		 if( (findt.attrib & _A_SUBDIR) && ( strncmp( findt.name , "." , 1 ) != 0 ) )
		{
			wsprintf( szDirectory , "%s\\%s" , lpszDir , (LPSTR)findt.name ) ;
			DeleteDirectory( szDirectory , lpszFileFormat ) ;
		}
	}
  }

  lstrcpy( szPath , lpszDir ) ;
  _findclose(nFindFlag);

  if( _rmdir( szPath ) == 0)
	return TRUE;
  else
    return FALSE;
}
*/
BOOL FindFileFormat ( wchar_t * szFile , char * szFormat )
{                             
   wchar_t drive[_MAX_DRIVE];
   wchar_t dir[_MAX_DIR];
   char szFileName[_MAX_FNAME];
   wchar_t szFileExt[_MAX_EXT];
   wchar_t szStrName[_MAX_FNAME];
   wchar_t szStrExt[_MAX_EXT];
	long lFile = 0L , lFormat = 0L ;
	long lPos = 0  , lMin;
	
   _splitpath( (const char *)szFile , (char *)drive, (char *)dir, szFileName, (char *)szFileExt );
   _splitpath( szFormat , (char *)drive, (char *)dir, (char *)szStrName, (char *)szStrExt );
	lFile = lstrlen ((LPCWSTR) szFileName );
	lFormat = lstrlen ( szStrName );
	lPos = 0 ;        
	_strupr((char *)szFileName);
	if ( strrchr ( (const char *)szStrName , '*' ) == NULL && lFile != lFormat )
		return FALSE ;
	lMin = min ( lFile , lFormat );
	while ( lPos < lMin )
	{               
		if ( szStrName[lPos] == '*' )
			break;
		if ( szStrName[lPos] == '?' || szFileName[lPos] == szStrName[lPos] )
			lPos ++ ;
		else
			return FALSE ;
	}                             
	if ( lFile < lFormat )
	{
		if ( szStrName[lMin] != '*' )
			return FALSE ;
	}
	if ( lFile > lFormat )
	{
		if ( strrchr ( (const char *)szStrName , '*' ) == NULL )
			return FALSE ;
	}                     
	
	//ext
	lFile = lstrlen ( szFileExt );
	_strupr((char *)szFileExt);
	lFormat = lstrlen ( szStrExt );
	lPos = 0 ;
	if ( strrchr ( (const char *)szStrExt , '*' ) == NULL && lFile != lFormat )
		return FALSE ;
	lMin = min ( lFile , lFormat );
	while ( lPos < lMin )
	{               
		if ( szStrExt[lPos] == '*' )
			break;
		if ( szStrExt[lPos] == '?' || szFileExt[lPos] == szStrExt[lPos] )
			lPos ++ ;
		else
			return FALSE ;
	}                             
	if ( lFile < lFormat )
	{
		if ( szStrExt[lMin] != '*' )
			return FALSE ;
	}
	if ( lFile > lFormat )
	{
		if ( strrchr ( (const char *)szStrExt , '*' ) == NULL )
			return FALSE ;
	}                          
	return TRUE ;
		
}
void ChangeFileExt(wchar_t *szFile, wchar_t const *szExt)
{
	wchar_t *pExt;
	pExt = wcsstr(szFile + lstrlen( szFile) - 4, L".");
	if(pExt)
		*pExt = '\0';
	if(szExt[0] == '.')
	{
		lstrcat(szFile, szExt);
	}
	else
	{
		lstrcat(szFile, L".");
		lstrcat(szFile, szExt);
	}
}


char *GetTeleFileTitleWord_Ansi( LPWSTR lpszFile, int nTitleLine, long *plWordNumber)
{
	static char szTitle[ TITLE_SIZE+1];
//	static wchar_t szTitle[ TITLE_SIZE+1];
	memset(szTitle, 0, TITLE_SIZE+1);

	CFile   fil ;
	
	if( plWordNumber )
		*plWordNumber = 0L ;

	if ( fil.Open( lpszFile , CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) == 0 )
    {		
		return szTitle ;
    }
	
    DWORD lBufferSize = (DWORD)fil.GetLength();
	if( lBufferSize > 60000L )
		lBufferSize = 60000L;
	
	LPBYTE lpszUnix = (LPBYTE)malloc(lBufferSize+1 ) ;
    if( lpszUnix == NULL )
    {
		fil.Close() ;
		return szTitle ;
    }
	
    memset(lpszUnix, 0, lBufferSize+1);
	
    lBufferSize = fil.Read( lpszUnix , lBufferSize ) ;
	fil.Close();
	lpszUnix[lBufferSize]=0;
	
	LPBYTE lpszDos = Unix2Dos(lpszUnix);
	free(lpszUnix);

	if (lpszDos)
	{
		lBufferSize = strlen((const char*)lpszDos);
		long lDelay = 0;

		lstrcpy((LPWSTR)szTitle, 
			GetTeleTitleWordW(lpszDos, lBufferSize, nTitleLine, plWordNumber, &lDelay));
		free(lpszDos);
	}

	return szTitle ;
}


LPBYTE Unix2Dos(LPBYTE szUnix)
{
	LPBYTE pDos = NULL;
	int len = strlen((const char *)szUnix);

	int nLine = 0;
	int i=0;
	for (i=0; i<len; i++){
		if (szUnix[i]=='\n' && (i==0 || (i>0 && szUnix[i-1]!='\r') ))
			nLine++;
	}

	pDos = (LPBYTE)malloc(len+nLine+2);

	if (pDos){		
		memset(pDos, 0, len+nLine+2);
		
		int j =0;
		
		for (i=0; i<len; i++){
			if (szUnix[i]=='\n' && (i==0 || (i>0 && szUnix[i-1]!='\r') ))
				pDos[j++] = '\r';
			pDos[j++] = szUnix[i];
		}
	}
	return pDos;
}

