#ifndef _WEBSOCKET_H_
#define _WEBSOCKET_H_


int web_shake(int cli_skt,char *recv_head);
int recv_decode(unsigned char *recvdata,unsigned char *buf);
int send_encode(unsigned char *senddata,unsigned char *buf);
#endif
