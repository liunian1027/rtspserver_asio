//Class CHttpSocket
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <assert.h>

#include "CHttpSocket.h"
#define MAXLINE 1024

CHttpSocket::CHttpSocket()
{
	
}

CHttpSocket::~CHttpSocket()
{

}

int CHttpSocket::createSocket()
{
	return socket(AF_INET, SOCK_STREAM,0);
}

bool CHttpSocket::bindSocket(int sockfd,  int const port)
{
	if (sockfd == -1)
	{
		return false;
	}
	
	m_servaddr.sin_family = AF_INET;
	m_servaddr.sin_port = htons(port);
	m_servaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(m_servaddr.sin_zero),8);
	
	return bind(sockfd, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr))  != -1;
}

bool CHttpSocket::listenSocket(int sockfd)
{
	if (listen(sockfd, 20) == -1)
	{
		return false;
	}
	
	printf("Accepting connections ...\n");
	return true;
}

bool CHttpSocket::connectSocket(int sockfd, char const*  ip, int  serPort)
{
	bzero(&m_servaddr, sizeof(m_servaddr));
	m_servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &m_servaddr.sin_addr);
	m_servaddr.sin_port = htons(serPort);
    
	if(connect(sockfd, (struct sockaddr *)&m_servaddr, sizeof(m_servaddr)) == -1)
	{
		printf("Connect failed\r\n");
		return false;	
	}

	return true;
}

//接受头

int CHttpSocket::readHeader(int sockfd,  char* buf)
{	
	int lenfd=0;
	char ch;
	int flag=0;
	int len = 0;

	while(1)
	{
		if ((lenfd = recv(sockfd, &ch,1 ,0)) > 0)
		{			
			buf[len] = ch;
			++len;

			if (len >= MAX_LENGTH)
			{
				assert(0);
			}
			
			if (ch == '\r' ||ch == '\n' )
			{
				flag++;			
			}
			else
			{
				flag=0;
			}

			if(flag==4)
			{
				break;
			}

		}
		else 
		{	
			perror("read failed!\r\n");
			return -1;
		}

	}
	
	printf("Http head:  \n%s\n", buf);
	
	return len;
}

//接受数据
int CHttpSocket::readData(int sockfd, int bodylen, char* buf)
{
	int ret  = 0;

	if ((ret = recv(sockfd, buf, bodylen,0)) <= 0)
	{	
		perror("Data Read failed!\n");
		return -1;
	}

	return ret;
}




//发送头
int CHttpSocket::sendData(int sockfd, const char* buf, int len)
{

	if (send(sockfd, buf, len, 0) == -1)
	{
		perror("Head sendfailed!\n");
		return -1;
	}
	
	printf("Head send success!\n");
	return 0;
}	

int CHttpSocket::sentFile( char* path)	
{

	return 0;
}

