#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char **argv)
{

    char *a = argv[0];
    int k = atoi(a);
    char str[100];
    char str1[] = "Message from p2 to p1";
    write(k, str1, strlen(str1) + 1);

    return 0;
}