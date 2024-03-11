#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

typedef struct Task
{
    void (*function)(void *arg);
    void *arg;
} Task;

struct ThreadPool
{
    Task *taskQ;
    int ququeCapacity;
    int queueSize;
    int queueRear;
    int queueFront;

    pthread_mutex_t mutexPool;
    pthread_mutex_t mutexBusy;
    pthread_t managerID;
    pthread_t *threads;
    int maxNum;
    int minNum;
    int liveNum;
    int busyNUm;
    int exitNum;

    pthread_cond_t notEmpty;
    pthread_cond_t notFull;

    int shutdown;
};

typedef struct ThreadPool ThreadPool;

ThreadPool *threadPoolCreate(int min, int max, int queueCapacity);

int threadPoolDestory(ThreadPool *pool);

void threadPoolAdd(ThreadPool *pool, void (*func)(void *), void *arg);

int threadBusyNum(ThreadPool *pool);

int threadAliveNum(ThreadPool *pool);

void *workThread(void *arg);

void *manageThread(void *arg);

ThreadPool *threadPoolCreate(int min, int max, int queueCapacity)
{
    ThreadPool *pool = NULL;
    do
    {
        if ((pool = (ThreadPool *)malloc(sizeof(ThreadPool))) == NULL)
        {
            printf("malloc threadpool failed\n");
            break;
        }

        if ((pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * max)) == NULL)
        {
            printf("malloc threads failed\n");
            break;
        }
        pool->maxNum = max;
        pool->minNum = min;
        pool->exitNum = 0;
        pool->liveNum = min;
        pool->busyNUm = 0;
        if (pthread_mutex_init(&pool->mutexBusy, NULL) != 0 ||
            pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
            pthread_cond_init(&pool->notEmpty, NULL) != 0 ||
            pthread_cond_init(&pool->notFull, NULL) != 0)
        {
            printf("init mutex or cond failed\n");
            break;
        }

        if ((pool->taskQ = (Task *)malloc(sizeof(Task) * queueCapacity)) == NULL)
        {
            printf("malloc tasks failed\n");
            break;
        }
        pool->queueRear=0;
        pool->queueFront=0;
        pool->ququeCapacity=queueCapacity;
        pool->queueSize=0;

        pool->shutdown=0;

        pthread_create(&pool->managerID,NULL,manageThread,(void*)pool);
        for(int i=0;i<min;++i){
            pthread_create(&pool->threads[i],NULL,workThread,(void*)poll);
        }
        return pool;
    } while (0);
    
    if(pool&&pool->taskQ){
        free(pool->taskQ);
    }
    if(pool&&pool->threads){
        free(pool->threads);
    }
    if(pool){
        free(pool);
    }
}

#endif