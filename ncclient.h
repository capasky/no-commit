/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               main
 * Comments:             This file include the entry point of client
 *                       function
 * Author:               capasky
 * Create Date:          2012.12.22
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/


/**
 * Revision Log:
 * @author              @date               @version
 * capasky              2012.12.22          1.0.0.0
 */

#ifndef NCCLIENT_H_INCLUDE
#define NCCLIENT_H_INCLUDE

#include <pthread.h>
#include <unistd.h>

#include "command.h"

#include "api/core/servernode.h"
#include "api/core/updater.h"

#include "api/inet/inetdef.h"
#include "api/inet/TCPClient.h"
#include "api/inet/protocol.h"

#include "api/utils/stringutils.h"

#define IP_ADDR_SERVER  	"192.168.130.34"
#define IP_PORT_SERVER  	5533

#define IP_ADDR_CLIENT		"127.0.0.1"
#define IP_ADDR_LOCAL		"127.0.0.1"

typedef struct sNCClient
{
	bool 			Active;
	char			DBFile[32];
	bool 			Connected;
	TCPClient * 	Client;
	Updater *		ServerUpdater;
	ServerNode *	CurrentServer;
	Command *		CurrentCommand;
	bool			CommandValid;
	NCProtocol* 	CurrentNCP;
	pthread_mutex_t	UpdaterMutex;
	pthread_t		UpdaterThread;
} NCClient;


/**
 * NCClient_Create 创建NCClient结构体对象
 * @return 成功则返回创建的NCClient对象的指针，否则返回NULL
 */
NCClient * NCClient_Create();

/**
 * NCClient_UpdateServer 更新服务器列表，用与分布式系统
 * @param ncc NCClient对象指针
 * @return 成功则返回true，否则返回false
 */
void NCClient_UpdateServer(NCClient * ncc);

/**
 * 根据当前操作对象选择分布式数据库服务器节点
 * @param ncc NCClient对象指针
 */
void NCClient_SelectServer(NCClient * ncc);

/**
 * NCClient_CheckCommand 检查命令
 * @param cmd 命令对象指针
 * @return 成功则返回true，否则返回false
 */
void NCClient_CheckCommand(NCClient * ncc);

/**
 * NCClient_Run 运行客户端
 * @param ncc NCClient对象指针
 */
void NCClient_Run(NCClient * ncc);

/**
 * NCClient_Preprocess 客户端预处理
 * @param ncc NCClient对象指针
 * @param cmdString 命令字符串
 */
void NCClient_Preprocess(NCClient * ncc, string cmdString);

/**
 * NCClient_PrepareData 客户端准备数据
 * @param ncc NCClient对象指针
 */
void NCClient_PrepareData(NCClient * ncc);

/**
 * NCClient_Execute 客户端执行
 * @param ncc NCClient对象指针
 */
void NCClient_Execute(NCClient * ncc);

/**
 * NCClient_ExecRemote 执行远程命令
 * @param ncc NCClient对象指针
 */
void NCClient_ExecRemote(NCClient * ncc);

/**
 * NCClient_Clean 客户端清理
 * @param ncc NCClient对象指针
 */
void NCClient_Clean(NCClient * ncc);

/**
 * NCClient_Updater 更新线程处理函数
 * @return 执行结束返回
 */
void * NCClient_Updater(void * ncc);

#endif
