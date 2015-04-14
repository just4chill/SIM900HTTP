
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "gsm.h"
#include "FreeRTOS.h"
#include "task.h"

uint8_t * gsm_buff;

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
				uint32_t timeout
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
	//_debug_print(resp_store);
	char * str = strstr(resp_store, resp);
	if(str != NULL)
	{
		//_debug_print("found\r\n");
		return 1;
	}
	else
	{
		//_debug_print("notfound\r\n");
		return -1;
	}

	/* Never execute */
	return 0;
}

void gsm_send_no_check_resp(
				char * cmd,
				uint8_t * resp_store,
				uint32_t timeout
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
}

void gsm_read_resp(
				uint8_t * resp_store,
				uint32_t timeout
		){
	vTaskDelay(timeout);
	uint8_t i = 0;
	do
	{
		char c = uart_getc(3);
		resp_store[i] = (uint8_t)c;
		i++;
	}
	while(uart3.num_bytes > 0);
}

int8_t gsm_send_at(void)
{
	return gsm_send_check_resp(	"AT\r",
								"OK",
								gsm_buff,
								100);
}

uint8_t gsm_check_network(void)
{
	gsm_send_no_check_resp(	"AT+CREG?\r",
							gsm_buff,
							200);
	// <cr><lf>+CREG: 1,1
	return gsm_buff[9] - 48;
}

int8_t gsm_get_rssi(void)
{
	gsm_send_no_check_resp(	"AT+CSQ\r",
							gsm_buff,
							200);
	// <cr><lf>+AT+CSQ: rssi,
	int8_t rssi = 0;
	if(gsm_buff[12] == ',')
	{
		rssi = gsm_buff[8] - 48;
	}
	else
	{
		rssi = ((gsm_buff[8] - 48) * 10) + (gsm_buff[9] - 48);
	}
	// If any conversion error
	if(rssi <= 100 && rssi >= 0)
	{
		return rssi;
	}
	// Error
	else
	{
		return -1;
	}
}

uint8_t gsm_set_apn(void)
{
	return gsm_send_check_resp(	"AT+CSTT=\"TATA.DOCOMO.INTERNET\"\r",
								"OK",
								gsm_buff,
								100);
}

uint8_t gsm_config_sapbr(void)
{
	return gsm_send_check_resp(	"AT+SAPBR=3,1,\"APN\",\"TATA.DOCOMO.INTERNET\"\r",
								"OK",
								gsm_buff,
								100);
}

uint8_t gsm_set_sapbr(void)
{
	return gsm_send_check_resp(	"AT+SAPBR=1,1\r",
								"OK",
								gsm_buff,
								2000);	
}

uint8_t gsm_bring_wl_up(void)
{
	return gsm_send_check_resp(	"AT+CIICR\r",
								"OK",
								gsm_buff,
								2000);	
}

uint8_t gsm_get_ip_addr(char * buff)
{
	gsm_send_no_check_resp(	"AT+CIFSR\r",
							gsm_buff,
							100);	

	char c = '\0';
	for(uint8_t i = 0;c != '\r';i++)
	{
		c = gsm_buff[i + 2];
		buff[i] = c;
	}
	//strcpy(buff, gsm_buff);

	// TODO: Validation
	return 1;
}

uint8_t gsm_get_gprs_state(void)
{
	// <cr><lf>+CGATT: state
	gsm_send_no_check_resp(	"AT+CGATT?\r",
							gsm_buff,
							100);
	return gsm_buff[10] - 48;	
}

// TODO: Error in string parsing
uint8_t gsm_get_ip_status(char * state)
{
	// <cr><lf>+STATE: <state><cr><lf>
	gsm_send_no_check_resp(	"AT+CIPSTATUS\r",
							gsm_buff,
							500);
	char c = '\0';
	for(uint8_t i = 0;c != '\r';i++)
	{
		c = gsm_buff[i + 15];
		state[i] = c;
	}	
	return 1;
}

uint8_t gsm_ip_shut(void)
{
	return gsm_send_check_resp(	"AT+CIPSHUT\r",
								"SHUT OK",
								gsm_buff,
								1000);
}

uint8_t gsm_enable_gprs(void)
{
	return gsm_send_check_resp(	"AT+CGATT=1\r",
								"OK",
								gsm_buff,
								100);
}

uint8_t gsm_http_init(void)
{
	return gsm_send_check_resp(	"AT+HTTPINIT\r",
								"OK",
								gsm_buff,
								500);	
}

uint8_t gsm_http_term(void)
{
	return gsm_send_check_resp(	"AT+HTTPTERM\r",
								"OK",
								gsm_buff,
								1000);	
}

uint16_t gsm_http_req_get(char * status)
{
	// <cr><lf>+HTTPACTION: 0,<status>
	gsm_send_no_check_resp(	"AT+HTTPACTION=0\r",
							gsm_buff,
							5000);	
	status[0] = gsm_buff[22];
	status[1] = gsm_buff[23];
	status[2] = gsm_buff[24];
	status[3] = '\0';
	uint16_t _status =  ((status[0] - 48) * 100) + ((status[1] - 48) * 10) + (status[2] - 48);
	return _status;
}

uint16_t gsm_http_req_post(char * status, char * url)
{
	// <cr><lf>+HTTPACTION: 0,<status>
	gsm_send_no_check_resp(	"AT+HTTPACTION=1\r",
							gsm_buff,
							5000);	
	status[0] = gsm_buff[22];
	status[1] = gsm_buff[23];
	status[2] = gsm_buff[24];
	status[3] = '\0';
	uint16_t _status =  ((status[0] - 48) * 100) + ((status[1] - 48) * 10) + (status[2] - 48);
	return _status;
}


uint8_t gsm_http_set_param(char * url)
{
	_gsm_print("AT+HTTPPARA=\"URL\",\"");
	_gsm_print(url);
	return gsm_send_check_resp(	"\"\r",
								"OK",
								gsm_buff,
								1000);	
	return 1;
}

uint8_t gsm_http_read(void)
{
	_gsm_print("AT+HTTPREAD\r");
	gsm_read_resp(gsm_buff, 1000);
	_debug_print(gsm_buff);
	return 1;
}
