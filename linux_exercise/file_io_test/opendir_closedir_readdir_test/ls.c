#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

int main(int argc,char* argv[]){
    DIR* dir;
    dir=opendir(argv[1]);
    if(dir==NULL){
        perror("opendir error");
        exit(1);
    }
    struct dirent *sdp;

    while((sdp=readdir(dir))!=NULL){
        if(strcmp(sdp->d_name,".")==0 || strcmp(sdp->d_name,"..")==0){
            continue;
        }
        printf("%s\t",sdp->d_name);
    }
    printf("\n");
    closedir(dir);
    return 0;
}