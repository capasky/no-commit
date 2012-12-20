/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               main
 * Comments:             This file include the implementation of command
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

#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "command.h"
#include "api/utils/stringutils.h"

/**
 * 测试命令是否符合特定格式
 * 命令字符串
 * 成功则返回True，否则返回False
 */
static bool Command_Test(char * cmdText)
{
	return true;
}

/**
 * Command_GetID 获取命令ID
 * @param cmd 命令内容
 * @return 成功则返回相应命令ID ，否则返回0
 */
static int Command_GetID(char * cmd)
{
	if (cmd != NULL)
	{
		if (strcmp(cmd, CMD_OPEN) == 0)
		{
			return CMD_OPEN_ID;
		}
		else if (strcmp(cmd, CMD_CLOSE) == 0)
		{
			return CMD_CLOSE_ID;
		}
		else if (strcmp(cmd, CMD_GET) == 0)
		{
			return CMD_GET_ID;
		}
		else if (strcmp(cmd, CMD_SET) == 0)
		{
			return CMD_SET_ID;
		}
		else if (strcmp(cmd, CMD_DEL) == 0)
		{
			return CMD_DEL_ID;
		}
		else if (strcmp(cmd, CMD_QUIT) == 0)
		{
			return CMD_QUIT_ID;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
	
}

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
					char * 	value)
{
	Command * cmd = (Command *) malloc ( sizeof(struct sCommand) );
	cmd->CommandID = commandID;
	cmd->CommandText = strdup( commandText );
	if (key != NULL)
	{
		cmd->Key = strdup(key);
	}
	else
	{
		cmd->Key = NULL;
	}
	if (value != NULL)
	{
		cmd->Value = strdup(value);
	}
	else
	{
		cmd->Value = NULL;
	}
	return cmd;
}

/**
 * Command_TryParse 尝试解析命令字符串为命令结构体对象
 * @param cmdString
 * @param cmd
 * @return 成功则返回True，否则返回False
 */
bool Command_TryParse(char * cmdString, Command * cmd)
{
	char * key = NULL,
		 * value = NULL;
	int index = 0;
	String commandString = String_Create(cmdString);
	if (cmd == NULL || commandString.Length < 1)
	{
		return false;
	}
	StringArrry sa = String_Split(
						commandString,
						String_Create(" ")
						);
	
	if (sa.Count > 1)
	{
		key = sa.Splited[1].data;
	}
	if (sa.Count > 2)
	{
		value = String_Substring(
						commandString,
						index,
						commandString.Length - 1
						).data;
	}
	
	cmd = Command_Create(
				Command_GetID(sa.Splited[0].data),
				sa.Splited[0].data,
				key,
				value
				);
	return true;
}
