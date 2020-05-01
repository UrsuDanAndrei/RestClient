#ifndef __USER_H__
#define __USER_H__

#include "utils.h"

#include <string.h>

typedef struct user_struct user;

user* create_user(const char *username, const char *password);
user* find_user(const char *username, const char *password, const user** users, int users_count);
user* copy_user(user *other);
void add_session_cookie_to_user(user *usr, char *session_cookie);
void delete_user(user *usr);

#endif // __USER_H__