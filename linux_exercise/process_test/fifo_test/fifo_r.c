#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

void sys_err(const char *str){
    perror(str);
    exit(1);
}

int main(){
    int fd,ret;
    char buf[4096];
    
    fd=open("testfifo",O_RDONLY);
    if(fd<0){
        sys_err("open");
    }

    while(1){
        ret=read(fd,buf,4096);
        write(STDOUT_FILENO,buf,strlen(buf));
    }

    close(fd);
    return 0;
}