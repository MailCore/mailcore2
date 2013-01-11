#ifndef __MAILCORE_MCASSERT_H_

#define __MAILCORE_MCASSERT_H_

#define MCAssert(cond) mailcore::assertInteral(__FILE__, __LINE__, cond, #cond)

namespace mailcore {

    void assertInteral(const char * filename, unsigned int line, int cond, const char * condString);
    
}

#endif
