#ifndef LOGGING_H
#define LOGGING_H

#include "config.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

extern FILE *log_file;

void open_log_file(const char *filename);
void close_log_file();
void log_printf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* LOGGING_H */