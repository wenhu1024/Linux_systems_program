#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
    int i;
    pid_t pid,tmpid,wpid;

    for(i=0;i<5;++i){
        pid=fork();
        if(pid==0){
            break;
        }
        if(i==2){
            tmpid=pid;
            printf("----pid get=%d----\n",tmpid);
        }
    }

    if(i==5){
        sleep(5);
        printf("parent, before waitpid\n");
        wpid=waitpid(tmpid,NULL,WNOHANG);
        if(wpid==-1){
            perror("waitpid");
            exit(1);
        }
        printf("parent, finish child=%d\n",wpid);
        printf("parent pid=%d\n",getpid());
    }else{
        sleep(i);
        printf("%d child pid=%d\n",i+1,getpid());
    }

    return 0;
}
