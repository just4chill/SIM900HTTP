
#include <stdint.h>
#include "uart.h"
#include <string.h>
#include "gsm.h"


char tcp_state[32];
char ip_address[32];
char http_resp_code[5];
char http_read_buff[128];

/*
	Command	:  	AT
	Response: 	OK
*/
uint8_t gsm_check_comm(void)
{
	uart3_rx_head = 0;
	uart_print(3, "AT\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');
	if(uart3_rx_fifo[0] == 'O' && uart3_rx_fifo[1] == 'K')
	{
		return 1;
	}
	return 0;
}

/*
	Command	:  	AT+CREG?
	Response: 	+CREG: 0,1
*/
uint8_t gsm_check_sim(void)
{
	uart3_rx_head = 0;
	uart_print(3, "AT+CREG?\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');
	if(uart3_rx_fifo[9] == '1')
	{
		return 1;
	}
	return 0;
}

/*
	Command	:  	AT+CSQ
	Response: 	+CSQ: <rssi>,<ber>
*/
uint8_t gsm_read_rssi(void)
{
	uint8_t temp[5];
	uint8_t rssi;
	uart3_rx_head = 0;
	uart_print(3, "AT+CSQ\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');

	if(uart3_rx_fifo[7] != ',')
	{
		rssi = (uart3_rx_fifo[7] - 48) + ((uart3_rx_fifo[6] - 48) * 10);
	}	
	else if(uart3_rx_fifo[7] == ',')
	{
		rssi = (uart3_rx_fifo[7] - 48);
	}
	else
		rssi = 0;

	return rssi;
}

/*
	Command	:  	AT+CSTT=<APN>
	Response: 	OK
*/
uint8_t gsm_set_apn(char * apn)
{
	uart3_rx_head = 0;

	uart_print(3, "AT+CSTT=\"");
	uart_print(3, apn);
	uart_print(3, "\"\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');

	if(uart3_rx_fifo[0] == 'O' && uart3_rx_fifo[1] == 'K')
	{
		return 1;
	}
	return 0;
}

/*
	Command	:  	AT+CIICR
	Response: 	OK
*/
uint8_t gsm_wireless_up(void)
{
	uart3_rx_head = 0;
	
	uart_print(3, "AT+CIICR\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');

	if(uart3_rx_fifo[0] == 'O' && uart3_rx_fifo[1] == 'K')
	{
		return 1;
	}
	return 0;
}

/*
	Command	:  	AT+CIPSTAUTS
	Response: 	<STATUS STRING>
*/
void gsm_read_tcpstate(void)
{
	uart3_rx_head = 0;
	
	uart_print(3, "AT+CIPSTATUS\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');

	memcpy(tcp_state, uart3_rx_fifo, uart3_rx_head);
}

/*
	Command	:  	AT+CIFSR
	Response: 	<IP ADDRESS>
*/
void gsm_read_tipaddress(void)
{
	uart3_rx_head = 0;
	
	uart_print(3, "AT+CIFSR\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');

	memcpy(ip_address, uart3_rx_fifo, uart3_rx_head);
}

/*
	Command	:  	AT+SAPBR=3,1,"APN",<APN>
	Response: 	OK
*/
uint8_t gsm_set_sapbr(char * apn)
{
	uart3_rx_head = 0;
	
	uart_print(3, "AT+SAPBR=3,1,\"APN\",\"");
	uart_print(3, apn);
	uart_print(3, "\"\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');

	if(uart3_rx_fifo[0] == 'O' && uart3_rx_fifo[1] == 'K')
	{
		return 1;
	}
	return 0;
}

/*
	Command	:  	AT+SAPBR=1,1
	Response: 	OK
*/
uint8_t gsm_up_sapbr(void)
{
	uart3_rx_head = 0;
	
	uart_print(3, "AT+SAPBR=1,1\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');

	if(uart3_rx_fifo[0] == 'O' && uart3_rx_fifo[1] == 'K')
	{
		return 1;
	}
	return 0;
}

/*
	Command	:  	AT+HTTPINIT
	Response: 	OK
*/
uint8_t gsm_http_init(void)
{
	uart3_rx_head = 0;
	
	uart_print(3, "AT+HTTPINIT\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');

	if(uart3_rx_fifo[0] == 'O' && uart3_rx_fifo[1] == 'K')
	{
		return 1;
	}
	return 0;
}

/*
	Command	:  	AT+HTTPTERM
	Response: 	OK
*/
uint8_t gsm_http_term(void)
{
	uart3_rx_head = 0;
	
	uart_print(3, "AT+HTTPTERM\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');

	if(uart3_rx_fifo[0] == 'O' && uart3_rx_fifo[1] == 'K')
	{
		return 1;
	}
	return 0;
}

/*
	Command	:  	AT+HTTPPARA
	Response: 	OK
*/
uint8_t gsm_http_para(char * url)
{
	uart3_rx_head = 0;
	
	uart_print(3, "AT+HTTPPARA=\"URL\",\"");
	uart_print(3, url);
	uart_print(3, "\"\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');

	if(uart3_rx_fifo[0] == 'O' && uart3_rx_fifo[1] == 'K')
	{
		return 1;
	}
	return 0;
}

/*
	Command	:  	AT+HTTPACTION=0
	Response: 	+HTTPACTION: <Method>,< StatusCode >,<DataLen>
*/
uint8_t gsm_http_get(void)
{
	uart3_rx_head = 0;
	
	uart_print(3, "AT+HTTPACTION=0\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');

	if(uart3_rx_fifo[13] == '0')
	{
		http_resp_code[0] = uart3_rx_fifo[15];
		http_resp_code[1] = uart3_rx_fifo[16];
		http_resp_code[2] = uart3_rx_fifo[17];
		http_resp_code[3] = NULL;
		if(	http_resp_code[0] == '2' &&
			http_resp_code[1] == '0' &&
			http_resp_code[2] == '0')
				return 1;
		return 0;
	}
	return 0;
}

/*
	Command	:  	AT+HTTPREAD
	Response: 	+HTTPREAD
				<data>
*/
uint16_t gsm_http_read(void)
{
	uint8_t dat_end = 0;
	uart3_rx_head = 0;
	
	uart_print(3, "AT+HTTPREAD\r\n");

	// TODO: Timer to unblock
	while(uart3_rx_fifo[uart3_rx_head] != '\n');
	uart3_rx_head = 0;
	while(uart3_rx_fifo[uart3_rx_head] != '\n');
	dat_end = uart3_rx_head;

	memcpy(http_read_buff, uart3_rx_fifo, dat_end);
	return dat_end;
}