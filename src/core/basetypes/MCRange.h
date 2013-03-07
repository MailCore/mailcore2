#ifndef __MAILCORE_MCRANGE_H_

#define __MAILCORE_MCRANGE_H_

#ifdef __cplusplus

#include <inttypes.h>

namespace mailcore {
    
    struct Range {
        uint64_t location;
        uint64_t length;
    };
    
    Range RangeMake(uint64_t location, uint64_t length);
}

#endif

#endif
