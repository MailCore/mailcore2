//
//  IMAPIdleOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPIdleOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncConnection.h"

using namespace mailcore;

IMAPIdleOperation::IMAPIdleOperation()
{
    mLastKnownUid = 0;
    mSetupSuccess = false;
    mInterrupted = false;
    pthread_mutex_init(&mLock, NULL);
}

IMAPIdleOperation::~IMAPIdleOperation()
{
    pthread_mutex_destroy(&mLock);
}

void IMAPIdleOperation::setLastKnownUID(uint32_t uid)
{
    mLastKnownUid = uid;
}

uint32_t IMAPIdleOperation::lastKnownUID()
{
    return mLastKnownUid;
}

void IMAPIdleOperation::prepare(void * data)
{
    if (isInterrupted()) {
        setSetupSuccess(false);
        return;
    }

    setSetupSuccess(session()->session()->setupIdle());
}

void IMAPIdleOperation::unprepare(void * data)
{
    if (setupSuccess()) {
        session()->session()->unsetupIdle();
    }
}

bool IMAPIdleOperation::isInterrupted() {
    pthread_mutex_lock(&mLock);
    bool interrupted = mInterrupted;
    pthread_mutex_unlock(&mLock);
    
    return interrupted;
}

void IMAPIdleOperation::setSetupSuccess(bool setupSuccess)
{
    pthread_mutex_lock(&mLock);
    mSetupSuccess = setupSuccess;
    pthread_mutex_unlock(&mLock);
}

bool IMAPIdleOperation::setupSuccess()
{
    pthread_mutex_lock(&mLock);
    bool setupSuccess = mSetupSuccess;
    pthread_mutex_unlock(&mLock);

    return setupSuccess;
}

void IMAPIdleOperation::main()
{
    if (isInterrupted()) {
        return;
    }
    
    ErrorCode error;
    session()->session()->selectIfNeeded(folder(), &error);
    if (error != ErrorNone) {
        setError(error);
        return;
    }
    
    performMethodOnCallbackThread((Object::Method) &IMAPIdleOperation::prepare, NULL, true);
    
    if (!setupSuccess()) {
        return;
    }
    
    session()->session()->idle(folder(), mLastKnownUid, &error);
    setError(error);
    
    performMethodOnCallbackThread((Object::Method) &IMAPIdleOperation::unprepare, NULL, true);
}

void IMAPIdleOperation::cancel()
{
    IMAPOperation::cancel();
    interruptIdle();
}

void IMAPIdleOperation::interruptIdle()
{
    bool setupSuccess;

    pthread_mutex_lock(&mLock);
    mInterrupted = true;
    setupSuccess = mSetupSuccess;
    pthread_mutex_unlock(&mLock);
    if (setupSuccess && session() != NULL) {
        session()->session()->interruptIdle();
    }
}
