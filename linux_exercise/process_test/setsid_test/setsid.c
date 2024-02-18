#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    pid_t pid;
    pid=fork();
    if(pid==-1){
        perror("fork");
        exit(1);
    }

    if(pid==0){
        printf("child ppid= %d\n",getppid());
        printf("child pid = %d\n",getpid());
        printf("child pgid= %d\n",getpgid(0));
        printf("child sid = %d\n",getsid(0));
        sleep(10);

        setsid();
        printf("child ppid= %d\n",getppid());
        printf("child pid = %d\n",getpid());
        printf("child pgid= %d\n",getpgid(0));
        printf("child sid = %d\n",getsid(0));
        sleep(10);    
    }
    return 0;
}