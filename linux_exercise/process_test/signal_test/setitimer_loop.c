#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

void func() { printf("hello\n"); }
int main()
{
    struct itimerval it, oldit;
    signal(SIGALRM, func);

    it.it_interval.tv_sec = 3;
    it.it_interval.tv_usec = 0;
    it.it_value.tv_sec = 2;
    it.it_value.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &it, &oldit);
    if (ret == -1)
    {
        perror("setitimer");
        exit(1);
    }
    while (1)
        ;

    return 0;
}