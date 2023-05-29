typedef enum req_t{
    POST_LOG,
    POST_RANK,
} req_t;

struct rank_info {
    int rank;
    int prec_playtime;
};

void init_connect();
void close_connect();
int is_connected();
int post_log(char *);
struct rank_info post_rank(int);
void init_tick();
void increase_tick();
int get_tick();
