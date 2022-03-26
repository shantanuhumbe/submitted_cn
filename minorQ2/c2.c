#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#define SIZE 1024
void *connection_handler(void *socket_desc)
{

    int sock = *(int *)socket_desc;
    int n;

    char sendBuff[100], client_message[2000];

    while ((n = recv(sock, client_message, 2000, 0)) > 0)
    {

        printf("%s\n", client_message);
        bzero(client_message, sizeof(client_message));
    }
    close(sock);
    return 0;
}
int main()
{
    printf("This is client2\n");
    char *ip = "127.0.0.1";
    int port = 8091;
    int e;

    int sfd;
    struct sockaddr_in server_addr;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    connect(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    char buf[100];
    int *new_sock;
    pthread_t sniffer_thread;
    new_sock = malloc(1);
    *new_sock = sfd;
    if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
    {
        perror("could not create thread");
        return 1;
    }
    while (1)
    {
        scanf("%s", buf);
        send(sfd, buf, sizeof(buf) + 1, 0);
        bzero(buf, sizeof(buf));
    }
    close(sfd);
    return 0;
}