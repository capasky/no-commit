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
 * @author				@date				@version				@par
 * capasky				2012.12.01			1.0.0.1					
 * capasky				2012.12.16			1.1.0.0					
 */
 
#ifndef STRINGUTILS_H_INCLUDED
#define STRINGUTILS_H_INCLUDED

typedef struct sString
{
	int Length;
	char * data;
} String;

typedef struct sStringArrry 
{
	int count;
	String * splited;
} StringArrry;

/**
 * String_Create 使用制定字符数组创建字符串
 * @param source 源字符数组
 * @return 返回创建的字符串
 */
String 	String_Create(char * source);

/**
 * String_Compare 比较两个指定的 String 对象（其中忽略或考虑其大小写），并返回一个整数，指示二者在排序顺序中的相对位置。
 * @param string1
 * @param string2
 * @param ignoreCase
 * @return
 */
int		String_Compare(String string1, String string2, bool ignoreCase);

/**
 * String_CompareOrdinal 通过计算每个字符串中相应 Char 对象的数值来比较两个指定的 String 对象。
 * @param string1
 * @param string2
 * @return
 */
int		String_CompareOrdinal(String string1, String string2);

/**
 * String_Concat 连接 String 的两个指定实例。
 * @param string1
 * @param string2
 * @return
 */
String	String_Concat(String string1, String string2);

/**
 * 	 返回一个值，该值指示指定的 String 对象是否出现在此字符串中。
 * @param source
 * @return
 */
bool	String_Contains(String source)

/**
 * String_Copy 创建一个与指定的 String 具有相同值的 String 的新实例。
 * @param source 源字符串
 * @return 复制到的字符串
 */
String	String_Copy(String source)；

/**
 * String_CopyTo 将指定数目的字符从此实例中的指定位置复制到 Unicode 字符数组中的指定位置。
 * @param source 源字符串
 * @param sourceIndex
 * @param destination
 * @param destinationIndex
 * @param count
 * @return
 */
bool	String_CopyTo(String source, int sourceIndex, char * destination, int destinationIndex, int count);

/**
 * String_EndsWith 确定此字符串实例的结尾是否与指定的字符串匹配。
 * @param value 要与此实例末尾的子字符串进行比较的字符串。
 * @return
 */
bool	String_EndsWith(String source, String value);
	
bool	String_Equals(String);	 确定此实例是否与另一个指定的 String 对象具有相同的值。

String	String_Format(String, Object[]);	 将指定字符串中的格式项替换为指定数组中相应对象的字符串表示形式。

int		String_IndexOf(String)	 报告指定字符串在此实例中的第一个匹配项的从零开始的索引。

int		String_IndexOfAny(Char[], Int32);	 报告指定 Unicode 字符数组中的任意字符在此实例中第一个匹配项的从零开始的索引。 该搜索从指定字符位置开始。

String	String_Insert	 返回指定字符串在这种情况下插入在指定索引位置的新字符串。

String	String_Join(StringArrry sa, String split);	 串联字符串数组的所有元素，其中在每个元素之间使用指定的分隔符。

int		String_LastIndexOf(String);	 报告指定字符串在此实例中的最后一个匹配项的从零开始的索引位置。
	
int		String_LastIndexOfAny(Char[], Int32);	 报告在 Unicode 数组中指定的一个或多个字符在此实例中的最后一个匹配项的从零开始的索引位置。 在指定的字符位置开始和在向后的右边该字符串的开头处理的搜索。
	
String	String_PadLeft(Int32, Char)	 返回一个新字符串，该字符串通过在此实例中的字符左侧填充指定的 Unicode 字符来达到指定的总长度，从而使这些字符右对齐。
	
String	String_PadRight(Int32, Char)	 返回一个新字符串，该字符串通过在此字符串中的字符右侧填充指定的 Unicode 字符来达到指定的总长度，从而使这些字符左对齐。
	
String	String_RemoveAt(Int32)	 返回当前实例中从指定位置到最后位置的所有以删除的字符的新字符串。
	
String	String_Remove(Int32, Int32)	 返回指定数量字符在当前这个实例起始点在已删除的指定的位置的新字符串。

/**
 * String_Replace 返回一个新字符串，其中当前实例中出现的所有指定字符串都替换为另一个指定的字符串。
 * @param source 源字符串
 * @param oldValue 要被替换的字符串。
 * @param newValue 要替换出现的所有 oldValue 的字符串。
 * @return 等效于当前字符串（除了 oldValue 的所有实例都已替换为 newValue 外）的字符串。
 */
String 	String_Replace(String source, String oldValue， String newValue);

StringArrry	String_Split(char[])	 返回的字符串数组包含此实例中的子字符串（由指定 Unicode 字符数组的元素分隔）。
	
bool	String_StartsWith(String)	 确定此字符串实例的开头是否与指定的字符串匹配。

String	String_Substring(Int32, Int32)	 从此实例检索子字符串。 子字符串从指定的字符位置开始且具有指定的长度。
	
char *	String_ToCharArray()	 将此实例中的字符复制到 Unicode 字符数组。

char *	String_ToCharArray(Int32, Int32)	 将此实例中的指定子字符串内的字符复制到 Unicode 字符数组。
	
String	String_ToLower()	 返回此字符串转换为小写形式的副本。
	
String	String_ToUpper()	 返回此字符串转换为大写形式的副本。
	
String	String_Trim()	 从当前 String 对象移除所有前导空白字符和尾部空白字符。
	
String	String_TrimEnd	 从当前 String 对象移除数组中指定的一组字符的所有尾部匹配项。
	
String	String_TrimStart	 从当前 String 对象移除数组中指定的一组字符的所有前导匹配项。
	
#endif



