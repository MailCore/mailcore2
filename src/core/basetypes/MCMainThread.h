#ifndef __MAILCORE_MCMAINTHREAD_H

#define __MAILCORE_MCMAINTHREAD_H

#ifdef __cplusplus

namespace mailcore {
	void callOnMainThread(void (*)(void *), void * context);
	void callOnMainThreadAndWait(void (*)(void *), void * context);
	void callAfterDelay(void (*)(void *), void * context, double time);
}

#endif

#endif
