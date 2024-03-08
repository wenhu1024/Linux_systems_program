#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>

#define SERV_PORT 8080

int main()
{
    int ret, lfd, cfd;
    char buf[BUFSIZ];

    struct sockaddr_in s_add, c_add;
    socklen_t c_add_len;

    s_add.sin_family = AF_INET;
    s_add.sin_port = htons(SERV_PORT);
    s_add.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = socket(AF_INET, SOCK_STREAM, 0);

    bind(lfd, (struct sockaddr *)&s_add, sizeof(s_add));

    listen(lfd, 128);

    fd_set rset, allset;
    FD_ZERO(&allset);
    FD_SET(lfd, &allset);
    int nready, maxfd;
    maxfd = lfd;

    while (1)
    {
        rset = allset;

        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (nready < 0)
        {
            perror("select error");
            exit(1);
        }

        if (FD_ISSET(lfd, &rset))
        {
            c_add_len = sizeof(c_add);

            cfd = accept(lfd, (struct sockaddr *)&c_add, &c_add_len);
            FD_SET(cfd, &allset);

            if (cfd > maxfd)
            {
                maxfd = cfd;
            }

            if (--nready == 0)
            {
                continue;
            }
        }

        for (int i = lfd + 1; i <= maxfd; ++i)
        {
            if (FD_ISSET(i, &rset))
            {
                ret = read(i, buf, BUFSIZ);
                if (ret == 0)
                {
                    close(cfd);
                    FD_CLR(cfd, &allset);
                    break;
                }
                else if(ret>0)
                {
                    for (int j = 0; j < ret; ++j)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    write(cfd, buf, ret);
                    write(STDOUT_FILENO, buf, ret);
                }
            }
        }
    }

    close(lfd);
    return 0;
}