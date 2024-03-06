#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <strings.h>
#include <ctype.h>
#include <sys/wait.h>

#define SER_PORT 9527

void fcn(int signo){
    while((waitpid(0,NULL,WNOHANG|WUNTRACED))>0){;}
    return;
}

int main(){
    int ret,lfd,cfd;
    struct sockaddr_in s_add,c_add;
    socklen_t c_add_len;
    pid_t pid;
    char buf[BUFSIZ];

    bzero(&s_add,sizeof(s_add));
    s_add.sin_family=AF_INET;
    s_add.sin_port=htons(SER_PORT);
    s_add.sin_addr.s_addr=htonl(INADDR_ANY);

    lfd=socket(AF_INET,SOCK_STREAM,0);
    
    bind(lfd,(struct sockaddr*)&s_add,sizeof(s_add));

    listen(lfd,128);

    c_add_len=sizeof(c_add);
    while(1){
        cfd=accept(lfd,(struct sockaddr*)&c_add,&c_add_len);
        
        sigset_t mask,pre;
        sigemptyset(&mask);
        sigaddset(&mask,SIGCHLD);
        sigprocmask(SIG_BLOCK,&mask,&pre);

        pid=fork();

        if(pid<0){
            perror("fork");
            exit(1);
        }else if(pid==0){
            sigprocmask(SIG_SETMASK,&pre,NULL);
            close(lfd);
            for(;;){
                ret=read(cfd,buf,BUFSIZ);
                if(ret==0){
                    close(cfd);
                    exit(1);
                }
                for(int i=0;i<ret;++i){
                    buf[i]=toupper(buf[i]);
                }
                write(cfd,buf,ret);
                write(STDOUT_FILENO,buf,ret);
            }
        }else{
            struct sigaction act;
            act.sa_handler=fcn;
            sigemptyset(&act.sa_mask);
            act.sa_flags=0;
            sigaction(SIGCHLD,&act,NULL);
            close(cfd);
            sigprocmask(SIG_SETMASK,&pre,NULL);
        }

    }
    return 0;
}
