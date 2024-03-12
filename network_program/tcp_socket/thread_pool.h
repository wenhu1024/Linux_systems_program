#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define SLEEP_TIME 3
#define NUMBER 10

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
    int busyNum;
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

void threadExit(ThreadPool *pool);

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
        pool->busyNum = 0;
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
        pool->queueRear = 0;
        pool->queueFront = 0;
        pool->ququeCapacity = queueCapacity;
        pool->queueSize = 0;

        pool->shutdown = 0;

        pthread_create(&pool->managerID, NULL, manageThread, (void *)pool);
        for (int i = 0; i < min; ++i)
        {
            pthread_create(&pool->threads[i], NULL, workThread, (void *)pool);
        }
        return pool;
    } while (0);

    if (pool && pool->taskQ)
    {
        free(pool->taskQ);
    }
    if (pool && pool->threads)
    {
        free(pool->threads);
    }
    if (pool)
    {
        free(pool);
    }
}

void *workThread(void *arg)
{
    ThreadPool *pool = (ThreadPool *)arg;
    while (1)
    {
        pthread_mutex_lock(&pool->mutexPool);
        while (pool->queueSize == 0 && pool->shutdown == 0)
        {
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

            if (pool->exitNum > 0)
            {
                --pool->exitNum;
                if (pool->liveNum > pool->minNum)
                {
                    --pool->liveNum;
                    pthread_mutex_unlock(&pool->mutexPool);
                    threadExit(pool);
                }
            }
        }

        if (pool->shutdown == 1)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }

        Task task;
        task.function = pool->taskQ[pool->queueFront].function;
        task.arg = pool->taskQ[pool->queueFront].arg;

        pool->queueFront = (pool->queueFront + 1) % pool->ququeCapacity;
        --pool->queueSize;

        pthread_cond_signal(&pool->notFull);
        pthread_mutex_unlock(&pool->mutexPool);

        printf("thread %ld starts work\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        ++pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

        task.function(task.arg);
        free(task.arg); // arg is malloced in main func
        task.arg = NULL;

        printf("thread %ld ends work\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        --pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

void *manageThread(void *arg)
{
    ThreadPool *pool = (ThreadPool *)arg;

    while (pool->shutdown==0)
    {
        sleep(SLEEP_TIME);
        pthread_mutex_lock(&pool->mutexPool);
        int maxNum = pool->maxNum;
        int liveNum = pool->liveNum;
        int queueSize = pool->queueSize;
        pthread_mutex_unlock(&pool->mutexPool);

        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

        if (queueSize > liveNum && liveNum < maxNum)
        {
            int cnt = 0;
            pthread_mutex_lock(&pool->mutexPool);
            for (int i = 0; i < maxNum && cnt < NUMBER && pool->liveNum < pool->maxNum; ++i)
            {
                pthread_create(&pool->threads[i], NULL, workThread, pool);
                ++cnt;
                ++pool->liveNum;
            }

            pthread_mutex_unlock(&pool->mutexPool);
        }

        if (busyNum*2<liveNum && liveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum=NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            
            for(int i=0;i<NUMBER;++i){
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }
    return NULL;
}

void threadExit(ThreadPool *pool)
{
    pthread_t tid = pthread_self();

    pthread_mutex_lock(&pool->mutexPool);

    for (int i = 0; i < pool->maxNum; ++i)
    {
        if (pool->threads[i] == tid)
        {
            printf("thread %ld exit\n", tid);
            pool->threads[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&pool->mutexPool);
    pthread_exit(NULL);
}

int threadBusyNum(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->mutexBusy);
    int busy = pool->busyNum;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busy;
}

int threadAliveNum(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->mutexPool);
    int live = pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);
    return live;
}



int threadPoolDestory(ThreadPool *pool){

    if(pool==NULL){
        return -1;
    }

    pool->shutdown=1;
    pthread_join(pool->managerID,NULL);

    for(int i=0;i<pool->liveNum;++i){
        pthread_cond_signal(&pool->notEmpty);
    }
    if(pool->taskQ){
        free(pool->taskQ);
    }
    if(pool->threads){
        free(pool->threads);
    }

    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    free(pool);

    return 0;
}

void threadPoolAdd(ThreadPool *pool, void (*func)(void *), void *arg){
    pthread_mutex_lock(&pool->mutexPool);
    while(pool->queueSize==pool->ququeCapacity && pool->shutdown==0){
        pthread_cond_wait(&pool->notFull,&pool->mutexPool);
    }
    if(pool->shutdown==1){
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }

    Task task;
    task.arg=arg;
    task.function=func;
    pool->taskQ[pool->queueRear]=task;
    pool->queueRear=(pool->queueRear+1)%pool->ququeCapacity;
    ++pool->queueSize;

    pthread_cond_signal(&pool->notEmpty);
    pthread_mutex_unlock(&pool->mutexPool);
}

#endif