#include "rank.h"

pthread_mutex_t lock;
struct record ranking[RANKING_SIZ + 1];
int record_num = 0;

void init_ranking(){
    for(int i = 0; i <= RANKING_SIZ; i++){
        strcpy(ranking[i].usr_id,"NULL");
        ranking[i].play_time = INT_MAX;
    }
}

int *get_record_num_pointer(){
    return &record_num;
}

struct rank_info update_rank(char *usr_name, int play_time){
    struct rank_info rinfo;
    int end_of_rank = RANKING_SIZ;
    
    pthread_mutex_lock(&lock);

    if(record_num <= RANKING_SIZ){
        record_num++;
    }

    ranking[end_of_rank].play_time = play_time;
    strncpy(ranking[end_of_rank].usr_id, usr_name, strlen(usr_name));

    qsort(ranking, RANKING_SIZ + 1, sizeof(struct record), compare);

    pthread_mutex_unlock(&lock);

    if((rinfo = get_own_rank(usr_name)).prec_playtime == -1){
        fprintf(stderr,"get_own_rank(%s) error\n", usr_name);
    }

    if(rinfo.rank == RANKING_SIZ){
        rinfo.rank = -RANKING_SIZ;
    }

    return rinfo;
}

struct rank_info get_own_rank(char *usr_name){
    struct rank_info rinfo;
    int found = 0;

    pthread_mutex_lock(&lock);
    for(int i = 0; i<= RANKING_SIZ ; i++){
        if(strcmp(usr_name,ranking[i].usr_id) == 0){
            rinfo.rank = i;
            rinfo.prec_playtime = i==0 ? ranking[i].play_time : ranking[i-1].play_time;
            found = 1;
        }
    }
    pthread_mutex_unlock(&lock);
    
    if(found == 0){
        fprintf(stderr, "get_own_rank(%s) error : no such data\n", usr_name);
        rinfo.rank = -1;
        rinfo.prec_playtime = -1;
    }

    return rinfo; 
}

int load_rank_data(){
    int fdrec;
    
    if((fdrec = open("rank/record",O_RDONLY)) == -1){
        perror("record file open error");
        return -1;
    }

    pthread_mutex_lock(&lock);

    if(read(fdrec, &record_num, sizeof(record_num)) == -1){
        perror("record file read error");
        return -1;
    }
    if(read(fdrec, ranking, sizeof(struct record) * (RANKING_SIZ+1)) == -1){
        perror("record file read error");
        return -1;
    }

    pthread_mutex_unlock(&lock);

    close(fdrec);
    return 0;
}

int save_rank_data(){
    int fdrec, fdread;
    FILE* fpread;

    pthread_mutex_lock(&lock);
    if((fdrec = open("rank/record", O_CREAT | O_WRONLY | O_TRUNC, 0644))== -1){
        perror("record file open error");
        return -1;
    }
    if(write(fdrec, &record_num, sizeof(record_num)) == -1){
        perror("record file write error");
        return -1;
    }
    if(write(fdrec, ranking, sizeof(struct record) * (RANKING_SIZ+1)) == -1){
        perror("record file write error");
        return -1;
    }
    
    if((fdread = open("rank/readable", O_CREAT | O_WRONLY | O_TRUNC, 0644))== -1){
        perror("record file open error");
        return -1;
    }
    if((fpread = fdopen(fdread, "w"))== NULL){
        perror("record file open error");
        return -1;
    }
    for(int i = 0; i <= RANKING_SIZ ; i++){
        fprintf(fpread, "랭킹%d위 - id: %s, 소요턴: %d틱\n", i, ranking[i].usr_id, ranking[i].play_time);
    }
    pthread_mutex_unlock(&lock);

    fclose(fpread);
    close(fdrec);
    return 0;
}

int compare (const void* first, const void* second){
    if(((struct record*)first)->play_time == ((struct record*)second)->play_time)
        return 0;
    else if (((struct record*)first)->play_time == 0 || ((struct record*)first)->play_time > ((struct record*)second)->play_time)
        return 1;
    else if (((struct record*)second)->play_time == 0 || ((struct record*)first)->play_time < ((struct record*)second)->play_time)
        return -1;
}

struct record* get_rank_buf(){
    return ranking;
}

void init_lock(){
    pthread_mutex_init(&lock, NULL);
}

void mutexlock(){
    pthread_mutex_lock(&lock);
}

void mutexunlock(){
    pthread_mutex_unlock(&lock);
}