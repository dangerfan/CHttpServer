#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp.h"

server_status_e bind_tcp_port(tcp_server *server, int port) {
    if (port && port > 9999) {
        return -1;
    }

    memset(server, 0, sizeof(*server));
    
    if ((server->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        printf("Could not create socket\n");
        return SERVER_SOCKET_ERROR;
    }

    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(port);

    if (bind(server->socket_fd, (struct sockaddr*)&server->address, sizeof(server->address)) < 0) {
        perror("bind");
        close(server->socket_fd);
        return SERVER_BIND_ERROR;
    }

    if (listen(server->socket_fd, MAX_CONNECTIONS) < 0) {
        perror("listen");
        close(server->socket_fd);
        return SERVER_LISTEN_ERROR;
    }

    printf("Server listening\n");

    return SERVER_OK;
}

int accept_client(int server_fd) {
    struct sockaddr_in client_addr_in = {0};
    socklen_t client_len = sizeof(client_addr_in);

    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr_in, &client_len);
    if (client_fd < 0) {
        perror("accept");
        return -1;
    }

    return client_fd;
}
