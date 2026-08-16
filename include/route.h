#ifndef ROUTE_H
#define ROUTE_H

#include "http.h"
#include <string.h>

#define MAX_ROUTES 10

typedef struct {
    http_method_e method;
    char path[128];
    void (*handler)(http_request *request, http_response *response);
} Route;

size_t install_route(http_method_e method, const char *path, void (*handler)(http_request *request, http_response *response));

#endif