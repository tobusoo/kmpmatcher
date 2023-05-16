#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "log.h"

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
        char buffer[MAX_LEN];

        vsnprintf(buffer, MAX_LEN, format, arg_ptr);
        fprintf(out, "%s", buffer);

        va_end(arg_ptr);
    }

    fclose(out);
}
