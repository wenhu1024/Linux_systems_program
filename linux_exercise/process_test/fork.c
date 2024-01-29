#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    printf("before fork \n");
    
    pid_t pid=fork();
    if(pid==-1){
        perror("fork error");
        exit(1);
    }else if(pid==0){
        printf("child is created\n");
    }else if(pid>0){
        printf("parent process: child pid is %d\n",pid);
    }

    printf("end...\n");
    return 0;
}