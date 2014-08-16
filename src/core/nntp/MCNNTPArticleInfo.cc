//
//  MCNNTPArticleInfo.cpp
//  mailcore2
//
//  Created by Robert Widmann on 3/6/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#include "MCNNTPArticleInfo.h"

using namespace mailcore;


void NNTPArticleInfo::init()
{
    mIndex = 0;
    mSize = 0;
    mUid = NULL;
}

NNTPArticleInfo::NNTPArticleInfo()
{
    init();
}

NNTPArticleInfo::NNTPArticleInfo(NNTPArticleInfo * other)
{
    init();
    mIndex = other->mIndex;
    mSize = other->mSize;
    MC_SAFE_REPLACE_COPY(String, mUid, other->mUid);
}

NNTPArticleInfo::~NNTPArticleInfo()
{
    MC_SAFE_RELEASE(mUid);
}

String * NNTPArticleInfo::description()
{
    return String::stringWithUTF8Format("<%s:%p %u %s %u>",
                                        MCUTF8(className()), this, mIndex, MCUTF8(mUid), mSize);
}

Object * NNTPArticleInfo::copy()
{
    return new NNTPArticleInfo(this);
}

void NNTPArticleInfo::setIndex(unsigned int index)
{
    mIndex = index;
}

unsigned int NNTPArticleInfo::index()
{
    return mIndex;
}

void NNTPArticleInfo::setSize(unsigned int size)
{
    mSize = size;
}

unsigned int NNTPArticleInfo::size()
{
    return mSize;
}

void NNTPArticleInfo::setUid(String * uid)
{
    MC_SAFE_REPLACE_COPY(String, mUid, uid);
}

String * NNTPArticleInfo::uid()
{
    return mUid;
}
