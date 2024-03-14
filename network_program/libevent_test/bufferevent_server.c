#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define SERV_PORT 8080


void read_cb(struct bufferevent* bev,void *arg){
    char buf[BUFSIZ]={0};
    bufferevent_read(bev,buf,BUFSIZ);
    printf("receive client: %s\n",buf);

    bufferevent_write(bev,"123",3);

}

void write_cb(struct bufferevent* bev,void *arg){
    printf("server write_cb...\n");
}

void listener_cb(struct evconnlistener *listener,
                 evutil_socket_t fd,
                 struct sockaddr *addr,
                 int len, void *ptr)
{
    printf("connect a new client\n");
    struct event_base* base=(struct event_base*)ptr;

    struct bufferevent *bev=NULL;

    bev=bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev,read_cb,write_cb,NULL,NULL);

    bufferevent_enable(bev,EV_READ);
}          

int main(){
    struct event_base *base=event_base_new();

    struct sockaddr_in s_add;

    bzero(&s_add,sizeof(s_add));
    s_add.sin_family=AF_INET;
    s_add.sin_port=htons(SERV_PORT);
    s_add.sin_addr.s_addr=htonl(INADDR_ANY);


    struct evconnlistener* listener;
    listener=evconnlistener_new_bind(
        base,listener_cb,base,LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,-1,
        (struct sockaddr*)&s_add,sizeof(s_add));
    
    event_base_dispatch(base);
    
    evconnlistener_free(listener);    
    event_base_free(base);
    return 0;
}