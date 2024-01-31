#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int i;

    for (i = 0; i < 5; ++i)
    {
        if (fork() == 0)
        {
            sleep(i);
            printf("%d child\n", i + 1);
            return 0;
        }
    }

    if (i == 5)
    {
        sleep(5);
        printf("parent\n");
    }

    return 0;
}