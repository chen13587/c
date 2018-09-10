#include "stdio.h"
#include "string.h"
int main()
{

	int h=0,l=0,x=0,y=0,sum=0;
	char a[2][3]={{1,2,3},{4,5,6}};
	char *p[2];
	p[0]=a[0];
	p[1]=a[1];	
	h=sizeof(a)/sizeof(a[0]);
	l=sizeof(a[0]);
	printf("h=%d,l=%d\n",h,l);	
	for(x=0;x<h;x++){
		for(y=0;y<l;y++){
			sum+=*(*(p+x)+y);
		}
	}
	printf("%d\n",sum);

	return 0;

}



