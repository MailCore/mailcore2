//
//  MCIMAPStoreFlagsOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPStoreFlagsOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPStoreFlagsOperation::IMAPStoreFlagsOperation()
{
    mFolder = NULL;
    mUids = NULL;
    mKind = IMAPStoreFlagsRequestKindAdd;
    mFlags = MessageFlagNone;
}

IMAPStoreFlagsOperation::~IMAPStoreFlagsOperation()
{
    MC_SAFE_RELEASE(mFolder);
    MC_SAFE_RELEASE(mUids);
}

void IMAPStoreFlagsOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPStoreFlagsOperation::folder()
{
    return mFolder;
}

void IMAPStoreFlagsOperation::setUids(Array * uids)
{
    MC_SAFE_REPLACE_RETAIN(Array, mUids, uids);
}

Array * IMAPStoreFlagsOperation::uids()
{
    return mUids;
}

void IMAPStoreFlagsOperation::setKind(IMAPStoreFlagsRequestKind kind)
{
    mKind = kind;
}

IMAPStoreFlagsRequestKind IMAPStoreFlagsOperation::kind()
{
    return mKind;
}

void IMAPStoreFlagsOperation::setFlags(MessageFlag flags)
{
    mFlags = flags;
}

MessageFlag IMAPStoreFlagsOperation::flags()
{
    return mFlags;
}

void IMAPStoreFlagsOperation::main()
{
    ErrorCode error;
    session()->session()->storeFlags(mFolder, mUids, mKind, mFlags, &error);
    setError(error);
}
