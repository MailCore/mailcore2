//
//  MCNNTPFetchHeaderOperation.cpp
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#include "MCNNTPFetchHeaderOperation.h"

#include "MCNNTPAsyncSession.h"
#include "MCNNTPSession.h"
#include "MCMessageHeader.h"

using namespace mailcore;

NNTPFetchHeaderOperation::NNTPFetchHeaderOperation()
{
    mMessageIndex = 0;
    mHeader = NULL;
}

NNTPFetchHeaderOperation::~NNTPFetchHeaderOperation()
{
    MC_SAFE_RELEASE(mHeader);
}

void NNTPFetchHeaderOperation::setMessageIndex(unsigned int messageIndex)
{
    mMessageIndex = messageIndex;
}

unsigned int NNTPFetchHeaderOperation::messageIndex()
{
    return mMessageIndex;
}

MessageHeader * NNTPFetchHeaderOperation::header()
{
    return mHeader;
}

void NNTPFetchHeaderOperation::main()
{
    ErrorCode error;
    mHeader = session()->session()->fetchHeader(mMessageIndex, &error);
    if (mHeader != NULL) {
        mHeader->retain();
    }
    setError(error);
}
