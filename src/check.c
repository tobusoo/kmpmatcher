#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "check.h"
#include "log.h"

char* get_suffix(char* file_name)
{
    while (*file_name != '\0') {
        if (*file_name == '.')
            return file_name;
        file_name++;
    }

    return NULL;
}

void print_error(char* string, int status, int column, int pointer_len)
{
    switch (status) {
    case FIRST_STAR:
        printf(RED_COLOR_STR("Error") " at pattern: Before the metacharacters '*' there must be some character\n");
        log_write(
                MESSAGE,
                "[ERROR] at pattern: Before the metacharacters '*' there must "
                "be "
                "some character\n\n\n");
        break;
    case SINGLE_BACKSLASH_END:
        printf(RED_COLOR_STR("Error") " at pattern: The character '\\' should be followed by some character\n");
        log_write(
                MESSAGE,
                "[ERROR] at pattern: The character '\\' should be followed by "
                "some character\n\n\n");
        break;
    case TWO_STAR:
        printf(RED_COLOR_STR("Error") " at pattern: Two '*' characters can't follow each other\n");
        log_write(
                MESSAGE,
                "[ERROR] at pattern: Two '*' characters can't follow each "
                "other\n\n\n");
        break;
    case CANT_OPEN_DIR:
        printf("\e[1;31mError\e[0m: can't open \"%s\" directory\n", string);
        log_write(MESSAGE, "[ERROR] can't open \"%s\" directory\n\n\n", string);
        exit(EXIT_FAILURE);
    case CANT_OPEN_FILE:
        printf("\e[1;31mError\e[0m: can't open \"%s\" directory\n", string);
        log_write(MESSAGE, "[ERROR] can't open \"%s\" directory\n\n\n", string);
        exit(EXIT_FAILURE);
    }
    printf(RED_COLOR_STR("\t%s\n\t"), string);

    int i;
    printf(RED_COLOR);

    for (i = 0; i < column; i++)
        putchar('~');

    for (int i = 0; i < pointer_len; i++)
        putchar('^');

    size_t str_len = strlen(string);
    for (; i < str_len - pointer_len; i++)
        putchar('~');

    printf(DEFAULT_COLOR "\n");
    exit(EXIT_FAILURE);
}

void print_usage_message(char* app_name)
{
    printf("Usage:\n%s [-r] <match> <dir>\n", app_name);
    exit(EXIT_FAILURE);
}

int skipp_file(struct dirent* file)
{
    char* suffix = get_suffix(file->d_name);

    if (strcmp(file->d_name, ".") == 0)
        return -1;

    if (strcmp(file->d_name, "..") == 0)
        return -1;

    if ((suffix == NULL || strcmp(suffix, ".txt") != 0)
        && (file->d_type & DT_REG) == DT_REG) {
        printf("\e[1;33m[INFO]\e[0m Skipped file \e[1;35m%s\e[0m\n",
               file->d_name);
        log_write(MESSAGE, "[INFO] Skipped file \"%s\"\n", file->d_name);
        return 1;
    }

    return 0;
}

void write_correct_pattern(char* buf, char* pattern, int i)
{
    strcpy(buf + i, pattern + i + 1);
}

int check_two_start1(char* pattern)
{
    return (pattern[0] != '\\' && pattern[1] == '*' && pattern[2] == '*');
}

int check_two_start2(char* pattern)
{
    return (pattern[0] == '\\' && pattern[2] == '*' && pattern[3] == '*');
}

void check_pattern(char* pattern)
{
    size_t len = strlen(pattern);
    char buf[len + 1];
    char patternbuf[len + 1];
    strcpy(buf, pattern);
    strcpy(patternbuf, pattern);

    if (pattern[0] == '*')
        print_error(pattern, FIRST_STAR, 0, 1);
    else if (pattern[len - 1] == '\\' && pattern[len - 2] != '\\' && len > 1)
        print_error(pattern, SINGLE_BACKSLASH_END, len - 1, 1);
    else if (len == 1 && pattern[0] == '\\')
        print_error(pattern, SINGLE_BACKSLASH_END, 0, 1);

    if (len < 2) {
        return;
    }

    for (int i = 0; i < len - 2; i++) {
        if (check_two_start1(pattern + i)) {
            strcpy(pattern, patternbuf);
            print_error(pattern, TWO_STAR, i + 1, 2);
        } else if (check_two_start2(pattern + i)) {
            strcpy(pattern, patternbuf);
            print_error(pattern, TWO_STAR, i + 2, 2);
        }
        if (pattern[i] == '\\' && pattern[i + 2] == '*'
            && pattern[i - 1] != '\\' && pattern[i + 1] != '.'
            && pattern[i + 1] != '*' && pattern[i + 1] != '\\') {
            write_correct_pattern(buf, pattern, i);
            strcpy(pattern, buf);
        }
    }
}
