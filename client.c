#include "client.h"

int main(int argc, char *argv[]) {
    int ret_code;
    while (1) {
        ret_code = execute_command_from_stdin();
        if (ret_code == 0) {
            return 0;
        }
    }

    return 0;
    
// //  register_account(USERNAME, PASSWORD);
//     char* session_cookie = login(USERNAME, PASSWORD);
//     printf("\nSession cookie \n%s\n", session_cookie);

//     char *token = get_library_access(session_cookie);

//     get_all_books(token);
//     add_book(token, "tristete", "eu si numai eu", "drama", 1000, "nepublicata");
//     get_all_books(token);

//     delete_book(token, 443);
//     get_all_books(token);

//     get_book(token, 266);
//     logout(session_cookie);

//     return 0;
}
