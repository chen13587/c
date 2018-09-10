#include "stdio.h"
#include "string.h"
int main()
{
	int c=0,b=0,a[2][3]={{1,2,3},{4,5,6}};
	int (*p)[3];
	p=a;
	b=sizeof(a)/sizeof(int);
	printf("%p %p\n",a,a+1);
	printf("%p %p\n",a,a[1]+1);
	
	
	printf("%d %d\n",a[1][1],p[1][1]);
	printf("%d %d\n",*(*(a+1)+1),*(*(p+1)+1));


	/*
	for(c=0;c<b;c++){
		printf("%d\n",*(p+c));
		
	}*/
	return 0;

}



