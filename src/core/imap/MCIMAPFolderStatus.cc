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
    mTotalUnseen = 0;
    mTotalMessages = 0;
    mTotalRecent = 0;
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
    setTotalUnseen(other->totalUnseen());
    setTotalMessages(other->totalMessage());
    setTotalRecent(other->totalRecent());
    setUidNext(other->uidNext());
    setUidValidity(other->uidValidity());    
}

IMAPFolderStatus::~IMAPFolderStatus()
{ }

Object * IMAPFolderStatus::copy()
{
    return new IMAPFolderStatus(this);
}

void IMAPFolderStatus::setTotalUnseen(u_int32_t unseen)
{
    mTotalUnseen = unseen;
}

u_int32_t IMAPFolderStatus::totalUnseen()
{
    return mTotalUnseen;
}

void IMAPFolderStatus::setTotalMessages(u_int32_t messages)
{
    mTotalMessages = messages;
}

u_int32_t IMAPFolderStatus::totalMessage()
{
    return mTotalMessages;
}

void IMAPFolderStatus::setTotalRecent(u_int32_t recent)
{
    mTotalRecent = recent;
}

u_int32_t IMAPFolderStatus::totalRecent()
{
    return mTotalRecent;
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
