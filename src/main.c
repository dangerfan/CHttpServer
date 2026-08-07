#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "http.h"
#include "tcp.h"


int main() {

    int port = 8080;
    tcp_server server = {0};
    http_request request = {0};

    
    if (bind_tcp_port(&server, port) != SERVER_OK) {
        printf("Failed to set up server\n");
        return 0;
    }

    int client_fd = accept_client(server.socket_fd);
    
    if (client_fd == -1) {
        printf("Failed to accept client connection\n");
        close(server.socket_fd);
        return 0;
    }

    printf("Client connected\n");
    if (read_http_request(client_fd, &request) != HTTP_PARSE_OK) {
        printf("Invalid request parse\n");
        close(client_fd);
        close(server.socket_fd);
        return 0;
    }

    printf("Parsed HTTP request:\n");
    printf("Method : Path : Protocol\t%s : %s : %s \n", request.method, request.path, request.protocol);
    
    if (parse_http_headers(request.buffer, &request) != HTTP_PARSE_OK) {
        printf("Invalid headers parse\n");
        close(client_fd);
        close(server.socket_fd);
        return 0;
    }

    printf("Parsed HTTP Headers:\n");
    for (size_t i = 0; i < request.header_count; i++) {
        printf("%s: %s\n", request.headers[i].key, request.headers[i].value);
    }

    free_http_headers(&request);

    close(client_fd);
    close(server.socket_fd);
    
    return 0;
}
