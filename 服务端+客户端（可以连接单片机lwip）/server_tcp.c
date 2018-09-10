#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "sys/types.h"
#include "unistd.h"	//实现多进程需要包含的头文件
int main(){
	int ser_socket,cli_socket;
	char buf[2048],write_buf[2048];	
	pid_t ser_read_task,cli_write_task;
	struct sockaddr_in ser_addr;
	memset(buf,0,sizeof(char)*2048);	
	memset(write_buf,0,sizeof(char)*2048);	
	ser_socket=socket(AF_INET,SOCK_STREAM,0);	//使用TCP协议
	if(ser_socket<0){	//创建失败
		perror("socket");		
		return -1;
	}	
	
	ser_addr.sin_port=htons(8880);
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
	printf("====== waiting for client's request\n");

	for(;;){
		if((cli_socket=accept(ser_socket,NULL,NULL))<0){	//允许一个连接
			perror("accept");	
		}
		printf("++++++ new client connected...\n");
		ser_read_task=fork();	//创建一个子进程
		if(ser_read_task==0){	//子进程
			printf("++++++ create read task\n");
			close(ser_socket);
			for(;;){
				read(cli_socket,buf,sizeof(buf));	//不断接收客户端发送来的内容
				if(strlen(buf)==0){		//客户端关闭了连接
					printf("------ client disconnect\n");
					break;
				}
				printf("buf len: %ld receive: %s\n",strlen(buf),buf);
				write(cli_socket,buf,strlen(buf));	//发送读取的内容到客户端
				
				memset(buf,0,sizeof(char)*512);	
			}
			exit(0);	//结束当前子进程
		}
		
		
		cli_write_task=fork();	//创建一个子进程
		
		if(cli_write_task==0){	//子进程
			printf("++++++ create write task\n");
			close(ser_socket);
			for(;;){
				printf("enter 'q' disconnect please putin: \n");
 		                fgets(write_buf,2048,stdin);
                  		if(strcmp(write_buf,"q\n")==0){
                          	printf("------ write disconnect\n");
                          		break;
                  		}	
                  		write(cli_socket,write_buf,strlen(write_buf));
                  		memset(write_buf,0,sizeof(char)*2048);

			}
			exit(0);	//结束当前子进程


		}




		close(cli_socket);
	}

}
