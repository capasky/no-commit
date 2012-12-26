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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>

#include "command.h"
#include "api/utils/stringutils.h"

/**
 * 测试命令是否符合特定格式
 * 命令字符串
 * 成功则返回True，否则返回False
 */
bool Command_Test(char * cmdText)
{
	int result;
	regex_t reg;
    regmatch_t pmatch[1];
    const size_t nmatch = 1;
    regcomp( &reg, CMD_RE, REG_EXTENDED | REG_ICASE | REG_NOSUB );
    result = regexec( &reg, cmdText, nmatch, pmatch, 0 );
    if ( result == 0 )
    {
        regfree(&reg);
        return true;
    }
    else
	{
		regfree(&reg);
		return false;
	}
}

/**
 * Command_GetID 获取命令ID
 * @param cmd 命令内容
 * @return 成功则返回相应命令ID ，否则返回0
 */
int Command_GetID(char * cmd)
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
 * @return 成功则返回解析的Command指针，否则返回NULL
 */
Command * Command_TryParse(char * cmdString)
{
	char * key = NULL,
		 * value = NULL;
	int index = 0;
	int srcLen = strlen(cmdString);
	int count = 0;
	if (srcLen < 1)
	{
		return NULL;
	}
	
	if ( Command_Test( cmdString ) == false )
	{
		return NULL;
	}
	
	string * splited = String_Split(cmdString, " ", &count);
	
	if (count > 1)
	{
		key = splited[1];
	}
	if (count > 2)
	{
		index = strlen(splited[0]) + strlen(splited[1]) + 3;
		value = String_Substring(cmdString, index, strlen(cmdString));
	}
	Command * cmd = Command_Create(
						Command_GetID(splited[0]),
						splited[0],
						key,
						value
						);
	return cmd;
}
