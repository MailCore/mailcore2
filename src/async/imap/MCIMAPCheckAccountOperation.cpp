//
//  MCIMAPCheckAccountOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPCheckAccountOperation.h"

#include "MCIMAPAsyncConnection.h"
#include "MCIMAPSession.h"

using namespace mailcore;

void IMAPCheckAccountOperation::main()
{
    ErrorCode error;
    session()->session()->connectIfNeeded(&error);
    if (error == ErrorNone)
        session()->session()->login(&error);
    setError(error);
}
