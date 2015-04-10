
#ifndef __gsm_h__
#define __gsm_h__

extern int8_t 	gsm_send_check_resp	(	char *, 
										char *
									);

extern void 	gsm_allocate_mem_for_tokens	(	uint8_t, 
												uint8_t
											);

extern void 	gsm_free_mem_for_tokens(uint8_t);

extern int8_t 	gsm_tokenize_response(	char *,
										char *,
										char *,
										char *,
										uint8_t
									);

extern char * respTokens[];

#endif