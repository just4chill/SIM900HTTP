
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "gsm.h"
#include "FreeRTOS.h"
#include "task.h"

void _gsm_print(char * str)
{
	for(uint8_t i = 0;str[i] != '\0';i++)
	{
		uart_puts(3, &str[i], 1);
	}
}

int8_t gsm_send_check_resp(
				char * cmd,
				char * resp,
				uint8_t * resp_store,
				uint16_t timeout,
				uint8_t crcount
		){

	bzero(resp_store, 512);
	_gsm_print(cmd);
	vTaskDelay(timeout);
	uint8_t i = 0;
	do
	{
		char c = uart_getc(3);
		resp_store[i] = (uint8_t)c;
		i++;
	}
	while(uart3.num_bytes > 0);
	_debug_print(resp_store);

	char * str = strstr(resp_store, resp);
	if(str != NULL)
	{
		_debug_print("Found\r\n");
		_debug_print("\r\n");
		uart_puts(0, &str[0],1);
		_debug_print("\r\n");
		return 1;
	}
	else
	{
		_debug_print("NotFound\r\n");
		return -1;
	}

	/* Never execute */
	return 0;
}
