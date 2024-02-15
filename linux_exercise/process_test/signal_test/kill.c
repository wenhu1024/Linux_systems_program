#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main()
{
    pid_t pid;
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (pid > 0)
    {
        printf("parent pid=%d\n", getpid());
        while (1)
        {
            ;
        }
    }
    else if (pid == 0)
    {

        printf("child pid=%d  ppid=%d\n", getpid(), getppid());
        sleep(2);
        kill(getppid(), SIGKILL);
    }
    return 0;
}