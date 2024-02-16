#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void print(sigset_t *set){
    for(int i=1;i<=32;++i){
        if(sigismember(set,i)==1){
            printf("1");
        }else{
            printf("0");
        }
    }
    printf("\n");
}

int main(){
    sigset_t set,oldset,peset;
    sigemptyset(&set);
    sigaddset(&set,SIGINT);

    int ret;
    ret=sigprocmask(SIG_BLOCK,&set,&oldset);
    if(ret==-1){
        perror("sigprocmask");
        exit(1);
    }

    while(1){
        sigpending(&peset);
        print(&peset);
        sleep(1);
    }
    return 0;
}