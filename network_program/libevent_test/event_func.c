#include <stdio.h>
#include <event2/event.h>

int main(){
    struct event_base* base=NULL;
    base=event_base_new();

    const char **buf1;
    buf1=event_get_supported_methods();

    for(int i=0;buf1[i]!=NULL;++i){
        printf("%s\n",buf1[i]);
    }
    printf("...\n");

    const char *buf2;
    buf2=event_base_get_method(base);
    printf("%s\n",buf2);
    event_base_free(base);
    return 0;
}
