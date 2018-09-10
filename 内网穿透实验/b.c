#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "sys/types.h"
#include "unistd.h"	//实现多进程需要包含的头文件

#include <arpa/inet.h>
#define SER "chenpl.xyz"
#define PORT 8889
typedef struct {
	struct in_addr ip;
	int port;
} IP;
typedef struct sockaddr SA;
typedef struct sockaddr_in SA_IN;

void callshow(int sockfd){		//连接成功，开启回显服务
	char buf[512];
	printf("++++++ new client connect\n");
	for(;;){
		memset(buf,0,512*sizeof(char));
		recv(sockfd,buf,sizeof(buf),0);
		printf("call recv: %s\n",buf);
		send(sockfd,buf,strlen(buf),0);
	}
}

int main(){
	int msocket[2],flag=1;
	IP ip;
	char buf[512],read_buf[512],i;
	pid_t cli_write_task,ser_read_task;
	memset(buf,0,sizeof(char)*512);	
	memset(read_buf,0,sizeof(char)*512);

	//pid_t child_task[2];
	SA_IN server,addr;
	socklen_t addrlen=sizeof(SA_IN);
	char s='b';


	
	msocket[0]=socket(AF_INET,SOCK_STREAM,0);	//使用TCP协议
	if(msocket[0]<0){	//创建失败
		perror("socket");		
		return -1;
	}
	
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=inet_addr(SER);	//?
	server.sin_family=AF_INET;

	if(setsockopt(msocket[0],SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(int))<0){
		perror("setsockopt");
		return -2;
	}
	connect(msocket[0],(SA*)&server,sizeof(SA_IN));
	send(msocket[0],&s,sizeof(char),0);	//告知服务端，我是 b客户端
	recv(msocket[0],&ip,sizeof(IP),0);	//阻塞，直到服务器下发目标的外网ip和port
	printf("ip: %s port: %d\n",inet_ntoa(ip.ip),ntohs(ip.port));	//终于接收到对方ip
	close(msocket[0]);		//关闭与服务器的连接
	msocket[1]=socket(AF_INET,SOCK_STREAM,0);	//使用TCP协议
	if(msocket[1]<0){	//创建失败
		perror("socket");		
		return -1;
	}
	
	server.sin_port=(ip.port);
	server.sin_addr=(ip.ip);	//?
	
	if(connect(msocket[1],(SA*)&server,sizeof(SA_IN))<0){	//连接目标
		perror("connect");
		return -2;
	}	
	printf("++++++ connect success!!!\n");
	
	printf("ip: %s port: %d\n",inet_ntoa(ip.ip),ntohs(ip.port));
	
	ser_read_task=fork();
	if(ser_read_task==0){
		//close(cli_socket);
		for(;;){
			   recv(msocket[1],read_buf,sizeof(read_buf),0);       //不断接收服务端发送来的内容
                           if(strlen(read_buf)==0){             //客户端关闭了连接
                           	printf("------ client disconnect\n");
                                break;
                           }
                           printf("buf len: %ld receive: %s\n",strlen(read_buf),read_buf);
                           //write(msocket[1],read_buf,strlen(read_buf));      //发送读取的内容到服务端
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
                        write(msocket[1],buf,strlen(buf));
                        memset(buf,0,sizeof(char)*512);         
                }
                exit(0);
//	 }

/*
		
	for(;;){	//测试
		printf("please putin:\n");
		fgets(buf,512,stdin);
		i=write(msocket[1],buf,strlen(buf));
		printf("write return %d\n",i);
		memset(buf,0,512*sizeof(char));

	}
*/

	close(msocket[1]);
	return 0;
}
