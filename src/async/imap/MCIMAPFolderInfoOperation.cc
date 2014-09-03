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
    mFirstUnseenUid = 0;
    mAllowsNewPermanentFlags = false;
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

uint32_t IMAPFolderInfoOperation::firstUnseenUid()
{
    return mFirstUnseenUid;
}

bool IMAPFolderInfoOperation::allowsNewPermanentFlags()
{
    return mAllowsNewPermanentFlags;
}

void IMAPFolderInfoOperation::main()
{
    ErrorCode error;
    
    session()->session()->loginIfNeeded(&error);
    if (error != ErrorNone) {
        setError(error);
        return;
    }
    
    session()->session()->select(folder(), &error);
    if (error != ErrorNone) {
        setError(error);
        return;
    }
    
    
    mUidNext = session()->session()->uidNext();
    mUidValidity = session()->session()->uidValidity();
    mModSequenceValue = session()->session()->modSequenceValue();
    mMessageCount = session()->session()->lastFolderMessageCount();
    mFirstUnseenUid = session()->session()->firstUnseenUid();
    mAllowsNewPermanentFlags = session()->session()->allowsNewPermanentFlags();
  
    setError(error);
}

