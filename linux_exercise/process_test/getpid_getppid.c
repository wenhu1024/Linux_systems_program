#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int main(){   
    pid_t pid=fork();
    if(pid==-1){
        perror("fork error");
        exit(1);
    }else if(pid==0){
        printf("child is created child pid is %d\n",getpid());
    }else if(pid>0){
        printf("parent process: child pid is %d,  my pid is %d,  my parent pid is %d\n",pid,getpid(),getppid());
    }
    return 0;
}