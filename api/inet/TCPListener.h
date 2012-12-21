/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.data.collection
 * Comments:             This file include the definition of TCPListener
 *                       struct and the interfaces using in TCPListener
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

#ifndef TCPSERVER_H_INCLUDE
#define TCPSERVER_H_INCLUDE

#include <stdbool.h>

#include "inetdef.h"

/**
 * TCPListener 结构体的定义
 * @author capasky
 * @version 1.0.0.1
 */
typedef struct sTCPListener
{
    bool        Active;         /* 是否正主动侦听客户端连接。*/
    char        IPAddress[INET_IPADDR_STRING_LEN]; 		/* 当前 TcpListener 的IP地址 */
    SOCKET      Server;         /* 基础网络 Socket。 */
    int         Port;           /* 端口号 */
} TCPListener;

/**
 * TCPListener_Create 创建一个TCPClient结构体对象
 * @param ipAddress 指示TCP监听服务端的IP地址
 * @param port 指示端口号
 * @return 成功则返回创建的TCPListener结构体的指针，否则返回NULL
 */
TCPListener * TCPListener_Create(char * ipAddress, int port);

/**
 * TCPListener_Accept 接受挂起的连接请求。
 * @param listener TCPListener结构体对象指针。
 * @return 成功则返回创建的与客户端通信的SOCKET，否则返回空的SOCKET。
 */
SOCKET TCPListener_Accept(TCPListener * listener);

/**
 * TCPListener_Start 启动对具有最大挂起连接数的传入连接请求的侦听。
 * @param listener TCPListener结构体对象指针。
 * @param backlog 挂起连接队列的最大长度。
 * @return 成功则返回true，否则返回false
 */
bool TCPListener_Start(TCPListener * listener, int backlog);

/**
 * TCPListener_Stop 关闭侦听器。
 * @param listener TCPListener结构体对象指针。
 * @return 成功则返回true，否则返回false
 */
bool TCPListener_Stop(TCPListener * listener);

#endif
