#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* prefix_func(char pattern[], size_t size)
{
    int* pi = (int*)calloc(size, sizeof(int));
    pi[0] = 0;
    for (int i = 1, j = 0; i < size; i++) {
        if (pattern[j] == '*' || pattern[j] == '\\') {
            pi[j++] = 0;
            continue;
        }

        while (j > 0 && pattern[j] != pattern[i])
            j = pi[j - 1];

        if (pattern[j] == pattern[i]
            || (pattern[j] == '.' && pattern[j + 1] != '*'))
            j++;

        pi[i] = j;
    }
    return pi;
}

int seek_substring_KMP(char text[], char pattern[], int* len)
{
    int i, j, k = 0;
    size_t text_len = strlen(text);
    size_t pattern_len = strlen(pattern);

    int* pi = prefix_func(pattern, pattern_len);

    int flag = 0;
    for (i = 0, j = 0; i < text_len; i++) {
        while (j > 0 && pattern[j] != text[i] && (pattern[j] != '.' || flag)
               && (pattern[j + 1] != '*' || flag)
               && (pattern[j] != '\\' || flag)) {
            j = pi[j - 1];
            k = 0;
            if (flag)
                i++;
            flag = 0;
        }
        if (pattern[j] == '\\' && pattern[j - 1] != '\\') {
            j++;
            k--;
            i--;
            if (pattern[j + 1] != '*')
                flag = 1;
        } else if (pattern[j + 1] == '*') {
            int i_before = i;
            if (pattern[j] == '.' && flag == 0) {
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
                int l = 0;
                for (l = j; l < pattern_len; l++) {
                    if (pattern[l] == '\\' && pattern[l + 1] == text[i]) {
                        l++;
                        break;
                    } else if (
                            text[i] != pattern[l]
                            && (pattern[l + 1] != '*' || pattern[l] == '\\')
                            && pattern[l] != '.') {
                        l = 0;
                        break;
                    } else if (text[i] != pattern[l] && pattern[l + 1] == '*') {
                        l++;
                        continue;
                    } else if (text[i] == pattern[l] || pattern[l] == '.') {
                        break;
                    }
                }
                l = l == pattern_len ? 0 : l;
                if (l != 0) {
                    k -= l - j;
                    j = l;
                    i--;
                } else if (l == 0 && j != 0) {
                    int skip = 0;
                    for (int jj = j; jj < pattern_len; jj++) {
                        if (pattern[jj + 1] != '*' || pattern[jj] == '\\') {
                            skip = 1;
                            break;
                        } else {
                            jj++;
                        }
                    }
                    if (skip) {
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
        } else if (pattern[j] == text[i] || (pattern[j] == '.' && flag == 0)) {
            j++;
            flag = 0;
        }
        if (j == pattern_len) {
            *len = j + k;
            return i - j - k + 1;
        }
    }

    return -1;
}

// r*e*t*u*r*n*\* OK
// r*e*t*u*r*n*\\\\ OK
// r*e*t*u*r*n*\. OK
// r*e*t*u*\.r*n* OK
// r*e*t*u*\*r*n* OK
// r*e*t*\ur*n* OK
// r*e*t*u*\\\\r*n* OK

// r*e*t*u*\r*n* КОСТЫЛЬ СДЕЛАТЬ

// r*e*t*u*r*n*\ ?
// *refks ?
// .*fd ?
// fd.* ?

// fd** ?
// fd**d

// int main(int argc, char* argv[])
// {
//     if (argc != 1)
//         printf("%s\n", argv[1]);
//     char text[] = "cou think it could ";
//     printf("%s\n", text);
//     char pattern1[] = ".*oul";
//     // char pattern2[] = "usa*o";
//     // char pattern3[] = "qu..k";
//     // char pattern4[] = "lol\\*i";
//     // char pattern5[] = "return\\\\";
//     // char pattern6[] = "return.";
//     // char pattern7[] = "her.*in";
//     int cnt1 = 0;
//     // int cnt2 = 0;
//     // int cnt3 = 0;
//     // int cnt4 = 0;
//     // int cnt5 = 0;
//     // int cnt6 = 0;
//     // int cnt7 = 0;
//     int index1 = seek_substring_KMP(text, pattern1, &cnt1);
//     // int index2 = seek_substring_KMP(text, pattern2, &cnt2);
//     // int index3 = seek_substring_KMP(text, pattern3, &cnt3);
//     // int index4 = seek_substring_KMP(text, pattern4, &cnt4);
//     // int index5 = seek_substring_KMP(text, pattern5, &cnt5);
//     // int index6 = seek_substring_KMP(text, pattern6, &cnt6);
//     // int index7 = seek_substring_KMP(text, pattern7, &cnt7);
//     printf("Index of '%s' %d len = %d\n", pattern1, index1, cnt1);
//     // printf("Index of '%s' %d len = %d\n", pattern2, index2, cnt2);
//     // printf("Index of '%s' %d len = %d\n", pattern3, index3, cnt3);
//     // printf("Index of '%s' %d len = %d\n", pattern4, index4, cnt4);
//     // printf("Index of '%s' %d len = %d\n", pattern5, index5, cnt5);
//     // printf("Index of '%s' %d len = %d\n", pattern6, index6, cnt6);
//     // printf("Index of '%s' %d len = %d\n", pattern7, index7, cnt7);

//     return 0;
// }
