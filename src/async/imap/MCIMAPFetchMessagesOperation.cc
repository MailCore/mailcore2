//
//  IMAPFetchMessagesOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPFetchMessagesOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncConnection.h"

using namespace mailcore;

IMAPFetchMessagesOperation::IMAPFetchMessagesOperation()
{
    mFetchByUidEnabled = false;
    mFirst = 0;
    mLast = 0;
    mUids = NULL;
    mNumbers = NULL;
    mMessages = NULL;
}

IMAPFetchMessagesOperation::~IMAPFetchMessagesOperation()
{
    MC_SAFE_RELEASE(mNumbers);
    MC_SAFE_RELEASE(mUids);
    MC_SAFE_RELEASE(mMessages);
}

void IMAPFetchMessagesOperation::setFetchByUidEnabled(bool enabled)
{
    mFetchByUidEnabled = enabled;
}

bool IMAPFetchMessagesOperation::isFetchByUidEnabled()
{
    return mFetchByUidEnabled;
}

void IMAPFetchMessagesOperation::setFirst(uint32_t first)
{
    mFirst = first;
}

uint32_t IMAPFetchMessagesOperation::first()
{
    return mFirst;
}

void IMAPFetchMessagesOperation::setLast(uint32_t last)
{
    mLast = last;
}

uint32_t IMAPFetchMessagesOperation::last()
{
    return mLast;
}

void IMAPFetchMessagesOperation::setUids(Array * uids)
{
    MC_SAFE_REPLACE_RETAIN(Array, mUids, uids);
}

Array * IMAPFetchMessagesOperation::uids()
{
    return mUids;
}

void IMAPFetchMessagesOperation::setNumbers(Array * numbers)
{
    MC_SAFE_REPLACE_RETAIN(Array, mNumbers, numbers);
}

Array * IMAPFetchMessagesOperation::numbers()
{
    return mNumbers;
}

void IMAPFetchMessagesOperation::setKind(IMAPMessagesRequestKind kind)
{
    mKind = kind;
}

IMAPMessagesRequestKind IMAPFetchMessagesOperation::kind()
{
    return mKind;
}

Array * IMAPFetchMessagesOperation::messages()
{
    return mMessages;
}

void IMAPFetchMessagesOperation::main()
{
    ErrorCode error;
    if (mFetchByUidEnabled) {
        if (mUids != NULL) {
            mMessages = session()->session()->fetchMessagesByUID(folder(), mKind, mUids, this, &error);
        }
        else {
            mMessages = session()->session()->fetchMessagesByUID(folder(), mKind, mFirst, mLast, this, &error);
        }
    }
    else {
        if (mNumbers != NULL) {
            mMessages = session()->session()->fetchMessagesByNumber(folder(), mKind, mNumbers, this, &error);
        }
        else {
            mMessages = session()->session()->fetchMessagesByNumber(folder(), mKind, mFirst, mLast, this, &error);
        }
    }
    MC_SAFE_RETAIN(mMessages);
    setError(error);
}
