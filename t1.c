#include "stdio.h"
int main()
{
	int a[10][10]={{0}};
	int x=0,y=0,h=0,l=0;
	for(x=0;x<10;x++){
		a[x][0]=1;
		printf("%d",a[x][0]);
		for(y=1;y<=x;y++){
			a[x][y]=a[x-1][y-1]+a[x-1][y];
			printf(" %d",a[x][y]);
		}
		printf("\n");

	}
	


	for(x=0;x<10;x++){
		for(y=0;y<x;y++){
			if(a[h][l]<a[x][y]){
				h=x;l=y;

			}
			
			
		}
	}
	printf("Max Value: %d\n",a[h][l]);
	
	
	return 0;

}







