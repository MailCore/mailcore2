//
//  IMAPSearchOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPSearchOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"
#include "MCIMAPSearchExpression.h"

using namespace mailcore;

IMAPSearchOperation::IMAPSearchOperation()
{
    mFolder = NULL;
    mKind = IMAPSearchKindNone;
    mSearchString = NULL;
    mExpression = NULL;
}

IMAPSearchOperation::~IMAPSearchOperation()
{
    MC_SAFE_RELEASE(mFolder);
    MC_SAFE_RELEASE(mSearchString);
    MC_SAFE_RELEASE(mExpression);
}

void IMAPSearchOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPSearchOperation::folder()
{
    return mFolder;
}

void IMAPSearchOperation::setSearchKind(IMAPSearchKind kind)
{
    mKind = kind;
}

IMAPSearchKind IMAPSearchOperation::searchKind()
{
    return mKind;
}

void IMAPSearchOperation::setSearchString(String * searchString)
{
    MC_SAFE_REPLACE_COPY(String, mSearchString, searchString);
}

String * IMAPSearchOperation::searchString()
{
    return mSearchString;
}

void IMAPSearchOperation::setSearchExpression(IMAPSearchExpression * expression)
{
    MC_SAFE_REPLACE_RETAIN(IMAPSearchExpression, mExpression, expression);
}

IMAPSearchExpression * IMAPSearchOperation::searchExpression()
{
    return mExpression;
}

void IMAPSearchOperation::main()
{
    ErrorCode error;
    if (mExpression != NULL) {
        session()->session()->search(mFolder, mExpression, &error);
    }
    else {
        session()->session()->search(mFolder, mKind, mSearchString, &error);
    }
    setError(error);
}

