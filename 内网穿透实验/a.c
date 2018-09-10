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
	int msocket[2],cli_socket,flag=1;
	IP ip;
	//pid_t child_task[2];
	SA_IN server,addr;
	socklen_t addrlen=sizeof(SA_IN);
	char s='a',i=0;
	char buf[512],write_buf[512];	
	pid_t ser_read_task,cli_write_task;
	memset(buf,0,sizeof(char)*512);	
	memset(write_buf,0,sizeof(char)*512);

	
	msocket[0]=socket(AF_INET,SOCK_STREAM,0);	//使用TCP协议
	if(msocket[0]<0){	//创建失败
		perror("socket");		
		return -1;
	}
	
	if(setsockopt(msocket[0],SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(int))<0){
		perror("setsockopt");
		return -2;
	}

	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=inet_addr(SER);	//?
	server.sin_family=AF_INET;

	connect(msocket[0],(SA*)&server,sizeof(SA_IN));
	send(msocket[0],&s,sizeof(char),0);	//告知服务端，我是 a客户端
	recv(msocket[0],&ip,sizeof(IP),0);	//阻塞，直到服务器下发a的外网ip和port
	printf("ip: %s port: %d\n",inet_ntoa(ip.ip),ntohs(ip.port));	//终于接收到对方ip
	close(msocket[0]);		//关闭与服务器的连接
		
	msocket[1]=socket(AF_INET,SOCK_STREAM,0);	//使用TCP协议
	if(msocket[1]<0){	//创建失败
		perror("socket");		
		return -1;
	}
	
	if(setsockopt(msocket[1],SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(int))<0){
		perror("setsockopt");
		return -2;
	}
	
     	server.sin_family=AF_INET;	
	server.sin_port=ip.port;
	server.sin_addr.s_addr=htonl(INADDR_ANY);	//?

	if(bind(msocket[1],(SA*)&server,sizeof(SA_IN))<0){
		perror("bind");
		return -3;
	}

	if(listen(msocket[1],10)<0){	//转换为服务端，监听连接，端口和上次连接S时一样
		perror("listen");
		return -4;

	}

	printf("====== start accetping...\n");	
	if((cli_socket=accept(msocket[1],(SA*)&addr,&addrlen))<0){	//等待一个连接
		perror("accept");
		return -5;
	}
	
	//callshow(accept(msocket[1],(SA*)&server,sizeof(SA_IN)));	//等待一个连接
	printf("ip: %s port: %d\n",inet_ntoa(ip.ip),ntohs(ip.port));
	printf("++++++ new client connected\n");





		ser_read_task=fork();	//创建一个子进程
		if(ser_read_task==0){	//子进程
			printf("++++++ create read task\n");
			close(msocket[1]);
			for(;;){
				read(cli_socket,buf,sizeof(buf));	//不断接收客户端发送来的内容
				if(strlen(buf)==0){		//客户端关闭了连接
					printf("------ client disconnect\n");
					break;
				}
				printf("buf len: %ld receive: %s\n",strlen(buf),buf);
				//write(msocket[1],buf,strlen(buf));	//发送读取的内容到客户端
				
				memset(buf,0,sizeof(char)*512);	
			}
			exit(0);	//结束当前子进程
		}
		
		
//		cli_write_task=fork();	//创建一个子进程
		
//		if(cli_write_task==0){	//子进程
//			printf("++++++ create write task\n");
//			close(msocket[1]);
			for(;;){
				printf("enter 'q' disconnect please putin: \n");
 		                fgets(write_buf,512,stdin);
                  		if(strcmp(write_buf,"q\n")==0){
                          	printf("------ write disconnect\n");
                          		break;
                  		}	
                  		write(cli_socket,write_buf,strlen(write_buf));
                  		memset(write_buf,0,sizeof(char)*512);

			}
			exit(0);	//结束当前子进程


//		}

/*


	for(;;){		//测试
		i=read(cli_socket,buf,sizeof(buf));
		printf("read return %d %s \n",i,buf);
		memset(buf,0,512*sizeof(char));

	}

*/


	close(cli_socket);
	printf("------ disconnect\n");
	return 0;



}
