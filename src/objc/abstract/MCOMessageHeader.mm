//
//  MCOMessageHeader.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMessageHeader.h"
#import "MCOMessageHeader+Private.h"

#include <typeinfo>

#include "MCMessageHeader.h"
#include "NSString+MCO.h"
#include "NSArray+MCO.h"
#include "NSObject+MCO.h"
#include "NSData+MCO.h"
#include "MCOAddress.h"
#include "MCOAddress+Private.h"
#include "MCAddress.h"

@implementation MCOMessageHeader {
    mailcore::MessageHeader * _nativeHeader;
}

#define nativeType mailcore::MessageHeader

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

- (id) copyWithZone:(NSZone *)zone
{
    nativeType * nativeObject = (nativeType *) [self mco_mcObject]->copy();
    id result = [[self class] mco_objectWithMCObject:nativeObject];
    MC_SAFE_RELEASE(nativeObject);
    return [result retain];
}

+ (id) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::MessageHeader * header = (mailcore::MessageHeader *) object;
    return [[[self alloc] initWithMCMessageHeader:header] autorelease];
}

- (mailcore::Object *) mco_mcObject
{
    return _nativeHeader;
}

- (id) init
{
    self = [super init];
    
    _nativeHeader = new mailcore::MessageHeader();
    
    return self;
}

- (id) initWithMCMessageHeader:(mailcore::MessageHeader *)header
{
    self = [super init];
    
    _nativeHeader = header;
    _nativeHeader->retain();
    
    return self;
}

+ (MCOMessageHeader *) messageHeaderWithMCMessageHeader:(mailcore::MessageHeader *)header
{
    if (header == NULL)
        return nil;
    
    return [[[self alloc] initWithMCMessageHeader:header] autorelease];
}

- (NSString *) description
{
    return MCO_OBJC_BRIDGE_GET(description);
}

MCO_OBJC_SYNTHESIZE_STRING(setMessageID, messageID)
MCO_OBJC_SYNTHESIZE_ARRAY(setReferences, references)
MCO_OBJC_SYNTHESIZE_ARRAY(setInReplyTo, inReplyTo)
MCO_OBJC_SYNTHESIZE_DATE(setDate, date)
MCO_OBJC_SYNTHESIZE_DATE(setReceivedDate, receivedDate)
MCO_OBJC_SYNTHESIZE(Address, setSender, sender)
MCO_OBJC_SYNTHESIZE(Address, setFrom, from)
MCO_OBJC_SYNTHESIZE_ARRAY(setTo, to)
MCO_OBJC_SYNTHESIZE_ARRAY(setCc, cc)
MCO_OBJC_SYNTHESIZE_ARRAY(setBcc, bcc)
MCO_OBJC_SYNTHESIZE_ARRAY(setReplyTo, replyTo)
MCO_OBJC_SYNTHESIZE_STRING(setSubject, subject)
MCO_OBJC_SYNTHESIZE_STRING(setUserAgent, userAgent)

- (NSString *) extractedSubject
{
    return MCO_OBJC_BRIDGE_GET(extractedSubject);
}

- (NSString *) partialExtractedSubject
{
    return MCO_OBJC_BRIDGE_GET(partialExtractedSubject);
}

- (void) importHeadersData:(NSData *)data
{
    _nativeHeader->importHeadersData([data mco_mcData]);
}

@end
