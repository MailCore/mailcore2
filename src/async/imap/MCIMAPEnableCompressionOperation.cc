//
//  MCIMAPEnableCompressionOperation.cpp
//  mailcore2
//
//  Created by Matt Ronge on 7/17/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPEnableCompressionOperation.h"

#include "MCIMAPAsyncConnection.h"
#include "MCIMAPSession.h"

using namespace mailcore;

void IMAPEnableCompressionOperation::main()
{
    ErrorCode error;
    session()->session()->enableCompression(&error);
    setError(error);
}
