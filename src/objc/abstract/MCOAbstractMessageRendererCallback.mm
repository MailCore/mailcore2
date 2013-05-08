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

bool MCOAbstractMessageRendererCallback::shouldShowPart(mailcore::AbstractPart * part)
{
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage:shouldShowPart:)]) {
        return [mRendererDelegate MCOAbstractMessage:mMessage shouldShowPart:MCO_TO_OBJC(part)];
    }
    return mailcore::HTMLRendererTemplateCallback::shouldShowPart(part);
}

mailcore::HashMap * MCOAbstractMessageRendererCallback::templateValuesForHeader(mailcore::MessageHeader * header)
{
    mailcore::HashMap * result = NULL;
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage:templateValuesForHeader:)]) {
        result = MCO_FROM_OBJC(mailcore::HashMap, [mRendererDelegate MCOAbstractMessage:mMessage templateValuesForHeader:MCO_TO_OBJC(header)]);
    }
    if (result == NULL) {
        result = mailcore::HTMLRendererTemplateCallback::templateValuesForHeader(header);
    }
    return result;
}

mailcore::HashMap * MCOAbstractMessageRendererCallback::templateValuesForPart(mailcore::AbstractPart * part)
{
    mailcore::HashMap * result = NULL;
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage:templateValuesForPart:)]) {
        result = MCO_FROM_OBJC(mailcore::HashMap, [mRendererDelegate MCOAbstractMessage:mMessage templateValuesForPart:MCO_TO_OBJC(part)]);
    }
    if (result == NULL) {
        result = mailcore::HTMLRendererTemplateCallback::templateValuesForPart(part);
    }
    return result;
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForMainHeader()
{
    mailcore::String * result = NULL;
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForMainHeader:)]) {
        result = MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForMainHeader:mMessage]);
    }
    if (result == NULL) {
        result = mailcore::HTMLRendererTemplateCallback::templateForMainHeader();
    }
    return result;
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForImage()
{
    mailcore::String * result = NULL;
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForImage:)]) {
        result = MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForImage:mMessage]);
    }
    if (result == NULL) {
        result = mailcore::HTMLRendererTemplateCallback::templateForImage();
    }
    return result;
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForAttachment()
{
    mailcore::String * result = NULL;
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForAttachment:)]) {
        result = MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForAttachment:mMessage]);
    }
    if (result == NULL) {
        result = mailcore::HTMLRendererTemplateCallback::templateForAttachment();
    }
    return result;
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForMessage()
{
    mailcore::String * result = NULL;
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForMessage:)]) {
        result = MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForMessage:mMessage]);
    }
    if (result == NULL) {
        result = mailcore::HTMLRendererTemplateCallback::templateForMessage();
    }
    return result;
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForEmbeddedMessage()
{
    mailcore::String * result = NULL;
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForEmbeddedMessage:)]) {
        result = MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForEmbeddedMessage:mMessage]);
    }
    if (result == NULL) {
        result = mailcore::HTMLRendererTemplateCallback::templateForEmbeddedMessage();
    }
    return result;
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForEmbeddedMessageHeader()
{
    mailcore::String * result = NULL;
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForEmbeddedMessageHeader:)]) {
        result = MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForEmbeddedMessageHeader:mMessage]);
    }
    if (result == NULL) {
        result = mailcore::HTMLRendererTemplateCallback::templateForEmbeddedMessageHeader();
    }
    return result;
}

mailcore::String * MCOAbstractMessageRendererCallback::templateForAttachmentSeparator()
{
    mailcore::String * result = NULL;
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage_templateForAttachmentSeparator:)]) {
        result = MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage_templateForAttachmentSeparator:mMessage]);
    }
    if (result == NULL) {
        result = mailcore::HTMLRendererTemplateCallback::templateForAttachmentSeparator();
    }
    return result;
}

mailcore::String * MCOAbstractMessageRendererCallback::filterHTMLForPart(mailcore::String * html)
{
    mailcore::String * result = NULL;
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage:filterHTMLForPart:)]) {
        result = MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage:mMessage filterHTMLForPart:MCO_TO_OBJC(html)]);
    }
    if (result == NULL) {
        result = mailcore::HTMLRendererTemplateCallback::filterHTMLForPart(html);
    }
    return result;
}

mailcore::String * MCOAbstractMessageRendererCallback::filterHTMLForMessage(mailcore::String * html)
{
    mailcore::String * result = NULL;
    if ([mRendererDelegate respondsToSelector:@selector(MCOAbstractMessage:filterHTMLForMessage:)]) {
        result = MCO_FROM_OBJC(mailcore::String, [mRendererDelegate MCOAbstractMessage:mMessage filterHTMLForMessage:MCO_TO_OBJC(html)]);
    }
    if (result == NULL) {
        result = mailcore::HTMLRendererTemplateCallback::filterHTMLForMessage(html);
    }
    return result;
}

mailcore::Data * MCOAbstractMessageRendererCallback::dataForIMAPPart(mailcore::String * folder, mailcore::IMAPPart * part)
{
    mailcore::Data * result = NULL;
    if ([mIMAPDelegate respondsToSelector:@selector(MCOAbstractMessage:dataForIMAPPart:folder:)]) {
        result = [[mIMAPDelegate MCOAbstractMessage:mMessage dataForIMAPPart:MCO_TO_OBJC(part) folder:MCO_TO_OBJC(folder)] mco_mcData];
    }
    return result;
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
