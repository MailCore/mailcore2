//
//  SMTPSendWithBuilderOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCSMTPSendWithBuilderOperation.h"

#include "MCSMTPAsyncSession.h"
#include "MCSMTPSession.h"
#include "MCMessageBuilder.h"

using namespace mailcore;

SMTPSendWithBuilderOperation::SMTPSendWithBuilderOperation()
{
    mBuilder = NULL;
}

SMTPSendWithBuilderOperation::~SMTPSendWithBuilderOperation()
{
    MC_SAFE_RELEASE(mBuilder);
}

void SMTPSendWithBuilderOperation::setBuilder(MessageBuilder * builder)
{
    MC_SAFE_REPLACE_RETAIN(MessageBuilder, mBuilder, builder);
}

MessageBuilder * SMTPSendWithBuilderOperation::builder()
{
    return mBuilder;
}

void SMTPSendWithBuilderOperation::main()
{
    ErrorCode error;
    
    session()->session()->sendMessage(mBuilder, this, &error);
    setError(error);
}
