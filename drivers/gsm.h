
#ifndef __gsm_h__
#define __gsm_h__

#define 	_gsmsend(str) uart_print(3, str)

extern uint8_t * gsm_buff;

extern int8_t gsm_send_check_resp(
				char *,
				char *,
				uint8_t *,
				uint32_t
		);
extern int8_t  gsm_send_at(void);
extern uint8_t gsm_check_network(void);
extern int8_t  gsm_get_rssi(void);
extern uint8_t gsm_set_apn(void);
extern uint8_t gsm_config_sapbr(void);
extern uint8_t gsm_set_sapbr(void);
extern uint8_t gsm_bring_wl_up(void);
extern uint8_t gsm_get_ip_addr(char *);
extern uint8_t gsm_get_gprs_state(void);
extern uint8_t gsm_get_ip_status(char *);
extern uint8_t gsm_enable_gprs(void);
extern uint8_t gsm_ip_shut(void);
extern uint8_t gsm_http_init(void);
extern uint8_t gsm_http_term(void);
extern uint16_t gsm_http_req_get(char *);
extern uint8_t gsm_http_set_param(char *);
extern uint8_t gsm_http_read(void);

#endif