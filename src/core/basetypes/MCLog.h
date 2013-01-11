#ifndef __MAILCORE_MCLOG_H_

#define __MAILCORE_MCLOG_H_

#include <stdio.h>

#define MCLog(...) mailcore::logInternal(NULL, __FILE__, __LINE__, 0, __VA_ARGS__)

namespace mailcore {
    
    void logInternal(const char * user,
                     const char * filename,
                     unsigned int line,
                     int dumpStack,
                     const char * format, ...) __printflike(5, 6);
    
}

#endif
