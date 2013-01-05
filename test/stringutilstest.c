

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringutils.h"

int main()
{
	int i;
	int count = 0;
	string str = "this is a  test  text! ddd";
	printf("%s\n", strstr(str, " "));
	printf("Parts:%d\n", String_Contains(str, " "));
	return 0;
	printf("Orgin:%s\nSub[1-12]:%s\n", str, String_Substring(str, 1, 12));
	string* sa = String_Split(
			str,
			" ",
			&count);
	
	printf("Split string _%s_ to %d parts:\n", str, count);
	for (i = 0; i < count; i++)
	{
		printf("%d:%s\n", i + 1, sa[i]);
	}
	return 0;
}
