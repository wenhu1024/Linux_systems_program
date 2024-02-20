#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;

void *fcn(void *arg){
    while(1){
        pthread_mutex_lock(&mutex);
        printf("hello ");
        sleep(rand()%3);
        printf("world\n");
        pthread_mutex_unlock(&mutex);
        sleep(rand()%3);
    }
}

int main(){
    srand(time(NULL));
    pthread_t tid;

    pthread_mutex_init(&mutex,NULL);
    
    pthread_create(&tid,NULL,fcn,NULL);

    while(1){
        pthread_mutex_lock(&mutex);
        printf("HELLO ");
        sleep(rand()%3);
        printf("WORLD\n");
        pthread_mutex_unlock(&mutex);
        sleep(rand()%3);
    }
    
    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);
}