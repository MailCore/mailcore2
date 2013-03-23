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

- (id) initWithPart:(mailcore::AbstractPart *)part
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

- (MCOPartType) partType
{
    return (MCOPartType) _part->partType();
}

- (void) setPartType:(MCOPartType)partType
{
    _part->setPartType((mailcore::PartType) partType);
}

- (NSString *) filename
{
    return [NSString mco_stringWithMCString:_part->filename()];
}

- (void) setFilename:(NSString *)filename
{
    _part->setFilename([filename mco_mcString]);
}

- (NSString *) mimeType
{
    return [NSString mco_stringWithMCString:_part->mimeType()];
}

- (void) setMimeType:(NSString *)mimeType
{
    _part->setMimeType([mimeType mco_mcString]);
}

- (NSString *) charset
{
    return [NSString mco_stringWithMCString:_part->charset()];
}

- (void) setCharset:(NSString *)charset
{
    _part->setCharset([charset mco_mcString]);
}

- (NSString *) uniqueID
{
    return [NSString mco_stringWithMCString:_part->uniqueID()];
}

- (void) setUniqueID:(NSString *)uniqueID
{
    _part->setUniqueID([uniqueID mco_mcString]);
}

- (NSString *) contentID
{
    return [NSString mco_stringWithMCString:_part->contentID()];
}

- (void) setContentID:(NSString *)contentID
{
    _part->setContentID([contentID mco_mcString]);
}

- (NSString *) contentLocation
{
    return [NSString mco_stringWithMCString:_part->contentLocation()];
}

- (void) setContentLocation:(NSString *)contentLocation
{
    _part->setContentLocation([contentLocation mco_mcString]);
}

- (BOOL) isInlineAttachment
{
    return _part->isInlineAttachment();
}

- (void) setInlineAttachment:(BOOL)inlineAttachment
{
    _part->setInlineAttachment(inlineAttachment);
}

- (MCOAbstractMessage *) message
{
    return (MCOAbstractMessage *) [NSObject mco_objectWithMCObject:_part->message()];
}

- (void) setMessage:(MCOAbstractMessage *)message
{
    _part->setMessage((mailcore::AbstractMessage *) [message mco_mcObject]);
}

- (MCOAbstractPart *) partForContentID:(NSString *)contentID
{
    return (MCOAbstractPart *) [NSObject mco_objectWithMCObject:_part->partForContentID([contentID mco_mcString])];
}

- (MCOAbstractPart *) partForUniqueID:(NSString *)uniqueID
{
    return (MCOAbstractPart *) [NSObject mco_objectWithMCObject:_part->partForUniqueID([uniqueID mco_mcString])];
}

- (mailcore::Object *) mco_mcObject
{
    return _part;
}

@end
