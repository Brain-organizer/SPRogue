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
    int clnt_adr_sz;
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
        clnt_adr_sz = sizeof(clnt_adr);
        if((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz)) == -1){
            perror("accept error");
            continue;
        }

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
    int str_len;
    struct rank_info rinfo;
    req_t req_type;
    FILE *logf;
    int check = 0;

    sem_wait(&second_copy);
    cinfo = *((struct clnt_info *)arg);
    sem_post(&first_copy);

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
        if (read(cinfo.sock, &req_type, sizeof(req_type)) == 0)
            break;
        if(write(cinfo.sock, &check, sizeof(check)) == -1){
            perror("write error in check");
            break;
        }

        if (req_type == POST_LOG)
        {
            if ((str_len = read(cinfo.sock,log,BUF_SIZ)) == 0) 
                break;
            if(write(cinfo.sock, &check, sizeof(check)) == -1){
                perror("write error in check");
                break;
            }
            log[str_len] = 0;
            fprintf(logf,"%s\n",log);
        }
        else if (req_type == POST_RANK)
        {
            if (read(cinfo.sock, &playtime, sizeof(int)) == 0)
                break;
        
            if((rinfo = update_rank(name, playtime)).prec_playtime == -1){
                fprintf(stderr,"update_rank(%s, %d) error\n", name, playtime);
                return NULL;
            }
            write(cinfo.sock, &rinfo, sizeof(rinfo));
        }
    }

    free(name);
    fclose(logf);
    close(cinfo.sock);

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
    char* path;

    new_name = malloc(strlen(name) + 10);
    strcpy(new_name, name);
    path = malloc(strlen(new_name)+6);

    i = 0;
    do{
        sprintf(new_name + strlen(name), "-%d", i++);
        sprintf(path, "logs/%s",new_name);
    } while (access(path, F_OK) == 0);

    free(path);

    return new_name;
}

FILE *create_log_file(char *filename){
    int fd;
    char* path;

    path = malloc(strlen(filename)+10);
    sprintf(path, "logs/%s",filename);
    

    fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd == -1)
    {
        fprintf(stderr, "%s ", path);
        perror("open");
        return NULL;
    }

    free(path);

    return fdopen(fd, "w");
}