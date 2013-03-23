//
//  MessagePart.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMessagePart.h"

#include "MCRFC822.h"

#import "NSObject+MCO.h"
#import "MCOAbstractPart+Private.h"

@implementation MCOMessagePart

#define nativeType mailcore::MessagePart

+ (void) initialize
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
    mailcore::MessagePart * part = (mailcore::MessagePart *) object;
    return [[[self alloc] initWithMCPart:part] autorelease];
}

@end
