//
//  MCIMAPExpungeOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPExpungeOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPExpungeOperation::IMAPExpungeOperation()
{
    mFolder = NULL;
}

IMAPExpungeOperation::~IMAPExpungeOperation()
{
    MC_SAFE_RELEASE(mFolder);
}

void IMAPExpungeOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPExpungeOperation::folder()
{
    return mFolder;
}

void IMAPExpungeOperation::main()
{
    ErrorCode error;
    session()->session()->expunge(mFolder, &error);
    setError(error);
}
