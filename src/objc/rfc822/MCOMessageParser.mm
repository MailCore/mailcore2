//
//  MCOMessageParser.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMessageParser.h"

#include "MCRFC822.h"

#import "NSObject+MCO.h"
#import "MCOAbstractMessage+Private.h"
#import "MCOUtils.h"

@implementation MCOMessageParser {
    id <MCOMessageParserDelegate> _delegate;
}

@synthesize delegate = _delegate;

#define nativeType mailcore::MessageParser

+ (void) initialize
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (id) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::MessageParser * msg = (mailcore::MessageParser *) object;
    return [[[self alloc] initWithMCMessage:msg] autorelease];
}

+ (MCOMessageParser *) messageParserWithData:(NSData *)data
{
    return [[[MCOMessageParser alloc] initWithData:data] autorelease];
}

- (id) initWithData:(NSData *)data
{
    mailcore::MessageParser * message = new mailcore::MessageParser([data mco_mcData]);
    self = [super initWithMCMessage:message];
    message->release();
    return self;
}

- (void) dealloc
{
    [super dealloc];
}

- (MCOAbstractPart *) mainPart
{
    return MCO_OBJC_BRIDGE_GET(mainPart);
}

- (NSData *) data
{
    return MCO_OBJC_BRIDGE_GET(data);
}

- (NSString *) htmlRendering
{
#warning should implement callbacks
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->htmlRendering());
}

@end
