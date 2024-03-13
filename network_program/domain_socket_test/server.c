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

int main(){
    int lfd,cfd,ret,len;
    char buf[BUFSIZ];
    struct sockaddr_un s_add,c_add;

    bzero(&s_add,sizeof(s_add));
    s_add.sun_family=AF_UNIX;
    strcpy(s_add.sun_path,SERV_ADD);

    lfd=socket(AF_UNIX,SOCK_STREAM,0);
    len=offsetof(struct sockaddr_un,sun_path)+strlen(s_add.sun_path);

    unlink(SERV_ADD);
    bind(lfd,(struct sockaddr*)&s_add,(socklen_t)len);

    listen(lfd,128);

    while(1){
        len=sizeof(c_add);
        cfd=accept(lfd,(struct sockaddr*)&c_add,(socklen_t*)&len);

        len-=offsetof(struct sockaddr_un,sun_path);
        c_add.sun_path[len]='\0';

        printf("client binds filename is:%s\n",c_add.sun_path);

        while((ret=read(cfd,buf,BUFSIZ))>0){
            printf("ret=%d\n",ret);
            for(int i=0;i<ret;++i){
                buf[i]=toupper(buf[i]);
            }
            write(cfd,buf,ret);
        }
        close(cfd);
    }
    close(lfd);
    return 0;
}
