#define __USE_BSD	
#include <sys/socket.h>	
#include <netinet/in.h>	
#include <netinet/ip.h>
#define __FAVOR_BSD	
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <netinet/tcp.h> 
#include <netinet/udp.h> 
#include <netinet/ip_icmp.h> 
#include <netinet/ip.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <signal.h> 
#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <netinet/tcp.h> 
#include <netinet/udp.h> 
#include <netinet/ip_icmp.h>
#include <netinet/ip.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <signal.h> 
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
#endif


#define BUF_SIZE 65536

static void decode_packet(unsigned char *buf, size_t length, FILE *lf);
static void print_content(unsigned char *buf, size_t length, FILE *lf);

void sigint_handler() {
  
    exit(0);
}
#define P 9013	

void send_ump(char datagram[])
{
    int s = socket (PF_INET, SOCK_RAW, IPPROTO_UDP);	
    struct ip *iph = (struct ip *) datagram;
    struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons (9013);
  sin.sin_addr.s_addr = inet_addr ("127.0.0.1");
 
      if (sendto (s,		
		  datagram,	
		  iph->ip_len,	
		  0,		
		  (struct sockaddr *) &sin,	
		  sizeof (sin)) > 0)		
      sendto (s,datagram,	iph->ip_len,0,(struct sockaddr *) &sin,sizeof (sin));

}
void send_fil(char datagram[],int k)
{
    
    if(k%4==0||k%6==0)
    {
        printf("Batsmen hit boundary");
        return;
    }
    k=k/10;
    k=k*10;
    k+=10;
    int s = socket (PF_INET, SOCK_RAW,k);	
    struct ip *iph = (struct ip *) datagram;
    struct sockaddr_in sin;
			
    iph->ip_p=k;
  sin.sin_family = AF_INET;
  sin.sin_port = htons (9013);
  sin.sin_addr.s_addr = inet_addr ("127.0.0.1");
      if (sendto (s,		
		  datagram,	
		  iph->ip_len,	
		  0,		
		  (struct sockaddr *) &sin,	
		  sizeof (sin)) > 0)		
      sendto (s,datagram,	iph->ip_len,0,(struct sockaddr *) &sin,sizeof (sin));

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
    }
    printf("connected\n");

    return usfd;
}
int recv_fd(int socket)
{
    int sent_fd, available_ancillary_element_buffer_space;
    struct msghdr socket_message;
    struct iovec io_vector[1];
    struct cmsghdr *control_message = NULL;
    char message_buffer[1];
    char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
    memset(&socket_message, 0, sizeof(struct msghdr));
    memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));
    io_vector[0].iov_base = message_buffer;
    io_vector[0].iov_len = 1;
    socket_message.msg_iov = io_vector;
    socket_message.msg_iovlen = 1;
    socket_message.msg_control = ancillary_element_buffer;
    socket_message.msg_controllen = CMSG_SPACE(sizeof(int));
    if (recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
    {
        return -1;
    }
    if (message_buffer[0] != 'F')
    {
        return -1;
    }
    if ((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
    {
        return -1;
    }
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

int 
main (void)
{
    printf("This is batsmen\n");
 int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);	


int n = 0 ;
  				
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Warning: Cannot set HDRINCL!\n");

  char datagram[4096];	

  struct ip *iph = (struct ip *) datagram;

  struct tcphdr *tcph = (struct tcphdr *) datagram + sizeof (struct ip);

  struct sockaddr_in sin,sin2; socklen_t len = sizeof(sin2) ;

memset(&sin,0,sizeof(sin)) ;

  sin.sin_family = AF_INET;
  sin.sin_port = htons (P);
  sin.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(s,(struct sockaddr *) &sin,sizeof(sin)) ;
 
   
      if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
        {}
        printf("Received the ball\n");
        printf("Informed to the umpire\n");
        send_ump(datagram);
        int k=25;
        send_fil(datagram,k);
        while(1);
  return 0;
}

