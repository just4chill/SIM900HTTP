
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
			128,
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
	if(gsm_check_comm())
	{
		test_level++;
		if(gsm_check_sim())
		{
			test_level++;
			rssi = gsm_read_rssi();
			if(rssi > 0)
			{
				test_level++;
				if(gsm_set_apn(APN))
				{
					test_level++;
					if(gsm_wireless_up())
					{
						test_level++;
						if(gsm_set_sapbr(APN))
						{
							test_level++;
							if(gsm_up_sapbr())
							{
								test_level++;
							}
						}
					}
				}
			}
		}
	}

	if(test_level == exp_level)
	{
		// All tests are passed
		// Continue task
		gsm_read_tcpstate();
		gsm_read_tipaddress();

		uart_print(0, "TCP State: ");
		uart_print(0, tcp_state);
		uart_append_crlf(0);
		uart_print(0, "IP: ");
		uart_print(0, ip_address);
		uart_append_crlf(0);
	}
	else
	{
		// Display error and kill the task
	}

	for(;;)
	{
		if(gsm_http_init())
		{
			if(gsm_http_para(get_url))
			{
				if(gsm_http_get())
				{
					if(gsm_http_read())
					{
						if(gsm_http_term())
						{
							// Term success
							uart_print(0, "RESP: ");
							uart_print(0, http_resp_code);
							uart_append_crlf(0);
							uart_print(0, "DATA: ");
							uart_print(0, http_read_buff);							
						}
					}
				}
			}
		}
		vTaskDelay(500);
	}
}

void prvSetupHardware( void )
{
	/* Initialte Clock */
	SystemInit();
	SystemCoreClockUpdate();
	uart_init(0, 115200);			// Debug port
	uart_init(1, 9600);				// Modem
	uart_print(0, "System started\r\n");
}