#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <event2/event.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void read_cb(evutil_socket_t fd,short what,void *arg){
    char buf[BUFSIZ]={0};
    int len=read(fd,buf,BUFSIZ);

    printf("read event ?  %s\n",(EV_READ & what ? "Yes":"No"));
    printf("data len=%d   buf=%s\n",len,buf);

    sleep(1);
}

int main(){
    unlink("myfifo");
    mkfifo("myfifo",0644);

    // int fd=open("myfifo",O_RDONLY|O_NONBLOCK);
    int fd=open("myfifo",O_RDONLY);
    if(fd==-1){
        perror("open error");
        exit(1);
    }

    struct event_base* base=NULL;
    base=event_base_new();

    struct event* ev=NULL;
    ev=event_new(base,fd,EV_READ|EV_PERSIST,read_cb,NULL);

    event_add(ev,NULL);

    event_base_dispatch(base);

    event_free(ev);
    event_base_free(base);
    close(fd);
    return 0;
}