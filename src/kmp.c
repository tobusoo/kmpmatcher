#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kmp.h"

int* prefix_func(char pattern[], size_t size)
{
    int* pi = (int*)malloc(size * sizeof(int));
    pi[0] = 0;
    for (int q = 1, k = 0; q < size; q++) {
        if (pattern[q] == '*' || pattern[q] == '\\' || pattern[q] == '.') {
            pi[q] = 0;
            k = 0;
            continue;
        }

        while (k > 0 && pattern[k] != pattern[q])
            k = pi[k - 1];

        if (pattern[k] == pattern[q])
            k++;

        pi[q] = k;
    }

    return pi;
}

int find_index(char* text, char* pattern, int* status, int esc_s, int i, int j)
{
    size_t pattern_len = strlen(pattern);
    int jj = 0;
    for (jj = j; jj < pattern_len; jj++) {
        if ((pattern[jj] == '.' && pattern[jj + 1] == '*' && esc_s)
            || (pattern[jj] == '\\' && pattern[jj + 1] == '*' && esc_s)) {
            *status = jj;
            jj += 2;
            break;
        } else if (pattern[jj] == '\\' && pattern[jj + 1] == text[i]) {
            jj++;
            break;
        } else if (
                text[i] != pattern[jj]
                && (pattern[jj + 1] != '*' || pattern[jj] == '\\')
                && pattern[jj] != '.') {
            jj = 0;
            break;
        } else if (text[i] != pattern[jj] && pattern[jj + 1] == '*') {
            jj++;
            continue;
        } else if (text[i] == pattern[jj] || pattern[jj] == '.')
            break;
    }

    return jj;
}

int is_need_skip(char* pattern, size_t pattern_len, int j)
{
    for (int jj = j; jj < pattern_len; jj++) {
        if (pattern[jj + 1] != '*' || pattern[jj] == '\\') {
            return 1;
        } else {
            jj++;
        }
    }

    return 0;
}

// condition for metacharacters...
int cnd_mch(char* pattern, int esc_sym, int j)
{
    int a = (pattern[j] != '.' || (esc_sym && pattern[j + 1] != '*'));
    int b = (pattern[j + 1] != '*' || esc_sym);
    int c = (pattern[j] != '\\' || (esc_sym && pattern[j + 1] != '*'));
    return a && b && c;
}

void find_n_ch_point(char* text, char* pattern, int* i, int* k, int j)
{
    size_t text_len = strlen(text);
    while (*i < text_len && text[*i] != pattern[j + 2]) {
        *i += 1;
        *k += 1;
    }
}

void find_n_ch(char* text, char* pattern, int* i, int* k, int j)
{
    size_t text_len = strlen(text);
    while (*i < text_len && text[*i] == pattern[j]) {
        *i += 1;
        *k += 1;
    }
}

int kmp_matcher(char text[], char pattern[], int* len)
{
    int i, j, k = 0;
    size_t text_len = strlen(text);
    size_t pattern_len = strlen(pattern);

    int* pi = prefix_func(pattern, pattern_len);

    int esc_s = 0;
    for (i = 0, j = 0; i < text_len; i++) {
        while (j > 0 && pattern[j] != text[i] && cnd_mch(pattern, esc_s, j)) {
            j = pi[j - 1];
            k = 0;
            if (esc_s)
                i++;
            esc_s = 0;
        }
        if (pattern[j] == '\\' && pattern[j - 1] != '\\') {
            j++;
            k--;
            i--;
            if (pattern[j + 1] != '*' || pattern[j] != '*')
                esc_s = 1;
            else if (pattern[j] == '.' && pattern[j + 1] == '*') {
                esc_s = 1;
            }
        } else if (pattern[j + 1] == '*') {
            int i_before = i;
            if (pattern[j] == '.' && esc_s == 0) {
                find_n_ch_point(text, pattern, &i, &k, j);
            } else {
                find_n_ch(text, pattern, &i, &k, j);
            }
            if (i == i_before) {
                int jj = 0;
                int status = 0;
                jj = find_index(text, pattern, &status, esc_s, i, j);
                jj = jj == pattern_len ? 0 : jj;
                if (status && jj == 0) {
                    k -= 2;
                    j = pattern_len;
                    i--;
                } else if (jj != 0) {
                    k -= jj - j;
                    j = jj;
                    i--;
                } else if (jj == 0 && j != 0) {
                    int need_skip = is_need_skip(pattern, pattern_len, j);
                    if (need_skip) {
                        j = 0;
                        k = 0;
                    } else {
                        k -= pattern_len - j;
                        i--;
                        j = pattern_len;
                    }
                }
            } else {
                j += 2;
                i--;
                k -= 2;
            }
        } else if (pattern[j] == text[i] || (pattern[j] == '.' && !esc_s)) {
            j++;
            esc_s = 0;
        }
        if (j == pattern_len) {
            *len = j + k;
            free(pi);
            return i - j - k + 1;
        }
    }

    free(pi);
    return -1;
}
