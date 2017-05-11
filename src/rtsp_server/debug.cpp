#include <stdio.h>

FILE* dbg_fp = NULL;

void dbg_open()
{
    dbg_fp = fopen("debug.txt","w");
}

void dbg_close()
{
    fclose(dbg_fp);
}

