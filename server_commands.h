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
#define HOST_PORT 8080

// for testing
#define USERNAME "GigelCelZambaret"
#define PASSWORD "oP@r0La_PuTeRn1Ca"

#define JSON_TYPE "application/json"

// URL-uri pentru diferite resurse
#define URL_REGISTER "/api/v1/tema/auth/register"
#define URL_LOGIN "/api/v1/tema/auth/login"
#define URL_LIBRARY_ACCESS "/api/v1/tema/library/access"
#define URL_BOOKS "/api/v1/tema/library/books"
#define URL_LOGOUT "/api/v1/tema/auth/logout"

#define MAX_ID_SIZE 10
#define URL_BOOKS_LEN 26

#define AUTHORIZATION_HEADER "Authorization: Bearer "
#define AUTHORIZATION_HEADER_LEN 22

// realizeaza conexiunea cu server-ul HOST_NAME
int connect_with_server();

/* inregistreaza un nou utilizator cu username-ul si parola
primite ca parametru, returneaza raspunsul HTTP primit de la server */
char *register_account(const char *username, const char *password);

/* logheaza utilizatorul cu username-ul si parola primite ca parametru,
returneaza raspunsul HTTP primit de la server din care se poate extrage
cookie-ul de sesiune */
char* login(const char *username, const char *password);

/* primeste un cookie de sesiune si returneaza raspunsul HTTP primit de la
server cand se solicita accesul in biblioteca, din mesajul returnat se poate
extrage un token JWT */
char *get_library_access(char *session_cookie);

/* primeste un token JWT pentru a demonstra accesul la biblioteca, returneaza
mesajul HTTP primit de la server in urma solicitarii tuturor cartilor */
char *get_all_books(const char *token);

/* primeste un token JWT pentru a demonstra accesul la biblioteca, returneaza
mesajul HTTP primit de la server in urma solicitarii cartii id */
char *get_book(const char *token, int id);

/* primeste un token JWT pentru a demonstra accesul la biblioteca, returneaza
mesajul HTTP primit de la server in urma adaugarii cartii definita de ceilalti
parametrii primiti */
char *add_book(const char *token, const char *title, const char *author,
                    const char *genre, int page_count, const char *publisher);

/* primeste un token JWT pentru a demonstra accesul la biblioteca, returneaza
mesajul HTTP primit de la server in urma stergerii cartii id */
char *delete_book(const char *token, int id);

/* primeste un token JWT pentru a demonstra accesul la biblioteca, returneaza
mesajul HTTP primit de la server in urma cererii de logout */
char *logout(const char *session_cookie);

#endif // __SERVER_COMMANDS_H__
