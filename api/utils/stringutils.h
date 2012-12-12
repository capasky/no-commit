/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.utils.stringutils
 * Comments:             This file include the definition of utilities 
 * 						 for string interfaces
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
 
#ifndef STRINGUTILS_H_INCLUDED
#define STRINGUTILS_H_INCLUDED

typedef struct sSplitedString 
{
	int count;
	char ** splited;
} SplitedString;

/**
 * String_Format 
 * @param format the format string
 * @param params the parameters of formating the orgin string
 * @param count the count of parameters in params
 * @return the formated string
 */
char * String_Format(char * format, char ** params, int count);

/**
 * String_Split
 * @param source 
 * @param splitString
 * @return 
 */
SplitedString String_Split(char * source, char * splitString);

/**
 * String_Split_Delete
 * @param ss
 * @return void
 */
void String_Split_Delete(SplitedString ss);

/**
 * String_Contains
 * @param source
 * @param pattern
 * @return the number of matched pattern
 */
int String_Contains(char * source, char * pattern);

#endif



