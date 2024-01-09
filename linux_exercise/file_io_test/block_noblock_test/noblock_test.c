#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define MSG_TRY "try again\n"  
#define MSG_TIMEOUT "time out\n"  

int main(){

    int fd=open("/dev/tty",O_RDONLY|O_NONBLOCK);
    if(fd<0){
        perror("open /dev/tty");
        exit(1);
    }

    printf("open /dev/tty suyccess ,fd=%d\n",fd);

    int n;
    char buff[10];

    int i=0;
    for(;i<5;++i){
        n=read(fd,buff,10);
        if(n>0){
            break;
        }
        if(errno!=EAGAIN){
            perror("read /dev/tty");
            exit(0);
        }else{
            write(STDOUT_FILENO,MSG_TRY,strlen(MSG_TRY));
            sleep(2);
        }
    }

    if(i==5){
        write(STDOUT_FILENO,MSG_TIMEOUT,strlen(MSG_TIMEOUT));
    }else{
        write(STDOUT_FILENO,buff,n);
    }
    return 0;
}


