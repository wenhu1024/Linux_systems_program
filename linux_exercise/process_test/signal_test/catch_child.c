#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void catch_child(int signum){
    int status;
    pid_t wpid;
    while((wpid=waitpid(-1,&status,0))!=-1){
        if(WIFEXITED(status)==1){
            printf("catch child pid=%d ret=%d\n",wpid,WEXITSTATUS(status));
        }
        if(WIFSIGNALED(status)==1){
            printf("catch child pid=%d signal=%d\n",wpid,WTERMSIG(status));
        }
    }
    return;
}

int main(){
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);
    sigprocmask(SIG_BLOCK,&set,NULL);

    pid_t pid;
    int i;

    for(i=0;i<15;++i){
        pid=fork();
        if(pid==-1){
            perror("fork");
            exit(1);
        }
        if(pid==0){
            break;
        }
    }

    if(i==15){
        struct sigaction act;
        act.sa_handler=catch_child;
        sigemptyset(&(act.sa_mask));
        act.sa_flags=0;
        sigaction(SIGCHLD,&act,NULL);
        sigprocmask(SIG_UNBLOCK,&set,NULL);
        printf("parent pid=%d\n",getpid());
        while(1);
    }else{
        printf("%d child pid=%d\n",i+1,getpid());
        return i+1;
    }
    return 0;
}