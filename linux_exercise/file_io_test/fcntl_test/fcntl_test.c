#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define MSG_TRY "try again\n" 

int main()
{

    char buff[10];
    int n;

    int flags = fcntl(STDIN_FILENO, F_GETFL);
    if (flags == -1)
    {
        perror("fcntl");
        exit(0);
    }
    flags |= O_NONBLOCK;
    int res = fcntl(STDIN_FILENO, F_SETFL, flags);
    if (res == -1)
    {
        perror("fcntl");
        exit(0);
    }

    try : 
    
    n = read(STDIN_FILENO, buff, 10);
    if (n < 0)
    {
        if(errno!=EAGAIN){
            perror("read /dev/tty");
            exit(1);
        }else{
            sleep(2);
            write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
            goto try;
        }
        
    }
    write(STDOUT_FILENO, buff, n);
    return 0;
}