
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

static TaskHandle_t task1 = NULL, task2 = NULL;

int main(void)
{
	/* Setup the Hardware */
	prvSetupHardware();

	xTaskCreate(sendToModem,
			(signed portCHAR *)"sendToModem",
			128,
			NULL,
			3,
			&task1);

	xTaskCreate(respFromModem,
			(signed portCHAR *)"respFromModem",
			128,
			NULL,
			4,
			&task2);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Never reach here */
	return 0;
}


static void sendToModem(void * pvParameters)
{
	uart_print(0, "Modem Task1\r\n");
	for(;;)
	{
	}
}

static void respFromModem(void * pvParameters)
{
	uart_print(0, "Modem Task2\r\n");
	for(;;)
	{
	}
}


void prvSetupHardware( void )
{
	/* Initialte Clock */
	SystemInit();
	SystemCoreClockUpdate();
	uart_init(0, 115200);				// Debug port
	uart_init(3, 115200);				// Modem
	setUpFIFO();
	uart_print(0, "System started\r\n");
}