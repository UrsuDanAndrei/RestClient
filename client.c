#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>


#include "helpers.h"
#include "requests.h"
#include "parson.h"

#define HOST_NAME "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com"
#define MAX_BODY_FIELDS 10

#define USERNAME "GigelCelZambaret"
#define PASSWORD "oP@r0La_PuTeRn1Ca"

#define JSON_TYPE "application/json"
#define URL_REGISTER "/api/v1/tema/auth/register"
#define URL_LOGIN "/api/v1/tema/auth/login"
#define URL_LIBRARY_ACCESS "/api/v1/tema/library/access"
#define CONNECT_SID "connect.sid="
#define CONTENT_LEN "Content-Length:"
#define LEN_NUMBER_OFFSET 17

char *get_json_string_username_password(const char *username, const char *password) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

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

void register_account(int sockfd, const char *username, const char *password) {
    char **body_fields = calloc(MAX_BODY_FIELDS, sizeof(char *));
    body_fields[0] = get_json_string_username_password(username, password);

    char *msg = compute_post_request(HOST_NAME, URL_REGISTER, JSON_TYPE, body_fields, 1, NULL, 0);
    send_to_server(sockfd, msg);

    char *response = receive_from_server(sockfd);
    printf("\nRaspuns autentificare: \n%s\n", response);
}

char* login(int sockfd, const char *username, const char *password) {
    char **body_fields = calloc(MAX_BODY_FIELDS, sizeof(char *));
    body_fields[0] = get_json_string_username_password(username, password);

    char *msg = compute_post_request(HOST_NAME, URL_LOGIN, JSON_TYPE, body_fields, 1, NULL, 0);
    send_to_server(sockfd, msg);

    char *response = receive_from_server(sockfd);
    return response;
}

char *get_library_access(int sockfd, char *session_cookie) {
    char **cookies = calloc(1, sizeof(char *));
    cookies[0] = session_cookie;

    char *msg = compute_get_request(HOST_NAME, URL_LIBRARY_ACCESS, NULL, cookies, 1);    
    send_to_server(sockfd, msg);

    char *response = receive_from_server(sockfd);
    return response;
}

char *get_message_body(const char* response) {
    strstr
}

int main(int argc, char *argv[]) {

    char *msg;
    char *response;
    
    char **auth = calloc(2, sizeof(char *));
    char **cookies = calloc(2, sizeof(char *));
    char **request_params = calloc(2, sizeof(char *));

    struct hostent *hostInfo = gethostbyname(HOST_NAME);
    char *hostIp = inet_ntoa((struct in_addr) *((struct in_addr *)hostInfo->h_addr_list[0]));
    int sockfd = open_connection(hostIp, 8080, AF_INET, SOCK_STREAM, 0);

    // ------------------------------------
    response = login(sockfd, USERNAME, PASSWORD);
    printf("\nRaspuns login: \n%s\n", response);

    char *session_cookie = get_session_cookie(response);
    printf("\ncookie: %s\n", session_cookie);

    response = get_library_access(sockfd, session_cookie);
    printf("%s\n\n6 ==========\n", response);

    char *body = get_message_body(response);
    

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

//     close_connection(sockfd);
//     close_connection(sockfd2);

    return 0;
}
