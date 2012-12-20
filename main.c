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

#include "base.h"
#include "api/data/collection.h"

#define CMD_MAX_LEN     64
#define CMD_OPEN        "open"
#define CMD_CLOSE       "close"
#define CMD_GET         "get"
#define CMD_SET         "set"
#define CMD_DEL         "delete"
#define CMD_QUIT        "quit"
#define CMD_RE          "open|close|get|set|delete\s([a-z][A-Z])+\s\w+"

int main ( int argc, char **argv )
{
    char cmd[CMD_MAX_LEN];
    char * key,
         * value;
    int cmp = -1;
    Collection * wow = NULL;

    printf("nocommit>>");
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
                    wow = Collection_Create(key);
                    printf("OK\n");
                }
                else
                {
                    printf("File \"%s\" has opened already!\n", wow->dbFile);
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
