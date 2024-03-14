#include <stdio.h>
#include <stdlib.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_ADD "127.0.0.1"
#define SERV_PORT 8080


void read_terminal_cb(evutil_socket_t fd,short what,void *arg){
    struct bufferevent* bev=(struct bufferevent*)arg;
    char buf[BUFSIZ]={0};
    read(fd,buf,BUFSIZ);
    bufferevent_write(bev,buf,strlen(buf)+1);
}

void write_cb(struct bufferevent* bev,void *arg){
    printf("client write_cb...\n");
}

void read_cb(struct bufferevent* bev,void *arg){
    char buf[BUFSIZ]={0};
    bufferevent_read(bev,buf,BUFSIZ);
    printf("receive server: %s\n",buf);
    sleep(1);
}

int main(){
    struct event_base* base=event_base_new();

    struct sockaddr_in s_add;

    bzero(&s_add,sizeof(s_add));
    s_add.sin_family=AF_INET;
    s_add.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET,IP_ADD,&s_add.sin_addr.s_addr);

    int fd=socket(AF_INET,SOCK_STREAM,0);

    struct bufferevent *bev=bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    
    bufferevent_socket_connect(bev,(struct sockaddr*)&s_add,sizeof(s_add));

    bufferevent_setcb(bev,read_cb,write_cb,NULL,NULL);

    bufferevent_enable(bev,EV_READ);
    
    struct event* ev=event_new(base,STDIN_FILENO,EV_READ|EV_PERSIST,read_terminal_cb,bev);

    event_add(ev,NULL);

    event_base_dispatch(base);

    event_free(ev);
    event_base_free(base);

    return 0;
}