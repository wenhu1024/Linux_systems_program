#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void sys_err(const char *str){
    perror(str);
    exit(1);
}

struct student
{
    int id;
    char name[256];
    int age;
};

int main(){
    int fd,ret,len;
    struct student stu={1,"Tom",20};
    struct student *p=NULL;

    len=sizeof(stu);
    fd=open("test",O_RDWR|O_CREAT|O_TRUNC,0666);
    if(fd==-1){
        sys_err("open");
    }
    ftruncate(fd,len);
    p=mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(p==MAP_FAILED){
        sys_err("mmap");
    }
    close(fd);
    // while(1){
        ++stu.id;
        memcpy(p,&stu,len);
        sleep(10);
    // }

    ret=munmap(p,len);
    if(ret==-1){
        sys_err("munmap");
    }
    return 0;
}


