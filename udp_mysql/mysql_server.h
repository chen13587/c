#ifndef UDP_MYSQL_MYSQL_SERVER_H
#define UDP_MYSQL_MYSQL_SERVER_H
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <mysql/mysql.h>

void mysql_server_task(char* host,char* user,char* pwd,char* db);

typedef struct __mysql {            //MySQL结构体
    MYSQL *mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int sql_pipe[2];
    uint8_t sql_recv[256];
}_mysql;
extern _mysql sql_para;


#endif