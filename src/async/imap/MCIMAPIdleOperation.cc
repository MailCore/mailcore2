//
//  IMAPIdleOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPIdleOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPIdleOperation::IMAPIdleOperation()
{
    mFolder = NULL;
    mLastKnownUid = 0;
}

IMAPIdleOperation::~IMAPIdleOperation()
{
    MC_SAFE_RELEASE(mFolder);
}

void IMAPIdleOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPIdleOperation::folder()
{
    return mFolder;
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
    session()->session()->setupIdle();
}

void IMAPIdleOperation::unprepare()
{
    session()->session()->unsetupIdle();
}

void IMAPIdleOperation::main()
{
    performMethodOnMainThread((Object::Method) &IMAPIdleOperation::prepare, NULL);
    
    ErrorCode error;
    session()->session()->idle(mFolder, mLastKnownUid, &error);
    setError(error);
    
    performMethodOnMainThread((Object::Method) &IMAPIdleOperation::unprepare, NULL);
}

void IMAPIdleOperation::cancel()
{
    session()->session()->interruptIdle();
}

