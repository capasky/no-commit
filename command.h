/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               main
 * Comments:             This file include the defination of commands
 *                       function
 * Author:               capasky
 * Create Date:          2012.12.15
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * capasky              2012.12.19          1.0.0.0
 */

#ifndef COMMAND_H_INCLUDE
#define COMMAND_H_INCLUDE

#define CMD_MAX_LEN     				64			/* 命令字符串的最大长度 */

#define CMD_VALID						0			/* 非法命令 */

#define CMD_OPEN_ID						1			/* 打开数据库 */
#define CMD_CLOSE_ID					2			/* 关闭数据库 */
#define CMD_GET_ID						3			/* 获取数据 */
#define CMD_SET_ID						4			/* 设置数据 */
#define CMD_DEL_ID						5			/* 删除数据 */
#define CMD_QUIT_ID						6			/* 退出命令 */
#define CMD_MESSAGE_ID					9			/*  */

#define	CMD_CLIENT_REQ_NODE_LIST		10		/* 客户端请求数据服务器节点列表命令 */
#define	CMD_CLIENT_REQ_NODE_CHANGE		11		/* 客户端查询Master服务器是否需要更新服务器列表命令 */

#define	CMD_SERVER_REQ_NODE_PEND		20		/* Master服务器查询数据服务器节点状态命令 */
#define CMD_SERVER_REP_NODE_LIST		21		/* Master服务器向客户端返回服务器列表命令 */
#define CMD_SERVER_REP_NODE_KEEP		22		/* Master服务器向客户端返回服务器状态保持命令 */

#define CMD_SERVER_REQ_NODE_START		30		/* 节点服务器启动命令 */
#define CMD_SERVER_REQ_NODE_CLOSE		31		/* 节点服务器关闭命令 */
#define CMD_SERVER_REP_NODE_ALIVE		32		/* 节点服务器回复Master服务器本节点为活动命令 */
#define CMD_SERVER_REP_SUCCESS			50		/* 节点服务器执行命令成功 */
#define CMD_SERVER_REP_FAILED			51		/* 节点服务器执行命令失败 */

#define CMD_OPEN        	"open"
#define CMD_CLOSE       	"close"
#define CMD_GET         	"get"
#define CMD_SET         	"set"
#define CMD_DEL         	"delete"
#define CMD_QUIT        	"quit"
#define CMD_RE          	"^(open\\s\\w+\\.\\w{2,5})|close|quit|((get|delete)\\s[0-9]+)|(set\\s[0-9]+\\s.+)$"

#include <stdbool.h>

typedef struct sCommand
{
	int 	CommandID;
	char *	CommandText;
	char *	Key;
	char * 	Value;
} Command;

/**
 * Command_Create 创建Command结构体对象
 * @param commandID 命令ID
 * @param commandText 命令内容
 * @param key 键
 * @param value 值
 * @return 成功则返回创建的Command指针，否则返回NULL
 */
Command * Command_Create(
					int 	commandID,
					char *	commandText,
					char *	key,
					char * 	value);

/**
 * Command_TryParse 尝试解析命令字符串为命令结构体对象
 * @param cmdString
 * @return 成功则返回解析的Command指针，否则返回NULL
 */
Command * Command_TryParse(char * cmdString);

/**
 * 测试命令是否符合特定格式
 * 命令字符串
 * 成功则返回True，否则返回False
 */
bool Command_Test(char * cmdText);

/**
 * Command_GetID 获取命令ID
 * @param cmd 命令内容
 * @return 成功则返回相应命令ID ，否则返回0
 */
int Command_GetID(char * cmd);



#endif
