#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

//char   str[]  = "\r+CSQ: 19,0\r";

char   str[]  = "\r+HTTPACTION: 0,200,400\r";
// char   str[]  = "\r+HTTPREAD: 19,0\r";

char * tokens[10];

void make_tokens(	char * raw			, char * start_token, 
					char * sec_token	, char * delim	,
					uint8_t len
				)
	{
		uint8_t i;
		char * tok[10];
		// Filtered raw
		char * fraw = strtok(raw, start_token);
		// Info
		char * info = strstr(fraw, sec_token) + strlen(sec_token);

		// Extract first token
		tok[0] = strtok(info, delim);
		strcpy(tokens[0], tok[0]);
		// Extract remaining
		for(i = 1;i < len;i++)
		{
			tok[i] = strtok(NULL, delim);
			strcpy(tokens[i], tok[i]);
		}

	}

void

int main(void)
{
	uint8_t i;

	for(i = 0;i <= 3;i++)
	{
		tokens[i] = (char *)malloc(15);
		//tok[i] = (char *)malloc(15);
	}

	make_tokens(	str, 
					"\r",
					": ", 
					",", 
					2
				);

	for(i = 0;i < 2;i++)
	{
		printf("%s\n", tokens[i]);
	}

	for(i = 0;i <= 3;i++)
	{
		free(tokens[i]);
		//free(tok[i]);
	}

	return 0;
}