//
//  MCOMessageBuilder.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMessageBuilder.h"

#include "MCRFC822.h"

#import "MCOUtils.h"
#import "MCOAbstractMessage+Private.h"

@implementation MCOMessageBuilder

#define nativeType mailcore::MessageBuilder

+ (void) initialize
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (id) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::MessageBuilder * msg = (mailcore::MessageBuilder *) object;
    return [[[self alloc] initWithMCMessage:msg] autorelease];
}

MCO_OBJC_SYNTHESIZE_STRING(setHTMLBody, htmlBody)
MCO_OBJC_SYNTHESIZE_STRING(setTextBody, textBody)
MCO_OBJC_SYNTHESIZE_ARRAY(setAttachments, attachments)
MCO_OBJC_SYNTHESIZE_ARRAY(setRelatedAttachments, relatedAttachments)
MCO_OBJC_SYNTHESIZE_STRING(setBoundaryPrefix, boundaryPrefix)

- (void) addAttachment:(MCOAttachment *)attachment
{
    MCO_NATIVE_INSTANCE->addAttachment(MCO_FROM_OBJC(mailcore::Attachment, attachment));
}

- (void) addRelatedAttachment:(MCOAttachment *)attachment
{
    MCO_NATIVE_INSTANCE->addRelatedAttachment(MCO_FROM_OBJC(mailcore::Attachment, attachment));
}

- (NSData *) data
{
    return MCO_OBJC_BRIDGE_GET(data);
}

@end
