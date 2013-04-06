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
#include "MCIMAPSyncResult.h"

using namespace mailcore;

IMAPFetchMessagesOperation::IMAPFetchMessagesOperation()
{
    mFetchByUidEnabled = false;
    mIndexes = NULL;
    mMessages = NULL;
    mVanishedMessages = NULL;
    mModSequenceValue = 0;
}

IMAPFetchMessagesOperation::~IMAPFetchMessagesOperation()
{
    MC_SAFE_RELEASE(mIndexes);
    MC_SAFE_RELEASE(mMessages);
    MC_SAFE_RELEASE(mVanishedMessages);
}

void IMAPFetchMessagesOperation::setFetchByUidEnabled(bool enabled)
{
    mFetchByUidEnabled = enabled;
}

bool IMAPFetchMessagesOperation::isFetchByUidEnabled()
{
    return mFetchByUidEnabled;
}

void IMAPFetchMessagesOperation::setIndexes(IndexSet * indexes)
{
    MC_SAFE_REPLACE_RETAIN(IndexSet, mIndexes, indexes);
}

IndexSet * IMAPFetchMessagesOperation::indexes()
{
    return mIndexes;
}

void IMAPFetchMessagesOperation::setModSequenceValue(uint64_t modseq)
{
    mModSequenceValue = modseq;
}

uint64_t IMAPFetchMessagesOperation::modSequenceValue()
{
    return mModSequenceValue;
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

IndexSet * IMAPFetchMessagesOperation::vanishedMessages()
{
    return mVanishedMessages;
}

void IMAPFetchMessagesOperation::main()
{
    ErrorCode error;
    if (mFetchByUidEnabled) {
        if (mModSequenceValue != 0) {
            IMAPSyncResult * syncResult;
            
            syncResult = session()->session()->syncMessagesByUID(folder(), mKind, mIndexes, mModSequenceValue, this, &error);
            if (syncResult != NULL) {
                mMessages = syncResult->modifiedOrAddedMessages();
                mVanishedMessages = syncResult->vanishedMessages();
            }
        }
        else {
            mMessages = session()->session()->fetchMessagesByUID(folder(), mKind, mIndexes, this, &error);
        }
    }
    else {
        mMessages = session()->session()->fetchMessagesByNumber(folder(), mKind, mIndexes, this, &error);
    }
    MC_SAFE_RETAIN(mMessages);
    MC_SAFE_RETAIN(mVanishedMessages);
    setError(error);
}
