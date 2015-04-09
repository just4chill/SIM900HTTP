
#include <stdint.h>
#include "uart.h"
#include <string.h>
#include "gsm.h"
#include <stdio.h>
#include "dealy.h"

char * respTokens[];

int8_t gsm_send_check_resp(	char * cmd,
								char * resp)
{
	uart_print(3, cmd);
	uart_print(3, "\r");
	_delay_ms(500);

	if(uart3_rx_fifo > 0)
	{
		if(strstr(uart3_rx_fifo, resp))
		{
			return 1;
		}
	}
	return -1;
}

void gsm_allocate_mem_for_tokens(uint8_t len, uint8_t _size)
{
	uint8_t i = 0;
	for(i = 0;i < len;i++)
	{
		respTokens[i] = (char *) malloc(_size);
	}
}

void gsm_free_mem_for_tokens(uint8_t len)
{
	uint8_t i = 0;
	for(i = 0;i < len;i++)
	{
		free(respTokens[i]);
	}
}

int8_t gsm_tokenize_response(	char * raw,
								char * start_token,
								char * sec_token,
								char * delim,
								uint8_t len
							)
{
	char * tempTokens[10];
	char * fraw = strtok(raw, start_token);
	char * info = strtok(fraw, sec_token);

	tempTokens[0] = strtok(info, delim);

	if(!tempTokens[0])
		return -1;

	strcpy(respTokens[0], tempTokens[0]);
	for(uint8_t i = 1;i < len;i++)
	{
		tempTokens[i] = strtok(NULL, delim);
		strcpy(respTokens[i], tempTokens[i]);
	}
	return 1;
}