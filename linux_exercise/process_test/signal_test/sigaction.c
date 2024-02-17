#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void func(int signo)
{
    if (signo == SIGINT)
    {
        printf("\tcatch SIGINT\n");
        sleep(3);
    }
    else if (signo == SIGQUIT)
    {
        printf("\tcatch SIGQUIT\n");
    }
    return;
}

int main()
{
    struct sigaction act,oldact;
    act.sa_handler=func;
    sigemptyset(&(act.sa_mask));

    int ret;
    ret=sigaction(SIGINT,&act,&oldact);
    if(ret==-1){
        perror("sigaction");
        exit(1);
    }
    ret=sigaction(SIGQUIT,&act,&oldact);
    if(ret==-1){
        perror("sigaction");
        exit(1);
    }    
    while (1)
    {
        ;
    }

    return 0;
}