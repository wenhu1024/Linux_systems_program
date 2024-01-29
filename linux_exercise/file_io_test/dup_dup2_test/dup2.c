#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
    int fd1=open("t1",O_RDWR);
    int fd2=open("t2",O_RDWR);

    int fdret=dup2(fd1,fd2);
    printf("fdret: %d\n",fdret);
    write(fd2,"test\n",5);
    dup2(fd2,STDOUT_FILENO);
    printf("printf\n");
    return 0;
}