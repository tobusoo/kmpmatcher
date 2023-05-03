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

void process_file(char* path, char* pattern, char* name)
{
    FILE* f = fopen(path, "rb");
    char word[MAX_LEN * 4] = {0};
    int line = 0;
    int cnt = 0;
    int len = 0, col = 0;

    printf("\e[1;35m%s\e[0m:\n", name);
    log_write(MESSAGE, "'%s' In file '%s':\n", pattern, path);

    while (fgets(word, MAX_LEN * 4, f) != NULL) {
        size_t word_size = strlen(word);
        line++;
        col = seek_substring_KMP(word, pattern, &len);

        for (int i = 0; i < word_size; i++) {
            if (i == col) {
                log_write(MESSAGE, "Ln %d, Col %d word: ", line, col + 1);
                highlite_word(word, col, col + len);
                log_write(MESSAGE, "\n");

                i = len + col - 1;
                col = seek_substring_KMP(word + i, pattern, &len) + i;

            } else {
                putchar(word[i]);
            }
        }
    }
    printf("\n");

    if (cnt == 0)
        log_write(MESSAGE, "No results found\n");
    fclose(f);
}

gint my_comparator(gconstpointer item1, gconstpointer item2)
{
    return g_ascii_strcasecmp(item1, item2);
}

void process_dirs_r(char* path, char* pattern)
{
    DIR* dir = opendir(path);
    if (!dir) {
        printf("\e[1;31mОшибка\e[0m: не удалось открыть директорию %s\n", path);
        return;
    }
    printf("in dir \e[1;34m%s\e[0m:\n", path);
    struct dirent* file;
    GList* dir_list = NULL;

    while ((file = readdir(dir)) != NULL) {
        if (strcmp(file->d_name, ".") == 0)
            continue;
        else if (strcmp(file->d_name, "..") == 0)
            continue;

        char* file_path = malloc(sizeof(char) * MAX_LEN);
        strncpy(file_path, path, MAX_LEN);
        strcat(file_path, "/");
        strncat(file_path, file->d_name, MAX_LEN);
        if ((file->d_type & DT_DIR) == DT_DIR) {
            dir_list = g_list_append(dir_list, file_path);
        } else if ((file->d_type & DT_REG) == DT_REG) {
            process_file(file_path, pattern, file->d_name);
        }
    }

    dir_list = g_list_sort(dir_list, my_comparator);
    for (GList* i = dir_list; i != NULL; i = i->next) {
        printf("\n");
        process_dirs_r((char*)i->data, pattern);
    }
    closedir(dir);
    g_list_free(dir_list);
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

int main(int argc, char* argv[])
{
    if (argc == 3) {
        printf("%s\n", argv[2]);
        log_write(TIME, NULL);
        process_dirs_r(argv[1], argv[2]);
        log_write(MESSAGE, "\n\n");
    } else {
        printf("Usage:\n%s <dir> <match>\n", argv[0]);
    }

    return 0;
}