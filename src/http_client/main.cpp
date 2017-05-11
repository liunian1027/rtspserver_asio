//main
#include <stdio.h>
#include "CClient.h"
int main(int argc, char* argv[])
{	
	if ( argc!=4 )
	{
		printf("usage: httpClient <IP> <Port> <Path>\n");
		return 0;
	}
	
	CClient C_Client;
	C_Client.Start(argv[1],argv[2],argv[3]);
	C_Client.Stop();
	return 0;
}

