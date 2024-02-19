#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct test{
    int n;
    char str[256];
};

void *fcn(void *arg){
    struct test *t=(struct test*)malloc(sizeof(struct test));
    t->n=100;
    strcpy(t->str,"just test");
    pthread_exit((void*)t);
}

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(){
    int ret;
    pthread_t tid;
    struct test *pt;

    ret=pthread_create(&tid,NULL,fcn,NULL);
    if(ret!=0){
        sys_err("pthread_create");
    }

    ret=pthread_join(tid,(void**)&pt);
    if(ret!=0){
        sys_err("pthread_join");
    }

    printf("%d      %s\n",pt->n,pt->str);
    free(pt);
    pthread_exit((void*)0);
}