#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
    int status;
    pid_t pid, wpid = 0;
    printf("parent ...\n");
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        printf("child pid=%d\n", getpid());
        sleep(10);
    }
    else if (pid > 0)
    {
        wpid = wait(&status);
        printf("parent pid=%d\twait pid=%d\n", getpid(), wpid);
        if(WIFEXITED(status)){
            printf("child exit with %d\n",WEXITSTATUS(status));
        }
        if(WIFSIGNALED(status)){
            printf("child kill with %d\n",WTERMSIG(status));
        }
    }

    return 0;
}
