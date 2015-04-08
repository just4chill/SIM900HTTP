
#ifndef __gsm_h__
#define __gsm_h__

extern void 	gsm_read_tcpstate(void);
extern void 	gsm_read_tipaddress(void);

extern uint8_t 	gsm_check_comm(void);
extern uint8_t 	gsm_check_sim(void);
extern uint8_t 	gsm_read_rssi(void);
extern uint8_t 	gsm_set_apn(char *);
extern uint8_t 	gsm_wireless_up(void);
extern uint8_t 	gsm_set_sapbr(char *);
extern uint8_t 	gsm_up_sapbr(void);
extern uint8_t 	gsm_http_init(void);
extern uint8_t 	gsm_http_term(void);
extern uint8_t 	gsm_http_para(char *);
extern uint8_t 	gsm_http_get(void);
extern uint16_t gsm_http_read(void);

extern char tcp_state[32];
extern char ip_address[32];
extern char http_resp_code[5];
extern char http_read_buff[128];

#endif