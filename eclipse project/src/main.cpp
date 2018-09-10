#include <stdio.h>
#include <string.h>
#include "server.h"
#include "websocket.h"
#include <stdlib.h>
#include <string>
#include "sys/socket.h"
#include "netinet/in.h"
#include "unistd.h"
#include "sys/types.h"
#include <iostream>
#include "sha1_base64.h"
#include <openssl/sha.h>

int main(){

	 printf("websocket服务器开启...\n");
	 int a=server_start();


	 return 0;
}

