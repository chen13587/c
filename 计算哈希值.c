#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <string>
#include "unistd.h"
#include "sys/types.h"
//#include <iostream>
#include <openssl/sha.h>

int main(){

//	 printf("websocket服务器开启...\n");
//	 int a=server_start();
//	 printf("a=%d",a);
	#define sha1_len 25
	unsigned char recbuf[sha1_len]={0};
	char outbuf[sha1_len*2]={0};
	char * p="hello world";
	memset(recbuf,0,sha1_len);
	memset(outbuf,0,sha1_len*2);
	SHA1((const unsigned char*)p,strlen(p),recbuf);
	for(int i=0;i<sha1_len;i++){
		sprintf(&outbuf[i*2],"%02x",(unsigned int)recbuf[i]);
	}
	printf("%s",outbuf);






	 return 0;
}


