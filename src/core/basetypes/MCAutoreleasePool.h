#ifndef __MAILCORE_MCAUTORELEASEPOOL_H_

#define __MAILCORE_MCAUTORELEASEPOOL_H_

#include <mailcore/MCObject.h>
#include <libetpan/libetpan.h>
#include <pthread.h>

#ifdef __cplusplus

namespace mailcore {

	class AutoreleasePool : public Object {
	public:
		AutoreleasePool();
		virtual ~AutoreleasePool();
		
		static void autorelease(Object * obj);
        
    public: // subclass behavior
		virtual String * description();
        
	private:
		static void init();
		static pthread_key_t autoreleasePoolStackKey;
		carray * mPoolObjects;
		static carray * createAutoreleasePoolStackIfNeeded();
		static void destroyAutoreleasePoolStack(void *);
		static void initAutoreleasePoolStackKey();
		static AutoreleasePool * currentAutoreleasePool();
		virtual void add(Object * obj);
	};

}

#endif

#endif
