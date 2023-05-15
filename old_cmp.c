#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* prefix_func(char pattern[], size_t size)
{
    int* pi = (int*)malloc(size * sizeof(int));
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

int seek_substring_KMP(char text[], char pattern[], int* len)
{
    int i, j, k = 0;
    size_t text_len = strlen(text);
    size_t pattern_len = strlen(pattern);

    int* pi = prefix_func(pattern, pattern_len);

    int flag = 0;
    for (i = 0, j = 0; i < text_len; i++) {
        while (j > 0 && pattern[j] != text[i] && pattern[j] != '.'
               && pattern[j + 1] != '*' && pattern[j] != '\\') {
            j = pi[j - 1];
            k = 0;
            flag = 0;
        }
        // printf("    %c     %c\n", text[i], pattern[j]);
        // printf("i = %d j = %d k = %d\n", i, j, k);
        if (pattern[j] == '\\' && pattern[j - 1] != '\\') {
            flag = 1;
            j++;
            k--;
            i--;
        } else if (pattern[j + 1] == '*') {
            int i_before = i;
            if (pattern[j] == '.' && flag == 0) {
                while (text[i] != pattern[j + 2]) {
                    i++;
                    k++;
                }
            } else {
                while (text[i] == pattern[j]) {
                    i++;
                    k++;
                }
            }
            if (i_before == i) {
                int l = 0;
                int o = 0;
                for (l = j; l < pattern_len - 1; l++) {
                    // printf("i = %c l = %c l + 1 = %c\n",
                    //        text[i],
                    //        pattern[l],
                    //        pattern[l + 1]);
                    if (text[i] != pattern[l] && pattern[l + 1] == '*') {
                        o++;
                        l++;
                        continue;
                    } else if (text[i] == pattern[l]) {
                        // printf("break here text[i] == pattern[l]\n");
                        break;
                    } else if (text[i] != pattern[l] && pattern[l + 1] != '*') {
                        l = 0;
                        // printf("break here text[i] != pattern[l] &&
                        // pattern[l"
                        //        "+ 1] != '*'\n");
                        break;
                    }
                }
                l = l == pattern_len - 1 ? 0 : l;
                if (j == 0 && l == 0) {
                    j -= 2;
                    k += 2;
                } else if (j == 0 && l != 0) {
                    j = l - 1;
                    k = -o * 2 + 2;
                    // printf("o = %d\n", o);
                    // printf("l+1 = %c\n", pattern[l + 1]);
                }
                i++;
            }
            j += 2;
            i--;
            k -= 2;
            // printf("after\n");
            // printf("    %c     %c\n", text[i], pattern[j]);
            // printf("i = %d j = %d k = %d\n", i, j, k);
        }
        if (pattern[j] == text[i] || (pattern[j] == '.' && flag == 0))
            j++;
        // printf("j = %d pattern_len = %ld\n", j, pattern_len);
        if (j == pattern_len) {
            *len = j + k;
            return i - j - k + 1;
        }
    }

    return -1;
}

// int main(int argc, char* argv[])
// {
//     if (argc != 1)
//         printf("%s\n", argv[1]);
//     char text[] = " eturn";
//     printf("%s\n", text);
//     char pattern1[] = "r*e*turn";
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
