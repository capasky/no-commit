/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               main
 * Comments:             This file include the server updater defination
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

#ifndef NCUPDATER_H_INCLUDE
#define NCCLIENT_H_INCLUDE

#include <stdbool.h>
#include "api/inet/inetdef.h"

#define MAX_SERVER_NAME_LEN		64
#define	MAX_SERVER_UPDATE_TAG	65536


typedef struct sServerNode
{
	int 	ID;
	char 	Name[MAX_SERVER_NAME_LEN];
	char 	IPAddress[INET_IPADDR_STRING_LEN];
	int		Port;
	int		DataCount;
	int		UpdateTag;
	int 	StartKey;
	int		EndKey;
} ServerNode;

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
					int dataCount, int updateTag, int start, int end);

/**
 * ServerNode_Dispose处理ServerNode，释放其所占内存
 * @parma sn ServerNode对象指针
 * @return 成功则返回true， 否则返回false
 */
bool ServerNode_Dispose(ServerNode * sn);

#endif
