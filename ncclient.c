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
		ncc->ServerUpdater	= NULL;
		ncc->CurrentCommand = NULL;
		ncc->CurrentNCP		= NULL;
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
 * @param ncc NCClient对象指针
 */
void NCClient_SelectServer(NCClient * ncc)
{
	int node = 0;
	if (ncc->CurrentCommand != NULL)
	{
		if (ncc->CurrentCommand->CommandID == CMD_DEL_ID ||
			ncc->CurrentCommand->CommandID == CMD_GET_ID ||
			ncc->CurrentCommand->CommandID == CMD_SET_ID)
		{
			node = atoi(ncc->CurrentCommand->Key);
			ncc->CurrentServer = Updater_GetServer( ncc->ServerUpdater, node);
		}
	}
	if (ncc->CurrentServer == NULL)
	{
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
		NCClient_Preprocess(ncc, cmdString);
		if (ncc->CommandValid)
		{
			NCClient_PrepareData(ncc);
			NCClient_Execute(ncc);
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
	}
	
	return;
}

void NCClient_Preprocess(NCClient * ncc, string cmdString)
{
	ncc->CurrentCommand = Command_TryParse(cmdString);
	NCClient_CheckCommand(ncc);
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

void NCClient_Execute(NCClient * ncc)
{
	int i;
	if (ncc->CurrentCommand->CommandID == CMD_DEL_ID ||
		ncc->CurrentCommand->CommandID == CMD_GET_ID ||
		ncc->CurrentCommand->CommandID == CMD_SET_ID)
	{
		//选择服务器并建立连接
		NCClient_SelectServer(ncc);
		NCClient_ExecRemote(ncc);
	}
	else
	{
		for (i = 0; i < ncc->ServerUpdater->NodeCount; i++)
		{
			if (ncc->ServerUpdater->ServerList[i]->State == SERVER_NODE_STATE_UPDATED)
			{
				ncc->CurrentServer = ncc->ServerUpdater->ServerList[i];
				NCClient_ExecRemote(ncc);
			}
		}
		
		if(ncc->CurrentCommand->CommandID == CMD_OPEN_ID)
		{
			strcpy(ncc->DBFile, ncc->CurrentCommand->Key);
			ncc->Active = true;
		}
		if ( ncc->CurrentCommand->CommandID == CMD_CLOSE_ID )
		{
			ncc->Active = false;
		}
	}
	return;
}

/**
 * NCClient_ExecRemote 执行远程命令
 * @param ncc NCClient对象指针
 */
void NCClient_ExecRemote(NCClient * ncc)
{
	int mlen = 0;
	char buffer[1024];
	char * data;
	if (ncc->Client == NULL)
	{
		ncc->Client = TCPClient_Create(IP_ADDR_SERVER, IP_PORT_SERVER);
	}
	strcpy(ncc->Client->RemoteAddress, ncc->CurrentServer->IPAddress);
	ncc->Client->RemotePort = ncc->CurrentServer->Port;
	printf("当前连接服务器：\n服务器名：%s\n服务器IP：%s\n服务器端口：%d\n",
				ncc->CurrentServer->Name,
				ncc->CurrentServer->IPAddress,
				ncc->CurrentServer->Port
	);
	if ( !(ncc->Connected) )
	{
		ncc->Connected = TCPClient_Connect(ncc->Client);
		if ( !(ncc->Connected) )
		{
			fprintf(stderr, "TCP连接出错，错误代码：%d\n", 1);
			return;
		}
	}
	//封装数据
	data = NCProtocol_Encapsul(ncc->CurrentNCP);
	//发送
	TCPClient_Send( ncc->Client, data, ncc->CurrentNCP->totalLength );
	//接受服务器回复
	mlen = TCPClient_Receive( ncc->Client, buffer, sizeof(buffer) );
	
	ncc->Connected = false;
	
	buffer[mlen] = 0;
	printf("服务器：%s\n", buffer);
	printf("*************************************************\n");

	free(data);
	TCPClient_Close(ncc->Client);
	return;
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
	return;
}

/**
 * NCClient_Updater 更新线程处理函数
 * @return 执行结束返回0
 */
void * NCClient_Updater(void * ncc)
{
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
		sleep(60);
	}
	
	return NULL;
}
