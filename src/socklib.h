#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <strings.h>
#include <arpa/inet.h>

int make_server_socket();
int make_server_socket_q(int);
int connect_to_server(char*);