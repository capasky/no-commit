/***********************************************************************
 * opyRright:           NoCommit Team
 * Project:             NoCommit
 * Module:              api.inet.TCPListener
 * Comments:            This file include the implementationon of 
 *						the interfaces using in TCPListener
 * Author:				yellhb
 * Create Date:			2012.12.13
 * Version:				1.0
 * Plateform:			Linux
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * yellhb               2012.12.14          1.0.0.1
 */

#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include "TCPListener.h"
#include "inetdef.h"

/**
 * TCPListener_Create 创建一个TCPClient结构体对象
 * @param ipAddress 指示TCP监听服务端的IP地址
 * @param port 指示端口号
 * @return 成功则返回创建的TCPListener结构体的指针，否则返回NULL
 */
TCPListener * TCPListener_Create(char * ipAddress, int port)
{
	TCPListener *listener = ( TCPListener * ) malloc ( sizeof ( TCPListener ));
	listener->Active = false;
	strcpy ( listener->IPAddress, ipAddress );
	listener->Server = -1;
	listener->Port = 0;

	return listener;
}

/**
 * TCPListener_Accept 接受挂起的连接请求。
 * @param listener TCPListener结构体对象指针。
 * @return 成功则返回创建的与客户端通信的SOCKET，否则返回空的SOCKET。
 */
SOCKET TCPListener_Accept(TCPListener * listener)
{
	return 0;
}

/**
 * TCPListener_Start 启动对具有最大挂起连接数的传入连接请求的侦听。
 * @param listener TCPListener结构体对象指针。
 * @param backlog 挂起连接队列的最大长度。
 * @return 成功则返回true，否则返回false
 */
bool TCPListener_Start(TCPListener * listener, int backlog)
{
	return listen ( listener->Server, backlog );
}

/**
 * TCPListener_Stop 关闭侦听器。
 * @param listener TCPListener结构体对象指针。
 * @return 成功则返回true，否则返回false
 */
bool TCPListener_Stop(TCPListener * listener)
{
	return close ( listener->Server );
}

