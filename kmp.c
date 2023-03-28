#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int *prefix_func(char pattern[], size_t size)
{
    int *pi = (int *)malloc(size * sizeof(int));
    pi[0] = 0;
    for (int i = 1, j = 0; i < size; i++)
    {
        while (j > 0 && pattern[j] != pattern[i] && pattern[j] != '.')
            j = pi[j - 1];
        if (pattern[j] == pattern[i] || pattern[j] == '.')
            j++;
        pi[i] = j;
    }
    return pi;
}

int seek_substring_KMP(char text[], char pattern[])
{
    int i, j;
    size_t text_len = strlen(text);
    size_t pattern_len = strlen(pattern);

    int *pi = prefix_func(pattern, pattern_len);

    for (i = 0, j = 0; i < text_len; i++)
    {
        while (j > 0 && (pattern[j] != text[i]) && pattern[j] != '.')
        {
            j = pi[j - 1];
        }
        if (pattern[j] == text[i] || pattern[j] == '.')
            j++;
        if (j == pattern_len)
        {
            free(pi);
            return i - j + 1;
        }
    }

    free(pi);
    return -1;
}

// TODO: uy part = (return.) WHY???????
// TODO: quic. = -1 ????
// int main()
// {
//     char text[] = "Advertisements want to persuade us to buy particular The quick brown fox jumps over the lazy dog You notice the name of the refreshing drink because you think it could be useful for you to satisfy your thirst.";
//     // char pattern1[] = ".o. ..... over";
//     // int index1 = seek_substring_KMP(text, pattern1);
//     // printf("Index of '%s' %d\n", pattern1, index1);
//     char pattern2[] = "retur.";
//     int index2 = seek_substring_KMP(text, pattern2);
//     printf("Index of '%s' %d\n", pattern2, index2);

//     char pattern3[] = "q...k";
//     char pattern4[] = "qu..k";
//     char pattern5[] = "quic.";
//     char pattern6[] = "...ick";
//     char pattern7[] = "notice";
//     int index3 = seek_substring_KMP(text, pattern3);
//     int index4 = seek_substring_KMP(text, pattern4);
//     int index5 = seek_substring_KMP(text, pattern5);
//     int index6 = seek_substring_KMP(text, pattern6);
//     int index7 = seek_substring_KMP(text, pattern7);
//     printf("Index of '%s' %d\n", pattern7, index7);
//     printf("Index of '%s' %d\n", pattern3, index3);
//     printf("Index of '%s' %d\n", pattern4, index4);
//     printf("Index of '%s' %d\n", pattern5, index5);
//     printf("Index of '%s' %d\n", pattern6, index6);

//     return 0;
// }
