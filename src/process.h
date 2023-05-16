#pragma once

#include <stdio.h> // for size_t

#define NOT_RECURSIVE 0
#define RECURSIVE 1

void highlite_word(char* str, int col, size_t str_len);
char* get_path(char* path, char* name);
void process_file(char* path, char* pattern, char* name);
void process_dirs(char* pattern, char* path, int is_recursive);
