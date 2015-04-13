
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "gsm.h"
#include "FreeRTOS.h"

int8_t gsm_send_check_resp(
				char * cmd,
				char * resp,
				uint16_t timeout,
				uint8_t tccount
		){

	_gsmsend(cmd);
	return 0;
}