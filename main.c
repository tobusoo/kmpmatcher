#include <dirent.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "kmp.h"
#include "list.h"

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

char* skip_space(char* str)
{
    char* temp = str;
    while (*temp == ' ') {
        temp++;
    }

    return temp;
}

void process_file(char* path, char* pattern, char* name)
{
    FILE* file = fopen(path, "rb");
    char* word = malloc(sizeof(char) * MAX_LEN * 4);
    int cnt = 0, len = 0, line = 0, column = 0;

    printf("\e[1;35m%s\e[0m:\n", name);
    log_write(MESSAGE, "In file \"%s\":\n", name);

    while (fgets(word, MAX_LEN * 4, file) != NULL) {
        // word = skip_space(word);
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
                // word = skip_space(word);
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

    free(word);
    free(path);
    fclose(file);
}

char* get_path(char* path, char* name)
{
    char* file_path = malloc(sizeof(char) * MAX_LEN);

    strncpy(file_path, path, MAX_LEN);
    strcat(file_path, "/");
    strncat(file_path, name, MAX_LEN);

    return file_path;
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

void process_dirs(char* pattern, char* path, int is_recursive)
{
    if (strcmp(path, "..") == 0) {
        printf("\e[1;31mERROR\e[0m: don't use \"%s\" directory\n", path);
        log_write(MESSAGE, "[ERROR] don't use \"%s\" directory\n", path);
        return;
    }
    DIR* dir = opendir(path);
    if (!dir) {
        printf("\e[1;31mError\e[0m: can't open \"%s\" directory\n", path);
        log_write(MESSAGE, "[ERROR] can't open \"%s\" directory\n", path);
        return;
    }

    printf("In dir \e[1;34m%s\e[0m:\n", path);
    log_write(MESSAGE, "In dir \"%s\":\n", path);
    struct dirent* file;
    List* dir_list = NULL;
    int file_cnt = 0;

    while ((file = readdir(dir)) != NULL) {
        int is_skipp = skipp_file(file);
        if (is_skipp == -1) {
            continue;
        } else if (is_skipp == 1) {
            file_cnt++;
            continue;
        }

        char* file_path = get_path(path, file->d_name);

        if ((file->d_type & DT_DIR) == DT_DIR) {
            dir_list = list_add(dir_list, file_path);
        } else if ((file->d_type & DT_REG) == DT_REG) {
            process_file(file_path, pattern, file->d_name);
        }
    }

    if (file_cnt == 0) {
        printf("\e[1;33m[INFO]\e[0m Empty directory\n");
        log_write(MESSAGE, "[INFO] Empty directory\n");
    }

    if (is_recursive) {
        list_sort(dir_list);

        for (List* i = dir_list; i != NULL; i = i->next) {
            printf("\n");
            process_dirs(pattern, i->string, RECURSIVE);
        }
    }

    closedir(dir);
    list_delete(dir_list);
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

    log_write(TIME, NULL);
    printf("Searching for \"\e[1;32m%s\e[0m\"\n\n", pattern);
    log_write(MESSAGE, "Searching for \"%s\"\n", pattern);

    process_dirs(pattern, directory, is_recursive);

    log_write(MESSAGE, "\n\n");

    return 0;
}