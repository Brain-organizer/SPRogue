#include "server.h"
#include "socklib.h"
#include "rank.h"
#include "handlsig.h"

#define BUF_SIZ 1024

sem_t second_copy, first_copy;

void exit_program(int signum){
    exit(0);
}

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    int ptocpipe[2];
    int ctoppipe[2];
    int pid;
    struct sockaddr_in clnt_adr;
    struct clnt_info cinfo;
    struct commu_args args;
    pthread_t t_id;
    FILE *fpin;

    signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
    signal(SIGTERM,SIG_IGN);
    signal(SIGALRM,SIG_IGN);
    signal(SIGKILL,SIG_IGN);
    signal(SIGUSR1,exit_program);

    init_lock();
    init_ranking();

    //fork해서 signal 핸들링 전용 프로세스 생성(얘가 부모)
    //signal 핸들링 전용 프로세스는 종료 인터럽트를 받을 때 진짜 종료할건지 묻는 역할과, 일정 간격마다 rank 파일을 save 해주는 역할을 한다.
    if(pipe(ptocpipe) == -1){
        perror("pipe error");
        exit(1);
    }
    if(pipe(ctoppipe) == -1){
        perror("pipe error");
        exit(1);
    }

    if((pid = fork()) == -1){
        perror("fork error");
        exit(1);
    } else if(pid != 0){
        close(ctoppipe[1]);
        close(ptocpipe[0]);
        handle_signal(ctoppipe[0], ptocpipe[1],pid);//
        exit(1);
    }

    sem_init(&second_copy, 0, 0);
    sem_init(&first_copy, 0, 1);

    load_rank_data();

    //부모 프로세스와 통신하는 쓰레드를 생성
    //해당 프로세스는 부모 프로세스가 pipe로 신호를 보내오면, ranking data를 보내준다.
    close(ctoppipe[0]);
    close(ptocpipe[1]);
    args.infd = ptocpipe[0];
    args.outfd = ctoppipe[1];
    pthread_create(&t_id, NULL, commuicate_with_sighandle_proc, (void *)&args);
    pthread_detach(t_id);

    serv_sock = make_server_socket();
    if (serv_sock == -1)
    {
        perror("make_server_socket error");
        exit(1);
    }

    while (1)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, NULL);

        sem_wait(&first_copy);
        cinfo.ip = inet_ntoa(clnt_adr.sin_addr);
        cinfo.sock = clnt_sock;
        sem_post(&second_copy);

        pthread_create(&t_id, NULL, handle_clnt, (void *)&cinfo); 
        pthread_detach(t_id);
        
    }

    close(serv_sock);
    return 0;
}

void *handle_clnt(void *arg)
{
    struct clnt_info cinfo;
    char *clnt_ip, *name;
    char log[BUF_SIZ];
    int playtime;
    struct rank_info rinfo;
    req_t req_type;
    FILE *logf, *sockfp, *wsockfp;

    sem_wait(&second_copy);
    cinfo = *((struct clnt_info *)arg);
    sem_post(&first_copy);

    sockfp = fdopen(cinfo.sock, "r");
    sockfp = fdopen(cinfo.sock, "w");
    clnt_ip = cinfo.ip;
    if ((name = create_name(clnt_ip)) == NULL)
    {
        fprintf(stderr, "create_name error\n");
        return NULL;
    }

    if ((logf = create_log_file(name)) == NULL)
    {
        fprintf(stderr, "create_log_file(%s) error\n", name);
        return NULL;
    }

    while (1)
    {
        if (fscanf(sockfp, "%d", (int *)&req_type) == EOF)
            break;

        if (req_type == POST_LOG)
        {
            if (fgets(log, BUF_SIZ, sockfp) == NULL) // log는 한줄 씩만 넘어와야 함.
                break;
            fputs(log, logf);
        }
        else if (req_type == POST_RANK)
        {
            if (fscanf(sockfp, "%d", &playtime) == EOF)
                break;
            if((rinfo = update_rank(name, playtime)).prec_playtime == -1){
                fprintf(stderr,"update_rank(%s, %d) error\n", name, playtime);
                return NULL;
            }
            fprintf(wsockfp,"%d",rinfo.rank);
            fprintf(wsockfp,"%d",rinfo.prec_playtime);
        }
    }

    free(clnt_ip);
    free(name);
    fclose(logf);
    fclose(sockfp);

    return NULL;
}

void *commuicate_with_sighandle_proc(void *arg){
    int infd, outfd;
    int nudge;

    infd = ((struct commu_args *)arg)->infd;
    outfd = ((struct commu_args *)arg)->outfd;
    
    while(1){
        if(read(infd, &nudge, sizeof(int)) == -1){
            perror("read error in commuicate");
            continue;
        }
        if(nudge == 0){
            if(write(outfd, get_record_num_pointer(),sizeof(int)) == -1)
                perror("write error in communicate");
            if(write(outfd, get_rank_buf(), sizeof(struct record) * (RANKING_SIZ+1)) == -1){
                perror("write error in communicate");
            }
        }
    }
}

char *create_name(char *name){
    int i;
    char *new_name;
    if (chdir("logs/") == -1)
    {
        perror("chdir to ./logs error");
        return NULL;
    }

    new_name = malloc(strlen(name) + 10);
    strcpy(new_name, name);

    i = 0;
    do
    {
        sprintf(new_name + strlen(new_name), "-%d", i++);
    } while (access(new_name, F_OK) == 0);

    if (chdir("../") == -1)
    {
        perror("chdir to ../ from logs error");
        return NULL;
    }

    return new_name;
}

FILE *create_log_file(char *filename){
    int fd;

    if (chdir("logs/") == -1)
    {
        perror("chdir to ./logs error");
        return NULL;
    }

    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd == -1)
    {
        fprintf(stderr, "%s ", filename);
        perror("open");
        return NULL;
    }

    if (chdir("../") == -1)
    {
        perror("chdir to ../ from logs error");
        return NULL;
    }

    return fdopen(fd, "w");
}