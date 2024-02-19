#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *tfn(void *arg){
    printf("pthread: pid = %d tid = %lu\n",getpid(),pthread_self());
    return NULL;
}

int main(){
    pthread_t tid;
    printf("main:   pid = %d  tid = %lu\n",getpid(),pthread_self());

    int ret=pthread_create(&tid,NULL,tfn,NULL);
    if(ret!=0){
        perror("pthread_create");
        exit(1);
    }
    sleep(20);
    return 0;
}