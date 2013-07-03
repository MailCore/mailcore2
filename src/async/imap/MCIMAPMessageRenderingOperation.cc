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
    mMessage = NULL;
    mRenderingType = RenderingTypePlainTextBody;
    mResult = NULL;
}

IMAPMessageRenderingOperation::~IMAPMessageRenderingOperation()
{
    MC_SAFE_RELEASE(mMessage);
    MC_SAFE_RELEASE(mResult);
}

void IMAPMessageRenderingOperation::setRenderingType(IMAPMessageRenderingType type)
{
    mRenderingType = type;
}

IMAPMessageRenderingType IMAPMessageRenderingOperation::renderingType()
{
    return mRenderingType;
}

void IMAPMessageRenderingOperation::setMessage(IMAPMessage * message)
{
    mMessage = MC_SAFE_REPLACE_COPY(IMAPMessage, mMessage, message);
}

IMAPMessage * IMAPMessageRenderingOperation::message()
{
    return mMessage;
}

String * IMAPMessageRenderingOperation::result()
{
    return mResult;
}

void IMAPMessageRenderingOperation::main()
{
    ErrorCode error;
    
    if (mRenderingType == RenderingTypeHTML) {
        mResult = session()->session()->htmlRendering(mMessage, folder());
    }
    else if (mRenderingType == RenderingTypeHTMLBody) {
        mResult = session()->session()->htmlBodyRendering(mMessage, folder());
    }
    else if (mRenderingType == RenderingTypePlainText) {
        mResult = session()->session()->plainTextRendering(mMessage, folder());
    }
    else if (mRenderingType == RenderingTypePlainTextBody) {
        mResult = session()->session()->plainTextBodyRendering(mMessage, folder());
    }
    
    setError(error);
}
