//
//  MCOMultipart.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMultipart.h"

#include "MCRFC822.h"

#import "NSObject+MCO.h"
#import "MCOAbstractPart+Private.h"

@implementation MCOMultipart

#define nativeType mailcore::Multipart

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (id) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::Attachment * attachment = (mailcore::Attachment *) object;
    return [[[self alloc] initWithMCPart:attachment] autorelease];
}

- (instancetype)init {
    mailcore::Multipart * multipart = new mailcore::Multipart();
    self = [super initWithMCPart:multipart];
    MC_SAFE_RELEASE(multipart);
    return self;
}

@end
