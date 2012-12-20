/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.inet.inetdef
 * Comments:             This file include the definition of protocol
 *                       struct and the interfaces using in protocol
 * Author:               capasky
 * Create Date:          2012.12.13
 * Version:              1.0
 * Plateform:            Any
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * capasky              2012.12.13          1.0.0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../base.h"
#include "inetdef.h"
#include "protocol.h"

/**
 * NCData_Create 创建一个 NCData 结构体对象
 * @param length 数据长度
 * @param data 数据
 * @return 成功则返回封装后的数据指针，否则返回NULL
 */
NCData * NCData_Create(int length, char * data)
{
    NCData * ncd = (NCData *)malloc(sizeof(struct sNCData));
    if (ncd == NULL)
    {
        fputs("分配内存出错", stderr);
        return NULL;
    }

    ncd->length = length;
    ncd->data = strdup(data);

    return ncd;
}

/**
 * NCData_Dispose 处理NCData，释放其所占内存
 * @param ncd 数据块结构体对象指针
 * @return 成功返回true，否则返回false。
 */
bool NCData_Dispose(NCData * ncd)
{
	if (ncd == NULL)
	{
		return false;
	}
	if (ncd->data != NULL)
	{
		printf("%s", ncd->data);
		free(ncd->data);
	}
	free(ncd);
	
	return true;
}

/**
 * NCProtocol_Create 创建一个 NCProtocol 结构体对象
 * @param command 命令标识
 * @param totalLength 协议数据总长度
 * @param chunkCount 数据块数
 * @param dataChunk 数据块
 * @return 成功则返回封装后的数据指针，否则返回NULL
 */
NCProtocol * NCProtocol_Create( int         command,
                                int         chunkCount,
                                NCData **    dataChunk
                                )
{
    int i;
    int len = 0;
    NCProtocol * ncp = (NCProtocol *)
                            malloc (sizeof(struct sNCProtocol));
    if (ncp == NULL)
    {
        fputs("分配内存出错", stderr);
        return NULL;
    }
    len = sizeof(int) * 3;
    for (i = 0; i < chunkCount; i++)
    {
        len += (sizeof(int) + dataChunk[i]->length);
    }
    ncp->command = command;
    ncp->totalLength = len;
    ncp->chunkCount = chunkCount;
    ncp->dataChunk = dataChunk;

    return ncp;
}

/**
 * NCProtocol_Dispose 处理NCProtocol，释放其所占内存
 * @param ncp 协议结构体对象指针
 * @return 成功返回true，否则返回false。
 */
bool NCProtocol_Dispose(NCProtocol * ncp)
{
	int i;
	if (ncp == NULL)
	{
		return false;
	}
	else
	{
		for (i = 0; i < ncp->chunkCount; i++)
		{
			if (ncp->dataChunk[i] != NULL)
			{
				if(!NCData_Dispose(ncp->dataChunk[i]))
				{
					return false;
				}
			}
		}
		free(ncp);
	}
	return true;
}

/**
 * NCProtocol_Parse 将数据解析为协议结构对象
 * @param data 数据指针
 * @param length 数据的长度
 * @return 成功则返回解析的 NCProtocol 结构体对象指针，否则返回 NULL
 */
NCProtocol * NCProtocol_Parse(char * data)
{
    int         i           = 0;
    int         len         = 0;
    char *      dat         = NULL;
    int         command     = 0;
    //int         totalLength = 0;
    int         chunkCount  = 0;
    NCData **    dataChunk;

    command = (int)(*data);
    data += sizeof(int);

    //totalLength = (int)(*data);
    data += sizeof(int);

    chunkCount = (int)(*data);
    data += sizeof(int);

    dataChunk = (NCData **)malloc(sizeof(struct sNCData *) * chunkCount);

    for(i = 0; i < chunkCount; i++)
    {
        len = (int)(*data);
        data += sizeof(int);
        dat = strndup(data, len);
        dataChunk[i] = NCData_Create(len, dat);
        if (dataChunk[i] == NULL)
        {
            free(dat);
            free(dataChunk);
            return NULL;
        }
        data += len;
    }
    return NCProtocol_Create( command, chunkCount, dataChunk );
}

/**
 * NCProtocol_Encapsul 将协议结构体对象封装为数据
 * @param ncp 协议结构体对象指针
 * @return 成功则返回封装后的数据指针，否则返回 NULL
 */
char * NCProtocol_Encapsul(NCProtocol * ncp)
{
    int i;
    char * data = NULL;
    char * p;
    data = (char *)malloc(sizeof(char) * ncp->totalLength);
    if (data == NULL)
    {
        fputs("分配内存出错", stderr);
        return NULL;
    }
    p = data;
    memcpy(p, &ncp->command, sizeof(int) * 3);
    p += sizeof(int) * 3;
    for (i = 0; i < ncp->chunkCount; i++)
    {
        memcpy(p, ncp->dataChunk[i], sizeof(int));
        p += sizeof(int);
        memcpy(p, ncp->dataChunk[i]->data, ncp->dataChunk[i]->length);
        p += ncp->dataChunk[i]->length;
    }

    return data;
}


