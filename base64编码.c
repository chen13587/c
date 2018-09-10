#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "netinet/in.h"
#include "unistd.h"
#include "sys/types.h"

#include <openssl/sha.h>

int main(){

	//base64编码
	int i=0,ob=0;
	char *code="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char recvbuf[512];
	char *outbuf=0;
	for(;;){
	i=0;
	ob=0;
	memset(recvbuf,0,512);


	fgets(recvbuf,512,stdin);
	recvbuf[strlen(recvbuf)-1]=0;		//去掉fgets加在末尾的空格

	int reclen=strlen(recvbuf);
	outbuf=(char*)malloc(4*(reclen/3)+4);
	memset(outbuf,0,4*(reclen/3)+4);
	char tmp[4];
	char sta=strlen(recvbuf)%3;
	for(;i<reclen;i+=3,ob+=4){		//每3个字节编码
		outbuf[ob]=code[(recvbuf[i]>>2)&0x3f];

		outbuf[ob+1]=code[(((recvbuf[i]<<4)&0x30)|((recvbuf[i+1]>>4)&0x0f))];
		if((sta==1)&&((reclen-i)<3)){
			outbuf[ob+2]='=';
			outbuf[ob+3]='=';
			outbuf[ob+4]='\0';
			break;
		}
		outbuf[ob+2]=code[(((recvbuf[i+1]<<2)&0x3c)|((recvbuf[i+2]>>6)&0x03))];
		if((sta==2)&&((reclen-i)<3)){
			outbuf[ob+3]='=';
			outbuf[ob+4]='\0';
			break;
		}
		outbuf[ob+3]=code[recvbuf[i+2]&0x3f];
	}
	if(sta==0){
		outbuf[ob+1]='\0';
	}
	printf("%s\n",(char*)outbuf);
	free(outbuf);
	outbuf=0;

	}


	 return 0;
}


