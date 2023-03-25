#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

#include "list.h"

void read_dir(char *path, char *shablon);

void print_name(Item item, char *shablon)
{
    printf("in dir \e[1;34m%s\e[0m:\n", item.path);
    read_dir(item.path, shablon);
}

int seek_substring_KMP(char s[], char p[])
{
    int i, j, N, M;
    N = strlen(s);
    M = strlen(p);
    // printf("\nmda1: %s\nmda2: %s\n", s, p);

    // Динамический массив длины М
    int *d = (int *)malloc(M * sizeof(int));

    // Вычисление префикс-функции
    d[0] = 0;
    for (i = 1, j = 0; i < M; i++)
    {
        while (j > 0 && p[j] != p[i])
            j = d[j - 1];
        if (p[j] == p[i])
            j++;
        d[i] = j;
    }

    // Поиск
    for (i = 0, j = 0; i < N; i++)
    {
        while (j > 0 && p[j] != s[i])
            j = d[j - 1];
        if (p[j] == s[i])
            j++;
        if (j == M)
        {
            // printf("Смещение %d\n", i - j + 1);
            return i - j + 1;
        }
    }
    free(d);
    return -1;
}

void read_dir(char *path, char *shablon)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        printf("Не удалось открыть директорию %s\n", path);
        return;
    }
    struct dirent *file;
    List dir_list;
    Item item;
    init_list(&dir_list);

    while ((file = readdir(dir)) != NULL)
    {
        if (strcmp(file->d_name, ".") == 0)
            continue;
        if (strcmp(file->d_name, "..") == 0)
            continue;
        if (strcmp(file->d_name, ".git") == 0)
            continue;
        // if (strcmp(file->d_name, "list.h") == 0)
        //     continue;
        // if (strcmp(file->d_name, "list.c") == 0)
        //     continue;
        // if (strcmp(file->d_name, "main.c") == 0)
        // continue;
        if (strcmp(file->d_name, "app") == 0)
            continue;
        if (strcmp(file->d_name, ".gitignore") == 0)
            continue;
        char *path_ = malloc(sizeof(*path) * 256);
        strncpy(path_, path, 256);
        strncat(path_, "/", 2);
        strncat(path_, file->d_name, 254);
        if ((file->d_type & DT_DIR) == DT_DIR)
        {
            item.path = path_;
            add_item_head(&dir_list, item);
        }
        else
        {
            printf("\e[1;35m%s\e[0m:\n", file->d_name);
            FILE *f = fopen(path_, "rb");
            char word[256] = {0};
            while (fgets(word, 256, f) != NULL)
            {
                int k = seek_substring_KMP(word, shablon);
                for (int i = 0; i < strlen(word); i++)
                {
                    int flag = 0;
                    if (i == k)
                    {
                        printf("\e[1;32;4m");
                        for (int j = k; j < k + strlen(shablon); j++)
                            putchar(word[j]);
                        printf("\e[0m");
                        i += strlen(shablon);
                        // printf("\nk before = %d", k);
                        int t = seek_substring_KMP(word + k + 1, shablon);
                        // printf("\tt = %d\n", t);
                        if (t != -1)
                        {
                            k += t + 1;
                        }
                        if (t == (strlen(shablon) - 1))
                        {
                            // k += 1;
                            i--;
                            flag = 1;
                        }

                        // printf(" i = %d k = %d t = %d\n", i, k, t);
                    }
                    if (flag == 0)
                        putchar(word[i]);
                }
            }
            printf("\n");
            fclose(f);
        }
    }

    printf("\n\n");
    traverse(&dir_list, print_name, shablon);
}

int main(int argc, char *argv[])
{

    if (argc == 2)
    {
        printf("in dir \e[1;34m.\e[0m:\n");
        read_dir(".", argv[1]);
    }
    return 0;
}