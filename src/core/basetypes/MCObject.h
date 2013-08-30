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
    class HashMap;

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
        virtual HashMap * serializable();
        virtual void importSerializable(HashMap * serializable);
        
        typedef void (Object::*Method) (void *);
        virtual void performMethod(Method method, void * context);
        virtual void performMethodOnMainThread(Method method, void * context, bool waitUntilDone = false);
        virtual void performMethodAfterDelay(Method method, void * context, double delay);
        virtual void cancelDelayedPerformMethod(Method method, void * context);
        
        // serialization utils
        static void registerObjectConstructor(const char * className, void * (* objectConstructor)(void));
        static Object * objectWithSerializable(HashMap * serializable);
        
    public: // private
        
    private:
        pthread_mutex_t mLock;
        int mCounter;
        void init();
        static void initObjectConstructors();
    };

}

#endif

#endif
