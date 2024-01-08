#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(int argc,char **argv){

    int fd1=open(argv[1],O_RDONLY);
    if(fd1==-1){
        perror("open1 errno\n");
    }

    int fd2=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0777);
    if(fd2==-1){
        perror("open2 errno\n");
    }

    int size;
    char buff[1024];

    while((size=read(fd1,buff,1024))!=0){
        if(size<0){
            perror("read errno\n");
        }
        write(fd2,buff,size);
    }

    close(fd1);
    close(fd2);
    return 0;
}