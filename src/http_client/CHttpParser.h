//CHttpParser.h
#ifndef _CHTTPPARSER_H_
#define _CHTTPPARSER_H_

#include "CParser.h"
#include "SDataType.h"


class CHttpParser: public CParser
{
public:

	CHttpParser();
	~CHttpParser();
	int parser(char* headbuf, int len);
	std:: string  formatHeader( struct HTTP_TYPE headdf);
	
	int onMethodGet();
	int onMethodPost();
};
#endif

