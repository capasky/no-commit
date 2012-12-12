/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.utils.stringutils
 * Comments:             This file include the implementation of 
 *						 utilities for string interfaces
 * Author:               capasky
 * Create Date:          2012.12.01
 * Version:              1.0
 * Plateform:            Any
 **********************************************************************/

/**
 * Revision Log:
 * @author				@date				@version
 * capasky				2012.12.01			1.0.0.1
 */

#include <stdlib.h>
#include <string.h>
#include "stringutils.h"

/**
 * String_Format 
 * @param format the format string
 * @param params the parameters of formating the orgin string
 * @param count the count of parameters in params
 * @return the formated string
 */
char * String_Format(char * format, char ** params, int count)
{
	/* TODO: Not Implement */
	return NULL;
}

/**
 * String_Split
 * @param source 
 * @param splitString
 */
SplitedString String_Split(char * source, char * splitString)
{
	int cnt = 0, i;
	struct sSplitedString ss;
	ss.count = String_Contains(source, splitString) + 1;
	cnt = ss.count;
	*(ss.splited) = (char *)malloc( ss.count);
	ss.splited[0] = strdup( strtok(source, splitString) );
	for (i = 1; i < cnt; i++)
	{
		ss.splited[i] = strdup( strtok(NULL, splitString) );
	}
	return ss;	
}

/**
 * String_Split_Delete
 * @param ss
 * @return void
 */
void String_Split_Delete(SplitedString ss)
{
	int i;
	if (ss.count == 0)
	{
		return;
	}
	for (i = 0; i < ss.count; i++)
	{
		free(ss.splited[i]);
	}
}

/**
 * String_Contains
 * @param source
 * @param pattern
 * @return the number of matched pattern
 */
int String_Contains(char * source, char * pattern)
{
	/* TODO: Not Implement */
	return 6;
}

