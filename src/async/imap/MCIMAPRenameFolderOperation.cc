//
//  MCIMAPRenameFolderOperation.cpp
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPRenameFolderOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPRenameFolderOperation::IMAPRenameFolderOperation()
{
    mFolder = NULL;
    mOtherName = NULL;
}

IMAPRenameFolderOperation::~IMAPRenameFolderOperation()
{
    MC_SAFE_RELEASE(mFolder);
    MC_SAFE_RELEASE(mOtherName);
}

void IMAPRenameFolderOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPRenameFolderOperation::folder()
{
    return mFolder;
}

void IMAPRenameFolderOperation::setOtherName(String * otherName)
{
    MC_SAFE_REPLACE_COPY(String, mOtherName, otherName);
}

String * IMAPRenameFolderOperation::otherName()
{
    return mOtherName;
}

void IMAPRenameFolderOperation::main()
{
    ErrorCode error;
    session()->session()->renameFolder(mFolder, mOtherName, &error);
    setError(error);
}

