#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc,char *argv[]){
    pid_t pid;
    printf("start from parent...\n");    
    pid=fork();

    if(pid<0){
        perror("fork");
        exit(1);
    }else if(pid==0){
        char *new_argv[4] = {"ls", "-l", "-h", NULL};
        execvp("ls",new_argv);
        perror("execvp");
        exit(1);
    }else if(pid>0){
        printf("parent pid:%d\n",getpid());
    }
}