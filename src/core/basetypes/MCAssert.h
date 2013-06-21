#ifndef __MAILCORE_MCASSERT_H_

#define __MAILCORE_MCASSERT_H_

#include "MCUtils.h"

#define MCAssert(cond) assertInteral(__FILE__, __LINE__, cond, #cond)

MAILCORE_EXTERN void assertInteral(const char * filename, unsigned int line, int cond, const char * condString);

#endif
