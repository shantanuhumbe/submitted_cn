#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <pthread.h>
#define SIZE 1024
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#define ADDRESS "mysocket"
#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef MSG_CMSG_CLOEXEC
#define MSG_CMSG_CLOEXEC 0
// we explicitly close all file descriptors obtained thought the recv_fd anyway
#endif
char *ip = "127.0.0.1";

int mksfd(int port)
{
    int sfd;
    struct sockaddr_in server_addr;
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
    int port = 9090;
    int e;
    int sfd;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size = sizeof(server_addr);
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    connect(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // printf("connected");
    printf("Enter 1/2/3 for petrol \n Enter 2/3 for diesel \n Enter 3 for gas \n According to the unit D1/D2/D3 you have to go ");
    char buf[10];
    scanf("%s", buf);
    send(sfd, buf, 10, 0);
    recv(sfd, buf, 10, 0);
    send(sfd, buf, 10, 0);
    printf("\ntotal bill was:- %s\n", buf);
    return 0;
}