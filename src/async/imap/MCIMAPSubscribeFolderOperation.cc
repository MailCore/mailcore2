//
//  MCIMAPSubscribeFolderOperation.cpp
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPSubscribeFolderOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPSubscribeFolderOperation::IMAPSubscribeFolderOperation()
{
    mFolder = NULL;
    mUnsubscribeEnabled = false;
}

IMAPSubscribeFolderOperation::~IMAPSubscribeFolderOperation()
{
    MC_SAFE_RELEASE(mFolder);
}

void IMAPSubscribeFolderOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPSubscribeFolderOperation::folder()
{
    return mFolder;
}

void IMAPSubscribeFolderOperation::setUnsubscribeEnabled(bool enabled)
{
    mUnsubscribeEnabled = enabled;
}

bool IMAPSubscribeFolderOperation::isUnsubscribeEnabled()
{
    return mUnsubscribeEnabled;
}

void IMAPSubscribeFolderOperation::main()
{
    ErrorCode error;
    if (mUnsubscribeEnabled) {
        session()->session()->unsubscribeFolder(mFolder, &error);
    }
    else {
        session()->session()->subscribeFolder(mFolder, &error);
    }
    setError(error);
}
