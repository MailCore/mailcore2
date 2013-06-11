//
//  MCOIMAPPart.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPPart.h"

#include "MCIMAP.h"

#import "MCOAbstractPart+Private.h"
#import "MCOUtils.h"

@implementation MCOIMAPPart

#define nativeType mailcore::IMAPPart

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
	[aCoder encodeObject:[self partID] forKey:@"partID"];
	[aCoder encodeInt64:[self size] forKey:@"size"];
	[aCoder encodeInt:[self encoding] forKey:@"encoding"];
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::IMAPPart * part = (mailcore::IMAPPart *) object;
    return [[[self alloc] initWithMCPart:part] autorelease];
}

- (id) initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
	
	[self setPartID:[aDecoder decodeObjectForKey:@"partID"]];
	[self setSize:(unsigned int) [aDecoder decodeInt64ForKey:@"size"]];
	[self setEncoding:(MCOEncoding) [aDecoder decodeIntForKey:@"encoding"]];

	return self;
}

MCO_OBJC_SYNTHESIZE_STRING(setPartID, partID)
MCO_OBJC_SYNTHESIZE_SCALAR(unsigned int, unsigned int, setSize, size)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOEncoding, mailcore::Encoding, setEncoding, encoding)

- (unsigned int) decodedSize
{
    return MCO_NATIVE_INSTANCE->decodedSize();
}

@end
