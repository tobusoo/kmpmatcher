#pragma once

#ifndef TIME
#define TIME 0
#endif

#ifndef MESSAGE
#define MESSAGE 1
#endif

#ifndef MAX_LEN
#define MAX_LEN 1024
#endif

void log_write(int status, char* format, ...);
