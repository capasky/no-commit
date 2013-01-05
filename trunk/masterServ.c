/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               Server
 * Comments:             This file include the implement of masterServer
 * Author:               yellhb
 * Create Date:          2013.1.5
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * yellhb               2013.1.5          	1.0.0.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <pthread.h>

#include "api/inet/TCPServer.h"
#include "api/inet/protocol.h"
#include "api/inet/TCPClient.h"
#include "api/data/collection.h"
#include "api/servercmd.h"
#include "api/core/servernode.h"
#include "command.h"

#define MAX_BUF_SIZE		1024

typedef struct sDataNode
{
	ServerNode 			node;
	struct sDataNode 	*next;
} DataNode;

typedef struct PARAM
{
	int sockfd;
	TCPServer*	server;
	NCProtocol*	ncprotocol;
	DataNode*	sNodes;
} Param;

char* excuteCMD ( NCProtocol *protocol, TCPServer* server );
void* tFunction ( void* pparam );
void* sFunction ( void* dataNode );
char* getLocalIP();
int			version = 0;
int 		servAmount = 0;
DataNode*	servNodes;

int main ( int arg, char** argv )
{
	TCPServer*	tcpServer;	
	pthread_t	tid;
	pthread_t	stid;
	Param*		param;
	char*		servIP;

	servIP = getLocalIP();
	tcpServer =  TCPServer_Create ( servIP, 5533 );
	
	if ( TCPServer_Bind ( tcpServer ) == -1 )
	{
		fprintf ( stderr, "绑定错误, %s : %d,", __FILE__, __LINE__ );
		fprintf ( stderr, "%s : %d\n", ( char* ) inet_ntoa ( tcpServer->addr.sin_addr), 
				ntohs ( tcpServer->addr.sin_port ));
		TCPServer_Close ( tcpServer );
		return -1;
	}

	TCPServer_Listen ( tcpServer );
	servNodes = (DataNode*) malloc ( sizeof ( DataNode ));

	printf ( "====================================================================\n" );
	printf ( "=            节点配置服务器初始化完成，开始监听...                 =\n" );
	printf ( "====================================================================\n" );

	/* 创建线程每5秒对所有节点轮寻 */
	if ( pthread_create ( &stid, NULL, sFunction, (void*)&servNodes ) != 0 )
	{
		fprintf ( stderr, "服务器线程创建失败! %s:%d\n", __FILE__, __LINE__ );
	}

	/* 监听客户端更新请求 */
	while ( 1 )
	{
		int newfd = TCPServer_Accept ( tcpServer );
		if ( newfd == -1 )
		{
			fprintf ( stderr, "接收错误, %s : %d\n", __FILE__, __LINE__ );
		}
		else 
		{
			param = ( Param* ) malloc ( sizeof ( Param ));
			param->sockfd = newfd;
			param->server = ( TCPServer* ) malloc ( sizeof ( TCPServer ));
			memcpy ( &param->server, &tcpServer, sizeof ( tcpServer ));
			param->sNodes = servNodes;
			if ( pthread_create ( &tid, NULL, tFunction, ( void* )param ) != 0 )
			{
				fprintf ( stderr, "线程创建失败,%s:%d\n", __FILE__, __LINE__ );
			}
		}
	}
	TCPServer_Close ( tcpServer );

	return 0;
}

void* tFunction ( void* pparam )
{
	char* 	buf;
	char	sbuf[MAX_BUF_SIZE];
	Param*	param = ( Param* )pparam;
	int 	idata;

	buf = TCPServer_Recv ( param->sockfd );
			
	if ( buf != NULL )
	{	
		param->ncprotocol = NCProtocol_Parse ( buf );
		printf ( "\n从 %s : %d 接收到消息\n 命令代码: %d	数据字段数: %d ", 
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
	else
		return NULL;

	memset ( sbuf, 0, MAX_BUF_SIZE );
	strcpy ( sbuf, excuteCMD ( param->ncprotocol, param->server ));
	if ( TCPServer_Send ( param->sockfd, sbuf ) > 0 )
	{
		printf ( "\n回复 %s : %d\n", 
				( char* ) inet_ntoa ( param->server->clientaddr.sin_addr ), 
				ntohs ( param->server->clientaddr.sin_port ));
	}
	TCPServer_SockClose ( param->sockfd );
}

void* sFunction ( void* dataNode )
{
	DataNode* 	sNodes = *(( DataNode** ) dataNode);
	DataNode* 	pNode = NULL;
	DataNode*	rNode = NULL;
	DataNode*	dNode = NULL;
	TCPClient*	client;

	while ( 1 )
	{
		if ( sNodes != NULL )
		{
			pNode = sNodes->next;
			rNode = sNodes;
		}

		while ( pNode != NULL )
		{
			client = TCPClient_Create ( pNode->node.IPAddress, pNode->node.Port );
			if ( TCPClient_Connect ( client ) == 0 )
			{
				rNode->next = pNode->next;
				dNode = pNode;
				pNode = pNode->next;
				free ( dNode );
			}	
			else
			{
				rNode = rNode->next;
				pNode = pNode->next;
			}
			TCPClient_Close ( client );
			free ( client );
		}
		sleep ( 5 );
	}
}

char * excuteCMD ( NCProtocol *protocol, TCPServer* server )
{
	NCData**	ncData;
	NCProtocol*	ncp;
	DataNode*	pNode = NULL;
	DataNode*	sNode = NULL;
	DataNode*	rNode = NULL;
	char		data[4];
	int			i;
	char* 		retMsg = ( char* ) malloc ( MAX_BUF_SIZE * sizeof ( char ));
	memset ( retMsg, 0, MAX_BUF_SIZE );
	

	switch ( protocol->command )
	{
		case CMD_CLIENT_REQ_NODE_LIST:
			ncData = ( NCData** ) malloc ( sizeof ( struct sNCData* ) * ( servAmount + 1 ));
			memcpy ( data, &(version), sizeof ( data ));
			ncData[0] = NCData_Create ( sizeof ( int ), data );
			
			if ( servNodes != NULL )
				pNode = servNodes->next;

			i = 0;
			while ( pNode )
			{
				ncData[++i] = NCData_Create ( sizeof ( ServerNode ), (char*)&pNode->node );
				pNode = pNode->next;
			}
			ncp = NCProtocol_Create ( CMD_SERVER_REP_NODE_LIST,
					servAmount + 1, ncData );
			retMsg = NCProtocol_Encapsul ( ncp );
			break;
		case CMD_SERVER_REQ_NODE_START:
			if ( servNodes != NULL )
				pNode = servNodes->next;
			rNode = servNodes;
			sNode = servNodes;

			while ( pNode )
			{
				if ( pNode->node.DataCount > sNode->node.DataCount )
				{
					sNode = rNode;
				}
				rNode = rNode->next;
				pNode = pNode->next;
			}
			
			rNode = ( DataNode* ) malloc ( sizeof ( DataNode ));
			rNode->node.ID = servAmount + 1;
			strcpy ( rNode->node.IPAddress, server->IPAddress );
			rNode->node.Port = server->Port;
			rNode->node.DataCount = 0;
			rNode->node.StartKey = sNode->next->node.StartKey;
			rNode->node.EndKey = sNode->next->node.EndKey / 2;
			rNode->next = sNode->next;
			sNode->next = rNode;
			break;
		defalut:
			retMsg = NULL;
	}
	
	return retMsg;
}

char* getLocalIP()
{
	char* ip = NULL;
	int fd, interface, retn = 0;
	struct ifreq buf[8];
	struct ifconf ifc;

	if (( fd = socket ( AF_INET, SOCK_STREAM, 0 )) >= 0 )
	{
		ifc.ifc_len = sizeof ( buf );
		ifc.ifc_buf = (caddr_t)buf;
		if ( !ioctl ( fd, SIOCGIFCONF, (char*)&ifc ))
		{
			interface = ifc.ifc_len / sizeof ( struct ifreq );

			while ( interface-- > 0 )
			{
				if ( !ioctl ( fd, SIOCGIFCONF, (char*)&buf[interface] ))
				{
					ip = inet_ntoa (((struct sockaddr_in*)(&buf[interface].ifr_addr))->sin_addr );
					break;
				}
			}
		}
		close ( fd );
	}

	return ip;
}
