#include "commands.h"

int execute_command_from_stdin() {
    static int isLogedIn = 0;
    static char *session_cookie = NULL;
    static char *token = NULL;

    char *ret_ptr = NULL;
    char *response = NULL;

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
        if (strlen(username) == 0) {
            printf("\nUsername can not be empty\n\n");
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
            printf("\nPassword can not be empty\n\n");
        }

        response = register_account(username, password);
        if (response == NULL || strlen(response) == 0) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }

        printf("\n%s\n\n", response);
    } else if (strcmp(command, LOGIN_CMD) == 0) {
        if (isLogedIn == 1) {
            printf("\nYou are already loged in\n\n");
            return -1;
        }

        // reads username
        char username[MAX_USERNAME_LEN];
        memset(username, 0, MAX_USERNAME_LEN);
        printf("username=");

        ret_ptr = fgets(username, MAX_USERNAME_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        username[strlen(username) - 1] = '\0';
        if (strlen(username) == 0) {
            printf("\nUsername can not be empty\n\n");
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
            printf("\nPassword can not be empty\n\n");
        }

        response = login(username, password);
        if (response == NULL || strlen(response) == 0) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }

        int statusCode = get_status_code(response);
        if (statusCode / 100 == 2) {
            session_cookie = get_session_cookie(response);
            if (session_cookie == NULL || strlen(session_cookie) == 0) {
                printf("\nSomething went wrong, please try again\n\n");
                return -1;
            }

            isLogedIn = 1;
        }

        printf("\n%s\n\n", response);
    } else if (strcmp(command, ENTER_CMD) == 0) {
        if (isLogedIn == 0) {
            printf("\nYou must log in first\n\n");
            return -1;
        }

        response = get_library_access(session_cookie);
        if (response == NULL || strlen(response) == 0) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }

        char *body = get_message_body(response);
        if (body == NULL || strlen(body) == 0) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }

        token = get_token_from_body(body);
        if (token == NULL || strlen(token) == 0) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }

        printf("\n%s\n\n", response);
    } else if (strcmp(command, GET_BOOKS_CMD) == 0) {
        if (isLogedIn == 0) {
            printf("\nYou must log in first\n\n");
            return -1;
        }

        if (token == NULL) {
            printf("\nYou must request access to library first\n\n");
            return -1;
        }

        response = get_all_books(token);
        if (response == NULL || strlen(response) == 0) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }

        printf("\n%s\n\n", response);
    } else if (strcmp(command, GET_BOOK_CMD) == 0) {
        if (isLogedIn == 0) {
            printf("\nYou must log in first\n\n");
            return -1;
        }

        if (token == NULL) {
            printf("\nYou must request access to library first\n\n");
            return -1;
        }

        char bookId[MAX_ID_LEN];
        memset(bookId, 0, MAX_ID_LEN);
        printf("id=");

        ret_ptr = fgets(bookId, MAX_ID_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        bookId[strlen(bookId) - 1] = '\0';

        if (strlen(bookId) == 0) {
            printf("\nId can not be empty\n\n");
            return -1;
        }        

        if (is_positiv_number(bookId) == 0) {
            printf("\nThe id must be a positive number\n\n");
            return -1;
        }

        int bookIdInt = atoi(bookId);
        response = get_book(token, bookIdInt);
        if (response == NULL || strlen(response) == 0) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }

        printf("\n%s\n\n", response);
    } else if (strcmp(command, ADD_BOOK_CMD) == 0) {
        if (isLogedIn == 0) {
            printf("\nYou must log in first\n\n");
            return -1;
        }

        if (token == NULL) {
            printf("\nYou must request access to library first\n\n");
            return -1;
        }

        char title[MAX_TITLE_LEN];
        memset(title, 0, MAX_TITLE_LEN);
        printf("title=");

        ret_ptr = fgets(title, MAX_TITLE_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        title[strlen(title) - 1] = '\0';

        if (strlen(title) == 0) {
            printf("\nTitle can not be empty\n\n");
            return -1;
        }

        char author[MAX_AUTHOR_LEN];
        memset(author, 0, MAX_AUTHOR_LEN);
        printf("author=");

        ret_ptr = fgets(author, MAX_AUTHOR_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        author[strlen(author) - 1] = '\0';

        if (strlen(author) == 0) {
            printf("\nAuthor can not be empty\n\n");
            return -1;
        }

        char genre[MAX_GENRE_LEN];
        memset(genre, 0, MAX_GENRE_LEN);
        printf("genre=");

        ret_ptr = fgets(genre, MAX_GENRE_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        genre[strlen(genre) - 1] = '\0';

        if (strlen(genre) == 0) {
            printf("\nGenre can not be empty\n\n");
            return -1;
        }

        char publisher[MAX_PUBLISHER_LEN];
        memset(publisher, 0, MAX_PUBLISHER_LEN);
        printf("publisher=");

        ret_ptr = fgets(publisher, MAX_PUBLISHER_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        publisher[strlen(publisher) - 1] = '\0';

        if (strlen(publisher) == 0) {
            printf("\nPublisher can not be empty\n\n");
            return -1;
        }

        char pageCount[MAX_PAGE_COUNT_LEN];
        memset(pageCount, 0, MAX_PAGE_COUNT_LEN);
        printf("page_count=");

        ret_ptr = fgets(pageCount, MAX_PAGE_COUNT_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        pageCount[strlen(pageCount) - 1] = '\0';

        if (strlen(pageCount) == 0) {
            printf("\nPage count can not be empty\n\n");
            return -1;
        }

        if (is_positiv_number(pageCount) == 0) {
            printf("\nThe page count must be a positive number\n\n");
            return -1;
        }

        int page_count = atoi(pageCount);

        response = add_book(token, title, author, genre, page_count, publisher);
        if (response == NULL || strlen(response) == 0) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }

        printf("\n%s\n\n", response);
    } else if (strcmp(command, DELETE_BOOK_CMD) == 0) {
        if (isLogedIn == 0) {
            printf("\nYou must log in first\n\n");
            return -1;
        }

        if (token == NULL) {
            printf("\nYou must request access to library first\n\n");
            return -1;
        }

        char bookId[MAX_ID_LEN];
        memset(bookId, 0, MAX_ID_LEN);
        printf("id=");

        ret_ptr = fgets(bookId, MAX_ID_LEN - 1, stdin);
        DIE(ret_ptr == NULL, "fgets");
        bookId[strlen(bookId) - 1] = '\0';

        if (strlen(bookId) == 0) {
            printf("\nId can not be empty\n\n");
            return -1;
        }        

        if (is_positiv_number(bookId) == 0) {
            printf("\nThe id must be a positive number\n\n");
            return -1;
        }

        int bookIdInt = atoi(bookId);
        response = delete_book(token, bookIdInt);
        if (response == NULL || strlen(response) == 0) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }

        printf("\n%s\n\n", response);
    } else if (strcmp(command, LOGOUT_CMD) == 0) {
        if (isLogedIn == 0) {
            printf("\nYou must log in first\n\n");
            return -1;
        }

        response = logout(session_cookie);
        if (response == NULL || strlen(response) == 0) {
            printf("\nSomething went wrong, please try again\n\n");
            return -1;
        }

        isLogedIn = 0;
        session_cookie = NULL;
        token = NULL;

        printf("\n%s\n\n", response);
    } else if (strcmp(command, EXIT_CMD) == 0) {
        return 0;
    } else {
        printf("\nComanda este invalida\n\n");
    }

    return 1;
}