#ifndef __MAILCORE_MCLOG_H_

#define __MAILCORE_MCLOG_H_

#include <stdio.h>

#define MCLog(...) MCLogInternal(NULL, __FILE__, __LINE__, 0, __VA_ARGS__)
    
extern int MCLogEnabled;
    
#ifndef __printflike
#define __printflike(a,b)
#endif

#ifdef __cplusplus
extern "C" {
#endif
    void MCLogInternal(const char * user,
                       const char * filename,
                       unsigned int line,
                       int dumpStack,
                       const char * format, ...) __printflike(5, 6);
#ifdef __cplusplus
}
#endif

#endif
