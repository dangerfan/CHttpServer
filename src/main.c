#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "http.h"
#include "tcp.h"


int main() {

    int port = 8080;
    tcp_server server = {0};
    
    if (bind_tcp_port(&server, port) != SERVER_OK) {
        printf("Failed to set up server\n");
        return 0;
    }

    for (;;) {
        int client_fd = accept_client(server.socket_fd);
        if (client_fd == -1) {
            printf("Failed to accept client connection\n");
            exit(EXIT_FAILURE);
        }

        printf("Client connected\n");

        http_request request = {0};
        http_response response = {0};
        
        init_http_response(&response);
        
        if (read_http_request(client_fd, &request) != HTTP_PARSE_OK) {
            perror("Could not read HTTP request");
            close(client_fd);
            return 0;
        }

        if (parse_http_headers(request.buffer, &request) != HTTP_PARSE_OK) {
            perror("Could not parse HTTP headers");
            close(client_fd);
            return 0;
        }

        char sanitized_path[1024] = {0};
        sanitize_path(request.path, sanitized_path, sizeof(sanitized_path));
        serve_file(sanitized_path, &response);

        send_http_response(client_fd, &response);
        printf("send_http_response complete\n");
        
        free_http_headers(&request);
        free_http_response(&response);
        printf("free_http_response complete\n");
        
        close(client_fd);
        printf("Response sent and client connection closed\n\n");
    }

    close(server.socket_fd);
    
    return 0;
}
