#include "client.h"

char *get_json_string_username_password(const char *username, const char *password) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

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

void register_account(int sockfd, const char *username, const char *password) {
    char **body_fields = calloc(MAX_BODY_FIELDS, sizeof(char *));
    body_fields[0] = get_json_string_username_password(username, password);

    char *msg = compute_post_request(HOST_NAME, URL_REGISTER, JSON_TYPE, body_fields, 1, NULL, 0);
    printf("din register_account, de trimis: %s\n", msg);
    send_to_server(sockfd, msg);

    char *response = receive_from_server(sockfd);
    printf("\ndin register_account: \n%s\n", response);
}

char* login(int sockfd, const char *username, const char *password) {
    printf("\ndin login\n");
    char **body_fields = calloc(MAX_BODY_FIELDS, sizeof(char *));
    body_fields[0] = get_json_string_username_password(username, password);

    char *msg = compute_post_request(HOST_NAME, URL_LOGIN, JSON_TYPE, body_fields, 1, NULL, 0);
    send_to_server(sockfd, msg);
    printf("\ntrimis din loging: %s\n", msg);

    char *response = receive_from_server(sockfd);
    printf("\nprimit de la server din login: %s\n", response);
    char *session_cookie = get_session_cookie(response);
    printf("cookie: %s\n", session_cookie);
    return session_cookie;
}

char *get_library_access(int sockfd, char *session_cookie) {
    char **cookies = calloc(1, sizeof(char *));
    cookies[0] = session_cookie;

    char *msg = compute_get_request(HOST_NAME, URL_LIBRARY_ACCESS, NULL, cookies, 1, NULL, 0);    
    send_to_server(sockfd, msg);

    char *response = receive_from_server(sockfd);
    return response;
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

int execute_command_from_stdin(int sockfd, user **users, int *user_count) {
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

        ret_ptr = fgets(username, MAX_USERNAME_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        username[strlen(username) - 1] = '\0';
        // // !!! poate faci sa se citeasca ca ***, sau deloc sa nu apara

        // // reads password
        char password[MAX_PASSWORD_LEN];
        memset(password, 0, MAX_PASSWORD_LEN);
        printf("password=");

        ret_ptr = fgets(password, MAX_PASSWORD_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        password[strlen(password) - 1] = '\0';

        register_account(sockfd, username, password);
      //     return 1;
       // users[*users_count] = create_user(username, password);
        //++(*users_count);
    } else if (strcmp(command, LOGIN_CMD) == 0) {
        // reads username
        char username[MAX_USERNAME_LEN];
        memset(username, 0, MAX_USERNAME_LEN);
        printf("username=");

        ret_ptr = fgets(username, MAX_USERNAME_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        username[strlen(username) - 1] = '\0';
        // // !!! poate faci sa se citeasca ca ***, sau deloc sa nu apara

        // // reads password
        char password[MAX_PASSWORD_LEN];
        memset(password, 0, MAX_PASSWORD_LEN);
        printf("password=");

        ret_ptr = fgets(password, MAX_PASSWORD_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        password[strlen(password) - 1] = '\0';

        // user *usr = find_user(username, password, users, *users_count);
        // if (usr == NULL) {
        //     printf("\nWrong username or password\n\n");
        //     return -1;
        // }

        char *session_cookie = login(sockfd, "abc16", "1234");
        login(sockfd, "abc17", "1234");
        printf("\ndupa login\n");
        if (session_cookie == NULL) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }
        printf("\ndin execute_command, cookie: %s\n\n", session_cookie);
        
    //    add_session_cookie_to_user(usr, session_cookie);
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
    char *msg;
    char *response;
    user *users[MAX_USERS];
    int users_count = 0;
    int ret_code;

    char **auth = calloc(2, sizeof(char *));
    char **cookies = calloc(2, sizeof(char *));
    char **request_params = calloc(2, sizeof(char *));
    char **headers = calloc(2, sizeof(char *));

    struct hostent *hostInfo = gethostbyname(HOST_NAME);
    char *hostIp = inet_ntoa((struct in_addr) *((struct in_addr *)hostInfo->h_addr_list[0]));
    int sockfd = open_connection(hostIp, 8080, AF_INET, SOCK_STREAM, 0);

    // ------------------------------------
    // while (1) {
    //     ret_code = execute_command_from_stdin(sockfd, users, &users_count);
    //     if (ret_code == 0) {
    //         return 0;
    //     }
    // }
    
  //  register_account(sockfd, "abc7", "1234");
    char* session_cookie = login(sockfd, USERNAME, PASSWORD);
    printf("\nSession cookie \n%s\n", session_cookie);

    //char *session_cookie = get_session_cookie(response);
    //printf("\ncookie: %s\n", session_cookie);

    response = get_library_access(sockfd, session_cookie);
    printf("%s\n\n6 ==========\n", response);

    char *body = get_message_body(response);
    printf("body: %s\n", body);

    char *token = get_token_from_body(body);
    printf("\ntoken: %s\n", token);

    headers[0] = calloc(22 + strlen(token) + 2, sizeof(char));
    strcpy(headers[0], "Authorization: Bearer ");
    memcpy(headers[0] + 22, token, strlen(token));
  //  printf("\nheader-ul arata asa: %s\n", headers[0]);

    msg = compute_get_request(HOST_NAME, URL_BOOKS, NULL, NULL, 0, headers, 1);
    printf("Mesajul arata asa: %s\n", msg);
    send_to_server(sockfd, msg);

    response = receive_from_server(sockfd);
    printf("%s\n\n3 ===========\n", response);

    // Ex 1.2: POST dummy and print response from main server
//     dummy[0] = strdup("key1=val1");
//     dummy[1] = strdup("key2=val2");

//     message = compute_post_request(host, "/api/v1/dummy", "application/x-www-form-urlencoded", dummy, 2, NULL, 0);
//     send_to_server(sockfd, message);

//     response = receive_from_server(sockfd);
//     printf("%s\n\n3 ===========\n", response);

//     printf("\n === celalat server === \n\n");


//     // Ex 2: Login into main server
//     cookies[0] = strdup("connect.sid=s%3AdNbAeMMOlXxWgbbN23QNeYC_S_oanahv.yJPp7me%2BiLydWdiUFnlqcS2jRswejL%2FULiEi4lU7ppY");

//     char host2[] = "api.openweathermap.org";
//     char *ip2 = inet_ntoa((struct in_addr) * ((struct in_addr *)
//             gethostbyname(host2)->h_addr_list[0]));
//     int sockfd2 = open_connection(ip2, 80, AF_INET, SOCK_STREAM, 0);

//     auth[0] = strdup("username=student&");
//     auth[1] = strdup("password=student");

//     message = compute_post_request(host, "/api/v1/auth/login", "application/x-www-form-urlencoded", auth, 2, NULL, 0);
//     send_to_server(sockfd, message);

//     response = receive_from_server(sockfd);
//     printf("%s\n\n4 =======\n", response);

//     // BONUS: make the main server return "Already logged in!"
//     auth[0] = strdup("username=student&");
//     auth[1] = strdup("password=student");

//     message = compute_post_request(host, "/api/v1/auth/login", "application/x-www-form-urlencoded", auth, 2, cookies, 1);
//     send_to_server(sockfd, message);

//     response = receive_from_server(sockfd);
//     printf("%s\n\n5 ======\n", response);

//     // Ex 3: GET weather key from main server
//     message = compute_get_request(host, "/api/v1/weather/key", NULL, cookies, 1);    
//     send_to_server(sockfd, message);

//     response = receive_from_server(sockfd);
//     printf("%s\n\n6 ==========\n", response);

//     // Ex 4: GET weather data from OpenWeather API
//     char query_params[] = "lat=44.426765&lon=26.102537&appid=b912dd495585fbf756dc6d8f415a7649";
//     message = compute_get_request(host2, "/data/2.5/weather", query_params, cookies, 1);
//     send_to_server(sockfd2, message);

//     response = receive_from_server(sockfd2);
//     printf("%s\n\n7 ==========\n", response);

//     // Ex 6: Logout from main server
//     message = compute_get_request(host, "/api/v1/auth/logout", NULL, cookies, 1);    
//     send_to_server(sockfd, message);

//     response = receive_from_server(sockfd);
//     printf("%s\n\n7 =============\n", response);

    close_connection(sockfd);
    // close_connection(sockfd2);

    return 0;
}
