/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.inet.TCPClient
 * Comments:             This file include the implementation of 
 *						 the interfaces using in TCPClient
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

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TCPClient.h"
#include "inetdef.h"

/**
 * TCPClient_Create 创建一个新的 TCPClient 对象。
 * @param ipAddress 客户端IP地址。
 * @param port 客户端端口号。
 * @return 返回创建的 TCPClient 对象指针。
 */
TCPClient * TCPClient_Create(char * ipAddress, int port)
{
	TCPClient *tcpClient = ( TCPClient * ) malloc ( sizeof ( TCPClient ));
	tcpClient->Active = false;
	tcpClient->Available = false;
	strcpy ( tcpClient->IPAddress, ipAddress );
	tcpClient->Client = 0;
	tcpClient->Port = port;
	tcpClient->Connected = false;
	tcpClient->ReceiveBufferSize = RECEIVE_BUFFER_SIZE;
	tcpClient->ReceiveTimeout = RECEIVE_TIMEOUT;
	tcpClient->SendBufferSize = SEND_BUFFER_SIZE;
	tcpClient->SendTimeout = SEND_TIMEOUT;
	return tcpClient;
}

/**
 * TCPClient_Connect connect to a client
 * @param client TCPClient 结构体对象指针。
 * @param ipAddress The IPAddress of the host to which you intend to connect.
 * @param port The port number to which you intend to connect.
 * @return 1 if success; or 0 if fail
 */
bool TCPClient_Connect(TCPClient * client, char * ipAddress, int port)
{
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons ( port );
	serveraddr.sin_addr.s_addr = inet_addr ( ipAddress );

	memset ( &serveraddr.sin_zero, 0, 8 );
	client->Client = socket ( PF_INET, SOCK_STREAM, 0 );
	if ( connect ( client->Client, ( struct sockaddr * ) &serveraddr,
				sizeof ( struct sockaddr )) == -1)
		return false;
	else
		return true;

}

/**
 * TCPClient_异步的连接
 * @param client TCPClient 结构体对象指针。
 * @param ipAddress 连接的主机的IP地址。
 * @param port 连接的主机的端口号。
 * @return 成功返回true，否则返回false。
 */
bool TCPClient_BeginConnect(TCPClient * client, char * ipAddress, int port)
{
	/* TODO */
	return false;
}

/**
 * TCPClient_EndConnect 异步的结束一个TCP连接。
 * @param client TCPClient 结构体对象指针。
 * @return 成功返回true，否则返回false。
 */
bool TCPClient_EndConnect(TCPClient * client)
{
	/* TODO */
	return false;
}

/**
 * TCPClient_Close 关闭一个TCP连接。
 * @param client TCPClient 结构体对象指针。
 * @return 成功返回true，否则返回false。
 */
bool TCPClient_Close(TCPClient * client)
{
	return close ( client->Client );
}

/**
 * TCPClient_Receive 接收数据。
 * @param client TCPClient 结构体对象指针。
 * @param buffer 接受数据的缓冲区。
 * @param length 缓冲区长度。
 * @return 返回接受到数据的字节长度。
 */
int TCPClient_Receive(TCPClient * client, char * buffer, int length)
{
	return recv ( client->Client, buffer, length, 0 );
}

/**
 * TCPClient_Send 发送数据。
 * @param client TCPClient 结构体对象指针。
 * @param data 发送的数据。
 * @param length 发送数据的长度。
 * @return 返回发送的数据的字节长度。
 */
int TCPClient_Send(TCPClient * client, char * data, int length)
{
	return send ( client->Client, data, length, 0 );
}
