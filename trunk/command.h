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

#define CMD_MAX_LEN     	64
#define CMD_OPEN_ID        	1
#define CMD_CLOSE_ID       	2
#define CMD_GET_ID         	3
#define CMD_SET_ID         	4
#define CMD_DEL_ID         	5
#define CMD_QUIT_ID        	6
#define CMD_MESSAGE_ID		102

#define CMD_OPEN        	"open"
#define CMD_CLOSE       	"close"
#define CMD_GET         	"get"
#define CMD_SET         	"set"
#define CMD_DEL         	"delete"
#define CMD_QUIT        	"quit"
#define CMD_RE          	"^(open\\s\\w+\\.\\w{2,5})|close|quit|((get|delete)\\s[0-9]+)|(set\\s[0-9]+\\s?.*)$"

#include "base.h"

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
