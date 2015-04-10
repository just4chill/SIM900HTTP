
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

char test_buff[] = "0,1,2,3,4,5";

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
	char * rx_buff;
	for(;;)
	{
		if(flag == 0)
		{
			flag = 1;
			// uart_print(0, strtok(test_buff, ","));
			// uart_print(0, strtok(NULL, ","));
			// uart_print(0, strtok(NULL, ","));
			// uart_print(0, strtok(NULL, ","));
			// uart_print(0, strtok(NULL, ","));
						// gsm_tokenize_response(	test_buff,
						// 						"\r",
						// 						"+HTTPACTION: ",
						// 						",",
						// 						3);
			// if(gsm_send_check_resp("AT\r", "OK"))
			// {
			// 	uart_print(0, "Got Rep for AT\r\n");
			// 	if(gsm_send_check_resp("AT+CREG?\r", "+CREG: 0,1"))
			// 	{
			// 		uart_print(0, "Got Rep for CREG\r\n");
			// 		uart_append_crlf(0);
			// 		uart3_rx_head = 0;
			// 		uart_print(3, "AT+CSQ\r");
			// 		_delay_ms(200);
			// 		if(uart3_rx_head > 0)
			// 		{
			// 			uart_print(0, "Head is > 0\r\n");
			// 			// rx_buff = (char *) pvPortMalloc(uart3_rx_head);
			// 			// bzero(rx_buff,uart3_rx_head);
			// 			// memcpy(rx_buff, uart3_rx_fifo, uart3_rx_head);
			// 			gsm_allocate_mem_for_tokens(2,10);
			// 			gsm_tokenize_response(	uart3_rx_fifo,
			// 									"\r",
			// 									"+CSQ: ",
			// 									",",
			// 									2);
			// 			uart_print(0, "TOKENS ARE:\r\n");
			// 			uart_print(0, respTokens[0]);
			// 			uart_append_crlf(0);
			// 			uart_print(0, respTokens[1]);
			// 			uart_append_crlf(0);
			// 			char buff[20];

			// 			//char * end;
  	// 					int rssi =  (((* (respTokens[0])) - 48) * 10) +
  	// 								(((* (respTokens[0]+1)) - 48));
  	// 					// if (!* end)
   //  				// 		printf(0, "Converted successfully\r\n");
  	// 					// else
   //  				// 		uart_print(0, "Conversion error\r\n");

			// 			//strcpy(buff, respTokens[0]);
			// 			// uint8_t rssi = atoi(buff);
			// 			// char * ptr;
			// 			// uint16_t rssi = stringToInt(respTokens[0]);
			// 			sprintf(buff, "RSSI: %d",rssi);
			// 			uart_append_crlf(0);
			// 			uart_print(0, buff);
			// 			if(gsm_send_check_resp("AT\r", "OK"))
			// 			{
			// 			uart_print(0, "AT OK\r\n");
			// 			if(gsm_send_check_resp("AT+CSTT=\"TATA.DOCOMO.INTERNET\"\r", "OK"))
			// 			{
			// 				uart_print(0, "APN Set Success");
			// 				uart_append_crlf(0);
			// 				_delay_ms(100);
			// 				if(gsm_send_check_resp("AT+CIICR\r", "OK"))
			// 				{
			// 					uart_print(0, "DC Success\r\n");
			// 					_delay_ms(300);
			// 					//uart3_rx_head = 0;	
			// 					uart_append_crlf(0);
			// 					uart_append_crlf(0);	
			// 					// uart_print(3, "AT+CIPSHUT\r");	
			// 					// _delay_ms(100);
			// 					// uart3_rx_head = 0;
			// 					// uart_print(3, "AT+CIPSTATUS\r");
			// 					// _delay_ms(200);
			// 					// if(uart3_rx_head > 0)
			// 					// {
			// 					// 	uart_puts(0, uart3_rx_fifo, uart3_rx_head);
			// 					// }
			// 						// gsm_allocate_mem_for_tokens(2,20);
			// 						// gsm_tokenize_response(	uart3_rx_fifo,
			// 						// 						"\r",
			// 						// 						" ",
			// 						// 						",",
			// 						// 						4);
			// 						// uart_print(0, "TOKENS ARE:\r\n");
			// 						// uart_print(0, respTokens[0]);
			// 						// uart_append_crlf(0);
			// 						// uart_print(0, respTokens[1]);
			// 						// uart_append_crlf(0);
			// 					uart3_rx_head = 0;
			// 					uart_print(3, "AT+CIFSR\r");
			// 					_delay_ms(300);
			// 					if(uart3_rx_head > 0)
			// 					{
			// 						uart_puts(0, uart3_rx_fifo, uart3_rx_head);
			// 					}
			// 					uart_append_crlf(0);
			// 					uart_append_crlf(0);
			// 					if(strstr(uart3_rx_fifo, "ERROR") == NULL)
			// 					{
			// 						uart_print(0, "no error");
			// 					if(gsm_send_check_resp("AT+SAPBR=3,1,\"APN\",\"TATA.DOCOMO.INTERNET\"\r", 
			// 						"OK"))
			// 					{
			// 						uart_print(0, "ab Success");
			// 						uart_append_crlf(0);
			// 						if(gsm_send_check_resp("AT+SAPBR=1,1\r", "OK"))
			// 						{
			// 							uart_print(0, "profile Success");
			// 							uart_append_crlf(0);
			// 							if(gsm_send_check_resp("AT+HTTPINIT\r", "OK"))
			// 							{
			// 								uart_print(0, "session create");
			// 								uart_append_crlf(0);
			// 								if(gsm_send_check_resp("AT+HTTPPARA=\"URL\",\"http://api.openweathermap.org/data/2.5/weather?q=Bengaluru,in\"\r", 
			// 									"OK"))
			// 								{
			// 									uart3_rx_head = 0;
			// 									uart_print(0, "Para success\r\n");
			// 									uart_print(3, "AT+HTTPACTION=0\r");
			// 									_delay_ms(1000);
			// 									if(uart3_rx_head > 0)
			// 									{
			// 										uart_puts(0, uart3_rx_fifo, uart3_rx_head);
			// 										uart_append_crlf(0);
			// 									}
			// 									// gsm_allocate_mem_for_tokens(4,20);
			// 									// gsm_tokenize_response(	uart3_rx_fifo,
			// 									// 						"\r",
			// 									// 						"+HTTPACTION: ",
			// 									// 						",",
			// 									// 						4);
			// 									// uart_print(0, "TOKENS ARE:\r\n");
			// 									// uart_print(0, respTokens[0]);
			// 									// uart_append_crlf(0);
			// 									// uart_print(0, respTokens[1]);
			// 									// uart_append_crlf(0);
			// 									if(gsm_send_check_resp("AT+HTTPTERM\r", "OK"))
			// 									{
			// 										uart_print(0, "session term");
			// 										uart_append_crlf(0);
			// 									}										
			// 								}

			// 							}
			// 						}
			// 					}
			// 				}
			// 				}
			// 			}
			// 		}
			// 			// uart_print(0, respTokens[1]);
			// 			//gsm_free_mem_for_tokens(2);
			// 		}
					
			// 	}
			// }
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