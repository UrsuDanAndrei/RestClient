#include "server_commands.h"

int connect_with_server() {
    // se extrage ip-ul numelui de domeniu
    struct hostent *hostInfo = gethostbyname(HOST_NAME);
    char *hostIp = inet_ntoa((struct in_addr) *((struct in_addr *)
                                                    hostInfo->h_addr_list[0]));
    // se deschide conexiunea TCP
    int sockfd = open_connection(hostIp, HOST_PORT, AF_INET, SOCK_STREAM, 0);
    return sockfd;
}

char *register_account(const char *username, const char *password) {
    /* se creeaza un json string care sa reprezinte corpul mesajului si care
    sa contina username-ul si parola primite ca parametru */
    char **body_fields = calloc(1, sizeof(char *));
    body_fields[0] = get_json_string_username_password(username, password);

    // se compune HTTP request-ul
    char *msg = compute_request(POST, HOST_NAME, URL_REGISTER, NULL, 0, NULL, 0,
                                            NULL, 0, JSON_TYPE, body_fields, 1);

    /* se deschide conexiunea, se trimite mesajul HTTP,
    se primeste raspunsul, se inchide conexiunea */
    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    free(msg);
    free(body_fields[0]);
    free(body_fields);

    return response;
}

char* login(const char *username, const char *password) {
    /* se creeaza un json string care sa reprezinte corpul mesajului si care
    sa contina username-ul si parola primite ca parametru */
    char **body_fields = calloc(1, sizeof(char *));
    body_fields[0] = get_json_string_username_password(username, password);

    // se compune HTTP request-ul
    char *msg = compute_request(POST, HOST_NAME, URL_LOGIN, NULL, 0, NULL, 0,
                                            NULL, 0, JSON_TYPE, body_fields, 1);

    /* se deschide conexiunea, se trimite mesajul HTTP,
    se primeste raspunsul, se inchide conexiunea */
    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    free(msg);
    free(body_fields[0]);
    free(body_fields);

    return response;
}

char *get_library_access(char *session_cookie) {
    // se aloca spatiu pentru cookie-ul de sesiune
    char **cookies = calloc(1, sizeof(char *));
    cookies[0] = session_cookie;

    // se compune HTTP request-ul
    char *msg = compute_request(GET, HOST_NAME, URL_LIBRARY_ACCESS, NULL, 0,
                                            NULL, 0, cookies, 1, NULL, NULL, 0);

    /* se deschide conexiunea, se trimite mesajul HTTP,
    se primeste raspunsul, se inchide conexiunea */
    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    free(msg);
    free(cookies);

    return response;
}

char *get_all_books(const char *token) {
    // se introduce token-ul primit ca parametru in header-ele mesajului HTTP
    char **headers = calloc(1, sizeof(char *));
    headers[0] = calloc(AUTHORIZATION_HEADER_LEN + strlen(token) + 1,
                                                            sizeof(char));
    strcpy(headers[0], AUTHORIZATION_HEADER);
    memcpy(headers[0] + AUTHORIZATION_HEADER_LEN, token, strlen(token));

    // se compune mesajul HTTP
    char *msg = compute_request(GET, HOST_NAME, URL_BOOKS, NULL, 0, headers, 1,
                                                        NULL, 0, NULL, NULL, 0);

    /* se deschide conexiunea, se trimite mesajul HTTP,
    se primeste raspunsul, se inchide conexiunea */
    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    free(msg);
    free(headers[0]);
    free(headers);

    return response;
}

char *get_book(const char *token, int id) {
    // se introduce token-ul primit ca parametru in header-ele mesajului HTTP
    char **headers = calloc(1, sizeof(char *));
    headers[0] = calloc(AUTHORIZATION_HEADER_LEN + strlen(token) + 1,
                                                            sizeof(char));
    strcpy(headers[0], AUTHORIZATION_HEADER);
    memcpy(headers[0] + AUTHORIZATION_HEADER_LEN, token, strlen(token));

    // se formeaza url-ul specific cartii id
    char *urlBook = calloc(URL_BOOKS_LEN + MAX_ID_SIZE, sizeof(char));
    memcpy(urlBook, URL_BOOKS, URL_BOOKS_LEN);
    urlBook[URL_BOOKS_LEN] = '/';
    sprintf(urlBook + URL_BOOKS_LEN + 1, "%d", id);

    // se compune mesajul HTTP
    char *msg = compute_request(GET, HOST_NAME, urlBook, NULL, 0, headers, 1,
                                                        NULL, 0, NULL, NULL, 0);

    /* se deschide conexiunea, se trimite mesajul HTTP,
    se primeste raspunsul, se inchide conexiunea */
    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    free(msg);
    free(headers[0]);
    free(headers);
    free(urlBook);

    return response;
}

char *add_book(const char *token, const char *title, const char *author,
                    const char *genre, int page_count, const char *publisher) {
    // se introduce token-ul primit ca parametru in header-ele mesajului HTTP
    char **headers = calloc(1, sizeof(char *));
    headers[0] = calloc(AUTHORIZATION_HEADER_LEN + strlen(token) + 1,
                                                            sizeof(char));
    strcpy(headers[0], AUTHORIZATION_HEADER);
    memcpy(headers[0] + AUTHORIZATION_HEADER_LEN, token, strlen(token));

    /* se creeaza un json string care sa reprezinte corpul mesajului si care
    sa contina toate informatiile legate de carte */
    char **body = calloc(1, sizeof(char *));
    body[0] = get_json_string_book(title, author, genre, page_count, publisher);

    // se compune mesajul
    char *msg = compute_request(POST, HOST_NAME, URL_BOOKS, NULL, 0, headers, 1,
                                                NULL, 0, JSON_TYPE, body, 1);

    /* se deschide conexiunea, se trimite mesajul HTTP,
    se primeste raspunsul, se inchide conexiunea */
    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    free(msg);
    free(headers[0]);
    free(headers);
    free(body[0]);
    free(body);

    return response;
}

char *delete_book(const char *token, int id) {
    // se introduce token-ul primit ca parametru in header-ele mesajului HTTP
    char **headers = calloc(1, sizeof(char *));
    headers[0] = calloc(AUTHORIZATION_HEADER_LEN + strlen(token) + 1,
                                                            sizeof(char));
    strcpy(headers[0], AUTHORIZATION_HEADER);
    memcpy(headers[0] + AUTHORIZATION_HEADER_LEN, token, strlen(token));

    // se formeaza url-ul specific cartii id
    char *urlBook = calloc(URL_BOOKS_LEN + MAX_ID_SIZE, sizeof(char));
    memcpy(urlBook, URL_BOOKS, URL_BOOKS_LEN);
    urlBook[URL_BOOKS_LEN] = '/';
    sprintf(urlBook + URL_BOOKS_LEN + 1, "%d", id);

    // se compune mesajul HTTP
    char *msg = compute_request(DELETE, HOST_NAME, urlBook, NULL, 0, headers, 1,
                                                        NULL, 0, NULL, NULL, 0);

    /* se deschide conexiunea, se trimite mesajul HTTP,
    se primeste raspunsul, se inchide conexiunea */    
    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    free(msg);
    free(headers[0]);
    free(headers);
    free(urlBook);

    return response;
}

char *logout(const char *session_cookie) {
    // se aloca spatiu pentru cookie-ul de sesiune
    char **cookies = calloc(1, sizeof(char *));
    cookies[0] = (char *) session_cookie;

    // se compune mesajul HTTP
    char *msg = compute_request(GET, HOST_NAME, URL_LOGOUT, NULL, 0, NULL, 0,
                                                    cookies, 1, NULL, NULL, 0);
    
    /* se deschide conexiunea, se trimite mesajul HTTP,
    se primeste raspunsul, se inchide conexiunea */
    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    free(msg);
    free(cookies);

    return response;
}
