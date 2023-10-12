//*******************************************************************
//
// Copyright (c) 2006 by Wen Lin, NUAA-nrc
// All Rights Reserved.
// file:smtp.cpp
/*************************************************************/
#include "StdAfx.h"
#include "smtp.h"

CSmtp::CSmtp()
{
	From      = new char[40];
	To        = new char[40];
	Date      = new char[40];
	Subject   = new char[40];
	Filename  = new char[100];

	recv_data = new char[512];
	userdes   = new char[40];
	passdes   = new char[40];
	port = 25;
	s    = INVALID_SOCKET;
}
CSmtp::~CSmtp()
{
	if(s!=INVALID_SOCKET)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		WSACleanup();
	}

	delete []From;
	delete []To;
	delete []Date;
	delete []Subject;
	delete []Filename;

	delete []recv_data;
	delete []userdes;
	delete []passdes;

	
}
void CSmtp::Base64_Code( unsigned char *chsrc, unsigned char *chdes)
{
	char chadd[3];
	unsigned char temp[4],t;
	
	int len,i;
	len = strlen((char *)chsrc);
	
    while(len>=3)
	{
		temp[0] = (*chsrc)>>2;
		t       = (*chsrc & 0x03)<<4;
		temp[1] = (*(chsrc+1)>>4)|t;
		t       = ((*(chsrc+1)<<2) & 0x3f);
		temp[2] = (*(chsrc+2)>>6)|t;
		temp[3] = (*(chsrc+2) & 0x3f);

		for(i=0;i<4;i++)
		{
			if(temp[i]>=0 && temp[i]<=25) 
				*(chdes+i) = temp[i]+65;
			if(temp[i]>=26 && temp[i]<=51) 
				*(chdes+i) = temp[i]+71;
			if(temp[i]>=52 && temp[i]<=61) 
				*(chdes+i) = temp[i]-4;
			if(temp[i]==62) 
				*(chdes+i) = 43;
			if(temp[i]==63) 
				*(chdes+i) = 47;
		}
        
		len -= 3;
		chsrc += 3;
		chdes += 4;
	}
    if(len!=0)
	{
		for(i=0;i<3;i++)
			chadd[i] = 0;
        memcpy(chadd,chsrc,len);

		temp[0] = chadd[0]>>2;
		t       = (chadd[0] & 0x03)<<4;
		temp[1] = (chadd[1]>>4)|t;
		t       = ((chadd[1]<<2) & 0x3f);
		temp[2] = (chadd[2]>>6)|t;
		temp[3] = chadd[2] & 0x3f;

		for(i=0;i<4;i++)
		{
			if(temp[i]>=0 && temp[i]<=25 && (i==0 || i==1)) *(chdes+i) = temp[i]+65;
			else  *(chdes+i) = 61;
			if(temp[i]>=26 && temp[i]<=51) *(chdes+i) = temp[i]+71;
			else if(temp[i]>=52 && temp[i]<=61) *(chdes+i) = temp[i]-4;
			else if(temp[i]==62) *(chdes+i) = 43;
			else if(temp[i]==63) *(chdes+i) = 47;
		}
		chdes += 4;
		*chdes = '\0';
		return;
	}
	*chdes = '\0';
}

void CSmtp::Base64_Decode( unsigned char *chsrc, unsigned char *chdes)
{
	unsigned char temp[4],t;
	int len,i;
	len = strlen((char *)chdes);
	while(len>=4)
	{
		for(i=0;i<4;i++)
		{
			if(*(chdes+i)>=65 && *(chdes+i)<=90) 
				 temp[i] = *(chdes+i)-65;
			if(*(chdes+i)>=97 && *(chdes+i)<=122) 
				temp[i] = *(chdes+i)-71;
			if(*(chdes+i)>=48 && *(chdes+i)<=57) 
				temp[i] = *(chdes+i)+4;
			if(*(chdes+i)==43) 
				temp[i] = 62;
			if(*(chdes+i)==47) 
				temp[i] = 63;
			if(*(chdes+i)==61)
				temp[i] = 0;
		}
		t = (temp[1]>>4)&0x03;
        *chsrc = (temp[0]<<2)|t;
		t = (temp[2]>>2)&0x0f;
		*(chsrc+1) = (temp[1]<<4)|t;
		t = temp[3];
		*(chsrc+2) = (temp[2]<<6)|t;

		chsrc += 3;
		chdes += 4;
		len   -= 4;
	}
	chsrc -= 3;
	for(i=0;i<3;i++)
	{
		if(*(chsrc+i) == 0)
		{
			*(chsrc+i) = '\0';
			break;
		}
	}
	if(i>=2)
		*(chsrc+3) = '\0';
}
BOOL CSmtp::GetResponse()
{
	
	try{
		rt = recv(s,recv_data,1024,0);
		if(rt == SOCKET_ERROR)
		{
			return FALSE;
		}
		recv_data[rt]='\0';
		dbMsg("recv is %s\n",recv_data);
	}
	catch(...)
	{
		return FALSE;
	}
	if(*recv_data == '5')
		return FALSE;
	return TRUE;
}
BOOL CSmtp::CreateSocket()
{
	rt = WSAStartup(MAKEWORD(2,2),&wsa);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	s = socket(AF_INET,SOCK_STREAM,0);
	if(s == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	long t = 5000;
	if(setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(char *)&t,sizeof(long)) == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	return TRUE;
}
BOOL CSmtp::ConnectHost(const char *hostname)
{
	if(hostname == NULL)
		return FALSE;
	ht=gethostbyname(hostname);
	if(ht==NULL)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	if(ht->h_addr_list[0] == NULL)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	memset(&remote,0,sizeof(remote));
	remote.sin_family = AF_INET;
	remote.sin_port   = htons(port);
	remote.sin_addr.S_un.S_addr = *(ULONG *)ht->h_addr_list[0];

	rt = connect(s,(sockaddr *)&remote,sizeof(remote));
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
    
	if(!GetResponse())
		return FALSE;
	return TRUE;
}
BOOL CSmtp::LogIn(char *username,char *password)
{
	char ch[100];
	if(username == NULL || password == NULL)
		return FALSE;
	usersrc = username;
	passsrc = password;
	Base64_Code((unsigned char *)usersrc,(unsigned char *)userdes);
	Base64_Code((unsigned char *)passsrc,(unsigned char *)passdes);

	send_data = "EHLO Localhost\r\n";
	
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}


	if(!GetResponse())
		return FALSE;
    
    send_data = "AUTH LOGIN\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	if(!GetResponse())
		return FALSE;
	
    wsprintf(ch,"%s\r\n",userdes);
	rt = send(s,(char *)ch,strlen(ch),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	if(!GetResponse())
		return FALSE;

	
	wsprintf(ch,"%s\r\n",passdes);
	dbMsg("ch = %s\n",ch);
	rt = send(s,(char *)ch,strlen(ch),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	if(!GetResponse())
		return FALSE;

	return TRUE;
}
BOOL CSmtp::SendMail(const char *from,const char *to,const char *date,const char *subject,const char *data)
{
	if(from == NULL || to == NULL || date == NULL || subject == NULL)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	wsprintf(From,"MAIL FROM: <%s>\r\n",from);
	rt = send(s,From,strlen(From),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	if(!GetResponse())
		return FALSE;

	wsprintf(To,"RCPT TO: <%s>\r\n",to);
	rt = send(s,To,strlen(To),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	if(!GetResponse())
		return FALSE;

	send_data = "DATA\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	if(!GetResponse())
		return FALSE;

	wsprintf(From,"From: %s\r\n",from);
	rt = send(s,From,strlen(From),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
    
	wsprintf(To,"To: %s\r\n",to);
	rt = send(s,To,strlen(To),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	

	wsprintf(Date,"Date: %s\r\n",date);
	rt = send(s,Date,strlen(Date),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	wsprintf(Subject,"Subject: %s\r\n",subject);
	rt = send(s,Subject,strlen(Subject),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	send_data = "MIME-Version: 1.0\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	send_data = "X-Mailer: SMail by WenLin\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	send_data = "Content-type: multipart/mixed; boundary=\"wenlin\"\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	send_data = "--wenlin\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	send_data = "Content-Type: text/plain; charset=GB2312\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	send_data = "Content-Transfer-Encoding: quoted-printable\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	send_data = "\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	rt = send(s,(char *)data,strlen(data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	send_data = "\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	return TRUE;
}
BOOL CSmtp::SendAffix(const char *filename,const int length)
{
	int  len;
	FILE *fp=NULL;
	unsigned char *datasrc,*datades;
	fp = fopen(filename,"rb");
	if(fp==NULL)
	{
		dbMsg("can not open file\n");
		return FALSE;
	}

	datasrc = new unsigned char[511];
	datades = new unsigned char[1024];
	send_data = "--wenlin\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		dbMsg("send error\n");
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	wsprintf(Filename,"Content-Type: application/octet-stream; name=%s\r\n",filename);
	rt = send(s,Filename,strlen(Filename),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	wsprintf(Filename,"Content-Disposition: attachment; filename=%s\r\n",filename);
	rt = send(s,Filename,strlen(Filename),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	send_data = "Content-Transfer-Encoding: base64\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	send_data = "\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	
	len = length;
	while(!feof(fp))
	{
	
		if(len>=510)
		{
          fread(datasrc,510,1,fp);
		  datasrc[510]='\0';
		  len -= 510;
		}
		else if(len>0)
		{
          fread(datasrc,len,1,fp);
		  datasrc[len]='\0';
		  len = 0;
		}
		else
			break;
		Base64_Code((unsigned char *)datasrc,(unsigned char *)datades);
        rt = send(s,(char *)datades,strlen((char *)datades),0);
		if(rt == SOCKET_ERROR)
		{
			shutdown(s,SD_BOTH);
			closesocket(s);
			return FALSE;
		}
	}

	fclose(fp);
	send_data = "\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	delete []datasrc;
	delete []datades;
	return TRUE;
}
BOOL CSmtp::End()
{
	send_data = "\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	send_data = "--wenlin--\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}

	send_data = "\r\n.\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	if(!GetResponse())
		return FALSE;

    send_data = "QUIT\r\n";
	rt = send(s,send_data,strlen(send_data),0);
	if(rt == SOCKET_ERROR)
	{
		shutdown(s,SD_BOTH);
		closesocket(s);
		return FALSE;
	}
	if(!GetResponse())
		return FALSE;
	return TRUE;
}

void CSmtp::Abort()
{
	
}