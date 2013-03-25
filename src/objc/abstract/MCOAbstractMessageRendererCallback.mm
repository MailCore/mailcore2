//
//  MCOAbstractMessageRendererCallback.cpp
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCOAbstractMessageRendererCallback.h"

#import "MCOHTMLRendererDelegate.h"
#import "MCOHTMLRendererIMAPDelegate.h"
#import "MCOUtils.h"

MCOAbstractMessageRendererCallback::MCOAbstractMessageRendererCallback(MCOAbstractMessage * message, id <MCOHTMLRendererDelegate> rendererDelegate,
                                   id <MCOHTMLRendererIMAPDelegate> rendererIMAPDelegate)
{
    mMessage = message;
    mRendererDelegate = rendererDelegate;
    mIMAPDelegate = rendererIMAPDelegate;
}

bool MCOAbstractMessageRendererCallback::canPreviewPart(mailcore::AbstractPart * part)
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage:canPreviewPart:)]) {
        return [mRendererDelegate MCOAbstractMessage:mMessage canPreviewPart:MCO_TO_OBJC(part)];
    }
    return mailcore::HTMLRendererTemplateCallback::canPreviewPart(part);
}

mailcore::HashMap * MCOAbstractMessageRendererCallback::templateValuesForHeader(mailcore::MessageHeader * header)
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage:templateValuesForHeader:)]) {
        return MCO_FROM_OBJC(mailcore::HashMap, [mRendererDelegate MCOAbstractMessage:mMessage templateValuesForHeader:MCO_TO_OBJC(header)]);
    }
    return mailcore::HTMLRendererTemplateCallback::templateValuesForHeader(header);
}

mailcore::HashMap * MCOAbstractMessageRendererCallback::templateValuesForPart(mailcore::AbstractPart * part)
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage:templateValuesForPart:)]) {
        return MCO_FROM_OBJC(mailcore::HashMap, [mRendererDelegate MCOAbstractMessage:mMessage templateValuesForPart:MCO_TO_OBJC(part)]);
    }
    return mailcore::HTMLRendererTemplateCallback::templateValuesForPart(part);
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForMainHeader()
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForMainHeader:)]) {
        return MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForMainHeader:mMessage]);
    }
    return mailcore::HTMLRendererTemplateCallback::templateForMainHeader();
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForImage()
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForImage:)]) {
        return MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForImage:mMessage]);
    }
    return mailcore::HTMLRendererTemplateCallback::templateForImage();
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForAttachment()
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForAttachment:)]) {
        return MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForAttachment:mMessage]);
    }
    return mailcore::HTMLRendererTemplateCallback::templateForAttachment();
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForMessage()
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForMessage:)]) {
        return MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForMessage:mMessage]);
    }
    return mailcore::HTMLRendererTemplateCallback::templateForMessage();
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForEmbeddedMessage()
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForEmbeddedMessage:)]) {
        return MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForEmbeddedMessage:mMessage]);
    }
    return mailcore::HTMLRendererTemplateCallback::templateForEmbeddedMessage();
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForEmbeddedMessageHeader()
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForEmbeddedMessageHeader:)]) {
        return MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForEmbeddedMessageHeader:mMessage]);
    }
    return mailcore::HTMLRendererTemplateCallback::templateForEmbeddedMessageHeader();
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForAttachmentSeparator()
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForAttachmentSeparator:)]) {
        return MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForAttachmentSeparator:mMessage]);
    }
    return mailcore::HTMLRendererTemplateCallback::templateForAttachmentSeparator();
}

mailcore::String * MCOAbstractMessageRendererCallback::filterHTMLForPart(mailcore::String * html)
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage:filterHTMLForPart:)]) {
        return MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage:mMessage filterHTMLForPart:MCO_TO_OBJC(html)]);
    }
    return mailcore::HTMLRendererTemplateCallback::filterHTMLForPart(html);
}

mailcore::String * MCOAbstractMessageRendererCallback::filterHTMLForMessage(mailcore::String * html)
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage:filterHTMLForMessage:)]) {
        return MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage:mMessage filterHTMLForMessage:MCO_TO_OBJC(html)]);
    }
    return mailcore::HTMLRendererTemplateCallback::filterHTMLForMessage(html);
}

mailcore::Data * MCOAbstractMessageRendererCallback::dataForIMAPPart(mailcore::String * folder, mailcore::IMAPPart * part)
{
    if ([mIMAPDelegate respondsToSelector:@selector(MCOAbstractMessage:dataForIMAPPart:folder:)]) {
        return [[mIMAPDelegate MCOAbstractMessage:mMessage dataForIMAPPart:MCO_TO_OBJC(part) folder:MCO_TO_OBJC(folder)] mco_mcData];
    }
    return nil;
}

void MCOAbstractMessageRendererCallback::prefetchAttachmentIMAPPart(mailcore::String * folder, mailcore::IMAPPart * part)
{
    if ([mIMAPDelegate respondsToSelector:@selector(MCOAbstractMessage:prefetchAttachmentIMAPPart:folder:)]) {
        [mIMAPDelegate MCOAbstractMessage:mMessage prefetchAttachmentIMAPPart:MCO_TO_OBJC(part) folder:MCO_TO_OBJC(folder)];
    }
}

void MCOAbstractMessageRendererCallback::prefetchImageIMAPPart(mailcore::String * folder, mailcore::IMAPPart * part)
{
    if ([mIMAPDelegate respondsToSelector:@selector(MCOAbstractMessage:prefetchImageIMAPPart:folder:)]) {
        [mIMAPDelegate MCOAbstractMessage:mMessage prefetchImageIMAPPart:MCO_TO_OBJC(part) folder:MCO_TO_OBJC(folder)];
    }
}
