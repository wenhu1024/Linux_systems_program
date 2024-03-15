#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/stat.h>

#define MAXSIZE 2048

int init_listener_fd(int port, int efd);

void epoll_run(int port);

void do_accept(int lfd, int efd);

void do_read(int cfd, int efd);

int get_line(int cfd, char *buf, int size);

void disconnect(int cfd,int efd);

void http_request(int cfd,const char* file);

void send_file(int cfd,const char *file);

void send_respond(int cfd, int no, char *disp, char *type, int len);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("./server port path\n");
    }

    int port = atoi(argv[1]);

    int ret = chdir(argv[2]);
    if (ret == -1)
    {
        perror("chdir error");
        exit(1);
    }

    epoll_run(port);

    return 0;
}

int init_listener_fd(int port, int efd)
{
    struct epoll_event ev;
    struct sockaddr_in s_add;

    bzero(&s_add, sizeof(s_add));
    s_add.sin_family = AF_INET;
    s_add.sin_port = htons(port);
    s_add.sin_addr.s_addr = htonl(INADDR_ANY);

    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt=1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    if (lfd == -1)
    {
        perror("socket error");
        exit(1);
    }

    int ret;

    ret = bind(lfd, (struct sockaddr *)&s_add, sizeof(s_add));
    if (ret == -1)
    {
        perror("bind error");
        exit(1);
    }

    ret = listen(lfd, 128);
    if (ret == -1)
    {
        perror("listen error");
        exit(1);
    }

    ev.data.fd = lfd;
    ev.events = EPOLLIN;

    ret = epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &ev);
    if (ret == -1)
    {
        perror("epoll_ctl error");
        exit(1);
    }

    return lfd;
}
void do_accept(int lfd, int efd)
{
    int cfd;

    struct sockaddr_in c_add;
    bzero(&c_add, sizeof(c_add));
    socklen_t c_add_len = sizeof(c_add);

    cfd = accept(lfd, (struct sockaddr *)&c_add, &c_add_len);
    if (cfd == -1)
    {
        perror("accept error");
        exit(1);
    }

    char str[64] = {0};
    printf("New client IP:%s, port:%d, cfd=%d\n",
           inet_ntop(AF_INET, &c_add.sin_addr.s_addr, str, sizeof(str)),
           ntohs(c_add.sin_port),
           cfd);

    struct epoll_event ev;
    ev.data.fd = cfd;
    ev.events = EPOLLIN | EPOLLET;

    int flg = fcntl(cfd, F_GETFL);
    flg |= O_NONBLOCK;
    fcntl(cfd, F_SETFL, flg);

    int ret = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);
    if (ret == -1)
    {
        perror("epoll_ctl error");
        exit(1);
    }
}

void epoll_run(int port)
{
    int efd = epoll_create(MAXSIZE);
    if (efd == -1)
    {
        perror("epoll_create error");
        exit(1);
    }

    int lfd = init_listener_fd(port, efd);

    int ret;
    struct epoll_event *ev, events[MAXSIZE];
    while (1)
    {
        ret = epoll_wait(efd, events, MAXSIZE, -1);
        if (ret < 0)
        {
            perror("epoll_wait error");
            exit(1);
        }

        for (int i = 0; i < ret; ++i)
        {
            ev = &events[i];
            if (!(ev->events & EPOLLIN))
            {
                continue;
            }
            if (ev->data.fd == lfd)
            {
                do_accept(lfd, efd);
            }
            else
            {
                do_read(ev->data.fd, efd);
            }
        }
    }
}

int get_line(int cfd, char *buf, int size)
{
    int index = 0;
    int ret;
    char c = '\0';

    while ((index < size - 1) && (c != '\n'))
    {
        ret = recv(cfd, &c, 1, 0);
        if (ret > 0)
        {
            if (c == '\r')
            {
                ret = recv(cfd, &c, 1, MSG_PEEK);
                if (ret > 0 && c == '\n')
                {
                    recv(cfd, &c, 1, 0);
                }
                else
                {
                    c = '\r';
                }
            }
            buf[index++] = c;
        }
        else
        {
            c = '\n';
        }
    }
    buf[index]='\0';
    return (ret == -1 ? ret : index);
}

void disconnect(int cfd,int efd){
    int ret=epoll_ctl(efd,EPOLL_CTL_DEL,cfd,NULL);
    if(ret!=0){
        perror("epoll_ctl error");
        exit(1);
    }
    close(cfd);
}

void do_read(int cfd, int efd){
    char line[1024];
    char method[16],path[256],protocol[16];
    int len=get_line(cfd,line,sizeof(line));

    if(len==0){
        printf(">>>server find client closed\n");
        disconnect(cfd,efd);
    }else{
        sscanf(line,"%[^ ] %[^ ] %[^ ]",method,path,protocol);
        printf("method=%s, path=%s, protocol=%s\n",method,path,protocol);
    }

    while(1){
        char buf[1024]={0};
        len=get_line(cfd,buf,sizeof(buf));
        if(len==-1){
            break;
        }
    }

    if(strncasecmp(method,"GET",3)==0){
        char *file=path+1;
        http_request(cfd,file);
        disconnect(cfd,efd);
    }
}


void http_request(int cfd,const char* file){
    struct stat sbuf;
    
    int ret=stat(file,&sbuf);
    
    if(ret!=0){
        // 404
        perror("stat error");
        exit(1); // 后面修改
    }

    if(S_ISREG(sbuf.st_mode)){
        send_respond(cfd, 200, "OK", " Content-Type: text/plain; charset=iso-8859-1", sbuf.st_size);	 
		//send_respond(cfd, 200, "OK", "Content-Type:image/jpeg", -1);
		//send_respond(cfd, 200, "OK", "audio/mpeg", -1);
        send_file(cfd,file);
    }
}


void send_respond(int cfd, int no, char *disp, char *type, int len){

	char buf[4096] = {0};
	
	sprintf(buf, "HTTP/1.1 %d %s\r\n", no, disp);
	send(cfd, buf, strlen(buf), 0);
	
	sprintf(buf, "Content-Type: %s\r\n", type);
	sprintf(buf+strlen(buf), "Content-Length:%d\r\n", len);
	send(cfd, buf, strlen(buf), 0);
	
	send(cfd, "\r\n", 2, 0);
}

void send_file(int cfd,const char *file){
    int fd=open(file,O_RDONLY);
    if(fd==-1){
        // 404
        perror("open error");
        exit(1);
    }

    int ret,n;
    char buf[4096];

    while((n=read(fd,buf,sizeof(buf)))>0){
        ret=send(cfd,buf,n,0);
        if(ret==-1){
            perror("send error");
            exit(1);
        }
        if(ret<4096){
            printf(">>>send %d byte(s)\n",ret);
        }
    }

    close(fd);
}