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
    mFirst = 0;
    mLast = 0;
    mUids = NULL;
    mNumbers = NULL;
    mMessages = NULL;
    mVanishedMessages = NULL;
    mModSequenceValue = 0;
}

IMAPFetchMessagesOperation::~IMAPFetchMessagesOperation()
{
    MC_SAFE_RELEASE(mNumbers);
    MC_SAFE_RELEASE(mUids);
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

Array * /* Value */ IMAPFetchMessagesOperation::vanishedMessages()
{
    return mVanishedMessages;
}

void IMAPFetchMessagesOperation::main()
{
    ErrorCode error;
    if (mFetchByUidEnabled) {
        if (mModSequenceValue != 0) {
            if (mUids != NULL) {
                IMAPSyncResult * syncResult;
                
                syncResult = session()->session()->syncMessagesByUIDForModSeq(folder(), mKind, mUids, mModSequenceValue, this, &error);
                if (syncResult != NULL) {
                    mMessages = syncResult->modifiedOrAddedMessages();
                    mVanishedMessages = syncResult->modifiedOrAddedMessages();
                }
            }
            else {
                IMAPSyncResult * syncResult;
                
                syncResult = session()->session()->syncMessagesByUIDForModSeq(folder(), mKind, mFirst, mLast, mModSequenceValue, this, &error);
                if (syncResult != NULL) {
                    mMessages = syncResult->modifiedOrAddedMessages();
                    mVanishedMessages = syncResult->modifiedOrAddedMessages();
                }
            }
        }
        else {
            if (mUids != NULL) {
                mMessages = session()->session()->fetchMessagesByUID(folder(), mKind, mUids, this, &error);
            }
            else {
                mMessages = session()->session()->fetchMessagesByUID(folder(), mKind, mFirst, mLast, this, &error);
            }
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
    MC_SAFE_RETAIN(mVanishedMessages);
    setError(error);
}
