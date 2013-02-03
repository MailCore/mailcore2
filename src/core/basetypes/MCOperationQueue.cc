#include "MCOperationQueue.h"

#include "MCOperation.h"
#include "MCOperationCallback.h"
#include "MCMainThread.h"
#include "MCUtils.h"
#include "MCArray.h"
#include "MCLog.h"
#include "MCAutoreleasePool.h"
#include <libetpan/libetpan.h>
#include <thread>

using namespace mailcore;

OperationQueue::OperationQueue()
{
	mOperations = new Array();
	mStarted = false;
    mWaiting = false;
    mOperationSem = mailsem_new();
    mStartSem = mailsem_new();
    mStopSem = mailsem_new();
    mWaitingFinishedSem = mailsem_new();
}

OperationQueue::~OperationQueue()
{
    MC_SAFE_RELEASE(mOperations);
    mailsem_free(mOperationSem);
    mailsem_free(mStartSem);
    mailsem_free(mStopSem);
    mailsem_free(mWaitingFinishedSem);
}

void OperationQueue::addOperation(Operation * op)
{
	mLock.lock();
    mOperations->addObject(op);
	mLock.unlock();
    mailsem_up(mOperationSem);
    startThread();
}

void OperationQueue::runOperations()
{
    MCLog("start thread");
    mailsem_up(mStartSem);
    
    while (true) {
        Operation * op = NULL;
        bool needsCheckRunning = false;
        bool quitting;
        
        AutoreleasePool * pool = new AutoreleasePool();
        
        mailsem_down(mOperationSem);
        
		mLock.lock();
        if (mOperations->count() > 0) {
            op = (Operation *) mOperations->objectAtIndex(0);
        }
        quitting = mQuitting;
		mLock.unlock();

        MCLog("quitting %i %p", mQuitting, op);
        if ((op == NULL) && quitting) {
            MCLog("stopping %p", this);
            mailsem_up(mStopSem);
            pool->release();
            break;
        }

        op->main();
        
        op->retain()->autorelease();
        
		mLock.lock();
        mOperations->removeObjectAtIndex(0);
        if (mOperations->count() == 0) {
            if (mWaiting) {
                mailsem_up(mWaitingFinishedSem);
            }
            needsCheckRunning = true;
        }
		mLock.unlock();
        
        if (!op->isCancelled()) {
            if (op->callback() != NULL) {
                performMethodOnMainThread((Object::Method) &OperationQueue::callbackOnMainThread, op, true);
            }
        }
        
        if (needsCheckRunning) {
            retain(); // (1)
            MCLog("check running %p", this);
            performMethodOnMainThread((Object::Method) &OperationQueue::checkRunningOnMainThread, this);
        }
        
        pool->release();
    }
    MCLog("cleanup thread %p", this);
}

void OperationQueue::callbackOnMainThread(Operation * op)
{
    if (op->callback() != NULL) {
        op->callback()->operationFinished(op);
    }
}

void OperationQueue::checkRunningOnMainThread(void * context)
{
    performMethodAfterDelay((Object::Method) &OperationQueue::checkRunningAfterDelay, NULL, 1);
}

void OperationQueue::checkRunningAfterDelay(void * context)
{
    bool quitting = false;
    
	mLock.lock();
    if (!mQuitting) {
        if (mOperations->count() == 0) {
            MCLog("trying to quit %p", this);
            mailsem_up(mOperationSem);
            mQuitting = true;
            quitting = true;
        }
    }
	mLock.unlock();
    
    // Number of operations can't be changed because it runs on main thread.
    // And addOperation() should also be called from main thread.
    
    if (quitting) {
        mailsem_down(mStopSem);
        mStarted = false;
    }
    
    release(); // (1)
}

void OperationQueue::startThread()
{
    if (mStarted)
        return;
    
    mQuitting = false;
    mStarted = true;
	std::thread t([=]()
	{
		runOperations();
	});
	t.detach();
    mailsem_down(mStartSem);
}

unsigned int OperationQueue::count()
{
    unsigned int count;
    
	mLock.lock();
    count = mOperations->count();
	mLock.unlock();
    
    return count;
}

#if 0
void OperationQueue::waitUntilAllOperationsAreFinished()
{
    bool waiting = false;
    
	mLock.lock();
    if (mOperations->count() > 0) {
        mWaiting = true;
        waiting = true;
    }
	mLock.unlock();
    
    if (waiting) {
        sem_wait(&mWaitingFinishedSem);
    }
    mWaiting = false;
}
#endif
