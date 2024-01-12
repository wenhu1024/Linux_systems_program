#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main(){
    int fd=open("./test.txt",O_RDWR);
    if(fd<0){
        perror("open");
        exit(0);
    }
    long int length=lseek(fd,0,SEEK_END);
    printf("%ld\n",length);
    close(fd);
    return 0;
}