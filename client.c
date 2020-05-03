#include "client.h"

char *get_json_string_username_password(const char *username, const char *password) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    char *serialized_string = json_serialize_to_string_pretty(root_value);
    return serialized_string;
}

char *get_json_string_book(const char *title, const char *author, const char *genre, int page_count, const char *publisher) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "author", author);
    json_object_set_string(root_object, "genre", genre);
    json_object_set_number(root_object, "page_count", page_count);
    json_object_set_string(root_object, "publisher", publisher);

    char *serialized_string = json_serialize_to_string_pretty(root_value);
    return serialized_string;
}

char *get_session_cookie(char *msg) {
    char *posStartCookie = strstr(msg, CONNECT_SID);
    char *posEndCookie = strchr(posStartCookie, ';');

    int cookieLen = posEndCookie - posStartCookie;
    char *cookie = (char *) calloc((cookieLen + 1), sizeof(char));

    memcpy(cookie, posStartCookie, cookieLen);
    return cookie;
}

char *get_token_from_body(const char *body) {
    JSON_Value *json_body = json_parse_string(body);
    char *token = (char *) json_object_get_string(json_object(json_body), TOKEN);
    return token;
}

char *get_message_body(const char *msg) {
    char *posStartLen = strstr(msg, CONTENT_LEN);
    posStartLen += LEN_NUMBER_OFFSET;

    // pentru o functionare corecta a functiei atoi se plaseaza '\0' la finalul numarului
    char *posEndLen = strchr(posStartLen, '\r');
    *posEndLen = '\0';

    int bodyLen = atoi(posStartLen);
    *posEndLen = '\r';

    char *posStartBody = strstr(msg, BODY_START_STRING);
    posStartBody += BODY_OFFSET;

    char *body = calloc(bodyLen, sizeof(char));
    memcpy(body, posStartBody, bodyLen);

    return body;
}

int get_status_code(const char *msg) {
    char *posStatusCodeStart = strchr(msg, ' ');
    ++posStatusCodeStart;

    char *statusCode = calloc(STATUS_CODE_LEN + 1, sizeof(char));
    memcpy(statusCode, posStatusCodeStart, STATUS_CODE_LEN);

    int statusCodeInt = atoi(statusCode);
    return statusCodeInt;
}

int connect_with_server() {
    struct hostent *hostInfo = gethostbyname(HOST_NAME);
    char *hostIp = inet_ntoa((struct in_addr) *((struct in_addr *)hostInfo->h_addr_list[0]));
    int sockfd = open_connection(hostIp, 8080, AF_INET, SOCK_STREAM, 0);
    return sockfd;
}

int is_positiv_number(const char *str) {
    for (int i = 0; i < strlen(str); ++i) {
        if ('0' > str[i] || str[i] > '9' ) {
            return 0;
        }
    }

    return 1;
}

char *register_account(const char *username, const char *password) {
    char **body_fields = calloc(1, sizeof(char *));
    body_fields[0] = get_json_string_username_password(username, password);
    char *msg = compute_request(POST, HOST_NAME, URL_REGISTER, NULL, 0, NULL, 0, NULL, 0, JSON_TYPE, body_fields, 1);

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    return response;
}

char* login(const char *username, const char *password) {
    char **body_fields = calloc(1, sizeof(char *));
    body_fields[0] = get_json_string_username_password(username, password);
    char *msg = compute_request(POST, HOST_NAME, URL_LOGIN, NULL, 0, NULL, 0, NULL, 0, JSON_TYPE, body_fields, 1);

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    return response;
}

char *get_library_access(char *session_cookie) {
    char **cookies = calloc(1, sizeof(char *));
    cookies[0] = session_cookie;
    char *msg = compute_request(GET, HOST_NAME, URL_LIBRARY_ACCESS, NULL, 0, NULL, 0, cookies, 1, NULL, NULL, 0);    

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    return response;
}

char *get_all_books(const char *token) {
    char **headers = calloc(1, sizeof(char *));
    headers[0] = calloc(AUTHORIZATION_HEADER_LEN + strlen(token) + 1, sizeof(char));
    strcpy(headers[0], AUTHORIZATION_HEADER);
    memcpy(headers[0] + AUTHORIZATION_HEADER_LEN, token, strlen(token));
    char *msg = compute_request(GET, HOST_NAME, URL_BOOKS, NULL, 0, headers, 1, NULL, 0, NULL, NULL, 0);

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    return response;
}

char *get_book(const char *token, int id) {
    char **headers = calloc(1, sizeof(char *));
    headers[0] = calloc(AUTHORIZATION_HEADER_LEN + strlen(token) + 1, sizeof(char));
    strcpy(headers[0], AUTHORIZATION_HEADER);
    memcpy(headers[0] + AUTHORIZATION_HEADER_LEN, token, strlen(token));

    char *urlBook = calloc(URL_BOOKS_LEN + MAX_ID_SIZE, sizeof(char));
    memcpy(urlBook, URL_BOOKS, URL_BOOKS_LEN);
    urlBook[URL_BOOKS_LEN] = '/';
    sprintf(urlBook + URL_BOOKS_LEN + 1, "%d", id);

    char *msg = compute_request(GET, HOST_NAME, urlBook, NULL, 0, headers, 1, NULL, 0, NULL, NULL, 0);

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    return response;
}

char *add_book(const char *token, const char *title, const char *author, const char *genre, int page_count, const char *publisher) {
    char **headers = calloc(1, sizeof(char *));
    headers[0] = calloc(AUTHORIZATION_HEADER_LEN + strlen(token) + 1, sizeof(char));
    strcpy(headers[0], AUTHORIZATION_HEADER);
    memcpy(headers[0] + AUTHORIZATION_HEADER_LEN, token, strlen(token));

    char **body = calloc(1, sizeof(char *));
    body[0] = get_json_string_book(title, author, genre, page_count, publisher);

    char *msg = compute_request(POST, HOST_NAME, URL_BOOKS, NULL, 0, headers, 1, NULL, 0, JSON_TYPE, body, 1);

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    return response;
}

char *delete_book(const char *token, int id) {
    char **headers = calloc(1, sizeof(char *));
    headers[0] = calloc(AUTHORIZATION_HEADER_LEN + strlen(token) + 1, sizeof(char));
    strcpy(headers[0], AUTHORIZATION_HEADER);
    memcpy(headers[0] + AUTHORIZATION_HEADER_LEN, token, strlen(token));

    char *urlBook = calloc(URL_BOOKS_LEN + MAX_ID_SIZE, sizeof(char));
    memcpy(urlBook, URL_BOOKS, URL_BOOKS_LEN);
    urlBook[URL_BOOKS_LEN] = '/';
    sprintf(urlBook + URL_BOOKS_LEN + 1, "%d", id);

    char *msg = compute_request(DELETE, HOST_NAME, urlBook, NULL, 0, headers, 1, NULL, 0, NULL, NULL, 0);

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    return response;
}

char *logout(const char *session_cookie) {
    char **cookies = calloc(1, sizeof(char *));
    cookies[0] = (char *) session_cookie;

    int sockfd = connect_with_server();
    char *msg = compute_request(GET, HOST_NAME, URL_LOGOUT, NULL, 0, NULL, 0, cookies, 1, NULL, NULL, 0);
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    return response;
}

// int execute_command_from_stdin() {
//     static int isLogedIn = 0;
//     static char *session_cookie = NULL;
//     static char *token = NULL;

//     char *ret_ptr = NULL;
//     char *response = NULL;

//     char command[MAX_COMMAND_LEN];
//     memset(command, 0, MAX_COMMAND_LEN);

//     ret_ptr = fgets(command, MAX_COMMAND_LEN - 1, stdin);
//     DIE(ret_ptr == NULL, "fgets");
//     command[strlen(command) - 1] = '\0';

//     if (strcmp(command, REGISTER_CMD) == 0) {
//         // reads username
//         char username[MAX_USERNAME_LEN];
//         memset(username, 0, MAX_USERNAME_LEN);
//         printf("username=");

//         ret_ptr = fgets(username, MAX_USERNAME_LEN - 1, stdin);
//         DIE(ret_ptr == NULL, "fgets");
//         username[strlen(username) - 1] = '\0';
//         if (strlen(username) == 0) {
//             printf("\nUsername can not be empty\n\n");
//             return -1;
//         }

//         // reads password
//         char password[MAX_PASSWORD_LEN];
//         memset(password, 0, MAX_PASSWORD_LEN);
//         printf("password=");

//         ret_ptr = fgets(password, MAX_PASSWORD_LEN - 1, stdin);
//         DIE(ret_ptr == NULL, "fgets");
//         password[strlen(password) - 1] = '\0';
//         if (strlen(password) == 0) {
//             printf("\nPassword can not be empty\n\n");
//         }

//         response = register_account(username, password);
//         if (response == NULL || strlen(response) == 0) {
//             printf("\nSomething went wrong, please try again\n\n");
//             return -1;
//         }

//         printf("\n%s\n\n", response);
//     } else if (strcmp(command, LOGIN_CMD) == 0) {
//         if (isLogedIn == 1) {
//             printf("\nYou are already loged in\n\n");
//             return -1;
//         }

//         // reads username
//         char username[MAX_USERNAME_LEN];
//         memset(username, 0, MAX_USERNAME_LEN);
//         printf("username=");

//         ret_ptr = fgets(username, MAX_USERNAME_LEN - 1, stdin);
//         DIE(ret_ptr == NULL, "fgets");
//         username[strlen(username) - 1] = '\0';
//         if (strlen(username) == 0) {
//             printf("\nUsername can not be empty\n\n");
//             return -1;
//         }

//         // reads password
//         char password[MAX_PASSWORD_LEN];
//         memset(password, 0, MAX_PASSWORD_LEN);
//         printf("password=");

//         ret_ptr = fgets(password, MAX_PASSWORD_LEN - 1, stdin);
//         DIE(ret_ptr == NULL, "fgets");
//         password[strlen(password) - 1] = '\0';
//         if (strlen(password) == 0) {
//             printf("\nPassword can not be empty\n\n");
//         }

//         response = login(username, password);
//         if (response == NULL || strlen(response) == 0) {
//             printf("\nSomething went wrong, please try again\n\n");
//             return -1;
//         }

//         int statusCode = get_status_code(response);
//         if (statusCode / 100 == 2) {
//             session_cookie = get_session_cookie(response);
//             if (session_cookie == NULL || strlen(session_cookie) == 0) {
//                 printf("\nSomething went wrong, please try again\n\n");
//                 return -1;
//             }

//             isLogedIn = 1;
//         }

//         printf("\n%s\n\n", response);
//     } else if (strcmp(command, ENTER_CMD) == 0) {
//         if (isLogedIn == 0) {
//             printf("\nYou must log in first\n\n");
//             return -1;
//         }

//         response = get_library_access(session_cookie);
//         if (response == NULL || strlen(response) == 0) {
//             printf("\nSomething went wrong, please try again\n\n");
//             return -1;
//         }

//         char *body = get_message_body(response);
//         if (body == NULL || strlen(body) == 0) {
//             printf("\nSomething went wrong, please try again\n\n");
//             return -1;
//         }

//         token = get_token_from_body(body);
//         if (token == NULL || strlen(token) == 0) {
//             printf("\nSomething went wrong, please try again\n\n");
//             return -1;
//         }

//         printf("\n%s\n\n", response);
//     } else if (strcmp(command, GET_BOOKS_CMD) == 0) {
//         if (isLogedIn == 0) {
//             printf("\nYou must log in first\n\n");
//             return -1;
//         }

//         if (token == NULL) {
//             printf("\nYou must request access to library first\n\n");
//             return -1;
//         }

//         response = get_all_books(token);
//         if (response == NULL || strlen(response) == 0) {
//             printf("\nSomething went wrong, please try again\n\n");
//             return -1;
//         }

//         printf("\n%s\n\n", response);
//     } else if (strcmp(command, GET_BOOK_CMD) == 0) {
//         if (isLogedIn == 0) {
//             printf("\nYou must log in first\n\n");
//             return -1;
//         }

//         if (token == NULL) {
//             printf("\nYou must request access to library first\n\n");
//             return -1;
//         }

//         char bookId[MAX_ID_SIZE];
//         memset(bookId, 0, MAX_ID_SIZE);
//         printf("id=");

//         ret_ptr = fgets(bookId, MAX_ID_SIZE - 1, stdin);
//         DIE(ret_ptr == NULL, "fgets");
//         bookId[strlen(bookId) - 1] = '\0';

//         if (strlen(bookId) == 0) {
//             printf("\nId can not be empty\n\n");
//             return -1;
//         }        

//         if (is_positiv_number(bookId) == 0) {
//             printf("\nThe id must be a positive number\n\n");
//             return -1;
//         }

//         int bookIdInt = atoi(bookId);
//         response = get_book(token, bookIdInt);
//         if (response == NULL || strlen(response) == 0) {
//             printf("\nSomething went wrong, please try again\n\n");
//             return -1;
//         }

//         printf("\n%s\n\n", response);
//     } else if (strcmp(command, ADD_BOOK_CMD) == 0) {
//         if (isLogedIn == 0) {
//             printf("\nYou must log in first\n\n");
//             return -1;
//         }

//         if (token == NULL) {
//             printf("\nYou must request access to library first\n\n");
//             return -1;
//         }

//         char title[MAX_TITLE_LEN];
//         memset(title, 0, MAX_TITLE_LEN);
//         printf("title=");

//         ret_ptr = fgets(title, MAX_TITLE_LEN - 1, stdin);
//         DIE(ret_ptr == NULL, "fgets");
//         title[strlen(title) - 1] = '\0';

//         if (strlen(title) == 0) {
//             printf("\nTitle can not be empty\n\n");
//             return -1;
//         }

//         char author[MAX_AUTHOR_LEN];
//         memset(author, 0, MAX_AUTHOR_LEN);
//         printf("author=");

//         ret_ptr = fgets(author, MAX_AUTHOR_LEN - 1, stdin);
//         DIE(ret_ptr == NULL, "fgets");
//         author[strlen(author) - 1] = '\0';

//         if (strlen(author) == 0) {
//             printf("\nAuthor can not be empty\n\n");
//             return -1;
//         }

//         char genre[MAX_GENRE_LEN];
//         memset(genre, 0, MAX_GENRE_LEN);
//         printf("genre=");

//         ret_ptr = fgets(genre, MAX_GENRE_LEN - 1, stdin);
//         DIE(ret_ptr == NULL, "fgets");
//         genre[strlen(genre) - 1] = '\0';

//         if (strlen(genre) == 0) {
//             printf("\nGenre can not be empty\n\n");
//             return -1;
//         }

//         char publisher[MAX_PUBLISHER_LEN];
//         memset(publisher, 0, MAX_PUBLISHER_LEN);
//         printf("publisher=");

//         ret_ptr = fgets(publisher, MAX_PUBLISHER_LEN - 1, stdin);
//         DIE(ret_ptr == NULL, "fgets");
//         publisher[strlen(publisher) - 1] = '\0';

//         if (strlen(publisher) == 0) {
//             printf("\nPublisher can not be empty\n\n");
//             return -1;
//         }

//         char pageCount[MAX_PAGE_COUNT_LEN];
//         memset(pageCount, 0, MAX_PAGE_COUNT_LEN);
//         printf("page_count=");

//         ret_ptr = fgets(pageCount, MAX_PAGE_COUNT_LEN - 1, stdin);
//         DIE(ret_ptr == NULL, "fgets");
//         pageCount[strlen(pageCount) - 1] = '\0';

//         if (strlen(pageCount) == 0) {
//             printf("\nPage count can not be empty\n\n");
//             return -1;
//         }

//         if (is_positiv_number(pageCount) == 0) {
//             printf("\nThe page count must be a positive number\n\n");
//             return -1;
//         }

//         int page_count = atoi(pageCount);

//         response = add_book(token, title, author, genre, page_count, publisher);
//         if (response == NULL || strlen(response) == 0) {
//             printf("\nSomething went wrong, please try again\n\n");
//             return -1;
//         }

//         printf("\n%s\n\n", response);
//     } else if (strcmp(command, DELETE_BOOK_CMD) == 0) {
//         if (isLogedIn == 0) {
//             printf("\nYou must log in first\n\n");
//             return -1;
//         }

//         if (token == NULL) {
//             printf("\nYou must request access to library first\n\n");
//             return -1;
//         }

//         char bookId[MAX_ID_SIZE];
//         memset(bookId, 0, MAX_ID_SIZE);
//         printf("id=");

//         ret_ptr = fgets(bookId, MAX_ID_SIZE - 1, stdin);
//         DIE(ret_ptr == NULL, "fgets");
//         bookId[strlen(bookId) - 1] = '\0';

//         if (strlen(bookId) == 0) {
//             printf("\nId can not be empty\n\n");
//             return -1;
//         }        

//         if (is_positiv_number(bookId) == 0) {
//             printf("\nThe id must be a positive number\n\n");
//             return -1;
//         }

//         int bookIdInt = atoi(bookId);
//         response = delete_book(token, bookIdInt);
//         if (response == NULL || strlen(response) == 0) {
//             printf("\nSomething went wrong, please try again\n\n");
//             return -1;
//         }

//         printf("\n%s\n\n", response);
//     } else if (strcmp(command, LOGOUT_CMD) == 0) {
//         if (isLogedIn == 0) {
//             printf("\nYou must log in first\n\n");
//             return -1;
//         }

//         response = logout(session_cookie);
//         if (response == NULL || strlen(response) == 0) {
//             printf("\nSomething went wrong, please try again\n\n");
//             return -1;
//         }

//         isLogedIn = 0;
//         session_cookie = NULL;
//         token = NULL;

//         printf("\n%s\n\n", response);
//     } else if (strcmp(command, EXIT_CMD) == 0) {
//         return 0;
//     } else {
//         printf("\nComanda este invalida\n\n");
//     }

//     return 1;
// }

int main(int argc, char *argv[]) {
    int ret_code;
    while (1) {
        ret_code = execute_command_from_stdin();
        if (ret_code == 0) {
            return 0;
        }
    }

    return 0;
    
// //  register_account(USERNAME, PASSWORD);
//     char* session_cookie = login(USERNAME, PASSWORD);
//     printf("\nSession cookie \n%s\n", session_cookie);

//     char *token = get_library_access(session_cookie);

//     get_all_books(token);
//     add_book(token, "tristete", "eu si numai eu", "drama", 1000, "nepublicata");
//     get_all_books(token);

//     delete_book(token, 443);
//     get_all_books(token);

//     get_book(token, 266);
//     logout(session_cookie);

//     return 0;
}
