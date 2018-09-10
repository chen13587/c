#include "stdio.h"
#include "string.h"
int main()
{
	char a[2][3]={{1,2,3},{4,5,6}};
	char* k[2]={a[0],a[1]};
	char* *p;	
	
	p=k;
	printf("%d\n",*(*(p+1)+1));

	return 0;

}



