//
//  MCOAbstractPart.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOAbstractPart.h"
#import "MCOAbstractPart+Private.h"

#include "MCAbstractPart.h"
#include "MCAbstractMessage.h"

#import "NSString+MCO.h"
#import "NSObject+MCO.h"
#import "NSData+MCO.h"

@implementation MCOAbstractPart {
    mailcore::AbstractPart * _part;
}

#define nativeType mailcore::AbstractPart

- (mailcore::Object *) mco_mcObject
{
    return _part;
}

- (id) init
{
    nativeType *part = new nativeType();
    self = [self initWithMCPart:part];
    part->release();
    return self;
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

- (NSString *) description
{
    return MCO_OBJC_BRIDGE_GET(description);
}

MCO_OBJC_SYNTHESIZE_SCALAR(MCOPartType, mailcore::PartType, setPartType, partType)

MCO_OBJC_SYNTHESIZE_STRING(setFilename, filename)
MCO_OBJC_SYNTHESIZE_STRING(setMimeType, mimeType)
MCO_OBJC_SYNTHESIZE_STRING(setCharset, charset)
MCO_OBJC_SYNTHESIZE_STRING(setUniqueID, uniqueID)
MCO_OBJC_SYNTHESIZE_STRING(setContentID, contentID)
MCO_OBJC_SYNTHESIZE_STRING(setContentLocation, contentLocation)
MCO_OBJC_SYNTHESIZE_BOOL(setInlineAttachment, isInlineAttachment)

- (MCOAbstractPart *) partForContentID:(NSString *)contentID
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->partForContentID([contentID mco_mcString]));
}

- (MCOAbstractPart *) partForUniqueID:(NSString *)uniqueID
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->partForUniqueID([uniqueID mco_mcString]));
}

- (NSString *) decodedStringForData:(NSData *)data
{
	return [NSString mco_stringWithMCString:MCO_NATIVE_INSTANCE->decodedStringForData([data mco_mcData])];
}
@end
