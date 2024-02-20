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



int main(){
    int ret;
    pthread_t tid;
    struct test *pt;

    ret=pthread_create(&tid,NULL,fcn,NULL);
    if(ret!=0){
        fprintf(stderr,"pthread_create errno : %s\n",strerror(ret));
        exit(1);
    }

    ret=pthread_join(tid,(void**)&pt);
    if(ret!=0){
        fprintf(stderr,"pthread_join errno : %s\n",strerror(ret));
        exit(1);
    }

    printf("%d      %s\n",pt->n,pt->str);
    free(pt);
    pthread_exit((void*)0);
}