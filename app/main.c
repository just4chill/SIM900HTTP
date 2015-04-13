
#include "LPC17xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include "system_LPC17xx.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void 		prvSetupHardware( void );
static void sendToModem(void * pvParameters);
static void respFromModem(void * pvParameters);

int main(void)
{
	/* Setup the Hardware */
	prvSetupHardware();

	xTaskCreate(sendToModem,
			(signed portCHAR *)"task1",
			128,
			NULL,
			3,
			NULL);

	xTaskCreate(respFromModem,
			(signed portCHAR *)"task2",
			128,
			NULL,
			4,
			NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Never reach here */
	return 0;
}


static void sendToModem(void * pvParameters)
{
	char * gsm_buff;
	uart_print(0, "Modem Task1\r\n");
	gsm_buff = (char *) pvPortMalloc(128);
	for(;;)
	{
		uart_print(3, "AT\r");
		uint8_t cr_count = 0;
		uint8_t i = 0;
		do
		{
			if(cr_count == 2)
			{
				break;
			}
			char c = uart_getc(3);
			if(c == '\r')
			{
				cr_count++;
			}
			if(c != '\n')
			{
				gsm_buff[i++] = c;
			}
		}
		while(uart3.num_bytes > 0);
		uart_puts(0, gsm_buff, i);
		uart_append_crlf(0);
		vTaskDelay(1000);
	}
}

static void respFromModem(void * pvParameters)
{
	uart_print(0, "Modem Task2\r\n");
	for(;;)
	{
		//uart_print(0, "Task2\r\n");
		vTaskDelay(5000);
	}
}


void prvSetupHardware( void )
{

	/* Initialte Clock */
	SystemInit();
	SystemCoreClockUpdate();
	uart_init(0, 115200);				// Debug port
	uart_init(3, 115200);				// Modem
	uart_init_fifo(3);
	uart_print(0, "System started\r\n");
}