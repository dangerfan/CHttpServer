#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "http.h"


int main() {

    const char *raw_request_good =
        "GET /index.html HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: curl/7.68.0\r\n"
        "Accept: */*\r\n"
        "\r\n";
    
    http_request request_good = {0};
    
    if (parse_http_headers(raw_request_good, &request_good) != HTTP_PARSE_OK) {
        printf("Parsing good headers failed.\n");
    }

    printf("Parsed HTTP Headers:\n");
    for (size_t i = 0; i < request_good.header_count; i++) {
        printf("%s: %s\n", request_good.headers[i].key, request_good.headers[i].value);
    }

    free_http_headers(&request_good);

    return 0;
}