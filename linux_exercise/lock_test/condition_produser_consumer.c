#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

struct msg
{
    int n;
    struct msg *next;
};

struct msg *head;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hasdata = PTHREAD_COND_INITIALIZER;

void err_thread(int ret, const char *str)
{
    fprintf(stderr, "%s : %s\n", str, strerror(ret));
    exit(1);
}

void *producer(void *arg)
{
    while (1)
    {
        struct msg *mp;
        mp = malloc(sizeof(struct msg));
        mp->n = rand() % 1000 + 1;
        printf("--produce %d\n", mp->n);

        pthread_mutex_lock(&mutex);
        mp->next = head;
        head = mp;
        pthread_mutex_unlock(&mutex);

        pthread_cond_signal(&hasdata);
        sleep(rand() % 3);
    }
    return NULL;
}

void *consumer(void *arg)
{
    while (1)
    {
        struct msg *mp;

        pthread_mutex_lock(&mutex);
        while (head == NULL)
        {
            pthread_cond_wait(&hasdata, &mutex);
        }

        mp = head;
        head = mp->next;

        pthread_mutex_unlock(&mutex);
        printf("---------consumer:%d\n", mp->n);

        free(mp);
        sleep(rand() % 3);
    }
    return NULL;
}

int main()
{
    int ret;
    pthread_t pid, cid;
    srand(time(NULL));

    ret = pthread_create(&pid, NULL, producer, NULL);
    if (ret != 0)
    {
        err_thread(ret, "pthread_create");
    }

    ret = pthread_create(&cid, NULL, consumer, NULL);
    if (ret != 0)
    {
        err_thread(ret, "pthread_create");
    }

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
    return 0;
}