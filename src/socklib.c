#include "socklib.h"

#define BACKLOG 5
#define PORT 8080


int make_server_socket(){
    return make_server_socket_q(BACKLOG);
}

int make_server_socket_q(int backlog){
    struct sockaddr_in saddr;
    int sock_id;

    sock_id = socket(PF_INET, SOCK_STREAM, 0);

    if(sock_id == -1){
        perror("socket() error");
        return -1;
    }

    bzero((void*)&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(PORT);

    if(bind(sock_id, (struct sockaddr*)&saddr, sizeof(saddr)) != 0){
        perror("bind error");
        return -1;
    }
    if(listen(sock_id, backlog) != 0){
        perror("listen error");
        return -1;
    }
    return sock_id;
}

int connect_to_server(char* host){
    int sock;
    struct sockaddr_in  servadd;
    struct hostent* hp;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket error");
        return -1;
    }
    bzero(&servadd, sizeof(servadd));
    hp = gethostbyname(host);
    if(hp == NULL) return -1;
    bcopy(hp->h_addr_list[0], (struct sockaddr*)&servadd.sin_addr, hp->h_length);
    servadd.sin_port = htons(PORT);
    servadd.sin_family = AF_INET;

    if(connect(sock, (struct sockaddr*)&servadd, sizeof(servadd)) != 0){
        perror("connect error");
        return -1;
    }
    return sock;
}