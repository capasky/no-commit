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
 * capasky				2012.12.15			1.0.1.0
 */

#ifndef PROTOCOL_H_INCLUDE
#define PROTOCOL_H_INCLUDE

#define DATA_MAX_LEN 1024

#include <stdbool.h>

/**
 * NCData 结构体的定义
 * @author capasky
 * @version 1.0.0.1
 */
typedef struct sNCData
{
    int     length;
    char *  data;
} NCData;

/**
 * NCProtocol 结构体的定义
 * @author capasky
 * @version 1.0.0.1
 */
typedef struct sNCProtocol
{
    int             command;
    int             totalLength;
    int             chunkCount;
    NCData **       dataChunk;
} NCProtocol;

/**
 * NCData_Create 创建一个 NCData 结构体对象
 * @param length 数据长度
 * @param data 数据
 * @return 成功则返回封装后的数据指针，否则返回NULL
 */
NCData * NCData_Create(int length, char * data);

/**
 * NCData_Dispose 处理NCData，释放其所占内存
 * @param ncd 数据块结构体对象指针
 * @return 成功返回true，否则返回false。
 */
bool NCData_Dispose(NCData * ncd);
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
                                NCData **   dataChunk);

/**
 * NCProtocol_Dispose 处理NCProtocol，释放其所占内存
 * @param ncp 协议结构体对象指针
 * @return 成功返回true，否则返回false。
 */
bool NCProtocol_Dispose(NCProtocol * ncp);

/**
 * NCProtocol_AddData 向NCProtocol协议对象中添加数据块
 * @param ncp 协议结构体对象指针
 * @param data 数据指针
 * @param length 数据的长度
 * @return 成功返回true，否则返回false。
 */
bool NCProtocol_AddData(NCProtocol * ncp, char * data, int length);

/**
 * NCProtocol_Parse 将数据解析为协议结构对象
 * @param data 数据指针
 * @return 成功则返回解析的 NCProtocol 结构体对象指针，否则返回NULL
 */
NCProtocol * NCProtocol_Parse(char * data);

/**
 * NCProtocol_Encapsul 将协议结构体对象封装为数据
 * @param ncp 协议结构体对象指针
 * @return 成功则返回封装后的数据指针，否则返回NULL
 */
char * NCProtocol_Encapsul(NCProtocol * ncp);

#endif
