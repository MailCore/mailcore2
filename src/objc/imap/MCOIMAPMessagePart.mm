//
//  MCOIMAPMessagePart.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPMessagePart.h"

#include "MCIMAP.h"

#import "MCOAbstractPart+Private.h"
#import "MCOUtils.h"

@implementation MCOIMAPMessagePart

#define nativeType mailcore::IMAPMessagePart

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
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::IMAPMessagePart * part = (mailcore::IMAPMessagePart *) object;
    return [[[self alloc] initWithMCPart:part] autorelease];
}

- (id) initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
	
	[self setPartID:[aDecoder decodeObjectForKey:@"partID"]];
	
	return self;
}

MCO_OBJC_SYNTHESIZE_STRING(setPartID, partID)

@end
