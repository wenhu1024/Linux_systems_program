#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(){
    int fd[2];
    pid_t pid;
    int ret;
    char buff[1024];

    ret=pipe(fd);
    if(ret==-1){
        sys_err("pipe");
    }

    pid=fork();
    if(pid>0){
        close(fd[0]);
        write(fd[1],"test\n",5);
        close(fd[1]);
    }else if(pid==0){
        close(fd[1]);
        ret=read(fd[0],buff,1024);
        printf("read %d sizes\n",ret);
        write(STDOUT_FILENO,buff,ret);
        close(fd[0]);
    }
    return 0;
}