#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tcp.h"


int main() {

    int port = 8080;
    tcp_server server;

    if (bind_tcp_port(&server, port) != SERVER_OK) {
        printf("Failed to set up server\n");
        return -1;
    }

    int client_fd = accept_client(server.socket_fd);
    
    if (client_fd == -1) {
        printf("Failed to accept client connection\n");
        close(server.socket_fd);
        return -1;
    }

    printf("Client connected\n");

    close(client_fd);
    close(server.socket_fd);

    return 0;
}
