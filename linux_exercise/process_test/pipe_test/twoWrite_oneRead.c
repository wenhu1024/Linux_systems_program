#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(){
    int ret;
    pid_t pid;
    int fd[2];
    int i;
    char buf[1024];

    ret=pipe(fd);
    if(ret==-1){
        sys_err("pipe");
    }

    for(i=0;i<2;++i){
        pid=fork();
        if(pid==-1){
            sys_err("fork");
        }
        if(pid==0){
            break;
        }
    }

    if(i==0){
        close(fd[0]);
        write(fd[1],"1.hello\n",strlen("1.hello\n"));
        close(fd[1]);
    }else if(i==1){
        close(fd[0]);
        write(fd[1],"2.world\n",strlen("2.world\n"));
        close(fd[1]);
    }else{
        close(fd[1]);
        sleep(1);
        ret=read(fd[0],buf,1024);
        write(STDOUT_FILENO,buf,ret);
        close(fd[0]);

        for(i=0;i<2;++i){
            wait(NULL);
        }
    }



    return 0;
}