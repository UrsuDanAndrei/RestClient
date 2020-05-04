#ifndef __SERVER_COMMANDS_H__
#define __SERVER_COMMANDS_H__

#include "requests.h"
#include "message_parser.h"

#include <arpa/inet.h>
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */

#define HOST_NAME "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com"

// for testing
#define USERNAME "GigelCelZambaret"
#define PASSWORD "oP@r0La_PuTeRn1Ca"

#define JSON_TYPE "application/json"

#define URL_REGISTER "/api/v1/tema/auth/register"
#define URL_LOGIN "/api/v1/tema/auth/login"
#define URL_LIBRARY_ACCESS "/api/v1/tema/library/access"
#define URL_BOOKS "/api/v1/tema/library/books"
#define URL_LOGOUT "/api/v1/tema/auth/logout"

#define MAX_ID_SIZE 10
#define URL_BOOKS_LEN 26

#define AUTHORIZATION_HEADER "Authorization: Bearer "
#define AUTHORIZATION_HEADER_LEN 22

int connect_with_server();

/* inregistreaza un nou utilizator cu username-ul si parola primite ca parametru */
char *register_account(const char *username, const char *password);

/* logheaza utilizatorul cu username-ul si parola primite ca parametru, returneaza cookie-ul
pentru sesiunea curenta care s-a creat in urma logarii */
char* login(const char *username, const char *password);

/* returneaza token-ul JWT pentru cookie-ul de sesiune primit ca parametru */
char *get_library_access(char *session_cookie);

char *get_all_books(const char *token);

char *get_book(const char *token, int id);

char *add_book(const char *token, const char *title, const char *author, const char *genre, int page_count, const char *publisher);

char *delete_book(const char *token, int id);

char *logout(const char *session_cookie);

#endif // __SERVER_COMMANDS_H__