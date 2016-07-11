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
#include "MCHTMLRendererCallback.h"

using namespace mailcore;

IMAPMessageRenderingOperation::IMAPMessageRenderingOperation()
{
    mMessage = NULL;
    mRenderingType = IMAPMessageRenderingTypePlainTextBody;
	mHtmlCallBack = NULL;
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

void IMAPMessageRenderingOperation::setHtmlCallBack(HTMLRendererTemplateCallback * htmlCallBack)
{
	mHtmlCallBack = htmlCallBack;
}

HTMLRendererTemplateCallback * IMAPMessageRenderingOperation::htmlCallBack()
{
	return mHtmlCallBack;
}

void IMAPMessageRenderingOperation::setMessage(IMAPMessage * message)
{
    MC_SAFE_REPLACE_COPY(IMAPMessage, mMessage, message);
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
    ErrorCode error = ErrorNone;
    
	if (mHtmlCallBack != NULL) {
		mResult = session()->session()->htmlBodyRendering(mMessage, folder(), mHtmlCallBack, &error);
	}
	else {
		if (mRenderingType == IMAPMessageRenderingTypeHTML) {
			mResult = session()->session()->htmlRendering(mMessage, folder(), &error);
		}
		else if (mRenderingType == IMAPMessageRenderingTypeHTMLBody) {
			mResult = session()->session()->htmlBodyRendering(mMessage, folder(), &error);
		}
		else if (mRenderingType == IMAPMessageRenderingTypePlainText) {
			mResult = session()->session()->plainTextRendering(mMessage, folder(), &error);
		}
		else if (mRenderingType == IMAPMessageRenderingTypePlainTextBody) {
			mResult = session()->session()->plainTextBodyRendering(mMessage, folder(), false, &error);
		}
		else if (mRenderingType == IMAPMessageRenderingTypePlainTextBodyAndStripWhitespace) {
			mResult = session()->session()->plainTextBodyRendering(mMessage, folder(), true, &error);
		}
	}
	
    MC_SAFE_RETAIN(mResult);
    setError(error);
}
