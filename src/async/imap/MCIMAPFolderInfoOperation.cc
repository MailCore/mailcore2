//
//  MCIMAPFolderInfoOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/13/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPFolderInfoOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPFolderInfoOperation::IMAPFolderInfoOperation()
{
    mFolder = NULL;
    mUidNext = 0;
    mUidValidity = 0;
    mMessageCount = 0;
}

IMAPFolderInfoOperation::~IMAPFolderInfoOperation()
{
    MC_SAFE_RELEASE(mFolder);
}

void IMAPFolderInfoOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPFolderInfoOperation::folder()
{
    return mFolder;
}

uint32_t IMAPFolderInfoOperation::uidNext()
{
    return mUidNext;
}

uint32_t IMAPFolderInfoOperation::uidValidity()
{
    return mUidValidity;
}

int IMAPFolderInfoOperation::messageCount()
{
    return mMessageCount;
}

void IMAPFolderInfoOperation::main()
{
    ErrorCode error;
    session()->session()->select(mFolder, &error);
    setError(error);
}

