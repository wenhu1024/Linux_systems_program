#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include <string.h>

#define SERV_PORT 8080
#define IP_ADD "127.0.0.1"

int main(){
    int sockfd,ret;
    struct sockaddr_in s_add;
    char buf[BUFSIZ];

    s_add.sin_family=AF_INET;
    s_add.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET,IP_ADD,&s_add.sin_addr);

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    while(fgets(buf,BUFSIZ,stdin)!=NULL){
        ret=sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&s_add,sizeof(s_add));
        if(ret==-1){
            perror("sendto error\n");
            exit(-1);
        }
        ret=recvfrom(sockfd,buf,BUFSIZ,0,NULL,NULL);
        if(ret==-1){
            perror("recvfrom error\n");
            exit(-1);
        }
        write(STDOUT_FILENO,buf,ret);
    }
    close(sockfd);
    return 0;
}