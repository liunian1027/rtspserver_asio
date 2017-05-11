//Class CClient.h
#ifndef _CCLIENT_H_
#define _CCLIENT_H_

class CClient
{	
public:
	CClient();
	~CClient();
	int Start(char const* ip, char const* port ,char const* path);
	int Stop();
	
private:
	int sockfd;	
};
#endif

