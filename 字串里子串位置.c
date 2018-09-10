#include <stdio.h>
#include <string.h>




int main(){

	 char *p="hello cpl";
	 const char *n=0;
	 n=strstr(p,"cpl");
	 int a=n-p;
	 printf("n=%d\n",a);


	return 0;
}


