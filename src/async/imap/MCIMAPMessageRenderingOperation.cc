//
//  MCIMAPMessageRenderingOperation.cc
//  mailcore2
//
//  Created by Paul Young on 27/06/2013.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPMessageRenderingOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncConnection.h"

using namespace mailcore;

IMAPMessageRenderingOperation::IMAPMessageRenderingOperation()
{
    mUid = 0;
    mData = NULL;
}

IMAPMessageRenderingOperation::~IMAPMessageRenderingOperation()
{
    MC_SAFE_RELEASE(mData);
}

void IMAPMessageRenderingOperation::setUid(uint32_t uid)
{
    mUid = uid;
}

uint32_t IMAPMessageRenderingOperation::uid()
{
    return mUid;
}

void htmlRendering()
{
    
}

void htmlBodyRendering()
{
    
}

void plainTextRendering()
{
    
}

void plainTextBodyRendering()
{
    
}

void IMAPMessageRenderingOperation::main()
{
    ErrorCode error;
    mData = session()->session()->fetchMessageByUID(folder(), mUid, this, &error);
    MC_SAFE_RETAIN(mData);
    setError(error);
}
