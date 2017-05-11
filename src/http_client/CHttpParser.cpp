//CHttpParsser.cpp
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <algorithm>


#include "SDataType.h"
#include "CHttpParser.h"

CHttpParser::CHttpParser()
{
	
}

CHttpParser::~CHttpParser()
{
	
}

int CHttpParser::parser(char* headbuf, int len)
{		
	char content[] = "Content-Length: ";
	char crlf[] = "\r\n";	
	char* pLengthbegin = NULL;
	//char* pLengthend = NULL;
	char* pContentend = NULL;
	char* pContentbegin = std::search(&headbuf[0],&headbuf[len],
						&content[0],&content[sizeof(content)-1]);
	
	if (pContentbegin == &headbuf[len] )
	{
		printf("Head Parser failed!\n");
		return -1;
	}
	
	pLengthbegin = pContentbegin+ sizeof(content)-1;

	pContentend = std::search(pLengthbegin, &headbuf[len], &crlf[0], &crlf[sizeof(crlf)-1]);

	
	if (pContentend== &headbuf[len] )
	{
		printf("Head Parser failed\n");
		return -1;
	}

	char value_buff[10];
	
	memset(value_buff, 0, sizeof(value_buff));

	//int length = pContentend -pLengthbegin;

	memcpy(value_buff, pLengthbegin, (int)pContentend -(int)pLengthbegin);

	printf("Head Parser success!\r\n");

	return atoi(value_buff);
		
}

std:: string  CHttpParser::formatHeader( struct HTTP_TYPE headdf)
{
	std::string buf;

	switch(headdf.http_method)
	{
		case M_GET:
			char tmp[1024];
			
			memset(tmp, 0 , 1024);
			sprintf(tmp, "GET %s HTTP/1.1\r\n", headdf.http_fpath);
			buf += tmp;
			sprintf(tmp,"Host:%s\r\n",headdf.http_url);
			buf += tmp;
			buf += "Connection:close\r\n";
			buf += "User-agent:Mozilla/4.0\r\n";
			buf += "Accept-language:zh-cn\r\n\r\n";
			printf("Format Http Header :\n");
			std::cout<<buf<<std::endl;
			break;
		
		default:
			printf("Format Http Header failed!\n");		
			break;
	}
	return buf;
}

int CHttpParser::onMethodGet()
{
  return 0;
}

int CHttpParser::onMethodPost()
{
  return 0;
}
