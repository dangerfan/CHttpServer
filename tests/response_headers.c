#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "http.h"

int main(void) {
    http_response response;
    init_http_response(&response);

    add_http_header(&response, "Content-Type", "text/html");
    add_http_header(&response, "Connecton", "close");

    printf("HTTP Response Headers:\n");

    for (size_t i = 0; i < response.header_count; i++) {
        printf("%s: %s\n", response.headers[i].key, response.headers[i].value);
    }

    free_http_response(&response);
    
    return 0;
}