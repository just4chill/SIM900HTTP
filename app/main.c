
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
	uart_print(0, "http started\r\n");
	uint8_t flag = 0;
	
	for(;;)
	{
		if(flag == 0)
		{
			flag = 1;
			if(gsm_send_check_resp("AT", "OK"))
			{
				uart_print(0, "Got Rep for AT\r\n");
				if(gsm_send_check_resp("AT+CREG?", "+CREG: 0,1"))
				{
					uart_print(0, "Got Rep for CREG\r\n");
				}
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