#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>
#include <sys/stat.h>
#include <pthread.h>
#define SIZE 1024
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#define ADDRESS "mysocket"
char *ip = "127.0.0.1";
int sfd1, sfd2, sock;
void *connection_handler(void *socket_desc)
{

    sock = *(int *)socket_desc;
    int n;

    char sendBuff[100], client_message[2000];

    while ((n = recv(sock, client_message, 2000, 0)) > 0)
    {
        if (strcmp(client_message, "maintainance") == 0)
        {
            raise(SIGUSR1);
            return 0;
        }
        if (strcmp(client_message, "done") == 0)
        {
            raise(SIGUSR2);
            return 0;
        }
        printf("%s\n", client_message);
    }
    close(sock);
    return 0;
}
void handler1()
{

    printf("Connecting to the Alternate server\n");
    pthread_t sniffer_thread;
    int *new_sock = malloc(1);
    *new_sock = sfd2;
    if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
    {
        perror("could not create thread");
    }
}
void handler2()
{

    printf("Switching to the main server\n");
    pthread_t sniffer_thread;
    int *new_sock = malloc(1);
    *new_sock = sfd1;
    if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
    {
        perror("could not create thread");
    }
}
int main()
{
    int port = 9090;
    int e;

    signal(SIGUSR1, handler1);
    signal(SIGUSR2, handler2);
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size = sizeof(server_addr);
    sfd1 = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    connect(sfd1, (struct sockaddr *)&server_addr, sizeof(server_addr));
    port = 9095;
    sfd2 = socket(AF_INET, SOCK_STREAM, 0);
    addr_size = sizeof(server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = port;
    connect(sfd2, (struct sockaddr *)&server_addr, sizeof(server_addr));
    pthread_t sniffer_thread;
    int *new_sock = malloc(1);
    *new_sock = sfd1;
    if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
    {
        perror("could not create thread");
        return 1;
    }

    while (1)
        ;
    return 0;
}