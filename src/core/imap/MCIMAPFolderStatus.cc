//
//  MCIMAPFolderStatus.cc
//  mailcore2
//
//  Created by Sebastian on 6/11/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPFolderStatus.h"

using namespace mailcore;

void IMAPFolderStatus::init()
{
    mUnseenCount = 0;
    mMessageCount = 0;
    mRecentCount = 0;
    mUidNext = 0;
    mUidValidity = 0;
}

IMAPFolderStatus::IMAPFolderStatus()
{
    init();
}


IMAPFolderStatus::IMAPFolderStatus(IMAPFolderStatus * other)
{
    init();
    setUnseenCount(other->unseenCount());
    setMessageCount(other->messageCount());
    setRecentCount(other->recentCount());
    setUidNext(other->uidNext());
    setUidValidity(other->uidValidity());    
}

IMAPFolderStatus::~IMAPFolderStatus()
{ }

Object * IMAPFolderStatus::copy()
{
    return new IMAPFolderStatus(this);
}

void IMAPFolderStatus::setUnseenCount(u_int32_t unseen)
{
    mUnseenCount = unseen;
}

u_int32_t IMAPFolderStatus::unseenCount()
{
    return mUnseenCount;
}

void IMAPFolderStatus::setMessageCount(u_int32_t messages)
{
    mMessageCount = messages;
}

u_int32_t IMAPFolderStatus::messageCount()
{
    return mMessageCount;
}

void IMAPFolderStatus::setRecentCount(u_int32_t recent)
{
    mRecentCount = recent;
}

u_int32_t IMAPFolderStatus::recentCount()
{
    return mRecentCount;
}

void IMAPFolderStatus::setUidNext(u_int32_t uidNext)
{
    mUidNext = uidNext;
}

u_int32_t IMAPFolderStatus::uidNext()
{
    return mUidNext;
}

void IMAPFolderStatus::setUidValidity(u_int32_t uidValidity)
{
    mUidValidity = uidValidity;
}

u_int32_t IMAPFolderStatus::uidValidity()
{
    return mUidValidity;
}
