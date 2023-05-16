#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kmp.h"

int* prefix_func(char pattern[], size_t size)
{
    int* pi = (int*)calloc(size, sizeof(int));
    pi[0] = 0;
    for (int i = 1, j = 0; i < size; i++) {
        if (pattern[j] == '*' || pattern[j] == '\\') {
            pi[j++] = 0;
            continue;
        }

        while (j > 0 && pattern[j] != pattern[i] && pattern[j] != '.')
            j = pi[j - 1];

        if (pattern[j] == pattern[i] || pattern[j] == '.')
            j++;

        pi[i] = j;
    }
    return pi;
}

int kmp_matcher(char text[], char pattern[], int* len)
{
    int i, j, k = 0;
    size_t text_len = strlen(text);
    size_t pattern_len = strlen(pattern);

    int* pi = prefix_func(pattern, pattern_len);

    int esc_sym = 0;
    for (i = 0, j = 0; i < text_len; i++) {
        while (j > 0 && pattern[j] != text[i] && (pattern[j] != '.' || esc_sym)
               && (pattern[j + 1] != '*' || esc_sym)
               && (pattern[j] != '\\' || esc_sym)) {
            j = pi[j - 1];
            k = 0;
            if (esc_sym)
                i++;
            esc_sym = 0;
        }
        if (pattern[j] == '\\' && pattern[j - 1] != '\\') {
            j++;
            k--;
            i--;
            if (pattern[j + 1] != '*')
                esc_sym = 1;
        } else if (pattern[j + 1] == '*') {
            int i_before = i;
            if (pattern[j] == '.' && esc_sym == 0) {
                while (i < text_len && text[i] != pattern[j + 2]) {
                    i++;
                    k++;
                }
            } else {
                while (text[i] == pattern[j]) {
                    i++;
                    k++;
                }
            }
            if (i == i_before) {
                int jj = 0;
                for (jj = j; jj < pattern_len; jj++) {
                    if (pattern[jj] == '\\' && pattern[jj + 1] == text[i]) {
                        jj++;
                        break;
                    } else if (
                            text[i] != pattern[jj]
                            && (pattern[jj + 1] != '*' || pattern[jj] == '\\')
                            && pattern[jj] != '.') {
                        jj = 0;
                        break;
                    } else if (
                            text[i] != pattern[jj] && pattern[jj + 1] == '*') {
                        jj++;
                        continue;
                    } else if (text[i] == pattern[jj] || pattern[jj] == '.') {
                        break;
                    }
                }
                jj = jj == pattern_len ? 0 : jj;
                if (jj != 0) {
                    k -= jj - j;
                    j = jj;
                    i--;
                } else if (jj == 0 && j != 0) {
                    int need_skip = 0;
                    for (int jj = j; jj < pattern_len; jj++) {
                        if (pattern[jj + 1] != '*' || pattern[jj] == '\\') {
                            need_skip = 1;
                            break;
                        } else {
                            jj++;
                        }
                    }
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
        } else if (
                pattern[j] == text[i] || (pattern[j] == '.' && esc_sym == 0)) {
            j++;
            esc_sym = 0;
        }
        if (j == pattern_len) {
            *len = j + k;
            return i - j - k + 1;
        }
    }

    return -1;
}
