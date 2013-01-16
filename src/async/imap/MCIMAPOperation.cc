//
//  MCIMAPOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPOperation.h"

#include <stdlib.h>

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"
#include "MCIMAPOperationCallback.h"

using namespace mailcore;

IMAPOperation::IMAPOperation()
{
    mSession = NULL;
    mImapCallback = NULL;
    mError = ErrorNone;
}

IMAPOperation::~IMAPOperation()
{
    MC_SAFE_RELEASE(mSession);
}

void IMAPOperation::setSession(IMAPAsyncSession * session)
{
    MC_SAFE_REPLACE_RETAIN(IMAPAsyncSession, mSession, session);
}

IMAPAsyncSession * IMAPOperation::session()
{
    return mSession;
}

void IMAPOperation::setImapCallback(IMAPOperationCallback * callback)
{
    mImapCallback = callback;
}

IMAPOperationCallback * IMAPOperation::imapCallback()
{
    return mImapCallback;
}

void IMAPOperation::setError(ErrorCode error)
{
    mError = error;
}

ErrorCode IMAPOperation::error()
{
    return mError;
}

void IMAPOperation::start()
{
    mSession->runOperation(this);
}

struct progressContext {
    unsigned int current;
    unsigned int maximum;
};

void IMAPOperation::bodyProgress(IMAPSession * session, unsigned int current, unsigned int maximum)
{
    struct progressContext * context = (struct progressContext *) calloc(sizeof(* context), 1);
    context->current = current;
    context->maximum = maximum;
    performMethodOnMainThread((Object::Method) &IMAPOperation::bodyProgressOnMainThread, context);
}

void IMAPOperation::bodyProgressOnMainThread(void * ctx)
{
    struct progressContext * context = (struct progressContext *) ctx;
    if (mImapCallback != NULL) {
        mImapCallback->bodyProgress(this, context->current, context->maximum);
    }
    free(context);
}

void IMAPOperation::itemsProgress(IMAPSession * session, unsigned int current, unsigned int maximum)
{
    struct progressContext * context = (struct progressContext *) calloc(sizeof(* context), 1);
    context->current = current;
    context->maximum = maximum;
    performMethodOnMainThread((Object::Method) &IMAPOperation::itemsProgressOnMainThread, context);
}

void IMAPOperation::itemsProgressOnMainThread(void * ctx)
{
    struct progressContext * context = (struct progressContext *) ctx;
    if (mImapCallback != NULL) {
        mImapCallback->itemProgress(this, context->current, context->maximum);
    }
    free(context);
}
