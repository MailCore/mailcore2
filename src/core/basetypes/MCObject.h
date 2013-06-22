#ifndef __MAILCORE_MCOBJECT_H_

#define __MAILCORE_MCOBJECT_H_

#include <pthread.h>

#ifdef __cplusplus

#define MC_PROPERTY(mcType, setter, getter) \
    virtual void setter(mcType * getter); \
    virtual mcType * getter();

namespace mailcore {

    extern bool zombieEnabled;
    
	class String;

	class Object {
	public:
		Object();
		virtual ~Object();

		virtual int retainCount();
		virtual Object * retain();
		virtual void release();
		virtual Object * autorelease();
		virtual String * description();
		virtual String * className();
        
		virtual bool isEqual(Object * otherObject);
		virtual unsigned int hash();
		
		// optional
		virtual Object * copy();
		
		typedef void (Object::*Method) (void *);
		virtual void performMethod(Method method, void * context);
		virtual void performMethodOnMainThread(Method method, void * context, bool waitUntilDone = false);
		virtual void performMethodAfterDelay(Method method, void * context, double delay);
		virtual void cancelDelayedPerformMethod(Method method, void * context);
        
	private:
		pthread_mutex_t mLock;
		int mCounter;
		void init();
	};

}

#endif

#endif
