#include "stdin_commands.h"

int is_positive_number(const char *str) {
    for (int i = 0; i < strlen(str); ++i) {
        if ('0' > str[i] || str[i] > '9' ) {
            return 0;
        }
    }

    return 1;
}

void execute_register(int *ret_code) {
    char *ret_ptr;
    char *response;

    // reads username
    char username[MAX_USERNAME_LEN];
    memset(username, 0, MAX_USERNAME_LEN);
    printf("username=");

    ret_ptr = fgets(username, MAX_USERNAME_LEN - 1, stdin);
    DIE(ret_ptr == NULL, "fgets");
    username[strlen(username) - 1] = '\0';
    if (strlen(username) == 0) {
        printf("\nUsername can not be empty\n\n");
        *ret_code = -1;
        return;
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
        *ret_code = -1;
        return;
    }

    // obtine si valideaza raspunsul obtinut in urma cererii trimise
    response = register_account(username, password);
    if (response == NULL || strlen(response) == 0) {
        printf("\nSomething went wrong, please try again\n\n");
        *ret_code = -1;
        return;
    }

    printf("\n%s\n\n", response);
    free(response);
    *ret_code = 1;
}

char* execute_login(int isLogedIn, int *ret_code) {
    char *ret_ptr;
    char *response;
    char *session_cookie;

    // se verifica daca utilizatorul este deja logat
    if (isLogedIn == 1) {
        printf("\nYou are already loged in\n\n");
        *ret_code = -1;
        return NULL;
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
        *ret_code = -1;
        return NULL;
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
        *ret_code = -1;
        return NULL;
    }

    // obtine si valideaza raspunsul obtinut in urma cererii trimise
    response = login(username, password);
    if (response == NULL || strlen(response) == 0) {
        printf("\nSomething went wrong, please try again\n\n");
        *ret_code = -1;
        return NULL;
    }

    printf("\n%s\n\n", response);

    // extrage si verifica status code-ul
    int statusCode = get_status_code(response);
    if (statusCode / 100 != 2) {
        *ret_code = -1;
        return NULL;
    }

    // extrage cookie-ul de sesiune din mesaj
    session_cookie = get_session_cookie(response);
    if (session_cookie == NULL || strlen(session_cookie) == 0) {
        printf("\nSomething went wrong, please try again\n\n");
        *ret_code = -1;
        return NULL;
    }

    free(response);
    *ret_code = 1;
    return session_cookie;
}

char* execute_enter_library(int isLogedIn, char *session_cookie,
                                                        int *ret_code) {
    char *token;
    char *response;

    // nu executa comanda decat daca utilizatorul este logat
    if (isLogedIn == 0) {
        printf("\nYou must log in first\n\n");
        *ret_code = -1;
        return NULL;
    }

    // obtine si valideaza raspunsul obtinut in urma cererii trimise
    response = get_library_access(session_cookie);
    if (response == NULL || strlen(response) == 0) {
        printf("\nSomething went wrong, please try again\n\n");
        *ret_code = -1;
        return NULL;
    }

    printf("\n%s\n\n", response);

    // verifica status code-ul
    int statusCode = get_status_code(response);
    if (statusCode / 100 != 2) {
        *ret_code = -1;
        return NULL;
    }

    // extrage corpul din mesajul HTTP
    char *body = get_message_body(response);
    if (body == NULL || strlen(body) == 0) {
        printf("\nSomething went wrong, please try again\n\n");
        *ret_code = -1;
        return NULL;
    }

    // extrage token-ul din corp
    token = get_token_from_body(body);
    if (token == NULL || strlen(token) == 0) {
        printf("\nSomething went wrong, please try again\n\n");
        *ret_code = -1;
        return NULL;
    }

    free(response);
    *ret_code = 1;
    return token;
}

void execute_get_books(int isLogedIn, char *token, int *ret_code) {
    char *response;

    if (isLogedIn == 0) {
        printf("\nYou must log in first\n\n");
        *ret_code = -1;
        return;
    }

    if (token == NULL) {
        printf("\nYou must request access to library first\n\n");
        *ret_code = -1;
        return;
    }

    // obtine si valideaza raspunsul obtinut in urma cererii trimise
    response = get_all_books(token);
    if (response == NULL || strlen(response) == 0) {
        printf("\nSomething went wrong, please try again\n\n");
        *ret_code = -1;
        return;
    }

    printf("\n%s\n\n", response);
    free(response);
    *ret_code = 1;
}

void execute_get_book(int isLogedIn, char *token, int *ret_code) {
    char *ret_ptr;
    char *response;

    if (isLogedIn == 0) {
        printf("\nYou must log in first\n\n");
        *ret_code = -1;
        return;
    }

    if (token == NULL) {
        printf("\nYou must request access to library first\n\n");
        *ret_code = -1;
        return;
    }

    // book id
    char bookId[MAX_ID_LEN];
    memset(bookId, 0, MAX_ID_LEN);
    printf("id=");

    ret_ptr = fgets(bookId, MAX_ID_LEN - 1, stdin);
    DIE(ret_ptr == NULL, "fgets");
    bookId[strlen(bookId) - 1] = '\0';

    if (strlen(bookId) == 0) {
        printf("\nId can not be empty\n\n");
        *ret_code = -1;
        return;
    }        

    if (is_positive_number(bookId) == 0) {
        printf("\nThe id must be a positive number\n\n");
        *ret_code = -1;
        return;
    }

    // obtine si valideaza raspunsul obtinut in urma cererii trimise
    int bookIdInt = atoi(bookId);
    response = get_book(token, bookIdInt);
    if (response == NULL || strlen(response) == 0) {
        printf("\nSomething went wrong, please try again\n\n");
        *ret_code = -1;
        return;
    }

    printf("\n%s\n\n", response);
    free(response);
    *ret_code = 1;
}

void execute_add_book(int isLogedIn, char *token, int *ret_code) {
    char *ret_ptr;
    char *response;

    if (isLogedIn == 0) {
        printf("\nYou must log in first\n\n");
        *ret_code = -1;
        return;
    }

    if (token == NULL) {
        printf("\nYou must request access to library first\n\n");
        *ret_code = -1;
        return;
    }

    // title
    char title[MAX_TITLE_LEN];
    memset(title, 0, MAX_TITLE_LEN);
    printf("title=");

    ret_ptr = fgets(title, MAX_TITLE_LEN - 1, stdin);
    DIE(ret_ptr == NULL, "fgets");
    title[strlen(title) - 1] = '\0';

    if (strlen(title) == 0) {
        printf("\nTitle can not be empty\n\n");
        *ret_code = -1;
        return;
    }

    // author
    char author[MAX_AUTHOR_LEN];
    memset(author, 0, MAX_AUTHOR_LEN);
    printf("author=");

    ret_ptr = fgets(author, MAX_AUTHOR_LEN - 1, stdin);
    DIE(ret_ptr == NULL, "fgets");
    author[strlen(author) - 1] = '\0';

    if (strlen(author) == 0) {
        printf("\nAuthor can not be empty\n\n");
        *ret_code = -1;
        return;
    }

    // genre
    char genre[MAX_GENRE_LEN];
    memset(genre, 0, MAX_GENRE_LEN);
    printf("genre=");

    ret_ptr = fgets(genre, MAX_GENRE_LEN - 1, stdin);
    DIE(ret_ptr == NULL, "fgets");
    genre[strlen(genre) - 1] = '\0';

    if (strlen(genre) == 0) {
        printf("\nGenre can not be empty\n\n");
        *ret_code = -1;
        return;
    }

    // publisher
    char publisher[MAX_PUBLISHER_LEN];
    memset(publisher, 0, MAX_PUBLISHER_LEN);
    printf("publisher=");

    ret_ptr = fgets(publisher, MAX_PUBLISHER_LEN - 1, stdin);
    DIE(ret_ptr == NULL, "fgets");
    publisher[strlen(publisher) - 1] = '\0';

    if (strlen(publisher) == 0) {
        printf("\nPublisher can not be empty\n\n");
        *ret_code = -1;
        return;
    }

    // page count
    char pageCount[MAX_PAGE_COUNT_LEN];
    memset(pageCount, 0, MAX_PAGE_COUNT_LEN);
    printf("page_count=");

    ret_ptr = fgets(pageCount, MAX_PAGE_COUNT_LEN - 1, stdin);
    DIE(ret_ptr == NULL, "fgets");
    pageCount[strlen(pageCount) - 1] = '\0';

    if (strlen(pageCount) == 0) {
        printf("\nPage count can not be empty\n\n");
        *ret_code = -1;
        return;
    }

    if (is_positive_number(pageCount) == 0) {
        printf("\nThe page count must be a positive number\n\n");
        *ret_code = -1;
        return;
    }

    int page_count = atoi(pageCount);

    // obtine si valideaza raspunsul obtinut in urma cererii trimise
    response = add_book(token, title, author, genre, page_count, publisher);
    if (response == NULL || strlen(response) == 0) {
        printf("\nSomething went wrong, please try again\n\n");
        *ret_code = -1;
        return;
    }

    printf("\n%s\n\n", response);
    free(response);
    *ret_code = 1;
}

void execute_delete_book(int isLogedIn, char *token, int *ret_code) {
    char *ret_ptr;
    char *response;

    if (isLogedIn == 0) {
        printf("\nYou must log in first\n\n");
        *ret_code = -1;
        return;
    }

    if (token == NULL) {
        printf("\nYou must request access to library first\n\n");
        *ret_code = -1;
        return;
    }

    // book id
    char bookId[MAX_ID_LEN];
    memset(bookId, 0, MAX_ID_LEN);
    printf("id=");

    ret_ptr = fgets(bookId, MAX_ID_LEN - 1, stdin);
    DIE(ret_ptr == NULL, "fgets");
    bookId[strlen(bookId) - 1] = '\0';

    if (strlen(bookId) == 0) {
        printf("\nId can not be empty\n\n");
        *ret_code = -1;
        return;
    }        

    if (is_positive_number(bookId) == 0) {
        printf("\nThe id must be a positive number\n\n");
        *ret_code = -1;
        return;
    }

    // obtine si valideaza raspunsul obtinut in urma cererii trimise
    int bookIdInt = atoi(bookId);
    response = delete_book(token, bookIdInt);
    if (response == NULL || strlen(response) == 0) {
        printf("\nSomething went wrong, please try again\n\n");
        *ret_code = -1;
        return;
    }

    printf("\n%s\n\n", response);
    free(response);
    *ret_code = 1;
}

void execute_logout(int isLogedIn, char *session_cookie, int *ret_code) {
    char *response;

    if (isLogedIn == 0) {
        printf("\nYou must log in first\n\n");
        *ret_code = -1;
        return;
    }

    // obtine si valideaza raspunsul obtinut in urma cererii trimise
    response = logout(session_cookie);
    if (response == NULL || strlen(response) == 0) {
        printf("\nSomething went wrong, please try again\n\n");
        *ret_code = -1;
        return;
    }

    int statusCode = get_status_code(response);
    if (statusCode / 100 != 2) {
        printf("\n%s\n\n", response);
        *ret_code = -1;
        return;
    }

    printf("\n%s\n\n", response);
    free(response);
    *ret_code = 1;
}

int execute_command_from_stdin() {
    // variabile utilizate pentru a mentine statusul utilizatorului
    static int isLogedIn = 0;
    static char *session_cookie = NULL;
    static char *token = NULL;

    int ret_code = 1;

    char *ret_ptr = NULL;

    char command[MAX_COMMAND_LEN];
    memset(command, 0, MAX_COMMAND_LEN);

    ret_ptr = fgets(command, MAX_COMMAND_LEN - 1, stdin);
    DIE(ret_ptr == NULL, "fgets");
    command[strlen(command) - 1] = '\0';

    if (strcmp(command, REGISTER_CMD) == 0) {
        execute_register(&ret_code);
    } else if (strcmp(command, LOGIN_CMD) == 0) {
        char *local_cookie = execute_login(isLogedIn, &ret_code);
        if (ret_code == 1) {
            isLogedIn = 1;
            session_cookie = local_cookie;
        }
    } else if (strcmp(command, ENTER_CMD) == 0) {
        char *local_token = execute_enter_library(isLogedIn, session_cookie,
                                                                    &ret_code);
        if (ret_code == 1) {
            token = local_token;
        }
    } else if (strcmp(command, GET_BOOKS_CMD) == 0) {
        execute_get_books(isLogedIn, token, &ret_code);
    } else if (strcmp(command, GET_BOOK_CMD) == 0) {
        execute_get_book(isLogedIn, token, &ret_code);
    } else if (strcmp(command, ADD_BOOK_CMD) == 0) {
        execute_add_book(isLogedIn, token, &ret_code);
    } else if (strcmp(command, DELETE_BOOK_CMD) == 0) {
        execute_delete_book(isLogedIn, token, &ret_code);
    } else if (strcmp(command, LOGOUT_CMD) == 0) {
        execute_logout(isLogedIn, session_cookie, &ret_code);
        if (ret_code == 1) {
            isLogedIn = 0;
            session_cookie = NULL;
            token = NULL;
        }
    } else if (strcmp(command, EXIT_CMD) == 0) {
        ret_code = 0;
    } else {
        printf("\nComanda este invalida\n\n");
        ret_code = -1;
    }

    return ret_code;
}
