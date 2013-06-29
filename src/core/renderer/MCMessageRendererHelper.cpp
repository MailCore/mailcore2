//
//  MCMessageRendererHelper.cpp
//  mailcore2
//
//  Created by Paul Young on 28/06/2013.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCMessageRendererHelper.h"

using namespace mailcore;

MessageRendererHelper::MessageRendererHelper()
{
    mSession = NULL;
}

MessageRendererHelper::~MessageRendererHelper()
{
    MC_SAFE_RELEASE(mSession);
}

void MessageRendererHelper::setSession(IMAPSession * session)
{
    mSession = session;
}

IMAPSession MessageRendererHelper::session()
{
    return mSession;
}

HTMLRendererIMAPCallback * MessageRendererHelper::dataCallback()
{

}

HTMLRendererTemplateCallback * MessageRendererHelper::htmlCallback()
{
    
}
