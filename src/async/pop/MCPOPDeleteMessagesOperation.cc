//
//  MCPOPDeleteMessagesOperation.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCPOPDeleteMessagesOperation.h"

#include "MCPOPAsyncSession.h"
#include "MCPOPSession.h"

using namespace mailcore;

POPDeleteMessagesOperation::POPDeleteMessagesOperation()
{
    mMessageIndexes = NULL;
}

POPDeleteMessagesOperation::~POPDeleteMessagesOperation()
{
    MC_SAFE_RELEASE(mMessageIndexes);
}

void POPDeleteMessagesOperation::setMessageIndexes(Array * indexes)
{
    MC_SAFE_REPLACE_RETAIN(Array, mMessageIndexes, indexes);
}

Array * POPDeleteMessagesOperation::messageIndexes()
{
    return mMessageIndexes;
}

void POPDeleteMessagesOperation::main()
{
    if (mMessageIndexes == NULL)
        return;
    
    ErrorCode error;
    for(unsigned int i = 0 ; i < mMessageIndexes->count() ; i ++) {
        Value * value = (Value *) mMessageIndexes->objectAtIndex(i);
        session()->session()->deleteMessage(value->unsignedIntValue(), &error);
        if (error != ErrorNone) {
            setError(error);
            return;
        }
    }
    session()->session()->disconnect();
}

