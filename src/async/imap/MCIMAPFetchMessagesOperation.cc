//
//  IMAPFetchMessagesOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPFetchMessagesOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPFetchMessagesOperation::IMAPFetchMessagesOperation()
{
    mFolder = NULL;
    mFetchByUidEnabled = false;
    mFirst = 0;
    mLast = 0;
    mUids = NULL;
    mNumbers = NULL;
    mMessages = NULL;
}

IMAPFetchMessagesOperation::~IMAPFetchMessagesOperation()
{
    MC_SAFE_RELEASE(mFolder);
    MC_SAFE_RELEASE(mNumbers);
    MC_SAFE_RELEASE(mUids);
    MC_SAFE_RELEASE(mMessages);
}

void IMAPFetchMessagesOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPFetchMessagesOperation::folder()
{
    return mFolder;
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
    MCLog("1");
    if (mFetchByUidEnabled) {
        MCLog("2");
        if (mUids != NULL) {
            MCLog("3");
            mMessages = session()->session()->fetchMessagesByUID(mFolder, mKind, mUids, this, &error);
        }
        else {
            MCLog("4");
            mMessages = session()->session()->fetchMessagesByUID(mFolder, mKind, mFirst, mLast, this, &error);
        }
    }
    else {
        MCLog("5");
        if (mNumbers != NULL) {
            MCLog("6");
            mMessages = session()->session()->fetchMessagesByNumber(mFolder, mKind, mNumbers, this, &error);
        }
        else {
            MCLog("7");
            mMessages = session()->session()->fetchMessagesByNumber(mFolder, mKind, mFirst, mLast, this, &error);
        }
    }
    setError(error);
}
