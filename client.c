/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               main
 * Comments:             This file include the entry point of main
 *                       function
 * Author:               capasky
 * Create Date:          2012.12.08
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * capasky              2012.12.08          1.0.0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api/inet/TCPClient.h"
#include "api/inet/protocol.h"

#define IP_ADDR_SERVER  "192.168.1.102"
#define IP_PORT_SERVER  8888

#define CMD_MAX_LEN     	64
#define CMD_OPEN_ID        	1
#define CMD_CLOSE_ID       	2
#define CMD_GET_ID         	3
#define CMD_SET_ID         	4
#define CMD_DEL_ID         	5
#define CMD_QUIT_ID        	6

#define CMD_OPEN        	"open"
#define CMD_CLOSE       	"close"
#define CMD_GET         	"get"
#define CMD_SET         	"set"
#define CMD_DEL         	"delete"
#define CMD_QUIT        	"quit"
#define CMD_RE          	"open|close|get|set|delete\s([a-z][A-Z])+\s\w+"

int main ( int argc, char **argv )
{
    char 			cmd[CMD_MAX_LEN];
    char * 			key,
         * 			value;
    int 			cmp = -1;
    bool 			result = false;
    char 			buffer[1024];
    TCPClient * 	tcpClient;

    NCProtocol * 	ncp;
    NCData * 		ncData[2];
    
    tcpClient = TCPClient_Create(IP_ADDR_SERVER, IP_PORT_SERVER);
    result = TCPClient_Connect(tcpClient, IP_ADDR_SERVER, IP_PORT_SERVER);
    if (!result)
	{
		fprintf(stderr, "TCP连接出错，错误代码：%d", errno);
		return ( EXIT_FAILURE );
	}
	printf("Connect to server %s successful!\n", tcpClient->IPAddress);
    printf("%s>>", tcpClient->IPAddress);
    gets(cmd);

    while ( strcmp( cmd, CMD_QUIT ) != 0 )
    {
        switch (*cmd)
        {
        case 'o':
            cmp = strncmp(cmd, CMD_OPEN, sizeof(CMD_OPEN) - 1);
            if ( cmp == 0 )
            {
                if (wow == NULL )
                {
                    key = strchr(cmd, ' ');
                    *key = '\0';
                    key++;
                    ncData[0] = NCData_Create(strlen(key), key);
                    ncp = NCProtocol_Create(
								CMD_OPEN_ID,
								1,
								ncData);
					TCPClient_Send(tcpClient,
								NCProtocol_Encapsul(ncp),
								ncp->totalLength);
					TCPClient_Receive(tcpClient,
								buffer,
								sizeof(buffer));
					
                    printf("服务器：%s\n", buffer);
                }
                else
                {
                    printf("文件 \"%s\" 已经打开!\n", wow->dbFile);
                }
            }
            else
            {
                printf("Can't find command \"%s\"\n", cmd);
            }
            break;
        case 'c':
            cmp = strncmp(cmd, CMD_CLOSE, sizeof(CMD_CLOSE) - 1);
            if ( cmp == 0 )
            {
                if ( wow != NULL )
                {
                    Collection_Dispose(wow);
                    wow = NULL;
                    printf("OK\n");
                }
                else
                {
                    printf("Please open or create a database file first!\n");
                }
            }
            else
            {
                printf("Can't find command \"%s\"\n", cmd);
            }
            break;
        case 's':
            cmp = strncmp(cmd, CMD_SET, sizeof(CMD_SET) - 1);
            if ( cmp == 0 )
            {
                if ( wow != NULL )
                {
                    key = strchr(cmd, ' ');
                    *key = '\0';
                    key++;
                    value = strchr(key, ' ');
                    *value = '\0';
                    value++;
                    Collection_AddStr(wow, key, value);
                    printf("OK\n");
                }
                else
                {
                    printf("Please open or create a database file first!\n");
                }
            }
            else
            {
                printf("Can't find command \"%s\"\n", cmd);
            }
            break;
        case 'g':
            cmp = strncmp(cmd, CMD_GET, sizeof(CMD_GET) - 1);
            if ( cmp == 0 )
            {
                if ( wow != NULL )
                {
                    key = strchr(cmd, ' ');
                    *key = '\0';
                    key++;
                    printf("{%s : %s}\n", key, Collection_GetStr(wow, key));
                    printf("OK\n");
                }
                else
                {
                    printf("Please open or create a database file first!\n");
                }
            }
            else
            {
                printf("Can't find command \"%s\"\n", cmd);
            }
            break;
        case 'd':
            cmp = strncmp(cmd, CMD_DEL, sizeof(CMD_DEL) - 1);
            if ( cmp == 0 )
            {
                if ( cmp == 0 && wow != NULL )
                {
                    key = strchr(cmd, ' ');
                    *key = '\0';
                    key++;
                    Collection_RemoveStr(wow, key);
                    printf("OK\n");
                }
                else
                {
                    printf("Please open or create a database file first!\n");
                }
            }
            else
            {
                printf("Can't find command \"%s\"\n", cmd);
            }
            break;
        default:
            printf("Can't find command \"%s\"\n", cmd);
            break;
        }

        fflush( stdin );
        if (wow == NULL)
        {
            printf("nocommit>>");
        }
        else
        {
            printf("nocommit::%s>>", wow->dbFile);
        }
        gets(cmd);
    }

    return 0;
}
