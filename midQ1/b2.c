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
char *ip = "127.0.0.1";
#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef MSG_CMSG_CLOEXEC
#define MSG_CMSG_CLOEXEC 0
// we explicitly close all file descriptors obtained thought the recv_fd anyway
#endif
int send_fd(int socket, int fd_to_send)
{
    struct msghdr socket_message;
    struct iovec io_vector[1];
    struct cmsghdr *control_message = NULL;
    char message_buffer[1];
    /* storage space needed for an ancillary element with a paylod of
   length is CMSG_SPACE(sizeof(length)) */
    char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
    int available_ancillary_element_buffer_space;
    /* at least one vector of one byte must be sent */
    message_buffer[0] = 'F';
    io_vector[0].iov_base = message_buffer;
    io_vector[0].iov_len = 1;
    /* initialize socket message */
    memset(&socket_message, 0, sizeof(struct msghdr));
    socket_message.msg_iov = io_vector;
    socket_message.msg_iovlen = 1;
    /* provide space for the ancillary data */
    available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
    memset(ancillary_element_buffer, 0,
           available_ancillary_element_buffer_space);
    socket_message.msg_control = ancillary_element_buffer;
    socket_message.msg_controllen =
        available_ancillary_element_buffer_space;
    /* initialize a single ancillary data element for fd passing */
    control_message = CMSG_FIRSTHDR(&socket_message);
    control_message->cmsg_level = SOL_SOCKET;
    control_message->cmsg_type = SCM_RIGHTS;
    control_message->cmsg_len = CMSG_LEN(sizeof(int));
    *((int *)CMSG_DATA(control_message)) = fd_to_send;
    return sendmsg(socket, &socket_message, 0);
}
int recv_fd(int socket)
{
    int sent_fd, available_ancillary_element_buffer_space;
    struct msghdr socket_message;
    struct iovec io_vector[1];
    struct cmsghdr *control_message = NULL;
    char message_buffer[1];
    char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
    /* start clean */
    memset(&socket_message, 0, sizeof(struct msghdr));
    memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));
    /* setup a place to fill in message contents */
    io_vector[0].iov_base = message_buffer;
    io_vector[0].iov_len = 1;
    socket_message.msg_iov = io_vector;
    socket_message.msg_iovlen = 1;
    /* provide space for the ancillary data */
    socket_message.msg_control = ancillary_element_buffer;
    socket_message.msg_controllen = CMSG_SPACE(sizeof(int));
    if (recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
    {
        // printf("\n1\n");
        return -1;
    }
    // printf("%s", message_buffer);
    if (message_buffer[0] != 'F')
    {
        /* this did not originate from the above function */
        // printf("\n2\n");
        return -1;
    }
    if ((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
    {

        /* we did not provide enough space for the ancillary element array */
        // printf("\n3\n");
        return -1;
    }
    /* iterate ancillary elements */
    for (control_message = CMSG_FIRSTHDR(&socket_message);
         control_message != NULL;
         control_message = CMSG_NXTHDR(&socket_message, control_message))
    {
        if ((control_message->cmsg_level == SOL_SOCKET) &&
            (control_message->cmsg_type == SCM_RIGHTS))
        {
            sent_fd = *((int *)CMSG_DATA(control_message));
            return sent_fd;
        }
    }
    return -1;
}

int mkusfd_server()
{
    int usfd;
    struct sockaddr_un userv_addr, ucli_addr;
    int userv_len, ucli_len;
    usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    // perror("socket");
    // printf("%d", usfd);
    bzero(&userv_addr, sizeof(userv_addr));
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, ADDRESS);
    unlink(ADDRESS);
    userv_len = sizeof(userv_addr);
    bind(usfd, (struct sockaddr *)&userv_addr, userv_len);
    listen(usfd, 5);
    ucli_len = sizeof(ucli_addr);
    socklen_t addr_size = sizeof(ucli_addr);
    int nusfd = accept(usfd, (struct sockaddr *)&ucli_addr, &addr_size);
    return nusfd;
}
int mkusfd_client()
{
    int usfd;
    struct sockaddr_un userv_addr;
    int userv_len, ucli_len;
    usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    bzero(&userv_addr, sizeof(userv_addr));
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, ADDRESS);
    userv_len = sizeof(userv_addr);
    while (connect(usfd, (struct sockaddr *)&userv_addr, userv_len) == -1)
    {
        // perror("\n connect ");
        // return 0;
    }
    printf("connected\n");
    return usfd;
}
void *connection_handler(void *socket_desc)
{

    int sock = *(int *)socket_desc;
    int n;

    char client_message[2000];

    while ((n = recv(sock, client_message, 2000, 0)) > 0)
    {
        printf("%s\n", client_message);
        bzero(client_message, sizeof(client_message));
    }
    close(sock);
    return 0;
}
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

    printf("This is a billing desk b2\n");
    char arr1[100];
    int fd;
    char myfifo[] = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_WRONLY);

    // accepting customer as client
    int usfd = mkusfd_server();
    while (1)
    {

        int sfd1 = recv_fd(usfd);
        char buf[10];
        recv(sfd1, buf, 10, 0);
        printf("bill received:\n");
        printf("%s\n", buf);
        write(fd, buf, 10);
        close(sfd1);
    }
    return 0;
}