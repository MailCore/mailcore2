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

using namespace mailcore;

IMAPFolderStatusOperation::IMAPFolderStatusOperation()
{
    uint32_t mUidNext = 0;
    uint32_t mUidValidity = 0;
    uint32_t mTotalMessages = 0;
    uint32_t mTotalRecent = 0;
    uint32_t mTotalUnseen = 0;
    String mFolderPath = "";
}

IMAPFolderStatusOperation::~IMAPFolderStatusOperation()
{
}

String IMAPFolderStatusOperation::folderPath()
{
    return mFolderPath;
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
    
    folder_status status = session()->session()->folderStatus(folder(), &error);
    if (error != ErrorCode::ErrorNone) {
        setError(error);
        return;
    }
    
    
    mUidNext = status.uid_next;
    mUidValidity = status.uid_validity;
    mTotalMessages = status.total_messages;
    mTotalRecent = status.total_recent;
    mTotalUnseen = status.total_unseen;


    
    setError(error);
}

