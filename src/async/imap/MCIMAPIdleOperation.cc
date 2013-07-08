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
}

IMAPIdleOperation::~IMAPIdleOperation()
{
}

void IMAPIdleOperation::setLastKnownUID(uint32_t uid)
{
    mLastKnownUid = uid;
}

uint32_t IMAPIdleOperation::lastKnownUID()
{
    return mLastKnownUid;
}

void IMAPIdleOperation::prepare()
{
    mSetupSuccess = session()->session()->setupIdle();
}

void IMAPIdleOperation::unprepare()
{
    if (mSetupSuccess) {
        session()->session()->unsetupIdle();
    }
}

void IMAPIdleOperation::main()
{
    performMethodOnMainThread((Object::Method) &IMAPIdleOperation::prepare, NULL, true);
    
    if (!mSetupSuccess) {
        return;
    }
    
    ErrorCode error;
    session()->session()->idle(folder(), mLastKnownUid, &error);
    setError(error);
    
    performMethodOnMainThread((Object::Method) &IMAPIdleOperation::unprepare, NULL, true);
}

void IMAPIdleOperation::interruptIdle()
{
    if (mSetupSuccess) {
        session()->session()->interruptIdle();
    }
}

