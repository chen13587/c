#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "sys/types.h"
#include "unistd.h"	//实现多进程需要包含的头文件

#include <arpa/inet.h>
typedef struct {
	struct in_addr ip;
	int port;
} IP;

int main(){
	int ser_socket,cli_socket[2],i=0;
	//SA_IN server,addr;
	IP ip[2];
	char buf[512],write_buf[512];	
	//pid_t child_task[2];
	struct sockaddr_in ser_addr,addr;
	socklen_t addrlen=sizeof(addr);

	memset(buf,0,sizeof(char)*512);	
	ser_socket=socket(AF_INET,SOCK_STREAM,0);	//使用TCP协议
	if(ser_socket<0){	//创建失败
		perror("socket");		
		return -1;
	}	
	
	ser_addr.sin_port=htons(8889);
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
		if((cli_socket[i]=accept(ser_socket,(struct sockaddr*)&addr,&addrlen))<0){	//允许一个连接
			perror("accept");	
		}
		
		printf("++++++ new client connected...\n");
		//memset(buf,0,sizeof(char)*512);
		memcpy(&ip[i].ip,&addr.sin_addr,sizeof(struct in_addr));	  //获取ip	
		ip[i].port=addr.sin_port;		//获取port
		printf("ip: %s port: %d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
		

		i++;
/*
		if(i>=2){
			i=0;
				//交叉发送客户端IP给客户端
	
			send(cli_socket[0],&ip[1],sizeof(IP),0);
			send(cli_socket[1],&ip[0],sizeof(IP),0);
			//write();
			printf("\n------sending\n");
			printf("send ip[0]: %s port[0]: %d\n",inet_ntoa(ip[0].ip),ntohs(ip[0].port));
			printf("send ip[1]: %s port[1]: %d\n",inet_ntoa(ip[1].ip),ntohs(ip[1].port));
			close(cli_socket[0]);
			close(cli_socket[1]);
			printf("------ all disconnect\n");
		}

*/
		if(i>=2){
			i=0;
				/******发送a客户端IP给两个客户端******/
	
			send(cli_socket[0],&ip[0],sizeof(IP),0);	//a
			close(cli_socket[0]);
			
			send(cli_socket[1],&ip[0],sizeof(IP),0);	//b
			close(cli_socket[1]);
			//write();
			printf("\n------sending\n");
			printf("send ip[0]: %s port[0]: %d\n",inet_ntoa(ip[0].ip),ntohs(ip[0].port));
			printf("send ip[0]: %s port[0]: %d\n",inet_ntoa(ip[0].ip),ntohs(ip[0].port));
			printf("------ all disconnect\n");
		}




	}



}
