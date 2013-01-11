#include "MCOperation.h"

using namespace mailcore;

Operation::Operation()
{
	mCallback = NULL;
	mCancelled = false;
	pthread_mutex_init(&mLock, NULL);
}

Operation::~Operation()
{
	pthread_mutex_destroy(&mLock);
}

void Operation::setCallback(OperationCallback * callback)
{
	mCallback = callback;
}

OperationCallback * Operation::callback()
{
	return mCallback;
}

void Operation::cancel()
{
	pthread_mutex_lock(&mLock);
	mCancelled = true;
	pthread_mutex_unlock(&mLock);
}

bool Operation::isCancelled()
{
	pthread_mutex_lock(&mLock);
	bool value = mCancelled;
	pthread_mutex_unlock(&mLock);
	
	return value;
}

void Operation::main()
{
}
