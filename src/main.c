#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cjson/cJSON.h>

#include "http.h"
#include "route.h"
#include "tcp.h"

typedef struct {
    int port;
} server_config;


char *loadfile(const char *file_name) {
    FILE *file = fopen(file_name, "r+");
    fseek(file, 0 , SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_content = malloc(file_size + 1);
    if (!file_content) {
        perror("Failed to allocate memory for file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(file_content, 1, file_size, file);
    fclose(file);
    file_content[file_size] = '\0';

    char *copied = malloc(file_size + 1);
    strcpy(copied, file_content);

    free(file_content);
    return copied;
}

int loadConfig(server_config *config) {
    int status = 0;

    char* configdata = loadfile("config.json");
    if (!configdata) {
      printf("Could not load config file\n");
      goto end;
    }

    cJSON *config_json = cJSON_Parse(configdata);
    if (config_json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        status = 0;
        goto end;
    }

    cJSON *port = cJSON_GetObjectItemCaseSensitive(config_json, "portnumber");
    if (!cJSON_IsNumber(port)) {
        status = 0;
        goto end;
    }

    
    if (port->valueint > 65535 || port->valueint < 0) {
        printf("Invalid port number specified in config.\n");
        status = 0;
        goto end;
    }

    config->port = (short)port->valueint;
    status = 1;
    end:
      cJSON_Delete(config_json);
      return status;
}

void hello_handler(http_request *req, http_response *res) {
    res->status_code = 200;
  
    if (!res->body) {
      res->body = malloc(64);
    }

    strcpy(res->body, "Hello, World!\n");
    res->body_length = 14;

    add_http_header(res, "Content-Length", "14");
}

int main() {

    tcp_server server = {0};

    server_config config = {
      .port = 8080,
    };

    if (loadConfig(&config) == 0) {
      printf("failed to load config, using default values.\n");
    }

    
    if (bind_tcp_port(&server, config.port) != SERVER_OK) {
        printf("Failed to set up server\n");
        return 0;
    }

    install_route(HTTP_METHOD_GET, "/hello", hello_handler);
    
    for (;;) {
        int client_fd = accept_client(server.socket_fd);
        if (client_fd == -1) {
            printf("Failed to accept client connection\n");
            exit(EXIT_FAILURE);
        }
        printf("Client FD: %d\n", client_fd);

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

        if (!handle_request(&request, &response))
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
