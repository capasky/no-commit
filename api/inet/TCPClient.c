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
 * capasky				2012.12.18			1.0.1.0
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "TCPClient.h"
#include "inetdef.h"

/**
 * TCPClient_Create 创建一个新的 TCPClient 对象。
 * @param remoteIPAddress 服务端IP地址。
 * @param remotePort 服务端端口号。
 * @return 返回创建的 TCPClient 对象指针。
 */
TCPClient * TCPClient_Create(char * remoteIPAddress, int remotePort)
{
	TCPClient *tcpClient = ( TCPClient * ) malloc ( sizeof ( TCPClient ));
	tcpClient->Available = false;
	tcpClient->Client = INET_SOCKET_INVALID;
	//strcpy ( tcpClient->IPAddress, ipAddress );
	strcpy ( tcpClient->RemoteAddress, remoteIPAddress );
	tcpClient->Client = 0;
	//tcpClient->Port = port;
	tcpClient->RemotePort = remotePort;
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
bool TCPClient_Connect(TCPClient * client)
{
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons ( client->RemotePort );
	serveraddr.sin_addr.s_addr = inet_addr ( client->RemoteAddress );

	memset ( &serveraddr.sin_zero, 0, 8 );
	
	client->Client = socket ( AF_INET, SOCK_STREAM, 0 );
	
	if ( client->Client == INET_SOCKET_INVALID ||
	connect ( client->Client, ( struct sockaddr * ) &serveraddr,
				sizeof ( struct sockaddr )) == -1)
	{
		return false;
	}
	else
	{
		client->Connected = true;
		return true;
	}
}

/**
 * TCPClient_异步的连接
 * @param client TCPClient 结构体对象指针。
 * @param ipAddress 连接的主机的IP地址。
 * @param port 连接的主机的端口号。
 * @return 成功返回true，否则返回false。
 */
bool TCPClient_BeginConnect(TCPClient * client)
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
	if( close ( client->Client ) == 0 )
	{
		client->Connected = false;
		return true;
	}
	else
	{
		return false;
	}
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
