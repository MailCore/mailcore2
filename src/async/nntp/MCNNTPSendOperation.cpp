//
//  MCNNTPSendOperation.cpp
//  mailcore2
//
//  Created by Daryle Walker on 2/21/16.
//  Copyright © 2016 MailCore. All rights reserved.
//

#include "MCNNTPSendOperation.h"

#include "MCNNTPAsyncSession.h"
#include "MCNNTPSession.h"

using namespace mailcore;

NNTPSendOperation::NNTPSendOperation()
{
    mMessageData = NULL;
    mMessageFilepath = NULL;
}

NNTPSendOperation::~NNTPSendOperation()
{
    MC_SAFE_RELEASE(mMessageFilepath);
    MC_SAFE_RELEASE(mMessageData);
}

void NNTPSendOperation::setMessageData(Data * data)
{
    MC_SAFE_REPLACE_RETAIN(Data, mMessageData, data);
}

Data * NNTPSendOperation::messageData()
{
    return mMessageData;
}

void NNTPSendOperation::setMessageFilepath(String * path)
{
    MC_SAFE_REPLACE_RETAIN(String, mMessageFilepath, path);
}

String * NNTPSendOperation::messageFilepath()
{
    return mMessageFilepath;
}

void NNTPSendOperation::main()
{
    ErrorCode error;
    if (mMessageFilepath != NULL) {
        session()->session()->sendMessage(mMessageFilepath, this, &error);
    }
    else {
        session()->session()->sendMessage(mMessageData, this, &error);
    }
    setError(error);
}
