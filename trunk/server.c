/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               Server
 * Comments:             This file include the implement of server
 * Author:               yellhb
 * Create Date:          2012.12.15
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * yellhb               2012.12.15          1.0.0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include "api/inet/TCPServer.h"
#include "api/inet/protocol.h"
#include "api/inet/TCPListener.h"
#include "api/data/collection.h"
#include "cmddef.h"

#define MAX_BUF_SIZE		1024

Collection *cltion;
char* excuteCMD ( NCProtocol *protocol );
void* tFunction ( void* param );

struct Param 
{
	int 		sockfd;
	TCPServer*	server;
	NCProtocol*	ncprotocol;
};

int main ( int arg, char** argv )
{
	TCPServer*	tcpServer;	
	pthread_t	tid;
	Param*		param;

	tcpServer =  TCPServer_Create ( "0.0.0.0", 5533 );
	
	if ( TCPServer_Bind ( tcpServer ) == -1 )
	{
		fprintf ( stderr, "绑定错误, %s : %d,", __FILE__, __LINE__ );
		fprintf ( stderr, "%s : %d\n", ( char* ) inet_ntoa ( tcpServer->addr.sin_addr), 
				ntohs ( tcpServer->addr.sin_port ));
		TCPServer_Close ( tcpServer );
		return -1;
	}

	TCPServer_Listen ( tcpServer );

	while ( 1 )
	{
		int idata;
		int newfd = TCPServer_Accept ( tcpServer );
		if ( newfd == -1 )
		{
			fprintf ( stderr, "接收错误, %s : %d\n", __FILE__, __LINE__ );
		}
		else 
		{
			param = ( Param* ) malloc ( sizeof ( Param ));
			param->sockfd = newfd;
			param->server = tcpServer;
			if ( pthread_create ( &tid, NULL, tFunction, ( void* )param ) != 0 )
			
		}
	}
	TCPServer_Close ( tcpServer );

	return 0;
}

void* tFunction ( void* param )
{
	char* 	buf;
	char	sbuf[MAX_BUF_SIZE];
	param = ( struct Param* ) param;

	buf = TCPServer_Recv ( param->sockfd );
			
	if ( buf != NULL )
	{	
		param->ncprotocol = NCProtocol_Parse ( buf );
		printf ( "从 %s : %d 接收到消息\n 命令代码: %d	数据字段数: %d ", 
				( char* ) inet_ntoa ( param->server->clientaddr.sin_addr), 
				ntohs ( param->server->clientaddr.sin_port ), 
				param->ncprotocol->command, 
				param->ncprotocol->chunkCount );
		for ( idata = 0; idata < param->ncprotocol->chunkCount; ++idata )
		{
			printf ( " 数据%d : %s ", idata + 1, param->ncprotocol->dataChunk[idata]->data );
		}
		printf ( "\n" );
	}
	
	memset ( sbuf, 0, MAX_BUF_SIZE );
	strcpy ( sbuf, excuteCMD ( param->ncprotocol ));
	if ( TCPServer_Send ( param->sockfd, sbuf ) > 0 )
	{
		printf ( "回复 %s : %d -> %s\n", 
				( char* ) inet_ntoa ( param->server->clientaddr.sin_addr ), 
				ntohs ( param->server->clientaddr.sin_port ), 
				sbuf );
	}
	TCPServet_Close ( param->sockfd );
}

char * excuteCMD ( NCProtocol *protocol )
{
	char *retMsg = ( char* ) malloc ( MAX_BUF_SIZE * sizeof ( char ));
	memset ( retMsg, 0, MAX_BUF_SIZE );

	switch ( protocol->command )
	{
		case CMD_OPEN_ID:
			if (( cltion =  Collection_Create ( protocol->dataChunk[0]->data )) != NULL )
				strcpy ( retMsg, "数据库打开正常！" );
			else 
				strcpy ( retMsg, "数据库打开失败！" );
			break;
		case CMD_CLOSE_ID:
			if ( Collection_Dispose ( cltion ) < 0 )
				strcpy ( retMsg, "数据库关闭失败！" );
			else 
			{
				strcpy ( retMsg, "数据库关闭成功！" );
				printf ( "%s\n", retMsg );
			}
			break;
		case CMD_GET_ID:
			retMsg = Collection_GetStr ( cltion, protocol->dataChunk[0]->data );
			if ( retMsg == NULL )
			{
				retMsg = ( char* ) malloc ( MAX_BUF_SIZE * sizeof ( char ));
				strcpy ( retMsg, "数据查找失败：无此数据！");
			}
			break;
		case CMD_SET_ID:
			if ( Collection_AddStr ( cltion, 
						protocol->dataChunk[0]->data, protocol->dataChunk[1]->data ) < 0 )
				strcpy ( retMsg, "数据插入失败！" );
			else 
				strcpy ( retMsg, "数据插入成功！" );
			break;
		case CMD_DEL_ID:
			if ( Collection_RemoveStr ( cltion, protocol->dataChunk[0]->data ) < 0 )
				strcpy ( retMsg, "数据删除失败！" );
			else
				strcpy ( retMsg, "数据删除成功！" );
			break;
		defalut:
			strcpy ( retMsg, "无效命令!" );
	}
	
	return retMsg;
}
