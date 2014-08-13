//
//  MCNNTPGroupInfo.cpp
//  mailcore2
//
//  Created by Robert Widmann on 3/6/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#include "MCNNTPGroupInfo.h"

using namespace mailcore;

void NNTPGroupInfo::init()
{
    mMessageCount = 0;
}

NNTPGroupInfo::NNTPGroupInfo()
{
    init();
}


NNTPGroupInfo::NNTPGroupInfo(NNTPGroupInfo * other)
{
    init();
    setMessageCount(other->messageCount());
    setName(other->name());
}

NNTPGroupInfo::~NNTPGroupInfo()
{
}

Object * NNTPGroupInfo::copy()
{
    return new NNTPGroupInfo(this);
}
