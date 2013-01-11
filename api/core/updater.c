/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               main
 * Comments:             本文件包含了对Updater结构体对象及其相关接口的实现
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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../inet/inetdef.h"
#include "../inet/protocol.h"

#include "servernode.h"
#include "updater.h"


/**
 * Updater_Create 创建一个Updater对象
 * @param updateServer 更新服务器IP地址
 * @param port 更新服务器端口
 * @return 成功则返回创建的Updater对象指针，否则返回NULL
 */
Updater * Updater_Create(char * updateServer, int port)
{
	int i;
	Updater * updater = (Updater *)malloc(sizeof(struct sUpdater));
	for (i = 0; i < MAX_SERVER_NODE; i++)
	{
		updater->ServerList[i] = NULL;
	}
	updater->DefaultNode = NULL;
	updater->NodeCount = 0;
	updater->Version = 0;
	strcpy(updater->UpdateServer, updateServer);
	updater->UpdateServerPort = port;
	updater->Client = TCPClient_Create(
						updater->UpdateServer,
						updater->UpdateServerPort);
	
	return updater;
}

/**
 * Updater_Dispose 销毁一个Updater对象
 * @param updater Updater对象指针
 * @return 成功则返回true，否则返回false
 */
bool Updater_Dispose(Updater * updater)
{
	int i;
	if (updater != NULL)
	{
		for (i = 0; i < updater->NodeCount; i++)
		{
			if (updater->ServerList[i] != NULL)
			{
				ServerNode_Dispose(updater->ServerList[i]);
			}
		}
		free(updater);
		return true;
	}
	return false;
	
}

/**
 * Updater_Check 查询更新服务器，确定本地服务器列表是否需要更新
 * @param updater Updater对象指针
 * @return 需要更新则返回true，否则返回false
 */
bool Updater_Check(Updater * updater)
{
	char 		data[4];
	char *		toSend;
	char		buf[128];
	NCProtocol*	ncp;
	NCData ** 	ncData;
	bool		result = true;

	ncData = (NCData **)malloc( sizeof( struct sNCData *) * 1);
	memcpy(data, &(updater->Version), sizeof(data));
	ncData[0] = NCData_Create(sizeof(int), data);
	ncp = NCProtocol_Create(
						CMD_CLIENT_REQ_NODE_CHANGE,
						1,
						ncData);
	toSend = NCProtocol_Encapsul(ncp); 
	if (updater->Client == NULL)
	{
		updater->Client = TCPClient_Create(
						updater->UpdateServer,
						updater->UpdateServerPort);
	}
	TCPClient_Connect(updater->Client);
	TCPClient_Send(updater->Client, toSend, sizeof(toSend));
	TCPClient_Receive(updater->Client, buf, sizeof(buf));
	NCProtocol_Dispose(ncp);
	free(toSend);
	ncp = NCProtocol_Parse(buf);
	if (ncp != NULL)
	{
		if (ncp->command == CMD_SERVER_REP_NODE_KEEP)
		{
			result = false; 
		}
		else
		{
			result = true;
		}
	}
	NCProtocol_Dispose(ncp);
	return result;
}

/**
 * Updater_UpdateServer 查询更新服务器更新本地服务器列表
 * @param updater Updater对象指针
 * @return 成功则返回true，否则返回false
 */
bool Updater_UpdateServer(Updater * updater)
{
	char 		data[4];
	char *		toSend;
	char		buf[1024] = { 0 };
	NCProtocol*	ncp;
	NCData ** 	ncData;
	bool		result = true;
	int 		i;
	int			count = 0;
	ServerNode * node;
	ncData = (NCData **)malloc( sizeof( struct sNCData *) * 1);
	memcpy(data, &(updater->Version), sizeof(int));
	ncData[0] = NCData_Create(sizeof(int), data);
	ncp = NCProtocol_Create(CMD_CLIENT_REQ_NODE_LIST, 1, ncData);
	if (updater->Client == NULL)
	{
		updater->Client = TCPClient_Create(updater->UpdateServer, updater->UpdateServerPort);
	}
	if ( !(TCPClient_Connect(updater->Client)) )
	{
		fprintf(stderr, "\nTCP连接出错，错误代码：%d\n", 1);
		NCProtocol_Dispose(ncp);
		return false;
	}
	toSend = NCProtocol_Encapsul(ncp); 	
	TCPClient_Send(updater->Client, toSend, ncp->totalLength);
	
	NCProtocol_Dispose(ncp);
	free(toSend);

	TCPClient_Receive(updater->Client, buf, sizeof(buf));
	
	ncp = NCProtocol_Parse(buf);
	if (ncp != NULL)
	{
		/* 如果服务器返回的为数据节点信息的列表，则更新 */
		if (ncp->command == CMD_SERVER_REP_NODE_LIST)
		{
			memcpy(&(updater->Version), ncp->dataChunk[0]->data, sizeof(int));
			printf("收到数据：");
			printf("\nVersion:%d\n", updater->Version);
			updater->NodeCount = 0;
			for (i = 1; i < ncp->chunkCount; i++)
			{
				if (updater->ServerList[i - 1] != NULL)
				{
					free(updater->ServerList[i - 1]);
				}
				updater->ServerList[i - 1] = (ServerNode *)malloc(sizeof(struct sServerNode));
				memcpy(updater->ServerList[i - 1], ncp->dataChunk[i]->data, ncp->dataChunk[i]->length);
				printf("服务器%2d: %s:%d, %d --> %d \n",
						updater->ServerList[i - 1]->ID,
						updater->ServerList[i - 1]->IPAddress,
						updater->ServerList[i - 1]->Port,
						updater->ServerList[i - 1]->StartKey,
						updater->ServerList[i - 1]->EndKey);
				//result &= Updater_UpdateNode(updater, node);
				updater->NodeCount++;
			}
			updater->DefaultNode = updater->ServerList[0];
			//result &= Updater_Clean(updater);
		}
		NCProtocol_Dispose(ncp);
	}
	else
	{
		result = false;
	}
	
	return result;
}

/**
 * 清理服务器列表
 * @param updater Updater对象指针
 * @return 成功则返回true，否则返回false
 */
bool Updater_Clean(Updater * updater)
{
	return true;
}

/**
 * Updater_FindNode 查找特定ID的服务器节点信息是否存在服务器列表中
 * @param updater Updater对象指针
 * @param key 要查找的节点ID
 * @param index 输出参数，若找到则为找到元素的下标，否则为插入位置
 * @return 查找到则返回true,否则返回false
 */
bool Updater_FindNode(Updater * updater, int key, int * index)
{
	int i = 0;
	for (i = 0; i < updater->NodeCount; i++)
	{
		if (updater->ServerList[i]->StartKey == key)
		{
			*index = i;
			return true;
		}
	}
	*index = i;
	return false;
}

/**
 * Updater_UpdateNode 更新服务器列表相应节点信息
 * @param updater Updater对象指针
 * @param node 要更新的节点ServerNode对象指针
 * @return 成功则返回true，否则返回false
 */
bool Updater_UpdateNode(Updater * updater, ServerNode * node)
{
	int index;
	int i;
	ServerNode * tmp = NULL;
	if (Updater_FindNode(updater, node->StartKey, &index))
	{
		tmp = updater->ServerList[index];
		/* 如果服务器的版本高于本地服务器节点信息的版本，则更新 */
		if (tmp != NULL && tmp->UpdateTag != node->UpdateTag)
		{
			memcpy(tmp, node, sizeof(struct sServerNode));
			/*
			strcpy(tmp->Name, node->Name);
			strcpy(tmp->IPAddress, node->IPAddress);
			tmp->Port = node->Port;
			tmp->DataCount = node->DataCount;
			tmp->UpdateTag = node->UpdateTag;
			tmp->StartKey = node->StartKey;
			tmp->EndKey = node->EndKey;
			*/
		}
		free(node);
		return true;
	}
	/* 如果未找到，则该节点为新增节点，将其插入服务器列表 */
	else
	{
		if (updater->NodeCount < MAX_SERVER_NODE)
		{
			updater->ServerList[index] = ServerNode_Create(
								node->ID,
								node->Name,
								node->IPAddress,
								node->Port,
								node->DataCount,
								node->UpdateTag,
								node->StartKey,
								node->EndKey);
			updater->NodeCount++;
			printf("添加服务器:%s:%d, %d --> %d \n", updater->ServerList[index]->IPAddress, updater->ServerList[index]->Port, updater->ServerList[index]->StartKey, updater->ServerList[index]->EndKey);
			free(node);
			return true;
		}
	}
	return false;
}

/**
 * Updater_InsertNode 将制定服务器节点插入服务器列表
 * @param updater Updater对象指针
 * @param node 要插入的节点ServerNode对象指针
 * @return 成功则返回true，否则返回false
 */
bool Updater_InsertNode(Updater * updater, ServerNode * node)
{
	return true;
}

/**
 * 从服务器列表中删除特定ID的服务器节点信息
 * @param updater Updater对象指针
 * @param id 要删除的节点ID
 * @return 成功则返回true，否则返回false
 */
bool Updater_DeleteNode(Updater * updater, int id)
{
	int i;
	int index = -1;
	ServerNode * tmp;
	if (Updater_FindNode(updater, id, &index))
	{
		tmp = updater->ServerList[index];
		for (i = index; i < updater->NodeCount - 1; i++)
		{
			updater->ServerList[i] = updater->ServerList[i + 1];
		}
		ServerNode_Dispose(tmp);
		return true;
	}
	return false;
}


/**
 * Updater_GetServer 获取特定数据服务器节点对象
 * @param updater Updater对象指针
 * @param node 数据键值
 * @return 成功则返回获取到的服务器节点对象指针，否则返回NULL
 */
ServerNode * Updater_GetServer(Updater * updater, int node)
{
	int i;
	for (i = 0; i < updater->NodeCount; i++)
	{
		if (node >= updater->ServerList[i]->StartKey &&
			node < updater->ServerList[i]->EndKey)
		{
			return updater->ServerList[i];
		}
	}
	return NULL;
}

/**
 * Updater_GetServer 获取特定数据服务器节点对象
 * @param updater Updater对象指针
 * @param node 数据键值
 * @return 成功则返回获取到的服务器节点对象指针，否则返回NULL
 */
ServerNode * Updater_GetNextServer(Updater * updater, int node)
{

	int i;
	for (i = 0; i < updater->NodeCount; i++)
	{
		if (node >= updater->ServerList[i]->StartKey &&
			node < updater->ServerList[i]->EndKey)
		{
			if (updater->ServerList[(i + 1) % updater->NodeCount] != NULL)
			{
				return updater->ServerList[(i + 1) % updater->NodeCount];
			}
		}
	}
	return NULL;
}
