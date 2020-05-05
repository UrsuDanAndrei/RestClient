#include "client.h"

int main(int argc, char *argv[]) {
    int ret_code;
    while (1) {
        // daca ret_code este 0, s-a dat comanda exit la stdin
        ret_code = execute_command_from_stdin();
        if (ret_code == 0) {
            return 0;
        }
    }

    return 0;
}
