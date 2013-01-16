//
//  MCIMAPStoreLabelsOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPStoreLabelsOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncSession.h"

using namespace mailcore;

IMAPStoreLabelsOperation::IMAPStoreLabelsOperation()
{
    mFolder = NULL;
    mUids = NULL;
    mKind = IMAPStoreFlagsRequestKindAdd;
    mLabels = NULL;
}

IMAPStoreLabelsOperation::~IMAPStoreLabelsOperation()
{
    MC_SAFE_RELEASE(mFolder);
    MC_SAFE_RELEASE(mUids);
    MC_SAFE_RELEASE(mLabels);
}

void IMAPStoreLabelsOperation::setFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mFolder, folder);
}

String * IMAPStoreLabelsOperation::folder()
{
    return mFolder;
}

void IMAPStoreLabelsOperation::setUids(Array * uids)
{
    MC_SAFE_REPLACE_RETAIN(Array, mUids, uids);
}

Array * IMAPStoreLabelsOperation::uids()
{
    return mUids;
}

void IMAPStoreLabelsOperation::setKind(IMAPStoreFlagsRequestKind kind)
{
    mKind = kind;
}

IMAPStoreFlagsRequestKind IMAPStoreLabelsOperation::kind()
{
    return mKind;
}

void IMAPStoreLabelsOperation::setLabels(Array * labels)
{
    MC_SAFE_REPLACE_RETAIN(Array, mLabels, labels);
}

Array * IMAPStoreLabelsOperation::labels()
{
    return mLabels;
}

void IMAPStoreLabelsOperation::main()
{
    ErrorCode error;
    session()->session()->storeLabels(mFolder, mUids, mKind, mLabels, &error);
    setError(error);
}

