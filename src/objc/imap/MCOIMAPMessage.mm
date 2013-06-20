//
//  MCOIMAPMessage.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPMessage.h"

#include "MCIMAP.h"

#import "MCOAbstractMessage+Private.h"
#import "MCOUtils.h"
#import "MCOAbstractMessageRendererCallback.h"
#import "MCOHTMLRendererDelegate.h"
#import "MCOHTMLRendererIMAPDelegate.h"

@implementation MCOIMAPMessage {
    mailcore::IMAPMessage * _message;
}

#define nativeType mailcore::IMAPMessage

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

- (void) encodeWithCoder:(NSCoder *)aCoder
{
	[super encodeWithCoder:aCoder];
	[aCoder encodeInt32:[self uid] forKey:@"uid"];
	[aCoder encodeInt:[self flags] forKey:@"flags"];
	[aCoder encodeInt:[self originalFlags] forKey:@"originalFlags"];
	[aCoder encodeInt64:[self modSeqValue] forKey:@"modSeqValue"];
	[aCoder encodeObject:[self mainPart] forKey:@"mainPart"];
	[aCoder encodeObject:[self gmailLabels] forKey:@"gmailLabels"];
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::IMAPMessage * msg = (mailcore::IMAPMessage *) object;
    return [[[self alloc] initWithMCMessage:msg] autorelease];
}

- (id) initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
	
	[self setUid:[aDecoder decodeInt32ForKey:@"uid"]];
	[self setFlags:(MCOMessageFlag)[aDecoder decodeIntForKey:@"flags"]];
	[self setOriginalFlags:(MCOMessageFlag)[aDecoder decodeIntForKey:@"originalFlags"]];
	[self setModSeqValue:[aDecoder decodeInt64ForKey:@"modSeqValue"]];
	[self setMainPart:[aDecoder decodeObjectForKey:@"mainPart"]];
	[self setGmailLabels:[aDecoder decodeObjectForKey:@"gmailLabels"]];

	return self;
}

MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setUid, uid)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOMessageFlag, mailcore::MessageFlag, setFlags, flags)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOMessageFlag, mailcore::MessageFlag, setOriginalFlags, originalFlags)
MCO_OBJC_SYNTHESIZE_SCALAR(uint64_t, uint64_t, setModSeqValue, modSeqValue)
MCO_OBJC_SYNTHESIZE(AbstractPart, setMainPart, mainPart)
MCO_OBJC_SYNTHESIZE_ARRAY(setGmailLabels, gmailLabels)

- (MCOAbstractPart *) partForPartID:(NSString *)partID
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->partForPartID([partID mco_mcString]));
}

- (NSString *) htmlRenderingWithFolder:(NSString *)folder
                              delegate:(id <MCOHTMLRendererIMAPDelegate>)delegate
{
    MCOAbstractMessageRendererCallback * htmlRenderCallback = new MCOAbstractMessageRendererCallback(self, delegate, delegate);
    NSString * result = MCO_TO_OBJC(MCO_NATIVE_INSTANCE->htmlRendering([folder mco_mcString], htmlRenderCallback, htmlRenderCallback));
    htmlRenderCallback->release();
    
    return result;
}

@end
