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
    socklen_t c_add_len;

    struct sockaddr_in c_add, s_add;
    s_add.sin_family = AF_INET;
    s_add.sin_port = htons(SERV_PORT);
    s_add.sin_addr.s_addr = htonl(INADDR_ANY);

    char buf[BUFSIZ], str[INET_ADDRSTRLEN];

    int client[FD_SETSIZE];
    for (int i = 0; i < FD_SETSIZE; ++i)
    {
        client[i] = -1;
    }

    lfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bind(lfd, (struct sockaddr *)&s_add, sizeof(s_add));

    listen(lfd, 128);

    fd_set rset, allset;
    FD_ZERO(&allset);
    FD_SET(lfd, &allset);

    int max_index = -1;
    int maxfd = lfd;
    int nready;

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
            printf("Received client port: %d\t ip: %s\n ",
                   ntohs(c_add.sin_port),
                   inet_ntop(AF_INET, &c_add.sin_addr, str, sizeof(str)));
            int index;
            for (index = 0; index < FD_SETSIZE; ++index)
            {
                if (client[index] == -1)
                {
                    client[index] = cfd;
                    break;
                }
            }

            if (index == FD_SETSIZE)
            {
                fputs("too many clients\n", stderr);
                exit(1);
            }
            FD_SET(cfd, &allset);
            if (cfd > maxfd)
            {
                maxfd = cfd;
            }
            if (index > max_index)
            {
                max_index = index;
            }
            if (--nready == 0)
            {
                continue;
            }
        }
        int j;
        for (j = 0; j <= max_index; ++j)
        {
            if ((cfd = client[j]) < 0)
            {
                continue;
            }

            if (FD_ISSET(cfd, &rset))
            {
                ret = read(cfd, buf, BUFSIZ);
                if (ret == 0)
                {
                    close(cfd);
                    FD_CLR(cfd, &allset);
                    client[j] = -1;
                }
                else if (ret > 0)
                {

                    for (int k = 0; k < ret; ++k)
                    {
                        buf[k] = toupper(buf[k]);
                    }
                    write(cfd, buf, ret);
                    write(STDOUT_FILENO, buf, ret);
                    if (--nready == 0)
                    {
                        break;
                    }
                }
            }
        }
    }
    close(lfd);
    return 0;
}