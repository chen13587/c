#include <stdio.h>
#include <stdlib.h>
inline void f(){
	printf("hello");

}

int main(){
	FILE *fp;
	if(NULL!=(fp=fopen("./file","r"))){
		char buf[1024];
		int num;
		fscanf(fp,"%d,%s",&num,&buf);
		printf("num=%d,buf=%s\n",num,buf);

	}else{
		perror("open error");

	}
	return 0;
}
