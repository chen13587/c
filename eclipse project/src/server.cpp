#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "sys/types.h"
#include "unistd.h"	//实现多进程需要包含的头文件

#include "websocket.h"
#include "server.h"

#define PORT 8880
int server_start(){
	int ser_socket,cli_socket;
	unsigned char buf[1024],write_buf[1024];
	pid_t ser_read_task,cli_write_task;
	struct sockaddr_in ser_addr;
	memset(buf,0,sizeof(char)*1024);
	memset(write_buf,0,sizeof(char)*1024);
	ser_socket=socket(AF_INET,SOCK_STREAM,0);	//使用TCP协议
	if(ser_socket<0){	//创建失败
		perror("socket");
		return -1;
	}

	ser_addr.sin_port=htons(PORT);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);	//?
	ser_addr.sin_family=AF_INET;

	if(bind(ser_socket, (struct sockaddr*)&ser_addr,sizeof(ser_addr))<0){	//绑定ip和端口
		perror("bind");
		return -2;
	}
	if(listen(ser_socket,10)<0){	//监听，允许10个客户端同时连接
		perror("listen");
		return -3;
	}

	for(;;){

		printf("====== waiting for client's request PORT=%d\n",PORT);
		if((cli_socket=accept(ser_socket,NULL,NULL))<0){	//允许一个连接
			perror("accept");
		}
		printf("++++++ new client connected!!!\n");

		read(cli_socket,buf,sizeof(buf));	//不断接收客户端发送来的内容

		printf("开始握手\n");
		if(web_shake(cli_socket,(char*)buf)){	//这行处理握手包
		printf("\n握手成功\n");
		memset(buf,0,1024);

		int pe[2];
		pipe(pe);		//管道传递消息因为read阻塞进程，使用失败

		ser_read_task=fork();	//创建一个子进程
		if(ser_read_task==0){	//子进程
			printf("++++++ read task\n");
			close(ser_socket);
			unsigned char data_r[1024];		//待优化

			for(;;){
				memset(data_r,0,1024);
				memset(buf,0,sizeof(char)*1024);
//				printf("开始等待数据...\r\n");
				read(cli_socket,buf,sizeof(buf));	//不断接收客户端发送来的内容

				if(strlen((char*)buf)==0){		//客户端关闭了连接
					printf("------ read disconnect\n");
					break;
				}
//				printf("buf len: %ld receive: %s\n",strlen(buf),buf);
				//write(cli_socket,buf,strlen(buf));	//发送读取的内容到客户端

				//这里调用websocket解码函数
				printf("接收到数据，长度：%d\r\n",strlen((char*)buf));
				if(1==recv_decode(buf,data_r)){
					printf("解码数据：%s\r\n",data_r);
				}
				else{
					/****发送断开包给客户端，让客户端断开*****/
					send_encode((unsigned char*)"0712",data_r);
					write(cli_socket,data_r,strlen((char*)data_r));

					printf("解码失败，断开连接\r\n");
					write(pe[1],"end",strlen("end"));		//管道的写入端
					break;
					//执行断开程序，进行回收工作，发送程序也要断开
				}
			}
			exit(0);	//结束当前子进程
		}

		cli_write_task=fork();	//创建一个子进程

		if(cli_write_task==0){	//子进程
			printf("++++++ write task\n");
			close(ser_socket);
			unsigned char data_s[1024];		//待优化
			unsigned char flag[5];
			for(;;){
				printf("enter 'q' disconnect please putin: \n");
				memset(write_buf,0,sizeof(char)*1024);
				memset(data_s,0,1024);
				fgets((char*)write_buf,1024,stdin);

//				read(pe[0],flag,sizeof(flag));		//管道的接收端，注意：会阻塞
//				if(0==strcmp((char*)flag,"end")){
//					printf("客户端已经断开，断开连接\r\n");
//					memset(flag,0,5);
//	 				break;
//				}
//				memset(flag,0,5);

				if(strcmp((char*)write_buf,"q\n")==0){

					/****发送断开包给客户端，让客户端断开*****/
					send_encode((unsigned char*)"0712",data_s);
					write(cli_socket,data_s,strlen((char*)data_s));
					printf("------ write disconnect\n");
					break;
				}
				write_buf[strlen((char*)write_buf)-1]=0;
				if(send_encode(write_buf,data_s)){
					write(cli_socket,data_s,strlen((char*)data_s));
					printf("封包成功\r\n");
				}
				else{
					/****发送断开包给客户端，让客户端断开*****/
					send_encode((unsigned char*)"0712",data_s);
					write(cli_socket,data_s,strlen((char*)data_s));
					printf("封包失败，将断开连接\r\n");
					break;
				}

			}
			exit(0);	//结束当前子进程
		}
		close(cli_socket);
	}
	else{
		memset(buf,0,sizeof(char)*1024);
		close(cli_socket);
		printf("\n握手失败！！！");
	}
}

}
