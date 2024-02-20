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
    int ret;
    pthread_t tid;

    ret=pthread_create(&tid,NULL,fcn,NULL);
    if(ret!=0){
        fprintf(stderr,"pthread_create errno : %s\n",strerror(ret));
    }

    ret=pthread_detach(tid);
    if(ret!=0){
        fprintf(stderr,"pthread_detach errno : %s\n",strerror(ret));
    }    

    sleep(1);

    ret=pthread_join(tid,NULL);
    if(ret!=0){
        fprintf(stderr,"pthread_detach errno : %s\n",strerror(ret));
    }       
    return 0;
}