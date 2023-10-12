/*
	smtp.h - declarations of structures and functions to send smtp mails

	still under construction...
	
	Written by: Leandro Vaz
*/
#pragma   comment(lib,   "ws2_32.lib ") 
#ifndef _SMTP_H_
#define _SMTP_H_


#ifndef _WINSOCK2API_
#	include <winsock2.h>
#endif

#ifndef _INC_STDLIB
#	include <stdlib.h>
#endif



#define ERROR_OPEN 101
#define HELP_REPLY 211
#define HELP_MSG 214
#define SERVICE_RDY 220
#define SERVICE_CLOSE 221
#define RQ_MAIL_OK 250
#define USER_NOT_LOCAL_FWD 251
#define ERROR_MAIL_DOT 354
#define NOT_AVAILABLE 421
#define RQ_MAIL_ERROR 450
#define RQ_ABORTED 451
#define RQ_NOT_TAKEN 452
#define UNKNOW_CMD 500
#define SYNTAX_ERROR 501
#define NOT_CMD 502
#define CMD_ERROR 503
#define NOT_CMD_PARAM 504
#define NOT_FOUND 550
#define USER_NOT_LOCAL 551
#define ACTION_ABORTED 552
#define NOT_ALLOWED 553
#define TRANS_FAILED 554

#define SOCK_ERROR -1
#define SEND_OK 1
#define SEND_ERROR 0
#define SMTP_OK 1
#define SMTP_ERROR 0

#define SMTP_PORT 25


typedef unsigned short port_t;
typedef int smtp_error_t;
typedef struct hostent SMTP_HOST;


typedef struct{
	CString from;
	CString to;
	char* header;
	CString message;
}SMTP_MAIL;

typedef struct{
	port_t port;
	SMTP_HOST* host;
}SMTP_CLIENT;


smtp_error_t GetSmtpResponseCode(char* buffer);
smtp_error_t GetSmtpResponseType(smtp_error_t code);
smtp_error_t SendSmtpMail(SMTP_CLIENT* SMTPClient, SMTP_MAIL* SMTPMail);


smtp_error_t GetSmtpResponseCode(char* buffer)
{
	char response[3];

	if(strlen(buffer) > 3)
	{
		memcpy(response, buffer, 3);
		return atoi(response);
	}

	return 0;
}

smtp_error_t GetSmtpResponseType(smtp_error_t code)
{
	switch(code)
	{
	case SERVICE_RDY: case SERVICE_CLOSE: case RQ_MAIL_OK: case USER_NOT_LOCAL_FWD:
	case ERROR_MAIL_DOT:
		return SMTP_OK;
	default:
		return SMTP_ERROR;
	}
}

smtp_error_t SendSmtpMail(SMTP_CLIENT* SMTPClient, SMTP_MAIL* SMTPMail)
{
	SOCKET smtp_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in smtp_host;
	smtp_error_t smtp_server_response;
	char buffer[1024];

	if(smtp_sock == -1) 
		return SOCK_ERROR;

	smtp_host.sin_family = AF_INET;
	smtp_host.sin_port = htons(SMTPClient->port);
	smtp_host.sin_addr.S_un.S_addr = (*(unsigned long *) SMTPClient->host->h_addr_list[0]);
//	AfxMessageBox(SMTPClient->host->h_addr_list[0]);
	if(connect(smtp_sock, (struct sockaddr*) &smtp_host, sizeof(struct sockaddr)) != S_OK)
		return SOCK_ERROR;

	recv(smtp_sock, buffer, 1024, 0);

// 	strcpy(buffer, "MAIL FROM: ");
// 	strcat(buffer, SMTPMail->from);
// 	strcat(buffer, "\r\n");
// 	send(smtp_sock, buffer, strlen(buffer), 0);
// 	recv(smtp_sock, buffer, 1024, 0);
// 	smtp_server_response = GetSmtpResponseCode(buffer);
// 	if(GetSmtpResponseType(smtp_server_response) == SMTP_ERROR)
// 		return smtp_server_response;
// 
// 	strcpy(buffer, "RCPT TO: ");
// 	strcat(buffer, SMTPMail->to);
// 	strcat(buffer, "\r\n");
// 	send(smtp_sock, buffer, strlen(buffer), 0);
// 	recv(smtp_sock, buffer, 1024, 0);
// 	smtp_server_response = GetSmtpResponseCode(buffer);
// 	if(GetSmtpResponseType(smtp_server_response) == SMTP_ERROR)
// 		return smtp_server_response;
// 
// 	strcpy(buffer, "DATA \r\n");
// 	send(smtp_sock, buffer, strlen(buffer), 0);
// 	recv(smtp_sock, buffer, 1024, 0);
// 	smtp_server_response = GetSmtpResponseCode(buffer);
// 	if(GetSmtpResponseType(smtp_server_response) == SMTP_ERROR)
// 		return smtp_server_response;
// 
// 	strcpy(buffer, "From: ");
// 	strcat(buffer, SMTPMail->from);
// 	strcat(buffer, "\r\n");
// 	send(smtp_sock, buffer, strlen(buffer), 0);
// 
// 	char buffer1[1024];
// 	strcpy(buffer1, "Body: ");
// 	strcpy(buffer1, SMTPMail->message);
// 	strcat(buffer1, "\r\n");
// 	send(smtp_sock, buffer1, strlen(buffer1), 0);
// 
// 	strcpy(buffer, "To: ");
// 	strcat(buffer, SMTPMail->to);
// 	strcat(buffer, "\r\n");
// 	send(smtp_sock, buffer, strlen(buffer), 0);
// 
// 	strcpy(buffer, "Subject: ");
// 	strcat(buffer, SMTPMail->header);
// 	strcat(buffer, "\r\n");
// 	send(smtp_sock, buffer, strlen(buffer), 0);
// 	
// 	strcpy(buffer, "Body: ");
// 	strcpy(buffer, SMTPMail->message);
// 	strcat(buffer, "\r\n");
// 	send(smtp_sock, buffer, strlen(buffer), 0);
// 
// 
// 	send(smtp_sock, ".\r\n", 3, 0);
// 
// 	recv(smtp_sock, buffer, 1024, 0);

	
	TCHAR buffer_new[1024];	
	char temp1[1024]={0};
	char temp2[1024]={0};
	CString strTmp;
	strTmp = (CString)SMTPMail->from;	
	wsprintf(buffer_new,_T("%s"),strTmp); 
	ZeroMemory(temp1, sizeof(temp1));
	WideCharToMultiByte(CP_ACP, 0, buffer_new, -1, temp1, sizeof(temp1), NULL, NULL);
	
	
	//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
// 	strcpy(buffer, "MAIL FROM: ");
// 	strcat(buffer, temp1);
// 	strcat(buffer, "\r\n");
	// send From

	strcpy(buffer, "helo");
	strcat(buffer, "\r\n");
	send(smtp_sock, buffer, strlen(buffer), 0);
	//AfxMessageBox(CString(buffer));
	recv(smtp_sock, buffer, 1024, 0);
	smtp_server_response = GetSmtpResponseCode(buffer);
	if(GetSmtpResponseType(smtp_server_response) == SMTP_ERROR)
		return smtp_server_response;

	strcpy(buffer, "auth login");
	strcat(buffer, "\r\n");
	send(smtp_sock, buffer, strlen(buffer), 0);
	//AfxMessageBox(CString(buffer));
	recv(smtp_sock, buffer, 1024, 0);
	smtp_server_response = GetSmtpResponseCode(buffer);
	if(GetSmtpResponseType(smtp_server_response) == SMTP_ERROR)
		return smtp_server_response;

	
 	strcpy(buffer, "MAIL FROM: ");
	strcat(buffer, temp1);
 	strcat(buffer, "\r\n");
 	send(smtp_sock, buffer, strlen(buffer), 0);
	//AfxMessageBox(CString(buffer));
 	recv(smtp_sock, buffer, 1024, 0);
 	smtp_server_response = GetSmtpResponseCode(buffer);
 	if(GetSmtpResponseType(smtp_server_response) == SMTP_ERROR)
			return smtp_server_response;


	// send to 
	strTmp = (CString)SMTPMail->to;	
	wsprintf(buffer_new,_T("%s"),strTmp); 
	ZeroMemory(temp2, sizeof(temp2));
	WideCharToMultiByte(CP_ACP, 0, buffer_new, -1, temp2, sizeof(temp2), NULL, NULL);
	
	//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
// 	strcpy(buffer, "RCPT TO: ");
// 	strcat(buffer, temp2);
// 	strcat(buffer, "\r\n");
	strcpy(buffer, "RCPT TO: ");
	strcat(buffer, temp2);
	strcat(buffer, "\r\n");
	send(smtp_sock, buffer, strlen(buffer), 0);
//	AfxMessageBox(CString(buffer));
	recv(smtp_sock, buffer, 1024, 0);

	smtp_server_response = GetSmtpResponseCode(buffer);
	if(GetSmtpResponseType(smtp_server_response) == SMTP_ERROR) 		
		return smtp_server_response;

	//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
	//strcpy(buffer, "DATA\r\n");
	strcpy_s(buffer, "DATA\r\n");
	send(smtp_sock, buffer, strlen(buffer), 0);
	recv(smtp_sock, buffer, 1024, 0);
	smtp_server_response = GetSmtpResponseCode(buffer);
	if(GetSmtpResponseType(smtp_server_response) == SMTP_ERROR)
		return smtp_server_response;
	//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
// 	strcpy(buffer, "From: ");
// 	strcat(buffer, temp1);
// 	strcat(buffer, "\r\n");
	strcpy_s(buffer, "From: ");
	strcat_s(buffer, temp1);
	strcat_s(buffer, "\r\n");
	send(smtp_sock, buffer, strlen(buffer), 0);

	//edit by leon 2012-08-13 for convert DBgraphic from VC 6.0 to 2010
	//strcpy(buffer, "To: ");
	//strcat(buffer, temp2);
	// trcat(buffer, "\r\n");

	strcpy_s(buffer, "To: ");
	strcat_s(buffer, temp2);
	strcat_s(buffer, "\r\n");
	send(smtp_sock, buffer, strlen(buffer), 0);
//	AfxMessageBox(CString(buffer));
	// Setting Subject and Body

	wsprintf(buffer_new,_T("Subject: %s\r\n\r\n%s\r\n.\r\n"),(CString)SMTPMail->header, SMTPMail->message); //message & subject
	char temp[1024]={0};
	ZeroMemory(temp, sizeof(temp));
	int len = WideCharToMultiByte(CP_ACP, 0, buffer_new, -1, temp, sizeof(temp), NULL, NULL);

	send(smtp_sock, temp, strlen(temp), 0);
//	send(smtp_sock, buffer, strlen(buffer), 0);


	send(smtp_sock, ".\r\n", 3, 0);
	
	recv(smtp_sock, buffer, 1024, 0);


	smtp_server_response = GetSmtpResponseCode(buffer);
	if(GetSmtpResponseType(smtp_server_response) == SMTP_ERROR)
		return smtp_server_response;

	return SEND_OK;
}


#endif //_SMTP_H_