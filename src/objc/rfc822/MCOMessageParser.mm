//
//  MCOMessageParser.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMessageParser.h"

#include "MCRFC822.h"
#include "MCRenderer.h"

#import "MCOHTMLRendererDelegate.h"
#import "NSObject+MCO.h"
#import "MCOUtils.h"
#import "MCOAbstractMessageRendererCallback.h"

@interface MCOMessageParser ()
- (id) initWithMCMessage:(mailcore::AbstractMessage *)message NS_DESIGNATED_INITIALIZER;
@end

@implementation MCOMessageParser

#define nativeType mailcore::MessageParser

+ (void) load
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
    mailcore::MessageParser * message = new mailcore::MessageParser((CFDataRef) data);
    self = [super initWithMCMessage:message];
    MC_SAFE_RELEASE(message);
    return self;
}

- (id) initWithMCMessage:(mailcore::AbstractMessage *)message
{
    return [super initWithMCMessage:message];
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

- (NSString *) htmlRenderingWithDelegate:(id <MCOHTMLRendererDelegate>)delegate
{
    MCOAbstractMessageRendererCallback * htmlRenderCallback = new MCOAbstractMessageRendererCallback(self, delegate, NULL);
    NSString * result = MCO_TO_OBJC(MCO_NATIVE_INSTANCE->htmlRendering(htmlRenderCallback));
    htmlRenderCallback->release();
    
    return result;
}

- (NSString *) htmlBodyRendering
{
    return MCO_OBJC_BRIDGE_GET(htmlBodyRendering);
}

- (NSString *) plainTextRendering
{
    return MCO_OBJC_BRIDGE_GET(plainTextRendering);
}

- (NSString *) plainTextBodyRendering
{
    return [self plainTextBodyRenderingAndStripWhitespace:YES];
}

- (NSString *) plainTextBodyRenderingAndStripWhitespace:(BOOL)stripWhitespace
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->plainTextBodyRendering(stripWhitespace));
}

@end
