#include "stdio.h"
#include "string.h"
int main()
{
	char a[]="hello cpl21";
	char b[20];
	int t,i=0;
	for(t=0;t<20;t++){
		b[t]=t;
	}	

	
//	i=sizeof(a)/sizeof(char);	
//	for(i;i>=0;i--){		//倒序输出
//			
//		putchar(a[i]);
//	}
//	putchar("\n");

	
//	i=sizeof(a)/sizeof(char);	//拷贝字符串
//	for(i;i>=0;i--){
//		b[i]=a[i];
//	}

	strcpy(b,a);
	printf("%ld \n",strstr(a,"ell")-a);	//查找字符串在字符串中的位置
	return 0;

}



