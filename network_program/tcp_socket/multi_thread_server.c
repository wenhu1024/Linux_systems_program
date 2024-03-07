#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

#define SERV_PORT 9527

struct s_info{
    struct sockaddr_in c_add;
    int cfd;
};

void *do_work(void *arg){
    struct s_info* info=(struct s_info*)arg;
    char buf[BUFSIZ];
    char str[INET_ADDRSTRLEN];
    int ret,i;

    while(1){
        ret=read(info->cfd,buf,BUFSIZ);
        if (ret == 0) {
            printf("the client %d closed...\n", info->cfd);
            close(info->cfd);
            break;                                            
        }
        printf("received from %s at PORT %d\n",
            inet_ntop(AF_INET,&(info->c_add.sin_addr),str,INET_ADDRSTRLEN),
            ntohs(info->c_add.sin_port)
            );
        
        for(i=0;i<ret;++i){
            buf[i]=toupper(buf[i]);
        }

        write(info->cfd,buf,ret);
        write(STDOUT_FILENO,buf,ret);
    }
    close(info->cfd);

    pthread_exit(NULL); 
}



int main(){
    int lfd,cfd,ret;
    struct sockaddr_in s_add,c_add;
    socklen_t c_add_len;
    struct s_info ts[256];

    s_add.sin_family=AF_INET;
    s_add.sin_port=htons(SERV_PORT);
    s_add.sin_addr.s_addr=htonl(INADDR_ANY);
    
    lfd=socket(AF_INET,SOCK_STREAM,0);
    
    bind(lfd,(struct sockaddr*)&s_add,sizeof(s_add));

    listen(lfd,128);

    printf("Accepting client connect ...\n");

    c_add_len=sizeof(c_add);
    
    int i=0;
    pthread_t tid;
    while(1){
        cfd=accept(lfd,(struct sockaddr*)&c_add,&c_add_len);
        ts[i].c_add=c_add;
        ts[i].cfd=cfd;

        pthread_create(&tid,NULL,do_work,(void *)&ts[i]);
        pthread_detach(tid);

        ++i;
    }

    pthread_exit(NULL);
}