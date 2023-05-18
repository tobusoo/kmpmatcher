#pragma once

#include <dirent.h>

#define DEFAULT_COLOR "\e[0m"
#define RED_COLOR "\e[1;31m"
#define RED_COLOR_STR(string) "\e[1;31m" string DEFAULT_COLOR

typedef enum {
    FIRST_STAR,
    SINGLE_BACKSLASH_END,
    TWO_STAR,
    CANT_OPEN_DIR,
    CANT_OPEN_FILE,
} Error;

void print_error(char* string, int status, int column, int pointer_len);
void print_usage_message(char* app_name);
int skipp_file(struct dirent* file);
void check_pattern(char* pattern);
