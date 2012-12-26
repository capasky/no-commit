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

#include "command.h"

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
	NCClient * ncc = (NCClient *) malloc( sizeof(struct sNCClient) );
	if (ncc != NULL)
	{
		ncc->Active 		= false;
		ncc->DBFile 		= NULL;
		ncc->Connected 		= false;
		ncc->Client 		= NULL;
		ncc->ServerList 	= NULL;
		ncc->CurrentCommand = NULL;
		ncc->CurrentNCP		= NULL;
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
	//TODO:
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
	char cmdString[1024];
	printf("NoCommit>>");
	while( gets(cmdString) != NULL)
	{
		NCClient_Preprocess(ncc, cmdString);
		printf("%d\n", ncc->CommandValid);
		if (ncc->CommandValid)
		{
			NCClient_PrepareData(ncc);
			NCClient_Execute(ncc);
		}
		if (ncc->Active)
		{
			printf("%s::%s>>",
				ncc->Client->RemoteAddress, ncc->DBFile);
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
	NCClient_UpdateServer(ncc);
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
	int mlen = 0;
	char buffer[1024];
	char * data;
	if (ncc->Client == NULL)
	{
		ncc->Client = TCPClient_Create( IP_ADDR_SERVER, IP_PORT_SERVER );
	}
	if ( !(ncc->Connected) )
	{
		ncc->Connected = TCPClient_Connect(ncc->Client);
		if ( !(ncc->Connected) )
		{
			fprintf(stderr, "TCP连接出错，错误代码：%d\n", 1);
			return;
		}
	}
	data = NCProtocol_Encapsul(ncc->CurrentNCP);
	
	TCPClient_Send( ncc->Client, data, ncc->CurrentNCP->totalLength );
	
	mlen = TCPClient_Receive( ncc->Client, buffer, sizeof(buffer) );
	
	ncc->Connected = false;
	
	buffer[mlen] = 0;
	printf("服务器：%s\n", buffer);

	free(data);
	
	if ( ncc->CurrentCommand->CommandID == CMD_OPEN_ID )
	{
		ncc->DBFile = strdup(ncc->CurrentCommand->Key);
		ncc->Active = true;
	}
	if ( ncc->CurrentCommand->CommandID == CMD_CLOSE_ID )
	{
		TCPClient_Close( ncc->Client );
		ncc->Active = false;
	}
	return;
}

void NCClient_Clean(NCClient * ncc)
{
	NCProtocol_Dispose(ncc->CurrentNCP);
	ncc->CurrentNCP = NULL;
	return;
}
