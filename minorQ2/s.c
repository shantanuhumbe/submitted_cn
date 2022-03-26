#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>
#include <poll.h>
#include <arpa/inet.h>
#define SIZE 1024
char *ip = "127.0.0.1";
int nsfds[10];
int I = 0;
void broadcast(char buf[])
{
    printf("broadcasting the message:-");
    printf("%s\n", buf);
    for (int i = 0; i < I; i++)
    {
        send(nsfds[i], buf, strlen(buf) + 1, 0);
    }
}
struct mesg_buffer
{
    long mesg_type;
    char mesg_text[100];
} message;
static struct sigaction siga;
static void multi_handler(int sig, siginfo_t *siginfo, void *context)
{
    int id;
    if (sig == SIGUSR1)
    {
        int c = 0;
        c = fork();
        I--;
        if (c > 0)
        {
        }
        else
        {
            dup2(nsfds[I--], 0);
            char *const parmList[] = {NULL};
            execv("./echos", parmList);
        }
    }

    return;
}
void *connection_handler(void *socket_desc)
{

    int sock = *(int *)socket_desc;
    int n;

    char sendBuff[100], client_message[2000];

    while ((n = recv(sock, client_message, 2000, 0)) > 0)
    {

        printf("%s\n", client_message);
    }
    close(sock);
    return 0;
}
int open_fifo()
{
    int fdr;

    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    fdr = open(myfifo, O_RDONLY);
    return fdr;
}
int open_fifo3()
{
    int fdr;

    char *myfifo = "/tmp/myfifo1";
    mkfifo(myfifo, 0666);
    fdr = open(myfifo, O_RDONLY);
    return fdr;
}
void send_p4()
{
    key_t key;
    int msgid;
    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.mesg_type = 2;
    sprintf(message.mesg_text, "%d", getpid());
    msgsnd(msgid, &message, sizeof(message), 2);
}
int main()
{

    printf("This is server program\n");
    send_p4();
    siga.sa_sigaction = *multi_handler;
    siga.sa_flags |= SA_SIGINFO;
    sigaction(SIGUSR1, &siga, NULL);
    sigaction(SIGUSR2, &siga, NULL);
    struct pollfd pfds[4];
    pfds[0].fd = open_fifo();
    pfds[0].events = POLLIN;
    pfds[1].fd = stdin;
    pfds[1].events = POLLIN;
    pfds[2].fd = open_fifo3();
    pfds[2].events = POLLIN;
    int sfd;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size = sizeof(server_addr);
    char buffer[1024];
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 8091;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sfd, 10);
    int nsfd;
    int c = sizeof(struct sockaddr_in);
    int *new_sock;
    pfds[3].fd = sfd;
    pfds[3].events = POLLIN;
    while (1)
    {

        int ret = poll(pfds, 10, -1);
        if (ret == -1)
        {
            perror("poll");
            exit(1);
        }
        if (pfds[0].revents & POLLIN)
        {
            char buf[100];
            read(pfds[0].fd, buf, 100);
            broadcast(buf);
        }
        if (pfds[1].revents & POLLIN)
        {
            char buf[100];
            read(pfds[1].fd, buf, 100);
            broadcast(buf);
        }
        if (pfds[2].revents & POLLIN)
        {
            char buf[100];
            read(pfds[2].fd, buf, 100);
            broadcast(buf);
        }
        if (pfds[3].revents & POLLIN)
        {
            int nsfd = accept(pfds[3].fd, (struct sockaddr *)&new_addr, &addr_size);
            pthread_t sniffer_thread;
            nsfds[I++] = nsfd;
        }
    }

    return 0;
}