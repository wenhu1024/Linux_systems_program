#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

int main(){
    struct itimerval it,oldit;
    it.it_value.tv_sec=1;
    it.it_value.tv_usec=0;
    it.it_interval.tv_sec=0;
    it.it_interval.tv_usec=0;

    int ret;
    ret=setitimer(ITIMER_REAL,&it,&oldit);
    if(ret==-1){
        perror("setitimer");
        exit(1);
    }
    for(int i=0;;++i){
        printf("%d\t",i);
    }
    return 0;
}
