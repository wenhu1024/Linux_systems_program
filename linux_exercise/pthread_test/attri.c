#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void *fcn(void *arg){
    printf("thread...\n");
    pthread_exit(NULL);
}

int main(){
    pthread_attr_t att;
    pthread_t tid;

    pthread_attr_init(&att);
    pthread_attr_setdetachstate(&att,PTHREAD_CREATE_DETACHED);
    pthread_create(&tid,&att,fcn,NULL);
    pthread_attr_destroy(&att);

    sleep(1);
    int ret=pthread_join(tid,NULL);
    if(ret!=0){
        fprintf(stderr,"pthread_join errno : %s\n",strerror(ret));
        exit(1);
    }
    pthread_exit(NULL);
}