//
//  MCHTMLBodyRendererTemplateCallback.cc
//  mailcore2
//
//  Created by Micah Rosales on 08/01/2013.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCHTMLBareRendererTemplateCallback.h"

using namespace mailcore;

mailcore::String * HTMLBareRendererTemplateCallback::templateForMainHeader(MessageHeader * header)
{
    return MCSTR("");
}


mailcore::String * HTMLBareRendererTemplateCallback::templateForAttachment(AbstractPart * part)
{
    return MCSTR("");
}

mailcore::String * HTMLBareRendererTemplateCallback::templateForMessage(AbstractMessage * message)
{
    return MCSTR("<div>{{BODY}}</div>");
}

mailcore::String * HTMLBareRendererTemplateCallback::filterHTMLForMessage(mailcore::String * html)
{
    return html;
}

mailcore::String * HTMLBareRendererTemplateCallback::filterHTMLForPart(mailcore::String * html)
{
    return html;
}