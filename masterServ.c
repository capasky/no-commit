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
 * yellhb				2013.1.8			1.0.0.1
 * yellhb				2013.1.11			1.0.0.2
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
void* sFunction ();
char* GetLocalIp();  
int			version = 0;
int 		servAmount = 0;
DataNode*	servNodes;
int 		sendLen;

int main ( int arg, char** argv )
{
	TCPServer*	tcpServer;	
	pthread_t	tid;
	pthread_t	stid;
	Param*		param;
	char*		servIP;

	servIP = GetLocalIp();
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
	servNodes = NULL;

	printf ( "当前服务器IP:%s\n", servIP );
	printf ( "====================================================================\n" );
	printf ( "=            节点配置服务器初始化完成，开始监听...                 =\n" );
	printf ( "====================================================================\n" );

	/* 创建线程每5秒对所有节点轮寻 */
	if ( pthread_create ( &stid, NULL, sFunction, NULL ) != 0 )
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
				free ( param );
				fprintf ( stderr, "线程创建失败,%s:%d\n", __FILE__, __LINE__ );
			}
			sleep ( 1 );
		}
	}
	TCPServer_Close ( tcpServer );

	free ( tcpServer );
	free ( servIP );
	free ( servNodes );
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
		printf ( "\n从 %s : %d 接收到消息\n 命令代码: %d	数据字段数: %d \n", 
				( char* ) inet_ntoa ( param->server->clientaddr.sin_addr), 
				ntohs ( param->server->clientaddr.sin_port ), 
				param->ncprotocol->command, 
				param->ncprotocol->chunkCount );
		//free ( buf );
	}
	else
		return NULL;

	memset ( sbuf, 0, MAX_BUF_SIZE );
	memcpy ( sbuf, excuteCMD ( param->ncprotocol, param->server ), sendLen );

	if ( sbuf == NULL )
	{
		TCPServer_SockClose ( param->sockfd );
		return NULL;
	}

	if ( TCPServer_SendL ( param->sockfd, sbuf, sendLen ) > 0 )
	{
		printf ( "\n回复 %s : %d\n", 
				( char* ) inet_ntoa ( param->server->clientaddr.sin_addr ), 
				ntohs ( param->server->clientaddr.sin_port ));
	}
	TCPServer_SockClose ( param->sockfd );
	NCProtocol_Dispose ( param->ncprotocol );
	free ( param );
	pthread_detach ( pthread_self() );
}

void* sFunction ( )
{
	while ( 1 )
	{
		DataNode* 	pNode = servNodes;
		DataNode*	rNode = NULL;
		DataNode*	dNode = NULL;
		TCPClient*	client;
		if ( servNodes != NULL )
		{
			printf ( "--------------------------------------------------\n" );
			printf ( "数据服务器列表：                                  \n" );
			while ( pNode != NULL )
			{
				client = TCPClient_Create ( pNode->node.IPAddress, pNode->node.Port );
				if ( TCPClient_Connect ( client ) == 0 )
				{
					printf ( "服务器下线\n" );
					if ( rNode != NULL || ( rNode == NULL && pNode->next != NULL ))
					{
						if ( rNode != NULL )
						{
							rNode->next = pNode->next;
							rNode->node.EndKey = pNode->node.EndKey;
						}
						else
						{
							servNodes = pNode->next;
							servNodes->node.StartKey = 0;
							servNodes->node.EndKey = 0x7FFFFFFF - 1;
						}
						dNode = pNode;
						pNode = pNode->next;
						free ( dNode );
					}
					else
					{
						free ( servNodes );
						servNodes = NULL;
						pNode = NULL;
					}
					servAmount--;
					version = ( version + 1 ) % 1024;
				}	
				else
				{
					printf ( "数据服务器%d： %s %d \n", pNode->node.ID, pNode->node.IPAddress, pNode->node.Port );
					rNode = pNode;
					pNode = pNode->next;
				}
				TCPClient_Close ( client );
				free ( client );
			}
			printf ( "-------------------------------------------------\n" );
		}
		else
		{
			printf ( "-------------------------------------------------\n" );
			printf ( "数据服务器列表：暂时没有数据服务器上线           \n" );
			printf ( "-------------------------------------------------\n" );
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
	int			i, tmpv, id, datacnt;
	char* 		retMsg = ( char* ) malloc ( MAX_BUF_SIZE * sizeof ( char ));
	memset ( retMsg, 0, MAX_BUF_SIZE );
	
	switch ( protocol->command )
	{
		case CMD_CLIENT_REQ_NODE_LIST:
			memcpy ( &tmpv, protocol->dataChunk[0]->data, 4 );
			if ( tmpv == version )
			{
				ncp = NCProtocol_Create ( CMD_SERVER_REP_NODE_KEEP, 0, NULL );
				retMsg = NCProtocol_Encapsul ( ncp );
				sendLen = ncp->totalLength;
				break;
			}
			ncData = ( NCData** ) malloc ( sizeof ( struct sNCData* ) * ( servAmount + 1 ));
			memcpy ( data, &(version), sizeof ( data ));
			ncData[0] = NCData_Create ( sizeof ( int ), data );
			
			if ( servNodes == NULL )
			{
				ncp = NCProtocol_Create ( CMD_SERVER_REP_NODE_KEEP, 0, NULL );
				retMsg = NCProtocol_Encapsul ( ncp );
				sendLen = ncp->totalLength;
				printf ( "sendlen:%d\n", sendLen );
				break;
			}
			else
				pNode = servNodes;
		
			i = 0;
			while ( pNode )
			{
				ncData[++i] = NCData_Create ( sizeof ( ServerNode ), ServerNode_ToByte ( &(pNode->node )));
				pNode = pNode->next;
			}
			ncp = NCProtocol_Create ( CMD_SERVER_REP_NODE_LIST,
					servAmount + 1, ncData );
			retMsg = NCProtocol_Encapsul ( ncp );
			sendLen = ncp->totalLength;
			break;
		case CMD_SERVER_REQ_NODE_START:
			memcpy ( &tmpv, protocol->dataChunk[1]->data, 4 );
			printf ( "新服务器加入列表:ID:%d IP:%s Port:%d\n", 
					servAmount + 1, protocol->dataChunk[0]->data, tmpv );
			if ( servNodes == NULL )
				sNode = NULL;
			else
			{
				pNode = servNodes;
				rNode = servNodes;
				sNode = servNodes;
			
				while ( pNode != NULL )
				{
					if ( pNode->node.DataCount > sNode->node.DataCount )
						sNode = rNode;

					if ( rNode != servNodes )
						rNode = pNode;
					pNode = pNode->next;
				}
			}
			
			rNode = ( DataNode* ) malloc ( sizeof ( DataNode ));
			rNode->node.ID = ( servAmount + 1 ) % 65536;
			strcpy ( rNode->node.Name, "nocommit" );
			strcpy ( rNode->node.IPAddress, protocol->dataChunk[0]->data );
			rNode->node.Port = tmpv ;
			rNode->node.DataCount = 0;
			if ( sNode == NULL )
			{
				rNode->node.StartKey = 0;
				rNode->node.EndKey = 0x7FFFFFFF - 1;
				rNode->next = NULL;
				servNodes = rNode;
			}
			else
			{
				if ( sNode->next != NULL )
				{
					rNode->node.StartKey = sNode->next->node.StartKey;
					rNode->node.EndKey = sNode->next->node.StartKey - 1;
					sNode->next->node.StartKey = sNode->next->node.EndKey / 2;
					rNode->next = sNode->next;
					sNode->next = rNode;
				}
				else
				{
					rNode->node.StartKey = sNode->node.EndKey / 2;
					rNode->node.EndKey = 0x7FFFFFFF - 1;
					sNode->node.EndKey = rNode->node.StartKey - 1;
					sNode->next = rNode;
					rNode->next = NULL;
				}
			};
			servAmount = ( servAmount + 1 ) % 1024;
			version = ( version + 1 ) % 1024;
			break;
		case CMD_SERVER_REP_NODE_ALIVE:
			memcpy ( &tmpv, protocol->dataChunk[1]->data, 4 );
			memcpy ( &datacnt, protocol->dataChunk[2]->data, 4 );
			printf ( "数据服务器信息更新:IP:%s Port:%d", 
					protocol->dataChunk[0]->data, tmpv );
			pNode = servNodes;
			while ( pNode )
			{
				if ( strcpy ( pNode->node.IPAddress, protocol->dataChunk[0]->data) >= 0 &&
						pNode->node.Port == tmpv )
				{
					pNode->node.DataCount = datacnt;
					break;
				}
				pNode = pNode->next;
			}
			break;
		defalut:
			retMsg = NULL;
	}
	
	printf ( "version:%d\n", version );
	return retMsg;
}

char* GetLocalIp()  
{        
    int MAXINTERFACES = 16;  
    char *ip = NULL;
    int fd, intrface, retn = 0;    
    struct ifreq buf[MAXINTERFACES];    
    struct ifconf ifc;    

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)    
    {    
        ifc.ifc_len = sizeof(buf);    
        ifc.ifc_buf = (caddr_t)buf;    
        if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc))    
        {    
            intrface = ifc.ifc_len / sizeof(struct ifreq);    

            while (intrface-- > 0)    
            {    
                if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))    
                {    
                    ip=(inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));    
                    break;  
                }                        
            }  
        }    
        close (fd);    
        return ip;    
    }  
} 
