#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>

void handle_signal(int infd, int outfd, int cpid);
void* kill_cproc(void *args);
void* save_cur_ranking(void* args);
void sigalrm_handle(int signum);
void exit_handle(int signum);