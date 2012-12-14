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

#ifndef PROTOCOL_H_INCLUDE
#define PROTOCOL_H_INCLUDE

#include "inetdef.h"

/**
 * NCData 结构体的定义
 * @author capasky
 * @version 1.0.0.1
 */
typedef struct sNCData
{
    int     legth;
    char *  data;
} NCData;

/**
 * NCProtocol 结构体的定义
 * @author capasky
 * @version 1.0.0.1
 */
typedef struct sNoCommit
{
    char            command[2];
    unsigned short  totalLength;
    int             chunkCount;
    NCData *        dataChunk;
} NCProtocol;

/**
 * NCProtocol_Parse 将数据解析为协议结构对象
 * @param data 数据指针
 * @param length 数据的长度
 * @return 成功则返回解析的 NCProtocol 结构体对象指针，否则返回 NULL
 */
NCProtocol * NCProtocol_Parse(char * data, int length);

/**
 * NCProtocol_Encapsul 将协议结构体对象封装为数据
 * @param ncp 协议结构体对象指针
 * @return 成功则返回封装后的数据指针，否则返回 NULL
 */
char * NCProtocol_Encapsul(NCProtocol * ncp);

#endif
