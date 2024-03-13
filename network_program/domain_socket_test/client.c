#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <sys/un.h>
#include <stddef.h>

#define SERV_ADD "server.socket"
#define CLIE_ADD "client.socket"

int main(){
    int cfd,ret,len;
    struct sockaddr_un s_add,c_add;
    char buf[BUFSIZ];

    bzero(&s_add,sizeof(s_add));
    s_add.sun_family=AF_UNIX;
    strcpy(s_add.sun_path,SERV_ADD);

    bzero(&c_add,sizeof(c_add));
    c_add.sun_family=AF_UNIX;
    strcpy(c_add.sun_path,CLIE_ADD);
    len=offsetof(struct sockaddr_un,sun_path)+strlen(c_add.sun_path);

    cfd=socket(AF_UNIX,SOCK_STREAM,0);

    unlink(CLIE_ADD);
    bind(cfd,(struct sockaddr*)&c_add,(socklen_t)len);

    len=offsetof(struct sockaddr_un,sun_path)+strlen(s_add.sun_path);
    connect(cfd,(struct sockaddr*)&s_add,(socklen_t)len);
    while(1){
        while(fgets(buf,BUFSIZ,stdin)!=NULL){
            write(cfd,buf,strlen(buf));
            ret=read(cfd,buf,BUFSIZ);
            write(STDOUT_FILENO,buf,ret);
        }
    }
    close(cfd);
    return 0;
}