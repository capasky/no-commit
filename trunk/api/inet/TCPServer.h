/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.inet.tcpserver
 * Comments:             This file include the definition of TCPServer
 *                       struct and the interfaces using in TCPServer
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

#ifndef TCPSERVER_H_INCLUDE
#define TCPSERVER_H_INCLUDE

#include <arpa/inet.h>

#include "inetdef.h"

/**
  * TCPServer 结构体的定义
  * @author yellhb
  * @version 1.0.0.1
  */
typedef struct sTCPServer
{
	char	IPAddress[INET_IPADDR_STRING_LEN];	/* IP Address */
	SOCKET 	sockfd;				/* 基础Socket  */
	int 	Port;				/* 端口号 */
	struct 	sockaddr_in addr;	/* socket 信息 */
} TCPServer;

/**
  * TCPServer_Create 创建一个新的TCPServer对象
  * @param ipAddress 服务器端IP地址
  * @param port	服务器端端口号
  * @return 返回创建的TCPServer 对象指针
  */
TCPServer * TCPServer_Create ( char *ipAddress, int port );


/**
  * TCPServer_Bind 绑定服务器socket和端口
  * @param TCPServer指针
  * @return 返回绑定是否成功 -1绑定失败，0绑定成功
  */
int TCPServer_Bind ( TCPServer *tcpServer );

/**
  * TCPServer_Close 关闭Server的socket
  * @param TCPServer指针
  * @return 返回关闭是否成功 -1关闭失败， 0关闭成功
  */
int TCPServer_Close ( TCPServer *tcpServer ); 

#endif
