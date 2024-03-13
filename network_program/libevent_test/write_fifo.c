#include <stdio.h>
#include <unistd.h>
#include <event2/event.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

void write_cb(evutil_socket_t fd,short what,void *arg){
    char buf[BUFSIZ];
    static int i=0;
    sprintf(buf,"hello world --%d\n",++i);
    write(fd,buf,strlen(buf)+1);

    sleep(1);
}

int main(){
    // int fd=open("myfifo",O_WRONLY|O_NONBLOCK);
    int fd=open("myfifo",O_WRONLY);

    if(fd==-1){
        perror("open error");
        exit(1);
    }

    struct event_base* base=event_base_new();
    
    struct event* ev=event_new(base,fd,EV_WRITE,write_cb,NULL);

    event_add(ev,NULL);

    event_base_dispatch(base);

    event_free(ev);

    event_base_free(base);

    close(fd);
    return 0;
}