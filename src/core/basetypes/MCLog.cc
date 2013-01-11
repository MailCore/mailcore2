#include "MCLog.h"

#include <stdarg.h>
#include <stdio.h>

static void logInternalv(FILE * file,
    const char * user, const char * filename, unsigned int line,
    int dumpStack, const char * format, va_list argp);

void mailcore::logInternal(const char * user,
	const char * filename, 
	unsigned int line, 
	int dumpStack, 
	const char * format, ...)
{
	va_list argp;
	
	va_start(argp, format);
    logInternalv(stderr, user, filename, line, dumpStack, format, argp);
	va_end(argp);
}

static void logInternalv(FILE * file,
    const char * user, const char * filename, unsigned int line,
    int dumpStack, const char * format, va_list argp)
{
    fprintf(file, "%s:%i: ", filename, line);
    vfprintf(file, format, argp);
    fprintf(file, "\n");
}
