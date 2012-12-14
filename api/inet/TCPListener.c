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

#include "TCPListener.h"

/**
  * TCPListener_Create 创建一个TCPListener结构体对象
  */
TCPListener * TCPListen_Create ( char * ipAddress, int port )
{
	
}

/**
  * TCPListener_Accept 接受挂起到连接请求
  */
socket TCPListener_Accept ( TCPListener * listener )
{

}

/**
  * TCPListener_Start 启动对具有最大挂起连接数的传入连接请求到侦听
  */
bool TCPListener_Start ( TCPListener * listener, int backlog )
{

}

/**
  * TCPListener_Stop 关闭侦听器
  */
bool TCPListener_Stop ( TCPListener * listener )
{
	
}

