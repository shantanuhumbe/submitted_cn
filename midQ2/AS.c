#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>
#include <pthread.h>
#define SIZE 1024;
char *ip = "127.0.0.1";
int nsfd;
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
void *connection_handler(void *socket_desc)
{

    int sock = *(int *)socket_desc;
    int n;
    char buf[] = "done";
    recv(sock, buf, sizeof(buf), 0);
    send(nsfd, buf, sizeof(buf), 0);
    return 0;
}
int main()
{
    printf("This is the Alternate server\n");
    int port = 9095;
    int sfd;
    struct sockaddr_in new_addr;
    socklen_t addr_size = sizeof(new_addr);
    sfd = mksfd(port);
    nsfd = accept(sfd, (struct sockaddr *)&new_addr, &addr_size);
    port = 9096;
    struct sockaddr_in server_addr;
    addr_size = sizeof(server_addr);
    int sfd1 = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    connect(sfd1, (struct sockaddr *)&server_addr, sizeof(server_addr));
    pthread_t sniffer_thread;
    int *new_sock = malloc(1);
    *new_sock = sfd1;
    if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
    {
        perror("could not create thread");
        return 1;
    }
    if (nsfd == -1)
    {
        perror("accept");
        exit(1);
    }
    while (1)
    {
        char buf[1024];
        scanf("%s", buf);
        send(nsfd, buf, sizeof(buf), 0);
    }
}