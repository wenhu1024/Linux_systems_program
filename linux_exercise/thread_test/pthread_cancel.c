#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void *fcn1(void *arg)
{
    printf("1 thread return\n");
    return (void *)(1);
}

void *fcn2(void *arg)
{
    printf("2 thread exit\n");
    pthread_exit((void *)2);
}

void *fcn3(void *arg)
{
    while (1)
    {
        // printf("3 thread\n");
        // sleep(1);
        pthread_testcancel();
    }
}

int main()
{
    int ret;
    pthread_t tid;
    void *res;

    ret = pthread_create(&tid, NULL, fcn1, NULL);
    pthread_join(tid,&res);
    printf("1 thread exit with %d\n", (int)res);

    ret = pthread_create(&tid, NULL, fcn2, NULL);
    pthread_join(tid,&res);
    printf("2 thread exit with %d\n", (int)res);

    pthread_create(&tid, NULL, fcn3, NULL);
    sleep(3);
    pthread_cancel(tid);
    pthread_join(tid,&res);
    printf("3 thread exit with %d\n", (int)res);

    return 0;
}