#include "user.h"

struct user_struct {
    char *name;
    char *password;
    char *session_cookie;
};

user* create_user(const char *username, const char *password) {
    user* usr = calloc(1, sizeof(user));
    DIE(usr == NULL, "calloc");

    usr->name = calloc(strlen(username) + 1, sizeof(char));
    usr->password = calloc(strlen(password) + 1, sizeof(char));

    memcpy(usr->name, username, strlen(username));
    memcpy(usr->password, password, strlen(password));

    return usr;
}

user* find_user(const char *username, const char *password, user** users, int users_count) {
    for (int i = 0; i < users_count; ++i) {
        if (strcmp(users[i]->name, username) == 0) {
            if (strcmp(users[i]->password, password) == 0) {
                return users[i];
            }
        }
    }

    return NULL;
}

user* copy_user(user *other) {
    user* usr = calloc(1, sizeof(user));
    DIE(usr == NULL, "calloc");

    usr->name = calloc(strlen(other->name) + 1, sizeof(char));
    usr->password = calloc(strlen(other->password) + 1, sizeof(char));

    memcpy(usr->name, other->name, strlen(other->name));
    memcpy(usr->password, other->password, strlen(other->password));

    return usr;
}

void add_session_cookie_to_user(user *usr, char *session_cookie) {
    usr->session_cookie = calloc(strlen(session_cookie) + 1, sizeof(char));
    memcpy(usr->session_cookie, session_cookie, strlen(session_cookie));
}

void delete_user(user *usr) {
    if (usr == NULL) {
        return;
    }

    if (usr->name != NULL) {
        free(usr->name);
    }

    if (usr->password != NULL) {
        free(usr->password);
    }

    if (usr->session_cookie != NULL) {
        free(usr->session_cookie);
    }

    free(usr);
}

