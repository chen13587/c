#include "stdio.h"
#include "string.h"
typedef void (*callback)();

void b(){
	printf("I'm from b\n");
}

void c(){
	printf("I'm from c\n");
}

void a(callback f){
	f();
}
int main()
{
	a(b);
	return 0;

}

