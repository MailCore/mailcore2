//
//  MCIMAPFetchFoldersOperation.cpp
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPFetchFoldersOperation.h"

#include <stdlib.h>

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPFetchFoldersOperation::IMAPFetchFoldersOperation()
{
    mFetchSubscribedEnabled = false;
    mFolders = NULL;
}

IMAPFetchFoldersOperation::~IMAPFetchFoldersOperation()
{
    MC_SAFE_RELEASE(mFolders);
}

void IMAPFetchFoldersOperation::setFetchSubscribedEnabled(bool enabled)
{
    mFetchSubscribedEnabled = enabled;
}

bool IMAPFetchFoldersOperation::isFetchSubscribedEnabled()
{
    return mFetchSubscribedEnabled;
}

Array * IMAPFetchFoldersOperation::folders()
{
    return mFolders;
}

void IMAPFetchFoldersOperation::main()
{
    ErrorCode error;
    if (mFetchSubscribedEnabled) {
        mFolders = session()->session()->fetchSubscribedFolders(&error);
    }
    else {
        mFolders = session()->session()->fetchAllFolders(&error);
    }
    MC_SAFE_RETAIN(mFolders);
    setError(error);
    
    char * delimiterData = (char *) malloc(1);
    * delimiterData = session()->session()->delimiter();
    session()->performMethodOnMainThread((Object::Method) &IMAPFetchFoldersOperation::setDelimiterDataOnMainThread,
                                         delimiterData);
    free(delimiterData);
}

void IMAPFetchFoldersOperation::setDelimiterDataOnMainThread(char * delimiterData)
{
    session()->setDelimiter(* delimiterData);
}
