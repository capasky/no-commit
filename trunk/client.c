/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               main
 * Comments:             This file include the entry point of client
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
 * capasky				2012.12.26			1.1.0.0
 */
#include <stdlib.h>
#include <string.h>

#include "ncclient.h"

NCClient * NCC = NULL;

int main(int argc, char ** argv)
{
	NCC = NCClient_Create();
	if (argc > 1)
	{
		strcpy(NCC->UpdateServerIP, argv[1]);
	}
	if (argc > 2)
	{
		NCC->UpdateServerPort = atoi(argv[2]);
	}
	NCClient_Run(NCC);
	return 0;
}
