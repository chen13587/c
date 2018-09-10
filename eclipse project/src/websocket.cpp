#include "websocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include "sys/socket.h"
#include "netinet/in.h"
#include "unistd.h"
#include "sys/types.h"
#include <iostream>
#include "sha1_base64.h"
#include "netinet/in.h"

//shake 成功返回1  失败返回0
int web_shake(int cli_skt,char * recv_head){
	char tmp[5];
	memset(tmp,0,5);
//	printf("接收到数据：%s\n",recv_head);
	strncpy(tmp,recv_head,3);
	printf("\ntmp1=%s\n",tmp);
	if(strcmp(tmp,"GET")){		//校验
		printf("\n\不是websocket握手包，停止解包...\n");
		return 0;
	}
//	printf("是客户端握手包\n");
	const char *a=0;
	if((a=strstr(recv_head,"Sec-WebSocket-Key: "))==NULL){
		printf("找不到握手包key，停止解包\n");
		return 0;
	}
	int b=(a-recv_head)+19;
//	printf("-------客户端key起始位置：%d----------",b);
	char *clikey=(char*)malloc(30);
	strncpy(clikey,recv_head+b,24);
	printf("客户端key：%s\n",clikey);
	//	生成应答key
	char *output=(char*)malloc(30);
	sha1_base64(clikey,output);
	printf("应答Key：%s\n",output);
	char *allowpack=(char *)malloc(200);
	sprintf(allowpack,
	"HTTP/1.1 101 Switching Protocols\r\nConnection: Upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Accept: %s\r\n\r\n",
	output);
	write(cli_skt,allowpack,strlen(allowpack));
//	printf("%s",allowpack);		//打印完整应答包

	/*******回收内存******/
	free(allowpack);
	allowpack=0;
	free(clikey);
	clikey=0;
	free(output);
	output=0;

	return 1;
}

//decode 成功返回1  失败返回0
int recv_decode(unsigned char *recvdata,unsigned char *buf){
	int size=strlen((char*)recvdata);
//	printf("荷载数据：%s",recvdata1);
//	unsigned char mask[4];
	unsigned int i=0;
	unsigned char *dcdata=(unsigned char*)malloc(1024);
	memset(dcdata,0,1024);
//								//fin位暂时未处理
//
	if((recvdata[0]&0x0f)==0x08){		//opcode位
		printf("客户端断开连接\r\n");
		return 0;
		/*
			0x01表示文本数据帧
			0x02表示二进制数据帧

			0x00表示附加数据帧
			0x08表示连接关闭
			0x09表示ping
			0x0a表示pong
			0x03-0x07保留
			0x0b-0x0f保留
		 */
	}
	if((recvdata[1]&0x80)==0x80){		//有掩码
//		printf("有掩码\r\n");
		if((recvdata[1]&0x7f)==126){	//获取4个字节掩码
			unsigned int datalen=0;
			datalen=(datalen|recvdata[2])<<8|recvdata[3];
//			for(i=0;i<4;i++){
//				mask[i]=recvdata[i+4];
//				printf("掩码%02x-",mask[i]);
//			}
			printf("解包负载数据长度：%d\r\n",datalen);
//			printf("转换字节序后长度：%d\r\n",ntohl(datalen));
			for(i=0;i<datalen;i++){
				dcdata[i]=recvdata[i+8]^recvdata[(i%4)+4];	//掩码从4开始 真实数据从(掩码+4)=8开始
			}
		}
		else if((recvdata[1]&0x7f)==127){
			unsigned long long datalen=0;
			for(i=0;i<8;i++){
				datalen|=recvdata[i+2]<<(8*i);
			}
//			for(i=0;i<4;i++){
//				mask[i]=recvdata[i+10];
//				printf("掩码为：%02x",mask[i]);
//			}
			printf("解包负载数据长度：%ld\r\n",datalen);
//			printf("转换字节序后长度：%ld\r\n",ntohl(datalen));
			for(i=0;i<datalen;i++){
				dcdata[i]=recvdata[i+14]^recvdata[(i%4)+10];	//掩码从10开始 真实数据从(掩码+4)=14开始
			}
		}
		else{
			unsigned int datalen=recvdata[1]&0x7f;
//			for(i=0;i<4;i++){
//				mask[i]=recvdata[i+2];//获取掩码
//				printf("掩码为：%02x",mask[i]);
//			}
			printf("解包负载数据长度：%d\r\n",datalen);
			for(i=0;i<datalen;i++){
				dcdata[i]=recvdata[i+6]^recvdata[(i%4)+2];
			}
//			printf("转换字节序后长度：%d\r\n",ntohl(datalen));
		}


	}
	else{
		free(dcdata);
		dcdata=0;
		printf("没有掩码，数据错误将断开\r\n");
		return 0;
	}
	if(0==strcmp((char*)dcdata,"0712")){
		printf("服务端要断开连接\r\n");
		return 0;
	}


	memcpy((char*)buf,(char*)dcdata,strlen((const char*)dcdata));
	free(dcdata);
	dcdata=0;

//	char *tmp=(char*)malloc(1024);
//
//	for(int i=0;i<size;i++){
//		sprintf(&tmp[i*2],"%02x",recvdata[i]);	//unsigned char
//	}
//	printf("data=%s\r\n",tmp);
//
//	free(tmp);
//	tmp=0;

//	printf("解码完毕\r\n");
	return 1;
}

//成功返回1 失败返回0
int send_encode(unsigned char *senddata,unsigned char *buf){	//发送给客户端的封包不可以加掩码
	unsigned char * eddata=(unsigned char *)malloc(1024);
//	unsigned char * tmpend=(unsigned char *)malloc(1024);
	memset(eddata,0,1024);
//	memset(tmpend,0,1024);

	unsigned long long sedlen=strlen((char*)senddata);
	int i;
	if(0==strcmp((char*)senddata,"0712")){
		eddata[0]=0x88;		//头帧：最后一帧，文本帧
	}else{
		eddata[0]=0x81;		//头帧：最后一帧，文本帧
	}

	if(sedlen<=125){
		eddata[1]=sedlen;
		printf("封包负载数据长度：%d\r\n",sedlen);
		//从tmp[2]开始拼接真实数据
		for(i=0;i<sedlen;i++){
			eddata[i+2]=senddata[i];
		}
	}
	else if(sedlen>125&&sedlen<=65535){
		eddata[1]=126;
		eddata[2]=(unsigned char)((sedlen>>8)&0xff);
		eddata[3]=(unsigned char)sedlen&0xff;

		printf("封包负载数据长度：%d\r\n",sedlen);
		//从tmp[4]开始拼接真实数据
		for(i=0;i<sedlen;i++){
			eddata[i+4]=senddata[i];
		}
	}
	else{
		eddata[1]=127;		//注意：如果需要接收这么大的包，请将缓存区malloc申请内存改大，否则内存溢出
		for(i=0;i<8;i++){
			eddata[i+2]=sedlen>>(8*(7-i));
		}
		printf("超大封包负载数据长度：%d\r\n",sedlen);
		//从tmp[4]开始拼接真实数据
		for(i=0;i<sedlen;i++){
			eddata[i+10]=senddata[i];
		}

	}
		//eddata是否需要加尾巴 \0		？？？
	sedlen=strlen((char*)eddata);
	sprintf((char*)eddata,"%s",(char*)eddata);		//可能会出现问题，排查的重点
	memcpy((char*)buf,(char*)eddata,strlen((const char*)eddata));

	return 1;
}



