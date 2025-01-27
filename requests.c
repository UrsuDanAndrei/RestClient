#include "requests.h"

char *compute_request(reuqest_type type, char *host, char *url,
                                char **query_params, int querys_count,
                                char **headers, int headers_count,
                                char **cookies, int cookies_count, 
                                char* content_type,
                                char **body_data, int body_data_fields_count) {
    // se aloca spatiu pentru request
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *all_query_params = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(LINELEN, sizeof(char));

    // se adauga parametrii de cerere daca este cazul
    if (query_params != NULL) {
        strcat(all_query_params, query_params[0]);
        for (int i = 1; i < querys_count; ++i) {
            strcat(all_query_params, "&");
            strcat(all_query_params, query_params[i]);
        }
    }

    // se adauga tipul cererii
    switch(type) {
        case GET:
            if (query_params != NULL) {
                sprintf(line, "GET %s?%s HTTP/1.1", url, all_query_params);
            } else {
                sprintf(line, "GET %s HTTP/1.1", url);
            }
            break;

        case POST:
            if (query_params != NULL) {
                sprintf(line, "POST %s?%s HTTP/1.1", url, all_query_params);
            } else {
                sprintf(line, "POST %s HTTP/1.1", url);
            }
            break;
        
        case DELETE:
            if (query_params != NULL) {
                sprintf(line, "DELETE %s?%s HTTP/1.1", url, all_query_params);
            } else {
                sprintf(line, "DELETE %s HTTP/1.1", url);
            }
            break;
        
        default:
            return NULL;
    }
    compute_message(message, line);

    // se adauga hostname-ul
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // se adauga eventualele headere primite ca parametru
    if (headers != NULL) {
        for (int i = 0; i < headers_count; ++i) {
            memset(line, 0, LINELEN);
            sprintf(line, "%s", headers[i]);
            compute_message(message, line);
        }
    }

    memset(line, 0, LINELEN);
    sprintf(line, "User-Agent: Mozilla/5.0");
    compute_message(message, line);

    memset(line, 0, LINELEN);
    sprintf(line, "Connection: keep-alive");
    compute_message(message, line);

    // se creaza corpul mesajului (daca acesta exista)
    if (body_data != NULL) {
        strcat(body_data_buffer, body_data[0]);
        for (int i  = 1; i < body_data_fields_count; ++i) {
            strcat(body_data_buffer, "&");
            strcat(body_data_buffer, body_data[i]);
        }

        // se specifica tipul continutului
        memset(line, 0, LINELEN);
        sprintf(line, "Content-Type: %s", content_type);
        compute_message(message, line);

        // se specifica lungimea corpului
        memset(line, 0, LINELEN);
        sprintf(line, "Content-Length: %ld", strlen(body_data_buffer));
        compute_message(message, line);
    }

    // se adauga cookies daca este necesar
    if (cookies != NULL) {
        memset(line, 0, LINELEN);
        sprintf(line, "Cookie: %s", cookies[0]);

        for (int i = 1; i < cookies_count; ++i) {
            strcat(line, "; ");
            strcat(line, cookies[i]);
        }

        compute_message(message, line);
    }

    // se marcheaza finalul sectiunii de headers
    memset(line, 0, LINELEN);
    compute_message(message, "");

    // se adauga corpul mesajului (daca este cazul)
    if (body_data != NULL) {
        compute_message(message, body_data_buffer);
    }

    free(line);
    free(all_query_params);
    free(body_data_buffer);
    return message;
}
