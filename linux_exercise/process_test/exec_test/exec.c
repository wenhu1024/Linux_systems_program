#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

int main(){
    pid_t pid;
    printf("start from parent...\n");    
    pid=fork();

    if(pid<0){
        perror("fork");
        exit(1);
    }else if(pid==0){
        execlp("./test.out","./test.out",NULL);
        perror("execlp");
        exit(1);
    }else if(pid>0){
        printf("parent pid:%d\n",getpid());
    }

    return 0;
}