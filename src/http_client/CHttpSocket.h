//Class CHttpSocket.h

#ifndef _CHTTPSOCKET_H_
#define _CHTTPSOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_LENGTH 1*1024

class CHttpSocket
{
public:
	CHttpSocket();
	 ~CHttpSocket();
	int createSocket();
	bool bindSocket(int sockfd, int const port);
	bool listenSocket(int sockfd);
	bool connectSocket(int sockfd, char const*  ip, int  serPort);
	int readHeader(int sockfd, char* buf);
	int readData(int sockfd,int bodylen,char* buf);
	int sendData(int sockfd, const char* buf, int len);
	int sentFile(char* path);		

private:
	struct sockaddr_in m_servaddr;
};
#endif
