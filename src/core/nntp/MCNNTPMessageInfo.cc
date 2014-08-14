//
//  MCNNTPMessageInfo.cpp
//  mailcore2
//
//  Created by Robert Widmann on 3/6/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#include "MCNNTPMessageInfo.h"

using namespace mailcore;

void NNTPMessageInfo::init()
{
    mIndex = 0;
    mSize = 0;
    mUid = NULL;
}

NNTPMessageInfo::NNTPMessageInfo(NNTPMessageInfo * other)
{
    init();
    mIndex = other->mIndex;
    mSize = other->mSize;
    MC_SAFE_REPLACE_COPY(String, mUid, other->mUid);
}

NNTPMessageInfo::~NNTPMessageInfo()
{
    MC_SAFE_RELEASE(mUid);
}

String * NNTPMessageInfo::description()
{
    return String::stringWithUTF8Format("<%s:%p %u %s %u>",
                                        MCUTF8(className()), this, mIndex, MCUTF8(mUid), mSize);
}

Object * NNTPMessageInfo::copy()
{
    return new NNTPMessageInfo(this);
}

void NNTPMessageInfo::setIndex(unsigned int index)
{
    mIndex = index;
}

unsigned int NNTPMessageInfo::index()
{
    return mIndex;
}

void NNTPMessageInfo::setSize(unsigned int size)
{
    mSize = size;
}

unsigned int NNTPMessageInfo::size()
{
    return mSize;
}

void NNTPMessageInfo::setUid(String * uid)
{
    MC_SAFE_REPLACE_COPY(String, mUid, uid);
}

String * NNTPMessageInfo::uid()
{
    return mUid;
}
