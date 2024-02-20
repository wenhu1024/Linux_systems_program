#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *tfn(void *arg){
    printf("pthread: pid = %d tid = %lu\n",getpid(),pthread_self());
    return NULL;
}

int main(){
    pthread_t tid;
    printf("main:   pid = %d  tid = %lu\n",getpid(),pthread_self());

    int ret=pthread_create(&tid,NULL,tfn,NULL);
    if(ret!=0){
        fprintf(stderr,"pthread_create errno : %s\n",strerror(ret));
        exit(1);
    }
    sleep(20);
    return 0;
}