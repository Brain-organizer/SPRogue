#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define RANKING_SIZ 150

struct record {
    char usr_id[30];
    int play_time;
};

struct rank_info {
    int rank;
    int prec_playtime;
};

struct rank_info update_rank(char *usr_name, int play_time);
struct rank_info get_own_rank(char *usr_name);
struct record* get_rank_buf();
int load_rank_data();
int save_rank_data();
int compare (const void* first, const void* second);
void init_lock();
void mutexlock();
void mutexunlock();
int *get_record_num_pointer();
void init_ranking();