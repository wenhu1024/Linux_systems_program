#include <stdio.h>
#include <unistd.h>

int main(){
    alarm(1);
    for(int i=0;;++i){
        printf("%d\t",i);
    }
    return 0;
}