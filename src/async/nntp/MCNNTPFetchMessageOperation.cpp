//
//  MCNNTPFetchMessagesOperation.cpp
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#include "MCNNTPFetchMessageOperation.h"

#include "MCNNTPAsyncSession.h"
#include "MCNNTPSession.h"

using namespace mailcore;

NNTPFetchMessageOperation::NNTPFetchMessageOperation()
{
    mMessageIndex = 0;
    mData = NULL;
}

NNTPFetchMessageOperation::~NNTPFetchMessageOperation()
{
    MC_SAFE_RELEASE(mData);
}

void NNTPFetchMessageOperation::setMessageIndex(unsigned int messageIndex)
{
    mMessageIndex = messageIndex;
}

unsigned int NNTPFetchMessageOperation::messageIndex()
{
    return mMessageIndex;
}

Data * NNTPFetchMessageOperation::data()
{
    return mData;
}

void NNTPFetchMessageOperation::main()
{
    ErrorCode error;
    mData = session()->session()->fetchMessage(mMessageIndex, this, &error);
    MC_SAFE_RETAIN(mData);
    setError(error);
}

