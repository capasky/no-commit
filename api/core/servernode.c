/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               main
 * Comments:             This file include the server updater implementation
 *                       functions
 * Author:               capasky
 * Create Date:          2012.12.22
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/


/**
 * Revision Log:
 * @author              @date               @version
 * capasky              2012.12.29          1.0.0.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../inet/inetdef.h"

#include "../utils/convert.h"

#include "servernode.h"

/**
 * ServerNode_Create 创建一个服务器节点对象
 * @param id 服务器ID
 * @param name 服务器名称
 * @param ip 服务器IP
 * @param port 服务器端口
 * @param dataCount 该服务器节点存储的数据个数
 * @param updateTag 该服务器节点信息更新戳
 * @param start 该服务器节点存储数据的起始键值
 * @param end 该服务器节点存储数据的终止键值
 * @return 成功则返回ServerNode对象指针，否则返回NULL
 */
ServerNode * ServerNode_Create(int id, char * name, char * ip, int port,
					int dataCount, int updateTag, int start, int end)
{
	int i;
	ServerNode * sn = NULL;
	
	sn = (ServerNode *)malloc(sizeof(struct sServerNode));
	if (strlen(name) > MAX_SERVER_NAME_LEN - 1)
	{
		name[MAX_SERVER_NAME_LEN - 1] = '\0';
	}
	for (i = 0; i < MAX_SERVER_NAME_LEN; i++)
	{
		sn->Name[i] = 0;
	}
	ip[INET_IPADDR_STRING_LEN - 1] = 0;
	for (i = 0; i < INET_IPADDR_STRING_LEN; i++)
	{
		sn->IPAddress[i] = 0;
	}
	sn->ID = id;
	sn->Port = port;
	strcpy(sn->Name, name);
	strcpy(sn->IPAddress, ip);
	sn->DataCount = dataCount;
	sn->UpdateTag = updateTag;
	sn->StartKey = start;
	sn->EndKey = end;
	
	return sn;
}


/**
 * ServerNode_Dispose处理ServerNode，释放其所占内存
 * @parma sn ServerNode对象指针
 * @return 成功则返回true， 否则返回false
 */
bool ServerNode_Dispose(ServerNode * sn)
{
	if (sn != NULL)
	{
		free(sn);
		sn = NULL;
		return true;
	}
	
	return false;
}

/**
 * ServerNode_Parse 将字节串解析为ServerNode对象
 * @param data 字节串指针
 * @return 成功则返回ServerNode对象指针，否则返回NULL
 */
ServerNode * ServerNode_Parse(char * data)
{
	ServerNode * sn = (ServerNode *)malloc(sizeof(struct sServerNode));
	memcpy(sn, data, sizeof(struct sServerNode));
	
	return sn;
}

/**
 * ServerNode_ToByte 将ServerNode对象转换为字节序列
 * @param node ServerNode对象指针
 * @return 返回转换后字节序列的指针
 */
char * ServerNode_ToByte(ServerNode * node)
{
	char * data = (char *)malloc(sizeof(char) * sizeof(struct sServerNode));
	memcpy(data, node, sizeof(struct sServerNode));
	
	return data;
}
