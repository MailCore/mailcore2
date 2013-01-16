//
//  MCIMAPCreateFolderOperation.cpp
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPCreateFolderOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPCreateFolderOperation::IMAPCreateFolderOperation()
{
    mFolder = NULL;
}

IMAPCreateFolderOperation::~IMAPCreateFolderOperation()
{
    MC_SAFE_RELEASE(mFolder);
}

void IMAPCreateFolderOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPCreateFolderOperation::folder()
{
    return mFolder;
}

void IMAPCreateFolderOperation::main()
{
    ErrorCode error;
    session()->session()->createFolder(mFolder, &error);
    setError(error);
}
