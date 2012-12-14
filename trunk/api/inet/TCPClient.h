/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.data.collection
 * Comments:             This file include the definition of TCPClient
 *                       struct and the interfaces using in TCPClient
 * Author:               capasky
 * Create Date:          2012.12.13
 * Version:              1.0
 * Plateform:            Any
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * capasky              2012.12.13          1.0.0.1
 */

#ifndef TCPCLIENT_H_INCLUDE
#define TCPCLIENT_H_INCLUDE

#include <arpa/inet.h>

#include "inetdef.h"

/**
 * TCPListener 结构体的定义
 * @author capasky
 * @version 1.0.0.1
 */
typedef struct sTCPClient
{
    bool    Active;                 /* 是否已建立连接。 */
    int     Available;              /* 已经从网络接收且可供读取的数据量。 */
    char    IPAddress[INET_IPADDR_STRING_LEN];       /* IP Address */
    socket  Client;                 /* 基础 Socket。 */
    bool    Connected;              /* Socket 是否已连接到远程主机。 */
    int     ReceiveBufferSize;      /* 接收缓冲区的大小。 */
    int     ReceiveTimeout;         /* TcpClient 等待接收数据的时间量。 */
    int     SendBufferSize;         /* 送缓冲区的大小。 */
    int     SendTimeout;            /* TcpClient 等待发送操作成功完成的时间量。 */

} TCPClient;

/**
 * TCPClient_Create 创建一个新的 TCPClient 对象。
 * @param ipAddress 客户端IP地址。
 * @param port 客户端端口号。
 * @return 返回创建的 TCPClient 对象指针。
 */
TCPClient * TCPClient_Create(char * ipAddress, int port);

/**
 * TCPClient_Connect connect to a client
 * @param client TCPClient 结构体对象指针。
 * @param ipAddress The IPAddress of the host to which you intend to connect.
 * @param port The port number to which you intend to connect.
 * @return 1 if success; or 0 if fail
 */
int TCPClient_Connect(TCPClient * client, char * ipAddress, int port);

/**
 * TCPClient_异步的连接
 * @param client TCPClient 结构体对象指针。
 * @param ipAddress 连接的主机的IP地址。
 * @param port 连接的主机的端口号。
 * @return 成功返回true，否则返回false。
 */
bool TCPClient_BeginConnect(TCPClient * client, char * ipAddress, int port);

/**
 * TCPClient_EndConnect 异步的结束一个TCP连接。
 * @param client TCPClient 结构体对象指针。
 * @return 成功返回true，否则返回false。
 */
bool TCPClient_EndConnect(TCPClient * client);

/**
 * TCPClient_Close 关闭一个TCP连接。
 * @param client TCPClient 结构体对象指针。
 * @return 成功返回true，否则返回false。
 */
bool TCPClient_Close(TCPClient * client);

/**
 * TCPClient_Receive 接收数据。
 * @param client TCPClient 结构体对象指针。
 * @param buffer 接受数据的缓冲区。
 * @param length 缓冲区长度。
 * @return 返回接受到数据的字节长度。
 */
int TCPClient_Receive(TCPClient * client, char * buffer, int length);

/**
 * TCPClient_Send 发送数据。
 * @param client TCPClient 结构体对象指针。
 * @param data 发送的数据。
 * @param length 发送数据的长度。
 * @return 返回发送的数据的字节长度。
 */
int TCPClient_Send(TCPClient * client, char * data, int length);

#endif
