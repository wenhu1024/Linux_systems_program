#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

void sys_err(const char *str){
    perror(str);
    exit(1);
}

int var=1;

int main(){
    int fd,ret,len;
    pid_t pid;
    int *p;

    fd=open("blood_test",O_RDWR|O_CREAT|O_TRUNC,0666);
    if(fd==-1){
        sys_err("open");
    }

    len=20;
    ftruncate(fd,len);
    p=mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(p==MAP_FAILED){
        perror("mmap");
    }
    close(fd);

    pid=fork();
    if(pid==-1){
        sys_err("fork");
    }
    if(pid==0){
        *p=100;
        var=2;
        printf("child: *p=%d  var=%d\n",*p,var);
    }else{
        sleep(1);
        printf("parent: *p=%d  var=%d\n",*p,var);
        wait(NULL);
    }

    ret=munmap(p,len);
    if(ret==-1){
        sys_err("munmap");
    }
    return 0;
}