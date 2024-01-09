#include <unistd.h>
#include <stdio.h>

extern int optind;
extern int opterr;
extern char *optarg;
extern int optopt;

int main(int argc, char *argv[])
{
    printf("%s\n", "          initial status");
    printf("argc=%d optind=%d opterr=%d\n", argc, optind, opterr);
    for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]=%s\n", i, argv[i]);
    }
    printf("\n");

    printf("%s\n", "          call getopt");
    int ch_opt;
    while ((ch_opt = getopt(argc, argv, "a:b::c")) != -1)
    {
        printf("optind=%d\n", optind);
        switch (ch_opt)
        {
        case 'a':
            printf("opt:-a\n");
            printf("arg:%s\n\n", optarg);
            break;
        case 'b':
            printf("opt:-b\n");
            printf("arg:%s\n\n", optarg);
            break;
        case 'c':
            printf("opt:-a\n\n");
            break;
        default:
            printf("Unknown option: %c\n\n",(char)optopt);
            break;
        }
    }

    printf("%s\n", "          finally status");
    printf("argc=%d optind=%d opterr=%d\n", argc, optind, opterr);
    for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]=%s\n", i, argv[i]);
    } 
    return 0;
}
