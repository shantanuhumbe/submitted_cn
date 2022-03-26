#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#define MAX 10
struct mesg_buffer
{
    long mesg_type;
    char mesg_text[100];
} message;
int main()
{
    printf("This is process p4\n");
    key_t key;
    int msgid;
    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    while (message.mesg_type != 2)
    {
        msgrcv(msgid, &message, sizeof(message), 2, 2);
    }

    int pid = atoi(message.mesg_text);
    printf("enter 1 if you want to send the signal\n");
    while (1)
    {
        int k;
        scanf("%d", &k);
        kill(pid, SIGUSR1);
        printf("signal sent\n");
    }

    return 0;
}
