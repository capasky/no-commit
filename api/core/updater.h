/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               main
 * Comments:             本文件包含了对Updater结构体对象及其相关接口的定义
 * Author:               capasky
 * Create Date:          2013.01.04
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/


/**
 * Revision Log:
 * @author				@date				@version
 * capasky				2013.01.04			1.0.0.0
 */

#ifndef UPDATER_H_INCLUDE
#define UPDATER_H_INCLUDE

#include <stdbool.h>

#include "servernode.h"
#include "../inet/inetdef.h"
#include "../inet/TCPClient.h"
#include "../../command.h"

#define MAX_SERVER_NODE		128

typedef struct sUpdater
{
	int				State;
	ServerNode *	ServerList[MAX_SERVER_NODE];
	ServerNode *	DefaultNode;
	int				NodeCount;
	int				Version;
	char			UpdateServer[INET_IPADDR_STRING_LEN];
	int				UpdateServerPort;
	TCPClient *		Client;
} Updater;

/**
 * Updater_Create 创建一个Updater对象
 * @param updateServer 更新服务器IP地址
 * @param port 更新服务器端口
 * @return 成功则返回创建的Updater对象指针，否则返回NULL
 */
Updater * Updater_Create(char * updateServer, int port);

/**
 * Updater_Dispose 销毁一个Updater对象
 * @param updater Updater对象指针
 * @return 成功则返回true，否则返回false
 */
bool Updater_Dispose(Updater * updater);

/**
 * Updater_Check 查询更新服务器，确定本地服务器列表是否需要更新
 * @param updater Updater对象指针
 * @return 需要更新则返回true，否则返回false
 */
bool Updater_Check(Updater * updater);

/**
 * Updater_UpdateServer 查询更新服务器更新本地服务器列表
 * @param updater Updater对象指针
 * @return 成功则返回true，否则返回false
 */
bool Updater_UpdateServer(Updater * updater);

/**
 * 清理服务器列表
 * @param updater Updater对象指针
 * @return 成功则返回true，否则返回false
 */
bool Updater_Clean(Updater * updater);

/**
 * Updater_FindNode 查找特定ID的服务器节点信息是否存在服务器列表中
 * @param updater Updater对象指针
 * @param id 要查找的节点ID
 * @param index 输出参数，若找到则为找到元素的下标，否则为插入位置
 * @return 查找到则返回true,否则返回false
 */
bool Updater_FindNode(Updater * updater, int id, int * index);

/**
 * Updater_UpdateNode 更新服务器列表相应节点信息，若该节点不存在则执行插入操作
 * @param updater Updater对象指针
 * @param node 要更新的节点ServerNode对象指针
 * @return 成功则返回true，否则返回false
 */
bool Updater_UpdateNode(Updater * updater, ServerNode * node);

/**
 * Updater_InsertNode 将制定服务器节点插入服务器列表
 * @param updater Updater对象指针
 * @param node 要插入的节点ServerNode对象指针
 * @return 成功则返回true，否则返回false
 */
bool Updater_InsertNode(Updater * updater, ServerNode * node);

/**
 * 从服务器列表中删除特定ID的服务器节点信息
 * @param updater Updater对象指针
 * @param id 要删除的节点ID
 * @return 成功则返回true，否则返回false
 */
bool Updater_DeleteNode(Updater * updater, int id);

/**
 * Updater_GetServer 获取特定数据服务器节点对象
 * @param updater Updater对象指针
 * @param node 数据键值
 * @return 成功则返回获取到的服务器节点对象指针，否则返回NULL
 */
ServerNode * Updater_GetServer(Updater * updater, int node);

#endif
