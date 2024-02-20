#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void *fcn(void *arg){
    int i=(int)arg;
    sleep(i);
    printf("%d thread pid=%d tid=%lu\n",i+1,getpid(),pthread_self());
    pthread_exit(NULL);
}



int main(){
    int ret,i;
    pthread_t tid[5];
    int *pi;

    for(i=0;i<5;++i){
        ret=pthread_create(&(tid[i]),NULL,fcn,(void*)i);
        if(ret!=0){
            fprintf(stderr,"pthread_create errno: %s\n",strerror(ret));
            exit(1);
        }
    }

    for(i=0;i<5;++i){
        ret=pthread_join(tid[i],(void**)&pi);
        if(ret!=0){
            fprintf(stderr,"pthread_join errno : %s\n",strerror(ret));
            exit(0);
        }        
        printf("%d thread finished\n",i+1);
    }
    pthread_exit((void*)0);
}