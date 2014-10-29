//
//  MCSMTPLoginOperation.cc
//  mailcore2
//
//  Created by Robert Widmann on 9/24/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCSMTPLoginOperation.h"

#include "MCSMTPAsyncSession.h"
#include "MCSMTPSession.h"

using namespace mailcore;

SMTPLoginOperation::SMTPLoginOperation()
{
}

SMTPLoginOperation::~SMTPLoginOperation()
{
}

void SMTPLoginOperation::main()
{
    ErrorCode error;
    session()->session()->loginIfNeeded(&error);
    setError(error);
}
