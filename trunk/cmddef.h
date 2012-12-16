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
 * capasky              2012.12.15          1.0.0.1
 */

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
#define CMD_RE          	"open|close|get|set|delete\s([a-z][A-Z])+\s\w+"
