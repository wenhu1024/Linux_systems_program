#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>


int main(int argc,char * argv[]){
    struct stat sb;
    int ret=lstat(argv[1],&sb);
    if(ret==-1){
        perror("stat");
        exit(1);
    }

    if(S_ISREG(sb.st_mode)){
        printf("It is a regular.\n");
    }else if(S_ISDIR(sb.st_mode)){
        printf("It is a dir.\n");
    }else if(S_ISFIFO(sb.st_mode)){
        printf("It is a pipe.\n");
    }else if(S_ISLNK(sb.st_mode)){
        printf("It is a sym link.\n");
    }

    return 0;
}