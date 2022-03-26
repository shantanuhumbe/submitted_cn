#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>
#define SIZE 1024;
char *ip = "127.0.0.1";
int mksfd(int port)
{
    int sfd;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[1024];
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sfd, 10);
    return sfd;
}
int main()
{
    printf("This is the main server\n");
    int port = 9090;
    int e;
    int sfd, nsfd;
    struct sockaddr_in new_addr;
    socklen_t addr_size = sizeof(new_addr);
    sfd = mksfd(port);
    nsfd = accept(sfd, (struct sockaddr *)&new_addr, &addr_size);
    sfd = mksfd(9096);
    int nsfd1 = accept(sfd, (struct sockaddr *)&new_addr, &addr_size);
    if (nsfd == -1)
    {
        perror("accept");
        exit(1);
    }
    while (1)
    {
        char buf[1024];
        scanf("%s", buf);
        if (strcmp(buf, "done") == 0)
        {
            printf("Sending server has reapaired message to the alternate server\n");
            send(nsfd1, buf, sizeof(buf), 0);
            continue;
        }
        if (strcmp(buf, "maintainance") == 0)
        {
            printf("Sending server has to be repaired message to the client\n");
        }
        send(nsfd, buf, sizeof(buf), 0);
    }
}