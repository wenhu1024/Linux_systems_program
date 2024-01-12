#include <stdio.h>
#include <unistd.h>

int main(){
    int ret=truncate("extend.txt",500);
    return 0;
}