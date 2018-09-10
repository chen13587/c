#include "stdio.h"
#include "string.h"

void* del_space(char *dat){
	char * tmp, *sat;
	char i;
	sat=dat;	//保存指针首地址
	tmp=dat;

	while(*dat){
		if(*dat!=' '){	//过滤的字符
			*tmp=*dat;
			tmp++;	
			
		}
		
		dat++;
	}
	*tmp='\0';
	printf("sta address= %p\n",&sat);	
	return sat;	
}


int main()
{
	char b[]="      hello sdd  dfe g";
	printf( "%s\n",(char*)del_space(b));
	return 0;

}



