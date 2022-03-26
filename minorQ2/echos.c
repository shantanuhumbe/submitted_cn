#include <stdio.h>
#include <stdlib.h>
int main()
{
    printf("echo server got created\n");
    char buf[256];
    while (1)
    {
        scanf("%s", buf);
        printf("%s", buf);
    }
    return 0;
}