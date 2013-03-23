//
//  MCOAbstractMessage.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOAbstractMessage.h"

#include "MCAbstractMessage.h"
#include "MCMessageHeader.h"
#include "MCAbstractPart.h"

#import "MCOMessageHeader.h"
#import "MCOAbstractPart.h"
#import "NSObject+MCO.h"
#import "NSString+MCO.h"

@implementation MCOAbstractMessage {
    mailcore::AbstractMessage * _message;
}

- (id) initWithPart:(mailcore::AbstractMessage *)message
{
    self = [super init];
    
    message->retain();
    _message = message;
    
    return self;
}

- (void) dealloc
{
    _message->release();
    [super dealloc];
}

- (MCOMessageHeader *) header
{
    return (MCOMessageHeader *) [NSObject mco_objectWithMCObject:((mailcore::AbstractMessage *) [self mco_mcObject])->header()];
}

- (void) setHeader:(MCOMessageHeader *)header
{
    ((mailcore::AbstractMessage *) [self mco_mcObject])->setHeader((mailcore::MessageHeader *) [header mco_mcObject]);
}

- (MCOAbstractPart *) partForContentID:(NSString *)contentID
{
    return (MCOAbstractPart *) [NSObject mco_objectWithMCObject:((mailcore::AbstractMessage *) [self mco_mcObject])->partForContentID([contentID mco_mcString])];
}

- (MCOAbstractPart *) partForUniqueID:(NSString *)uniqueID
{
    return (MCOAbstractPart *) [NSObject mco_objectWithMCObject:((mailcore::AbstractMessage *) [self mco_mcObject])->partForUniqueID([uniqueID mco_mcString])];
}

- (mailcore::Object *) mco_mcObject
{
    return _message;
}

@end
