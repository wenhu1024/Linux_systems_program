#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    int fd;
    fd=open("./test.txt",O_RDWR|O_CREAT,0777);
    if(fd<0){
        perror("open test.txt");
        exit(1);
    }
    char ch;
    int size;
    const char *p="just a lseek test\n";
    size=write(fd,p,strlen(p));
    if(size<0){
        perror("write string");
        exit(1);
    }
    
    lseek(fd,0,SEEK_SET);

    while((size=read(fd,&ch,1))>0){
        if(size<0){
            perror("read test.txt");
            exit(1);
        }
        write(STDOUT_FILENO,&ch,size);
    }
    close(fd);
    return 0;
}