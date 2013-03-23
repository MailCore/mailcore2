//
//  MCOAbstractMessagePart.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOAbstractMessagePart.h"

#import "NSObject+MCO.h"
#import "MCOMessageHeader.h"

#include "MCAbstractMessagePart.h"
#include "MCMessageHeader.h"

@implementation MCOAbstractMessagePart

- (void) setHeader:(MCOMessageHeader *)header
{
    ((mailcore::AbstractMessagePart *) [self mco_mcObject])->setHeader((mailcore::MessageHeader *) [header mco_mcObject]);
}

- (MCOMessageHeader *) header
{
    return (MCOMessageHeader *) [NSObject mco_objectWithMCObject:((mailcore::AbstractMessagePart *) [self mco_mcObject])->header()];
}

- (void) setMainPart:(MCOAbstractPart *)part
{
    ((mailcore::AbstractMessagePart *) [self mco_mcObject])->setMainPart((mailcore::AbstractPart *) [part mco_mcObject]);
}

- (MCOAbstractPart *) mainPart
{
    return (MCOAbstractPart *) [NSObject mco_objectWithMCObject:((mailcore::AbstractMessagePart *) [self mco_mcObject])->mainPart()];
}

@end
