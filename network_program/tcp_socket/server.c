#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

#define SERV_PORT 9527

int main(){
    int lfd,cfd,ret;
    struct sockaddr_in s_add,c_add;
    socklen_t c_add_len;
    char ip_str[1024];
    char buf[BUFSIZ];

    s_add.sin_family=AF_INET;
    s_add.sin_port=htons(SERV_PORT);
    s_add.sin_addr.s_addr=htonl(INADDR_ANY);

    lfd=socket(AF_INET,SOCK_STREAM,0);
    bind(lfd,(struct sockaddr*)&s_add,sizeof(s_add));
    listen(lfd,128);
    c_add_len=sizeof(c_add);
    cfd=accept(lfd,(struct sockaddr*)&c_add,&c_add_len);

    printf("client ip:%s  port:%d\n",
        inet_ntop(AF_INET,&c_add.sin_addr.s_addr,ip_str,1024)
        ,ntohs(c_add.sin_port)
    );

    while(1){
        ret=read(cfd,buf,BUFSIZ);
        write(STDOUT_FILENO,buf,ret);
        for(int i=0;i<ret;++i){
            buf[i]=toupper(buf[i]);
        }
        write(cfd,buf,ret);        
    }
    return 0;
}