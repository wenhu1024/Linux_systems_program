#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SER_PORT 9527


int main(){
    int cfd,ret;
    struct sockaddr_in s_add;
    char buf[BUFSIZ];

    s_add.sin_family=AF_INET;
    s_add.sin_port=htons(SER_PORT);
    inet_pton(AF_INET,"127.0.0.1",&s_add.sin_addr);

    cfd=socket(AF_INET,SOCK_STREAM,0);
    connect(cfd,(struct sockaddr*)&s_add,sizeof(s_add));

    while(1){
        write(cfd,"hello\n",6);
        ret=read(cfd,buf,BUFSIZ);
        write(STDOUT_FILENO,buf,ret);
        sleep(1);
    }

    close(cfd);
    return 0;
}