#ifndef __MAILCORE_MCASSERT_H_

#define __MAILCORE_MCASSERT_H_

#define MCAssert(cond) MCAssertInternal(__FILE__, __LINE__, cond, #cond)

#ifdef __cplusplus
extern "C" {
#endif	
	void MCAssertInternal(const char * filename, unsigned int line, int cond, const char * condString);
#ifdef __cplusplus
}
#endif

#endif
