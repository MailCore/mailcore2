//
//  MCIMAPAppendMessageOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPAppendMessageOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPAppendMessageOperation::IMAPAppendMessageOperation()
{
    mFolder = NULL;
    mMessageData = NULL;
    mFlags = MessageFlagNone;
    mCreatedUID = 0;
}

IMAPAppendMessageOperation::~IMAPAppendMessageOperation()
{
    MC_SAFE_RELEASE(mFolder);
    MC_SAFE_RELEASE(mMessageData);
}

void IMAPAppendMessageOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPAppendMessageOperation::folder()
{
    return mFolder;
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

uint32_t IMAPAppendMessageOperation::createdUID()
{
    return mCreatedUID;
}

void IMAPAppendMessageOperation::main()
{
    ErrorCode error;
    session()->session()->appendMessage(mFolder, mMessageData, mFlags, this, &mCreatedUID, &error);
    setError(error);
}

