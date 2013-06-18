#ifndef __MAILCORE_MCRANGE_H_

#define __MAILCORE_MCRANGE_H_

#ifdef __cplusplus

#include <inttypes.h>

#ifndef UINT64_MAX
# define UINT64_MAX 18446744073709551615ULL
#endif

namespace mailcore {
    
    class IndexSet;
    
    // infinite length : UINT64_MAX
    // empty range : location = UINT64_MAX
    struct Range {
        uint64_t location;
        uint64_t length;
    };
    
    extern Range RangeEmpty;
    
    Range RangeMake(uint64_t location, uint64_t length);
    IndexSet * RangeRemoveRange(Range range1, Range range2);
    IndexSet * RangeUnion(Range range1, Range range2);
    Range RangeIntersection(Range range1, Range range2);
    bool RangeHasIntersection(Range range1, Range range2);
    uint64_t RangeLeftBound(Range range);
    uint64_t RangeRightBound(Range range);
}

#endif

#endif
