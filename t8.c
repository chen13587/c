#include "stdio.h"
#include "string.h"
typedef char (*callback)();

char b(char e){
	return e;
}
void a(callback f){
	printf("%d\n",f);
}
int main(){
	a(b(23));
	return 0;

}
/*
typedef char (*callback)(char,char);

char a(char a1,char a2){
        return a1+a2;
}
int main(){
        callback f;
        f=a;

        printf("a=%d\n",(*f)(4,8));
        return 0;

}
*/
