#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "udp_server.h"
#include "mysql_server.h"

using namespace std;
pid_t udp_server_pid=0,mysql_server_pid=0;

#define HOST      "127.0.0.1"
#define USER      "chen"
#define PASSWD    "5920"
#define DATABASE  "cpl"

int main() {
    pipe(sql_para.sql_pipe);

    printf("System start\n");
    mysql_server_pid=fork();
    if(mysql_server_pid==0){
        mysql_server_task(HOST,USER,PASSWD,DATABASE);
    }
    udp_server_pid=fork();
    if(udp_server_pid==0){
        udp_server_task(5683);
    }

    while(1);
}
