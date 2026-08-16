#include "route.h"
#include <stdio.h>

Route routes[MAX_ROUTES];
int route_count = 0;

size_t install_route(http_method_e method, const char *path, void (*handler)(http_request *request, http_response *response)) {
    if (route_count < MAX_ROUTES) {
        routes[route_count].method = method;
        strncpy(routes[route_count].path, path, sizeof(routes[route_count].path));
        routes[route_count].handler = handler;
        printf("Route count: %d\n", route_count + 1);    
        return ++route_count;
    }

    printf("Route count: %d\n", route_count);
    return route_count;
}
