//
//  MCOAbstractPart.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOAbstractPart.h"

#include "MCAbstractPart.h"
#include "MCAbstractMessage.h"

#import "NSString+MCO.h"
#import "NSObject+MCO.h"

@implementation MCOAbstractPart {
    mailcore::AbstractPart * _part;
}

#define nativeType mailcore::AbstractPart

- (mailcore::Object *) mco_mcObject
{
    return _part;
}

- (id) initWithMCPart:(mailcore::AbstractPart *)part
{
    self = [super init];
    
    part->retain();
    _part = part;
    
    return self;
}

- (void) dealloc
{
    _part->release();
    [super dealloc];
}

MCO_OBJC_SYNTHESIZE_SCALAR(MCOPartType, mailcore::PartType, setPartType, partType)

MCO_OBJC_SYNTHESIZE_STRING(setFilename, filename)
MCO_OBJC_SYNTHESIZE_STRING(setMimeType, mimeType)
MCO_OBJC_SYNTHESIZE_STRING(setCharset, charset)
MCO_OBJC_SYNTHESIZE_STRING(setUniqueID, uniqueID)
MCO_OBJC_SYNTHESIZE_STRING(setContentID, contentID)
MCO_OBJC_SYNTHESIZE_STRING(setContentLocation, contentLocation)
MCO_OBJC_SYNTHESIZE_BOOL(setInlineAttachment, isInlineAttachment)
MCO_OBJC_SYNTHESIZE(AbstractMessage, setMessage, message)

- (MCOAbstractPart *) partForContentID:(NSString *)contentID
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->partForContentID([contentID mco_mcString]));
}

- (MCOAbstractPart *) partForUniqueID:(NSString *)uniqueID
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->partForUniqueID([uniqueID mco_mcString]));
}

@end
