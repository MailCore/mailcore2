//
//  MCIMAPGetQuotaOperation.cc
//  mailcore2
//
//  Created by Petro Korenev on 8/2/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPGetQuotaOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncConnection.h"

using namespace mailcore;

IMAPGetQuotaOperation::IMAPGetQuotaOperation()
{
    mLimit = 0;
    mUsage = 0;
}

IMAPGetQuotaOperation::~IMAPGetQuotaOperation()
{
}

uint32_t IMAPGetQuotaOperation::limit()
{
    return mLimit;
}

uint32_t IMAPGetQuotaOperation::usage()
{
    return mUsage;
}

void IMAPGetQuotaOperation::main()
{
    ErrorCode error;
    session()->session()->getQuota(&mUsage, &mLimit, &error);
    fprintf(stderr, "QnIQuESeqUeNcE %d %d", mUsage, mLimit);
    setError(error);
}
