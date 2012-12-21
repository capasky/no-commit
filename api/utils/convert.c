/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.utils.convert
 * Comments:             This file include the implementation of utilities 
 * 						 for convert interfaces
 * Author:               capasky
 * Create Date:          2012.12.01
 * Version:              1.0
 * Plateform:            Any
 **********************************************************************/ 

/**
 * Revision Log:
 * @author				@date				@version
 * capasky				2012.12.01			1.0.0.1
 * yellhb				2012.12.14			1.0.1.0
 */

#include <stdlib.h>
#include <stdbool.h>

#include "convert.h"

/**
 * IntToString convert an integer to string
 * @param num the orgin integer
 * @return the converted string if success, or NULL if fail
 */
char * Convert_IntToString(int num)
{	
	char 	*arr;
	int 	tmp, k, i;
	
	tmp = num;
	k = 1;
	while (( tmp /= 10 ) != 0 )
		k++;

	arr = ( char *) malloc ( sizeof ( char ) * ( k + 1 ));
	arr[0] = '0';

	i = 0;
	while ( num != 0 )
	{
		arr[k - i - 1] = num % 10 + 48;
		num /= 10;
		i++;
	}

	arr[k] = '\0';
	return arr;
}
