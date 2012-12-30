/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.servercmd
 * Comments:             This file include the definition of servercmd
 * Author:               capasky
 * Create Date:          2012.12.29
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * capasky              2012.12.29          1.0.0.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "inet/protocol.h"
#include "utils/stringutils.h"
#include "../command.h"

NCProtocol * Server_ParseCommandToProtocol(string cmdString)
{
	NCData ** 	ncData;
	int 		count = 0;
	Command * cmd = Command_TryParse(cmdString);
	if (cmd == NULL)
	{
		return NULL;
	}
	ncData = (NCData **)malloc( sizeof( struct sNCData *) * 2);
	
	if (cmd->Key != NULL)
	{
		ncData[0] = NCData_Create(strlen(cmd->Key), cmd->Key);
		count++;
		if (cmd->Value != NULL)
		{
			ncData[1] = NCData_Create(strlen(cmd->Value), cmd->Value);
			count++;
		}
		else
		{
			ncData[1] = NULL;
		}
	}
	else
	{
		ncData[0] = NULL;
	}
	return NCProtocol_Create( cmd->CommandID, count, ncData );
}

