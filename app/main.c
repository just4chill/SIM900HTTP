
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

char 	ip_addr[20];
char 	gprs_state[20];
char 	http_status_code[20];
uint16_t http_status = 404;
int8_t 	rssi = 0;
char 	rssi_buff[3];

void 		prvSetupHardware( void );
static void sendToModem(void * pvParameters);
static void respFromModem(void * pvParameters);

void upModem(void);
void http_get_test(void);

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
		http_get_test();
		vTaskDelay(1000);
	}
}

static void respFromModem(void * pvParameters)
{
	_debug_print("Modem respFromModem\r\n");
	for(;;)
	{
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

void http_get_test(void)
{
	if(gsm_http_init() == 1)
	{
		_debug_print("http init ok\r\n");
		if(gsm_http_set_param("http://thawing-wave-2888.herokuapp.com/weight/1") == 1)
		{
			_debug_print("http set para ok\r\n");
			http_status = gsm_http_req_get(http_status_code);
			_debug_print("status: ");
			_debug_print(http_status_code);
			_debug_print("\r\n");
			if(http_status == 200)
			{
				_debug_print("http 200\r\n");
				gsm_http_read();
			}
			if(gsm_http_term() == 1)
			{
				_debug_print("http term ok\r\n");
			}
		}
		else
		{
			_debug_print("http para failed\r\n");
		}
	}
	else
	{
		_debug_print("http init failed\r\n");
		_debug_print(gsm_buff);
	}
}

void modem_testA(void)
{
	if(gsm_send_at() == 1)
		_debug_print("Got OK\r\n");
	else
		_debug_print("Error: \"AT\"\r\n");

	vTaskDelay(500);
	if(gsm_check_network() == 1)
		_debug_print("Network reg OK\r\n");
	else
		_debug_print("Error: \"AT+CREG?\"\r\n");
	vTaskDelay(500);
	// Read RSSI
	rssi = gsm_get_rssi();
	rssi_buff[0] = (rssi / 10) + 48;
	rssi_buff[1] = (rssi % 10) + 48;
	rssi_buff[2] = '\0';
	if(rssi > 10)
	{
		_debug_print("RSSI: ");
		_debug_print(rssi_buff);
		_debug_print("\r\n");
	}
	else
		_debug_print("Error: \"AT+CSQ\"\r\n");
}

void modem_testB(void)
{
	if(gsm_get_gprs_state() == 1)
		_debug_print("GPRS Enabled\r\n");
	else
		_debug_print("Error: \"AT+CGATT?\"\r\n");

	if(gsm_set_apn() == 1)
		_debug_print("APN Set success\r\n");
	else
		_debug_print("Error: \"AT+CSTT\"\r\n");

	if(gsm_bring_wl_up() == 1)
		_debug_print("wl up success\r\n");
	else
		_debug_print("wl up failed\r\n");

	if(gsm_config_sapbr() == 1)
		_debug_print("SAPBR config success\r\n");
	else
		_debug_print("Error: \"AT+SAPBR\"\r\n");

	if(gsm_set_sapbr() == 1)
		_debug_print("SAPBR set success\r\n");
	else
		_debug_print("Error: \"AT+SAPBR\"\r\n");
}

void upModem(void)
{
	modem_testA();
	modem_testB();
	gsm_get_ip_status(gprs_state);
	gsm_get_ip_addr(ip_addr);
	_debug_print("ip: ");
	_debug_print(ip_addr);
	_debug_print("\r\n");
	_debug_print("status: ");
	_debug_print(gprs_state);
	_debug_print("\r\n");
	vTaskDelay(1000);
}