#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main(){
    int fd=open("./extend.txt",O_RDWR);
    if(fd<0){
        perror("open");
        exit(0);
    }
    lseek(fd,99,SEEK_END);  
    printf("current seek: %ld\n",lseek(fd,0,SEEK_CUR));  
    write(fd,"+",1);
    printf("after writing '+',file size: %ld\n",lseek(fd,0,SEEK_END));  
    close(fd);
    return 0;
}
