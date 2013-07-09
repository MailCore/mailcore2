//
//  HTMLBodyRendererTemplateCallback.cc
//  mailcore2
//
//  Created by Paul Young on 02/07/2013.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "HTMLBodyRendererTemplateCallback.h"

using namespace mailcore;

mailcore::String * HTMLBodyRendererTemplateCallback::templateForMainHeader(MessageHeader * header)
{
    return MCSTR("");
}
