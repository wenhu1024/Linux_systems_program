#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

#define SERV_PORT 8080

int main(){
    int sockfd,ret;
    char buf[BUFSIZ],str[INET_ADDRSTRLEN];
    struct sockaddr_in c_add,s_add;
    socklen_t c_add_len;

    s_add.sin_family=AF_INET;
    s_add.sin_port=htons(SERV_PORT);
    s_add.sin_addr.s_addr=htonl(INADDR_ANY);

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    bind(sockfd,(struct sockaddr*)&s_add,sizeof(s_add));

    while(1){
        c_add_len=sizeof(c_add);
        ret=recvfrom(sockfd,buf,BUFSIZ,0,(struct sockaddr*)&c_add,&c_add_len);
        if(ret==-1){
            perror("recvfrom error\n");
            exit(-1);
        }
        printf("Receive from %s ad port %d\n",
            inet_ntop(AF_INET,&c_add.sin_addr,str,INET_ADDRSTRLEN),
            ntohs(c_add.sin_port)
        );    
        for(int i=0;i<ret;++i){
            buf[i]=toupper(buf[i]);
        }
        ret=sendto(sockfd,buf,ret,0,(struct sockaddr*)&c_add,c_add_len);
        if(ret==-1){
            perror("sendto error\n");
            exit(-1);          
        }
    }
    close(sockfd);
    return 0;
}