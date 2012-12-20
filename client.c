/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               main
 * Comments:             This file include the entry point of main
 *                       function
 * Author:               capasky
 * Create Date:          2012.12.08
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * capasky              2012.12.08          1.0.0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "api/inet/TCPClient.h"
#include "api/inet/protocol.h"
#include "api/inet/inetdef.h"

#include "command.h"

#define IP_ADDR_SERVER  	"127.0.0.1"
#define IP_PORT_SERVER  	5533

#define IP_ADDR_CLIENT		"127.0.0.1"

#define IP_ADDR_LOCAL		"127.0.0.1"

typedef struct sNetContext
{
	bool 		Active;
	char *		DBFile;
	bool 		Connected;
	TCPClient * Client;
} NetContext;

static NetContext Current;

void ExecuteCommand(Command * cmd);
NCProtocol * GenerateNCP(Command * cmd);
bool CheckCommand(Command * cmd);

int main ( int argc, char **argv )
{
	char 			cmdString[CMD_MAX_LEN];
	Command *		cmd;

	Current.Active 		= false;
	Current.Connected 	= false;
	Current.Client 		= NULL;
	Current.DBFile 		= NULL;
	
	printf("NoCommit1>>");
	while( gets(cmdString) != NULL)
	{
		cmd = Command_TryParse(cmdString);
		if (CheckCommand(cmd))
		{
			ExecuteCommand(cmd);
		}
		if (Current.Active)
		{
			printf("%s::%s>>",
				Current.Client->RemoteAddress, Current.DBFile);
		}
		else
		{
			printf("NoCommit>>");
		}
	}
	return 0;

}

void ExecuteCommand(Command * cmd)
{
	int 		i;
	int 		mlen = 0;
	char 		buffer[1024];
	NCProtocol *ncp;
	NCData * 	ncData[2];
	int 		count = 0;
	if (cmd == NULL || cmd->CommandID == CMD_QUIT_ID)
	{
		return;
	}
	
	if (cmd->Key != NULL)
	{
		ncData[0] = NCData_Create(strlen(cmd->Key), cmd->Key);
		count++;
		if (cmd->Value != NULL)
		{
			ncData[1] = NCData_Create(strlen(cmd->Value), cmd->Value);
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
	ncp = NCProtocol_Create( cmd->CommandID, count, ncData );
	
	if (Current.Client == NULL)
	{
		Current.Client = TCPClient_Create( IP_ADDR_SERVER, IP_PORT_SERVER );
	}
	if ( !(Current.Connected) )
	{
		Current.Connected = TCPClient_Connect(Current.Client);
		if ( !(Current.Connected) )
		{
			fprintf(stderr, "TCP连接出错，错误代码：%d\n", 1);
			return;
		}
	}
	
	TCPClient_Send( Current.Client, NCProtocol_Encapsul(ncp), ncp->totalLength );
	mlen = TCPClient_Receive( Current.Client, buffer, sizeof(buffer) );
	
	Current.Connected = false;
	
	buffer[mlen] = '\0';
	printf("服务器：%s\n", buffer);
	if ( cmd->CommandID == CMD_OPEN_ID )
	{
		Current.DBFile = strdup(cmd->Key);
		Current.Active = true;
	}
	if ( cmd->CommandID == CMD_CLOSE_ID )
	{
		TCPClient_Close( Current.Client );
		Current.Active = false;
	}
	
	return;
}

bool CheckCommand(Command * cmd)
{
	if (cmd == NULL)
	{
		printf("命令格式错误！\n");
		return false;
	}
	if (cmd->CommandID == CMD_QUIT_ID)
	{
		exit(0);
	}
	if (cmd->CommandID == CMD_OPEN_ID && Current.Active)
	{
		printf("数据库连接已打开!\n");
		return false;
	}
	if (cmd->CommandID != CMD_OPEN_ID && !(Current.Active))
	{
		printf("请先打开数据库连接!用法：open filename.ext\n");
		return false;
	}
	return true;
}
