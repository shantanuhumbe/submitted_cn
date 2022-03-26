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
#define BUF_SIZE 65536
static void decode_packet(unsigned char *buf, size_t length, FILE *lf);
static void print_content(unsigned char *buf, size_t length, FILE *lf);
void sigint_handler() {
    exit(0);
}
#define P 9013	
int main (void)
{
 printf("This is umpire\n");
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
 
    int bowltime,hittime;

    //receive from bowler
      if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
	printf ("error\n");
         struct iphdr *ip_head = (struct iphdr*)datagram;
         bowltime=ip_head->ttl;

         
            printf("bowler bowled the ball when %d remaining\n",bowltime);
        //receive from batsman
         if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
	printf ("error\n");
            struct iphdr *ip_head1 = (struct iphdr*)datagram;
            hittime=ip_head1->ttl;
            printf("batsmen hit the ball when %d remaining\n",hittime);
            if(bowltime-hittime>10)
            {
                printf("Batsman is Out\n");
            }
 
        //receive from fielder
         if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
	printf ("error\n");
    printf("from fielder:-\n");
    printf("Out\n");
        while(1);
  return 0;
}