#ifndef __MAILCORE_MCRANGE_H_

#define __MAILCORE_MCRANGE_H_

#ifdef __cplusplus

namespace mailcore {
    
    struct Range {
        unsigned int index;
        unsigned int length;
    };
    
    Range RangeMake(unsigned int index, unsigned int length);
}

#endif

#endif
