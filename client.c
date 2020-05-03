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
    printf("\ndin get_session_cookie: %s\n", msg);
    char *posStartCookie = strstr(msg, CONNECT_SID);
    char *posEndCookie = strchr(posStartCookie, ';');

    int cookieLen = posEndCookie - posStartCookie;
    char *cookie = (char *) calloc((cookieLen + 1), sizeof(char));

    memcpy(cookie, posStartCookie, cookieLen);
    return cookie;
}

int connect_with_server() {
    struct hostent *hostInfo = gethostbyname(HOST_NAME);
    char *hostIp = inet_ntoa((struct in_addr) *((struct in_addr *)hostInfo->h_addr_list[0]));
    int sockfd = open_connection(hostIp, 8080, AF_INET, SOCK_STREAM, 0);
    return sockfd;
}

void register_account(const char *username, const char *password) {
    char **body_fields = calloc(1, sizeof(char *));
    body_fields[0] = get_json_string_username_password(username, password);
    char *msg = compute_request(POST, HOST_NAME, URL_REGISTER, NULL, 0, NULL, 0, NULL, 0, JSON_TYPE, body_fields, 1);

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("din register_account, de trimis: %s\n", msg);
    printf("\ndin register_account: \n%s\n", response);
}

char* login(const char *username, const char *password) {
    printf("\ndin login\n");
    char **body_fields = calloc(1, sizeof(char *));
    body_fields[0] = get_json_string_username_password(username, password);
    char *msg = compute_request(POST, HOST_NAME, URL_LOGIN, NULL, 0, NULL, 0, NULL, 0, JSON_TYPE, body_fields, 1);

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("\ntrimis din loging: %s\n", msg);
    printf("\nprimit de la server din login: %s\n", response);

    char *session_cookie = get_session_cookie(response);
    printf("cookie: %s\n", session_cookie);
    return session_cookie;
}

char *get_library_access(char *session_cookie) {
    char **cookies = calloc(1, sizeof(char *));
    cookies[0] = session_cookie;
    char *msg = compute_request(GET, HOST_NAME, URL_LIBRARY_ACCESS, NULL, 0, NULL, 0, cookies, 1, NULL, NULL, 0);    

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    char *body = get_message_body(response);
    char *token = get_token_from_body(body);
    printf("\ntoken: %s\n", token);

    return token;
}

char *get_message_body(const char* msg) {
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

char *get_token_from_body(char* body) {
    JSON_Value *json_body = json_parse_string(body);
    char *token = json_object_get_string(json_object(json_body), TOKEN);
    return token;
}

void get_all_books(const char *token) {
    char **headers = calloc(1, sizeof(char *));
    headers[0] = calloc(AUTHORIZATION_HEADER_LEN + strlen(token) + 1, sizeof(char));
    strcpy(headers[0], AUTHORIZATION_HEADER);
    memcpy(headers[0] + AUTHORIZATION_HEADER_LEN, token, strlen(token));
    char *msg = compute_request(GET, HOST_NAME, URL_BOOKS, NULL, 0, headers, 1, NULL, 0, NULL, NULL, 0);

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("\nDin get_all_books: \n%s\n", response);
}

void get_book(const char *token, int id) {
    char **headers = calloc(1, sizeof(char *));
    headers[0] = calloc(AUTHORIZATION_HEADER_LEN + strlen(token) + 1, sizeof(char));
    strcpy(headers[0], AUTHORIZATION_HEADER);
    memcpy(headers[0] + AUTHORIZATION_HEADER_LEN, token, strlen(token));

    char *urlBook = calloc(URL_BOOKS_SIZE + MAX_ID_SIZE, sizeof(char));
    memcpy(urlBook, URL_BOOKS, URL_BOOKS_SIZE);
    urlBook[URL_BOOKS_SIZE] = '/';
    sprintf(urlBook + URL_BOOKS_SIZE + 1, "%d", id);

    char *msg = compute_request(GET, HOST_NAME, urlBook, NULL, 0, headers, 1, NULL, 0, NULL, NULL, 0);

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("\nDin get book: \n%s\n", response);
}

void add_book(const char *token, const char *title, const char *author, const char *genre, int page_count, const char *publisher) {
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

    printf("\nDin add_book: \n%s\n", response);
}

void delete_book(const char *token, int id) {
    char **headers = calloc(1, sizeof(char *));
    headers[0] = calloc(AUTHORIZATION_HEADER_LEN + strlen(token) + 1, sizeof(char));
    strcpy(headers[0], AUTHORIZATION_HEADER);
    memcpy(headers[0] + AUTHORIZATION_HEADER_LEN, token, strlen(token));

    char *urlBook = calloc(URL_BOOKS_SIZE + MAX_ID_SIZE, sizeof(char));
    memcpy(urlBook, URL_BOOKS, URL_BOOKS_SIZE);
    urlBook[URL_BOOKS_SIZE] = '/';
    sprintf(urlBook + URL_BOOKS_SIZE + 1, "%d", id);

    printf("\nZZZZZZZZZZZZZZZZZZZ url book: %s\n", urlBook);
    char *msg = compute_request(DELETE, HOST_NAME, urlBook, NULL, 0, headers, 1, NULL, 0, NULL, NULL, 0);

    int sockfd = connect_with_server();
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("\nDin delelte book: \n%s\n", response);
}

void logout(const char *session_cookie) {
    char **cookies = calloc(2, sizeof(char *));
    cookies[0] = session_cookie;

    int sockfd = connect_with_server();
    char *msg = compute_request(GET, HOST_NAME, URL_LOGOUT, NULL, 0, NULL, 0, cookies, 1, NULL, NULL, 0);
    send_to_server(sockfd, msg);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("%s\n\n30 ===========\n", response);
}

int execute_command_from_stdin() {
    static isLogedIn = 0;
    static char *cookie = NULL;
    static char *token = NULL;

    char *ret_ptr;
    char command[MAX_COMMAND_LEN];
    memset(command, 0, MAX_COMMAND_LEN);

    ret_ptr = fgets(command, MAX_COMMAND_LEN - 1, stdin);
    DIE(ret_ptr == NULL, "fgets");
    command[strlen(command) - 1] = '\0';

    if (strcmp(command, REGISTER_CMD) == 0) {
        // reads username
        char username[MAX_USERNAME_LEN];
        memset(username, 0, MAX_USERNAME_LEN);
        printf("username=");
        if (strlen(username) == 0) {
            printf("\nUsername can not be empty\n");
            return -1;
        }

        ret_ptr = fgets(username, MAX_USERNAME_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        username[strlen(username) - 1] = '\0';

        // reads password
        char password[MAX_PASSWORD_LEN];
        memset(password, 0, MAX_PASSWORD_LEN);
        printf("password=");
        if (strlen(password) == 0) {
            printf("\nPassword can not be empty\n");
        }

        ret_ptr = fgets(password, MAX_PASSWORD_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        password[strlen(password) - 1] = '\0';

        register_account(username, password);
    } else if (strcmp(command, LOGIN_CMD) == 0) {
        // reads username
        char username[MAX_USERNAME_LEN];
        memset(username, 0, MAX_USERNAME_LEN);
        printf("username=");

        ret_ptr = fgets(username, MAX_USERNAME_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        username[strlen(username) - 1] = '\0';
        if (strlen(username) == 0) {
            printf("\nUsername can not be empty\n");
            return -1;
        }

        // reads password
        char password[MAX_PASSWORD_LEN];
        memset(password, 0, MAX_PASSWORD_LEN);
        printf("password=");

        ret_ptr = fgets(password, MAX_PASSWORD_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        password[strlen(password) - 1] = '\0';
        if (strlen(password) == 0) {
            printf("\nPassword can not be empty\n");
        }

        char *session_cookie = login("abc16", "1234");
        login("abc17", "1234");
        printf("\ndupa login\n");
        if (session_cookie == NULL) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }
        printf("\ndin execute_command, cookie: %s\n\n", session_cookie);
    } else if (strcmp(command, ENTER_CMD) == 0) {
        
    } else if (strcmp(command, GET_BOOKS_CMD) == 0) {
        
    } else if (strcmp(command, GET_BOOK_CMD) == 0) {
        
    } else if (strcmp(command, ADD_BOOK_CMD) == 0) {
        
    } else if (strcmp(command, DELETE_BOOK_CMD) == 0) {
        
    } else if (strcmp(command, LOGOUT_CMD) == 0) {
        
    } else if (strcmp(command, LOGOUT_CMD) == 0) {
        
    } else if (strcmp(command, EXIT_CMD) == 0) {
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[]) {
    int ret_code;
    while (1) {
        ret_code = execute_command_from_stdin();
        if (ret_code == 0) {
            return 0;
        }
    }

    return 0;
    char *msg;
    char *response;
    user *users[MAX_USERS];
    int users_count = 0;
    int ret_code;

    char **auth = calloc(2, sizeof(char *));
    char **cookies = calloc(2, sizeof(char *));
    char **request_params = calloc(2, sizeof(char *));
    char **headers = calloc(2, sizeof(char *));
    char **the_body = calloc(2, sizeof(char *));

    // int sockfd = connect_with_server();

    // ------------------------------------
    
  //  register_account(sockfd, "abc7", "1234");
    char* session_cookie = login(USERNAME, PASSWORD);
    printf("\nSession cookie \n%s\n", session_cookie);

    char *token = get_library_access(session_cookie);

    get_all_books(token);
    add_book(token, "tristete", "eu si numai eu", "drama", 1000, "nepublicata");
    get_all_books(token);

    delete_book(token, 443);
    get_all_books(token);

    get_book(token, 266);
    logout(session_cookie);

    return 0;
}
