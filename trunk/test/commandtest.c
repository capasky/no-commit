#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

int main()
{
	char buf[64];
	Command * cmd;
	while ( gets(buf) != NULL )
	{
		cmd = Command_TryParse(buf);
		if ( cmd != NULL )
		{
			printf("CommandID: %d\nCommandText: %s\nKey: %s\nValue: %s\n",
					cmd->CommandID,
					cmd->CommandText,
					cmd->Key,
					cmd->Value
					);
		}
		else
		{
			printf("命令非法!\n");
		}
		
	}
	return 0;
}
