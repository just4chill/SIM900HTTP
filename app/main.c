
#include "LPC17xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include "system_LPC17xx.h"
#include "uart.h"
#include "gsm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char ip_addr[20];

void 		prvSetupHardware( void );
static void sendToModem(void * pvParameters);
static void respFromModem(void * pvParameters);

void upModem(void);
void testHttp(void);

uint8_t * gsm_buff;

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
	_debug_print("Modem sendToModem\r\n");
	uint8_t test_flag = 0;
	gsm_buff = (uint8_t *) pvPortMalloc(512);
	for(;;)
	{
		if(test_flag == 0)
		{
			upModem();
			test_flag = 1;
		}
		testHttp();
		vTaskDelay(1000);
	}
}

static void respFromModem(void * pvParameters)
{
	_debug_print("Modem respFromModem\r\n");
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
	_debug_print("System started\r\n");
}

void testHttp(void)
{
	if(
		gsm_send_check_resp("AT+HTTPINIT\r",
						"OK",
						gsm_buff,
						300,
						5)
		){
		_debug_print("Finished init");
		uart_append_crlf(0);
		if(
			gsm_send_check_resp("AT+HTTPPARA=\"URL\",\"http://httpbin.org/ip\"\r",
							"OK",
							gsm_buff,
							300,
							5)
			){
			if(
				gsm_send_check_resp("AT+HTTPACTION=0\r",
								"OK",
								gsm_buff,
								10000,
								5)
				){	
				char http_status[10];
				// +HTTPACTION: 0,
				http_status[0] = gsm_buff[sizeof("+HTTPACTION: 0,") + 6];
				http_status[1] = gsm_buff[sizeof("+HTTPACTION: 0,") + 7];
				http_status[2] = gsm_buff[sizeof("+HTTPACTION: 0,") + 8];
				http_status[3] = '\0';
				_debug_print("Status: ");
				_debug_print(http_status);
				if(
					gsm_send_check_resp("AT+HTTPREAD\r",
									"OK",
									gsm_buff,
									2000,
									5)
					){	
					_debug_print("Finished reading - ");
					_debug_print(gsm_buff);
					uart_append_crlf(0);
					if(
					gsm_send_check_resp("AT+HTTPTERM\r",
									"OK",
									gsm_buff,
									1000,
									5)
					){
						_debug_print("Finished terminating");
						uart_append_crlf(0);
					}
				}					
			}
		}
	}
}

void upModem(void)
{
	char buff[20];
	// Check AT OK
	_debug_print("Sending AT");
	uart_append_crlf(0);
	if(
		gsm_send_check_resp("AT\r",
						"OK",
						gsm_buff,
						100,
						5)
		){
		uart_append_crlf(0);
		_debug_print("AT OK");
		uart_append_crlf(0);

		vTaskDelay(500);

		// Check SIM Insertion
		_debug_print("Sending CREG");
		uart_append_crlf(0);
		if(
			gsm_send_check_resp("AT+CREG?\r",
							"+CREG: 1,1",
							gsm_buff,
							100,
							5)
		){
			uart_append_crlf(0);
			_debug_print("CREG OK");
			uart_append_crlf(0);
			vTaskDelay(2000);	
			_debug_print("Sending CSQ");
			uart_append_crlf(0);

			if(
			gsm_send_check_resp("AT+CSQ\r",
								"OK",
								gsm_buff,
								100,
								5)
			){
			uart_append_crlf(0);
			_debug_print("RSSI: ");
			//sprintf(buff, "%d",get_rssi(gsm_buff));
			//uint8_t rssi = get_rssi(gsm_buff);
			buff[0] = gsm_buff[8];
			if(gsm_buff[9] == ',')
			{
				buff[1] = '\0';
			}
			else
			{
				buff[1] = gsm_buff[9];
				buff[2] = '\0';
			}

			uint8_t rssi = ((buff[0] - 48) * 10) + (buff[1] - 48);
			
			_debug_print(buff);
			uart_append_crlf(0);
			vTaskDelay(2000);	
			_debug_print("Sending CSTT");
			uart_append_crlf(0);

			// Register APN		
				if(
					gsm_send_check_resp("AT+CSTT=\"TATA.DOCOMO.INTERNET\"\r",
									"OK",
									gsm_buff,
									100,
									5)
				){
					uart_append_crlf(0);
					_debug_print("APN Success");
					uart_append_crlf(0);
					vTaskDelay(2000);			
					_debug_print("Sending CIICR");
					uart_append_crlf(0);
					if(
						gsm_send_check_resp("AT+CIICR\r",
										"OK",
										gsm_buff,
										1000,
										5)
					){
						uart_append_crlf(0);
						_debug_print("Bring wireless up");
						uart_append_crlf(0);
						vTaskDelay(2000);	
						_debug_print("Sending CIFSR");
						uart_append_crlf(0);
						if(
							gsm_send_check_resp("AT+CIFSR\r",
											"OK",
											gsm_buff,
											300,
											5)
						){
							uart_append_crlf(0);
							strcpy(ip_addr, gsm_buff);
							_debug_print("IP Address: ");
							_debug_print(ip_addr);
							uart_append_crlf(0);	
							vTaskDelay(2000);
							_debug_print("Sending SAPBR");
							uart_append_crlf(0);
							if(
								gsm_send_check_resp("AT+SAPBR=3,1,\"APN\",\"TATA.DOCOMO.INTERNET\"\r",
												"OK",
												gsm_buff,
												100,
												5)
							){
								uart_append_crlf(0);
								_debug_print("SAPBR1");
								uart_append_crlf(0);	
								vTaskDelay(2000);
								if(
									gsm_send_check_resp("AT+SAPBR=1,1\r",
													"OK",
													gsm_buff,
													100,
													5)
								){
									uart_append_crlf(0);
									_debug_print("SAPBR2");
									uart_append_crlf(0);	
									vTaskDelay(500);
								}
							}
						}
					}
				}
			}
		}
	}
}