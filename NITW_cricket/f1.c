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

unsigned short		/* this function generates header checksums */
csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

void sigint_handler() {
    exit(0);
}
#define P 9013	
void send_ump()
{
    int s = socket (PF_INET, SOCK_RAW, IPPROTO_UDP);	
  char datagram[4096];	
  
  struct ip *iph = (struct ip *) datagram;

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons (9013);
  sin.sin_addr.s_addr = inet_addr ("127.0.0.1");

  memset(&datagram,0,4096);	
 
  iph->ip_hl = 5;
  iph->ip_v = 4;
  iph->ip_tos = 0;
  iph->ip_len =sizeof (struct ip);	
  iph->ip_id = 123;
  iph->ip_off = 0;
  iph->ip_ttl =255;
  iph->ip_p = 6;
  iph->ip_sum = 0;		
  iph->ip_src.s_addr = inet_addr ("1.2.3.4");
  iph->ip_dst.s_addr = sin.sin_addr.s_addr;

  iph->ip_sum = csum ((unsigned short *) datagram, iph->ip_len >> 1);
    char buff[10];
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("\n\t Warning: I was not able to set HDRINCL!\n");


      if (sendto (s,datagram,iph->ip_len,0,(struct sockaddr *) &sin,	sizeof (sin)) > 0)		
        {}

}
int 
main (void)
{
 
    printf("This is fielder f1\n");
 int s = socket (PF_INET, SOCK_RAW, 10);	


int n = 0 ;
  				
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Warning: Cannot set HDRINCL!\n");

  char datagram[4096];	

 
  struct tcphdr *tcph = (struct tcphdr *) datagram + sizeof (struct ip);

  struct sockaddr_in sin,sin2; socklen_t len = sizeof(sin2) ;

memset(&sin,0,sizeof(sin)) ;

  sin.sin_family = AF_INET;
  sin.sin_port = htons (P);
  sin.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(s,(struct sockaddr *) &sin,sizeof(sin)) ;
 
   
      if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
	printf ("error\n");

         struct ip *iph = (struct ip *) datagram;
         printf("Got the ball\n");
        send_ump();
        while(1);
  return 0;
}