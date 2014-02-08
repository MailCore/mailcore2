//
//  MCIMAPAppendMessageOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPAppendMessageOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncConnection.h"

using namespace mailcore;

IMAPAppendMessageOperation::IMAPAppendMessageOperation()
{
    mMessageData = NULL;
    mFlags = MessageFlagNone;
    mCustomFlags = NULL;
    mCreatedUID = 0;
}

IMAPAppendMessageOperation::~IMAPAppendMessageOperation()
{
    MC_SAFE_RELEASE(mMessageData);
    MC_SAFE_RELEASE(mCustomFlags);
}

void IMAPAppendMessageOperation::setMessageData(Data * messageData)
{
    MC_SAFE_REPLACE_RETAIN(Data, mMessageData, messageData);
}

Data * IMAPAppendMessageOperation::messageData()
{
    return mMessageData;
}

void IMAPAppendMessageOperation::setFlags(MessageFlag flags)
{
    mFlags = flags;
}

MessageFlag IMAPAppendMessageOperation::flags()
{
    return mFlags;
}

void IMAPAppendMessageOperation::setCustomFlags(Array * customFlags)
{
    MC_SAFE_REPLACE_COPY(Array, mCustomFlags, customFlags);
}

Array * IMAPAppendMessageOperation::customFlags()
{
    return customFlags();
}

uint32_t IMAPAppendMessageOperation::createdUID()
{
    return mCreatedUID;
}

void IMAPAppendMessageOperation::main()
{
    ErrorCode error;
    session()->session()->appendMessageWithCustomFlags(folder(), mMessageData, mFlags, mCustomFlags, this, &mCreatedUID, &error);
    setError(error);
}

