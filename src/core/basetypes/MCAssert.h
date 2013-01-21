#ifndef __MAILCORE_MCASSERT_H_

#define __MAILCORE_MCASSERT_H_

#ifdef __cplusplus

#define MCAssert(cond) mailcore::assertInteral(__FILE__, __LINE__, cond, #cond)

namespace mailcore {

    void assertInteral(const char * filename, unsigned int line, int cond, const char * condString);
    
}

#endif

#endif
