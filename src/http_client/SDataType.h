//SDataType
#ifndef _SDATATYPE_H_
#define _SDATATYPE_H_


enum 
{
   M_GET       = 0,
   M_PUT       = 1,
   M_POST      = 2,
   M_DELETE    = 3,
   M_CONNECT   = 4,
   M_OPTIONS   = 5,
   M_TRACE     = 6,
   M_INVALID   = 7,
   M_HTTP      = 8
};

struct HTTP_TYPE
{
	int 	http_method;
	char http_rstate[1024];
	char http_url[1024];
	char http_fpath[1024];
	char http_rtype[1024];
	int    len;
};
#endif

