#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    pid_t pid;
    int ret,fd;
    
    pid=fork();
    if(pid>0){
        exit(0);
    }

    pid=setsid();
    if(pid==-1){
        perror("setsid");
        exit(1);
    }

    ret=chdir("/home");
    if(ret==-1){
        perror("chdir");
        exit(1);
    }
    umask(0022);

    close(STDIN_FILENO);
    fd=open("/dev/null",O_RDWR);
    if(fd==-1){
        perror("open");
        exit(1);       
    }
    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);

    while(1);

    return 0;
}