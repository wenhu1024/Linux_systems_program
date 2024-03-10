#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <ctype.h>

#define SERV_PORT 8080
#define MAX_LEN 10
int main()
{
    int ret, len, lfd, cfd, efd, wait_ret, socketfd;
    char buf[MAX_LEN];

    struct sockaddr_in s_add, c_add;
    socklen_t c_add_len;
    s_add.sin_family = AF_INET;
    s_add.sin_port = htons(SERV_PORT);
    s_add.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = socket(AF_INET, SOCK_STREAM, 0);

    bind(lfd, (struct sockaddr *)&s_add, sizeof(s_add));

    listen(lfd, 128);

    efd = epoll_create(1);

    struct epoll_event event, events[128];
    event.events = EPOLLIN;
    event.data.fd = lfd;

    ret = epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &event);
    if (ret == -1)
    {
        perror("epoll_ctl error");
        exit(1);
    }

    while (1)
    {
        wait_ret = epoll_wait(efd, events, 128, -1);
        for (int i = 0; i < wait_ret; ++i)
        {
            socketfd = events[i].data.fd;
            if (socketfd == lfd)
            {
                c_add_len = sizeof(c_add);
                cfd = accept(lfd, (struct sockaddr*)&c_add, &c_add_len);

                int flg = fcntl(cfd, F_GETFD);
                flg |= O_NONBLOCK;
                fcntl(cfd, F_SETFD, flg);
                
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = cfd;
                ret = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &event);
                if (ret == -1)
                {
                    perror("epoll_ctl error");
                    exit(1);
                }
            }
            else
            {
                socketfd = events[i].data.fd;
                len = read(socketfd,buf,MAX_LEN/2);

                if(len==0){
                    close(socketfd);
                    epoll_ctl(efd,EPOLL_CTL_DEL,socketfd,NULL);
                }else if(len<0){
                    perror("read error");
                    exit(1);
                }else{
                    for(int j=0;j<len;++j){
                        buf[i]=toupper(buf[i]);
                    }
                    write(cfd,buf,len);
                    write(STDOUT_FILENO,buf,len);
                }
            }
        }
    }
    close(lfd);
    return 0;
}