//
//  MCNNTPFetchMessagesOperation.cpp
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#include "MCNNTPFetchMessagesOperation.h"

#include "MCNNTPAsyncSession.h"
#include "MCNNTPSession.h"

using namespace mailcore;

NNTPFetchMessagesOperation::NNTPFetchMessagesOperation()
{
    mGroupName = NULL;
    mMessages = NULL;
}

NNTPFetchMessagesOperation::~NNTPFetchMessagesOperation()
{
    MC_SAFE_RELEASE(mGroupName);
    MC_SAFE_RELEASE(mMessages);
}

void NNTPFetchMessagesOperation::setGroupName(String * groupname)
{
    MC_SAFE_REPLACE_COPY(String, mGroupName, groupname);
}

String * NNTPFetchMessagesOperation::groupName()
{
    return mGroupName;
}

Array * NNTPFetchMessagesOperation::messages()
{
    return mMessages;
}

void NNTPFetchMessagesOperation::main()
{
    ErrorCode error;
    mMessages = session()->session()->fetchMessages(mGroupName, &error);
    setError(error);
    MC_SAFE_RETAIN(mMessages);
}
