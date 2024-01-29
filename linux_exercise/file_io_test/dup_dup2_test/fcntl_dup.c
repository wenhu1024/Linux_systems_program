#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
    int fd=open("t2",O_RDWR);
    printf("fd %d\n",fd);

    int newfd1=fcntl(fd,F_DUPFD,0);
    printf("newfd1 %d\n",newfd1);

    int newfd2=fcntl(fd,F_DUPFD,7);
    printf("newfd2 %d\n",newfd2);

    write(newfd2,"fcntl\n",6);
    return 0;
}