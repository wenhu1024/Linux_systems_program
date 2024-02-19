#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *tfn(void *arg){
    int i=(int)arg;
    sleep(i);
    printf("%d child tid=%lu pid=%d\n",i+1,pthread_self(),getpid());
    return NULL;
}

int main(){
    int i,ret;
    pthread_t tid;

    for(i=0;i<5;++i){
        ret=pthread_create(&tid,NULL,tfn,(void*)i);
    }
    sleep(i);
    printf("main    tid=%lu pid=%d\n",pthread_self(),getpid()); 
    return 0;
}