
#include <stdint.h>
#include "uart.h"
#include <string.h>
#include "gsm.h"
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "FreeRTOS.h"
#include "uart.h"

char * respTokens[50];

int8_t gsm_send_check_resp(	char * cmd,
								char * resp)
{
	uart3_rx_head = 0;
	uart_print(3, cmd);
	//uart_print(3, "\n");
	_delay_ms(75);

	if(uart3_rx_head > 0)
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
		respTokens[i] = (char *) pvPortMalloc(_size);
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
	uart_print(0, raw);
	char * fraw = strtok(raw, start_token);
	char * info = strtok(fraw, sec_token);

	uart_print(0, "--RAW--\r\n");
	uart_append_crlf(0);
	uart_print(0, fraw);
	uart_append_crlf(0);
	uart_print(0, info);
	uart_append_crlf(0);

	char * save_ptr;
	char tmp_buff[50];
	strcpy(tmp_buff, info);
	uart_print(0, "--INFO--\r\n");
	uart_print(0, tmp_buff);
	uart_append_crlf(0);
	char * savePtr;
	tempTokens[0] = strtok_r(info, delim, &savePtr);
	if(!tempTokens[0])
	{
		uart_print(0, "returning");
		return -1;
	}

	//char * ptr;
	strcpy(respTokens[0], tempTokens[0]);
	uart_print(0, "--TOKENS--\r\n");
	uart_print(0, respTokens[0]);
	// ptr = tempTokens[0] + strlen(tempTokens[0]);
	// uart_print(0, ptr);
	char * ptr;
	for(uint8_t i = 1;i <= len;i++)
	{
		if(i == 1)
			ptr = strstr(tmp_buff, delim);
		else
			ptr = strstr(ptr, delim);
		
		
		tempTokens[i] = strtok(ptr, delim);
		uart_print(0, tempTokens[i]);
		uart_append_crlf(0);
		strcpy(respTokens[i], tempTokens[i]);		
		// ptr = strtok(NULL, delim);
		// // ptr = tempTokens[i] + strlen(tempTokens[i]);
		// uart_print(0, ptr);
		// uart_append_crlf(0);
		//strcpy(respTokens[i], tempTokens[i]);
	}
	
	return 1;
}