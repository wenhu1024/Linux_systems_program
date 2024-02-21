#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 5
sem_t blank_num, product_num;
int queue[NUM];

void *consumer(void *arg)
{
    int i = 0;
    while (1)
    {
        sem_wait(&product_num);
        printf("-Consume---%d\n", queue[i]);
        queue[i] = 0;
        sem_post(&blank_num);

        i = (i + 1) % NUM;
        sleep(rand() % 3);
    }
}

void *produser(void *arg)
{
    int i=0;
    while (1)
    {
        sem_wait(&blank_num);
        queue[i]=rand()%1000+1;
        printf("----Produce---%d\n", queue[i]); 
        sem_post(&product_num);

        i = (i + 1) % NUM;
        sleep(rand() % 1);
    }
}

int main()
{
    pthread_t pid, cid;
    srand(time(NULL));

    sem_init(&blank_num, 0, NUM);
    sem_init(&product_num, 0, 0);

    pthread_create(&pid, NULL, produser, NULL);
    pthread_create(&cid, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    sem_destroy(&blank_num);
    sem_destroy(&product_num);

    return 0;
}