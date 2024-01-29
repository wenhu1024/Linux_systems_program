#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(){

    int fd1=open("t1",O_RDONLY);
    int newfd=dup(fd1);
    printf("newfd: %d\n",newfd);
    return 0;
}