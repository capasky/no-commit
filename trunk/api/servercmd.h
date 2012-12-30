/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.servercmd
 * Comments:             This file include the definition of servercmd
 * Author:               capasky
 * Create Date:          2012.12.29
 * Version:              1.0
 * Plateform:            Any
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * capasky              2012.12.29          1.0.0.0
 */

 #ifndef SERVERCMD_H_INCLUDE
 #define SERVERCMD_H_INCLUDE

#include "inet/protocol.h"

/**
 * Server_ParseCommandToProtocol 将命令解析之后封装为NCProtocol对象
 * @param cmdString 命令字符串
 * @return 命令合法解析成功返回新建的NCProtocol对象，否则返回NULL
 */
NCProtocol * Server_ParseCommandToProtocol(char * cmdString);

 #endif
