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
 * yellhb				2012.12.20			1.0.1.1
 * yellhb				2012.12.21			1.0.1.2
 * yellhb				2012.12.22			1.0.1.3
 * yellhb				2013.1.5			1.0.2.1
 * yellhb				2013.1.12			1.0.2.2
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <pthread.h>

#include "api/inet/TCPServer.h"
#include "api/inet/protocol.h"
#include "api/inet/TCPListener.h"
#include "api/inet/TCPClient.h"
#include "api/data/collection.h"
#include "api/servercmd.h"
#include "command.h"

#define MAX_BUF_SIZE		1024
#define MASTERSERVER_IP		"192.168.130.24"

typedef struct PARAM
{
	int 		sockfd;
	TCPServer*	server;
	NCProtocol*	ncprotocol;
} Param;

int excuteCMD ( NCProtocol *protocol, char** retMsg );
void* tFunction ( void* pparam );
void* sFunction ();
void* suFunction ( void* server );
char* GetLocalIp (); 
void conMaster ( TCPServer* server );
Collection* cltion;

int main ( int arg, char** argv )
{
	TCPServer*	tcpServer;	
	NCProtocol* sprotocol;
	pthread_t	tid;
	pthread_t	stid;
	Param*		param;
	char*		rbk;
	char*		servIP;

	cltion = NULL;
	servIP = GetLocalIp();
	tcpServer =  TCPServer_Create ( servIP, rand() % 50000 + 1024 );
	
	while ( TCPServer_Bind ( tcpServer ) == -1 )
	{
		fprintf ( stderr, "尝试绑定 %s : %d 失败\n", ( char* ) inet_ntoa ( tcpServer->addr.sin_addr), 
				ntohs ( tcpServer->addr.sin_port ));
		TCPServer_Close ( tcpServer );
		free ( tcpServer );
		tcpServer = TCPServer_Create ( servIP, rand() % 50000 + 1024 );
	}
	
	conMaster ( tcpServer );
	TCPServer_Listen ( tcpServer );

	sprotocol = Server_ParseCommandToProtocol ( "open tchdb.hdb" );
	excuteCMD ( sprotocol, &rbk );
	free ( rbk );
	NCProtocol_Dispose ( sprotocol );

	printf ( "====================================================================\n" );
	printf ( "=                  服务器初始化完成，开始监听...                   =\n" );
	printf ( "=                  当前服务器地址:%15s                  =\n",(char*) inet_ntoa ( tcpServer->addr.sin_addr ));
	printf ( "=                  当前端口:%5d                                  =\n", ntohs ( tcpServer->addr.sin_port ));
	printf ( "====================================================================\n" );

	/* 增加线程处理服务器端命令 */
	if ( pthread_create ( &stid, NULL, sFunction, NULL ) != 0 )
	{
		fprintf ( stderr, "服务器线程创建失败! %s:%d\n", __FILE__, __LINE__ );
	}

	/* 增加线程向服务器更新信息 */
	if ( pthread_create ( &stid, NULL, suFunction, ( void* )tcpServer ) != 0 )
	{
		fprintf ( stderr, "服务器更新线程创建失败！ %s:%d\n", __FILE__, __LINE__ );
	}

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
			if ( pthread_create ( &tid, NULL, tFunction, ( void* )param ) != 0 )
			{
				fprintf ( stderr, "线程创建失败,%s:%d\n", __FILE__, __LINE__ );
			}
		}
		sleep ( 1 );
	}
	TCPServer_Close ( tcpServer );

	free ( tcpServer );
	free ( servIP );
	free ( cltion );
	return 0;
}

void* suFunction ( void* vserver )
{
	TCPServer* server = ( TCPServer* )vserver;
	int tmp = 0;

	while ( 1 )
	{ 
		TCPClient* 	client;
		char 		data[4];
		char* 		toSend;
		NCData**	ncData;
		NCProtocol*	ncp;
		
		client = TCPClient_Create ( MASTERSERVER_IP, 5533 );
		//client = TCPClient_Create ( getLocalIP(), 5533 );
		ncData = ( NCData** ) malloc ( sizeof ( struct sNCData* ) * 3 );
		ncData[0] = NCData_Create ( sizeof ( server->IPAddress ), server->IPAddress );
		memcpy ( data, &server->Port, 4 );
		ncData[1] = NCData_Create ( sizeof ( data ), data );
		if ( cltion != NULL )
			memcpy ( data, &cltion->dataCount, 4 );
		else
			memcpy ( data, &tmp, 4 );

		ncData[2] = NCData_Create ( sizeof ( data ), data );
		if ( TCPClient_Connect ( client ) == 0 )
			return;

		ncp = NCProtocol_Create ( CMD_SERVER_REP_NODE_ALIVE, 3, ncData );
		toSend = NCProtocol_Encapsul ( ncp );

		TCPClient_Send ( client, toSend, ncp->totalLength );

		TCPClient_Close ( client );
		sleep ( 10 );
	}
}

void* tFunction ( void* pparam )
{
	char* 		buf;
	char*		sbuf;
	Param*		param = ( Param* )pparam;
	NCProtocol*	ncp;
	NCData**	ncData;
	int 		idata;
	int 		flag;

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
	{
		pthread_detach ( pthread_self() );
		return NULL;
	}

	flag = excuteCMD ( param->ncprotocol, &sbuf );
	ncData = ( NCData** ) malloc ( sizeof ( struct sNCData* ));
	ncData[0] = NCData_Create ( strlen( sbuf ) + 1, sbuf );
	
	if ( flag == 1 )
		ncp = NCProtocol_Create ( CMD_SERVER_REP_SUCCESS, 1, ncData );
	else
		ncp = NCProtocol_Create ( CMD_SERVER_REP_FAILED, 1, ncData );
	buf = NCProtocol_Encapsul ( ncp );
	if ( TCPServer_SendL ( param->sockfd, buf, ncp->totalLength ) > 0 )
	{
		printf ( "\n回复 %s : %d -> %s\n", 
				( char* ) inet_ntoa ( param->server->clientaddr.sin_addr ), 
				ntohs ( param->server->clientaddr.sin_port ), 
				sbuf );
	}
	TCPServer_SockClose ( param->sockfd );
	NCProtocol_Dispose ( param->ncprotocol );
	free ( param );
	pthread_detach ( pthread_self() );
}

void* sFunction ()
{
	char* 		cmdBuf;
	char*		rbkBuf;
	NCProtocol *sprotocol;
	printf ( ">>" );

	cmdBuf = ( char* ) malloc ( sizeof ( char ) * MAX_BUF_SIZE );
	while ( gets(cmdBuf) )
	{
		sprotocol = Server_ParseCommandToProtocol ( cmdBuf );
		excuteCMD ( sprotocol, &rbkBuf );
		printf ( "\nserver: %s\n", rbkBuf );
		printf ( ">>" );
	}
	free ( rbkBuf );
	free ( sprotocol );
	pthread_detach ( pthread_self() );
	return NULL;
}

int excuteCMD ( NCProtocol *protocol, char** retMsg )
{
	int flag;
	*retMsg = ( char* ) malloc ( MAX_BUF_SIZE * sizeof ( char ));
	memset ( *retMsg, 0, MAX_BUF_SIZE );

	flag = 0;
	switch ( protocol->command )
	{
		case CMD_OPEN_ID:
			if ( cltion == NULL )
			{
				if (( cltion =  Collection_Create ( protocol->dataChunk[0]->data )) != NULL )
				{
					cltion->itemCount = 1;
					strcpy ( *retMsg, "数据库打开正常！" );
					flag = 1;
				}
				else 
					strcpy ( *retMsg, "数据库打开失败！" );
			}
			else 
			{
				cltion->itemCount++;
				strcpy ( *retMsg, "数据库已经打开！" );
				flag = 1;
			}
			break;
		case CMD_CLOSE_ID:
			if ( cltion != NULL )
			{
				if ( cltion->itemCount == 1 )
				{
					if ( Collection_Dispose ( cltion ) < 0 )
						strcpy ( *retMsg, "数据库关闭失败!" );
					else 
					{
						cltion = NULL;
						strcpy ( *retMsg, "数据库关闭成功!" );
						flag = 1;
					}
				}
				else if ( cltion->itemCount > 1 )
				{
					cltion->itemCount--;
					strcpy ( *retMsg, "数据库关闭成功!" );
					flag = 1;
				}
			}
			else 
			{
				strcpy ( *retMsg, "数据库关闭失败！" );
			}
			break;
		case CMD_GET_ID:
			if ( cltion != NULL )
			{
				*retMsg = Collection_GetStr ( cltion, protocol->dataChunk[0]->data );
				flag = 1;
			}
			else 
			{
				strcpy ( *retMsg, "链接已丢失!" );
			}

			if ( *retMsg == NULL )
			{
				*retMsg = ( char* ) malloc ( MAX_BUF_SIZE * sizeof ( char ));
				strcpy ( *retMsg, "数据查找失败：无此数据！");
				flag = 0;
			}
			break;
		case CMD_SET_ID:
			if ( cltion == NULL || Collection_AddStr ( cltion, 
						protocol->dataChunk[0]->data, protocol->dataChunk[1]->data ) < 0 )
				strcpy ( *retMsg, "数据插入失败！" );
			else 
			{
				flag = 1;
				strcpy ( *retMsg, "数据插入成功！" );
			}
			break;
		case CMD_DEL_ID:
			if ( cltion == NULL || Collection_RemoveStr ( cltion, protocol->dataChunk[0]->data ) < 0 )
				strcpy ( *retMsg, "数据删除失败！" );
			else
			{
				flag = 1;
				strcpy ( *retMsg, "数据删除成功！" );
			}
			break;
		defalut:
			strcpy ( *retMsg, "无效命令!" );
	}
	
	return flag;
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

void conMaster ( TCPServer* server )
{
	TCPClient* 	client;
	char 		data[4];
	char* 		toSend;
	NCData**	ncData;
	NCProtocol*	ncp;
	
	client = TCPClient_Create ( MASTERSERVER_IP, 5533 );
	//client = TCPClient_Create ( getLocalIP(), 5533 );
	ncData = ( NCData** ) malloc ( sizeof ( struct sNCData* ) * 2 );
	ncData[0] = NCData_Create ( sizeof ( server->IPAddress ), server->IPAddress );
	memcpy ( data, &server->Port, 4 );
	ncData[1] = NCData_Create ( sizeof ( data ), data );
	if ( TCPClient_Connect ( client ) == 0 )
		return;

	ncp = NCProtocol_Create ( CMD_SERVER_REQ_NODE_START, 2, ncData );
	toSend = NCProtocol_Encapsul ( ncp );

	TCPClient_Send ( client, toSend, ncp->totalLength );

	TCPClient_Close ( client );
}
