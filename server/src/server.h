#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <arpa/inet.h>

struct clnt_info{
    int sock;
    char *ip;
};

struct commu_args{
    int infd;
    int outfd;
};

typedef enum req_t{
    POST_LOG,
    POST_RANK,
} req_t;

void* handle_clnt(void * arg);
void *commuicate_with_sighandle_proc(void *arg);
char *create_name(char *name);
FILE *create_log_file(char *filename);
void exit_program(int signum);

