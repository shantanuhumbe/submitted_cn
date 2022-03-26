#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
int main()
{

    printf("This is process p2\n");
    char buf[100];
    int fdr;
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    fdr = open(myfifo, O_RDWR);

    while (1)
    {
        scanf("%s", buf);
        char but[100];
        write(fdr, buf, strlen(buf) + 1);
    }
    return 0;
}