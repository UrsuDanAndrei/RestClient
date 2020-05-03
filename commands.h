#ifndef __COMMADNS_H__
#define __COMMANDS_H__

#include "client.h"
#include "helpers.h"
#include "requests.h"
#include "utils.h"

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

int execute_command_from_stdin();

#endif // __COMMANDS_H__