#include "mysql_server.h"
#include "tool.h"
#include <stdbool.h>
#include "stdio.h"
#include "iostream"
using namespace std;
/*
 * 注意：如果报错 mysql_init 未定义的引用    请链接库
 *      link_libraries(libmysqlclient.so)
 *
 * 命令行g++编译需要链接 -l mysqlclient库
 *      g++ mysql_server.cpp udp_server.cpp main.cpp -o main -l mysqlclient
 * 协议：
 *      0位 标识           0xFF
 *      1位 标识           0xFE
 *      2位 指令           0x01|0x02|0x03|0x04
 *      3位 data长度高位    length&0xff00
 *      4位 data长度低位    length&0x00ff
 *      .....             data
 *      .....             data
 *      倒数第2位           CRC校验高位
 *      倒数第1位           CRC校验低位
 *
 *      注意：CRC校验从data首位开始 到 data最后一位
 *
 * */
_mysql sql_para;

#define SQL_INSERT  0X01
#define SQL_DELETE  0X02
#define SQL_UPDATE  0X03
#define SQL_SELECT  0X04

typedef struct __cmd_func{
    int cmd;
    int (*func)(char* buf,int len);
}cmd_handler;


int sql_insert(char* buf,int len){
    printf("插入\n");

    return 0;
}
int sql_delete(char* buf,int len){
    printf("删除\n");

    return 0;
}
int sql_update(char* buf,int len){
    printf("更新\n");

    return 0;
}
#include "string.h"
int sql_select(char* buf,int len){
    printf("查询\n");
    buf[len]='\0';	//
    //char tm[100]={0};
    //sprintf(tm,"%s",buf);      //TODO
    //printf("拼接：%s\n",buf);
	if(0 == mysql_query(sql_para.mysql,buf)) {
        if(NULL != (sql_para.res = mysql_store_result(sql_para.mysql))) {
            //printf("id\tname\tage\tpwd\n");
            while (NULL!=(sql_para.row=mysql_fetch_row(sql_para.res))){
                printf("%s\t\t%s\t\t%s\n", sql_para.row[0],sql_para.row[1],sql_para.row[2]);
            }
        }else{
            perror("result error\n");
            return 1;
        }
    } else {
        perror("query error\n");
        return 1;
    }
    return 0;
}
cmd_handler cmdHandler[]={
        {SQL_INSERT,sql_insert},
        {SQL_DELETE,sql_delete},
        {SQL_UPDATE,sql_update},
        {SQL_SELECT,sql_select},
};

//返回0执行成功 返回1执行失败
int search_handler(uint8_t cmd,char* buf,int len){      //只传入data数据段
    for(int i=0;i<sizeof(cmdHandler)/ sizeof(cmd_handler);i++){
        if(cmdHandler[i].cmd==cmd){
            if(cmdHandler[i].func!=NULL){
                return cmdHandler[i].func(buf,len);
            }else {
                printf("func is NULL\n");
                return 1;
            }
        }
    }
    printf("not func\n");
    return 1;
}

void mysql_server_task(char* host,char* user,char* pwd,char* db) {
    uint16_t read_len=0,data_len=0;
    sql_para.mysql = mysql_init(sql_para.mysql) ;
    if (NULL!=mysql_real_connect(sql_para.mysql, host,user, pwd,db, 3306, NULL, 0)) {
        printf("connect success\n");
        for(;;){
            memset(sql_para.sql_recv, 0, sizeof(sql_para.sql_recv));

            read_len=read(sql_para.sql_pipe[0],sql_para.sql_recv, sizeof(sql_para.sql_recv));
            //read_len=hex2ascii(sql_para.sql_recv,sql_para.sql_recv,read_len);

            if(!strncmp("exit",(char*)sql_para.sql_recv,strlen("exit")))
                break;
            data_len=u8_2_u16(&sql_para.sql_recv[3]);
            printf("data_len=%d\n",data_len);
		if((0xff==sql_para.sql_recv[0])&&(0xfe==sql_para.sql_recv[1])){
                if(check_crc(&sql_para.sql_recv[5],data_len,&sql_para.sql_recv[read_len-2])){

                    sql_para.sql_recv[5+data_len]=0;
                    printf("SQL recv: %s\n",&sql_para.sql_recv[5]);
		   // sql_para.sql_recv[read_len-2]='\0';
                    if(0==search_handler(sql_para.sql_recv[2],(char*)&sql_para.sql_recv[5],data_len)){
                        printf("SQL execute success\n");
                    }else{
                        printf("SQL execute failed\n");
                    }
                }
            }else{
                printf("not SQL packet\n");
            }
        }
    } else {
        perror("connect error\n");
    }

    mysql_free_result(sql_para.res);
    mysql_close(sql_para.mysql);
    printf("exit MySQL");
}
