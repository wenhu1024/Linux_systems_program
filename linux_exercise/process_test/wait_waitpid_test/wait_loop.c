#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid,wpid;
    int i;
    for(i=0;i<5;++i){
        pid=fork();
        if(pid==0){
            break;
        }
    }

    if(i==5){
        while((wpid=wait(NULL))!=-1){
            printf("wait child=%d\n",wpid);
        }
    }else{
        sleep(i);
        printf("%d child=%d\n",i+1,getpid());
    }

    return 0;
}