//
//  MCIMAPFolderInfoOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/13/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPFolderInfoOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncConnection.h"

using namespace mailcore;

IMAPFolderInfoOperation::IMAPFolderInfoOperation()
{
    mUidNext = 0;
    mUidValidity = 0;
    mMessageCount = 0;
    mModSequenceValue = 0;
    mUnseenCount = 0;
}

IMAPFolderInfoOperation::~IMAPFolderInfoOperation()
{
}

uint32_t IMAPFolderInfoOperation::uidNext()
{
    return mUidNext;
}

uint32_t IMAPFolderInfoOperation::uidValidity()
{
    return mUidValidity;
}

uint64_t IMAPFolderInfoOperation::modSequenceValue()
{
    return mModSequenceValue;
}

int IMAPFolderInfoOperation::messageCount()
{
    return mMessageCount;
}

int IMAPFolderInfoOperation::unseenCount()
{
    return mUnseenCount;
}

void IMAPFolderInfoOperation::main()
{
    ErrorCode error;
    
    session()->session()->loginIfNeeded(&error);
    if (error != ErrorCode::ErrorNone) {
        setError(error);
        return;
    }
    
    session()->session()->select(folder(), &error);
    if (error != ErrorCode::ErrorNone) {
        setError(error);
        return;
    }
    
    
    mUidNext = session()->session()->uidNext();
    mUidValidity = session()->session()->uidValidity();
    mModSequenceValue = session()->session()->modSequenceValue();
    mMessageCount = session()->session()->lastFolderMessageCount();
    mUnseenCount = session()->session()->lastFolderUnseenCount();

    setError(error);
}

