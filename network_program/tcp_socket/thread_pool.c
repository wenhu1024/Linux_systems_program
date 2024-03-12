#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "thread_pool.h"


void function(void *arg){
    int n=*(int*)arg;
    printf("thread %ld is working, number is %d\n",pthread_self(),n);
    sleep(1);
}

int main(){
    ThreadPool *pool=threadPoolCreate(3,10,100);

    for(int i=0;i<100;++i){
        int *num=(int*)malloc(sizeof(int));
        *num=i;
        threadPoolAdd(pool,function,(void*)num);
    }

    sleep(20);
    threadPoolDestory(pool);
    printf("over\n");
    pthread_exit((void*)0);
}