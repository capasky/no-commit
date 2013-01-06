/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               main
 * Comments:             This file include the entry point of client
 *                       function
 * Author:               capasky
 * Create Date:          2012.12.22
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/


/**
 * Revision Log:
 * @author              @date               @version
 * capasky              2012.12.22          1.0.0.0
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "command.h"

#include "api/core/servernode.h"
#include "api/core/updater.h"

#include "api/utils/stringutils.h"

#include "api/inet/inetdef.h"
#include "api/inet/TCPClient.h"
#include "api/inet/protocol.h"

#include "ncclient.h"

/**
 * NCClient_Create 创建NCClient结构体对象
 * @return 成功则返回创建的NCClient对象的指针，否则返回NULL
 */
NCClient * NCClient_Create()
{
	int i;
	NCClient * ncc = (NCClient *) malloc( sizeof(struct sNCClient) );
	if (ncc != NULL)
	{
		ncc->Active 		= false;
		memset(ncc->DBFile, 0, sizeof(ncc->DBFile));
		ncc->Connected 		= false;
		ncc->Client 		= TCPClient_Create( IP_ADDR_SERVER, IP_PORT_SERVER );
		ncc->PrevNode		= NULL;
		ncc->ServerUpdater	= NULL;
		ncc->CurrentCommand = NULL;
		ncc->CurrentNCP		= NULL;
		ncc->ReSelect		= false;
		ncc->QuitFlag		= false;
		pthread_mutex_init(&(ncc->UpdaterMutex), NULL);
	}
	return ncc;
}

/**
 * NCClient_UpdateServer 更新服务器列表，用与分布式系统
 * @param ncc NCClient对象指针
 * @return 成功则返回true，否则返回false
 */
void NCClient_UpdateServer(NCClient * ncc)
{
	int i;
	i = pthread_create(&(ncc->UpdaterThread), NULL, &NCClient_Updater, (void *)ncc);
	if (i != 0)
	{
		fprintf ( stderr, "更新线程创建失败,%s:%d\n", __FILE__, __LINE__ );
	}
	return;
}

/**
 * 根据当前操作对象选择分布式数据库服务器节点
 * @param retry 尝试重新选择服务器
 */
void NCClient_SelectServer(NCClient * ncc)
{
	int node = 0;
	int i;
	if (ncc->CurrentCommand != NULL)
	{
		node = atoi(ncc->CurrentCommand->Key);
		if (ncc->ReSelect)
		{
			ncc->CurrentServer = Updater_GetNextServer( ncc->ServerUpdater, node );
		}
		else
		{
			//ncc->CurrentServer = Updater_GetServer( ncc->ServerUpdater, node );
			for (i = 0; i < ncc->ServerUpdater->NodeCount; i++)
			{
				printf("Key:%d\n", node);
				printf("Search from %d to %d\n", ncc->ServerUpdater->ServerList[i]->StartKey, ncc->ServerUpdater->ServerList[i]->EndKey);
				if (node >= ncc->ServerUpdater->ServerList[i]->StartKey &&
					node < ncc->ServerUpdater->ServerList[i]->EndKey)
				{
					ncc->CurrentServer = ncc->ServerUpdater->ServerList[i];
					break;
				}
			}
		}
	}
	if (ncc->CurrentServer == NULL)
	{
		printf("NULL FIND!\n");
		ncc->CurrentServer = ncc->ServerUpdater->DefaultNode;
	}
	return;
}

/**
 * NCClient_CheckCommand 检查命令
 * @param cmd 命令对象指针
 * @return 成功则返回true，否则返回false
 */
void NCClient_CheckCommand(NCClient * ncc)
{
	if (ncc->CurrentCommand == NULL)
	{
		printf("命令格式错误！\n");
		ncc->CommandValid = false;
		return;
	}
	if (ncc->CurrentCommand->CommandID == CMD_QUIT_ID)
	{
		exit(0);
	}
	if (ncc->CurrentCommand->CommandID == CMD_OPEN_ID && ncc->Active)
	{
		printf("数据库连接已打开!\n");
		ncc->CommandValid = false;
		return;
	}
	if (ncc->CurrentCommand->CommandID != CMD_OPEN_ID && !(ncc->Active))
	{
		printf("请先打开数据库连接!用法：open filename.ext\n");
		ncc->CommandValid = false;
		return;
	}
	if (ncc->ServerUpdater->NodeCount < 1)
	{
		printf("没有数据服务器， 无法操作！\n");
		ncc->CommandValid = false;
		return;
	}
	
	ncc->CommandValid = true;
	
	return;
}

void NCClient_Run(NCClient * ncc)
{
	char cmdString[128];
	NCClient_UpdateServer(ncc);
	printf("NoCommit>>");
	while( gets(cmdString) != NULL)
	{
		ncc->CurrentCommand = Command_TryParse(cmdString);
		NCClient_CheckCommand(ncc);
		if (ncc->CommandValid)
		{
			NCClient_PrepareData(ncc);
			switch (ncc->CurrentCommand->CommandID)
			{
				case CMD_OPEN_ID:
					OpenCommandHandler(ncc);
					break;
				case CMD_CLOSE_ID:
					CloseCommandHandler(ncc);
					break;
				case CMD_GET_ID:
					GetCommandHandler(ncc);
					ncc->ReSelect = false;
					break;
				case CMD_SET_ID:
					SetCommandHandler(ncc);
					break;
				case CMD_DEL_ID:
					DeleteCommandHandler(ncc);
					break;
				case CMD_QUIT_ID:
					QuitCommandHandler(ncc);
					break;
				default:
					break;
			}
		}
		if (ncc->Active)
		{
			printf("%s::%s>>",
				ncc->CurrentServer->Name, ncc->DBFile);
		}
		else
		{
			printf("NoCommit>>");
		}
		NCClient_Clean(ncc);
		if (ncc->QuitFlag)
		{
			break;
		}
		
	}
	
	return;
}

void NCClient_PrepareData(NCClient * ncc)
{
	NCData ** 	ncData;
	int 		count = 0;
	if(ncc->CurrentNCP != NULL)
	{
		return;
	}
	if (ncc->CurrentCommand == NULL || ncc->CurrentCommand->CommandID == CMD_QUIT_ID)
	{
		return;
	}

	ncData = (NCData **)malloc( sizeof( struct sNCData *) * 2);
	
	if (ncc->CurrentCommand->Key != NULL)
	{
		ncData[0] = NCData_Create(strlen(ncc->CurrentCommand->Key), ncc->CurrentCommand->Key);
		count++;
		if (ncc->CurrentCommand->Value != NULL)
		{
			ncData[1] = NCData_Create(strlen(ncc->CurrentCommand->Value), ncc->CurrentCommand->Value);
			count++;
		}
		else
		{
			ncData[1] = NULL;
		}
	}
	else
	{
		ncData[0] = NULL;
	}
	ncc->CurrentNCP = NCProtocol_Create( ncc->CurrentCommand->CommandID, count, ncData );
	
	return;
}

/**
 * NCClient_ExecRemote 执行远程命令
 * @param ncc NCClient对象指针
 */
NCProtocol * NCClient_ExecRemote(NCClient * ncc)
{
	int mlen = 0;
	char buffer[1024];
	char * data;
	NCProtocol * ncp;
	if (ncc->Client == NULL)
	{
		ncc->Client = TCPClient_Create(IP_ADDR_SERVER, IP_PORT_SERVER);
	}
	
	strcpy(ncc->Client->RemoteAddress, ncc->CurrentServer->IPAddress);
	ncc->Client->RemotePort = ncc->CurrentServer->Port;
	printf("当前连接服务器：\n服务器名：%s\n服务器IP：%s\n服务器端口：%d\n",
				ncc->CurrentServer->Name,
				ncc->CurrentServer->IPAddress,
				ncc->CurrentServer->Port);
	ncc->Connected = TCPClient_Connect(ncc->Client);
	if ( !(ncc->Connected) )
	{
		fprintf(stderr, "TCP连接出错，错误代码：%d\n", 1);
		return NULL;
	}
	//封装数据
	data = NCProtocol_Encapsul(ncc->CurrentNCP);
	//发送
	TCPClient_Send( ncc->Client, data, ncc->CurrentNCP->totalLength );
	free(data);
	//接受服务器回复
	mlen = TCPClient_Receive( ncc->Client, buffer, sizeof(buffer) );
	printf("接收数据长度:%d\n", mlen);
	buffer[mlen] = 0;

	TCPClient_Close(ncc->Client);
	
	ncp = NCProtocol_Parse(&buffer[0]);
	return ncp;
}

/**
 * NCClient_Clean 客户端清理
 * @param ncc NCClient对象指针
 */
void NCClient_Clean(NCClient * ncc)
{
	fflush(stdin);
	NCProtocol_Dispose(ncc->CurrentNCP);
	ncc->CurrentNCP = NULL;
	if (ncc->PrevNode)
	{
		ServerNode_Dispose(ncc->PrevNode);
	}
	
	return;
}

/**
 * NCClient_Updater 更新线程处理函数
 * @return 执行结束返回
 */
void * NCClient_Updater(void * ncc)
{
	int i, j, k;
	NCClient * nc = (NCClient *)(ncc);
	Updater * updater = Updater_Create(
				IP_ADDR_SERVER,
				IP_PORT_SERVER);
	nc->ServerUpdater = updater;
	while (1)
	{
		pthread_mutex_lock(&(nc->UpdaterMutex));
		Updater_UpdateServer(updater);
		pthread_mutex_unlock(&(nc->UpdaterMutex));
		sleep(10);
		
	}
	
	return NULL;
}


/**
 * NCClient_ConsistProcess 一致性维护线程处理函数
 * @param ncc
 * @return 执行结束返回
 */
void * NCClient_ConsistProcess(void * ncc)
{
	NCClient * nc = (NCClient *)(ncc);
	NCProtocol * ncp = NULL;
	int mlen = 0;
	char buffer[1024];
	char * data;
	NCData ** 	ncData;
	int 		count = 0;

	if (nc->CurrentCommand == NULL || nc->CurrentCommand->CommandID == CMD_QUIT_ID)
	{
		return NULL;
	}

	ncData = (NCData **)malloc( sizeof( struct sNCData *) * 2);
	
	if (nc->CurrentCommand->Key != NULL)
	{
		ncData[0] = NCData_Create(strlen(nc->CurrentCommand->Key), nc->CurrentCommand->Key);
		count++;
		if (nc->CurrentCommand->Value != NULL)
		{
			ncData[1] = NCData_Create(strlen(nc->CurrentCommand->Value), nc->CurrentCommand->Value);
			count++;
		}
		else
		{
			ncData[1] = NULL;
		}
	}
	else
	{
		ncData[0] = NULL;
	}
	
	if (nc->Client == NULL)
	{
		nc->Client = TCPClient_Create(IP_ADDR_SERVER, IP_PORT_SERVER);
	}
	strcpy(nc->Client->RemoteAddress, nc->PrevNode->IPAddress);
	nc->Client->RemotePort = nc->PrevNode->Port;
	printf("当前连接服务器：\n服务器名：%s\n服务器IP：%s\n服务器端口：%d\n",
				nc->PrevNode->Name,
				nc->PrevNode->IPAddress,
				nc->PrevNode->Port);
				
	nc->Connected = TCPClient_Connect(nc->Client);
	if ( !(nc->Connected) )
	{
		fprintf(stderr, "TCP连接出错，错误代码：%d\n", 1);
		return NULL;
	}
	ncp = NCProtocol_Create(
				CMD_SET_ID,
				2,
				ncData);
	
	//封装数据
	data = NCProtocol_Encapsul(ncp);
	//发送
	TCPClient_Send( nc->Client, data, ncp->totalLength );
	//接受服务器回复
	mlen = TCPClient_Receive( nc->Client, buffer, sizeof(buffer) );
	
	buffer[mlen] = 0;
	//printf("服务器：%s\n", buffer);
	//printf("*************************************************\n");

	free(data);
	NCProtocol_Dispose(ncp);
	TCPClient_Close(nc->Client);
	
	return NULL;
}

void OpenCommandHandler(NCClient * ncc)
{
	int i;
	bool result = true;
	NCProtocol * ncp;
	for (i = 0; i < ncc->ServerUpdater->NodeCount; i++)
	{
		printf("NodeCount:%d\n", ncc->ServerUpdater->NodeCount);
		//if (ncc->ServerUpdater->ServerList[i]->NodeState == SERVER_NODE_STATE_UPDATED)
		//{
			ncc->CurrentServer = ncc->ServerUpdater->ServerList[i];

			ncp = NCClient_ExecRemote(ncc);
			if (ncp->command == CMD_SERVER_REP_SUCCESS)
			{
				printf("打开数据库成功，位于服务器：%s\n", ncc->CurrentServer->Name);
			}
			else
			{
				printf("打开数据库失败，位于服务器：%s\n", ncc->CurrentServer->Name);
				result = false;
			}
			NCProtocol_Dispose(ncp);
		//}
	}
	if (result)
	{
		strcpy(ncc->DBFile, ncc->CurrentCommand->Key);
		ncc->Active = true;
	}

	return;
}

void CloseCommandHandler(NCClient * ncc)
{
	
	int i;
	bool result = true;
	NCProtocol * ncp;
	for (i = 0; i < ncc->ServerUpdater->NodeCount; i++)
	{
		//if (ncc->ServerUpdater->ServerList[i]->NodeState == SERVER_NODE_STATE_UPDATED)
		//{
			ncc->CurrentServer = ncc->ServerUpdater->ServerList[i];
			ncp = NCClient_ExecRemote(ncc);
			if (ncp->command == CMD_SERVER_REP_SUCCESS)
			{
				printf("关闭数据库成功，位于服务器：%s\n", ncc->CurrentServer->Name);
			}
			else
			{
				printf("关闭数据库失败，位于服务器：%s\n", ncc->CurrentServer->Name);
				result = false;
			}
			NCProtocol_Dispose(ncp);
		//}
	}
	if (result)
	{
		ncc->Active = false;
	}

	return;
}

void GetCommandHandler(NCClient * ncc)
{
	int i;
	NCProtocol * ncp;
	ServerNode * node = NULL;
	NCClient_SelectServer(ncc);
	ncp = NCClient_ExecRemote(ncc);
	/* ncp返回NULL表明该服务器已关闭或当机，需要更新本地服务器列表并重新选择服务器 */
	if (ncp == NULL)
	{
		printf("获取数据失败，位于服务器：%s\n", ncc->CurrentServer->IPAddress);
		printf("尝试重新获取。。。。\n");
		/* 更新服务器列表 */
		pthread_mutex_lock(&(ncc->UpdaterMutex));
		Updater_UpdateServer(ncc->ServerUpdater);
		pthread_mutex_unlock(&(ncc->UpdaterMutex));
		GetCommandHandler(ncc);
	}
	/* 获取数据失败，并且不是第二次选择服务器，表明选择的服务器为新增节点，需要向后查找一次服务器并重新获取数据 */
	else if (ncp->command == CMD_SERVER_REP_FAILED &&
			ncc->ReSelect == false)
	{
		ncc->ReSelect = true;
		ncc->PrevNode = ServerNode_Create(
				ncc->CurrentServer->ID,
				ncc->CurrentServer->Name,
				ncc->CurrentServer->IPAddress,
				ncc->CurrentServer->Port,
				ncc->CurrentServer->DataCount,
				ncc->CurrentServer->UpdateTag,
				ncc->CurrentServer->StartKey,
				ncc->CurrentServer->EndKey);
		GetCommandHandler(ncc);
	}
	else if (ncp->command == CMD_SERVER_REP_SUCCESS)
	{
		printf("获取数据成功，位于服务器：%s\n数据：%s\n", ncc->CurrentServer->IPAddress, ncp->dataChunk[0]->data);
		/* 如果重新选择过服务器，表明需要进行一致性维护，将当前获取到的数据同步到之前的服务器 */
		if (ncc->ReSelect)
		{
			ncc->CurrentCommand->Value = strdup(ncp->dataChunk[0]->data);
			i = pthread_create(&(ncc->ConsistThread), NULL, &NCClient_ConsistProcess, (void *)ncc);
			if (i != 0)
			{
				fprintf ( stderr, "线程创建失败,%s:%d\n", __FILE__, __LINE__ );
			}
		}
		sleep(2);
	}
	else
	{
		printf("获取数据失败，位于服务器：%s\n", ncc->CurrentServer->IPAddress);
	}
	if (ncp != NULL)
	{
		NCProtocol_Dispose(ncp);
	}

	return;
}

void SetCommandHandler(NCClient * ncc)
{
	NCProtocol * ncp;
	NCClient_SelectServer(ncc);
	ncp = NCClient_ExecRemote(ncc);
	if (ncp->command == CMD_SERVER_REP_SUCCESS)
	{
		printf("更新或插入数据成功，位于服务器：%s\n", ncc->CurrentServer->IPAddress);
	}
	else
	{
		printf("更新或插入数据失败，位于服务器：%s\n", ncc->CurrentServer->IPAddress);
	}
	NCProtocol_Dispose(ncp);

	return;
}

void DeleteCommandHandler(NCClient * ncc)
{
	NCProtocol * ncp;
	NCClient_SelectServer(ncc);
	ncp = NCClient_ExecRemote(ncc);
	if (ncp->command == CMD_SERVER_REP_SUCCESS)
	{
		printf("删除数据成功，位于服务器：%s\n", ncc->CurrentServer->IPAddress);
	}
	else
	{
		printf("删除数据失败，位于服务器：%s\n", ncc->CurrentServer->IPAddress);
	}
	NCProtocol_Dispose(ncp);

	return;
}

void QuitCommandHandler(NCClient * ncc)
{
	ncc->QuitFlag = true;
	return;
}
