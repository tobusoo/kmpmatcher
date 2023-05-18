#include <stdio.h>
#include <string.h>

#include "check.h"
#include "log.h"
#include "process.h"

int main(int argc, char* argv[])
{
    int is_recursive = NOT_RECURSIVE;
    char* directory = argv[2];
    char* pattern = argv[1];

    if (argc != 4 && argc != 3) {
        print_usage_message(argv[0]);
    } else if (strcmp(argv[1], "-r") == 0) {
        if (argc == 4) {
            is_recursive = RECURSIVE;
            directory = argv[3];
            pattern = argv[2];
        } else
            print_usage_message(argv[0]);
    } else if (argc == 4) {
        print_usage_message(argv[0]);
    }

    log_write(TIME, NULL);
    printf("Searching for \"\e[1;32m%s\e[0m\"\n\n", pattern);
    log_write(MESSAGE, "Searching for \"%s\"\n", pattern);
    check_pattern(pattern);

    process_dirs(pattern, directory, is_recursive);
    log_write(MESSAGE, "\n\n");
    return 0;
}
