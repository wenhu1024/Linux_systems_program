#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void func(int signo){
    printf("hello,signo=%d\n",signo);
}

int main(){
    signal(SIGINT,func);
    while(1);
    return 0;
}