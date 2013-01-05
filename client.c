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

#include "ncclient.h"
NCClient * NCC;
int main()
{
	NCC = NCClient_Create();
	NCClient_Run(NCC);
	return 0;
}
