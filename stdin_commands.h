#ifndef __STDIN_COMMADNS_H__
#define __STDIN_COMMANDS_H__

#include "server_commands.h"
#include "helpers.h"

#include <arpa/inet.h>
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */

#define MAX_USERNAME_LEN 64
#define MAX_PASSWORD_LEN 64

#define MAX_TITLE_LEN 64
#define MAX_AUTHOR_LEN 64
#define MAX_GENRE_LEN 64
#define MAX_PUBLISHER_LEN 64
#define MAX_PAGE_COUNT_LEN 10

#define MAX_ID_LEN 10
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

void execute_register(int *ret_code);

char* execute_login(int isLogedIn, int *ret_code);

char* execute_enter_library(int isLogedIn, char *session_cookie, int *ret_code);

void execute_get_books(int isLogedIn, char *token, int *ret_code);

void execute_get_book(int isLogedIn, char *token, int *ret_code);

void execute_add_book(int isLogedIn, char *token, int *ret_code);

void execute_delete_book(int isLogedIn, char *token, int *ret_code);

void execute_logout(int isLogedIn, char *session_cookie, int *ret_code);

/* executa comenzile primite de la stdin, returneaza 0 in cazul in care s-a primit
comanda exit si trebuie oprita executia programului, -1 daca s-a introdus o 
comanda invalida si 1 daca totul este ok */
int execute_command_from_stdin();

#endif // __STDIN_COMMANDS_H__
