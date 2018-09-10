#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "netinet/in.h"
#include "unistd.h"
#include "sys/types.h"
#include <openssl/sha.h>
#include <openssl/sha.h>

void sha1_base64(char *recvbuf,char *buf){
	#define sha1_len 25		//理论上是20位，但一定要大于20
	unsigned char recbuf[sha1_len]={0};
	char outbuf_sha1[sha1_len*2]={0};
	//base64编码
	int i=0,ob=0;
	char *code="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	char *outbuf=0;
//	for(;;){
	i=0;
	ob=0;
	memset(recbuf,0,sha1_len);
	memset(outbuf_sha1,0,sha1_len*2);

	char *tmp1=(char*)malloc(256);		//如果出现乱码，检查这步空间是否不够
	memset(tmp1,0,256);
//	printf("源字串:%s\n",recvbuf);

	strcat(tmp1,recvbuf);		//移到大空间
	strcat(tmp1,"258EAFA5-E914-47DA-95CA-C5AB0DC85B11");		//拼接魔法字串GUID


	SHA1((const unsigned char*)tmp1,strlen(tmp1),recbuf);
	free(tmp1);
	tmp1=0;
	//输出sha1检查
	for(int i=0;i<sha1_len;i++){
		sprintf(&outbuf_sha1[i*2],"%02x",(unsigned int)recbuf[i]);
	}
	printf("sha1值：%s\n",outbuf_sha1);

	/*******************************************/
	int reclen=20;		//固定为20字节 之前strlen((const char *)recbuf);
	outbuf=(char*)malloc(4*(reclen/3)+4);
	memset(outbuf,0,4*(reclen/3)+4);
//	char tmp[4];
	char sta=strlen((const char *)recbuf)%3;
	for(;i<reclen;i+=3,ob+=4){		//每3个字节编码
		outbuf[ob]=code[(recbuf[i]>>2)&0x3f];

		outbuf[ob+1]=code[(((recbuf[i]<<4)&0x30)|((recbuf[i+1]>>4)&0x0f))];
		if((sta==1)&&((reclen-i)<3)){
			outbuf[ob+2]='=';
			outbuf[ob+3]='=';
			outbuf[ob+4]='\0';
			break;
		}
		outbuf[ob+2]=code[(((recbuf[i+1]<<2)&0x3c)|((recbuf[i+2]>>6)&0x03))];
		if((sta==2)&&((reclen-i)<3)){
			outbuf[ob+3]='=';
			outbuf[ob+4]='\0';
			break;
		}
		outbuf[ob+3]=code[recbuf[i+2]&0x3f];
	}
	if(sta==0){
		outbuf[ob+1]='\0';
	}
	printf("sha1->base64值：%s\n",(char*)outbuf);
	/*******************************************/
	memcpy(buf,outbuf,strlen((const char*)outbuf));
	free(outbuf);
	outbuf=0;
//	}

}
