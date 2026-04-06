#include <stdio.h>

int main(void)
{
	char source[] = "Hello, World! This is a test string for my_strcpy function.\n";
	char destination[101];

	char *ps = source;
	char *pd = destination;
	while ((*pd++ = *ps++) != '\0')
		;

	printf("拷贝后的字符串: %s", destination);

	return 0;
}
