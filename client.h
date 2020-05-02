#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "helpers.h"
#include "parson.h"
#include "requests.h"
#include "user.h"
#include "utils.h"

#include <arpa/inet.h>
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */

#define HOST_NAME "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com"
#define USERNAME "GigelCelZambaret"
#define PASSWORD "oP@r0La_PuTeRn1Ca"

#define MAX_USERS 100

#define MAX_USERNAME_LEN 64
#define MAX_PASSWORD_LEN 64

#define JSON_TYPE "application/json"

#define URL_REGISTER "/api/v1/tema/auth/register"
#define URL_LOGIN "/api/v1/tema/auth/login"
#define URL_LIBRARY_ACCESS "/api/v1/tema/library/access"
#define URL_BOOKS "/api/v1/tema/library/books"
#define URL_LOGOUT "/api/v1/tema/auth/logout"

#define CONNECT_SID "connect.sid="
#define CONTENT_LEN "Content-Length: "
#define LEN_NUMBER_OFFSET 16
#define TOKEN "token"

#define BODY_START_STRING "\r\n\r\n"
#define BODY_OFFSET 4
#define MAX_BODY_FIELDS 10

#define MAX_COMMAND_LEN 64
#define REGISTER_CMD "register"
#define LOGIN_CMD "login"
#define ENTER_CMD "enter_library"
#define GET_BOOKS_CMD "get_books"
#define GET_BOOK_CMD "get_book"
#define ADD_BOOK_CMD "add_book"
#define DELETE_BOOK_CMD "delete_book"
#define LOGOUT_CMD "logout"
#define EXIT_CMD "exit"

/* returneaza un string in format JSON ce contine username-ul si parola trimise ca parametru */
char *get_json_string_username_password(const char *username, const char *password);

/* reurneaza cookie-ul de sesiune extras din mesajul primit ca parametru */
char *get_session_cookie(char *msg);

/* inregistreaza un nou utilizator cu username-ul si parola primite ca parametru */
void register_account(int sockfd, const char *username, const char *password);

/* logheaza utilizatorul cu username-ul si parola primite ca parametru, returneaza cookie-ul
pentru sesiunea curenta care s-a creat in urma logarii */
char* login(int sockfd, const char *username, const char *password);

/* returneaza token-ul JWT pentru cookie-ul de sesiune primit ca parametru */
char *get_library_access(int sockfd, char *session_cookie);

/* extrage componenta body din mesajul HTTP primit ca parametru */
char *get_message_body(const char* msg);

/* extrage token-ul JWT din body-ul primit ca parametru */
char *get_token_from_body(char* body);

/* executa comenzile primite de la stdin, returneaza 0 in cazul in care s-a primit
comanda exit si trebuie oprita executia programului, -1 daca s-a introdus o 
comanda invalida si 1 daca totul este ok */
int execute_command_from_stdin();

#endif // __CLIENT_H__