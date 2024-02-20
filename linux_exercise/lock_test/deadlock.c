#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *thread1_function(void *arg) {
    pthread_mutex_lock(&mutex1);
    printf("Thread 1: Acquired mutex1\n");
    sleep(1); // 为了让线程2有足够的时间去获得 mutex2
    printf("Thread 1: Trying to acquire mutex2\n");
    pthread_mutex_lock(&mutex2);
    printf("Thread 1: Acquired mutex2\n");
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}

void *thread2_function(void *arg) {
    pthread_mutex_lock(&mutex2);
    printf("Thread 2: Acquired mutex2\n");
    sleep(1); // 为了让线程1有足够的时间去获得 mutex1
    printf("Thread 2: Trying to acquire mutex1\n");
    pthread_mutex_lock(&mutex1);
    printf("Thread 2: Acquired mutex1\n");
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread1_function, NULL);
    pthread_create(&thread2, NULL, thread2_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
