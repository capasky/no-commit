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
#include "utils/stringutils.h"

NCProtocol * Server_ParseCommandToProtocol(string cmdString);

 #endif
