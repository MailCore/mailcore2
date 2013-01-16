//
//  MCIMAPDeleteFolderOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPDeleteFolderOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPDeleteFolderOperation::IMAPDeleteFolderOperation()
{
    mFolder = NULL;
}

IMAPDeleteFolderOperation::~IMAPDeleteFolderOperation()
{
    MC_SAFE_RELEASE(mFolder);
}

void IMAPDeleteFolderOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPDeleteFolderOperation::folder()
{
    return mFolder;
}

void IMAPDeleteFolderOperation::main()
{
    ErrorCode error;
    session()->session()->deleteFolder(mFolder, &error);
    setError(error);
}
