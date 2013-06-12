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
{
    mUidNext = 0;
	mUidValidity = 0;
	mMessageCount = 0;
	mRecentCount = 0;
	mUnreadCount = 0;
}

IMAPFolderStatusOperation::~IMAPFolderStatusOperation()
{}

uint32_t IMAPFolderStatusOperation::uidNext()
{
    return mUidNext;
}

uint32_t IMAPFolderStatusOperation::uidValidity()
{
    return mUidValidity;
}

uint32_t IMAPFolderStatusOperation::messageCount()
{
    return mMessageCount;
}

uint32_t IMAPFolderStatusOperation::recentCount()
{
    return mRecentCount;
}

uint32_t IMAPFolderStatusOperation::unreadCount()
{
    return mUnreadCount;
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
    mMessageCount = status->messageCount();
    mRecentCount = status->recentCount();
    mUnreadCount = status->unreadCount();
    
    setError(error);
}

