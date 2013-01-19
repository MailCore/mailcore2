//
//  IMAPIdentityOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPIdentityOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncConnection.h"

using namespace mailcore;

IMAPIdentityOperation::IMAPIdentityOperation()
{
    mVendor = NULL;
    mName = NULL;
    mVersion = NULL;
    mServerIdentity = NULL;
}

IMAPIdentityOperation::~IMAPIdentityOperation()
{
    MC_SAFE_RELEASE(mVendor);
    MC_SAFE_RELEASE(mName);
    MC_SAFE_RELEASE(mVersion);
    MC_SAFE_RELEASE(mServerIdentity);
}

void IMAPIdentityOperation::setVendor(String * vendor)
{
    MC_SAFE_REPLACE_COPY(String, mVendor, vendor);
}

String * IMAPIdentityOperation::vendor()
{
    return mVendor;
}

void IMAPIdentityOperation::setName(String * name)
{
    MC_SAFE_REPLACE_COPY(String, mName, name);
}

String * IMAPIdentityOperation::name()
{
    return mName;
}

void IMAPIdentityOperation::setVersion(String * version)
{
    MC_SAFE_REPLACE_COPY(String, mVersion, version);
}

String * IMAPIdentityOperation::version()
{
    return mVersion;
}

HashMap * IMAPIdentityOperation::serverIdentity()
{
    return mServerIdentity;
}

void IMAPIdentityOperation::main()
{
    ErrorCode error;
    mServerIdentity = session()->session()->identity(mVendor, mName, mVersion, &error);
    MC_SAFE_RETAIN(mServerIdentity);
    setError(error);
}

