
#include <stdio.h>
#include <string.h>

int main(void)
{
	char str[100] = "\r\nOK\r\n";
	char * key = strstr(str, "OK");
	printf("Pointer: %s\n", key);
	return 0;
}