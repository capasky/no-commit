#include <stdio.h>
#include <stdlib.h>

#include "protocol.h"

int main()
{
    int i;
    char * p = NULL;
    NCData * ncdata[10];
    for (i = 0; i < 10; i++)
    {
        ncdata[i] = NCData_Create(4, "123");
    }
    NCProtocol * ncp = NCProtocol_Create( 1, 10, ncdata);
    p = NCProtocol_Encapsul(ncp);
    printf("a:%d\nb:%d\ns:%s\n", *p, *(p+4), (char *)(p+16));
    ncp = NCProtocol_Parse(p);
    printf("C:%d\nB:%d\nS:%s\n", ncp->chunkCount, ncp->totalLength, ncp->dataChunk[0]->data);
    getchar();
    return 0;
}

