#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fd=open("./save.md",O_RDWR|O_CREAT|O_TRUNC,0644);
    if(fd==-1){
        perror("open");
        exit(1);
    }
    dup2(fd,STDOUT_FILENO);
    execlp("ps","ps","aux",NULL);
    return 0;
}