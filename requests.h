#ifndef __REQUESTS_H__
#define __REQUESTS_H__

#include "helpers.h"

#include <arpa/inet.h>
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */

typedef enum {GET, POST, DELETE} reuqest_type;

/* compune un request HTTP de tipul type pe baza parametrilor primiti */
char *compute_request(reuqest_type type, char *host, char *url,
                                char **query_params, int querys_count,
                                char **headers, int headers_count,
                                char **cookies, int cookies_count, 
                                char* content_type,
                                char **body_data, int body_data_fields_count);

#endif // __REQUESTS_H__
