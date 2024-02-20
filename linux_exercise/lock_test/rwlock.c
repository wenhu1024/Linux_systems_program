#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


int cnt;
pthread_rwlock_t rwlock;



void* wr_fcn(void *arg){
    int i=(int)arg;
    int t;
    while(1){
        pthread_rwlock_wrlock(&rwlock);
        t=cnt;
        sleep(1);
        printf("read %d  tid=%lu cnt=%d t=%d\n",i,pthread_self(),++cnt,t);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    return (void*)NULL;  
}

void* rd_fcn(void *arg){
    int i=(int)arg;
    while(1){
        pthread_rwlock_rdlock(&rwlock);
        printf("read %d  tid=%lu cnt=%d\n",i,pthread_self(),cnt);
        pthread_rwlock_unlock(&rwlock);
        sleep(2);
    }
    return (void*)NULL;
}

int main(){
    pthread_t tid[8];
    int i;
    
    pthread_rwlock_init(&rwlock,NULL);

    for(i=0;i<3;++i){
        pthread_create(&(tid[i]),NULL,wr_fcn,(void*)i);
    }

    for(i=0;i<5;++i){
        pthread_create(&(tid[i+3]),NULL,rd_fcn,(void*)i);
    }

    for(i=0;i<8;++i){
        pthread_join(tid[i],NULL);
    }

    pthread_rwlock_destroy(&rwlock);
    
    pthread_exit(NULL);
}