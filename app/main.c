
#include "LPC17xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include "system_LPC17xx.h"
#include "uart.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gsm.h"

#define APN "TATA.DOCOMO.INTERNET"

#define get_url "http://api.openweathermap.org/data/2.5/weather?q=bengaluru,in"

void 		prvSetupHardware( void );
static void httpProcess(void * pvParameters);

int main(void)
{
	/* Setup the Hardware */
	prvSetupHardware();

	xTaskCreate(httpProcess,
			(signed portCHAR *)"httpProcess",
			256,
			NULL,
			3,
			NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Never reach here */
	return 0;
}

static void httpProcess(void * pvParameters)
{
	uint8_t test_level = 0;
	uint8_t exp_level = 7;
	uint8_t rssi = 0;
	uart_print(0, "http started\r\n");
	char buff[20];
	uint8_t flag = 0;
	
	for(;;)
	{
		if(flag == 0)
		{
			flag = 1;
			uart_print(3, "AT\r");
			vTaskDelay(500);
			char * key1 = strstr(uart3_rx_fifo, "OK");
			if(key1)
			{
				uart_print(0,"Got response\r\n");
			}
			
			uart_print(3, "AT+CREG?\r");
			vTaskDelay(500);
			key1 = strstr(uart3_rx_fifo, "+CREG: 0,1");
			if(key1)
			{
				uart_print(0,"Network register\r\n");
			}

			uart_print(3, "AT+CSQ\r");
			vTaskDelay(500);
			key1 = strstr(uart3_rx_fifo, "+CSQ:");
			if(key1)
			{
				char * key2 = memchr(uart3_rx_fifo, ':',uart3_rx_head);
				char * token = strtok(uart3_rx_fifo, ":");
				sprintf(buff,"%s,%s,%s\r\n",key1,key2,token);
				uart_print(0, buff);
			}
		}
	}
}

void prvSetupHardware( void )
{
	/* Initialte Clock */
	SystemInit();
	SystemCoreClockUpdate();
	uart_init(0, 115200);			// Debug port
	uart_init(3, 9600);				// Modem
	uart_print(0, "System started\r\n");
}