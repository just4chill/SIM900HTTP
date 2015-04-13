
#ifndef __gsm_h__
#define __gsm_h__

#define 	_gsmsend(str) uart_print(3, str)

extern int8_t gsm_send_check_resp(
				char *,
				char *,
				uint8_t *,
				uint16_t,
				uint8_t
		);

#endif