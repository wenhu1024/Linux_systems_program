#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(){
    int ret;
    pid_t pid;
    int fd[2];
    int i;

    ret=pipe(fd);
    if(ret==-1){
        sys_err("pipe");
    }

    for(i=0;i<2;++i){
        pid=fork();
        if(pid==-1){
            sys_err("fork");
        }else if(pid==0){
            break;
        }
    }

    if(i==2){
        close(fd[0]);
        close(fd[1]);
    }else if(i==1){
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        execlp("ls","ls",NULL);
        sys_err("execlp");
    }else if(i==0){
        close(fd[1]);
        dup2(fd[0],STDIN_FILENO);
        execlp("wc","wc","-l",NULL);
        sys_err("execlp");
    }
    return 0;
}
