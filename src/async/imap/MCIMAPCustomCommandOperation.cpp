//
//  MCIMAPCustomCommandOperation.cpp
//  mailcore2
//
//  Created by Libor Huspenina on 18/10/2015.
//  Copyright © 2015 MailCore. All rights reserved.
//

#include "MCIMAPCustomCommandOperation.h"

#include "MCIMAPSession.h"
#include "MCIMAPAsyncConnection.h"

using namespace mailcore;

IMAPCustomCommandOperation::IMAPCustomCommandOperation()
{
    mCustomCommand = NULL;
}

IMAPCustomCommandOperation::~IMAPCustomCommandOperation()
{
    MC_SAFE_RELEASE(mCustomCommand);
}

void IMAPCustomCommandOperation::setCustomCommand(mailcore::String * command)
{
    mCustomCommand = command;
}

void IMAPCustomCommandOperation::main()
{

    session()->session()->sendCustomCommand(mCustomCommand);

}
