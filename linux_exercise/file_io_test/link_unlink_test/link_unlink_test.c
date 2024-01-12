#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main()
{
    int fd;
    fd = open("./tempt.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd<0){
        perror("open");
        exit(1);
    }

    int r = unlink("./tempt.txt");
    if (r < 0)
    {
        perror("unlink");
        exit(1);
    }


    char *p = "just a link-unlink test\n";
    int ret = write(fd, p, strlen(p));
    if (ret < 0)
    {
        perror("write");
        exit(1);
    }
    printf("write success\n");
    p[3]='2';
    printf("enter a character:\n");
    getchar();

    close(fd);
    return 0;
}
