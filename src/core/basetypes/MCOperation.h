#ifndef __MAILCORE_MCOPERATION_H_

#define __MAILCORE_MCOPERATION_H_

#include <pthread.h>
#include <MailCore/MCObject.h>

#ifdef __cplusplus

namespace mailcore {

	class OperationCallback;

	class Operation : public Object {
	public:
		Operation();
		virtual ~Operation();
		
		virtual void setCallback(OperationCallback * callback);
		virtual OperationCallback * callback();
        
		virtual void cancel();
		virtual bool isCancelled();
		
        // Will be called on main thread.
        virtual void beforeMain();
        
		virtual void main();
        
        // Will be called on main thread.
        virtual void afterMain();

        virtual void start();
        
	private:
		OperationCallback * mCallback;
		bool mCancelled;
		pthread_mutex_t mLock;
		
	};
	
}

#endif

#endif
