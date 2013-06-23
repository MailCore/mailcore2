//
//  SMTPSendWithDataOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCSMTPSendWithDataOperation.h"

#include "MCSMTPAsyncSession.h"
#include "MCSMTPSession.h"

using namespace mailcore;

SMTPSendWithDataOperation::SMTPSendWithDataOperation()
{
    mMessageData = NULL;
}

SMTPSendWithDataOperation::~SMTPSendWithDataOperation()
{
    MC_SAFE_RELEASE(mMessageData);
}

void SMTPSendWithDataOperation::setMessageData(Data * data)
{
    MC_SAFE_REPLACE_RETAIN(Data, mMessageData, data);
}

Data * SMTPSendWithDataOperation::messageData()
{
    return mMessageData;
}

void SMTPSendWithDataOperation::main()
{
    ErrorCode error;
    session()->session()->sendMessage(mMessageData, this, &error);
    setError(error);
    
    //tryAutomaticDisconnect();
}
