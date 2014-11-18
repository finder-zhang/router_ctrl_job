//============================================================================
// Name        : rtr_demo.cpp
// Author      : finder
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
using namespace std;

#include "Process_F.h"
#include "Socket_F.h"
#include "Thread_F.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/un.h>


const char* PROC_NAME = "/root/workspace/router_proj/zbc/Debug/zbc";


void* ThreadClient(void* pData)
{
	char ch[1024];
	char chWrite[1024];
	CTcp_F* pTcp = (CTcp_F*)pData;
	const sockaddr_in& sin = pTcp->GetInetAddr();
	printf("get client ip = %s,port = %d\r\n",inet_ntoa(sin.sin_addr),sin.sin_port);

	do
	{
		int iRead = pTcp->Read(ch,1024);
		int iWrite = sprintf(chWrite,"server repeat %s\r\n",ch);
		pTcp->Write(chWrite,iWrite);
		printf("Get client: %s\r\n",ch);
	}while(1);
	return NULL;
}

void* ThreadAccept(void* pData)
{
	CSocket_F* pSock = (CSocket_F*)pData;

	while(1) {
		CTcp_F* pClient = pSock->Accept();
		if (pClient) {
			CThread_F thClient;
			thClient.Create(ThreadClient,pClient);
		}
		else {
			printf("accept error,number = %d,%s\r\n",errno,strerror(errno));
		}
	};
	return NULL;
}

int main(int argc,char* argv[]) {

	int zbcIn,zbcOut;

	char chRead[100];		//use to test the pipe's input and output
	char chWrite[100];

	for (int i=0;i<argc;++i) {
		printf("%s\r\n",argv[i]);
	}

	printf("start------------------\r\n");


	CSocket_F sock;
	sock.Open(AF_INET,SOCK_STREAM,0);
	sock.TcpBind("192.168.1.251",9001,3);

	CThread_F thAccept;
	thAccept.Create(ThreadAccept,&sock);


	printf("++++++++\r\n");

	while(1);

	return 0;
}
