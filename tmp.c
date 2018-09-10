#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "sys/types.h"
#include "unistd.h"	//实现多进程需要包含的头文件
int main(){
	int cli_socket;
	char buf[512],read_buf[512];	
	pid_t cli_write_task,ser_read_task;
	struct sockaddr_in ser_addr;
	memset(buf,0,sizeof(char)*512);	
	memset(read_buf,0,sizeof(char)*512);
	cli_socket=socket(AF_INET,SOCK_STREAM,0);	//创建套接字 使用TCP协议
	if(cli_socket<0){	//创建失败
		perror("socket");		
		return -1;
	}	
	ser_addr.sin_port=htons(8089);
	ser_addr.sin_addr.s_addr=inet_addr("10.10.70.53");	//?
	ser_addr.sin_family=AF_INET;
	printf("====== start connecting...\n");	
	if(connect(cli_socket, (struct sockaddr*)&ser_addr,sizeof(ser_addr))<0){	//连接服务端
		perror("connect");
		return -2;
	}
	printf("++++++ connect success!\n");

	ser_read_task=fork();
	if(ser_read_task==0){
		//close(cli_socket);
		for(;;){
			   read(cli_socket,read_buf,sizeof(read_buf));       //不断接收服务端发送来的内容
                           if(strlen(read_buf)==0){             //客户端关闭了连接
                           	printf("------ client disconnect\n");
                                break;
                           }
                           printf("buf len: %ld receive: %s\n",strlen(read_buf),read_buf);
                           write(cli_socket,read_buf,strlen(read_buf));      //发送读取的内容到服务端
                           memset(read_buf,0,sizeof(char)*512);			
			
		}

		exit(0);	//结束当前子进程
	}
	
//	cli_write_task=fork();
//     	if(cli_write_task==0){
                printf("++++++ create read task\n");
                //close(cli_socket);
                for(;;){
                        printf("enter 'q' disconnect pease putin:\n");
                        fgets(buf,512,stdin); 
                        if(strcmp(buf,"q\n")==0){
                                printf("------ disconnect\n");
                                break;
                        }
                        write(cli_socket,buf,strlen(buf));
                        memset(buf,0,sizeof(char)*512);         
                }
                exit(0);
//	 }


	close(cli_socket);
}
