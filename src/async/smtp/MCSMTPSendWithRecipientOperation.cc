//
//  SMTPSendWithRecipientOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCSMTPSendWithRecipientOperation.h"

#include "MCSMTPAsyncSession.h"
#include "MCSMTPSession.h"

using namespace mailcore;

void SMTPSendWithRecipientOperation::init()
{
    mRecipients = NULL;
    mFrom = NULL;
    mMessageData = NULL;
}

SMTPSendWithRecipientOperation::SMTPSendWithRecipientOperation()
{
    init();
}

SMTPSendWithRecipientOperation::~SMTPSendWithRecipientOperation()
{
    MC_SAFE_RELEASE(mRecipients);
    MC_SAFE_RELEASE(mFrom);
    MC_SAFE_RELEASE(mMessageData);
}

void SMTPSendWithRecipientOperation::setFrom(Address * from)
{
    MC_SAFE_REPLACE_RETAIN(Address, mFrom, from);
}

Address * SMTPSendWithRecipientOperation::from()
{
    return mFrom;
}

void SMTPSendWithRecipientOperation::setRecipients(Array * recipients)
{
    MC_SAFE_REPLACE_RETAIN(Array, mRecipients, recipients);
}

Array * SMTPSendWithRecipientOperation::recipients()
{
    return mRecipients;
}

void SMTPSendWithRecipientOperation::setMessageData(Data * data)
{
    MC_SAFE_REPLACE_RETAIN(Data, mMessageData, data);
}

Data * SMTPSendWithRecipientOperation::messageData()
{
    return mMessageData;
}

void SMTPSendWithRecipientOperation::main()
{
    ErrorCode error;
    
    session()->session()->sendMessage(mFrom, mRecipients, mMessageData, this, &error);
    setError(error);
}
