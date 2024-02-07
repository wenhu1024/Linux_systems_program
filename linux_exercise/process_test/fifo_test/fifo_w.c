#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(){
    int fd,ret;
    char buf[4096];

    fd=open("testfifo",O_WRONLY);
    if(fd<0){
        sys_err("open");
    }

    int i=0;
    while(1){
        sprintf(buf,"test id = %d\n",i++);
        ret=write(fd,buf,strlen(buf));
        sleep(1);
    }
    close(fd);
    return 0;
}
