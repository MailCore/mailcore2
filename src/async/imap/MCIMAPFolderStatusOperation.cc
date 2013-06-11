//
//  MCIMAPFolderStatusOperation.cc
//  mailcore2
//
//  Created by Sebastian on 6/5/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPFolderStatusOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncConnection.h"
#include "MCIMAPFolderStatus.h"

using namespace mailcore;

IMAPFolderStatusOperation::IMAPFolderStatusOperation()
{}

IMAPFolderStatusOperation::~IMAPFolderStatusOperation()
{
	mUidNext = 0;
	mUidValidity = 0;
	mTotalMessages = 0;
	mTotalRecent = 0;
	mTotalUnseen = 0;
}

uint32_t IMAPFolderStatusOperation::uidNext()
{
    return mUidNext;
}

uint32_t IMAPFolderStatusOperation::uidValidity()
{
    return mUidValidity;
}

uint32_t IMAPFolderStatusOperation::totalMessages()
{
    return mTotalMessages;
}

uint32_t IMAPFolderStatusOperation::totalRecent()
{
    return mTotalRecent;
}

uint32_t IMAPFolderStatusOperation::totalUnseen()
{
    return mTotalUnseen;
}

void IMAPFolderStatusOperation::main()
{
    ErrorCode error;
    
    session()->session()->loginIfNeeded(&error);
    if (error != ErrorCode::ErrorNone) {
        setError(error);
        return;
    }
    
    IMAPFolderStatus *status = session()->session()->folderStatus(folder(), &error);
    if (error != ErrorCode::ErrorNone) {
        setError(error);
        return;
    }
    
    
    mUidNext = status->uidNext();
    mUidValidity = status->uidValidity();
    mTotalMessages = status->totalMessage();
    mTotalRecent = status->totalRecent();
    mTotalUnseen = status->totalUnseen();
    
    setError(error);
}

