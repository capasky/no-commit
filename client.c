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
#include "api/inet/inetdef.h"

#include "cmddef.h"

#define IP_ADDR_SERVER  	"192.168.130.194"
#define IP_PORT_SERVER  	5533

#define IP_ADDR_CLIENT		"192.168.130.143"

int main ( int argc, char **argv )
{
    char 			cmd[CMD_MAX_LEN];
    char * 			key,
         * 			value;
    int 			cmp = -1;
    bool 			result = false;
    char 			buffer[1024];
    char *			dbFile;
    TCPClient * 	tcpClient = NULL;

    NCProtocol * 	ncp;
    NCData * 		ncData[2];

	int 			mlen = 0;
    printf("NoCommit>>");
    
    gets(cmd);

    while ( strcmp( cmd, CMD_QUIT ) != 0 )
    {
        switch (*cmd)
        {
        case 'o':
            cmp = strncmp(cmd, CMD_OPEN, sizeof(CMD_OPEN) - 1);
            if ( cmp == 0 )
            {
                if (tcpClient == NULL )
                {
                    key = strchr(cmd, ' ');
                    *key = '\0';
                    key++;
                    dbFile = strdup(key);

					tcpClient = TCPClient_Create(IP_ADDR_CLIENT, 0);
					result = TCPClient_Connect(tcpClient, IP_ADDR_SERVER, IP_PORT_SERVER);
					if (!result || tcpClient == NULL)
					{
						fprintf(stderr, "TCP连接出错，错误代码：%d\n", 1);
					}
					else
					{
						printf("已连接到服务器 %s !\n", tcpClient->IPAddress);
						ncData[0] = NCData_Create(strlen(key), key);
						ncp = NCProtocol_Create(
									CMD_OPEN_ID,
									1,
									ncData);
					}
                }
                else
                {
                    printf("文件 \"%s\" 已经打开!\n", dbFile);
                }
            }
            else
            {
                printf("没有此命令： \"%s\"\n", cmd);
            }
            break;
        case 'c':
            cmp = strncmp(cmd, CMD_CLOSE, sizeof(CMD_CLOSE) - 1);
            if ( cmp == 0 )
            {
                if ( tcpClient != NULL )
                {
                    ncp = NCProtocol_Create(
								CMD_CLOSE_ID,
								0,
								NULL);
					TCPClient_Send(tcpClient,
								NCProtocol_Encapsul(ncp),
								ncp->totalLength);
					mlen = TCPClient_Receive(tcpClient,
								buffer,
								sizeof(buffer));
					//NCProtocol_Dispose(ncp);
					buffer[mlen] = '\0';
					printf("服务器：%s\n", buffer);
					printf("%s::%s>>", tcpClient->IPAddress, dbFile);
					if (TCPClient_Close(tcpClient))
					{
						printf("已关闭与服务器的连接");
					}
					free(tcpClient);
					tcpClient = NULL;
                }
                else
                {
                    printf("请先与服务器建立连接!\n");
                }
            }
            else
            {
                printf("没有此命令： \"%s\"\n", cmd);
            }
            break;
        case 's':
            cmp = strncmp(cmd, CMD_SET, sizeof(CMD_SET) - 1);
            if ( cmp == 0 )
            {
                if ( tcpClient != NULL )
                {
					result = TCPClient_Connect(tcpClient, IP_ADDR_SERVER, IP_PORT_SERVER);
                    key = strchr(cmd, ' ');
                    *key = '\0';
                    key++;
                    value = strchr(key, ' ');
                    *value = '\0';
                    value++;
                    
                    ncData[0] = NCData_Create(strlen(key), key);
                    ncData[1] = NCData_Create(strlen(value), value);
                    
                    ncp = NCProtocol_Create(
								CMD_SET_ID,
								2,
								ncData);
                }
                else
                {
                    printf("先与服务器建立连接!\n");
                }
            }
            else
            {
                printf("没有此命令： \"%s\"\n", cmd);
            }
            break;
        case 'g':
            cmp = strncmp(cmd, CMD_GET, sizeof(CMD_GET) - 1);
            if ( cmp == 0 )
            {
                if ( tcpClient != NULL )
                {
					result = TCPClient_Connect(tcpClient, IP_ADDR_SERVER, IP_PORT_SERVER);
                    key = strchr(cmd, ' ');
                    *key = '\0';
                    key++;
                    ncData[0] = NCData_Create(strlen(key), key);
                    ncp = NCProtocol_Create(
								CMD_GET_ID,
								1,
								ncData);
                }
                else
                {
                    printf("先与服务器建立连接!\n");
                }
            }
            else
            {
                printf("没有此命令： \"%s\"\n", cmd);
            }
            break;
        case 'd':
            cmp = strncmp(cmd, CMD_DEL, sizeof(CMD_DEL) - 1);
            if ( cmp == 0 )
            {
                if ( cmp == 0 && tcpClient != NULL )
                {
					result = TCPClient_Connect(tcpClient, IP_ADDR_SERVER, IP_PORT_SERVER);
                    key = strchr(cmd, ' ');
                    *key = '\0';
                    key++;
                    
                    ncData[0] = NCData_Create(strlen(key), key);
                    
                    ncp = NCProtocol_Create(
								CMD_DEL_ID,
								1,
								ncData);
                }
                else
                {
                    printf("先与服务器建立连接!\n");
                }
            }
            else
            {
                printf("没有此命令： \"%s\"\n", cmd);
            }
            break;
        default:
            printf("没有此命令： \"%s\"\n", cmd);
            break;
        }
        fflush( stdin );
        if (tcpClient == NULL)
        {
            printf("nocommit>>");
        }
        else
        {
			TCPClient_Send(tcpClient,
						NCProtocol_Encapsul(ncp),
						ncp->totalLength);
			mlen = TCPClient_Receive(tcpClient,
						buffer,
						sizeof(buffer));
			//NCProtocol_Dispose(ncp);
			buffer[mlen] = '\0';
			printf("服务器：%s\n", buffer);
            printf("%s::%s>>", tcpClient->IPAddress, dbFile);
        }
        gets(cmd);
    }

    return 0;
}
