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
    mDataCallback = new HTMLRendererIMAPCallback();
    mHtmlCallback = new HTMLBodyRendererTemplateCallback();
}

MessageRendererHelper::~MessageRendererHelper()
{
    MC_SAFE_RELEASE(mDataCallback);
    MC_SAFE_RELEASE(mHtmlCallback);
}

HTMLRendererIMAPCallback * MessageRendererHelper::dataCallback()
{
    return mDataCallback;
}

HTMLBodyRendererTemplateCallback * MessageRendererHelper::htmlBodyCallback()
{
    return mHtmlCallback;
}
