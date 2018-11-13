#ifndef UDP_MYSQL_UDP_SERVER_H
#define UDP_MYSQL_UDP_SERVER_H
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct __sock_para{
#define BUF_SIZE 550
    struct sockaddr_in cli,addr;
    int sock_pipe[2];
    int sd=0,send_len=0,recv_len=0;
    uint8_t send_buf[BUF_SIZE], recv_buf[BUF_SIZE];
}_sock_para;

void udp_server_task(uint16_t port);
#endif