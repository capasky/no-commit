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
 * @author			@date				@version				@par
 * capasky			2012.12.01			1.0.0.1					创建
 * capasky			2012.12.19			1.0.1.0					更新N多接口实现
 * capasky			2012.12.19			1.0.1.1					修复String_Substring的BUG
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "stringutils.h"

/**
 * String_Compare 比较两个指定的 string 对象（其中忽略或考虑其大小写），并返回一个整数，指示二者在排序顺序中的相对位置。
 * @param string1
 * @param string2
 * @param ignoreCase
 * @return
 */
int		String_Compare(string string1, string string2, bool ignoreCase)
{
	if (ignoreCase)
	{
		return strcasecmp(string1, string2);
	}
	else
	{
		return strcmp(string1, string2);
	}
}

/**
 * String_CompareOrdinal 通过计算每个字符串中相应 Char 对象的数值来比较两个指定的 string 对象。
 * @param string1
 * @param string2
 * @return
 */
int		String_CompareOrdinal(string string1, string string2)
{

}

/**
 * String_Concat 连接 string 的两个指定实例。
 * @param string1
 * @param string2
 * @return
 */
string	String_Concat(string string1, string string2)
{

}

/**
 * 	 返回一个值，该值指示指定的 string 对象是否出现在此字符串中。
 * @param source
 * @param token
 * @return
 */
int	String_Contains(string source, string token)
{
	return 7;
}

/**
 * String_Copy 创建一个与指定的 string 具有相同值的 string 的新实例。
 * @param source 源字符串
 * @return 复制到的字符串
 */
string	String_Copy(string source)
{

}

/**
 * String_EndsWith 确定此字符串实例的结尾是否与指定的字符串匹配。
 * @param value 要与此实例末尾的子字符串进行比较的字符串。
 * @return
 */
bool	String_EndsWith(string source, string value)
{

}

/**
 * 确定此实例是否与另一个指定的 string 对象具有相同的值。
 * @param source
 * @return
 */
bool	String_Equals(string source)
{

}

/**
 * String_Format 将指定字符串中的格式项替换为指定数组中相应对象的字符串表示形式。
 * @param format
 * @param args
 * @return 
 */
string	String_Format(string format, string* args)
{

}
/**
 * String_IndexOf 报告指定字符串在此实例中的第一个匹配项的从零开始的索引。
 * @param source
 * @param value
 */
int		String_IndexOf(string source, string value)
{

}

/**
 *  返回指定字符串在这种情况下插入在指定索引位置的新字符串。
 * @param source
 * @param index
 * @param value
 * @return
 */
string	String_InsertAt(string source, int index, char value)
{

}

/**
 *  返回指定字符串在这种情况下插入在指定索引位置的新字符串。
 * @param source
 * @param index
 * @param value
 * @return
 */
string String_Insert(string source, int index, string value)
{

}

/**
 * 	 串联字符串数组的所有元素，其中在每个元素之间使用指定的分隔符。
 * @param sa
 * @param split
 * @return
 */
string	String_Join(string* sa, string split)
{

}

/**
 *  报告指定字符串在此实例中的最后一个匹配项的从零开始的索引位置。
 * @param source
 * @param value
 * @return
 */
int		String_LastIndexOf(string source, string value)
{

}

/**
 *  返回当前实例中从指定位置到最后位置的所有以删除的字符的新字符串。
 * @param source
 * @param index
 * @return
 */
string	String_RemoveAt(string source, int index)
{

}

/**
 *  返回指定数量字符在当前这个实例起始点在已删除的指定的位置的新字符串。
 * @param source
 * @param start
 * @param end
 * @return
 */
string	String_Remove(string source, int start, int end)
{

}

/**
 * String_Replace 返回一个新字符串，其中当前实例中出现的所有指定字符串都替换为另一个指定的字符串。
 * @param source 源字符串
 * @param oldValue 要被替换的字符串。
 * @param newValue 要替换出现的所有 oldValue 的字符串。
 * @return 等效于当前字符串（除了 oldValue 的所有实例都已替换为 newValue 外）的字符串。
 */
string String_Replace(string source, string oldValue, string newValue)
{

}

/**
 * 	 返回的字符串数组包含此实例中的子字符串（由指定 Unicode 字符数组的元素分隔）。
 * @param source
 * @param token
 * @return
 */
string*	String_Split(string source, string token, int * count)
{
	int i;
	char * src,
		 * tmp;
	string * splited;
	*count = String_Contains(source, token) + 1;
	splited = (string *) malloc ( sizeof(string) * (*count));
	src = strdup(source);
	tmp = strtok(src, token);
	if (tmp == NULL)
	{
		free(splited);
		splited = NULL;
		*count = 0;
		return splited;
	}
	splited[0] = tmp;
	
	for (i = 1; i < (*count); i++)
	{
		tmp = strtok(NULL,  token);
		if (tmp != NULL)
		{
			splited[i] = tmp;
		}
		else
		{
			break;
		}
	}
	if (i < *count)
	{
		*count = i;
	}
	
	return splited;
}

/**
 * 确定此字符串实例的开头是否与指定的字符串匹配。
 * @param source
 * @param value
 * @return
 */
bool	String_StartsWith(string source, string value)
{

}

/**
 * 	 从此实例检索子字符串。 子字符串从指定的字符位置开始且具有指定的长度。
 * @param source
 * @param start
 * @param end
 * @return
 */
string	String_Substring(string source, int start, int end)
{
	string result = NULL;
	int srcLen = strlen(source);
	if ( srcLen == 0 || source == NULL ||
		start < 1 || end > srcLen ||
		start > end)
	{
		return NULL;
	}
	result = strdup( &(source[start - 1]) );
	result[end - start + 1] = 0;
	
	return result;
}

/**
 * 返回此字符串转换为小写形式的副本。
 * @param source
 * @return
 */
string	String_ToLower(string source)
{

}

/**
 * 	 返回此字符串转换为大写形式的副本。
 * @param source
 * @return
 */
string	String_ToUpper(string source)
{

}

/**
 * 	 从当前 string 对象移除所有前导空白字符和尾部空白字符。
 * @param source
 * @return
 */
string	String_Trim(string source)
{

}

/**
 * 	 从当前 string 对象移除数组中指定的一组字符的所有尾部匹配项。
 * @param source
 * @return
 */
string	String_TrimEnd(string source)
{

}

/**
 * 	 从当前 string 对象移除数组中指定的一组字符的所有前导匹配项。
 * @param source
 * @return
 */
string	String_TrimStart(string source)
{

}
