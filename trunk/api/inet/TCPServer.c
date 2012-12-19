/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.inet.tcpserver
 * Comments:             This file include the implementation of 
 * 						 TCPServer
 * Author:               yellhb
 * Create Date:          2012.12.19
 * Version:              1.0
 * Plateform:            Any
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * yellhb               2012.12.19          1.0.0.1
 */

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "TCPServer.h"
#include "inetdef.h"

/**
  * TCPServer_Create 创建一个新的TCPServer对象
  * @param ipAddress 服务器端口IP地址
  * @param port 服务器端端口号
  * @return 返回创建的TCPServer对象指针
  */
TCPServer * TCPServer_Create ( char *ipAddress, int port )
{
	TCPServer *tcpServer = ( TCPServer * ) malloc ( sizeof (TCPServer ));
	tcpServer->sockfd = socket ( PF_INET, SOCK_STREAM, 0);
	tcpServer->Port = port;
	strcpy ( tcpServer->IPAddress, ipAddress );
	tcpServer->addr.sin_family = AF_INET;
	tcpServer->addr.sin_port = htons ( port );
	tcpServer->addr.sin_addr.s_addr = inet_addr ( tcpServer->IPAddress );
	memset ( &(tcpServer->addr.sin_zero), 0, 8 );

	return tcpServer;
}

/**
  * TCPServer_Bind 绑定服务器socket和端口
  * @param TCPServer指针
  * @return 返回绑定是否成功 -1绑定失败，0绑定成功
  */
int TCPServer_Bind ( TCPServer *tcpServer )
{
	return  bind ( tcpServer->sockfd, 
				 ( struct sockaddr * ) &(tcpServer->addr), 
				 sizeof ( struct sockaddr ));
}

/**
  * TCPServer_Close 关闭Server的socket
  * @param TCPServer指针
  * @return 返回关闭是否成功 -1关闭失败， 0关闭成功
  */
int TCPServer_Close ( TCPServer *tcpServer )
{
	return close ( tcpServer->sockfd );
}

