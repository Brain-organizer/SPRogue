#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "socklib.h"
#include "client.h"

int sockfd;
int connected;
int tick;

void init_connect(){
    if((sockfd = connect_to_server("175.45.193.147")) == -1){
        perror("connect to server failed");
        connected = 0;
    }
    else{
        connected = 1;
    }
}

void close_connect(){
    if(is_connected()){
        close(sockfd);
        
        connected = 0;
    }
}

int post_log(char *log_msg){
    req_t req = POST_LOG;
    int check = 0;
    if(!is_connected()){
        return 0;
    }

    if(write(sockfd,&req,sizeof(req)) == -1){
        perror("write error in post_log\n");
        close_connect();
    }
    else if(read(sockfd, &check, sizeof(check)) <= 0){
        perror("read error in post_log");
        close_connect();
    }
    else if(write(sockfd,log_msg,strlen(log_msg)) == -1){
        perror("write error in post_log\n");
        close_connect();
    }
    else if(read(sockfd, &check, sizeof(check)) <= 0){
        perror("read error in post_log");
        close_connect();
    }

    return 0;
}

struct rank_info post_rank(int playtime){
    struct rank_info rinfo = {0,0};
    req_t req = POST_RANK;
    int check = 0;
    
    if(is_connected()){
        if(write(sockfd,&req,sizeof(req)) == -1){
            perror("write error in post_rank");
            close_connect();
        }
        else if(read(sockfd, &check, sizeof(check)) <= 0){
            perror("read error in post_rank");
            close_connect();
        }
        else if(write(sockfd, &playtime, sizeof(int)) == -1){
            perror("write error in post_rank");
            close_connect();
        }
        else if(read(sockfd, &rinfo, sizeof(rinfo)) <= 0){
            perror("read error in post_rank");
            close_connect();
        }
    }

    return rinfo;
}

int is_connected(){
    return connected;
}

void init_tick(){
    tick = 0;
}

void increase_tick(){
    tick++;
}

int get_tick(){
    return tick;
}

// 게임 다 깼을 때, post_rank(tick) 해주고 struct rank_info 받아서 
// "당신의 순위는 ?위, 바로 앞 순위의 플레이어는 ?틱만에 클리어했어요!" 출력해주기.
// 그리고 close_connect()해준다.