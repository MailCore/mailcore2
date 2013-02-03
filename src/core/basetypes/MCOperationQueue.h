#ifndef __MAILCORE_MCOPERATIONQUEUE_H_

#define __MAILCORE_MCOPERATIONQUEUE_H_

#include <pthread.h>
#include <semaphore.h>
#include <mailcore/MCObject.h>
#include <libetpan/libetpan.h>
#include <mutex>

#ifdef __cplusplus

namespace mailcore {
	
	class Operation;
	class Array;
    
	class OperationQueue : public Object {
	public:
		OperationQueue();
		virtual ~OperationQueue();
		
		virtual void addOperation(Operation * op);
		
        virtual unsigned int count();
        
	private:
		Array * mOperations;
		pthread_t mThreadID;
		bool mStarted;
		struct mailsem * mOperationSem;
		struct mailsem * mStartSem;
		struct mailsem * mStopSem;
		std::mutex mLock;
		bool mWaiting;
		struct mailsem * mWaitingFinishedSem;
		bool mQuitting;
        
		void startThread();
		void runOperations();
		void callbackOnMainThread(Operation * op);
		void checkRunningOnMainThread(void * context);
		void checkRunningAfterDelay(void * context);
		
	};
	
}

#endif

#endif
