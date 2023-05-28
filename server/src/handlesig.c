#include "handlsig.h"
#include "rank.h"

int pid, in, out;

void handle_signal(int infd, int outfd, int cpid){
    pid = cpid; in = infd;  out = outfd;

    struct itimerval timeset;
    
    timeset.it_interval.tv_sec = 60;
    timeset.it_interval.tv_usec = 0;
    timeset.it_value.tv_sec = 60;
    timeset.it_value.tv_usec = 0;

    if(setitimer(ITIMER_REAL, &timeset, NULL) == -1){
        perror("itimer set error");
        return;
    }
    
    signal(SIGINT, kill_cproc);
    signal(SIGQUIT, kill_cproc);
    signal(SIGKILL, kill_cproc);
    signal(SIGALRM, sigalrm_handle);

    while(1){
        ;
    }
}

void kill_cproc(int signum){
    char yorn[10];
    
    printf("서버를 종료하시겠습니까? (yes/no)\n");
    fgets(yorn, 10, stdin);

    if(strcmp(yorn,"yes\n") != 0)
        return;
    
    signal(SIGALRM, SIG_IGN);
    
    mutexlock();
    kill(pid, SIGUSR1);
    waitpid(pid, NULL, 0);
    mutexunlock();

    exit(0);
}

void sigalrm_handle(int signum){
    pthread_t tid;
    if(pthread_create(&tid, NULL, save_cur_ranking, NULL) == -1){
        perror("pthread_create error in sigalrm_handle");
    }
    if(pthread_detach(tid) == -1){
        perror("pthread_detach error in sigalrm_handle");
    }
}

void* save_cur_ranking(void * args){
    int nudge = 0;
    
    if(write(out, &nudge, sizeof(int)) == -1){
        perror("nudge to childproc error");
    }
    if(read(in, get_record_num_pointer(),sizeof(int)) == -1){
        perror("read record_num from childproc error");
        return NULL;
    }
    if(read(in, get_rank_buf(), sizeof(struct record) * (RANKING_SIZ+1)) == -1){
        perror("read rank buf from childproc error");
        return NULL;
    }
    if(save_rank_data()==-1){
        fprintf(stderr, "save_rank_data error\n");
    }
}