#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

void read_dir(const char*name,void(*funct)(const char*)){
    char path[256];
    DIR *dir;
    struct dirent* sdp;

    dir=opendir(name);
    if(dir==NULL){
        perror("opendir errno");
        return;
    }
    while(sdp=readdir(dir)){
        if(strcmp(sdp->d_name,".")==0 || strcmp(sdp->d_name,"..")==0){
            continue;
        }
        sprintf(path,"%s/%s",name,sdp->d_name);
        (*funct)(path);
    }
    closedir(dir);
}

void isFile(const char* name){
    int ret;
    struct stat sb;

    ret=stat(name,&sb);
    if(ret==-1){
        perror("stat error");
        return;
    }
    if(S_ISDIR(sb.st_mode)){
        read_dir(name,isFile);
    }
    printf("%10s\t\t%ld\n",name,sb.st_size);
}

int main(int argc,char * argv[]){
    if(argc==1){
        isFile(".");
    }else{
        isFile(argv[1]);
    }
    return 0;
}