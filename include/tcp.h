#ifndef TCP_H
#define TCP_H

#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CONNECTIONS 8

typedef enum {
    SERVER_OK,
    SERVER_SOCKET_ERROR,
    SERVER_BIND_ERROR,
    SERVER_LISTEN_ERROR,
    SERVER_ACCEPT_ERROR
} server_status_e;

typedef struct {
    struct sockaddr_in address;
    int socket_fd;
} tcp_server;


server_status_e bind_tcp_port(tcp_server *server, int port);
int accept_client(int server_fd);

#endif