#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <zephyr.h>

/* Initialize the HTTP client */
int http_client_init(void);

/* Send GET request to the specified URL */
int http_client_get(const char *url, char *response_buffer, size_t buffer_size);

/* Send POST request to the specified URL with JSON payload */
int http_client_post(const char *url, const char *json_payload, char *response_buffer, size_t buffer_size);

#endif // HTTP_CLIENT_H

