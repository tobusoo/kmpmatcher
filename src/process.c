#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "check.h"
#include "kmp.h"
#include "list.h"
#include "log.h"
#include "process.h"

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
    FILE* file = fopen(path, "rb");
    char* word = malloc(sizeof(char) * MAX_LEN * 4);
    int cnt = 0, len = 0, line = 0, column = 0;

    printf("\e[1;35m%s\e[0m:\n", name);
    log_write(MESSAGE, "In file \"%s\":\n", name);

    while (fgets(word, MAX_LEN * 4, file) != NULL) {
        size_t word_size = strlen(word);
        line++;
        column = kmp_matcher(word, pattern, &len);

        for (int i = 0; i < word_size; i++) {
            if (i == column) {
                cnt++;
                log_write(MESSAGE, "Ln %d, Col %d word: \"", line, column + 1);
                highlite_word(word, column, column + len);
                log_write(MESSAGE, "\"\n");

                i = len + column;
                column = kmp_matcher(word + i, pattern, &len) + i;
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
