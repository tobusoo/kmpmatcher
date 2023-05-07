#include <dirent.h>
#include <glib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "kmp.h"

#define TIME 0
#define MESSAGE 1
#define MAX_LEN 260
#define MAX_MESSAGE_LEN 1024

#define NOT_RECURSIVE 0
#define RECURSIVE 1

void log_write(int status, char* format, ...);

void highlite_word(char* str, int col, size_t str_len)
{
    printf("\e[1;32;4m");
    for (int j = col; j < str_len; j++) {
        putchar(str[j]);
        log_write(MESSAGE, "%c", str[j]);
    }
    printf("\e[0m");
}

char* get_suffix(char* file_name)
{
    while (*file_name != '\0') {
        if (*file_name == '.')
            return file_name;
        file_name++;
    }

    return NULL;
}

void process_file(char* path, char* pattern, char* name)
{
    FILE* file = fopen(path, "rb");
    char word[MAX_LEN * 4] = {0};
    int cnt = 0, len = 0, line = 0, column = 0;

    printf("\e[1;35m%s\e[0m:\n", name);
    log_write(MESSAGE, "In file '%s':\n", path);

    while (fgets(word, MAX_LEN * 4, file) != NULL) {
        size_t word_size = strlen(word);
        line++;
        column = seek_substring_KMP(word, pattern, &len);

        for (int i = 0; i < word_size; i++) {
            if (i == column) {
                cnt++;
                log_write(MESSAGE, "Ln %d, Col %d word: \"", line, column + 1);
                highlite_word(word, column, column + len);
                log_write(MESSAGE, "\"\n");

                i = len + column;
                column = seek_substring_KMP(word + i, pattern, &len) + i;
                i--;
            } else {
                putchar(word[i]);
            }
        }
    }
    printf("\n");

    if (cnt == 0)
        log_write(MESSAGE, "No results found\n");

    free(path);
    fclose(file);
}

gint my_comparator(gconstpointer item1, gconstpointer item2)
{
    return g_ascii_strcasecmp(item1, item2);
}

char* get_path(char* path, char* name)
{
    char* file_path = malloc(sizeof(char) * MAX_LEN);

    strncpy(file_path, path, MAX_LEN);
    strcat(file_path, "/");
    strncat(file_path, name, MAX_LEN);

    return file_path;
}

void process_dirs(char* pattern, char* path, int is_recursive)
{
    DIR* dir = opendir(path);
    if (!dir) {
        printf("\e[1;31mError\e[0m: can't open \"%s\" directory\n", path);
        log_write(MESSAGE, "[ERROR] can't open \"%s\" directory\n", path);
        return;
    }

    printf("in dir \e[1;34m%s\e[0m:\n", path);
    struct dirent* file;
    GList* dir_list = NULL;

    while ((file = readdir(dir)) != NULL) {
        char* name = get_suffix(file->d_name);
        if (strcmp(file->d_name, ".") == 0)
            continue;
        else if (strcmp(file->d_name, "..") == 0)
            continue;
        else if (
                name != NULL && strcmp(name, ".txt") != 0
                && (file->d_type & DT_REG) == DT_REG) {
            printf("\e[1;33m[INFO]\e[0m Skipped file \e[1;35m%s\e[0m\n",
                   file->d_name);
            log_write(MESSAGE, "[INFO] Skipped file \"%s\"\n", file->d_name);
            continue;
        }

        char* file_path = get_path(path, file->d_name);

        if ((file->d_type & DT_DIR) == DT_DIR) {
            dir_list = g_list_append(dir_list, file_path);
        } else if ((file->d_type & DT_REG) == DT_REG) {
            if (name == NULL) {
                printf("\e[1;33m[INFO]\e[0m Skipped file \e[1;35m%s\e[0m\n",
                       file->d_name);
                log_write(MESSAGE, "[INFO] Skipped file \"%s\"\n", file_path);
                continue;
            }
            process_file(file_path, pattern, file->d_name);
        }
    }

    if (is_recursive) {
        dir_list = g_list_sort(dir_list, my_comparator);

        for (GList* i = dir_list; i != NULL; i = i->next) {
            printf("\n");
            process_dirs(pattern, (char*)i->data, 1);
        }
    }

    closedir(dir);
    g_list_free_full(dir_list, free);
}

void log_write(int status, char* format, ...)
{
    FILE* out = fopen("logs/log.log", "a+");

    if (status == TIME) {
        struct tm* date;
        time_t ltime;
        char buf[30] = {0};

        ltime = time(NULL);
        date = localtime(&ltime);

        strftime(buf, 30, "%d %b %Y %H:%M:%S", date);
        fprintf(out, "[%s]\n", buf);
    } else {
        va_list arg_ptr;
        va_start(arg_ptr, format);
        char buffer[MAX_MESSAGE_LEN];

        vsnprintf(buffer, MAX_MESSAGE_LEN, format, arg_ptr);
        fprintf(out, "%s", buffer);

        va_end(arg_ptr);
    }

    fclose(out);
}

void print_usage_message(char* app_name)
{
    printf("Usage:\n%s [-r] <match> <dir>\n", app_name);
    exit(EXIT_FAILURE);
}

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
    }

    printf("Searching for \"\e[1;32m%s\e[0m\"\n\n", pattern);
    log_write(TIME, NULL);
    log_write(MESSAGE, "Searching for \"%s\"\n", pattern);

    process_dirs(pattern, directory, is_recursive);

    log_write(MESSAGE, "\n\n");

    return 0;
}