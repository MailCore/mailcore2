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
    mRenderingType = RenderingTypePlainTextBody;
    mResult = NULL;
    mUid = 0;
}

IMAPMessageRenderingOperation::~IMAPMessageRenderingOperation()
{
    MC_SAFE_RELEASE(mResult);
}

void IMAPMessageRenderingOperation::setRenderingType(RenderingType type)
{
    mRenderingType = type;
}

RenderingType IMAPMessageRenderingOperation::renderingType()
{
    return mRenderingType;
}

void IMAPMessageRenderingOperation::setUid(uint32_t uid)
{
    mUid = uid;
}

uint32_t IMAPMessageRenderingOperation::uid()
{
    return mUid;
}

String * result()
{
    return mResult;
}

void IMAPMessageRenderingOperation::main()
{
    ErrorCode error;
    
    if (mRenderingType == RenderingTypeHTML) {
        mResult = session()->session()->htmlRendering(msg, folder());
    }
    else if (mRenderingType == RenderingTypeHTMLBody) {
        mResult = session()->session()->htmlBodyRendering(msg, folder());
    }
    else if (mRenderingType == RenderingTypePlainText) {
        mResult = session()->session()->plainTextRendering(msg, folder());
    }
    else if (mRenderingType == RenderingTypePlainTextBody) {
        mResult = session()->session()->plainTextBodyRendering(msg, folder());
    }
    
    setError(error);
}
