
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
static void httpProcess(void * pvParameters);


int main(void)
{
	/* Setup the Hardware */
	prvSetupHardware();

	xTaskCreate(httpProcess,
			(signed portCHAR *)"httpProcess",
			128,
			NULL,
			3,
			NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Never reach here */
	return 0;
}

void testModem(char * cmd, uint8_t _cr)
{
		uint8_t i = 0;
		uint8_t crCount = 0;
		char c = '\0';
		char * buff;

		uart_print(3, cmd);
		vTaskDelay(300);

		buff = (uint8_t *)pvPortMalloc(uart3.num_bytes);
		while(uart3.num_bytes > 0)
		{
			buff[i] = uart_getbyte();
			i++;		
		}
		buff[i + 1] = NULL;

		uart_print(0, buff);

		if(uart3.rx_ovf)
		{
			uart3.rx_ovf = 0;
			uart_print(0, "OVERFLOW\r\n");
		}

		vPortFree(buff);
}

static void httpProcess(void * pvParameters)
{
	uart_print(0, "http started\r\n");
	for(;;)
	{
		testModem("AT\r",2);
		vTaskDelay(2000);
		testModem("AT+CSQ\r",2);
		vTaskDelay(2000);
		testModem("AT+CREG?\r",2);
		vTaskDelay(2000);
		testModem("AT+CGATT?\r",2);
		vTaskDelay(2000);
		testModem("AT+CGMI\r",2);
		vTaskDelay(2000);
		testModem("AT+CGMM\r",2);
		vTaskDelay(2000);
		testModem("AT+CGMR\r",2);
		vTaskDelay(2000);
		testModem("AT+CGSN\r",2);
		vTaskDelay(2000);
		// testModem("ATI\r",2);
		// vTaskDelay(2000);
		testModem("AT+GSV\r",2);
		vTaskDelay(2000);
	}
}

void prvSetupHardware( void )
{
	/* Initialte Clock */
	SystemInit();
	SystemCoreClockUpdate();
	uart_init(0, 115200);				// Debug port
	uart_init(3, 115200);				// Modem
	uart3.i_first = 0;			
 	uart3.i_last = 0;				
 	uart3.rx_ovf = 0;
 	uart3.fifo_full = 0;
 	uart3.num_bytes = 0;
 	uart3.rx_not_empty = 0;
 	uart3.rx_fifo = (uint8_t *) pvPortMalloc(_FIFO_SIZE_);
	uart_print(0, "System started\r\n");
}