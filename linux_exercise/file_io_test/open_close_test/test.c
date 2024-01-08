#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(){
    int fd;

    fd=open("./text.txt",O_RDONLY|O_CREAT|O_TRUNC);
    printf("fd=%d errno=%d : %s\n",fd,errno,strerror(errno));

    close(fd);
}
