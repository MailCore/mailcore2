//
//  MCMessageRendererHelper.cpp
//  mailcore2
//
//  Created by Paul Young on 28/06/2013.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCMessageRendererHelper.h"

using namespace mailcore;

HTMLRendererIMAPCallback * MessageRendererHelper::dataCallback()
{
    return new HTMLRendererIMAPCallback();
}

HTMLRendererTemplateCallback * MessageRendererHelper::htmlCallback()
{
    return new HTMLRendererTemplateCallback();
}
