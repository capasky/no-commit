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

#include "../../base.h"
#include "stringutils.h"


/**
 * String_Create 使用制定字符数组创建字符串
 * @param source 源字符数组
 * @return 返回创建的字符串
 */
String 	String_Create(char * source)
{
	String s;
	s.Length = 0;
	s.data = NULL;
	if (source != NULL)
	{
		s.Length = strlen(source);
		s.data = strdup(source);
	}
	return s;
}

/**
 * String_Compare 比较两个指定的 String 对象（其中忽略或考虑其大小写），并返回一个整数，指示二者在排序顺序中的相对位置。
 * @param string1
 * @param string2
 * @param ignoreCase
 * @return
 */
int		String_Compare(String string1, String string2, bool ignoreCase)
{

}

/**
 * String_CompareOrdinal 通过计算每个字符串中相应 Char 对象的数值来比较两个指定的 String 对象。
 * @param string1
 * @param string2
 * @return
 */
int		String_CompareOrdinal(String string1, String string2)
{

}

/**
 * String_Concat 连接 String 的两个指定实例。
 * @param string1
 * @param string2
 * @return
 */
String	String_Concat(String string1, String string2)
{

}

/**
 * 	 返回一个值，该值指示指定的 String 对象是否出现在此字符串中。
 * @param source
 * @param token
 * @return
 */
int	String_Contains(String source, String token)
{
	return 7;
}

/**
 * String_Copy 创建一个与指定的 String 具有相同值的 String 的新实例。
 * @param source 源字符串
 * @return 复制到的字符串
 */
String	String_Copy(String source)
{

}

/**
 * String_EndsWith 确定此字符串实例的结尾是否与指定的字符串匹配。
 * @param value 要与此实例末尾的子字符串进行比较的字符串。
 * @return
 */
bool	String_EndsWith(String source, String value)
{

}

/**
 * 确定此实例是否与另一个指定的 String 对象具有相同的值。
 * @param source
 * @return
 */
bool	String_Equals(String source)
{

}

/**
 * String_Format 将指定字符串中的格式项替换为指定数组中相应对象的字符串表示形式。
 * @param format
 * @param args
 * @return 
 */
String	String_Format(String format, StringArrry args)
{

}
/**
 * String_IndexOf 报告指定字符串在此实例中的第一个匹配项的从零开始的索引。
 * @param source
 * @param value
 */
int		String_IndexOf(String source, String value)
{

}

/**
 *  返回指定字符串在这种情况下插入在指定索引位置的新字符串。
 * @param source
 * @param index
 * @param value
 * @return
 */
String	String_InsertAt(String source, int index, char value)
{

}

/**
 *  返回指定字符串在这种情况下插入在指定索引位置的新字符串。
 * @param source
 * @param index
 * @param value
 * @return
 */
String String_Insert(String source, int index, String value)
{

}

/**
 * 	 串联字符串数组的所有元素，其中在每个元素之间使用指定的分隔符。
 * @param sa
 * @param split
 * @return
 */
String	String_Join(StringArrry sa, String split)
{

}

/**
 *  报告指定字符串在此实例中的最后一个匹配项的从零开始的索引位置。
 * @param source
 * @param value
 * @return
 */
int		String_LastIndexOf(String source, String value)
{

}

/**
 *  返回当前实例中从指定位置到最后位置的所有以删除的字符的新字符串。
 * @param source
 * @param index
 * @return
 */
String	String_RemoveAt(String source, int index)
{

}

/**
 *  返回指定数量字符在当前这个实例起始点在已删除的指定的位置的新字符串。
 * @param source
 * @param start
 * @param end
 * @return
 */
String	String_Remove(String source, int start, int end)
{

}

/**
 * String_Replace 返回一个新字符串，其中当前实例中出现的所有指定字符串都替换为另一个指定的字符串。
 * @param source 源字符串
 * @param oldValue 要被替换的字符串。
 * @param newValue 要替换出现的所有 oldValue 的字符串。
 * @return 等效于当前字符串（除了 oldValue 的所有实例都已替换为 newValue 外）的字符串。
 */
String String_Replace(String source, String oldValue, String newValue)
{

}

/**
 * 	 返回的字符串数组包含此实例中的子字符串（由指定 Unicode 字符数组的元素分隔）。
 * @param source
 * @param token
 * @return
 */
StringArrry	String_Split(String source, String token)
{
	int i;
	char * src,
		 * tmp;
	StringArrry sa;
	sa.Count = String_Contains(source, token) + 1;
	sa.Splited = (String *) malloc ( sizeof(struct sString) * sa.Count);
	src = strdup(source.data);
	tmp = strtok(src, token.data);
	if (tmp == NULL)
	{
		free(sa.Splited);
		sa.Splited = NULL;
		sa.Count = 0;
		return sa;
	}
	sa.Splited[0] = String_Create( tmp );
	
	for (i = 1; i < sa.Count; i++)
	{
		tmp = strtok(NULL,  token.data);
		if (tmp != NULL)
		{
			sa.Splited[i] = String_Create( tmp );
		}
		else
		{
			break;
		}
	}
	if (i < sa.Count)
	{
		sa.Count = i;
	}
	
	return sa;
}

/**
 * 确定此字符串实例的开头是否与指定的字符串匹配。
 * @param source
 * @param value
 * @return
 */
bool	String_StartsWith(String source, String value)
{

}

/**
 * 	 从此实例检索子字符串。 子字符串从指定的字符位置开始且具有指定的长度。
 * @param source
 * @param start
 * @param end
 * @return
 */
String	String_Substring(String source, int start, int end)
{
	String result;
	if (source.Length == 0 || source.data == NULL ||
		start < 1 || end > source.Length ||
		start > end)
	{
		return String_Create("");
	}
	result = String_Create( &(source.data[start - 1]) );
	result.data[end - start + 1] = 0;
	
	if (result.data == NULL)
	{
		printf("NULL\n");
	}
	
	return result;
}

/**
 * 返回此字符串转换为小写形式的副本。
 * @param source
 * @return
 */
String	String_ToLower(String source)
{

}

/**
 * 	 返回此字符串转换为大写形式的副本。
 * @param source
 * @return
 */
String	String_ToUpper(String source)
{

}

/**
 * 	 从当前 String 对象移除所有前导空白字符和尾部空白字符。
 * @param source
 * @return
 */
String	String_Trim(String source)
{

}

/**
 * 	 从当前 String 对象移除数组中指定的一组字符的所有尾部匹配项。
 * @param source
 * @return
 */
String	String_TrimEnd(String source)
{

}

/**
 * 	 从当前 String 对象移除数组中指定的一组字符的所有前导匹配项。
 * @param source
 * @return
 */
String	String_TrimStart(String source)
{

}
