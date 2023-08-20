#include "logging.h"
#include <stdarg.h>
#include <time.h>

FILE *log_file = NULL;

void open_log_file(const char *filename) {
    fopen_s(&log_file, filename, "a");
    if (log_file == NULL) {
        fprintf(stderr, "Failed to open log file.\n");
    }
}

void close_log_file() {
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
}

void log_printf(const char *format, ...) {
    if (log_file == NULL) {
        fprintf(stderr, "Log file not opened.\n");
        return;
    }

    // Get current time
    time_t rawtime;
    struct tm timeinfo;
    char timestamp[20];
    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);

    // Print timestamp and log message
    fprintf(log_file, "[%s] ", timestamp);

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fflush(log_file);
}