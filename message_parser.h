#ifndef __MESSAGE_PARSER_H__
#define __MESSAGE_PARSER_H__

#include "parson.h"

#include <arpa/inet.h>
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */

#define CONNECT_SID "connect.sid="
#define CONTENT_LEN "Content-Length: "
#define LEN_NUMBER_OFFSET 16

#define BODY_START_STRING "\r\n\r\n"
#define BODY_OFFSET 4
#define MAX_BODY_FIELDS 10

#define TOKEN "token"
#define STATUS_CODE_LEN 3

int is_positive_number(const char *str);

/* returneaza un string in format JSON ce contine username-ul si parola trimise ca parametru */
char *get_json_string_username_password(const char *username, const char *password);

char *get_json_string_book(const char *title, const char *author, const char *genre, int page_count, const char *publisher);

/* reurneaza cookie-ul de sesiune extras din mesajul primit ca parametru */
char *get_session_cookie(char *msg);

/* extrage token-ul JWT din body-ul primit ca parametru */
char *get_token_from_body(const char* body);

/* extrage componenta body din mesajul HTTP primit ca parametru */
char *get_message_body(const char* msg);

int get_status_code(const char *msg);

#endif // __MESSAGE_PARSER_H__