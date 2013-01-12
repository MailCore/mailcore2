#include "MCLog.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

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
    while (1) {
        const char * p = filename;
        
        p = strchr(filename, '/');
        if (p == NULL) {
            break;
        }
        filename = p + 1;
    }
    
	struct timeval tv;
	struct tm tm_value;
    pthread_t thread_id = pthread_self();
    
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &tm_value);
	fprintf(file, "%04u-%02u-%02u %02u:%02u:%02u.%03u ", tm_value.tm_year + 1900, tm_value.tm_mon + 1, tm_value.tm_mday, tm_value.tm_hour, tm_value.tm_min, tm_value.tm_sec, tv.tv_usec / 1000);
    
    if (pthread_main_np()) {
        fprintf(file, "[%i:main] %s:%i: ", getpid(), filename, line);
    }
    else {
#ifdef _MACH_PORT_T
        fprintf(file, "[%i:%x] %s:%i: ", getpid(), pthread_mach_thread_np(thread_id), filename, line);
#else
        fprintf(file, "[%i:%p] %s:%i: ", getpid(), (void *) thread_id, filename, line);
#endif
    }
    vfprintf(file, format, argp);
    fprintf(file, "\n");
}
